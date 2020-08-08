#include <Arduino.h>

// Program strings
const char S_MSG[][6][17] = {{"Travaille produ-", // BREAKIN
                               "-ctivement pend-",
                               "-ant 52 minutes!"},
                              {"Prends ta pause ", // TAKEBREAK
                               "de 17 minutes   ",
                               "(appuie sur le  ",
                               "bouton pour rep-",
                               "-rendre ton     ",
                               "travail)        "}};

const int led = 10;  // the pin with a LED
const int button = 11;

#define LCD_WIDTH 16
#define LCD_HEIGHT 2

// The timer represents time in 0.5 sec multiples, mins, hrs and sec are the formatted variant for display
// use volatile for shared variables between interrupt and code
int ledState = LOW;
volatile int blinkLed = 0; //boolean if led should be blinked in incTimer

volatile unsigned long timer = 0;
unsigned long hrs = 0;
unsigned long mins = 0;
unsigned int secs = 0;

int globalState = 0; //0 = BREAKIN, 1 = TAKEBREAK
int stateMessages[] = {2,5}; //3 messages for BREAKIN, 6 messages for TAKEBREAK
int message = 0; //Message id displayed
