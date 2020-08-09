#include <FTDebouncer.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>

#include "definitions.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
FTDebouncer pinDebouncer;

void setup() {
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  Serial.begin(9600);

  pinDebouncer.addPin(button, LOW);
  pinDebouncer.begin();
  
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH); // Turn off led
  
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
  timerCopy = timer;
  interrupts();

  Serial.print("timer = ");
  Serial.print(timerCopy);
  Serial.print(" message = ");
  Serial.print(message);
  Serial.print(" subState = ");
  Serial.print(subState);
  Serial.print(" globalState = ");
  Serial.print(globalState);
  Serial.print("Triggered (A|B)");
  Serial.print(triggeredA);
  Serial.println(triggeredB);

  calculateTimeUnits(timerCopy);
  displayMessage(timerCopy);
  displayTimeUnits(timerCopy);

  if (globalState == 0) { // Work time
    if (timer > 120*2 && !triggeredA) {
      // Break time after 120 sec
      blinkLed = 1;
      subState = 1;
      message = 0;
      triggeredA = 1;
    }
  
  } else if (globalState == 1) { // Break time
    if (timer > /*80*/25*2 && !triggeredB) {
      // Back to work after 25 sec
      blinkLed = 1; 
      subState = 1;
      message = 0;
      triggeredB = 1;
    }
  }

  if (timer > 20*2) {
    // Stop blinking led 20 sec after globalState switch.
    blinkLed = 0;
    digitalWrite(led, HIGH); // Turn off led
  }
}

// Used by FTDebouncer, when buttons get activated
void onPinActivated(int pin){
  Serial.println("Button pressed down");
  if (pin == button) {
    globalState = !globalState; subState = 0;
    triggeredA = 0; triggeredB = 0;

    noInterrupts();
    timer = 0; // Reset timer when switching between globalStates
    message = 0;
    interrupts();
  }
}
void onPinDeactivated(int pin){
}
