#include <Arduino.h>

#define LCD_WIDTH 16
#define LCD_HEIGHT 2
#define DEBUG /*Enable debug features*/

#define TIMERFREQ 500000 /*Timer run frequency, set at 0.5s. Should not be changed!*/
#define MSG_UPDATEFREQ 5 /*Speed of scrolling text = 2.5 sec. In 0.5s (timer) multiples.*/

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

byte globalState = 0; //0 = BREAKIN, 1 = TAKEBREAK
byte subState = 0; // 0: continue, 1: should, 2: must, 3: additional message
// The subState & globalState dictates which message will be shown.
