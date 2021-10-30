#include "iic.h"
#include "delay.h"

void iic_init(void)
{
	iic_sda=1;
	iic_scl=1;
}

void iic_start(void)
{
	sda_out();
	iic_sda=1;
	iic_scl=1;
	delay_us(4);
	iic_sda=0;
	delay_us(4);
	iic_scl=0;
}

void iic_stop(void)
{
	sda_out();
	iic_scl=0;
	iic_sda=0;
	delay_us(4);
	iic_scl=1;
	iic_sda=1;
	delay_us(4);
}

u8 iic_wait_ack(void)
{
	u8 wait_time=0;
	sda_in();
	iic_sda=1;
	delay_us(1);
	iic_scl=1;
	delay_us(1);
	while(read_sda)
	{
		wait_time++;
		if(wait_time>250)
		{
			iic_stop();
			return 1;
		}
	}
	iic_scl=0;
	return 0;
}

void iic_ack(void)
{
	iic_scl=0;
	sda_out();
	iic_sda=0;
	delay_us(2);
	iic_scl=1;
	delay_us(2);
	iic_scl=0;
}

void iic_nack(void)
{
	iic_scl=0;
	sda_out();
	iic_sda=1;
	delay_us(2);
	iic_scl=1;
	delay_us(2);
	iic_scl=0;
}

u8 iic_read_byte(u8 ack)
{
	u8 i,receive=0;
	sda_in();
	for(i=0;i<8;i++)
	{
		iic_scl=0;
		delay_us(2);
		iic_scl=1;
		receive<<=1;
		if(read_sda) receive++;
		delay_us (1);
		
	}
	if(!ack) iic_nack();
	else iic_ack();
	return receive;
}


void iic_write_byte(u8 data)
{
	u8 i=0;
	sda_out();
	iic_scl=0;
	for(i=0;i<8;i++)
	{
		iic_sda=(data&0x80)>>7;
		data<<=1;
		delay_us(2);
		iic_scl=1;
		delay_us(2);
		iic_scl=0;
		delay_us(2);
	}
}
