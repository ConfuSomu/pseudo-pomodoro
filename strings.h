namespace message {
  byte part = 0; // Message part currently being displayed
  byte len[][4] = {{3, 3, 4, 7}, {5, 3, 6}}; // Length of each message, ex. 0 for a message of length 1.
}

// Program strings
/* Hex values are character codes for special characters, as defined in the HD44780U datasheet.
 * Note that the international/european version (ROM code: A02) is required for these characters to display properly. The more common, japanese version (ROM code: A00), will display greek characters instead of the following.
 * E0=11100000=à
 * EA=11101010=ê
 * E9=11101001=é
 * E8=11101000=è
 */
const char S_MSG[2][4][11][LCD_WIDTH+1] PROGMEM =
                               {{ // Break in… (globalState=0)
                                 {"Make sure to    ", // subState=0
                                  "work productive-",
                                  "ly for 52 mins! ",
                                  "Don't give up!  "},
                                 {"You should now  ", // subState=1
                                  "take a break!   ",
                                  "Press on the    ",
                                  "button.         "},
                                 {"It's nearly bre-", // subState=2
                                  "ak time! Focus  ",
                                  "during these    ",
                                  "last minutes.   ",
                                  "You can do this!"},
                                 {"For your health ", // subState=3
                                  "and well being, ",
                                  "it is important ",
                                  "to take a break.",
                                  "Take yours, and ",
                                  "track your time,",
                                  "by pressing on  ",
                                  "the button.     "}
                                                     },
                                { // Take a break… (globalState=1)
                                 {"Now is your time", // subState=0
                                  "to relax. Once  ",
                                  "you are done, or",
                                  "the light blinks",
                                  "press on the    ",
                                  "button.         "},
                                 {"You should head ", // subState=1
                                  "back to work. Go",
                                  "on, press on the",
                                  "button.         "},
                                 {"Taking a break  ", // subState=2
                                  "for too long    ",
                                  "will make you   ",
                                  "distracted. It  ",
                                  "won't be easy to",
                                  "be productive   ",
                                  "later on...     "},
                                                     }};
