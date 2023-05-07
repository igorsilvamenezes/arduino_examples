#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the I2C address of the LCD display
#define LCD_ADDRESS 0X27

// Define the I2C address of the RTC module
#define RTC_ADDRESS 0x68

// Define the pins numbers for the buttons
#define BUTTON_RETURN 14
#define BUTTON_ENTER 15

// Define the pin number for the LED
#define LED_PIN 9

// Pin number for the buzzer
#define BUZZER_PIN 6

// Define the pin number for the interrupt
#define INTERRUPT_PIN 2

// Define the frequency of Notes
#define NOTE_ALARM 1500
#define NOTE_OK 1760

// Define the initial alarm that the RTC module will be set
#define START_ALARM1_HOUR 9
#define START_ALARM1_MINUTE 0
#define START_ALARM1_STATUS true
#define START_ALARM2_HOUR 9
#define START_ALARM2_MINUTE 15
#define START_ALARM2_STATUS true

// Initialize the LCD display object
LiquidCrystal_I2C lcd( LCD_ADDRESS, 20, 4);

// Flag to keep track of alarm is playing
volatile bool alarmPlaying = false;

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

  // Set LED pin as OUTPUT
  pinMode(LED_PIN, OUTPUT);
  
  // Set the buttons pins as a input
  pinMode(BUTTON_RETURN, INPUT);
  pinMode(BUTTON_ENTER, INPUT);

  // Set up an interrupt to trigger the callback function turnOnAlarm on falling edge
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), turnOnAlarm, FALLING);

  // Check if the enter button is pressed
  // and update the time alarms in the RTC module
  if( digitalRead(BUTTON_ENTER) == HIGH ) {
    updateAlarms();
  }
}

void loop() {
  // Show the currente date and time every second
  showDateTime();
  showAlarms();

  if( alarmPlaying ){

    // Check if the button reset is pressed
    if( digitalRead(BUTTON_RETURN) ){
      // Stop playing the alarm
      alarmPlaying = false;

      // play two beep for confirmation
      beep(NOTE_OK);
      beep(NOTE_OK);

    } else {
      // Blink the LED
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);

      // Play the alarm beep
      beep(NOTE_ALARM);
      delay(100);
    }

  } else {
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  }
}

void showDateTime(){
  int second, minute, hour, day, month, year;
  String dateString;
  String timeString;

  // Request the date and time from the RCT module
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0); // Start at register 0 (second)
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDRESS, 7); // Request 7 bytes (seconds, minutes, day, date, month, year)

  // Read the date and time from RTC module
  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read());
  day = bcdToDec(Wire.read());
  Wire.read();
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());

  // Format the date and time as a string
  dateString = dateToString(day, month, year);
  timeString = timeToString(hour, minute, second);

  // Write the formatted datetime to the LCD display
  lcd.setCursor(0, 0);
  lcd.print(dateString + " " + timeString);

  // Print the formatted datetime to the serial monitor
  Serial.print(dateString + " " + timeString);
}

String dateToString(int day, int month, int year){
  String dateString;
  
  // Format the date as a string
  dateString = String(day < 10 ? "0" : "") + String(day, DEC) + "/" 
                          + String(month < 10 ? "0" : "") + String(month, DEC) + "/" 
                          + String(year < 10 ? "0" : "") + String(year, DEC);

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

// Function to convert binary-coded decimal (BCD) to decimal
int bcdToDec(int bcd){
  return ((bcd / 16) * 10) + (bcd % 16);
}

// Function to convert decimal to binary-coded decimal (BCD)
int decToBcd(int val) {
  return ((val / 10) << 4) + (val % 10);
}

void updateAlarms(){
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x07);  
  Wire.write(0);                              // Second value for alarm1
  Wire.write(decToBcd(START_ALARM1_MINUTE));  // Minute value for alarm1
  Wire.write(decToBcd(START_ALARM1_HOUR));    // Hour value for alarm1
  Wire.write(0x80);                           // Alarm1 mask, set to match hour and minute
  Wire.write(decToBcd(START_ALARM2_MINUTE));  // Minute value for alarm2
  Wire.write(decToBcd(START_ALARM2_HOUR));    // Hour value for alarm2
  Wire.write(0x80);                           // Alarm2 mask, set to match hour and minute
  Wire.write(4 | START_ALARM1_STATUS | (START_ALARM2_STATUS << 1)); // Write data to DS3231 control register (enable interrupt when alarm)   
  Wire.write(0x00);                           // Alarm1 day/date value, set to 0x00 for day of month match
  Wire.endTransmission();
  delay(200);
}

void showAlarms(){
  byte controlReg;
  int minute1, hour1, minute2, hour2;
  bool status1, status2;
  String alarm1Str;
  String alarm2Str;

  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x08); // Register address for alarm1 and alarm2 status (0x07)
  Wire.endTransmission(false);
  Wire.requestFrom(RTC_ADDRESS, 7); // Request 2 bytes (alarm1 and alarm2 status)

  if( Wire.available() ){
    minute1 = bcdToDec(Wire.read());    // Read alarm1 minutes
    hour1 = bcdToDec(Wire.read());      // Read alarm1 hours
    Wire.read();                        // Skip alarm1 day/date register
    minute2 = bcdToDec(Wire.read());    // Read alarm2 minutes
    hour2 = bcdToDec(Wire.read());      // Read alarm2 hours
    Wire.read();                        // Skip alarm1 day/date register

    // Read the DS3231 control register
    controlReg = Wire.read();

    // Read alarms interrupt enable bit (A1IE) from DS3231 control register
    status1 = bitRead(controlReg, 0);
    status2 = bitRead(controlReg, 1);

    alarm1Str = "Alarm1: " 
                    + String(hour1 < 10 ? "0" : "") + String(hour1, DEC) + ":" 
                    + String(minute1 < 10 ? "0" : "") + String(minute1, DEC) + " " 
                    + String(status1 ? "ON " : "OFF" );

    alarm2Str = "Alarm2: " 
                    + String(hour2 < 10 ? "0" : "") + String(hour2, DEC) + ":" 
                    + String(minute2 < 10 ? "0" : "") + String(minute2, DEC) + " " 
                    + String(status2 ? "ON " : "OFF" );

    // Write the formatted alarms to the LCD display
    lcd.setCursor(0, 1);
    lcd.print(alarm1Str);
    lcd.setCursor(0, 2);
    lcd.print(alarm2Str);    

    // Print the formatted alarms to the serial monitor
    Serial.print(" " + alarm1Str);
    Serial.println(" " + alarm2Str);
  }
}

void turnOnAlarm(){
  Serial.println("---- PLAYING ALARM ----");
  // Set the alarme flag to true to start playing the buzzer
  alarmPlaying = true;
}

void beep(unsigned int note) {
  tone(BUZZER_PIN, note, 100);
  delay(100);
  noTone(BUZZER_PIN);
}