#include <Arduino.h>

const int led = 10;  // the pin with a LED
const int button = 11;

#define LCD_WIDTH 16
#define LCD_HEIGHT 2
#define DEBUG

// Program strings
const char S_MSG[2][2][6][LCD_WIDTH+1] = 
                               {{ // Break in…
                                 {"Travaille produ-", // subState=0
                                  "-ctivement pend-",
                                  "-ant 52 minutes!"},
                                 {"Tu deverais pre-", // subState=1
                                  "-endre une pause",
                                  "Appuie sur le   ",
                                  "bouton.         "}
                                                     },
                                { // Take a break…
                                 {"Prends ta pause ", // subState=0
                                  "de 17 minutes   ",
                                  "(appuie sur le  ",
                                  "bouton pour rep-",
                                  "-rendre ton     ",
                                  "travail)        "},
                                 {"Faudrait contin-", // subState=1
                                  "uer a travailler",
                                  "Aller! appuie   ",
                                  "sur ce bouton.  "}
                                                     }};

// The timer represents time in 0.5 sec multiples, mins, hrs and sec are the formatted variant for display
// use volatile for shared variables between interrupt and code
int ledState = LOW;
volatile int blinkLed = 0; //boolean if led should be blinked in incTimer

volatile unsigned long timer = 0;
unsigned long blinkStartTime;
unsigned long hrs = 0;
unsigned long mins = 0;
unsigned int secs = 0;
int message = 0; //Message part currently displayed

int globalState = 0; //0 = BREAKIN, 1 = TAKEBREAK
int subState = 0; //Breakin: 0 = working, 1 = should take break; Takebreak: 0 = break, 1 = should work
int messageLen[][2] = {{2, 3}, {5, 3}}; //3 messages for BREAKIN, 6 messages for TAKEBREAK
#define SS_SHOULDSWITCH 1 /*Message id used to indicate that you should switch between work & break*/
