#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the I2C addresses of the LCD display
#define LCD_ADDRESS 0x27

// Define the I2C addresses of the RTC module
#define RTC_ADDRESS 0x68

// Initialize the LCD display object
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

void setup() {
  // Initialize the I2C communication
  Wire.begin();
  
  // Initialize the serial communication
  Serial.begin(9600);
  
  // Initialize the LCD display
  lcd.init();
  lcd.backlight();

  // Print input pattern for date and time
  Serial.println("Please enter the date and time in the following format:");
  Serial.println("dd/MM/yy hh:mm:ss w");
  Serial.println("dd = day, MM = month, yy = year, hh = hour, mm = minute, ss = second, w = day of week");
  Serial.println("For example: 01/04/23 12:34:56 1");
}

void loop() {
  // show the date and time to the display
  showDateTime();

  // Verify and read serial data
  readSerialInput();
}

void showDateTime() {
  String dayOfWeekString;
  String dateString;
  String timeString;

  int second;
  int minute;
  int hour;
  int week;
  int day;
  int month;
  int year;

  // Request the date and time from the RTC module
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0); // Start at register 0 (seconds)
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDRESS, 7); // Request 7 bytes (seconds, minutes, hours, day, date, month, year)
  
  // Read the date and time from the RTC module
  second = bcdToDec(Wire.read() & 0x7F);
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0x3F);
  week = bcdToDec(Wire.read());
  day = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());

  dateString = dateToString(day, week, month, year);

  timeString = timeToString(hour, minute, second);
  
  // Write the formatted date and time to the LCD display
  lcd.setCursor(0, 0);
  lcd.print(dateString);

  lcd.setCursor(0, 1);
  lcd.print(timeString);
}

void readSerialInput(){
  String dateString;
  String timeString;
  
  int second;
  int minute;
  int hour;
  int week;
  int day;
  int month;
  int year;

    // Read date and time from Serial input
  if (Serial.available() > 0) {
    day = Serial.parseInt();
    month = Serial.parseInt();
    year = Serial.parseInt();
    hour = Serial.parseInt();
    minute = Serial.parseInt();
    second = Serial.parseInt();
    week = Serial.parseInt();

    dateString = dateToString(day, week, month, year);

    timeString = timeToString(hour, minute, second);

    Serial.read(); // Discard newline character
    Serial.print("Updating date and time to: ");
    Serial.print(dateString);
    Serial.print(" ");
    Serial.print(timeString);

    setDateTime(day, month, year, hour, minute, second, week);
  }
}

void setDateTime(int day, int month, int year, int hour, int minute, int second, int dow) {
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0); // Start from register 0
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(dow));
  Wire.write(decToBcd(day));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();

  lcd.clear();
}

// Function to convert decimal to binary-coded decimal (BCD)
int decToBcd(int val) {
  return ((val / 10) << 4) + (val % 10);
}

// Function to convert binary-coded decimal (BCD) to decimal
int bcdToDec(int val) {
  return ((val >> 4) * 10) + (val & 0x0F);
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