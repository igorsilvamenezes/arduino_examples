#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the I2C address of the LCD display
#define LCD_ADDRESS 0X27

// Initialize the LCD display object
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

// Initialize a counter variable
int counter = 0;

void setup() {
  // Initialize the LCD display
  lcd.init();
  lcd.backlight();

  // Write the initial text to the display
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");
  lcd.setCursor(0, 1);
  lcd.print("Counter: ");
  lcd.print(counter);
}

void loop() {
  // Increment the counter variable
  counter++;

  // Write the counter value to the LCD display
  lcd.setCursor(9, 1);
  lcd.print("         ");
  lcd.setCursor(9, 1);
  lcd.print(counter);

  // Wait for one second before looping again
  delay(1000);
}