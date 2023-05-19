#include "rtc_alarm.h"

void init_rtc_alarm(){

  // Set up an interrupt to trigger the callback function turnOnAlarm on falling edge
  //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), turnOnAlarm, FALLING);
}

void updateAlarms(alarm_t *alarm){
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x07);  
  Wire.write(0);                              // Second value for alarm1
  Wire.write(decToBcd(alarm->alarm1.minute));  // Minute value for alarm1
  Wire.write(decToBcd(alarm->alarm1.hour));    // Hour value for alarm1
  Wire.write(0x80);                           // Alarm1 mask, set to match hour and minute
  Wire.write(decToBcd(alarm->alarm2.minute));  // Minute value for alarm2
  Wire.write(decToBcd(alarm->alarm2.hour));    // Hour value for alarm2
  Wire.write(0x80);                           // Alarm2 mask, set to match hour and minute
  Wire.write(4 | alarm->alarm1Status | (alarm->alarm2Status << 1)); // Write data to DS3231 control register (enable interrupt when alarm)   
  Wire.write(0x00);                           // Clear alarm flag bits
  Wire.endTransmission();                     // Stop transmission and release the I2C bus
  delay(200);                                 // Wait 200ms
}


void readAlarms(alarm_t *alarm){
  byte controlReg;
  byte statusReg;

  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x08); // Send register address
  Wire.endTransmission(false);
  Wire.requestFrom(RTC_ADDRESS, 8); // Request 2 bytes (alarm1 and alarm2 status)

  if( Wire.available() ){
    alarm->alarm1.minute = bcdToDec(Wire.read());    // Read alarm1 minutes
    alarm->alarm1.hour = bcdToDec(Wire.read());      // Read alarm1 hours
    Wire.read();                              // Skip alarm1 day/date register
    alarm->alarm2.minute = bcdToDec(Wire.read());    // Read alarm2 minutes
    alarm->alarm2.hour = bcdToDec(Wire.read());      // Read alarm2 hours
    Wire.read();                              // Skip alarm1 day/date register

    // Read the DS3231 control register
    controlReg = Wire.read();

    statusReg = Wire.read();

    // Read alarms interrupt enable bit (A1IE) from DS3231 control register
    alarm->alarm1Status = bitRead(controlReg, 0);
    alarm->alarm1Status = bitRead(controlReg, 1);
  }
}