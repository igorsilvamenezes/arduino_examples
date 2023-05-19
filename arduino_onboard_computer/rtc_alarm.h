#ifndef RTC_ALARM_H
#define RTC_ALARM_H

typedef struct {
  time_t alarm1;
  bool alarm1Status;
  time_t alarm2;
  bool alarm2Status;
} alarm_t;

#endif