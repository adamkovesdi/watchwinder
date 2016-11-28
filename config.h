#ifndef _CONFIG_H
#define _CONFIG_H

// alterable configuration
/**************************************************/
// Wind direction constants
#define	WD_CW			0
#define	WD_CCW		1
#define	WD_BOTH		2

extern const uint16_t	values_tpd[];
extern const uint16_t	values_winddirection[];
extern const uint16_t	values_windintervalminutes[];

extern uint16_t config_winddirection;
extern uint16_t config_tpd;
extern uint16_t config_windintervalminutes;
extern uint8_t config_starthour;
extern uint8_t config_endhour;

// value calculation macros
/**************************************************/
#define windinterval	(values_windintervalminutes[config_windintervalminutes] * 60)
#define windhours			(config_endhour - config_starthour)
#define	windevents		(windhours * 3600 / windinterval)
#define turnsperwind  (uint16_t)(((values_tpd[config_tpd] / (float)windevents))+1)
#define tpd						(values_tpd[config_tpd])
#define winddirection	(values_winddirection[config_winddirection])
#define windintervalminutes		(values_windintervalminutes[config_windintervalminutes])

// function definitions 
/**************************************************/
extern void value_inc(uint16_t *index, const uint16_t *array);
extern void value_dec(uint16_t *index, const uint16_t *array);
extern void starthour_inc();
extern void starthour_dec();
extern void endhour_inc();
extern void endhour_dec();

#endif
