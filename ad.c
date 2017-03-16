/*******************************************************************************
名称：ad.c   
版本：V1.0_131118
功能：
说明：
*******************************************************************************/
#include "common.h"

const uint16 Ntc_Tbl[NTC_iCNT]=
{
1218	,	//	-40
1292	,	//	-39
1370	,	//	-38
1451	,	//	-37
1536	,	//	-36
1625	,	//	-35
1718	,	//	-34
1816	,	//	-33
1917	,	//	-32
2023	,	//	-31
2133	,	//	-30
2247	,	//	-29
2366	,	//	-28
2489	,	//	-27
2617	,	//	-26
2749	,	//	-25
2886	,	//	-24
3027	,	//	-23
3173	,	//	-22
3323	,	//	-21
3477	,	//	-20
3636	,	//	-19
3798	,	//	-18
3965	,	//	-17
4136	,	//	-16
4310	,	//	-15
4488	,	//	-14
4669	,	//	-13
4854	,	//	-12
5042	,	//	-11
5232	,	//	-10
5425	,	//	-9
5621	,	//	-8
5818	,	//	-7
6018	,	//	-6
6219	,	//	-5
6422	,	//	-4
6625	,	//	-3
6830	,	//	-2
7035	,	//	-1
7240	,	//	0
7445	,	//	1
7650	,	//	2
7855	,	//	3
8058	,	//	4
8261	,	//	5
8462	,	//	6
8662	,	//	7
8860	,	//	8
9056	,	//	9
9250	,	//	10
9442	,	//	11
9631	,	//	12
9818	,	//	13
10001	,	//	14
10181	,	//	15
10359	,	//	16
10533	,	//	17
10704	,	//	18
10871	,	//	19
11037	,	//	20
11196	,	//	21
11353	,	//	22
11508	,	//	23
11657	,	//	24
11804	,	//	25
11945	,	//	26
12085	,	//	27
12220	,	//	28
12352	,	//	29
12479	,	//	30
12603	,	//	31
12724	,	//	32
12841	,	//	33
12957	,	//	34
13066	,	//	35
13173	,	//	36
13278	,	//	37
13378	,	//	38
13477	,	//	39
13571	,	//	40
};

const int8 Lc_Adjust_Tem[6] = { 20,     20,     30,     40,     50,     50   };//冷藏修正低到高
const int8 Ld_Adjust_Tem[6] = { -10,    -10,    -10,    -10,    -10,    -10  };//冷冻修正低到高
//const int8 Lc_Adjust_Tem[6] = { 20,     0,     20,     40,     50,     50   };
//const int8 Ld_Adjust_Tem[6] = { 0,     0,     0,      -10,    -10,    -10  };

const uint8 Ad_Address[3]=			//AD地址
{
    0b00101101,		//AN11		LC_AD
    0b00100001,		//AN8			LD_AD
    0b00100101		//AN9			LD_HS_AD
};

uint8 Ad_Num;									//AD通道
uint16 Ad_Temp[3];						//AD寄存器		//LC_AD		BW_AD		LD_AD
uint8 Ad_Dec[3];							//AD的过采样//
uint16 Ad_Data_Sum[3];				//AD的数据和//

int8 Lc_Integer_Tem;
int8 Ld_Hs_Integer_Tem;
int8 Ld_Integer_Tem;					//冷冻室温度整数


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
		data1_temp  = (int16)(ad_data - x1)*10;
		data2_temp  = (int16)(Ntc_Tbl[num] - x1);	        
		tem_data = data1_temp / data2_temp;
		tem_data = tem_data + (int16)(num*10-10) + NTC_tMIN; 
	}
	return tem_data;
}

int8 data_4s5r_deal(int16 data)
{
    int8 data_5_temp;
    int8 temp;
    if(data >= 0)
    {
        data_5_temp = 5;
    }
    else
    {
        data_5_temp = -5;
    }
    temp = (data + data_5_temp)/10;	
    return temp;
}
//======================
void ad_deal(void)
{
    int16 tem_data;
    
//	int8 adjust_tem;
	uint16 data_temp;
	while(GO_nDONE);        
    ADCON0 = Ad_Address[Ad_Num];
    GO_nDONE = 1;                                   //ad启动 
    while(GO_nDONE);
    Ad_Temp[Ad_Num] = (uint16)(ADRESH << 8) + ADRESL;         
	switch(Ad_Num)
	{
 	case 0:	
		Ad_Data_Sum[0] += Ad_Temp[0];
		if(++Ad_Dec[0] == 64)				//--0 = 256；
		{		
            Ad_Dec[0] = 0;
			data_temp = (uint16)(Ad_Data_Sum[0] >> 2);
			Ad_Data_Sum[0]= 0;
			if(data_temp > NTC_AD_MAX || data_temp < NTC_AD_MIN)
			{
				Flag_Data.Lc_Tem_Err = 1;				//温度故障				
			}
			else
			{				
				tem_data = ad_tem_deal(data_temp);			
				Lc_Room_Tem = tem_data + Lc_Adjust_Tem[Hj_Range];
				Lc_Integer_Tem = data_4s5r_deal(tem_data);   // + data_5_temp)/10;		
				Flag_Data.Lc_Tem_Err = 0;	//温度正常			
			}			
		}
		Ad_Num = 1;
		break; 
	case 1:
		Ad_Data_Sum[1] += Ad_Temp[1];
		if(++Ad_Dec[1] == 64)				//--0 = 256；
		{
			Ad_Dec[1] = 0;						//相当于256		
			data_temp = (uint16)(Ad_Data_Sum[1] >> 2);
			Ad_Data_Sum[1] = 0;				
			if(data_temp > NTC_AD_MAX || data_temp < NTC_AD_MIN)
			{
				Flag_Data.Ld_Tem_Err = 1;			//温度故障
			}
			else
			{	
				tem_data = ad_tem_deal(data_temp);						
				Ld_Room_Tem = tem_data + Ld_Adjust_Tem[Hj_Range];
				Ld_Integer_Tem = data_4s5r_deal(tem_data);   // + data_5_temp)/10;
				Flag_Data.Ld_Tem_Err = 0;	//温度正常			
			}			
		}
		Ad_Num = 2;
		break;
 	case 2:
		Ad_Data_Sum[2] += Ad_Temp[2];
		if(++Ad_Dec[2] == 64)				//--0 = 256；
		{
			Ad_Dec[2] = 0;						//相当于256		
			data_temp = (uint16)(Ad_Data_Sum[2] >> 2);		
			Ad_Data_Sum[2] = 0;		
			if(data_temp > NTC_AD_MAX || data_temp < NTC_AD_MIN)
			{
				Flag_Data.Ld_Hs_Tem_Err = 1;			//温度故障
			}
			else
			{				
				tem_data = ad_tem_deal(data_temp);
				Ld_Hs_Room_Tem = tem_data;
				Ld_Hs_Integer_Tem = data_4s5r_deal(tem_data);    // + data_5_temp)/10;			
				Flag_Data.Ld_Hs_Tem_Err = 0;	//温度正常			
			}		
		}
		Ad_Num = 0;
		break;
	}
    ADCON0 = Ad_Address[Ad_Num];
    GO_nDONE = 1;  
//#ifdef TEST_H
//    Flag_Data.Lc_Tem_Err = 0;
//    Flag_Data.Ld_Tem_Err = 0;
//    Flag_Data.Ld_Hs_Tem_Err = 0;
//
//#endif
    
}

