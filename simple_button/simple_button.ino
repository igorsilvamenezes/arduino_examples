// Define the pin numbers for the LED and button as constants
#define LED_PIN 4
#define BUTTON_PIN 5

void setup() {
  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);

  // Set the button pin as a input with internal pull-up resistor enabled
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Check if the button is pressed
  if( digitalRead(BUTTON_PIN) == HIGH ){
    // Turn on the LED if the button is pressed
    digitalWrite(LED_PIN, HIGH);
  } else {
    // Turn off the LED if the button is not pressed
    digitalWrite(LED_PIN, LOW);
  }
}