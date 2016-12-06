// Watch winder (c) 2016 by Adam Kovesdi - all rights reserved
#include <Arduino.h>
#include <EEPROM.h>
#include <Pushbutton.h>
#include <Stepper.h>
#include "config.h"
#include "display.h"
#include "clock.h"

#define MENUTIMEOUT			10000
#define REDRAWINTERVAL	100

// configure buttons
Pushbutton enter(8);
Pushbutton select(9);

// configure according to motor
#define MOTORSTEPS		50
#define MOTORSPEED		40
Stepper windmotor(MOTORSTEPS, 10, 11, 6, 5);

unsigned long lastwind = 0;
unsigned long	nextwind = 0;
unsigned int	windstoday = 0;
unsigned int	lastwindday = 0;

unsigned long lastrefresh = 0;
unsigned long lastmenu_draw = 0;

int configmode = 0;
bool winderactive = true;

bool allowedtowind()
{
	// these prohibit winding
  if(!winderactive) return 0;
	if(clock_gethour()<config_starthour) return 0;
	if(clock_gethour()>=config_endhour) return 0;
	if(windstoday>=tpd) return 0;
	return 1;
}

void windhandler()
{
	lastwind = clock_getunixtime();
  if(!allowedtowind()) return;
	char buf[21];
  sprintf(buf, "....winding time....");
  display_banner(buf);
  delay(500);
  // TODO: proper watch winding algorithm
	int i;
	for(i=0;i<turnsperwind;i++)
	{
		windmotor.step(MOTORSTEPS);
		windstoday++;
		if(!allowedtowind()) return;
	}
}

void saveconfig()
{
  char buf[21];
	EEPROM.update(0,0xab);
	EEPROM.update(1,config_tpd);
	EEPROM.update(2,config_windintervalminutes);
	EEPROM.update(3,config_winddirection);
	EEPROM.update(4,config_starthour);
	EEPROM.update(5,config_endhour);
  sprintf(buf, "....config saved....");
  configmode = 0;
  display_banner(buf);
  delay(1000);
}

void draw_confmenu()
{
  lastmenu_draw = millis();
  char buf[21];
  sprintf(buf, "                    ");
  display_showmenu(buf);
  switch (configmode)
  {
    case 0: sprintf(buf, "                    ", tpd); break;
    case 7: configmode = 1;
    case 1: sprintf(buf, "CFG TPD       [%4d]", tpd); break;
    case 2: sprintf(buf, "CFG interval  [%4d]", windintervalminutes); break;
		// TODO: display winding direction
    case 3: sprintf(buf, "CFG direction [%4s]", "Mix"); break;
    case 4: sprintf(buf, "CFG from hour [%4d]", config_starthour); break;
    case 5: sprintf(buf, "CFG to hour   [%4d]", config_endhour); break;
    case 6: sprintf(buf, "Save config   [ENTR]", config_endhour); break;
  }
  display_showmenu(buf);
}

void enterpress()
{
  switch (configmode)
  {
    case 0: winderactive = !winderactive; break;
    case 1: value_inc(&config_tpd, values_tpd);	break;
    case 2: value_inc(&config_windintervalminutes, values_windintervalminutes);	break;
		// TODO: config menu for changing wind direction
    case 3: break; 
    case 4: starthour_inc();			break;
    case 5: endhour_inc(); break;
    case 6: saveconfig(); break;
  }
  draw_confmenu();
}

void selectpress()
{
  configmode++;
  draw_confmenu();
}

void calculate()
{
	if (lastwindday != clock_getday())
	{
		// day change (or first run)
		windstoday = 0;
		lastwindday = clock_getday();
	}
	nextwind = lastwind + windinterval;
}

void setup()
{
  display_init();
  clock_init();
  lastwind = clock_getunixtime();
  // TODO: read config from eeprom
	if(EEPROM.read(0)!=0xab) return;	// no previous config
	config_tpd=EEPROM.read(1);
	config_windintervalminutes=EEPROM.read(2);
	config_winddirection=EEPROM.read(3);
	config_starthour=EEPROM.read(4);
	config_endhour=EEPROM.read(5);
	windmotor.setSpeed(MOTORSPEED);
}

void draw_screen()
{
  display_drawclock(clock_getyear(), clock_getmonth(), clock_getday(), clock_gethour(), clock_getminute(), clock_getsecond());
	display_showdata(tpd, windstoday, nextwind - clock_getunixtime(), winderactive, config_starthour, config_endhour, windintervalminutes, winddirection, allowedtowind());
  lastrefresh = millis();
}

void loop()
{
	/*
		millis takes 49+_days to rollover (and so what if it does?) = 2^32mS * 1sec/1000mS * 1min/60sec * 1hr/60min * 1day/24 hrs
		micros takes some number of hours to rollover = 2^32mS * 1sec/1000000uS * 1min/60sec * 1hr/60min * 1day/24 hrs

		if all your time calculations are done as:
		if  ((later_time - earlier_time ) >=duration ) {action}
		then the rollover does generally not come into play.
	*/
	calculate();
  if (enter.getSingleDebouncedPress()) enterpress();
  if (select.getSingleDebouncedPress()) selectpress();
	if (clock_getunixtime() >= nextwind) windhandler();
  if ((millis() - lastmenu_draw) >= MENUTIMEOUT) {
    configmode = 0;
    draw_confmenu();
  }
  if ((millis() - lastrefresh) >= REDRAWINTERVAL) {
    draw_screen();
  }
	// if (clock_getunixtime() %10==0) wind();		// this is a test
}

