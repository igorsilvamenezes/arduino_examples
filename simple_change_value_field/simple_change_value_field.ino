#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the pins numbers for the buttons
#define BUTTON_ENTER_PIN 15
#define BUTTON_INCREMENT_PIN 14

// Define the default values
#define SIZE_FIELDS 4
#define MIN_VALUE_FIELD 0
#define MAX_VALUE_FIELD 50
#define FIELD_NOT_SELECTED -1

// Initialize the LCD display object
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Initialize the selected field
signed int selectedField = FIELD_NOT_SELECTED;

// Initialize the initial values of the fields
byte values[SIZE_FIELDS] = { 10, 20, 30, 40 };

void setup() {
  // Initialize the LCD display
  lcd.init();
  lcd.backlight();

  // Set the buttons pins as a input
  pinMode(BUTTON_ENTER_PIN, INPUT);
  pinMode(BUTTON_INCREMENT_PIN, INPUT);

  // Display the fields and theis initial values
  updateDisplay();
}

void loop() {
  // Check if the enter button is pressed
  if (digitalRead(BUTTON_ENTER_PIN)) {

    // Increment the selected field
    selectedField++;
    if(selectedField > (SIZE_FIELDS -1) ){      
      selectedField = FIELD_NOT_SELECTED; // Reset the selected field
    }

    // Display the fields and theis values
    updateDisplay();

    delay(250); // debounce
    while (digitalRead(BUTTON_ENTER_PIN)); // wait for button release
  }
  
  // Check if there is any field selected
  if(selectedField != FIELD_NOT_SELECTED ){

    blinkSelectedValue();

    // Check if the increment button is pressed
    if (digitalRead(BUTTON_INCREMENT_PIN)) {

      // While the increment button is pressed
      while (digitalRead(BUTTON_INCREMENT_PIN)) {

        // Increment the value of field
        // set minimum value if it exceeds the maximum value
        values[selectedField]++;
        if(values[selectedField] > MAX_VALUE_FIELD){
          values[selectedField] = MIN_VALUE_FIELD;
        }

        updateValueDisplay(selectedField);
        delay(250); // debounce
      }
    }
  }
}

void updateDisplay() {
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Field 1: ");
  lcd.print(values[0]);
  
  lcd.setCursor(0, 1);
  lcd.print("Field 2: ");
  lcd.print(values[1]);
  
  lcd.setCursor(0, 2);
  lcd.print("Field 3: ");
  lcd.print(values[2]);
  
  lcd.setCursor(0, 3);
  lcd.print("Field 4: ");
  lcd.print(values[3]);
}

void updateValueDisplay(byte selectedField) {
  lcd.setCursor(9, selectedField);
  lcd.print("           ");
  lcd.setCursor(9, selectedField);
  lcd.print(values[selectedField]);
}

void blinkSelectedValue() {
  lcd.setCursor(9, selectedField);

  if (millis() % 1000 < 500) {
    lcd.print("           ");
  } else {
    lcd.print(values[selectedField]);
  }
}