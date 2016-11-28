// helpers, etc.

#include <stdint.h>
#include "config.h"

const uint16_t	values_tpd [] = { 0x5, 500, 650, 800, 950, 1300 };
const uint16_t	values_winddirection [] = { 0x3, WD_CW, WD_CCW, WD_BOTH };
const uint16_t	values_windintervalminutes [] = { 0x7, 5, 10, 15, 20, 30, 45, 60 };

// Wind direction
uint8_t config_winddirection = 3;
// Turns Per Day
uint8_t config_tpd = 2;
// Winding interval in minutes
uint8_t config_windintervalminutes = 4;
// Starting hour -> if hour >= this then wind
uint8_t config_starthour = 9;
// Ending hour -> if hour < this then wind
uint8_t config_endhour = 23;

void value_inc(uint8_t *index, const uint16_t *array)
{
	uint16_t length=*array;
	if((*index+1)>length) *index=1;
	else *index+=1;
}

void value_dec(uint8_t *index, const uint16_t *array)
{
	uint16_t length=*array;
	if((*index-1)<1) *index=length;
	else *index-=1;
}

void starthour_inc()
{
	// if((config_starthour>=config_endhour-1) || (config_starthour>=23) ) return;		// non rollover version
	if((config_starthour>=config_endhour-1) || (config_starthour>=23) ) config_starthour=0;		// rollover version
	else config_starthour++;
}

void starthour_dec()
{
	// if(config_starthour==0) return;		// non rollover version
	if(config_starthour==0) config_starthour=config_endhour-1;		// rollover version
	else config_starthour--;
}

void endhour_inc()
{
	// if(config_endhour>=23) return;		// non rollover version
	if(config_endhour>=23) config_endhour=config_starthour+1;		// rollover version
	else config_endhour++;
}

void endhour_dec()
{
	// if((config_endhour<=1)||(config_starthour+1>=config_endhour)) return;		// non rollover version
	if((config_endhour<=1)||(config_starthour+1>=config_endhour)) config_endhour=23;		// rollover version
	else config_endhour--;
}


