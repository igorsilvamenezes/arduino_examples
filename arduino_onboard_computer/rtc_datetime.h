#ifndef RTC_DATETIME_H
#define RTC_DATETIME_H

typedef struct{
  unsigned int week;
  unsigned int day;
  unsigned int month;
  unsigned int year;
} date_t;

typedef struct {
  unsigned int second;
  unsigned int minute;
  unsigned int hour;
} time_t;

typedef struct {
  date_t date;
  time_t time;
} datetime_t;

#endif