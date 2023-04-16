// Define the pin numbers for the LEDs as constants
#define GREEN_LED 4
#define YELLOW_LED 3
#define RED_LED 2

// Define the pin number for the potentiometer as constants
#define POTENTIOMETER A0

void setup() {
  // Set the LEDs pins as output
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  // Read the voltage on the potentiometer
  int voltage = analogRead(POTENTIOMETER);
  // Convert the voltage to a value between 0 and 5V
  float voltageFloat = voltage * (5.0 / 1023.0);

  // Check the voltage and light up the appropriate LED
  if( voltageFloat >= 0 && voltageFloat < 2.0 ){
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
  else if( voltageFloat >= 2.1 && voltageFloat < 4.0 ){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);    
  }
  else if( voltageFloat >= 4.1 ){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }
}