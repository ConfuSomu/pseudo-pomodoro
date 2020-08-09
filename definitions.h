#include <Arduino.h>

#define LCD_WIDTH 16
#define LCD_HEIGHT 2
#define DEBUG /*Enable debug features*/

#define TIMERFREQ 500000 /*Timer run frequency, set at 0.5s. Should not be changed!*/
#define MSG_UPDATEFREQ 20 /*Speed of scrolling text. In 0.5s (timer) multiples.*/
#define BREAKIN_TIME (120*2) /*Time before user is asked to take a break. In 0.5s multiples.*/
#define MUSTBREAK_TIME (140*2) /*Time user must take break. In 0.5s multiples.*/
#define NEARLYBREAK_TIME (60*2) /*Time before Nearly break is displayed. In 0.5s multiples.*/
#define BACKTOWORK_TIME (25*2) /*Time before user is asked to work. In 0.5s multiples.*/
#define BLINKDURATION (20*2) /*LED blink duration. In 0.5s multiples.*/

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
  int len[][4] = {{2, 3, 4, 7}, {5, 3}}; // Length of each message, ex. 0 for a message of length 1.
}

int globalState = 0; //0 = BREAKIN, 1 = TAKEBREAK
int subState = 0; //Breakin: 0 = working, 1 = should take break; Takebreak: 0 = break, 1 = should work
// The subState & globalState dictates which message will be shown.


// Program strings
const char S_MSG[2][4][11][LCD_WIDTH+1] =
                               {{ // Break in… (globalState=0)
                                 {"Travaille produ-", // subState=0
                                  "-ctivement pend-",
                                  "-ant 52 minutes!"},
                                 {"Tu deverais pre-", // subState=1
                                  "-endre une pause",
                                  "Appuie sur le   ",
                                  "bouton.         "},
                                 {"Tu y est presque", // subState=2
                                  "Concentre-toi   ",
                                  "bien pour ces 20",
                                  "dernieres  minu-",
                                  "-tes. Allez!    "},
                                 {"Il est extremem-", // subState=3
                                  "-ent important  ",
                                  "que tu prennes  ",
                                  "une pause pour  ",
                                  "ton bien-etre.  ",
                                  "Prends la pause ",
                                  "en appuyant sur ",
                                  "le bouton.      "}
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
