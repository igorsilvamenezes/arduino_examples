#include <Wire.h>
#define DS3231_I2C_ADDRESS 0x68

void setup() {
  Serial.begin(9600);
  Wire.begin();

  printRTCRegister();
}

void loop() {

}

void printRTCRegister(){
  byte msbTemperature;
  byte lsbTemperature;

  // read the DS3231 registers
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set register pointer to 0x00
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 19); // read 19 bytes

  // print the values of each register in binary and decimal
  Serial.println("Register values:");
  Serial.println("**********************************************************************");

  printRegister(0x00, "Seconds"           , Wire.read(), true);
  printRegister(0x01, "Minutes"           , Wire.read(), true);
  printRegister(0x02, "Hour"              , Wire.read(), true);
  printRegister(0x03, "Day"               , Wire.read(), true);
  printRegister(0x04, "Date"              , Wire.read(), true);
  printRegister(0x05, "Month"             , Wire.read(), true);
  printRegister(0x06, "Year"              , Wire.read(), true);
  printRegister(0x07, "Alarm 1 Seconds"   , Wire.read(), true);
  printRegister(0x08, "Alarm 1 Minutes"   , Wire.read(), true);
  printRegister(0x09, "Alarm 1 Hour"      , Wire.read(), true);
  printRegister(0x0A, "Alarm 1 Day"       , Wire.read(), true);
  printRegister(0x0B, "Alarm 2 Minutes"   , Wire.read(), true);
  printRegister(0x0C, "Alarm 2 Hour"      , Wire.read(), true);
  printRegister(0x0D, "Alarm 2 Day"       , Wire.read(), true);
  printRegister(0x0E, "Control"           , Wire.read(), false);
  printRegister(0x0F, "Control/Status"    , Wire.read(), false);
  printRegister(0x10, "Aging Offset"      , Wire.read(), false);

  msbTemperature = Wire.read();
  lsbTemperature = Wire.read();

  printRegister(0x11, "MSB of Temperature", msbTemperature, false);
  printRegister(0x12, "LSB of Temperature", lsbTemperature, false);
  
  Serial.println();
  printTemperature(msbTemperature, lsbTemperature);

  Serial.println("**********************************************************************");
}

// helper function to print the value of a register in binary and decimal
void printRegister(byte address, String label, byte value, bool isBcd) {

  Serial.print("0x");
  Serial.print(address < 16 ? "0" : "" );
  Serial.print(address, HEX);
  Serial.print("  |  ");

  // Print the Label
  Serial.print(label);
  for(int i = 1; i < 20 - label.length(); i++) {
    Serial.print(" ");
  }  
  Serial.print("  |  ");

  // Print Byte In Binary
  for (int i = 7; i >= 0; i--) {
    Serial.print((value >> i) & 1);
  }

  Serial.print("  |  ");

  if(isBcd){
    Serial.print(bcdToDec(value), DEC);
  } else {
    Serial.print(value, DEC);
  }

  Serial.println();
}

void printTemperature(byte msb, byte lsb){
  int16_t temp;
  float temperature;

  // Calculate the temperature in Celsius
  temp = ( (msb << 8) | lsb ) >> 6; // Combine the MSB and LSB, and shift right by 6 bits
  temperature = temp * 0.25; // Each bit representes 0.25 degrees Celsius  

  Serial.print("Temperature: ");
  Serial.println(temperature);
}

// Function to convert binary-coded decimal (BCD) to decimal
int bcdToDec(int bcd){
  return ((bcd / 16) * 10) + (bcd % 16);
}