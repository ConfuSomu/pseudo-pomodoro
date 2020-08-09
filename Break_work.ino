#include <FTDebouncer.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>

#include "definitions.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
FTDebouncer pinDebouncer;

void setup() {
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  
  #ifdef DEBUG
  Serial.begin(9600);
  #endif

  pinDebouncer.addPin(button::pin, LOW);
  pinDebouncer.begin();
  
  pinMode(led::pin, OUTPUT);
  
  Timer1.initialize(500000);
  Timer1.attachInterrupt(incTime); // incTime to run every 0.5 seconds
}

void loop() {
  pinDebouncer.update();
  
  unsigned long timerCopy;  // holds a copy of the timer
  //unsigned long blinkLedCopy;  // holds a copy of blinkLed
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

  if (globalState == 0) { // Work time
    if (timerCopy > 120*2 && subState != 1) {
      // Break time after 120 sec
      led::blink = 1;
      led::startTime = timerCopy;
      message::part = 0;
      subState = 1;
    }
  
  } else if (globalState == 1) { // Break time
    if (timerCopy > 25*2 && subState != 1) {
      // Back to work after 25 sec
      led::blink = 1;
      led::startTime = timerCopy;
      message::part = 0;
      subState = 1;
    }
  }

  if (led::blink && timerCopy > led::startTime+20*2) {
    // Stop blinking led 20 sec after globalState switch.
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
