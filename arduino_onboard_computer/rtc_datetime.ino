//datetime_t currentDatetime;

void init_rt_datetime(){
  //memset(&currentDatetime, 0, sizeof(currentDatetime));
}

// Write the start date and time to the RTC module
void updateDatetime(datetime_t *datetime) {
  // Start an I2C transmission to the RTC module
  Wire.beginTransmission(RTC_ADDRESS);

  // Set the register pointer to 0 to start writing from the first register
  Wire.write(0);
  
  // Write de values of the date and time
  Wire.write(decToBcd(datetime->time.second));
  Wire.write(decToBcd(datetime->time.minute));
  Wire.write(decToBcd(datetime->time.hour));
  Wire.write(decToBcd(datetime->date.week));
  Wire.write(decToBcd(datetime->date.day));
  Wire.write(decToBcd(datetime->date.month));
  Wire.write(decToBcd(datetime->date.year));

  // End the I2C transmission
  Wire.endTransmission();
}

void readDateTime(datetime_t *datetime){
  // Request the date and time from the RCT module
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0); // Start at register 0 (second)
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDRESS, 7); // Request 7 bytes (seconds, minutes, day, date, month, year)

  // Read the date and time from RTC module
  datetime->time.second = bcdToDec(Wire.read() & 0x7F);
  datetime->time.minute = bcdToDec(Wire.read());
  datetime->time.hour   = bcdToDec(Wire.read() & 0x3F);
  datetime->date.week   = bcdToDec(Wire.read());
  datetime->date.day    = bcdToDec(Wire.read());
  datetime->date.month  = bcdToDec(Wire.read());
  datetime->date.year   = bcdToDec(Wire.read());
}

// Function to convert binary-coded decimal (BCD) to decimal
int bcdToDec(int bcd){
  return ((bcd / 16) * 10) + (bcd % 16);
}

// Function to convert decimal to binary-coded decimal (BCD)
int decToBcd(int val) {
  return ((val / 10) << 4) + (val % 10);
}