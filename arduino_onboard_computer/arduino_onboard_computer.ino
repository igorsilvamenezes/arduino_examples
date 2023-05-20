#include <Wire.h>
#include "app.h"
#include "rtc_datetime.h"
#include "rtc_alarm.h"

#define EVENT_TIME_SHOW_DATE_TIME 1000
#define EVENT_TIME_SHOW_TEMPERATURE 5000
#define EVENT_TIME_SHOW_ALARMS 1000

unsigned long previousTime_datetime = 0;
unsigned long previousTime_temperature = 0;
unsigned long previousTime_alarms = 0;

field_t *selectedField = NULL;
int currentIndex = -1;

datetime_t currentDatetime;
float currentTemperature;
alarm_t currentAlarms;

SystemState currentState;

void setup() {
  // Open serial comunication and wait for port to open
  Serial.begin(9600);
  while( !Serial){
    ;
  } // wait for serial port to connect

  // Initialize the I2C communication
  Wire.begin();

  // Initialize the datetime
  init_rt_datetime();

  // Initialize the buttons
  init_buttons();

  // Initialize the LCD display
  init_display();

  currentState = IDLE;

  currentTemperature = readTemperature();
  showTemperature(currentTemperature);  
}

void loop() {
  // Updates frequnetly
  unsigned long currentTime = millis();
  

  if( (currentTime - previousTime_datetime) >= EVENT_TIME_SHOW_DATE_TIME) {

    if(currentState != EDIT_DATETIME){
      readDateTime(&currentDatetime);
      showDateTime(&currentDatetime);
    }

    previousTime_datetime = currentTime;
    showSystemState();
  }

  if( (currentTime - previousTime_temperature) >= EVENT_TIME_SHOW_TEMPERATURE){
    currentTemperature = readTemperature();
    //float minTemp = calcMinTemperature(currentTemperature);
    //float maxTemp = calcMaxTemperature(currentTemperature);

    showTemperature(currentTemperature);
    //showTemperature(currentTemperature, minTemp, maxTemp);
    previousTime_temperature = currentTime;
  }

  if( (currentTime - previousTime_alarms) >= EVENT_TIME_SHOW_ALARMS){

    if(currentState != EDIT_ALARMS){
      readAlarms(&currentAlarms);
      showAlarms(&currentAlarms);
    }

    previousTime_alarms = currentTime;
  }

  handleButtons();
  blinkSelectedValue();
}

void changeState(){
  currentState = static_cast<SystemState>((currentState + 1) % NUM_SYSTEM_STATES);
}

void enterEditing(){
  if(currentState == IDLE){
    currentState = EDIT_DATETIME;
    currentIndex = 0;
    selectedField = &fields[currentIndex];    
    showDateTime(&currentDatetime);
    beep(NOTE_FIELD);
    beep(NOTE_FIELD);

  } else if(currentState == EDIT_DATETIME || currentState == EDIT_ALARMS){
    saveEditing();
    beep(NOTE_FIELD);
    beep(NOTE_FIELD);
    beep(NOTE_FIELD);
  }
}

void selectNextField(){
  if( (currentState == EDIT_DATETIME || currentState == EDIT_ALARMS ) 
    && currentIndex < NUM_FIELDS -1){

    beep(NOTE_FIELD);
    currentIndex++;
    selectedField = &fields[currentIndex];

    if(currentIndex >= 7){
      currentState = EDIT_ALARMS;
    }
    
    //Show the previous field
    showSelectedValue(&fields[currentIndex -1]);

  } else {
    beep(NOTE_ERROR);
    beep(NOTE_ERROR);
  }
}

void selectPreviousField(){    
  if( (currentState == EDIT_DATETIME || currentState == EDIT_ALARMS)
    && currentIndex > 0){

    beep(NOTE_FIELD);
    currentIndex--;
    selectedField = &fields[currentIndex];
    currentState == EDIT_DATETIME;

    if(currentIndex < 7){
      currentState = EDIT_DATETIME;
    }
    
    //Show the previous field
    showSelectedValue(&fields[currentIndex + 1]);

  } else {
    beep(NOTE_ERROR);
    beep(NOTE_ERROR);
  }
}

void saveEditing(){
  if(currentState == EDIT_DATETIME){
    selectedField = NULL;
    currentState = IDLE;
    currentIndex = -1;
    
    datetime_t datetime = decodeDate();
    updateDatetime(&datetime);
  } else if(currentState == EDIT_ALARMS){
    selectedField = NULL;
    currentState = IDLE;
    currentIndex = -1;
    
    alarm_t alarms = decodeAlarms();
    updateAlarms(&alarms);
  } else {
    beep(NOTE_ERROR);
    beep(NOTE_ERROR);
  }
}

void cancelEditing(){
  if(currentState == EDIT_DATETIME || currentState == EDIT_ALARMS){
    selectedField = NULL;
    currentState = IDLE;
    currentIndex = -1;
    
    readDateTime(&currentDatetime);
    showDateTime(&currentDatetime);

    readAlarms(&currentAlarms);
    showAlarms(&currentAlarms);


    beep(NOTE_FIELD);
    beep(NOTE_FIELD);
  } else {
    beep(NOTE_ERROR);
    beep(NOTE_ERROR);
  }
}

void incrementFieldValue(){
  int tempValue = 0;

  if( (currentState == EDIT_DATETIME || currentState == EDIT_ALARMS) 
    && selectedField != NULL){
    
    tempValue = selectedField->value.toInt();

    if(tempValue < selectedField->maximum){
      selectedField->value = ++tempValue;
      //showDateTime(&currentDatetime);
      beep(NOTE_VALUE);
    } else {
      beep(NOTE_ERROR);
    }
  } else {
    beep(NOTE_ERROR);
    beep(NOTE_ERROR);
  }
}

void decrementFieldValue(){
  int tempValue = 0;

  if( (currentState == EDIT_DATETIME || currentState == EDIT_ALARMS) 
    && selectedField != NULL){

    tempValue = selectedField->value.toInt();

    if(tempValue > selectedField->mininum){
      selectedField->value = --tempValue;
      //showDateTime(&currentDatetime);
      beep(NOTE_VALUE);
    } else {
      beep(NOTE_ERROR);
    }

  } else {
    beep(NOTE_ERROR);
    beep(NOTE_ERROR);
  }
}