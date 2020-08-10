#include <avr/pgmspace.h>
#define PSTR2(s) (__extension__({static unsigned char __c[] PROGMEM = (s); &__c[0];}))

// Print to LCD from PROGMEM.
// This function allows printing strings from the program memory.
// Source: https://forum.arduino.cc/index.php?topic=83021.msg623517#msg623517
// add. info.: https://forum.arduino.cc/index.php?topic=87902.msg675646#msg675646
// Usage example:
// lcd_print_progmem(lcd, PSTR2("Hello World!");
// Arguments:
// - lcd: The LiquidCrystal object representing the LCD to print to.
// - data: The string to print. Please note that the must be enclosed in the macro PSTR2().
// Returns: void
void print_progmem(LiquidCrystal lcd, const char /*prog_uchar*/ *data){
  char lcdBuf[16] = {0};
  byte c, i = 0;
  c =(char)pgm_read_byte(data);
  while(c != 0 && i < 16){
     lcdBuf[i++] = c;
     c =(char)(pgm_read_byte(++data));
  }
  lcd.print(lcdBuf);
}
