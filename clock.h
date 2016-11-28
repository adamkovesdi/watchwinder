#ifndef _CLOCK_H
#define _CLOCK_H

void clock_init();
unsigned long clock_getunixtime();
int clock_getyear();
int clock_getmonth();
int clock_getday();
int clock_gethour();
int clock_getminute();
int clock_getsecond();

#endif
