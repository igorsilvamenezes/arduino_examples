
float readTemperature(){
  byte msb;
  byte lsb;
  int16_t temp;
  float temperature;

  // Request the temperature from the DS3231 module
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0X11); // Register address from temperature (0x11)
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDRESS, 2); // Request 2 byte (temperature data)

  // Read the temperature data from the DS3231 module
  msb = Wire.read();
  lsb = Wire.read();

  // Calculate the temperature in Celsius
  temp = ( (msb << 8) | lsb ) >> 6; // Combine the MSB and LSB, and shift right by 6 bits
  temperature = temp * 0.25; // Each bit representes 0.25 degrees Celsius

  return temperature;
}

float calcMinTemperature(float temperature){
  static float minTemperature = 99;

  if(temperature < minTemperature){
    minTemperature = temperature;
  }

  return minTemperature;
}

float calcMaxTemperature(float temperature){
  static float maxTemperature = 0;

  if( temperature > maxTemperature){
    maxTemperature = temperature;
  }

  return maxTemperature;
}