pseudoPomodoro
-------

This is (kinda) an implementation of Pomodoro, but following work/break intervals of 52/17 minutes. It uses an LCD display to show the time passed working/having a break, status icons and also messages to continue working: why not.

The intervals can be ajusted by changing the BREAKIN_TIME and BACKTOWORK_TIME (and others) defines. Messages can be changed in strings.h: make sure to ajust the lenght of the messages changed in message::len.

The Arduino's TIMER1 is used via the (TimerOne)[https://github.com/PaulStoffregen/TimerOne] library. This allows a higher precision than using delay(). The (FTDebouncer)[https://github.com/ubidefeo/FTDebouncer] library allows reading the button without dealing with bouncing. Both libraries are installable from the Arduino's IDE library manager.

