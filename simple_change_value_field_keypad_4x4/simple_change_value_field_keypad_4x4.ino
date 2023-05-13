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

// Define the numbers of rows and cols
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

// Pin number for the buzzer
#define BUZZER_PIN 6

// Define the default values
#define SIZE_FIELDS 4
#define MIN_VALUE_FIELD 0
#define MAX_VALUE_FIELD 50
#define FIELD_NOT_SELECTED -1

// Define the frequency of Notes
#define NOTE_ERROR 262
#define NOTE_FIELD 1760
#define NOTE_VALUE 1500

// Define the number of digits allowed
#define MAX_NUM_DIGITS 4

// Define the time in second for debounce
#define DEBOUNCE_TIME 1

// Define Keys
#define NO_KEY '\0'
#define NUN_0_KEY '0'
#define NUN_1_KEY '1'
#define NUN_2_KEY '2'
#define NUN_3_KEY '3'
#define NUN_4_KEY '4'
#define NUN_5_KEY '5'
#define NUN_6_KEY '6'
#define NUN_7_KEY '7'
#define NUN_8_KEY '8'
#define NUN_9_KEY '9'
#define FUNC_ENTER_KEY 'A'
#define FUNC_RETURN_KEY 'B'
#define FUNC_UP_KEY 'C'
#define FUNC_DOWN_KEY 'D'
#define FUNC_CLEAN_KEY '*'
#define FUNC_UNDO_KEY '#'

// Define the state enum
enum State{
  IDLE, 
  WAITING_FOR_INPUT, 
  INPUTTING_NUMBER,
  NUMBER_CONFIRMED,
  NUMBER_TOO_LARGE
};

// Initialize the LCD display object
LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);

// Set keypad pins
const byte ROW_PINS[ KEYPAD_ROWS ] = {36, 34, 32, 30};
const byte COL_PINS[ KEYPAD_COLS ] = {28, 26, 24, 22};

// Initialize the keys values 
int keys[ KEYPAD_ROWS ][ KEYPAD_COLS ] = {
  { NUN_1_KEY,      NUN_2_KEY,  NUN_3_KEY,     FUNC_ENTER_KEY},
  { NUN_4_KEY,      NUN_5_KEY,  NUN_6_KEY,     FUNC_RETURN_KEY},
  { NUN_7_KEY,      NUN_8_KEY,  NUN_9_KEY,     FUNC_UP_KEY},
  { FUNC_CLEAN_KEY, NUN_0_KEY,  FUNC_UNDO_KEY, FUNC_DOWN_KEY},
};

// Initialize the selected field
signed int selectedField = FIELD_NOT_SELECTED;

// Initialize the initial values of the fields
unsigned int values[SIZE_FIELDS] = { 10, 20, 30, 40 };

// Stores the last value during editing
unsigned int lastFieldValue;

// Initialize the state variable
State currentState = IDLE;

// Stores the time in milliseconds used in the debounce
unsigned long startTime;

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

bool checkNumDigits(unsigned int number) {
  byte numDigits = 0;

  while(number != 0){
    number /= 10;
    numDigits++;
  }

  return numDigits <= MAX_NUM_DIGITS;
}
void handleNumberKeys(char key){
  unsigned int newValue = 0;
  byte digit = 0;

  if(currentState == NUMBER_TOO_LARGE){
      beep(NOTE_ERROR);
      beep(NOTE_ERROR);      
  } else {

    if(isdigit(key)){
      digit = key - '0';

      if(currentState == WAITING_FOR_INPUT){
        newValue = digit;

      } else if(currentState == INPUTTING_NUMBER) {    
        newValue = values[selectedField] * 10 + digit;

      }

      if(checkNumDigits(newValue)){
        values[selectedField] = newValue;      
        currentState = INPUTTING_NUMBER;
        beep(NOTE_FIELD);

      } else {
        currentState = NUMBER_TOO_LARGE;
        beep(NOTE_ERROR);
        beep(NOTE_ERROR);
      }    
    }

    // Display the value of the selected field
    updateValueDisplay(selectedField);
  }
}

void handleCleanKey() {
  // Reset the field value 
  values[selectedField] = 0;
  currentState = INPUTTING_NUMBER;
  beep(NOTE_FIELD);
  beep(NOTE_FIELD);

  // Display the value of the selected field
  updateValueDisplay(selectedField);
}

void handleUndoKey(){
  if(currentState == INPUTTING_NUMBER || currentState == NUMBER_TOO_LARGE){
    values[selectedField] = lastFieldValue;
    currentState = WAITING_FOR_INPUT;

    beep(NOTE_FIELD);
    beep(NOTE_FIELD);
  } else {
    beep(NOTE_ERROR);
    beep(NOTE_ERROR);
  }
}

void handleDownKey() {
  // Check if the selected field is the last one
  if(selectedField < (SIZE_FIELDS -1) ){
    // Updates the selected field to the next field
    selectedField++;
    currentState = WAITING_FOR_INPUT;
    lastFieldValue = values[selectedField];

    // Display the value of the previous selected field
    updateValueDisplay(selectedField-1);

    beep(NOTE_FIELD);
  } else {
    beep(NOTE_ERROR);
    beep(NOTE_ERROR);
  }

  // Display the value of the selected field
  updateValueDisplay(selectedField);
}

void handleUpKey() {
  // Check if the selected field is the fisrt one
  if(selectedField > (FIELD_NOT_SELECTED + 1) ){
    // Updates the selected field to the previous field
    selectedField--;
    currentState = WAITING_FOR_INPUT;
    lastFieldValue = values[selectedField];

    // Display the value of the previous selected field
    updateValueDisplay(selectedField+1);

    beep(NOTE_FIELD);
  } else {
    beep(NOTE_ERROR);
    beep(NOTE_ERROR);
  }

  // Display the value of the selected field
  updateValueDisplay(selectedField);
}

void handleReturnKey() {
  // Display the value of the selected field
  updateValueDisplay(selectedField);

  // Reset the selected field
  selectedField = FIELD_NOT_SELECTED;
  currentState = IDLE;
  lastFieldValue  = 0;

  beep(NOTE_FIELD);
  beep(NOTE_FIELD);
  beep(NOTE_FIELD);
}

void handleEnterKey() {
  // Select the first field
  selectedField++;
  currentState = WAITING_FOR_INPUT;
  lastFieldValue = values[selectedField];

  beep(NOTE_FIELD);
  beep(NOTE_FIELD);
}

// Function to read the matrix keypad
char readKeypad() {
  char key = NO_KEY;

  // Check if the debounce time has passed
	if ( (millis()-startTime) > DEBOUNCE_TIME ) {

    // Re-intialize the row pins. Allows sharing these pins with other hardware.
    for (byte row = 0; row < KEYPAD_ROWS; row++) {
      pinMode( ROW_PINS[row], INPUT_PULLUP);
    }

    // Check each column for a pressed key
    for(byte col = 0; col < KEYPAD_COLS; col++){

      // Set the current column to OUTPUT
      pinMode( COL_PINS[col], OUTPUT );
      // Set the current column to LOW
      digitalWrite( COL_PINS[col], LOW);

      // Check each row for a pressed key
      for(int row = 0; row < KEYPAD_ROWS; row++){
        if(digitalRead( ROW_PINS[row] ) == LOW ){        
          key = keys[ row ][ col ];

          //delay(250); // debounce
          while( digitalRead( ROW_PINS[ row ] ) == LOW);
        }
      }

      // Set pin to high impedance input. Effectively ends column pulse.
      digitalWrite(COL_PINS[col], HIGH);
      pinMode(COL_PINS[col], INPUT);
    }

    // Save the current time
		startTime = millis();
	}  

  return key;
}

void setup() {
  Serial.begin(9600);

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
  // Read the matrix keypad
  char key = readKeypad();

  // Checks if no field is selected
  if(selectedField == FIELD_NOT_SELECTED ){ 

    // Check if the enter key is pressed
    if(key == FUNC_ENTER_KEY){
      handleEnterKey();
    }
    
  } else { // If any field is selected

    blinkSelectedValue();

    switch(key){
      case FUNC_RETURN_KEY:
        handleReturnKey();
        break;

      case FUNC_UP_KEY:
        handleUpKey();
        break;
      
      case FUNC_DOWN_KEY:
        handleDownKey();
        break;

      case FUNC_CLEAN_KEY:
        handleCleanKey();
        break;

      case FUNC_UNDO_KEY:
        handleUndoKey();
        break;

      case NUN_0_KEY:
      case NUN_1_KEY:
      case NUN_2_KEY:
      case NUN_3_KEY:
      case NUN_4_KEY:
      case NUN_5_KEY:
      case NUN_6_KEY:
      case NUN_7_KEY:
      case NUN_8_KEY:
      case NUN_9_KEY:
        handleNumberKeys(key);
        break;
    }
  }
}