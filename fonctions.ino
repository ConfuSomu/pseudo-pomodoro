void incTime() {
  timer++;
  if (!(timer % 20)) message++; // Increment messageId each 10 secs
  
  if (blinkLed && timer % 2) {
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(led, ledState);
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
    hrs = 0;
    mins = 0;
    secs = 0;
  } else {
    // Flooring, in this case truncation, is done by type casting,
    // as hrs & mins are longs and secs is an int.
    hrs = (timer/2) / 3600;
    mins = ((timer/2) - (hrs*3600)) / 60;
    secs = (timer/2) % 60;
  }
}

void displayMessage(unsigned long timer) {
  if (message > messageLen[globalState][subState]) message = 0;
  
  lcd.setCursor(0,0);
  lcd.print(S_MSG[globalState][subState][message]);
  #ifdef DEBUG
  Serial.println(S_MSG[globalState][subState][message]);
  #endif
}

void displayTimeUnits(unsigned long timer) {
//  char output[LCD_WIDTH+1];

  // Note that sprintf's return value is the string lenght! Use for error checking.
  // sprintf does not want to co-op with me…
//  sprintf(output, "(%03d) %02d:%02d:%02d", (timer/2), hrs, mins, secs);
//  Serial.print(output);
  
  lcd.setCursor(0,1);
//  lcd.print(output);
  displayZero(hrs);
  lcd.print(hrs);
  lcd.print(":");
  displayZero(mins);
  lcd.print(mins);
  lcd.print(":");
  displayZero(secs);
  lcd.print(secs);
  lcd.print(" (");
  lcd.print(timer/2);
  lcd.print(")   ");
}
