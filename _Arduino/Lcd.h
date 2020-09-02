#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20,4);

void init_lcd()
{
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(6,1);
  lcd.print("Welcome!");
  lcd.setCursor(5,3);
  lcd.print("RoboDog RC");
}

void start_lcd()
{
//  lcd.clear();
//  lcd.setCursor(0,3);
//  lcd.print("Pose");
}

void loop_lcd()
{
  //lcd.clear();
}
