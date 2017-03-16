/*************************************************************
���ƣ�I2C.c
�汾��V1.0_141117
���ܣ�
˵����
****************************************************************/
#include "common.h"

uint8 Ack_Bit;
 

//===================================================
//��ʱ
//===================================================
void delay(void)
{
nop();
nop();
nop();
}
//================================================
//i2c��ʼ��
//=================================================
void i2c_init(void)		
{
	SCL_IO = OUT;
	SDA_IO = OUT;
  SCL = 1;
  SDA = 1;
}
//=================================================
//i2c_start��ʼ
//=================================================
void i2c_start(void)
{	
	SDA_IO = OUT;
  SDA = 1; 
  delay();
  SCL = 1;
  delay();
  SDA = 0; 
  delay();
  SCL = 0;
  delay();
}
//==================================================
//i2c_stopֹͣ
//===================================================
void i2c_stop(void)
{
	SDA_IO = OUT;
  SDA = 0; 
  delay(); 
  SCL = 1; 
  delay();
  SDA = 1; 
  delay();
}
void i2c_ack(uint8 a)
{  
	SDA_IO = OUT;
  if(0 == a)
	{
		SDA = 0;
	} 
  else 
	{
		SDA = 1;
	}
  delay();
  SCL = 1;
  delay();
  SCL = 0; 
  delay();
}
//======================================================
//i2c��������
//=======================================================
void i2c_sendbyte(uint8 write_data)//дһ�ֽ�
{
	uint8 i;	
	SDA_IO = OUT;
	for(i=0;i<8;i++)
	{	
		if((write_data<<i)&0x80)
		{
			SDA = 1;
		}
		else
		{
		 	SDA = 0;
		}
		delay();
		SCL = 1;
		delay();
		SCL = 0;		
	}
	delay();
	SDA_IO = IN;
	delay();
	SCL = 1;	
	delay();
	if(1 == SDA)
	{
		Ack_Bit = 0;		
	}
	else
	{
		Ack_Bit = 1;	
	}		
	SCL = 0;
	delay();	
}
//=============================================================
//i2c��������
//==============================================================
uint8 i2c_recbyte(void)//��һ�ֽ�
{
	uint8 i,read_data = 0;
	SDA_IO = IN; 
	for(i=0;i<8;i++)
	{
	
		delay();
		SCL = 0;
		delay();
		SCL = 1;
		delay();
		read_data=  read_data << 1;
		if( SDA == 1 )
		{
			read_data = read_data+1;
		}	
		delay();
	}
	SCL = 0;
	delay();
	return(read_data);
}
 //==========================================================
//i2cд����
//==========================================================
uint8 i2c_write_data(uint8 addr,uint8 *data,uint8 num)//���ֽ���д������
{
	uint8 i;
	uint8 j;
	i2c_start();
	i2c_sendbyte(SLAVE);	
	if(0 == Ack_Bit)
	{
		for(j=0;j<10;j++)
		{
			i2c_start();
			i2c_sendbyte(SLAVE);
			if(1 == Ack_Bit)
			{
				break;
			}
		}			
	}	
	i2c_sendbyte(addr);
	if(0 == Ack_Bit)
	{
		return(0);
	}	
	for(i=0; i<num; i++)
  {  
		i2c_sendbyte(*data);
		if(0 == Ack_Bit)
		{
			return(0);
		}
		data++;
	}
	i2c_stop();
	return(1);
} 
//=======================================================
//i2c������
//========================================================
uint8 i2c_read_data(uint8 addr,uint8 *data,uint8 num)
{
	uint8 i;
	uint8 j;
	i2c_start();
	i2c_sendbyte(SLAVE);
	i2c_sendbyte(addr);
	if(0 == Ack_Bit)
	{
		for(j=0;j<10;j++)
		{
			i2c_start();
			i2c_sendbyte(SLAVE);
			i2c_sendbyte(addr);
			if(1 == Ack_Bit)
			{
				break;
			}
		}			
	}
	i2c_start();
	i2c_sendbyte(SLAVE+1);
	if(0 == Ack_Bit)
	{
		return(0);
	}
	for(i=0;i<num-1;i++)
  {   
		*data = i2c_recbyte();
		i2c_ack(0);
		data++;
	}
	*data = i2c_recbyte();
	i2c_ack(1);
	i2c_stop();
	return(1);
}
