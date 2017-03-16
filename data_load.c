/*******************************************************************************    
���ƣ�data_load.c   
�汾��V1.0_090708
���ܣ�����һ����λ����Ҫ�޸İ����������򡢿�������س���
˵����
*******************************************************************************/
#include "common.h"

bool Data_Err_Flag;												//���ݴ����־λ
bool Data_Write_Enable;										//���ݲ�дʹ��λ
bool Data_Read_Enable;   									//��������ʹ��λ

uint8 R_F_Data[UINT8_EEPROM_DATA_NUM];		//R��F�Ĳ���
//0��r1��λ�¶��趨��1��r2�ز��趨��2��F1��λ�¶��趨��3��F2�ز��趨��4��R_Fѹ����ʱ�趨��5��R��λ�趨��6��F��λ�趨��7��R��λ�ϴ��趨��8��F��λ�ϴ��趨��9����У��
uint8 Data_Temp[UINT8_EEPROM_DATA_NUM];		//��ʱ����

uint8 Data_N   = 0;												//���ݼ���
uint8 Data_Sum = 0;  											//��У��
uint8 Data1,Data2;												//����1��2


//int8 R_Data_EE[] = {}
//0���¶��趨ֵ��1����λ��2���ز3����ʱʱ�䣬4����˪���ڣ�5����˪ʱ�䣬6�����������Ͽ�ʱ�䣬7�����������Ϲ�ʱ��
//8����λ1�¶�	9����λ2�¶�	10����λ3�¶ȣ�11����λ4�¶ȣ�12����λ5�¶ȣ�13����λ6�¶ȣ�14����λ7�¶� 

							
//										��Чֵ		1			2			3			4			5			6			7
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
//R	-0-8--	0���¶��趨ֵ��1����λ��2���ز3:�¶�У���� 4����ʱʱ�䣬5�����������Ͽ�ʱ�䣬6�����������Ϲ�ʱ�� 7����˪���ڣ�8����˪ʱ�䣬
12,1,2,0,3,20,10,6,20,
//F	-9-17-	0���¶��趨ֵ��1����λ��2���ز3:�¶�У���� 4����ʱʱ�䣬5�����������Ͽ�ʱ�䣬6�����������Ϲ�ʱ�� 7����˪���ڣ�8����˪ʱ�䣬
-10,1,2,0,3,20,10,6,20,
//R	-18-24-	1����λ1�¶�	2����λ2�¶�	3����λ3�¶ȣ�4����λ4�¶ȣ�5����λ5�¶ȣ�6����λ6�¶ȣ�7����λ7�¶� 
12,9,6,3,0,-3,-6,
//F	-25-31-	1����λ1�¶�	2����λ2�¶�	3����λ3�¶ȣ�4����λ4�¶ȣ�5����λ5�¶ȣ�6����λ6�¶ȣ�7����λ7�¶� 
-10,-12,-13,-14,-16,-18,-20,
//RF	-33-	1��У���


};

int8 R_Data[9];
int8 F_Data[9];
uint8 R_F_Data[UINT8_EEPROM_DATA_NUM];		//R��F�����в���

//R	-0-8--	0���¶��趨ֵ��1����λ��2���ز3:�¶�У���� 4����ʱʱ�䣬5�����������Ͽ�ʱ�䣬6�����������Ϲ�ʱ�� 7����˪���ڣ�8����˪ʱ�䣬
//F	-9-17-	0���¶��趨ֵ��1����λ��2���ز3:�¶�У���� 4����ʱʱ�䣬5�����������Ͽ�ʱ�䣬6�����������Ϲ�ʱ�� 7����˪���ڣ�8����˪ʱ�䣬
//R	-18-24-	1����λ1�¶�	2����λ2�¶�	3����λ3�¶ȣ�4����λ4�¶ȣ�5����λ5�¶ȣ�6����λ6�¶ȣ�7����λ7�¶� 
//F	-25-31-	1����λ1�¶�	2����λ2�¶�	3����λ3�¶ȣ�4����λ4�¶ȣ�5����λ5�¶ȣ�6����λ6�¶ȣ�7����λ7�¶� 
//RF	-33-	1��У���
//=================================================================
//EEPROM �ֽڶ�����
//����: ���ڲ�EEPROM ��һ���ֽ�
//���: EEADR  = ��ַ
//����: EEDATA = ����
//=================================================================
uint8 eeprom_read (uint8 addr)
{
	while ( RD||WR ) 	//�ȴ���һ��д��������
	{
	clr_wdt(); 				//ι��
	}
	EEADR = addr;
	EEPGD = 0 ; 			//���÷���Ŀ��ΪEEPROM
	RD    = 1 ; 			//����һ�ζ�����
	return EEDATA;		
}
//==================================================================
//EEPROM �ֽ�д����
//����: дһ���ֽڵ��ڲ�EEPROM
//���: EEADR  = ��ַ
//      EEDATA = ����
//==================================================================
void eeprom_write (uint8 addr,uint8 value)
{	
	while ( RD||WR ) 	//�ȴ���һ��д��������
	{
	clr_wdt(); 				//ι��
	}
	EEADR  = addr;
	EEDATA = value;
	EEPGD  = 0;				//���÷���Ŀ��ΪEEPROM
	WREN   = 1;				//�������д����
	GIE    = 0; 			//��ֹ�ж�
	EECON2 = 0x55;
	EECON2 = 0xAA;
	WR     = 1; 			//����һ��д����
	GIE    = 1; 			//ʹ���ж�
	WREN   = 0; 			//�ر�д����
}
//==============================================================================
//EEPROM���ݶ�ȡ
//==============================================================================
void data_read(void)    //��ȡEEPROM
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
//EEPROM���ݲ�д
//==============================================================================
void data_write(void) //дEEPROM
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
//���ݻָ�
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
//���ݴ���
//==============================================================================
void data_deal(void)
{
  data_read();	
  data_write();
}