#include <TimerOne.h>
#include <LiquidCrystal.h>
#include "definitions.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH); // turn off led
  Timer1.initialize(500000);
  Timer1.attachInterrupt(incTime); // incTime to run every 0.5 seconds
  Serial.begin(9600);
}

// The main program will print the blink count
// to the Arduino Serial Monitor
void loop() {
  unsigned long timerCopy;  // holds a copy of the timer
  //unsigned long blinkLedCopy;  // holds a copy of blinkLed
  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  noInterrupts();
  timerCopy = timer;
  //blinkLedCopy = blinkLed;
  interrupts();

  Serial.print("timer = ");
  Serial.print(timerCopy);
  Serial.print(" message = ");
  Serial.print(message);
  Serial.print(" globalState = ");
  Serial.println(globalState);

  calculateTimeUnits(timerCopy);
  displayMessage(timerCopy);
  displayTimeUnits(timerCopy);

  if (timer > 120*2) { // 120 sec till break
    blinkLed = 1;
    globalState = 1;
  } else {
    blinkLed = 0;
    globalState = 0;
  }

  delay(100);
}
