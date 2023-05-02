#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the I2C address of the LCD display
#define LCD_ADDRESS 0X27

// Define the I2C address of the RTC module
#define RTC_ADDRESS 0x68

// Define the pin number for the button
#define BUTTON_ENTER 15

// Define the initial date and time that the RTC module will be set
// to when the system is restarted with the enter button pressed
#define START_SECOND 00
#define START_MINUTE 56
#define START_HOUR 21
#define START_DAY 01
#define START_DAY_OF_WEEK 01
#define START_MONTH 05
#define START_YEAR 23

// Initialize the LCD display object
LiquidCrystal_I2C lcd( LCD_ADDRESS, 20, 4);

void setup() {
  // Open serial communications and wait for port to open
  Serial.begin(9600);
  while(!Serial){
    ;
  } // wait for serial port to connect

  // Initialize the I2C communication
  Wire.begin();

  // Initialize the LCD display
  lcd.init();
  lcd.backlight();

  // Set the button pin as a input
  pinMode(BUTTON_ENTER, INPUT);

  // Check if the enter button is pressed
  // and update the date and time in the RTC module 
  if( digitalRead(BUTTON_ENTER) == HIGH){
    updateDateTime();
  }
}

void loop() {
  // Show the currente date and time every second
  showDateTime();
  delay(1000);
}

// Write the start date and time to the RTC module
void updateDateTime() {
  // Start an I2C transmission to the RTC module
  Wire.beginTransmission(RTC_ADDRESS);

  // Set the register pointer to 0 to start writing from the first register
  Wire.write(0);
  
  // Write de values of the date and time
  Wire.write(decToBcd(START_SECOND));
  Wire.write(decToBcd(START_MINUTE));
  Wire.write(decToBcd(START_HOUR));
  Wire.write(decToBcd(START_DAY));
  Wire.write(decToBcd(START_DAY_OF_WEEK));
  Wire.write(decToBcd(START_MONTH));
  Wire.write(decToBcd(START_YEAR));

  // End the I2C transmission
  Wire.endTransmission();
}

void showDateTime(){
  int second, minute, hour, day, dayOfWeek, month, year;
  String dateString;
  String timeString;

  // Request the date and time from the RCT module
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0); // Start at register 0 (second)
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDRESS, 7); // Request 7 bytes (seconds, minutes, day, date, month, year)

  // Read the date and time from RTC module
  second = bcdToDec(Wire.read() & 0x7F);
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0x3F);
  day = bcdToDec(Wire.read());
  dayOfWeek = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());

  // Format the date and time as a string
  dateString = dateToString(day, dayOfWeek, month, year);
  timeString = timeToString(hour, minute, second);

  // Write the formatted date to the LCD display
  lcd.setCursor(0, 0);
  lcd.print(dateString);

  // Write the formatted time to the LCD display
  lcd.setCursor(0, 1);
  lcd.print(timeString);

  // Print the formatted date and time to the serial monitor
  Serial.print(dateString);
  Serial.print(" ");
  Serial.println(timeString);
}

String dateToString(int day, int week, int month, int year){
  String dayOfWeekString;
  String dateString;

  dayOfWeekString = dayOfWeekToString(week);
  
  // Format the date as a string
  dateString = String(day < 10 ? "0" : "") + String(day, DEC) + "/" 
                          + String(month < 10 ? "0" : "") + String(month, DEC) + "/" 
                          + String(year < 10 ? "0" : "") + String(year, DEC)
                          + dayOfWeekString;

  return dateString;
}

String timeToString(int hour, int minute, int second){
  String timeString;
 
  // Format the time as a string
  timeString = String(hour < 10 ? "0" : "") + String(hour, DEC) + ":" 
                          + String(minute < 10 ? "0" : "") + String(minute, DEC) + ":" 
                          + String(second < 10 ? "0" : "") + String(second, DEC);

  return timeString;
}

String dayOfWeekToString(int week){
  String dayOfWeekString;

  switch(week) {
    case 0:
      dayOfWeekString = " Sun";
      break;
    case 1:
      dayOfWeekString = " Mon";
      break;
    case 2:
      dayOfWeekString = " Tue";
      break;
    case 3:
      dayOfWeekString = " Wed";
      break;
    case 4:
      dayOfWeekString = " Thur";
      break;
    case 5:
      dayOfWeekString = " Fri";
      break;
    case 6:
      dayOfWeekString = " Sat";
  }

  return dayOfWeekString;
}

// Function to convert binary-coded decimal (BCD) to decimal
int bcdToDec(int bcd){
  return ((bcd / 16) * 10) + (bcd % 16);
}

// Function to convert decimal to binary-coded decimal (BCD)
int decToBcd(int val) {
  return ((val / 10) << 4) + (val % 10);
}