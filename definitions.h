#include <Arduino.h>

#define LCD_WIDTH 16
#define LCD_HEIGHT 2
#define DEBUG

// LED
namespace led {
  const byte pin = 10;
  int state = LOW;
  volatile int blink = 0; // boolean if led should be blinked in incTimer
  unsigned long startTime; // blinkStartTime
}

// Button
namespace button {
  const byte pin = 11;
}

// Timer
namespace timer {
  // The timer represents time in 0.5 sec multiples, mins, hrs and sec are the formatted variant for display
  // use volatile for shared variables between interrupt and code
  volatile unsigned long t = 0;
  unsigned long hrs = 0;
  unsigned long mins = 0;
  unsigned int secs = 0;
}

namespace message {
  int part = 0; // Message part currently being displayed
  int len[][2] = {{2, 3}, {5, 3}}; //3 messages for BREAKIN, 6 messages for TAKEBREAK
}

int globalState = 0; //0 = BREAKIN, 1 = TAKEBREAK
int subState = 0; //Breakin: 0 = working, 1 = should take break; Takebreak: 0 = break, 1 = should work
// The subState & globalState dictates which message will be shown.


// Program strings
const char S_MSG[2][2][6][LCD_WIDTH+1] = 
                               {{ // Break in… (globalState=0)
                                 {"Travaille produ-", // subState=0
                                  "-ctivement pend-",
                                  "-ant 52 minutes!"},
                                 {"Tu deverais pre-", // subState=1
                                  "-endre une pause",
                                  "Appuie sur le   ",
                                  "bouton.         "}
                                                     },
                                { // Take a break… (globalState=0)
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
