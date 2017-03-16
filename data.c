/*******************************************************************************    
名称：data.c   
日期：13.03.14
备注：
*******************************************************************************/
#include "common.h"


int8 Data[7];			//所有参数
uint8 Data_Temp;
uint8 Data_N;
uint8 Data_Sum = 0xA5;

//=================================================================
//EEPROM 字节读程序
//功能: 从内部EEPROM 读一个字节
//入口: EEADR  = 地址
//出口: EEDATA = 数据
//=================================================================
uint8 eeprom_read (uint8 addr)
{
	while ( RD||WR ) 	//等待上一次写操作结束
	{
		clr_wdt(); 				//喂狗
	}
	EEADR = addr;
	EEPGD = 0 ; 			//设置访问目标为EEPROM
	RD    = 1 ; 			//启动一次读操作
	return EEDATA;		
}
//==================================================================
//EEPROM 字节写程序
//功能: 写一个字节到内部EEPROM
//入口: EEADR  = 地址
//      EEDATA = 数据
//==================================================================
void eeprom_w (uint8 addr,uint8 value)
{	
	while ( RD||WR ) 	//等待上一次写操作结束
	{
		clr_wdt(); 				//喂狗
	}
	EEADR  = addr;
	EEDATA = value;
	EEPGD  = 0;				//设置访问目标为EEPROM
	WREN   = 1;				//允许进行写操作
	GIE    = 0; 			//禁止中断
	EECON2 = 0x55;
	EECON2 = 0xAA;
	WR     = 1; 			//启动一次写操作
	GIE    = 1; 			//使能中断
	WREN   = 0; 			//关闭写操作
}
//==============================================================================
//EEPROM数据读取
//==============================================================================
void data_read(void)    //读取EEPROM
{ 
	if(1 == Flag_Data.Read_Enable)
    {
        if(Data_N < 7)
        {
            Data_Temp    = eeprom_read(Data_N);
            Data[Data_N] = Data_Temp;		
            if(Data_N++ < 6)
		 	{
		 		Data_Sum += Data_Temp;
		 	}
			else
		 	{					
				if(Data_Sum != Data_Temp)
				{
					Data[0] = 4;
					Data[1] = -18;
					Data[2] = 2;
					Data[3] = 0;				//耗能模式标记	
					Data[4] = 0;
					Data[5] = 0;
					Data[6] = 153;					
				}			
				//Flag_Data.Jn_Test = Data[4];
				if(SET_MS_DATA == SD_MS)
				{
					Data[6] -= SD_MS;
					SET_MS_DATA = 0;
				} 
				else if(SET_MS_DATA == SL_MS)
				{
					Data[6] -= SL_MS; 
					SET_MS_DATA = 0;
				} 
				Data_Sum          = 0XA5;
				Data_N            = 0;
				Flag_Data.Read_Enable  = 0;				
		  } 
  	}
  }
}
//==============================================================================
//EEPROM数据擦写
//==============================================================================
void data_write(void) //写EEPROM
{
	if(1 == Flag_Data.Write_Enable)
    {	
        if(Data_N < 7)
        {
            Data_Temp = eeprom_read(Data_N);
            if(Data_Temp != Data[Data_N])
            {
                Data_Temp = Data[Data_N];
                eeprom_w(Data_N,Data_Temp);  
            }  
            if(Data_N++ < 6)
            {
                ;
            }
            else
            {	
                Flag_Data.Data_Change = 1;	
                Data_Sum          = 0XA5;
                Data_N            = 0;
                Flag_Data.Write_Enable = 0;	
            }      	
        }   
    }	
}
//==============================================================================
//数据处理
//==============================================================================
void data_deal(void)
{
  data_read();
  data_write();
}