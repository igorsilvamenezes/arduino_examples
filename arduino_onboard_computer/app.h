#ifndef APP_H
#define APP_H

// Define the I2C address of the RTC module
#define RTC_ADDRESS 0x68

#define NUM_FIELDS 13

// Define the frequency of Notes
#define NOTE_ERROR 262
#define NOTE_FIELD 1760
#define NOTE_VALUE 1500

typedef struct{
  String name;
  String value;
  unsigned int column;
  unsigned int line;
  unsigned int size;
  unsigned int mininum;
  unsigned int maximum;
  String (*format)(String);
} field_t;


String formatWeek(String week);
String formatNumber(String value);
String formatAlarmStatus(String value);

field_t fields[NUM_FIELDS] = {
  {"day"      , "", 0,  0, 2, 1, 31, formatNumber},
  {"month"    , "", 3,  0, 2, 1, 12, formatNumber},
  {"year"     , "", 6,  0, 2, 0, 99, formatNumber},
  {"week"     , "", 9,  0, 2, 0, 6,  formatWeek},
  {"hour"     , "", 12, 0, 2, 0, 23, formatNumber},
  {"minute"   , "", 15, 0, 2, 0, 59, formatNumber},
  {"second"   , "", 18, 0, 2, 0, 59, formatNumber},
  {"a1Hour"   , "", 4,  2, 2, 0, 23, formatNumber},
  {"a1Minute" , "", 7,  2, 2, 0, 59, formatNumber},
  {"a1Status" , "", 10, 2, 3, 0, 1 , formatAlarmStatus},
  {"a2Hour"   , "", 4,  3, 2, 0, 23, formatNumber},
  {"a2Minute" , "", 7,  3, 2, 0, 59, formatNumber},
  {"a2Status" , "", 10, 3, 3, 0, 1, formatAlarmStatus}
};

enum SystemState {
  IDLE, 
  EDIT_DATETIME
};

#endif