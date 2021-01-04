#include <Arduino.h>

#define LCD_WIDTH 16
#define LCD_HEIGHT 2
#define DEBUG /*Enable debug features*/

#define TIMERFREQ 500000 /*Timer run frequency, set at 0.5s. Should not be changed!*/
#define MSG_UPDATEFREQ 6 /*Speed of scrolling text = 3 sec. In 0.5s (timer) multiples.*/

#define BREAKIN_TIME (52*60*2) /*Time before user is asked to take a break = 52 min. In 0.5s multiples.*/
#define MUSTBREAK_TIME (80*60*2) /*Time user must take break = 80 min. In 0.5s multiples.*/
#define NEARLYBREAK_TIME (43*60*2) /*Time before Nearly break is displayed = 43 min. In 0.5s multiples.*/
#define BACKTOWORK_TIME (17*60*2) /*Time before user is asked to work = 17 min. In 0.5s multiples.*/
#define MUSTWORK_TIME (22*60*2) /*Time user must work = 22 min. In 0.5s multiples.*/

#define BLINKDURATION (20*2) /*LED blink duration = 20 sec. In 0.5s multiples.*/

void changeStates(byte new_subState, unsigned long timer = 0, byte do_blinkLed = 0);

// LED
namespace led {
  const byte pin = 10;
  byte state = LOW;
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
  byte part = 0; // Message part currently being displayed
  byte len[][4] = {{2, 3, 4, 7}, {5, 3, 4}}; // Length of each message, ex. 0 for a message of length 1.
}

byte globalState = 0; //0 = BREAKIN, 1 = TAKEBREAK
byte subState = 0; //Breakin: 0 = working, 1 = should take break; Takebreak: 0 = break, 1 = should work
// The subState & globalState dictates which message will be shown.


// Program strings
/* Hex values are character codes for special characters, as defined in the HD44780U datasheet.
 * Note that the international/european version (ROM code: A02) is required for these characters to display properly. The more common, japanese version, will display greek characters instead of the following.
 * E0=11100000=à
 * EA=11101010=ê
 * E9=11101001=é
 * E8=11101000=è
 */
const char S_MSG[2][4][11][LCD_WIDTH+1] PROGMEM =
                               {{ // Break in… (globalState=0)
                                 {"Travaille produ-", // subState=0
                                  "-ctivement pend-",
                                  "-ant 52 minutes!"},
                                 {"Tu devrais pre- ", // subState=1
                                  "-endre une pause",
                                  "Appuies sur le  ",
                                  "bouton.         "},
                                 {"Tu y es  presque", // subState=2
                                  "Concentre-toi   ",
                                  "bien pour ces 9 ",
                                  "derni\xE8res  minu-",
                                  "-tes. Allez!    "},
                                 {"Il est extr\xEAmem-", // subState=3
                                  "-ent important  ",
                                  "que tu prennes  ",
                                  "une pause pour  ",
                                  "ton bien-\xEAtre.  ",
                                  "Prends la pause ",
                                  "en appuyant sur ",
                                  "le bouton.      "}
                                                     },
                                { // Take a break… (globalState=1)
                                 {"Prends ta pause ", // subState=0
                                  "de 17 minutes   ",
                                  "(appuies sur le ",
                                  "bouton pour rep-",
                                  "-rendre ton     ",
                                  "travail)        "},
                                 {"Faudrait contin-", // subState=1
                                  "uer \xE0 travailler",
                                  "Allez! appuyez  ",
                                  "sur ce bouton.  "},
                                 {"N'arr\xEAte pas de ", // subState=2
                                  "travailler pend-",
                                  "-ant trop longte",
                                  "mps! Tu ne seras",
                                  "plus concentr/xE9. "},
                                                     }};
