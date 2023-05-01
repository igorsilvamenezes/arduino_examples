#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BUZZER_PIN 6

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

const byte ROW_PINS[ KEYPAD_ROWS ] = {36, 34, 32, 30};
const byte COL_PINS[ KEYPAD_COLS ] = {28, 26, 24, 22};

char keys[ KEYPAD_ROWS ][ KEYPAD_COLS ] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // Open serial communications and wait for port to open
  Serial.begin(9600);
  while(!Serial){
    ;
  } // wait for serial port to connect

  Wire.begin();

  // initialize the LCD display
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // initialize all rows the matrix keypad
  for(int i = 0; i < KEYPAD_ROWS; i++ ){
    pinMode(ROW_PINS[i], INPUT_PULLUP);
  }

  // initialize all cols the matrix keypad
  for(int i = 0; i < KEYPAD_COLS; i++){
    pinMode(COL_PINS[i], INPUT_PULLUP);
  }

  Serial.println("Press a key:");
}

void loop() {
  char key;

  lcd.setCursor(0, 0);
  lcd.print("Press a key:");  

  // read the matrix keypad
  key = readKeypad();

  // if a key is pressed, display it on the LCD and Serial
  if( key != '\0' ){
    lcd.setCursor(0, 0);
    lcd.print("Key pressed: ");
    lcd.print(key);

    Serial.print("Key pressed: ");
    Serial.println(key);

    delay(1000);
    lcd.clear();    
  }
}

// Function to read the matrix keypad
char readKeypad() {
  char key = '\0';

  // check each column for a pressed key
  for(int col = 0; col < KEYPAD_COLS; col++){

    // set the current column to OUTPUT
    pinMode( COL_PINS[col], OUTPUT );
    // set the current column to LOW
    digitalWrite( COL_PINS[col], LOW);

    // check each row for a pressed key
    for(int row = 0; row < KEYPAD_ROWS; row++){

      // check if the current row is LOW (key pressed)
      if(digitalRead( ROW_PINS[row] ) == LOW ){

        // play a beep
        beep();
       
        // set the key variable to the pressed key
        key = keys[ row ][ col ];        

        // Wait for the key to be released
        while( digitalRead( ROW_PINS[ row ] ) == LOW);

        // exit inner for loop when key is found
        break;
      }
    }

    // set all columns back to INPUT_PULLUP
    for(int i = 0; i < KEYPAD_COLS; i++){
      pinMode(COL_PINS[i], INPUT_PULLUP);
    }
  }

  return key;
}

void beep() {
  tone(BUZZER_PIN, 1500, 100);
  delay(100);
  noTone(BUZZER_PIN);
}