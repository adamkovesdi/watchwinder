#include <Arduino.h>
#include <RTClib.h>
#include "clock.h"

static RTC_DS1307 rtc;

void clock_init()
{
  if (! rtc.begin()) {
    while (1);
  }
}

unsigned long clock_getunixtime()
{
	DateTime now=rtc.now();
	return now.unixtime();
}

int clock_getyear() { DateTime now=rtc.now(); return now.year(); }
int clock_getmonth() { DateTime now=rtc.now(); return now.month(); }
int clock_getday() { DateTime now=rtc.now(); return now.day(); }
int clock_gethour() { DateTime now=rtc.now(); return now.hour(); }
int clock_getminute() { DateTime now=rtc.now(); return now.minute(); }
int clock_getsecond() { DateTime now=rtc.now(); return now.second(); }

