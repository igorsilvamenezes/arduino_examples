#define BUZZER_PIN 6 // Pin number for the buzzer
#define LED_PIN 9 // Pin number for the LED
#define INTERRUPT_PIN 2 // Pin number for the interrupt

volatile bool ledState = LOW; // Global variable to store LED state

void setup() {
  pinMode(LED_PIN, OUTPUT); // Set LED pin as OUTPUT
  pinMode(BUZZER_PIN, OUTPUT); // Set BUZZER pin as OUTPUT
  pinMode(INTERRUPT_PIN, INPUT_PULLUP); // Set interrupt pin as INPUT with internal pull-up resistor

  // Initialize LED to LOW
  digitalWrite(LED_PIN, ledState);

  // Set up an interrupt to trigger the callback function toggleLed on falling edge
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), toggleLed, FALLING);
}

void loop() {
  // Main code here, if any
} 

void toggleLed() {
  // Invert LED state
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);

  // Play a beep
  tone(BUZZER_PIN, 1500, 100);
}