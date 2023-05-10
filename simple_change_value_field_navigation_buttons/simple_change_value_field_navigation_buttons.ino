#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the I2C address of the LCD display
#define LCD_ADDRESS 0x27

// Define the pins numbers for the buttons
#define BUTTON_UP_PIN     12
#define BUTTON_DOWN_PIN   11
#define BUTTON_RIGHT_PIN  13
#define BUTTON_LEFT_PIN   10
#define BUTTON_RETURN_PIN 14
#define BUTTON_ENTER_PIN  15

// Pin number for the buzzer
#define BUZZER_PIN 6

// Define the default values
#define SIZE_FIELDS 4
#define MIN_VALUE_FIELD 0
#define MAX_VALUE_FIELD 50
#define FIELD_NOT_SELECTED -1

// Define the frequency of Notes
#define NOTE_FIELD 1760
#define NOTE_VALUE 1500

// Initialize the LCD display object
LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);

// Initialize the selected field
signed int selectedField = FIELD_NOT_SELECTED;

// Initialize the initial values of the fields
byte values[SIZE_FIELDS] = { 10, 20, 30, 40 };

void setup() {
  // Initialize the LCD display
  lcd.init();
  lcd.backlight();

  // Set the buttons pins as a input
  pinMode(BUTTON_UP_PIN     , INPUT);
  pinMode(BUTTON_DOWN_PIN   , INPUT);
  pinMode(BUTTON_RIGHT_PIN  , INPUT);
  pinMode(BUTTON_LEFT_PIN   , INPUT);
  pinMode(BUTTON_ENTER_PIN  , INPUT);
  pinMode(BUTTON_RETURN_PIN , INPUT);

  // Display the fields and theis initial values
  updateDisplay();
}

void loop() {

  // Checks if no field is selected
  if(selectedField == FIELD_NOT_SELECTED ){ 
    
    handleEnterButton();  
  
  } else { // If any field is selected

    blinkSelectedValue();

    handleReturnButton();

    handleUpButton();

    handleDownButton();

    handleRightButton();    

    handleLeftButton();    
  }
}

void handleEnterButton() {
  // Check if the enter button is pressed
  if (digitalRead(BUTTON_ENTER_PIN)) {
    beep(NOTE_FIELD);
    beep(NOTE_FIELD);

    // Select the first field
    selectedField++;

    delay(250); // debounce
    while (digitalRead(BUTTON_ENTER_PIN)); // wait for button release
  }
}

void handleReturnButton() {
  // Check if the return button is pressed
  if (digitalRead(BUTTON_RETURN_PIN)) {    

    // Display the value of the selected field
    updateValueDisplay(selectedField);

    // Reset the selected field
    selectedField = FIELD_NOT_SELECTED;

    beep(NOTE_FIELD);
    beep(NOTE_FIELD);
    beep(NOTE_FIELD);

    delay(250); // debounce
    while (digitalRead(BUTTON_RETURN_PIN)); // wait for button release
  }
}

void handleUpButton() {
  // Check if the up button is pressed
  if (digitalRead(BUTTON_UP_PIN)) {

    // Check if the selected field is the fisrt one
    if(selectedField > (FIELD_NOT_SELECTED + 1) ){
      // Updates the selected field to the previous field
      selectedField--;

      // Display the value of the previous selected field
      updateValueDisplay(selectedField+1);

      beep(NOTE_FIELD);
    } else {
      beep(NOTE_FIELD);
      beep(NOTE_FIELD);
    }

    // Display the value of the selected field
    updateValueDisplay(selectedField);

    delay(250); // debounce
    while (digitalRead(BUTTON_UP_PIN)); // Wait for button release
  }
}

void handleDownButton() {
  // Check if the down button is pressed
  if (digitalRead(BUTTON_DOWN_PIN)) {

    // Check if the selected field is the last one
    if(selectedField < (SIZE_FIELDS -1) ){
      // Updates the selected field to the next field
      selectedField++;

      // Display the value of the previous selected field
      updateValueDisplay(selectedField-1);

      beep(NOTE_FIELD);
    } else {
      beep(NOTE_FIELD);
      beep(NOTE_FIELD);
    }

    // Display the value of the selected field
    updateValueDisplay(selectedField);
    
    delay(250); // debounce
    while (digitalRead(BUTTON_DOWN_PIN)); // Wait for button release
  }
}

void handleRightButton() {
  // Check if the right button is pressed
  if (digitalRead(BUTTON_RIGHT_PIN)) {

    // While the right button is pressed
    while (digitalRead(BUTTON_RIGHT_PIN)) {
      
      // Check if the current field value if less than maximum allowed value
      if(values[selectedField] < MAX_VALUE_FIELD){
        beep(NOTE_VALUE);

        // Increment the field value
        values[selectedField]++;
      } else {
        beep(NOTE_VALUE);
        beep(NOTE_VALUE);
      }

      // Display the value of the selected field
      updateValueDisplay(selectedField);

      delay(150); // debounce
    }
  }
}

void handleLeftButton() {
  // Check if the left button is pressed
  if (digitalRead(BUTTON_LEFT_PIN)) {

    // While the left button is pressed
    while (digitalRead(BUTTON_LEFT_PIN)) {
      
      // Check if the current field value if greater than minimum allowed value
      if(values[selectedField] > MIN_VALUE_FIELD){
        beep(NOTE_VALUE);

        // Decrements the field value
        values[selectedField]--;
      } else {
        beep(NOTE_VALUE);
        beep(NOTE_VALUE);
      }

      // Display the value of the selected field
      updateValueDisplay(selectedField);

      delay(150); // debounce
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

void beep(unsigned int note) {
  tone(BUZZER_PIN, note, 100);
  delay(150);
  noTone(BUZZER_PIN);
}