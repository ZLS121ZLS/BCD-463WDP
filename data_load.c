/*******************************************************************************    
名称：data_load.c   
版本：V1.0_090708
功能：增加一个档位，需要修改按键、主程序、控制中相关程序。
说明：
*******************************************************************************/
#include "common.h"

bool Data_Err_Flag;												//数据错误标志位
bool Data_Write_Enable;										//数据擦写使能位
bool Data_Read_Enable;   									//数据下载使能位

uint8 R_F_Data[UINT8_EEPROM_DATA_NUM];		//R和F的参数
//0：r1档位温度设定；1：r2回差设定；2：F1档位温度设定；3：F2回差设定；4：R_F压机延时设定；5：R档位设定；6：F档位设定；7：R档位上次设定；8：F档位上次设定；9：和校验
uint8 Data_Temp[UINT8_EEPROM_DATA_NUM];		//临时数据

uint8 Data_N   = 0;												//数据计数
uint8 Data_Sum = 0;  											//和校验
uint8 Data1,Data2;												//数据1、2


//int8 R_Data_EE[] = {}
//0：温度设定值，1：档位，2：回差，3：延时时间，4：除霜周期，5：除霜时间，6：传感器故障开时间，7：传感器故障关时间
//8：档位1温度	9：档位2温度	10：档位3温度，11：档位4温度，12：档位5温度，13：档位6温度，14：档位7温度 

							
//										无效值		1			2			3			4			5			6			7
const int8 R1_Dw_Tem[8] = {0,		-6,		-3,		0,		3,		6,		9,		12};
const int8 R2_Dw_Tem[8] = {0,		-6,		-3,		-1,		0,		2,		4,		6};
const int8 R3_Dw_Tem[8] = {0,		0,		1,		2,		3,		4,		5,		6};
const int8 R4_Dw_Tem[8] = {0,		-5,		-3,		-1,		0,		2,		4,		6};
const int8 R5_Dw_Tem[8] = {0,		-4,		-2,		-1,		0,		1,		2,		3};
const int8 R6_Dw_Tem[8] = {0,		0,		2,		4,		5,		6,		8,		10};
const int8 R7_Dw_Tem[8] = {0,		-5,		-2,		0,		2,		5,		7,		10};

const int8 F1_Dw_Tem[8] = {0,		-10,	-12,	-13,	-14,	-16,	-18,	-20};
const int8 F2_Dw_Tem[8] = {0,		-12,	-15,	-17,	-19,	-21,	-22,	-23};
const int8 F3_Dw_Tem[8] = {0,		-6,		-8,		-10,	-12,	-14,	-16,	-18};
const int8 F4_Dw_Tem[8] = {0,		-9,		-10,	-11,	-13,	-14,	-16,	-18};
const int8 F5_Dw_Tem[8] = {0,		-10,	-11,	-12,	-13,	-14,	-15,	-16};
const int8 F6_Dw_Tem[8] = {0,		-6,		-7,		-8,		-9,		-10,	-11,	-12};

const uint8 Data_Backup[UINT8_EEPROM_DATA_NUM] = 
{
//R	-0-8--	0：温度设定值，1：档位，2：回差，3:温度校正； 4：延时时间，5：传感器故障开时间，6：传感器故障关时间 7：除霜周期，8：除霜时间，
12,1,2,0,3,20,10,6,20,
//F	-9-17-	0：温度设定值，1：档位，2：回差，3:温度校正； 4：延时时间，5：传感器故障开时间，6：传感器故障关时间 7：除霜周期，8：除霜时间，
-10,1,2,0,3,20,10,6,20,
//R	-18-24-	1：档位1温度	2：档位2温度	3：档位3温度，4：档位4温度，5：档位5温度，6：档位6温度，7：档位7温度 
12,9,6,3,0,-3,-6,
//F	-25-31-	1：档位1温度	2：档位2温度	3：档位3温度，4：档位4温度，5：档位5温度，6：档位6温度，7：档位7温度 
-10,-12,-13,-14,-16,-18,-20,
//RF	-33-	1：校验和


};

int8 R_Data[9];
int8 F_Data[9];
uint8 R_F_Data[UINT8_EEPROM_DATA_NUM];		//R和F的所有参数

//R	-0-8--	0：温度设定值，1：档位，2：回差，3:温度校正； 4：延时时间，5：传感器故障开时间，6：传感器故障关时间 7：除霜周期，8：除霜时间，
//F	-9-17-	0：温度设定值，1：档位，2：回差，3:温度校正； 4：延时时间，5：传感器故障开时间，6：传感器故障关时间 7：除霜周期，8：除霜时间，
//R	-18-24-	1：档位1温度	2：档位2温度	3：档位3温度，4：档位4温度，5：档位5温度，6：档位6温度，7：档位7温度 
//F	-25-31-	1：档位1温度	2：档位2温度	3：档位3温度，4：档位4温度，5：档位5温度，6：档位6温度，7：档位7温度 
//RF	-33-	1：校验和
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
void eeprom_write (uint8 addr,uint8 value)
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
  uint8 data_i; 
	if(Data_Read_Enable)
  {
    if(Data_N < UINT8_EEPROM_DATA_NUM)
    {
      Data1 = eeprom_read(Data_N);
      Data2 = ~eeprom_read(UINT8_EEPROM_DATA_NUM+Data_N);
      if(Data1 == Data2)
      {
        R_F_Data[Data_N] = Data1;
        if(Data_N++ < UINT8_EEPROM_DATA_NUM_1)
        {
          Data_Sum += Data1;
        }
        else
        {
          if(Data_Sum == Data1)
          {
            Data_Err_Flag  = 0;
						for(data_i=0; data_i<9; data_i++)
						{
							R_Data[data_i] = R_F_Data[data_i];
							F_Data[data_i] = R_F_Data[data_i+9]; 
						}
          }
          else
          {
            Data_Err_Flag  = 1;
          }
          Data_Read_Enable = 0;
					Data_N           = 0;
					Data_Sum         = 0;
        }
      }
      else
      {
        Data_Err_Flag    = 1;
        Data_Read_Enable = 0;
        Data_N           = 0;
				Data_Sum         = 0;
      }      
    }
  }
}


//==============================================================================
//EEPROM数据擦写
//==============================================================================
void data_write(void) //写EEPROM
{
	if(Data_Write_Enable)
  {
    if(Data_N < UINT8_EEPROM_DATA_NUM)
    {
      Data1 = eeprom_read(Data_N);
      Data2 = ~eeprom_read(UINT8_EEPROM_DATA_NUM+Data_N);
      if(Data1 == Data2)
      {
        if(Data1 != R_F_Data[Data_N])
        {
          Data1 = R_F_Data[Data_N];
          eeprom_write(Data_N,Data1);
          eeprom_write(UINT8_EEPROM_DATA_NUM+Data_N,~Data1);
          Data2 = eeprom_read(Data_N);          
        }
      }
      else
      {
        Data1 = R_F_Data[Data_N];
        eeprom_write(Data_N,Data1);
        eeprom_write(UINT8_EEPROM_DATA_NUM+Data_N,~Data1);
        Data2 = eeprom_read(Data_N);                
      }
      if(Data1 == Data2)
      {
        Data2 = ~eeprom_read(UINT8_EEPROM_DATA_NUM+Data_N);
      }
      else
      {
        Data_Err_Flag     = 1;
        Data_Write_Enable = 0;
        Data_N            = 0;
				Data_Sum          = 0;
      }
      if(Data1 == Data2)
      {
        if(Data_N++ < UINT8_EEPROM_DATA_NUM_1)
        {
          Data_Sum += Data1;
        }
        else
        {
          if(Data_Sum == Data1)
          {
            Data_Err_Flag = 0;
          }
          else
          {
            Data_Err_Flag = 1;
          }
          Data_Write_Enable = 0;          
          Data_N            = 0;
					Data_Sum          = 0;
        }
      }
      else
      {
        Data_Err_Flag     = 1;
        Data_Write_Enable = 0;
        Data_N            = 0;
				Data_Sum          = 0;        
      }
    }
    else
    {
      Data_Err_Flag     = 1;
      Data_Write_Enable = 0;
      Data_N            = 0;
			Data_Sum          = 0;
    }
  }	
}
//==============================================================================
//数据恢复
//==============================================================================
void data_resume(void)
{
  uint8 data_i;
  for(data_i = 0; data_i < UINT8_EEPROM_DATA_NUM; data_i++)
  {
    Data_Temp[data_i] = Data_Backup[data_i];
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