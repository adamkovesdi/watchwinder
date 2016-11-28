#ifndef _DISPLAY_H
#define _DISPLAY_H

void display_init();
void display_drawclock(int year, int month, int day, int hour, int minute, int second);
void display_showdata(int turnsperday,int turns,int next,bool active,int from, int to, int interval,int direction, bool allowed);
void display_showmenu(char *buf);
void display_banner(char *buf);

#endif
