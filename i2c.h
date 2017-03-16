/*************************************************************
名称：I2C.h
版本：V1.0_141117
功能：
说明：
****************************************************************/
#ifndef I2C_H
#define I2C_H

#define nop()	asm("nop")


#define SCL RA4
#define SDA RA5

#define SCL_IO	TRISA4
#define SDA_IO	TRISA5

#define IN		1
#define OUT		0

#define IIC_ADDRESS 55
#define SLAVE 0x6e				//55<<1+1/0(0写，1读)

extern uint8 Ack_Bit;
 

extern void i2c_init(void);
extern void i2c_start(void);
extern void i2c_stop(void);
extern void i2c_ack(uint8 a);
extern void i2c_sendbyte(uint8 write_data);
extern uint8 i2c_recbyte(void);
extern uint8 i2c_write_data(uint8 addr,uint8 *dat,uint8 num);
extern uint8 i2c_read_data(uint8 addr,uint8 *data,uint8 num);

#endif