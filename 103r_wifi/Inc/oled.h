#ifndef OLED_H
#define OLED_H
#include "main.h"

void writecmd(u8 command);
void writedata(u8 data);
void oled_init(void);
void oled_on(void);
void oled_off(void);
void oled_setpos(u8 x,u8 y,u8 i);
void oled_showchar(u8 x,u8 y,u8 chr,u8 size);
void oled_showstr(u8 x,u8 y,const u8 *p,u8 size);
void oled_chinese(u8 x,u8 y,u8 num);
void oled_refreshram(void);
void oled_fill(u8 x1,u8 y1,u8 x2,u8 y2);
void oled_clear(void);


#endif
