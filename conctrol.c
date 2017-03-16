/*******************************************************************************
名称：conctrol.c   
版本：V1.0_131119
功能：
说明：首次运行更改，改正，修改冷藏补偿控制。
*******************************************************************************/
#include "common.h"

const uint8 Light_Duty_Data[153] = //{0,2,8,17,29,45,63,82,103,125,147,168,188,205,221,233,242,248,250};
{
    10	,	//	0
    11	,	//	1
    11	,	//	2
    11	,	//	3
    12	,	//	4
    12	,	//	5
    12	,	//	6
    13	,	//	7
    13	,	//	8
    14	,	//	9
    14	,	//	10
    15	,	//	11
    15	,	//	12
    16	,	//	13
    17	,	//	14
    18	,	//	15
    18	,	//	16
    19	,	//	17
    20	,	//	18
    21	,	//	19
    22	,	//	20
    23	,	//	21
    24	,	//	22
    25	,	//	23
    26	,	//	24
    27	,	//	25
    28	,	//	26
    29	,	//	27
    30	,	//	28
    31	,	//	29
    33	,	//	30
    34	,	//	31
    35	,	//	32
    36	,	//	33
    38	,	//	34
    39	,	//	35
    41	,	//	36
    42	,	//	37
    44	,	//	38
    45	,	//	39
    47	,	//	40
    48	,	//	41
    50	,	//	42
    51	,	//	43
    53	,	//	44
    55	,	//	45
    56	,	//	46
    58	,	//	47
    60	,	//	48
    62	,	//	49
    63	,	//	50
    65	,	//	51
    67	,	//	52
    69	,	//	53
    71	,	//	54
    73	,	//	55
    74	,	//	56
    76	,	//	57
    78	,	//	58
    80	,	//	59
    82	,	//	60
    84	,	//	61
    86	,	//	62
    88	,	//	63
    90	,	//	64
    92	,	//	65
    94	,	//	66
    96	,	//	67
    98	,	//	68
    101	,	//	69
    103	,	//	70
    105	,	//	71
    107	,	//	72
    109	,	//	73
    111	,	//	74
    113	,	//	75
    115	,	//	76
    118	,	//	77
    120	,	//	78
    122	,	//	79
    124	,	//	80
    126	,	//	81
    128	,	//	82
    131	,	//	83
    133	,	//	84
    135	,	//	85
    137	,	//	86
    139	,	//	87
    142	,	//	88
    144	,	//	89
    146	,	//	90
    148	,	//	91
    150	,	//	92
    152	,	//	93
    155	,	//	94
    157	,	//	95
    159	,	//	96
    161	,	//	97
    163	,	//	98
    165	,	//	99
    167	,	//	100
    169	,	//	101
    172	,	//	102
    174	,	//	103
    176	,	//	104
    178	,	//	105
    180	,	//	106
    182	,	//	107
    184	,	//	108
    186	,	//	109
    188	,	//	110
    190	,	//	111
    192	,	//	112
    194	,	//	113
    196	,	//	114
    198	,	//	115
    199	,	//	116
    201	,	//	117
    203	,	//	118
    205	,	//	119
    207	,	//	120
    208	,	//	121
    210	,	//	122
    212	,	//	123
    214	,	//	124
    215	,	//	125
    217	,	//	126
    219	,	//	127
    220	,	//	128
    222	,	//	129
    223	,	//	130
    225	,	//	131
    226	,	//	132
    228	,	//	133
    229	,	//	134
    231	,	//	135
    232	,	//	136
    234	,	//	137
    235	,	//	138
    236	,	//	139
    237	,	//	140
    239	,	//	141
    240	,	//	142
    241	,	//	143
    242	,	//	144
    243	,	//	145
    244	,	//	146
    245	,	//	147
    246	,	//	148
    247	,	//	149
    248	,	//	150
    249	,	//	151
    250		//	152
};

//  40	45	50	55	60	65  70	75	80 85 90 95	100	105 110	115 120	125 130	135 140 145 150
//	194,173,155,141,129,119,111,103,97,91,86,81,77, 73,	70,	67, 64,	62, 59,	57, 55, 53, 51 
//  97,	86,	77,	70,	64,	59, 55,	51,	48,45,43,40,38, 36, 35, 33,	32,	31, 29, 28, 27, 26, 25
const uint8 Comp_Cycle_Data[9] = 
{
  //45      55      65      75      80      90      100     115     125
    173,    141,    119,    103,    97,     86,     77,     67,     62
};
const uint8 Comp_Duty_Data[9] = 
{
 // 45      55      65      75      80      90      100     115     125
    86,     70,     59,     51,     48,     43,     38,     33,     31
};
//通常运转
const uint8 First_Freq[6][2][2] =
{
    FREQ_S1,    FREQ_S1,        FREQ_S1,    FREQ_S1,
    FREQ_S1,    FREQ_S1,        FREQ_S0,    FREQ_S0,
    FREQ_S4,    FREQ_S4,        FREQ_S4,    FREQ_S4,
    FREQ_S4,    FREQ_S4,        FREQ_S4,    FREQ_S4,
    FREQ_S6,    FREQ_S6,        FREQ_S3,    FREQ_S3,
    FREQ_S8,    FREQ_S8,        FREQ_S8,    FREQ_S8
};
//化霜后首次运转
const uint8 Def_Freq[6][2][2] =
{
    FREQ_S7,    FREQ_S7,        FREQ_S7,    FREQ_S7,
    FREQ_S7,    FREQ_S7,        FREQ_S0,    FREQ_S0,
    FREQ_S7,    FREQ_S7,        FREQ_S7,    FREQ_S7,
    FREQ_S7,    FREQ_S7,        FREQ_S7,    FREQ_S7,
    FREQ_S8,    FREQ_S8,        FREQ_S3,    FREQ_S3,
    FREQ_S8,    FREQ_S8,        FREQ_S7,    FREQ_S7
};
//升降档时间
const uint8 Updata_Time[9][6] = 
{
    TIME_90SEC, TIME_90SEC, TIME_90SEC, TIME_90SEC, TIME_90SEC, TIME_90SEC,
    TIME_90SEC, TIME_90SEC, TIME_60SEC, TIME_60SEC, TIME_90SEC, TIME_90SEC,
    TIME_90SEC, TIME_90SEC, TIME_60SEC, TIME_60SEC, TIME_120SEC,TIME_90SEC,
    TIME_90SEC, TIME_90SEC, TIME_60SEC, TIME_60SEC, TIME_120SEC,TIME_90SEC,
    TIME_60SEC, TIME_60SEC, TIME_60SEC, TIME_60SEC, TIME_60SEC, TIME_60SEC,
    TIME_60SEC, TIME_60SEC, TIME_60SEC, TIME_60SEC, TIME_60SEC, TIME_60SEC
};
//加热器开时间
const uint8 Db_Heat_On_Time[6] = 
{   TIME_2SEC,  TIME_3SEC,  TIME_4SEC,  TIME_5SEC,  TIME_6SEC,  TIME_8SEC};
//加热器关时间
const uint8 Db_Heat_Off_Time[6] = 
{   TIME_8SEC,  TIME_7SEC,  TIME_6SEC,  TIME_5SEC,  TIME_4SEC,  TIME_2SEC};

int16 Comp_Move_Tem;
int16 Comp_Stop_Tem;

int16 Ref_Move_Tem;
int16 Ref_Stop_Tem;

uint8 Fan_Run_Err_Num;
uint8 Fan_Run_Num;
uint8 Fan_Stop_Err_Num;

uint8 Light_Duty;
uint8 Door_Open_Time_Counter;

uint8 Comp_Freq_Data;
uint16 Power_On_Time_Counter;

uint8 Fan_3sec_Counter;								//风机3秒
uint8 No_Fan_3sec_Counter;

uint8 Def_Prepare_Time_Counter;			//除霜准备时间计时

uint16 Door_Open_10min_Counter;
uint16 Def_Time_Counter;

uint16 Err_No_Comp_Time_Counter;
uint16 Err_Comp_Time_Counter;
uint16 Err_No_Fan_Door_Time_Counter;
uint16 Err_Fan_Door_Time_Counter;

uint8 Fan_Door_Delay_Time_Counter;	//风门延时计时
uint8 Fan_Door_Delay_Time;					//风门延时

uint16 Fan_Delay_Time;

uint16 No_Comp_Time_Counter;					//压缩机关机时间
uint16 No_Comp_Time;									//压缩机开机延时
uint8 Comp_Stop_15sec_Counter;
uint8 Comp_Move_30sec_Counter;				//压缩机运行30S
uint8 Comp_Move_1min_Counter;
uint16 Comp_Continue_Time_Counter;	//压缩机连续工作时间
uint16 Comp_Move_Time_Counter;			//压缩机累计运行时阿计数
uint16 Comp_Move_Time;							//压缩机累计运行时阿

uint8 Step_State;
uint16 Step_Open_Num;
uint16 Step_Close_Num;

uint8 Quick_Fre_1min_Counter;
uint8 Quick_Ref_1min_Counter;
uint16 Quick_Fre_Time_Counter;			//速冻时间计时
uint16 Quick_Ref_Time_Counter;			//速冻时间计时

uint8 Comp_Move_3min_Counter;

uint8 Comp_Freq_Updata_Time_Counter;
uint8 Comp_Freq_Updata_Time;
uint16 Fan_Delay_Time_Counter;


int16 Tem_Differ;
int8 Freq_Add_Num;
uint8 Time_Fan_Door_Counter;
uint8 Db_Heat_Off_Time_Counter;
uint8 Db_Heat_On_Time_Counter;
uint16 Freq_Time_1sec_Counter;
uint16 Fan_Door_No_Act_Time_Counter;
uint16 Fb_Freq_Counter;
uint8 Fan_Test_Counter;
uint8 Time_1_Data;
uint16 Fan_Err_Time_Counter;
uint8 Door_Open_Num;//开门次数统计
uint8 All_Door_Open_Time;
uint8 Jn_1min_Counter;
uint16 Jn_24hour_Counter;
uint16 Jn_Def_Time;
uint16 Jn_Def_Time_Counter;
uint8 Jn_Def_Num;
uint8 Hj_Range_Old;
int16 Set_Lc_Tem;
int16 Set_Ld_Tem;
void light_deal(void)//渐亮处理，呼吸灯效果
{
    if(1 == Flag_Data.Light_Enable)
    {
        Flag_Data.Light_Enable = 0;
        if(1 == Flag_Data.Light)
        {	
            if(Light_Duty < 152)
            {
                Light_Duty++;
            }
            TRIS_LIGHT = 0;	
        }
        else
        {	
            Light_Duty = 0;
            TRIS_LIGHT = 1;
        }
        CCPR_LIGHT = Light_Duty_Data[Light_Duty];
    }
}

void door_key_deal(void)//门按键处理
{
    if(1 == Flag_Data.Normal_Work)
    {
        if(1 == Flag_Data.Door_Key_Clock)
        {
            Flag_Data.Door_Key_Clock = 0;
            //----------------------------------------------------------
            //门开关判断
            //----------------------------------------------------
            Flag_Data.Door_Temp = PORT_DOOR;	
            if(Flag_Data.Door_Temp == Flag_Data.Door_Old)
            {
                if(1 == Flag_Data.Door_Temp)//温度标记?
                {
                    if(0 == Flag_Data.Door)
                    {
                        Flag_Data.Door = 1;
                        Door_Open_Num++;
                    }
                    if(1 == Flag_Data.Light_Off)
                    {
                        Flag_Data.Light = 0;
                    }
                    else
                    {
                        Flag_Data.Light = 1;
                    }
                }
                else
                {	
                    Flag_Data.Door = 0;
                    Flag_Data.Light = 0;
                    Flag_Data.Light_Off = 0;
                }
            }
            Flag_Data.Door_Old = Flag_Data.Door_Temp; 
            if(0 == Flag_Data.Lc_Tem_Err && 0 == Flag_Data.Ld_Tem_Err && 0 == Flag_Data.Ld_Hs_Tem_Err 
                    && 0 == Flag_Data.Hs_Err && 0 == Flag_Data.Fan_Err && 0 == Flag_Data.Comm_Err)		//
            {             
                if(0 == PORT_KEY)
                {
                    Flag_Data.Key_Temp = 1;	//
                }
                else
                {
                    Flag_Data.Key_Temp = 0;
                }
                if(Flag_Data.Key_Temp == Flag_Data.Key_Old)
                {
                    if(1 == Flag_Data.Key_Temp)
                    {
                        if(0 == Flag_Data.Key)
                        {
                            Flag_Data.Key = 1;
                            if(Test_Mode_Data < 3)
                            {			
                                Test_Mode_Data++;	
                                Freq_Time_1sec_Counter = 0;
                            }		
                        }
                    }
                    else
                    {
                        Flag_Data.Key = 0;
                    }
                }
                Flag_Data.Key_Old = Flag_Data.Key_Temp;
            }
            else
            {
                if(Test_Mode_Data > 0)
                {
                    Test_Mode_Data = 3;
                }
            }            
        }
    }
}
void door_deal(void)        //1SEC基准
{
    //-----------------------------------------------
    //门开关动作
    //-------------------------------------------------
    if(1 == Flag_Data.Door)
    {
        if(Door_Open_Time_Counter < TIME_80SEC)
        {
            Door_Open_Time_Counter++;
        }	
        else
        {
            Comp_Move_Time = TIME_12HOUR;            
        }
        if(Door_Open_Time_Counter >= 70)//大于70秒
        {
            Flag_Data.Door_Open_70sec = 1;
        }
        if(All_Door_Open_Time < 80) //80秒
        {
            All_Door_Open_Time++;
        }
        if(Door_Open_10min_Counter < TIME_10MIN)
        {            
            Door_Open_10min_Counter = Door_Open_10min_Counter + Time_1_Data;	           
        }
        else
        {	
            Flag_Data.Light_Off = 1;
        }								
    }
    else
    {
        Door_Open_10min_Counter = 0;
        if(Door_Open_Time_Counter < TIME_80SEC && Comp_Move_Time > TIME_12HOUR)
        {
            Comp_Move_Time = Comp_Move_Time - Door_Open_Time_Counter*10;
        }
        Door_Open_Time_Counter = 0;
    }
}
//=========================================
//风门处理
//==========================================
void fan_door_step_deal(void)
{
	if(1 == Flag_Data.Fan_Door_Clock)	//3ms
	{
		Flag_Data.Fan_Door_Clock = 0;
        if(1 == Flag_Data.Fan_Door_Open_Enable)
        {
            Step_Close_Num = 0;
            Fan_Door_No_Act_Time_Counter = 0;	
            if(++Step_Open_Num > 3600)//2000)
            {
                Step_Open_Num = 0;
                Flag_Data.Fan_Door_Open_Enable = 0;					
            }
            else
            {
                if(Step_Open_Num > 3400)//1800)
                {
                    Step_State = Step_Open_Num%4;						
                }
                else
                {													
                    Step_State = 3 - Step_Open_Num%4;
                }
            }				
            PORT_C = 1;
        }	
        else if(1 == Flag_Data.Fan_Door_Close_Enable)
        {
            Step_Open_Num = 0;
            Fan_Door_No_Act_Time_Counter = 0;				
            if(++Step_Close_Num > 1850)
            {
                Step_Close_Num = 0;
                Flag_Data.Fan_Door_Close_Enable = 0;					
            }
            Step_State = Step_Close_Num%4;
            PORT_C = 1;
        }
        else
        {
            PORT_C = 0;
            Step_State = 0;
        }			
        switch(Step_State)
        {
        case 0:			
            PORT_A = 0;
            PORT_B = 0;		
            break;
        case 1:			
            PORT_A = 1;
            PORT_B = 0;		
            break;
        case 2:			
            PORT_A = 1;
            PORT_B = 1;			
            break;
        case 3:			
            PORT_A = 0;
            PORT_B = 1;				
            break;
        }	
    }
}

//==============================================
//控制点确定
//===============================================
void set_tem_deal(void)
{
    int8 lc_set_tem;
    int8 ld_set_tem;
//    int16 tem_data;
	switch(SET_MS_DATA)
	{
	case ZN_MS:
		lc_set_tem = 4;
		ld_set_tem = -18;
		break;
	case JN_MS:
		lc_set_tem = 8;
		ld_set_tem = -18;
		break;
	default:
		if(0 == Flag_Data.Comm_Err)
		{
            if(SL_MS == SET_MS_DATA)
			{
				lc_set_tem = 2;
			}
            else if(9 == SET_LC_TEM)
            {
                lc_set_tem = 90;
            }
			else
			{
				lc_set_tem = SET_LC_TEM;                
			}          
            ld_set_tem = SET_LD_TEM;             
		}
		else
		{
			lc_set_tem = 5;
			ld_set_tem = -18;
		}
		break;
	} 	    
    Set_Lc_Tem = lc_set_tem*10;
    Ref_Move_Tem = Set_Lc_Tem + REF_UP_RETURN;
    Ref_Stop_Tem = Set_Lc_Tem + REF_DOWN_RETURN;
    if(1 == Flag_Data.Def_Prepare)		//	
    {
      ld_set_tem -= 3;       
    }	
    Set_Ld_Tem = ld_set_tem*10;
    Comp_Move_Tem = Set_Ld_Tem + COMP_UP_RETURN;
    Comp_Stop_Tem = Set_Ld_Tem + COMP_DOWN_RETURN;   
    if(0 == Flag_Data.Short_Time)
    {
        Time_1_Data = 1;
    }
    else
    {
        Time_1_Data = 60;
    }
}
//=============================================
//压缩机启动
//=============================================
void comp_start_deal(void)
{
	if(1 == Flag_Data.No_Comp_Time_End)
	{	
		Flag_Data.No_Comp_Time_End = 0;				
		Flag_Data.Comp = 1;							
	}					
}
//=============================================
//压缩机关闭
//============================================= 
//void comp_close_deal(void)      // 可以省略
//{
//	if(1 == Flag_Data.Comp)
//	{
//		Flag_Data.Comp = 0;
//	}
//}
//=============================================
//风门打开
//=============================================
void fandoor_open_deal(void)
{
	if(0 == Flag_Data.Fan_Door)
	{
		if(0 == Flag_Data.Fan_Door_Open_Enable && 0 == Flag_Data.Fan_Door_Close_Enable)
		{				
			Flag_Data.Fan_Door = 1;
			Flag_Data.Fan_Door_State = 1;
			Flag_Data.Fan_Door_Open_Enable = 1;				
		}		
	}		
	else
	{
		Flag_Data.Fan_Door_State = 1;
	}		
}
//=============================================
//风门关闭
//=============================================
void fandoor_close_deal(void)
{
	if(1 == Flag_Data.Fan_Door)
	{
		if(0 == Flag_Data.Fan_Door_Open_Enable && 0 == Flag_Data.Fan_Door_Close_Enable)
		{				
			Flag_Data.Fan_Door = 0;
			Flag_Data.Fan_Door_State = 0;
			Flag_Data.Fan_Door_Close_Enable = 1;				
		}		
	}	
	else
	{
		Flag_Data.Fan_Door_State = 0;
	}				
}
//=================================================
//化霜启动
//====================================================
void def_begain_deal(void)
{
    if(SD_MS != SET_MS_DATA)
    {       
       Flag_Data.Def = 1;        
    }
}
//==============================================
//制冷（压缩机）压机、风门的处理,除霜
//================================================
void comp_fandoor_def_deal(void)        //1秒
{	
	if(0 == Flag_Data.Def)
	{			
		//---------------------------------------------
		//非除霜时压缩机的处理
		//--------------------------------------------
		if(0 == Flag_Data.Ld_Tem_Err)//传感器故障
		{	
			if(SD_MS != SET_MS_DATA)
			{				              
				if(Ld_Room_Tem >= Comp_Move_Tem) 			//传感器温度达到开机温度
				{											
					comp_start_deal();
				}
				else if(Ld_Room_Tem <= Comp_Stop_Tem)
				{
                    if(1 == Flag_Data.Comp )
                    {
                        Flag_Data.Comp = 0;	
                        Flag_Data.First_Comp_End = 1;
                        if( Comp_Continue_Time_Counter > MOVE_TIME_120MIN)//压缩机连续工作时间
                        {
                            if(Comp_Freq_Data > FREQ_S6)
                            {
                                Flag_Data.Forst_Freq_S6 = 1;
                            }
                        }
                    }
				}							
			}
			else		//速冻模式
			{
				comp_start_deal();
                if(Quick_Fre_1min_Counter < TIME_1MIN && 0 == Flag_Data.Short_Time)
                {
                    Quick_Fre_1min_Counter++;
                }
                else
                {
                    Quick_Fre_1min_Counter = 0;
                    if(Quick_Fre_Time_Counter < TIME_12HOUR)
                    {
                        Quick_Fre_Time_Counter++;
                    }
                    else
                    {
                        Quick_Fre_Time_Counter = 0;
                        Data[6] = Data[6] - Data[2];
                        Data[2] = 0;
                        Flag_Data.Write_Enable = 1;	
                    } 
                }
			}
            if(SL_MS == SET_MS_DATA)　　//速冷模式
            {
                if(Quick_Ref_1min_Counter < TIME_1MIN && 0 == Flag_Data.Short_Time)
                {
                    Quick_Ref_1min_Counter++;
                }
                else
                {
                    Quick_Ref_1min_Counter = 0;
                    if(Quick_Ref_Time_Counter < TIME_5HOUR)
                    {
                        Quick_Ref_Time_Counter++;
                    }
                    else
                    {
                        Quick_Ref_Time_Counter = 0;
                        Data[6] = Data[6] - Data[2];
                        Data[2] = 0;
                        Flag_Data.Write_Enable = 1;	
                    }
                }
            }
		}
        else        //冷冻传感器故障
        {    
            if(0 == Flag_Data.Comp)
            {
                Err_Comp_Time_Counter = 0;
                if(Err_No_Comp_Time_Counter < TIME_30MIN)		//30分钟
                {
                    Err_No_Comp_Time_Counter = Err_No_Comp_Time_Counter + Time_1_Data;
                }
                else
                {                   
                    Flag_Data.Comp = 1;                    
                }
            }
            else
            {
                Err_No_Comp_Time_Counter = 0;
                if(Err_Comp_Time_Counter < TIME_30MIN)		//30分钟
                {
                    Err_Comp_Time_Counter = Err_Comp_Time_Counter + Time_1_Data;                   
                }
                else
                {
                    Flag_Data.Comp = 0;
                }
            }
        }
		//------------------------------------------------------
		//非除霜时风门的处理
		//--------------------------------------------------------
		if(0 == Flag_Data.Lc_Tem_Err)//传感器故障
		{	
			if(Lc_Room_Tem >= Ref_Move_Tem) 		//冷藏传感器温度达到开机温度
			{	
				if(0 == Flag_Data.Fan_Door_State)
				{
					if(1 == Flag_Data.Fan_Door_Delay_Time_End)
					{
						Flag_Data.Fan_Door_Delay_Time_End = 0;			
						fandoor_open_deal();
					}	
				}
				else
				{
					fandoor_open_deal();
				}									
			}
			else 
			{
				if(Lc_Room_Tem <= Ref_Stop_Tem)
				{		
					fandoor_close_deal();						
				}					
				else
				{ 
					if(1 == Flag_Data.Fan_Door)
					{
						if(0 == Flag_Data.Fan_Door_State) 
						{						
							fandoor_close_deal();								
						}
					}
					else
					{
						if(1 == Flag_Data.Fan_Door_State)
						{
							fandoor_open_deal();
						}
					}
				}
			}										
		}	
        else
        {
            if(0 == Flag_Data.Fan_Door_State)
            {
                Err_Fan_Door_Time_Counter = 0;
                if(Err_No_Fan_Door_Time_Counter < TIME_30MIN)		//30分钟
                { 
                    Err_No_Fan_Door_Time_Counter = Err_No_Fan_Door_Time_Counter + Time_1_Data;                   
                }
                else
                {
                    fandoor_open_deal();	
                }
            }
            else
            {
                Err_No_Fan_Door_Time_Counter = 0;
                if(Err_Fan_Door_Time_Counter < TIME_15MIN)		//15分钟
                {
                    Err_Fan_Door_Time_Counter = Err_Fan_Door_Time_Counter + Time_1_Data;                   
                }
                else
                {
                    fandoor_close_deal();
                }
            }
        }
        Def_Time_Counter = 0;
        Flag_Data.Def_Out = 0;
        if(Ld_Hs_Room_Tem >= DEF_END_TEM && 0 == Flag_Data.Ld_Hs_Tem_Err)
        {
            Flag_Data.Hs_Err = 0;
        }				
	}		
	else
	{ 
        Fan_Delay_Time = TIME_14MIN;		//16分钟 
        Fan_Door_Delay_Time = TIME_14MIN;		//16分钟 
        No_Comp_Time = TIME_8MIN; 
        Comp_Move_Time = TIME_50HOUR;//50小时 32*60  
        Quick_Fre_Time_Counter = 0;
        No_Comp_Time_Counter = 0;       
        Def_Prepare_Time_Counter = 0;
        Fan_Delay_Time_Counter = 0;       
        Fan_Door_Delay_Time_Counter = 0; 
        Comp_Move_Time_Counter = 0;
        Comp_Move_1min_Counter = 0;
        Comp_Continue_Time_Counter = 0;	
        Jn_Def_Time_Counter = 0;
        Flag_Data.Def_Freq_State = 1;
        Flag_Data.No_Comp_Time_End  = 0; 
        Flag_Data.Def_Prepare = 0;
        Flag_Data.Fan_Delay_Time_End = 0;	 
        Flag_Data.Fan_Door_Delay_Time_End = 0;   
        Flag_Data.First_Comp_End = 1;
        Flag_Data.Comp = 0;	  
        //No_Def_Time = 100;	
        if(Def_Time_Counter < TIME_120MIN)		//120分化霜时间钟
        {
            Def_Time_Counter = Def_Time_Counter + Time_1_Data;           
        }
        else
        {
            Flag_Data.Hs_Err = 1;
            Flag_Data.Def = 0;
            if(2 == Test_Mode_Data)	
            {
                Test_Mode_Data = 0;
            } 
        }	
        if(0 == Flag_Data.Ld_Hs_Tem_Err)
        {
            if(Ld_Hs_Room_Tem >= DEF_END_TEM )
            {
                Flag_Data.Def = 0;
                Flag_Data.Hs_Err = 0;
                if(2 == Test_Mode_Data)	
                {
                    Test_Mode_Data = 0;
                }
            } 
            else
            {
                Flag_Data.Def_Out = 1;
            }
        }
        fandoor_close_deal();					
	}
    //---------------------------------------------------------
    //压机处理
    //---------------------------------------------------------
    if(0 == Flag_Data.Comp)     //1SEC
    {
        Comp_Move_3min_Counter = 0;
        Freq_Add_Num = 0;
        Comp_Continue_Time_Counter = 0;		//连续运行清零
        Comp_Move_30sec_Counter = 0;	    
        if(Comp_Stop_15sec_Counter < TIME_15SEC)
        {
            Comp_Stop_15sec_Counter++;
        }	
        if(No_Comp_Time_Counter < No_Comp_Time)
        {
            No_Comp_Time_Counter = No_Comp_Time_Counter + Time_1_Data;            
        }
        else
        {
            Flag_Data.No_Comp_Time_End = 1;
        }	
        if(Comp_Move_Time_Counter >= Comp_Move_Time)
        {
            if(0 == Flag_Data.Jn_Test)
            {
                Flag_Data.Def_Prepare = 1;	
            }
        }
        if(1 == Flag_Data.Def_Prepare)		//	
        {	
            def_begain_deal();
        }				
    }
    else
    {        
        No_Comp_Time_Counter = 0;
        Flag_Data.No_Comp_Time_End = 0;	
        Comp_Stop_15sec_Counter = 0;
        No_Comp_Time = TIME_10MIN;
        if(Comp_Move_30sec_Counter < TIME_30SEC)
        {	
            Comp_Move_30sec_Counter++;	
        }	
        if(Comp_Move_1min_Counter < TIME_1MIN && 0 == Flag_Data.Short_Time)
        {
            Comp_Move_1min_Counter++;
        }
        else
        {
            Comp_Move_1min_Counter = 0;
            if(Comp_Continue_Time_Counter < TIME_24HOUR)	//压机连续工作24小时BCD-366改成10小时
            {           
                Comp_Continue_Time_Counter++;
            }
            else
            {
                 if(0 == Flag_Data.Jn_Test)
                {
                    Flag_Data.Def_Prepare = 1;	
                }
            } 
            if(Comp_Move_Time_Counter < Comp_Move_Time)
            {
                Comp_Move_Time_Counter++;
            }
            else
            {
                if(0 == Flag_Data.Jn_Test)
                {
                    Flag_Data.Def_Prepare = 1;	
                }
            }
            //----------------------------------------------
            //准备除霜处理
            //----------------------------------
            if(1 == Flag_Data.Def_Prepare)	//准备化霜
            {
                if(Def_Prepare_Time_Counter < PREPARE_TIME_30MIN)
                {
                    Def_Prepare_Time_Counter++;
                }
                else
                {
                    def_begain_deal();
                }
            }
        }      
    }
    //------------------------
    //风门处理
    //--------------------------------
    if(0 == Flag_Data.Fan_Door_State)
    {
        if(Fan_Door_Delay_Time_Counter < Fan_Door_Delay_Time)
        {
            Fan_Door_Delay_Time_Counter = Fan_Door_Delay_Time_Counter + Time_1_Data;            
        }
        else
        {
            Fan_Door_Delay_Time = 0;	
            Fan_Door_Delay_Time_Counter = 0;	
            Flag_Data.Fan_Door_Delay_Time_End = 1;
        }	
    }
    else
    {
        Fan_Door_Delay_Time_Counter = 0;
    }
    //-----------------------------------------------------------
    //风门连续45分钟无动作
    //-------------------------------------------------------------			
    if(0 == Flag_Data.Fan_Door_Open_Enable && 0 == Flag_Data.Fan_Door_Close_Enable)
    {
        if(Fan_Door_No_Act_Time_Counter < TIME_45MIN)//45
        {
            Fan_Door_No_Act_Time_Counter = Fan_Door_No_Act_Time_Counter + Time_1_Data;           
        }
        else
        {
            Fan_Door_No_Act_Time_Counter = 0;	
            if(0 == Flag_Data.Fan_Door)
            {	
                Flag_Data.Fan_Door = 1;		
                Flag_Data.Fan_Door_Open_Enable = 1;			
            }
            else
            {
                Flag_Data.Fan_Door = 0;
                Flag_Data.Fan_Door_Close_Enable = 1 ;	
            }
        }
    }						
}


void fan_fb_deal(void)
{
    if(1 == Flag_Data.Power_On_End)
    {
    if(Freq_Time_1sec_Counter < 1000)		//1秒
    {		
        Freq_Time_1sec_Counter++;	
        Flag_Data.Fan_Fb = PORT_FAN_FB;	
        if(0 == Flag_Data.Fan_Fb_Old)
        {
            if(1 == Flag_Data.Fan_Fb)
            {
                Fb_Freq_Counter++;	
            }
        }	
        Flag_Data.Fan_Fb_Old = Flag_Data.Fan_Fb;
    }	
    else
    {       
        Freq_Time_1sec_Counter = 0;
        if(1 == Flag_Data.Fan)
        {
            No_Fan_3sec_Counter = 0;	
            
            if(Fan_3sec_Counter < TIME_3SEC)		//6*500ms = 3S
            {
                Fan_3sec_Counter++;
            }
            else
            {
                
                if(Fb_Freq_Counter <= 32)
                {
                    if(Fan_Run_Err_Num < 5)
                    {
                        Fan_Run_Err_Num++;
                    }
                    else
                    {				
                        Fan_Run_Num = 0;
                        Flag_Data.Fan_Err_Temp = 1;
                    }
                }			
                else
                {
                    if(Fan_Run_Num < 5)
                    {
                        Fan_Run_Num++;
                    }
                    else
                    {				
                        Fan_Run_Err_Num = 0;
                        Flag_Data.Fan_Err_Temp = 0;
                    }
                }
            }
        }
        else
        {
            Fan_3sec_Counter = 0;	
            if(No_Fan_3sec_Counter < TIME_3SEC)
            {
                No_Fan_3sec_Counter++;
            }
            else
            {
                if(Fb_Freq_Counter >= 100)
                {
                    if(Fan_Stop_Err_Num < 5)
                    {			
                        Fan_Stop_Err_Num++;	
                    }	
                    else
                    {
                        Flag_Data.Fan_Err_Temp = 1;
                    }
                }			
                else
                {
                    Fan_Stop_Err_Num = 0;
                }
            }
        }   
        Fb_Freq_Counter = 0;
    }    
}
}
void fan_begain_deal(void)
{
    if(1 == Flag_Data.Fan_Delay_Time_End)
    {
        Flag_Data.Fan_Delay_Time_End = 0;		
        Flag_Data.Fan = 1;
    }
}
//===================================
//冷冻室风机
//===================================
void ld_fan_deal(void)
{
	if(0 == Flag_Data.Fan_Test)
	{
		if(1 == Flag_Data.Comp)
		{
			if(Comp_Move_30sec_Counter >= TIME_30SEC)			
			{
				fan_begain_deal();
			}
		}
		else
		{
			if(0 == Flag_Data.Def)
			{
				if(Comp_Stop_15sec_Counter < TIME_15SEC)
				{
					fan_begain_deal();
				}
				else
				{  
					Flag_Data.Fan = Flag_Data.Fan_Door_State;	
				}
			}
			else
			{
				Flag_Data.Fan = 0;
			}	
		}
		if(1 == Flag_Data.Door || 1 == Flag_Data.Fan_Err)
		{
			Flag_Data.Fan = 0;
		}
        if(1 == Flag_Data.Fan_Err_Temp && 0 == 	Flag_Data.Fan_Err)
        {
            Flag_Data.Fan_Err_Temp = 0;	
            Flag_Data.Fan_Test = 1;	
        }
        if(0 == Flag_Data.Fan)
        {
            if(Fan_Delay_Time_Counter < Fan_Delay_Time)
            {
                Fan_Delay_Time_Counter = Fan_Delay_Time_Counter + Time_1_Data;              
            }
            else
            {
                Fan_Delay_Time = 0;
                Fan_Delay_Time_Counter = 0;	
                Flag_Data.Fan_Delay_Time_End = 1;
            } 
        }	            
	}	
    else
    { 
        if(++Fan_Test_Counter <= 15)
        {
            Flag_Data.Fan = 1;
        }
        else if(Fan_Test_Counter <= 30)
        {
            Flag_Data.Fan = 0;	
        }
        else if(Fan_Test_Counter <= 45)
        {
            Flag_Data.Fan = 1;	
        }
        else if(Fan_Test_Counter <= 60)
        {
            Flag_Data.Fan = 0;	
        }
        else if(Fan_Test_Counter <= 75)
        {
            Flag_Data.Fan = 1;	
        }	
        else
        {
            Fan_Test_Counter = 0;
            Flag_Data.Fan_Test = 0;
            if(1 == Flag_Data.Fan_Err_Temp)
            {
                Flag_Data.Fan_Err_Temp = 0;	
                Flag_Data.Fan_Err = 1;  //风机故障
            }	
            else
            {
                Flag_Data.Fan_Err = 0; 
            }      	
		}
	}
    //------------------------------------
    //风机故障
    //----------------------------------
    if(1 == Flag_Data.Fan_Err)
    {
        if(Fan_Err_Time_Counter < FAN_ERR_3HOUR)	//1S
        {
            Fan_Err_Time_Counter = Fan_Err_Time_Counter + Time_1_Data;
        }
        else
        {
            Fan_Err_Time_Counter = 0;
            Flag_Data.Fan_Test = 1;
        }
    }
	if(1 == Flag_Data.Fan)
	{	
        if(Hj_Range <= 1)
        {
            CCPR_FAN = 85;
        }
        else if(Hj_Range <= 3)
		{	
			CCPR_FAN = 150;
		}
		else
		{	
			CCPR_FAN = 250;
		}	 
		TRIS_FAN = OUT;	//风机输出	
	}
	else
	{
		TRIS_FAN = IN;
	}	
}

//=========================================
//风门加热处理
//===================================
void fan_door_heat_deal(void)
{
	//----------------------------------------
	//风门加热
	//---------------------------------------			
    if(Hj_Range >= 5 )//大于34
	{
		Flag_Data.Fan_Door_Heat = 1;
	}
	else
	{
		if(1 == Flag_Data.Fan_Door_State)
		{
			Flag_Data.Fan_Door_Heat = 0;
		}
		else
		{
			Flag_Data.Fan_Door_Heat = 1;
		}
	}
}

//===============================================
//压缩机频率处理
//=============================================
void comp_freq_deal(void)
{  
    int16 lc_differ;
    int16 ld_differ;
    uint8 freq_temp;
	if(1 == Flag_Data.Comp)
	{
        if(0 == Flag_Data.Comp_Freq_State)
        {
            Flag_Data.Comp_Freq_State = 1;  
            if(0 == Flag_Data.Def_Freq_State) 
            {
                if(0 == Flag_Data.Forst_Freq_S6)
                {
                    Comp_Freq_Data = First_Freq[Hj_Range][Flag_Data.Jn_Test][Flag_Data.Fan_Door_State];
                }
                else
                {
                    Flag_Data.Forst_Freq_S6 = 0;
                    Comp_Freq_Data = FREQ_S6;
                }
            }
            else
            {
                Flag_Data.Def_Freq_State = 0;       
                Comp_Freq_Data = Def_Freq[Hj_Range][Flag_Data.Jn_Test][Flag_Data.Fan_Door_State];
            }
        }     
       
        if(0 == Flag_Data.Lc_Tem_Err && 0 == Flag_Data.Ld_Tem_Err && 1 == Flag_Data.First_Comp_End && 0 == Flag_Data.Jn_Test)
        {            
            if(SD_MS != SET_MS_DATA)
            {            
                if(0 == Flag_Data.Jn_Test)
                {
                    if(Comp_Move_3min_Counter < TIME_3MIN)
                    {
                        Comp_Move_3min_Counter = Comp_Move_3min_Counter + Time_1_Data;
                    }
                    else
                    {                        
                        Comp_Move_3min_Counter = 0;
                        Freq_Add_Num = 0;  //需要修改
                        ld_differ = Ld_Room_Tem - Set_Ld_Tem;
                        lc_differ = Lc_Room_Tem - Set_Lc_Tem;
                        if(ld_differ >= lc_differ)
                        {
                            Tem_Differ = ld_differ;
                        }
                        else
                        {
                            Tem_Differ = lc_differ;
                        }
                        
                        if(Tem_Differ < 20)
                        {
                            Freq_Add_Num = -1;
                        }
                        else
                        {
                            if(Tem_Differ < 40)
                            {
                                Freq_Add_Num = 1;
                            }
                            else
                            {
                                if(Tem_Differ < 60)
                                {
                                    Freq_Add_Num = 2;
                                }
                                else
                                {
                                    if(Tem_Differ < 80)
                                    {
                                        Freq_Add_Num = 3;
                                    }
                                    else
                                    {
                                        if(Tem_Differ < 100)
                                        {
                                            Freq_Add_Num = 4;
                                        }
                                        else
                                        {
                                            if(Tem_Differ < 120)
                                            {
                                                Freq_Add_Num = 5;
                                            }
                                            else
                                            {
                                                Freq_Add_Num = 6;
                                            }
                                        }
                                    }
                                }
                            }
                        } 
                    }	
                }
            }    
            else
            {
                Freq_Add_Num = 6;
            }
        }       
        
        if(0 != Freq_Add_Num)
        {
            if(0 == Flag_Data.Jn_Test)
            {
                Comp_Freq_Updata_Time = TIME_60SEC;
            }
            else
            {
                Comp_Freq_Updata_Time = Updata_Time[Comp_Freq_Data][Hj_Range];
            }
            if(Comp_Freq_Updata_Time_Counter < Comp_Freq_Updata_Time)
            {
                Comp_Freq_Updata_Time_Counter++;
            }
            else
            {
                Comp_Freq_Updata_Time_Counter = 0;
                if(Freq_Add_Num > 0)
                {
                    Freq_Add_Num--;                    
                    if(Comp_Freq_Data < FREQ_S8)
                    {
                        Comp_Freq_Data++;
                    }
                }
                else
                {
                    Freq_Add_Num++;
                    freq_temp = First_Freq[Hj_Range][Flag_Data.Jn_Test][Flag_Data.Fan_Door_State];
                    if(Comp_Freq_Data > freq_temp)
                    {
                        Comp_Freq_Data--;
                    }
                }
            }
        }       
		PR_COMP    = Comp_Cycle_Data[Comp_Freq_Data];
		CCPR_COMP = Comp_Duty_Data[Comp_Freq_Data];
		TRIS_COMP = OUT;	
	}
	else
	{        
        Flag_Data.Comp_Freq_State = 0;
		TRIS_COMP = IN;	
	}
} 
//===================================================
//测试处理
//===================================================
void test_deal(void)
{
	switch(Test_Mode_Data)
	{
	case 1:		
		PORT_COMP = 1;
		PR_COMP    = Comp_Cycle_Data[FREQ_S8];
		CCPR_COMP = Comp_Duty_Data[FREQ_S8];
		TRIS_COMP = OUT;	
		TRIS_FAN = OUT;
		PORT_DB_HEAT = 0;
		PORT_FAN_DOOR_HEAT = 0;		
		PORT_DEF = 0;		
		Flag_Data.Def= 0;
		Flag_Data.Comp = 1;
		fandoor_open_deal();
		if(1000 == Freq_Time_1sec_Counter)
		{
			Freq_Time_1sec_Counter = 0;			
			if(Fb_Freq_Counter <= 20)
			{									
				Flag_Data.Fan_Err = 1;				
			}			
			Fb_Freq_Counter = 0;		
		}
		break;
	case 2:
		PORT_COMP = 0;
		TRIS_COMP = IN;
		TRIS_FAN = IN;
		PORT_DB_HEAT = 1;
		PORT_FAN_DOOR_HEAT = 1;
		Flag_Data.Def = 1;
		Flag_Data.Comp = 0;
		PORT_DEF = 1;	
		//PORT_FLZ = 0;		
		fandoor_close_deal();
		break;
	default:
		break;	
	}
}
//=======================================
//上电5分钟内处理
//=========================================
void power_on_deal(void)		//1s  
{	   
	switch(Power_On_Time_Counter)
	{
	case 1:		//1s
		PORT_DEF = 1;		
		break;
	case 6:		//1+5=6s
		PORT_DEF = 0;		
		break;
	case 8:		//6+2=8s	
		PORT_COMP = 1;
		TRIS_COMP = OUT;	
		break;
 	case 11:		//8+3=11s	
		PORT_DB_HEAT = 1;
		break;
	case 14:		//11+3=14s
		PORT_DB_HEAT = 0;
		break;
	case 15:		//14+1=15s
		PORT_FAN_DOOR_HEAT = 1;
		break;
	case 18:		//15+3=18s
		PORT_FAN_DOOR_HEAT = 0;
		break;
	case 19:			//18+1=19s
		TRIS_FAN = OUT;
		break;
	case 23:		//19+4=23s
		TRIS_FAN = IN;
		break;
	case 24: 				//23+1=24s
		Flag_Data.Fan_Door_Open_Enable = 1;
		Flag_Data.Fan_Door_Close_Enable = 1;
		break;
	case 25:		//24+1=25s	
		Flag_Data.Power_On_End = 1;
        Flag_Data.Jn_Test = 1;
        Jn_Def_Time = TIME_12HOUR;
		break;
	default:
		break;		
	}	
}
//============================================
//挡板加热处理
//=============================================
void db_heat_deal(void)
{
    if(1 == Flag_Data.Db_Heat)
    {
        Db_Heat_Off_Time_Counter = 0;
        if(Db_Heat_On_Time_Counter < Db_Heat_On_Time[Hj_Range])
        {
            Db_Heat_On_Time_Counter++;
        }
        else
        {
            Flag_Data.Db_Heat = 0;
        }
    }
    else
    {
        Db_Heat_On_Time_Counter = 0;
        if(Db_Heat_Off_Time_Counter < Db_Heat_Off_Time[Hj_Range])
        {	
            Db_Heat_Off_Time_Counter++;	
        }
        else
        {
            Flag_Data.Db_Heat = 1;
        }
    }
}

void jn_deal(void)
{
#ifdef TEST_H
    Test_Data = Flag_Data.Jn_Test;
#endif
    if(Jn_1min_Counter < TIME_1MIN && 0 == Flag_Data.Short_Time)
    {
        Jn_1min_Counter++;
    }
    else
    {
        Jn_1min_Counter = 0;
        if(1 == Flag_Data.Jn_Test)
        {
            if(0 == Flag_Data.Def)
            {
                if(Jn_Def_Time_Counter < Jn_Def_Time)
                {
                    Jn_Def_Time_Counter++;
                }
                else
                {
                    Jn_Def_Time_Counter = 0;
                    Flag_Data.Def = 1;
//#ifdef TEST_H
//                    Jn_Def_Time = 1;
//#else
                    Jn_Def_Time = TIME_72HOUR;
//#endif
                    if(Jn_Def_Num < NUM_3)
                    {
                        Jn_Def_Num++;
                    }
                    else
                    {
                        Flag_Data.Close_Jn_Test_Enable = 1;
                    }
                }
            }
        }
        if(Hj_Range == Hj_Range_Old)
        {
            if( Hj_Range < 5 && SET_MS_DATA == 0 && 9 != SET_LC_TEM 
                    && Door_Open_Num < 3 && 0 == Flag_Data.Door_Open_70sec && All_Door_Open_Time < 80)
            { 
//#ifdef TEST_H
//                if(Jn_24hour_Counter < TIME_2HOUR)
//#else
                if(Jn_24hour_Counter < TIME_24HOUR)
//#endif
                {
                    Jn_24hour_Counter++;
                }
                else
                {
                    Jn_24hour_Counter = 0;
                    Door_Open_Num = 0;
                    All_Door_Open_Time = 0;
                    if(0 == Flag_Data.Jn_Test)
                    {
                        Flag_Data.Jn_Test = 1;
//#ifdef TEST_H
//                        Jn_Def_Time = 1;
//#else
                        Jn_Def_Time = TIME_12HOUR;
//#endif
                        Jn_Def_Time_Counter = 0;
                        Jn_Def_Num = 0;
                    }
                }
            }
            else
            {
                if(1 == Flag_Data.Close_Jn_Test_Enable)
                {
                    Flag_Data.Close_Jn_Test_Enable = 0;
                    Jn_24hour_Counter = 0;
                    Flag_Data.Door_Open_70sec = 0;
                    Door_Open_Num = 0;
                    All_Door_Open_Time = 0;
                    Flag_Data.Jn_Test = 0;
                    Jn_Def_Num = 0;
                }
            }								
        }
        else
        {
            Hj_Range_Old = Hj_Range;
            Flag_Data.Close_Jn_Test_Enable = 0;
            Jn_24hour_Counter = 0;
            Flag_Data.Door_Open_70sec = 0;
            Door_Open_Num = 0;
            All_Door_Open_Time = 0;
            Flag_Data.Jn_Test = 0;
            Jn_Def_Num = 0;
        } 
    }
}
//==========================================================
//控制处理
//==========================================================
void control_deal(void)
{	
	if(1 == Flag_Data.Control_Enable)
	{
		Flag_Data.Control_Enable = 0;          
        door_deal();
        jn_deal();
        if(0 == Test_Mode_Data)
        {
            if(Power_On_Time_Counter < 299)
            {
                Power_On_Time_Counter++;
            }
            if(1 == Flag_Data.Power_On_End)
            {
                set_tem_deal();
                comp_fandoor_def_deal();  
                if(Power_On_Time_Counter < 299)
                {
                    Power_On_Time_Counter = Power_On_Time_Counter + Time_1_Data -1;
                    Flag_Data.Comp = 1;			//必须在压缩机频率处理之前     
                    Flag_Data.Comp_Freq_State = 1;  //上电5分钟以S4开启                    
                    if(Hj_Range >= 5)
                    {
                       Comp_Freq_Data = FREQ_S8;
                       PR_COMP = PR_FREQ_125;
                       CCPR_COMP = CCPR_FREQ_125;
                    }
                    else
                    {
                        Comp_Freq_Data = FREQ_S6;
                        PR_COMP = PR_FREQ_100;
                        CCPR_COMP = CCPR_FREQ_100;
                    }
                }
                else
                {
                    comp_freq_deal();   
                }
                ld_fan_deal();	
                fan_door_heat_deal();	
                db_heat_deal();
                PORT_COMP = Flag_Data.Comp;	
                //PORT_FLZ = Flag_Data.Flz_Out;
                PORT_DEF = Flag_Data.Def_Out;
                PORT_DB_HEAT = Flag_Data.Db_Heat;
                PORT_FAN_DOOR_HEAT = Flag_Data.Fan_Door_Heat;
            }
            else
            {
                power_on_deal();
            }            
        }
        else
        {
            test_deal();
        }  
	}
}