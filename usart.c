//==============================================================================
/*******************************************************************************
名称：usart.c   
版本：V1.0_131204
功能：通讯处理
说明：
*******************************************************************************/ 
#include "common.h"
//CRC位域8表(小端):
const uint8 Crc8_Table[256] = {
0x00,0x5E,0xBC,0xE2,0x61,0x3F,0xDD,0x83,0xC2,0x9C,0x7E,0x20,0xA3,0xFD,0x1F,0x41,	//0		-		15
0x9D,0xC3,0x21,0x7F,0xFC,0xA2,0x40,0x1E,0x5F,0x01,0xE3,0xBD,0x3E,0x60,0x82,0xDC,	//16	-		31
0x23,0x7D,0x9F,0xC1,0x42,0x1C,0xFE,0xA0,0xE1,0xBF,0x5D,0x03,0x80,0xDE,0x3C,0x62,	//32	-		47
0xBE,0xE0,0x02,0x5C,0xDF,0x81,0x63,0x3D,0x7C,0x22,0xC0,0x9E,0x1D,0x43,0xA1,0xFF,	//48	-		63		
0x46,0x18,0xFA,0xA4,0x27,0x79,0x9B,0xC5,0x84,0xDA,0x38,0x66,0xE5,0xBB,0x59,0x07,	//64	-		79
0xDB,0x85,0x67,0x39,0xBA,0xE4,0x06,0x58,0x19,0x47,0xA5,0xFB,0x78,0x26,0xC4,0x9A,	//80	-		95
0x65,0x3B,0xD9,0x87,0x04,0x5A,0xB8,0xE6,0xA7,0xF9,0x1B,0x45,0xC6,0x98,0x7A,0x24,	//96	-		111
0xF8,0xA6,0x44,0x1A,0x99,0xC7,0x25,0x7B,0x3A,0x64,0x86,0xD8,0x5B,0x05,0xE7,0xB9,	//112	-		127
0x8C,0xD2,0x30,0x6E,0xED,0xB3,0x51,0x0F,0x4E,0x10,0xF2,0xAC,0x2F,0x71,0x93,0xCD,	//128	-		143
0x11,0x4F,0xAD,0xF3,0x70,0x2E,0xCC,0x92,0xD3,0x8D,0x6F,0x31,0xB2,0xEC,0x0E,0x50,	//144	-		159
0xAF,0xF1,0x13,0x4D,0xCE,0x90,0x72,0x2C,0x6D,0x33,0xD1,0x8F,0x0C,0x52,0xB0,0xEE,	//160	-		175
0x32,0x6C,0x8E,0xD0,0x53,0x0D,0xEF,0xB1,0xF0,0xAE,0x4C,0x12,0x91,0xCF,0x2D,0x73,	//176	-		191
0xCA,0x94,0x76,0x28,0xAB,0xF5,0x17,0x49,0x08,0x56,0xB4,0xEA,0x69,0x37,0xD5,0x8B,	//192	-		207
0x57,0x09,0xEB,0xB5,0x36,0x68,0x8A,0xD4,0x95,0xCB,0x29,0x77,0xF4,0xAA,0x48,0x16,	//208	-		223
0xE9,0xB7,0x55,0x0B,0x88,0xD6,0x34,0x6A,0x2B,0x75,0x97,0xC9,0x4A,0x14,0xF6,0xA8,	//224	-		239
0x74,0x2A,0xC8,0x96,0x15,0x4B,0xA9,0xF7,0xB6,0xE8,0x0A,0x54,0xD7,0x89,0x6B,0x35		//240	-		255
};

const int16 Hj_Tem_Num[10] = 
{
    120,    131,    180,    191,    220,    231,    270,    281,    340,    351
    //0		1		2		3		4		5		6       7       8       9
};

uint8 Test_Mode_Data;//测试模式数据

int16 Hj_Tem_Data;
uint8 Hj_Range;



uint8 Receive_Num;
uint8 Receive_Data[RECEIVE_iCNT];
uint8 No_Receive_Time_Counter;

uint8 Send_Data[SEND_iCNT];
uint8 Comm_Check_Err_Counter;
uint8 Comm_Head_Err_Counter;			

//=====================================================================
//校验发送处理
//=====================================================================
//CRC位域8查表算法(小端):
uint8 crc_check(uint8 *data,uint8 num)
{
	uint8 i;
	uint8 crc = 0x00;
	for(i = 0; i < num; i++) 
	{
		crc = Crc8_Table[crc ^ data[i]];
	}	
	return crc;
}

//=====================================================================
//通信处理
//======================================================================
void comm_deal(void)
{
	uint8 num;
	uint8 check_data;
	int_bit rs_data;
	if(1 == Flag_Data.Comm_Enable)//1s处理一次
	{
		Flag_Data.Comm_Enable = 0;	
	
		Send_Data[0] = 0x5A;			
		Send_Data[1] = Lc_Integer_Tem ;		
        Send_Data[2] = Ld_Integer_Tem;
        Send_Data[3] = Ld_Hs_Integer_Tem;
        rs_data.byte.bit0 = Flag_Data.Data_Change;
        rs_data.byte.bit1 = Flag_Data.Hs_Err;
        rs_data.byte.bit2 = Flag_Data.Fan_Err;
        rs_data.byte.bit3 = Flag_Data.Door;
        rs_data.byte.bit4 = Flag_Data.Lc_Tem_Err;
        rs_data.byte.bit5 = Flag_Data.Ld_Tem_Err;
        rs_data.byte.bit6 = Flag_Data.Ld_Hs_Tem_Err;
        rs_data.byte.bit7 = 0;
        Send_Data[4] = rs_data.int_data;		//		
		Send_Data[5] = Data[0];
        Send_Data[6] = Data[1];
        Send_Data[7] = Data[2];
#ifdef TEST_H
        
        Send_Data[8] = Test_Data;
#else
        Send_Data[8] = Test_Mode_Data;
#endif
		Send_Data[9] = crc_check(Send_Data,SEND_iCNT_1);  				
		for(num = 0; num < SEND_iCNT; num++)  
        {  
            while(!TRMT);//发送数据寄存器为非空 等待  
            TXREG = Send_Data[num];  
        }			
		if(No_Receive_Time_Counter < TIME_2MIN)
		{
			No_Receive_Time_Counter++;
		}
		else
		{				
			Flag_Data.Comm_Err = 1;	
            Hj_Range = 3;
		}	
	}
	if(1 == Flag_Data.Receive_End)
	{
		Flag_Data.Receive_End = 0;	
		Flag_Data.Receive_Enable = 0;	
		if(0x5A == Receive_Data[0])
		{				
			check_data = crc_check(Receive_Data,RECEIVE_iCNT_1);
			if(Receive_Data[RECEIVE_iCNT_1] == check_data)
			{
                rs_data.int_data = Receive_Data[3];
                Flag_Data.Short_Time = rs_data.byte.bit0;
                Flag_Data.Hj_Tem_Err = rs_data.byte.bit2;
                if(1 == rs_data.byte.bit1)
                {
                    Data[0] = Receive_Data[4];
                    Data[1] = Receive_Data[5];
                    Data[2] = Receive_Data[6];
                    Data[3] = Receive_Data[7];
                    Data[6] = Data[0]+Data[1]+Data[2]+Data[3]+Data[4]+Data[5]+0xA5;		
                    Flag_Data.Write_Enable = 1;
                }
				if(1 == Flag_Data.Hj_Tem_Err)
				{						
					Hj_Range = 3;
				}
				else
				{
					Hj_Tem_Data = (int16)(Receive_Data[1]*100) + Receive_Data[2];	
					for(num = 0; num < 10; ++num)
				 	{
                        if(Hj_Tem_Data <= Hj_Tem_Num[num])
                        {
                            break;
                        }
					}						
					if(0 == num%2)
					{
						Hj_Range = num/2;
					}
					else
					{
						if(Hj_Range != ((num-1)/2))
						{
							Hj_Range = (num+1)/2;
						}					
					}	
				}					
				Flag_Data.Comm_Err = 0;			
				Comm_Check_Err_Counter = 0;
				Comm_Head_Err_Counter = 0;				
			}
			else
			{				
				Receive_Num  = 0;						
				if(Comm_Check_Err_Counter < 250)
				{
					Comm_Check_Err_Counter++;
				}
				else
				{
					Comm_Check_Err_Counter = 0;
					Flag_Data.Comm_Err = 1;
					Hj_Range = 3;
				}
			}
		}
		else
		{
			Receive_Num  = 0;			
			if(Comm_Head_Err_Counter < 250)
			{
				Comm_Head_Err_Counter++;
			}	
			else
			{
				Comm_Head_Err_Counter = 0;
				Flag_Data.Comm_Err = 1;
				Hj_Range = 3;
			}							
		}
	}	
}
