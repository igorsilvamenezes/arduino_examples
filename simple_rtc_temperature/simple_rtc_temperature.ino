#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the I2C address of the LCD display
#define LCD_ADDRESS 0X27

// Define the I2C address of the RTC module
#define RTC_ADDRESS 0X68

// Initialize the LCD display object
LiquidCrystal_I2C lcd( LCD_ADDRESS, 20, 4);

void setup() {
  // Open serial communication and wait for port to open
  Serial.begin(9600);
  while(!Serial){
    ;
  } // wait for serial port to connect

  // Initialize the I2C communication
  Wire.begin();

  // Initialize the LCD display
  lcd.init();
  lcd.backlight();
}

void loop() {
  float temperature = readTemperature();
  displayTemperature(temperature);

  // Print the Temperature to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("C");

  delay(1000);
}

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

void displayTemperature(float temperature){
  // Display the temperature on LCD module
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  lcd.print(temperature);
  lcd.print("C");
}