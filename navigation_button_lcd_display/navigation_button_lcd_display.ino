#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the I2C address of the LCD display
#define LCD_ADDRESS 0X27

// Define the pin numbers for the buttons
#define BUTTON_UP 12
#define BUTTON_DOWN 11
#define BUTTON_RIGHT 13
#define BUTTON_LEFT 10
#define BUTTON_ENTER 15
#define BUTTON_RETURN 14

// Define the pin numbers for the LEDs
#define LED_GREEN 7
#define LED_YELLOW 8
#define LED_RED 9

// Define the number of buttons
#define BUTTONS_SIZE 6

// Define a blank line
#define BLANK_LINE "                    " 

// Define the Button type
typedef struct{
  byte pin;
  char* name;
} Button;

// Initialize the LCD display object
LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);

int counter = 0;

// Define and initialize the button Mapping
Button buttonMap[BUTTONS_SIZE] {
    { BUTTON_UP     , "UP"    },
    { BUTTON_DOWN   , "DOWN"  },
    { BUTTON_RIGHT  , "RIGHT" },
    { BUTTON_LEFT   , "LEFT"  },
    { BUTTON_ENTER  , "ENTER" },
    { BUTTON_RETURN , "RETURN"}
};

// Define a button not selected
const Button BUTTON_NONE = {-1, "NONE"};

void setup() {
  // Open serial communicatons and wait for port to open
  Serial.begin(9600);
  while(!Serial){
    ;
  } // wait fo serial port to connect.

  // Initialize the LCD display
  lcd.init();
  lcd.backlight();

  // Set the LED pins as output
  pinMode( LED_GREEN, OUTPUT);
  pinMode( LED_YELLOW, OUTPUT);
  pinMode( LED_RED, OUTPUT);

  // Set the button pins as input with internal pull-up resistors enabled
  pinMode( BUTTON_UP      , INPUT );
  pinMode( BUTTON_DOWN    , INPUT );
  pinMode( BUTTON_RIGHT   , INPUT );
  pinMode( BUTTON_LEFT    , INPUT );
  pinMode( BUTTON_ENTER   , INPUT );
  pinMode( BUTTON_RETURN  , INPUT );
}

void loop() {
  Button buttonPressed;

  // Write the initial text to the display
  lcd.setCursor(0, 0);
  lcd.print( "Press a key..." );

  // Increment the counter variable
  counter++;

  // Blink the LED every 20 interations
  if( (counter % 20) == 0 )
    digitalWrite(LED_GREEN, !digitalRead(LED_GREEN));

  buttonPressed = checkButtonPressed();
  if( buttonPressed.pin != BUTTON_NONE.pin){
      lcd.setCursor(0, 0);
      lcd.print(BLANK_LINE);
      lcd.setCursor(0, 0);
      lcd.print( buttonPressed.name );
      lcd.print( " pressed");
      delay(500);

      releaseButton(buttonPressed);
  }
}

Button checkButtonPressed(){
  for(int button = 0; button < BUTTONS_SIZE; button++){
    
    if( digitalRead( buttonMap[button].pin ) == HIGH ){

      digitalWrite(LED_GREEN, LOW);

      Serial.print(buttonMap[button].name);
      Serial.println(" - button pressed");

      digitalWrite( LED_YELLOW, HIGH );
      tone(6, 262, 1000 / 4);

      return buttonMap[button];
    }     
  }

  return BUTTON_NONE;
}

void releaseButton(Button button){
  
  // Wait until button released
  while( digitalRead( button.pin ) == HIGH );
  
  Serial.print(button.name);
  Serial.println(" - button relesed");

  lcd.setCursor(0, 0);
  lcd.print(BLANK_LINE);  
          
  digitalWrite( LED_YELLOW, LOW );
}