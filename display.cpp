#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "display.h"
#include "config.h"

// Global variable for lcd
static LiquidCrystal_I2C lcd(0x3f, 20, 4);

void display_init()
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void display_drawclock(int year, int month, int day, int hour, int minute, int second)
{
  char buf[21];
  sprintf(buf,"%04d.%02d.%02d. %02d:%02d:%02d",year,month,day,hour,minute,second);
  lcd.setCursor(0,0);
  lcd.print(buf);
}

void display_showdata(int turnsperday,int turns,int next,bool active,int from, int to, int interval,int direction, bool allowed)
{
	char buf[21];

	if(allowed)sprintf(buf,"TPD %04d/%04d N %04d",turnsperday,turns,next);
	else sprintf(buf,"TPD %04d/%04d N XXXX",turnsperday,turns,next);
  lcd.setCursor(0,1);
  lcd.print(buf);

	if(active) sprintf(buf,"%02d-%02dh %3dm  DIR ACT",from,to,interval);
	else sprintf(buf,"%02d-%02dh %3dm  DIR OFF",from,to,interval);
  lcd.setCursor(0,2);
  lcd.print(buf);

  lcd.setCursor(13,2);
	switch(direction)
	{
		case WD_CW: lcd.print(" CW"); break;
		case WD_CCW: lcd.print("CCW"); break;
		case WD_BOTH: lcd.print("Mix"); break;
	}
}

void display_showmenu(char *buf)
{
	lcd.setCursor(0,3);
	lcd.print(buf);
}

void display_banner(char *buf)
{
	lcd.clear();
	lcd.setCursor(0,2);
	lcd.print(buf);
}

