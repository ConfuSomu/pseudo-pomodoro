namespace message {
  byte part = 0; // Message part currently being displayed
  byte len[][4] = {{2, 3, 4, 7}, {5, 3, 4}}; // Length of each message, ex. 0 for a message of length 1.
}

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
