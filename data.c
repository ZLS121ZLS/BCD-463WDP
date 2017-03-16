/*******************************************************************************    
���ƣ�data.c   
���ڣ�13.03.14
��ע��
*******************************************************************************/
#include "common.h"


int8 Data[7];			//���в���
uint8 Data_Temp;
uint8 Data_N;
uint8 Data_Sum = 0xA5;

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
void eeprom_w (uint8 addr,uint8 value)
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
					Data[3] = 0;				//����ģʽ���	
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
//EEPROM���ݲ�д
//==============================================================================
void data_write(void) //дEEPROM
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
//���ݴ���
//==============================================================================
void data_deal(void)
{
  data_read();
  data_write();
}