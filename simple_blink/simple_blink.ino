// Define the LED pin number as a constant
#define LED_PIN  13

void setup() {
  // Set the LED pin number as a constant
  pinMode(LED_PIN , OUTPUT);
}

void loop() {
  //Turn the LED on
  digitalWrite(LED_PIN , HIGH);

  // Wait for 1 second
  delay(1000);

  // Turn the LED off
  digitalWrite(LED_PIN , LOW);

  // Wait for 1 seconf
  delay(1000);
}