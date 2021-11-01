#include "oled.h"
#include "iic.h"
#include "oledfont.h"
#include "delay.h"


void writecmd(u8 command)
{
	iic_start();
	iic_write_byte(0x78);
	iic_ack();
	iic_write_byte(0x00);
	iic_ack();
	iic_write_byte(command);
	iic_ack();
	iic_stop();
}

void writedata(u8 data)
{
	iic_start();
	iic_write_byte(0x78);
	iic_ack();
	iic_write_byte(0x40);
	iic_ack();
	iic_write_byte(data);
	iic_ack();
	iic_stop();
}

void oled_init(void)
{
	delay_ms(1000);
	writecmd(0xAE); //display off
  writecmd(0x20); //Set Memory Addressing Mode    
	writecmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	writecmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
	writecmd(0xc8); //Set COM Output Scan Direction
	writecmd(0x00); //---set low column address
	writecmd(0x10); //---set high column address
	writecmd(0x40); //--set start line address
	writecmd(0x81); //--set contrast control register
	writecmd(0xff); //???? 0x00~0xff
	writecmd(0xa1); //--set segment re-map 0 to 127
	writecmd(0xa6); //--set normal display
	writecmd(0xa8); //--set multiplex ratio(1 to 64)
	writecmd(0x3F); //
	writecmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	writecmd(0xd3); //-set display offset
	writecmd(0x00); //-not offset
	writecmd(0xd5); //--set display clock divide ratio/oscillator frequency
	writecmd(0xf0); //--set divide ratio
	writecmd(0xd9); //--set pre-charge period
	writecmd(0x22); //
	writecmd(0xda); //--set com pins hardware configuration
	writecmd(0x12);
	writecmd(0xdb); //--set vcomh
	writecmd(0x20); //0x20,0.77xVcc
	writecmd(0x8d); //--set DC-DC enable
	writecmd(0x14); //
	writecmd(0xaf); //--turn on oled panel
	oled_clear();
}

void oled_on(void)
{
	writecmd(0x8d);
	writecmd(0X14);
	writecmd(0XAF);
}

void oled_off(void)
{
	writecmd(0x8d);
	writecmd(0X10);
	writecmd(0XAE);
}

u8 OLED_GRAM[128][8];

void oled_refreshram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		writecmd (0xb0+i);    //设置页地址（0~7）
		writecmd (0x00);      //设置显示位置—列低地址
		writecmd (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++) writedata(OLED_GRAM[n][i]); 
	}
}

void oled_setpos(u8 x,u8 y,u8 i)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;
	pos=(y)/8;
	bx=y%8;
	temp=1<<bx;
	if(i) OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;
}

void oled_showchar(u8 x,u8 y,u8 chr,u8 size)
{
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);
	chr=chr-' ';
	for(t=0;t<csize;t++)
  {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
    for(t1=0;t1<8;t1++)
		{
			if(temp&0x80) oled_setpos(x,y,1);
			else oled_setpos(x,y,0);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
  }
}

void oled_showstr(u8 x,u8 y,const u8 *p,u8 size)
{
	while((*p<='~')&&(*p>=' '))
	{       
		if(x>(128-(size/2))){x=0;y+=size;}
		if(y>(64-size)){y=x=0;oled_clear();}
		oled_showchar(x,y,*p,size);	 
		x+=size/2;
		p++;
	} 
}

void oled_chinese(u8 x,u8 y,u8 num)
{
	u8 t,t1,tem1,size=16;
	u16 y0=y;
	for(t=0;t<32;t++)
	{
		tem1=hanzi_1616[num][t];
		for(t1=0;t1<8;t1++)
		{			    
			if(tem1&0x80) oled_setpos(x,y,1);
			tem1<<=1;
			y++;
			if(y>=64)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=128)return;	//超区域了
				break;
			}
		}
	}
}



void oled_fill(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 x,y;
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++) oled_setpos(x,y,1);
	}
	oled_refreshram();
}

void oled_clear()
{
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++) OLED_GRAM[n][i]=0X00;  
	oled_refreshram();
}

void oled_printf(unsigned char* strings, u8 rows)
{
	oled_clear();
	oled_showstr(1,(rows-1)*12-1,strings,12);
	oled_refreshram();
}
