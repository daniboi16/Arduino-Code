#include <Wire.h>                  //library for I2C communications
#include <LiquidCrystal_I2C.h>     //library for LCD communications

LiquidCrystal_I2C lcd(0x27,20,4);  // initialise the I2C address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();                 // turn on backlight
  lcd.setCursor(0,0);              // set cursor to (0,0)
  lcd.print("CAR");                // to display "CAR"
}

void loop()
{}
