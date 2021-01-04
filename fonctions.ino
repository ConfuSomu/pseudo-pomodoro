void incTime() {
  timer::t++;
  if (!(timer::t % MSG_UPDATEFREQ)) message::part++; // Increment messageId at a speed defined by MSG_UPDATEFREQ
  
  if (led::blink && timer::t % 2) {
    led::state = ! led::state;
    digitalWrite(led::pin, led::state);
  }
}

// Pad the number to two digits
char zeroPad(unsigned long n) {
  if (n < 10) return '0';
  else return '\0';
}

void displayZero(unsigned long n) {
  char zero = zeroPad(n);
  if (zero != '\0') lcd.print(zero);
}

void calculateTimeUnits(unsigned long timer) {
  if (timer/2 <= 0) {
    timer::hrs = 0;
    timer::mins = 0;
    timer::secs = 0;
  } else {
    // Flooring, in this case truncation, is done by type casting,
    // as hrs & mins are longs and secs is an int.
    timer::hrs = (timer/2) / 3600;
    timer::mins = ((timer/2) - (timer::hrs*3600)) / 60;
    timer::secs = (timer/2) % 60;
  }
}

void displayMessage(unsigned long timer) {
  if (message::part > message::len[globalState][subState]) message::part = 0;
  
  lcd.setCursor(0,0);
  print_progmem(lcd, S_MSG[globalState][subState][message::part]);
  #ifdef DEBUG
  //Serial.println(S_MSG[globalState][subState][message::part]);
  #endif
}

void displayTimeUnits(unsigned long timer) {
  lcd.setCursor(4,1); // Second line, fifth position for centering
  displayZero(timer::hrs);
  lcd.print(timer::hrs);
  lcd.print(":");
  displayZero(timer::mins);
  lcd.print(timer::mins);
  lcd.print(":");
  displayZero(timer::secs);
  lcd.print(timer::secs);
}

void changeStates(byte new_subState, unsigned long timer, byte do_blinkLed) {
  message::part = 0;
  subState = new_subState;

  // Check for led blinking
  if (do_blinkLed) {
    led::blink = do_blinkLed; // Allow for non bool parameters, overkill for this project
    led::startTime = timer;
  }
}
