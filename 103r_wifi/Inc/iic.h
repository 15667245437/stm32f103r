#ifndef IIC_H
#define IIC_H
#include "main.h"


#define sda_in() {GPIOB->CRL&=(0X0FFFFFFF);GPIOB->CRL|=((u32)8<<28);}
#define sda_out() {GPIOB->CRL&=(0X0FFFFFFF);GPIOB->CRL|=((u32)3<<28);}

#define iic_scl PBout(6)
#define iic_sda PBout(7)
#define read_sda PBin(7)


void iic_init(void);
void iic_start(void);
void iic_stop(void);
void iic_write_byte(u8 data);
u8 iic_read_byte(u8 ack);
u8 iic_wait_ack(void);
void iic_ack(void);
void iic_nack(void);

#endif
