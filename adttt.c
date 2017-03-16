/*******************************************************************************
名称：ad.c   
版本：V1.0_131118
功能：石塚热敏电阻
说明：lc温度校正-3
*******************************************************************************/
#include "common.h"

const uint8 Ad_Address[3]=			//AD地址
{
0b00101101,		//AN11		LC_AD
0b00100001,		//AN8			LD_AD
0b00100101		//AN9			LD_HS_AD
};

uint8 Ad_Num;									//AD通道
uint16 Ad_Temp[3];						//AD寄存器		//LC_AD		BW_AD		LD_AD
uint8 Ad_Dec[3];							//AD的过采样//
uint32 Ad_Data_Sum[3];				//AD的数据和//

int8 Lc_Integer_Tem;
int8 Ld_Hs_Integer_Tem;
int8 Ld_Integer_Tem;					//冷冻室温度整数

int16 Lc_Tem_Data;
int16 Ld_Tem_Data;						//冷冻室温度数据
int16 Ld_Hs_Tem_Data;

int16 Lc_Room_Tem;
int16 Ld_Room_Tem;
int16 Ld_Hs_Room_Tem;


int16 ad_tem_deal(uint16 ad_data)
{
	uint8 num;
  int16 x1;
  int16 data1_temp,data2_temp;
	int16 tem_data;
		
	for(num = NTC_iMIN; num < NTC_iCNT; ++num)
	{
	 	if(ad_data <= Ntc_Tbl[num])
		{
			break;
		}
	}	
	if(0 == num)
	{
		tem_data = -400;
	}       
	else
	{
		x1          = Ntc_Tbl[num-1];
		data1_temp  = (int16)(Ad_Data_Sum[0] - x1);
		data2_temp  = (int16)(Ntc_Tbl[num] - x1);	        
		tem_data = data1_temp *10 / data2_temp;
		tem_data = tem_data + (int16)(num*10-10) + NTC_tMIN; 
	}
	return tem_data;
}

void ad_deal(void)
{
	int8 adjust_tem;
	uint8 num;
  int16 x1;
  int16 data1_temp,data2_temp;
  int8 data_5_temp;
	while(GO_nDONE);        
  ADCON0 = Ad_Address[Ad_Num];
  GO_nDONE = 1;                                   //ad启动 
  while(GO_nDONE);
  Ad_Temp[Ad_Num] = ADRESH << 8; 
  Ad_Temp[Ad_Num] = Ad_Temp[Ad_Num] + ADRESL;
  if(2 == Ad_Num)
  {
    ADCON0 = Ad_Address[0];
  }
  else
  {
    ADCON0 = Ad_Address[Ad_Num+1];
  }
  GO_nDONE = 1;                   //ad启动  
	switch(Ad_Num)
	{
 	case 0:	
		Ad_Data_Sum[0] += Ad_Temp[0];
		if(--Ad_Dec[0] == 0)				//--0 = 256；
		{
			Ad_Dec[0] = 0;						//相当于256		
			Ad_Data_Sum[0] >>= 2;	
			Ad_Data_Sum[0] = LC_TEST_15;	
			if(Ad_Data_Sum[0] > NTC_AD_MAX || Ad_Data_Sum[0] < NTC_AD_MIN)
			{
				Flag_Data.Lc_Tem_Err = 1;				//温度故障				
			}
			else
			{				
				Lc_Tem_Data = ad_tem_deal(Ad_Data_Sum[0]);
/*
				switch(Hj_Range)
				{
				case 0:
				case 1:
				case 2:
				case 3:
					adjust_tem = -40;
					break;	
				case 4: 
					adjust_tem = -20;
					break;
				default:
					adjust_tem = 0;
					break;
				}*/
				adjust_tem = -40;
				Lc_Room_Tem = Lc_Tem_Data + adjust_tem;					
				if(Lc_Tem_Data >= 0)
				{
					data_5_temp = 5;
				}
				else
				{
					data_5_temp = -5;
				}
				Lc_Integer_Tem = (Lc_Tem_Data + data_5_temp)/10;		
				Flag_Data.Lc_Tem_Err = 0;	//温度正常			
			}
			Ad_Data_Sum[0]= 0;
		}
		Ad_Num = 1;
		break; 
	case 1:
		Ad_Data_Sum[1] += Ad_Temp[1];
		if(--Ad_Dec[1] == 0)				//--0 = 256；
		{
			Ad_Dec[1] = 0;						//相当于256		
			Ad_Data_Sum[1] >>= 2;		
			if(Ad_Data_Sum[1] > NTC_AD_MAX || Ad_Data_Sum[1] < NTC_AD_MIN)
			{
				Flag_Data.Ld_Tem_Err = 1;			//温度故障
			}
			else
			{	
				Ld_Tem_Data = ad_tem_deal(Ad_Data_Sum[1]);
				/*
				if(Hj_Range <= 3)
				{
					adjust_tem = 10;					
				}
				else
				{
					adjust_tem = 0;
				}
				*/
				adjust_tem = 10;
				Ld_Room_Tem = Ld_Tem_Data+adjust_tem;
				if(Ld_Tem_Data >= 0)
				{
					data_5_temp = 5;
				}
				else
				{
					data_5_temp = -5;
				}
				Ld_Integer_Tem = (Ld_Tem_Data + data_5_temp)/10;
				Flag_Data.Ld_Tem_Err = 0;	//温度正常			
			}
			Ad_Data_Sum[1]= 0;
		}
		Ad_Num = 2;
		break;
 	case 2:
		Ad_Data_Sum[2] += Ad_Temp[2];
		if(--Ad_Dec[2] == 0)				//--0 = 256；
		{
			Ad_Dec[2] = 0;						//相当于256		
			Ad_Data_Sum[2] >>= 2;		
			if(Ad_Data_Sum[2] > NTC_AD_MAX || Ad_Data_Sum[2] < NTC_AD_MIN)
			{
				Flag_Data.Ld_Hs_Tem_Err = 1;			//温度故障
			}
			else
			{				
				Ld_Hs_Tem_Data = ad_tem_deal(Ad_Data_Sum[2]);
				Ld_Hs_Room_Tem = Ld_Hs_Tem_Data;/// + LD_TEM_ADJUST;
				if(Ld_Hs_Tem_Data >= 0)
				{
					data_5_temp = 5;
				}
				else
				{
					data_5_temp = -5;
				}
				Ld_Hs_Integer_Tem = (Ld_Hs_Tem_Data + data_5_temp)/10;			
				Flag_Data.Ld_Hs_Tem_Err = 0;	//温度正常			
			}
			Ad_Data_Sum[2]= 0;
		}
		Ad_Num = 0;
		break;
	}	
}

