// Define the pin numbers for the LEDs
#define GREEN_LED   4
#define YELLOW_LED  3
#define RED_LED     2

void setup() {
  // Set the LED pins as outputs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  // Turn on the green LED and wait for 5 seconds
  digitalWrite(GREEN_LED, HIGH);
  delay(5000);

  // Turn off the green LED and turn on the yellow LED
  // wait for 2 seconds
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);
  delay(2000);

  //Turn off thr yellow LED and turn on the red LED
  // wait for 4 seconds
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  delay(4000);

  //Turn off the red LED and start the sequence over again
  digitalWrite(RED_LED, LOW);
}
