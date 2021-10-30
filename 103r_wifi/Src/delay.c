#include "delay.h"

void systick_delayinit(void)
{
	SysTick->CTRL|=0X04;
	SysTick->LOAD=1800-1;
	SysTick->CTRL|=0x05;
}

void delay_us(u32 nus)
{
	int total_time=0,last_val=0,latest_val=0;
	last_val=SysTick->VAL;
	while(total_time<(71*nus))
	{
		latest_val=SysTick->VAL;
		if(last_val>latest_val)	total_time+=last_val-latest_val;
		else total_time+=last_val+1799-latest_val;
		last_val=latest_val;
	}
}

//void delay_us(u32 nus)
//{
//	unsigned short last_val=0,latest_val=0;
//	int total_time=0;
//	int temp=0;
//	last_val=SysTick->VAL;
//	while(total_time<(71*nus))
//	{
//		latest_val=SysTick->VAL;
//		if(latest_val<last_val)
//		{
//			temp=last_val-latest_val;
//			total_time+=temp;
//		}
//		else 
//		{
//			temp=1799-latest_val+last_val;
//			total_time+=temp;
//		}
//		last_val=latest_val;
//	}
//}
void delay_ms(u32 nms)
{
	for(int i=0;i<nms;i++)
	{
		delay_us(1000);
	}
}