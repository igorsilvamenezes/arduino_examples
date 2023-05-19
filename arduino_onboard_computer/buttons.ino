// Define the pins numbers for the buttons
#define BUTTON_UP_PIN     12
#define BUTTON_DOWN_PIN   11
#define BUTTON_RIGHT_PIN  13
#define BUTTON_LEFT_PIN   10
#define BUTTON_RETURN_PIN 14
#define BUTTON_ENTER_PIN  15

// Set the buttons pins as a input
void init_buttons(){
  pinMode(BUTTON_UP_PIN     , INPUT);
  pinMode(BUTTON_DOWN_PIN   , INPUT);
  pinMode(BUTTON_RIGHT_PIN  , INPUT);
  pinMode(BUTTON_LEFT_PIN   , INPUT);
  pinMode(BUTTON_ENTER_PIN  , INPUT);
  pinMode(BUTTON_RETURN_PIN , INPUT);
}

void handleEnterButton() {
  // Check if the enter button is pressed
  if (digitalRead(BUTTON_ENTER_PIN)) {

    enterEditing();

    delay(250); // debounce
    while (digitalRead(BUTTON_ENTER_PIN)); // wait for button release
  }
}

void handleReturnButton() {
  // Check if the return button is pressed
  if (digitalRead(BUTTON_RETURN_PIN)) {    

    cancelEditing();

    delay(250); // debounce
    while (digitalRead(BUTTON_RETURN_PIN)); // wait for button release
  }
}

void handleUpButton() {
  // Check if the up button is pressed
  if (digitalRead(BUTTON_UP_PIN)) {

    // While the up button is pressed
    while (digitalRead(BUTTON_UP_PIN)) {      
      incrementFieldValue();

      delay(150); // debounce
    }  
  }
}

void handleDownButton() {
  // Check if the down button is pressed
  if (digitalRead(BUTTON_DOWN_PIN)) {
    
    // While the down button is pressed
    while (digitalRead(BUTTON_DOWN_PIN)) {      
      decrementFieldValue();

      delay(150); // debounce
    }    
  }
}

void handleRightButton() {
  // Check if the right button is pressed
  if (digitalRead(BUTTON_RIGHT_PIN)) {

    selectNextField();

    delay(250); // debounce
    while (digitalRead(BUTTON_RIGHT_PIN)); // Wait for button release    
  }
}

void handleLeftButton() {
  // Check if the left button is pressed
  if (digitalRead(BUTTON_LEFT_PIN)) {
    
    selectPreviousField();

    delay(250); // debounce
    while (digitalRead(BUTTON_LEFT_PIN)); // Wait for button release    
  }
}

void handleButtons(){
  handleEnterButton();
  handleReturnButton();
  handleUpButton();
  handleDownButton();
  handleRightButton();
  handleLeftButton();
}