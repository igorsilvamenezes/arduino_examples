// Define the pin numbers for the LEDs constants
#define GREEN_LED 4
#define YELLOW_LED 3
#define RED_LED 2

// Define the pin numbers for the buttons as constants
#define GREEN_BUTTON 5
#define YELLOW_BUTTON 6
#define RED_BUTTON 7

void setup() {
  // Set the LED pins as outputs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Set the button pins as inputs with internal pull-up resistors enabled
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON, INPUT_PULLUP);
  pinMode(RED_BUTTON, INPUT_PULLUP);
}

void loop() {
  // Check if the green button is pressed
  if( digitalRead(GREEN_BUTTON) == HIGH ){
    // Turn on the green LED and turn off the others LEDs
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    delay(2000);
  }

  // Check id the yellow button is pressed
  if( digitalRead(YELLOW_BUTTON) == HIGH ){
    // Turn on the yellow LED and turn off the others LEDS
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    delay(2000);
  }

  if( digitalRead(RED_BUTTON) == HIGH ){
    // Turn on the red LED and turn off the others LEDs
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    delay(2000);
  }

  // Turn off all LEDs when no button are pressed
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
}