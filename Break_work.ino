#include <FTDebouncer.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>

#include "lcd-progmem.h"
#include "definitions.h"
#include "chars.h"
#include "strings.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
FTDebouncer pinDebouncer;

void setup() {
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  registerChars(lcd);
  lcd.print("\xA2pseudoPomodoro\xA3"); // "「peusdoPomodoro」" → requires character ROM code A00. See strings.h
  delay(1000);

  pinDebouncer.addPin(button::pin, LOW);
  pinDebouncer.begin();
  
  pinMode(led::pin, OUTPUT);
  
  #ifdef DEBUG
  Serial.begin(9600);
  digitalWrite(led::pin, HIGH);
  delay(200);
  digitalWrite(led::pin, LOW);
  #endif
  
  Timer1.initialize(TIMERFREQ);
  Timer1.attachInterrupt(incTime); // incTime to run every 0.5 seconds
}

void loop() {
  pinDebouncer.update();
  
  unsigned long timerCopy;  // holds a copy of the timer
  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  noInterrupts();
  timerCopy = timer::t;
  interrupts();

  #ifdef DEBUG
  Serial.print("timer = ");
  Serial.print(timerCopy);
  Serial.print("blinkStartTime = ");
  Serial.print(led::startTime);
  Serial.print(" part = ");
  Serial.print(message::part);
  Serial.print(" subState = ");
  Serial.print(subState);
  Serial.print(" globalState = ");
  Serial.println(globalState);
  #endif

  calculateTimeUnits(timerCopy);
  displayMessage(timerCopy);
  displayTimeUnits(timerCopy);
  displayStatusIcons();

  if (globalState == 0) { // Work time
    if (timerCopy > NEARLYBREAK_TIME && timerCopy < BREAKIN_TIME && subState != 3) {
      // Close to break time (additional message)
      changeStates(3);
    } else if (timerCopy > BREAKIN_TIME && timerCopy < MUSTBREAK_TIME && subState != 1) {
      // User should take a break (should)
      changeStates(1, timerCopy, 1);
    } else if (timerCopy > MUSTBREAK_TIME && subState != 2) {
      // User must take a break (must)
      changeStates(2, timerCopy, 1);
    }
  } else if (globalState == 1) { // Break time
    if (timerCopy > BACKTOWORK_TIME && timerCopy < MUSTWORK_TIME && subState != 1) {
      // User should work (should)
      changeStates(1, timerCopy, 1);
    } else if (timerCopy > MUSTWORK_TIME && subState != 2) {
      // User must work (must)
      changeStates(2, timerCopy, 1);
    }
  }

  if (led::blink && timerCopy > led::startTime+BLINKDURATION) {
    // Stop blinking led after globalState switch.
    led::blink = 0;
    digitalWrite(led::pin, LOW);
  }
}

// Used by FTDebouncer, when buttons get activated
void onPinActivated(int pin){
  Serial.println("Button pressed down");
  if (pin == button::pin) {
    globalState = !globalState; // Invert globalState, effectively switching between the two modes
    subState = 0;
    led::blink = 0;

    noInterrupts();
    timer::t = 0; // Reset timer when switching between globalStates
    message::part = 0;
    interrupts();
  }
}
void onPinDeactivated(int pin){
}
