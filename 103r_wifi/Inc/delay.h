#ifndef DELAY_H
#define DELAY_H
#include "main.h"

void systick_delayinit(void);
void delay_us(u32 nus);
void delay_ms(u32 nms);

#endif
