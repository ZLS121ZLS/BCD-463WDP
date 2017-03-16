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

sensor_data Lc_Room_Tem;
sensor_data Ld_Room_Tem;
sensor_data Ld_Hs_Room_Tem;

uint16 Ad_Temp[3];						//AD寄存器		//LC_AD		BW_AD		LD_AD
/*uint8 Ad_Dec[3];							//AD的过采样//
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
*/
void ad_tem_deal (sensor_data (*room_sensor))
{
	int16 tem_data;	
	int8 data_5_temp;
	uint8 num;
	int16 x1;
  int16 data1_temp,data2_temp;
	(*room_sensor).ad_sum += (*room_sensor).ad_temp;
		
	if(--(*room_sensor).ad_dec == 0)
	{
		(*room_sensor).ad_dec = 0;		
		(*room_sensor).ad_sum >>= 2;
		
		
		if((*room_sensor).ad_sum > NTC_AD_MAX || (*room_sensor).ad_sum < NTC_AD_MIN)
		{
			(*room_sensor).err = 1;				//温度故障				
		}
		else
		{				
			for(num = NTC_iMIN; num < NTC_iCNT; ++num)
		 	{
		   	if((*room_sensor).ad_sum <= Ntc_Tbl[num])
		   	{
					break;
		   	}
		 	}	
			Test_Data = num;		
			
			if(0 == num)
			{
				tem_data = -400;
			}       
			else
			{
				x1          = Ntc_Tbl[num-1];
				data1_temp  = (int16)((*room_sensor).ad_sum - x1);
        data2_temp  = (int16)(Ntc_Tbl[num] - x1);	        
        tem_data = data1_temp *10 / data2_temp;
        tem_data = tem_data + (int16)(num*10-10) + NTC_tMIN; 
			}		
			(*room_sensor).room_tem = tem_data + (*room_sensor).tem_adjust;					
			if(tem_data >= 0)
			{
				data_5_temp = 5;
			}
			else
			{
				data_5_temp = -5;
			}
			(*room_sensor).integer_tem = 5;//(int8)((tem_data + data_5_temp)/10);		
			(*room_sensor).err = 0;	//温度正常			
		}
		(*room_sensor).ad_sum= 0;
	}
}

void ad_deal(void)
{	
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
		Lc_Room_Tem.ad_temp = 512;//Ad_Temp[0];		
		ad_tem_deal(&Lc_Room_Tem);		
		//Flag_Data.Lc_Tem_Err = Lc_Room_Tem.err;
		Ad_Num = 0;
		break; 
	case 1:
		Ld_Room_Tem.ad_temp = Ad_Temp[1];
		ad_tem_deal(&Ld_Room_Tem);
		//Flag_Data.Ld_Tem_Err = Ld_Room_Tem.err;
		Ad_Num = 2;
		break;
 	case 2:
		Ld_Hs_Room_Tem.ad_temp = Ad_Temp[2];
		ad_tem_deal(&Ld_Hs_Room_Tem);
		//Flag_Data.Ld_Hs_Tem_Err = Ld_Hs_Room_Tem.err;
		Ad_Num = 0;
		break;
	}	
}

