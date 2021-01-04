byte bell[] = {
  B00000,
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00100,
  B00000
};

byte warn[] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B00100,
  B00000,
  B01110,
  B01110
};

void registerChars(LiquidCrystal lcd) {
  lcd.createChar(0, bell);
  lcd.createChar(1, warn);
}
