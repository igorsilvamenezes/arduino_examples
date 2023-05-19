#include <LiquidCrystal_I2C.h>

// Define the I2C address of the LCd display
#define LCD_ADDRESS 0x27

#define MASK_LINE_0 "  /  /        :  :  "
#define MASK_LINE_1 "                    "
#define MASK_LINE_2 "A1:   :             "
#define MASK_LINE_3 "A2:   :             "

#define DEGREE_SYMBOL "\xDF"
#define CELSIUS_SYMBOL "C"

// Initialize the LCD display object
LiquidCrystal_I2C lcd( LCD_ADDRESS, 20, 4);

// Initialize the LCD display
void init_display(){
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(MASK_LINE_0);
  lcd.setCursor(0, 1);
  lcd.print(MASK_LINE_1);
  lcd.setCursor(0, 2);
  lcd.print(MASK_LINE_2);
  lcd.setCursor(0, 3);
  lcd.print(MASK_LINE_3);
}

void showDateTime(datetime_t *datetime){
  String tmpData;
  
  if(currentState == IDLE){
    encodeDate(datetime);
  }

  for(int i = 0; i < NUM_FIELDS -6; i++){
    tmpData = fields[i].value;

    if(fields[i].format != NULL){
      tmpData = fields[i].format(tmpData);
    }

    lcd.setCursor(fields[i].column, fields[i].line);
    lcd.print(tmpData);
  }
}

void encodeDate(datetime_t *datetime){
  // Format the date as a string
  fields[0].value = datetime->date.day;
  fields[1].value = datetime->date.month;
  fields[2].value = datetime->date.year;
  fields[3].value = datetime->date.week;
  fields[4].value = datetime->time.hour;
  fields[5].value = datetime->time.minute;
  fields[6].value = datetime->time.second;
}

datetime_t decodeDate(){
  datetime_t dtResult;  
  dtResult.date.day    = fields[0].value.toInt();
  dtResult.date.month  = fields[1].value.toInt();
  dtResult.date.year   = fields[2].value.toInt();
  dtResult.date.week   = fields[3].value.toInt();
  dtResult.time.hour   = fields[4].value.toInt();
  dtResult.time.minute = fields[5].value.toInt();
  dtResult.time.second = fields[6].value.toInt();

  return dtResult;
}

void encodeAlarms(alarm_t *alarms){
  // Format the alarm as a string
  fields[7].value = alarms->alarm1.hour;
  fields[8].value = alarms->alarm1.minute;
  fields[9].value = alarms->alarm1Status;
  fields[10].value = alarms->alarm1.hour;
  fields[11].value = alarms->alarm1.minute;
  fields[12].value = alarms->alarm1Status;  
}

String formatNumber(String value){
  unsigned int valueNun = value.toInt();

  return String(valueNun < 10 ? "0" : "") + String(valueNun, DEC);
}

String formatWeek(String week){
  byte weekNun = 0;
  String weekStr = "";

  weekNun = week.toInt();

  switch(weekNun) {
    case 0: weekStr = "Su"; break;
    case 1: weekStr = "Mo"; break;
    case 2: weekStr = "Tu"; break;
    case 3: weekStr = "We"; break;
    case 4: weekStr = "Th"; break;
    case 5: weekStr = "Fr"; break;
    case 6: weekStr = "Sa";  
  }

  return weekStr;
}

String formatAlarmStatus(String value){
  return String(value.equals("1") ? "ON " : "OFF" );;
}


void blinkSelectedValue() {
  String tmpData;

  if(selectedField != NULL){
    lcd.setCursor(selectedField->column, selectedField->line);

    tmpData = selectedField->value;
    if(selectedField->format != NULL){
      tmpData = selectedField->format(tmpData);
    }    

    if (millis() % 1000 < 500) {
      lcd.print("  ");
    } else {
      lcd.print(tmpData);
    }
  }
}

void showTemperature(float temperature){
  String temperatureStr = String(temperature, 1) + DEGREE_SYMBOL + CELSIUS_SYMBOL;

  // Display the temperature on LCD module
  lcd.setCursor(0, 1);
  lcd.print(temperatureStr);
}

//TODO: Unificar essa função com a ShowDatetime
void showAlarms(alarm_t *alarm){
  String tmpData;

  for(int i = NUM_FIELDS -7; i < NUM_FIELDS; i++){
    tmpData = fields[i].value;

    if(fields[i].format != NULL){
      tmpData = fields[i].format(tmpData);
    }

    lcd.setCursor(fields[i].column, fields[i].line);
    lcd.print(tmpData);
  }
}