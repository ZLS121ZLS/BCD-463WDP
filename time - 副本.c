/*******************************************************************************
名称：time0_isr.c   
版本：V1.0_090205
功能：(mega48V)
说明：time0定时中断
*******************************************************************************/
#include "common.h"
//------------------------------------------------------------------------------
//T1工作在定时方式
//定时器采用1MHz系统时钟作为定时时钟
//每记1个数（2ms）溢出1次
//------------------------------------------------------------------------------
const uint8 Db_Heat_On_Time[7] = {80,80,63,50,37,20,20};
const uint8 Db_Heat_Off_Time[7] = {20,20,37,50,63,80,80};

uint8 Time_1ms_Counter   = 0;       //1ms计数变量
uint8 Time_5ms_Counter   = 0;				//5ms
uint8 Time_50ms_Counter  = 0;				//50ms
uint8 Time_100ms_Counter = 0;       //100ms计数变量
uint8 Time_500ms_Counter = 0;       //500ms计数

uint8 Time_1s_Short_Counter;				//1S
//uint8 Time_1m_Counter   = 0;  			//1分钟
//uint8 Time_24h_Counter;							//24小时

uint8 Err_No_Fan_Door_Time_Counter;	//故障时风门比例关时间
uint8 Err_Fan_Door_Time_Counter;		//故障是风门比例开时间

uint8 No_Normal_Time_Counter; 			//非常规时间计时
//uint8 Power_Time_Counter;						//通电时间

uint8 No_Comp_Time_Counter;					//压缩机关机时间
uint8 No_Comp_Time;									//压缩机开机延时


uint8 Err_No_Comp_Time_Counter;			//故障是压缩机比例关时间
uint8 Err_Comp_Time_Counter;				//故障是压缩机比列开时间

uint16 Comp_Continue_Time_Counter;	//压缩机连续工作时间
uint16 Comp_Move_Time_Counter;			//压缩机累计运行时阿
uint16 Comp_Move_Time;							//压缩机运行时间

uint16 Quick_Fre_Time_Counter;			//速冻时间计时
uint16 Quick_Ref_Time_Counter;
uint8 Def_Time_Counter;							//除霜时间
//uint8 No_Def_Time_Counter;					//非除霜时间计时
//uint8 No_Def_Time;									//非除霜时间
uint8 Def_Prepare_Time_Counter;			//除霜准备时间计时

uint8 Comp_Stop_15s_Counter;				//压缩机停15S
uint8 Comp_Move_30s_Counter;				//压缩机运行30S

uint8 Fan_Delay_Time_Counter;				//风机延时计时
uint8 Fan_Delay_Time;								//风机延时

//uint8 Fan_3s_Counter;								//风机3秒
//uint8 No_Fan_3s_Counter;
uint8 Fan_Test_Counter;							//风机测试次数
uint8 Fan_Err_Time_Counter;					//风机故障时间计时

uint8 Fan_Door_Delay_Time_Counter;	//风门延时计时
uint8 Fan_Door_Delay_Time;					//风门延时

uint8 Door_Open_Time_Counter;				//门打开时间
uint8 Door_Open_10m_Counter;				//门打开10分钟
//uint8 Door1_Open_10m_Counter;
//uint8 Door2_Open_10m_Counter;

//uint8 Flz_On_Time_Counter;					//负离子开时间
//uint8 Flz_Off_Time_Counter;					//负离子关时间
uint8 Db_Heat_On_Time_Counter;			//挡板加热开时间
uint8 Db_Heat_Off_Time_Counter;			//挡板加热关时间
uint8 Fan_Door_No_Act_Time_Counter;	//风门没动作计时
uint8 Comp_Move_90m_Counter;				//压缩机运行90分钟
uint8 Comp_Move_10m_Counter; 				//压缩机运行10分钟

int8 Def_End_Tem;										//除霜终止温度
//uint8 Def_Tem_Counter;							//除霜温度更改次数

uint8 Light_Duty;
//uint8 Light1_Duty;
//uint8 Light2_Duty;
uint8 Comp_Freq_Updata_Time_Counter;
uint8 Comp_Freq_Updata_Time;
uint8 Comp_Move_3min_Counter;

int16 Lc_Differ;
int16 Ld_Differ;
int16 Tem_Differ;
int8 Freq_Add_Num;

//==============================================================================
//时间处理
//==============================================================================
void time_deal(void)
{	
	uint16 data_temp;
	if(++Time_1ms_Counter >= 5)
	{
		Time_1ms_Counter      = 0;		
		Flag_Data.Send_Enable = 1;			//通信发送使能									
		//------------------------------------------------------------------
		//10ms基准计时
		//------------------------------------------------------------------			
		if(++Time_5ms_Counter >= 10)//50ms
		{
			Time_5ms_Counter = 0;				
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
				
			//------------------------------------------------------------------------
			//100ms基准计时
			//-------------------------------------------------------------------------
			if(++Time_50ms_Counter >= 2)        //100ms		
			{
				Time_50ms_Counter = 0;	
				//----------------------------------------------------------
				//门开关判断
				//----------------------------------------------------
				Flag_Data.Door_Temp = PORT_DOOR;	
				if(Flag_Data.Door_Temp == Flag_Data.Door_Old)
				{
					if(1 == Flag_Data.Door_Temp)
					{
						Flag_Data.Door = 1;
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
				//--------------------------------------------
				//正常工作
				//--------------------------------------------										
				if(0 == Flag_Data.Normal_Work)
				{
					if(++No_Normal_Time_Counter > 15)
					{										
						Flag_Data.Normal_Work = 1;								
					}								
				}	
				else
				{
					if(0 == Flag_Data.Lc_Tem_Err && 0 == Flag_Data.Ld_Tem_Err && 0 == Flag_Data.Ld_Hs_Tem_Err 
					  && 0 == Flag_Data.Hs_Err && 0 == Flag_Data.Fan_Err && 0 == Flag_Data.Comm_Err)		//
					{	
						if(0 == PORT_KEY)
						{
							Flag_Data.Key_Temp = 1;			
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
										Freq_Time_1s_Counter = 0;									
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
				//--------------------------------------------
				//挡板加热处理
				//---------------------------------------------
				//if(0 == Flag_Data.Jn_Test)
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
/*
				else
				{
					Flag_Data.Db_Heat = 0;
				}
*/
				//----------------------------------------------------------------------
				//500ms基准计时
				//-----------------------------------------------------------------------
				if(++Time_100ms_Counter >= 5)      //500ms
                {
			 		Time_100ms_Counter = 0;							
					//--------------------------------------------------
					//通信30s没有接收到数据报错及自适应用
					//----------------------------------------------
					if(++No_Receive_Time_Counter >= 60)
					{	
						No_Receive_Time_Counter    = 0;
						Flag_Data.Comm_Err         = 1;					
						ABDEN                      = 1;	
						Flag_Data.Auto_Baud_Detect = 1;						
						Flag_Data.Receive_End      = 0;
						Receive_Num                = 0;					
					}
					else
					{ 
						if(No_Receive_Time_Counter%5 == 0 && No_Receive_Time_Counter > 5)//1s
						{
							ABDEN                      = 1;	
							Flag_Data.Auto_Baud_Detect = 1;
							Flag_Data.Receive_End      = 0;
							Receive_Num                = 0;					
						}
						else 
						{
							if(No_Receive_Time_Counter == 5)
							{
								SPBRGH = 0x00; 
								SPBRGL = 0x67;
								ABDEN  = 0;	
								Flag_Data.Auto_Baud_Detect = 0;
								Flag_Data.Receive_End      = 0;
								Receive_Num                = 0;					
							}
						}
					}						
					//---------------------------------------------------------------------
					//1s基准时间计时
					//---------------------------------------------------------------------
					if(++Time_500ms_Counter >= 2)		//1s
					{
						Time_500ms_Counter = 0;		
                        Flag_Data.Control_Enable = 1;				//控制使能
						//---------------------------------------------------------
						//压机处理
						//---------------------------------------------------------
						if(0 == Flag_Data.Comp)
						{
                            Comp_Move_3min_Counter = 0;
                            Freq_Add_Num = 0;
							Comp_Continue_Time_Counter = 0;		//连续运行清零
							Comp_Move_30s_Counter = 0;	
							Comp_Move_90m_Counter = 0;		
							Comp_Move_10m_Counter = 0;					
							if(Comp_Stop_15s_Counter < 15)
							{
								Comp_Stop_15s_Counter++;
							}							
						}
						else
						{
							No_Comp_Time_Counter = 0;
							Flag_Data.No_Comp_Time_End = 0;							
							Comp_Stop_15s_Counter = 0;
							No_Comp_Time = 7;
							if(Comp_Move_30s_Counter < 30)
							{	
								Comp_Move_30s_Counter++;							
							}	
                            if(0 != Freq_Add_Num)
                            {
                                if(0 == Flag_Data.Jn_Test)
                                {
                                    Comp_Freq_Updata_Time = 59;
                                }
                                else
                                {
                                    if(Hj_Tem_Data < 120)
                                    {
                                        if(Comp_Freq_Data <= FREQ_S4)
                                        {
                                            Comp_Freq_Updata_Time = 89;
                                        }
                                        else
                                        {
                                            Comp_Freq_Updata_Time = 59;
                                        }
                                    }
                                    else
                                    {
                                        if(Hj_Tem_Data < 180)
                                        {
                                            if(Comp_Freq_Data <= FREQ_S4)
                                            {
                                                Comp_Freq_Updata_Time = 89;
                                            }
                                            else
                                            {
                                                Comp_Freq_Updata_Time = 59;
                                            }
                                        }
                                        else
                                        {
                                            if(Hj_Tem_Data < 220)
                                            {
                                                if(Comp_Freq_Data >= FREQ_S1)
                                                {
                                                    Comp_Freq_Updata_Time = 59;
                                                }
                                                else
                                                {
                                                    Comp_Freq_Updata_Time = 89;
                                                }
                                            }
                                            else
                                            {
                                                if(Hj_Tem_Data < 270)
                                                {
                                                    if(Comp_Freq_Data >= FREQ_S1)
                                                    {
                                                        Comp_Freq_Updata_Time = 59;
                                                    }
                                                    else
                                                    {
                                                        Comp_Freq_Updata_Time = 89;
                                                    }
                                                }
                                                else
                                                {
                                                    if(Hj_Tem_Data < 340)
                                                    {
                                                        if(Comp_Freq_Data >= FREQ_S5)
                                                        {
                                                            Comp_Freq_Updata_Time = 59;
                                                        }
                                                        else
                                                        {
                                                            if(Comp_Freq_Data >= FREQ_S2)
                                                            {
                                                                Comp_Freq_Updata_Time = 119;
                                                            }
                                                            else
                                                            {
                                                                Comp_Freq_Updata_Time = 89;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
						}	
						//--------------------------------
						//风机处理
						//----------------------------------------
						if(0 == Flag_Data.Fan_Test)
						{
							if(1 == Flag_Data.Fan)
							{
								Fan_Delay_Time_Counter = 0;								
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
						}
						//-----------------------------------------------
						//门开关动作
						//-------------------------------------------------
						if(1 == Flag_Data.Door)
						{
							if(Door_Open_Time_Counter < 80)
							{
								Door_Open_Time_Counter++;								
							}	
							else
							{
								Comp_Move_Time = 420;
								Flag_Data.Comp_140hz_Enable = 1;
							}
						}
						else
						{
							if(Door_Open_Time_Counter < 80 && Comp_Move_Time > 420)
							{
								Comp_Move_Time = Comp_Move_Time - Door_Open_Time_Counter*10;
							}
							Door_Open_Time_Counter = 0;
						}			
						if(0 == Fan_Delay_Time)
						{
							Flag_Data.Fan_Delay_Time_End = 1;		
						}
						//if(1 == Flag_Data.Door || 0 == Flag_Data.Flz)
						//{
						//	Flag_Data.Flz_Out = 0;
						//}
/*
						if(0 == Flag_Data.Jn_Test)
						{
							Def_End_Tem = 80;
						}
*/
/*
						if(0 == Flag_Data.Door && Hj_Range == 3 && SET_MS_DATA == 0 && 0 == Flag_Data.Lc_Tem_Err && 0 == Flag_Data.Ld_Tem_Err && 0 == Flag_Data.Ld_Hs_Tem_Err 
	 								 && 0 == Flag_Data.Hs_Err && 0 == Flag_Data.Fan_Err && 0 == Flag_Data.Comm_Err && 0 == Data[4])		//
						{
							;
						}
						else
						{
							Time_24h_Counter = 0;
							Flag_Data.Jn_Test = 0;									
						}	
*/
/*
						if(1 == Flag_Data.Jn_Test)
						{
							switch(Def_Tem_Counter)
							{
							case 0:
								Def_End_Tem = -100;
								break;
							case 1:
								Def_End_Tem = -100;
								break;
							case 2:
								Def_End_Tem = 80;
								break;
							}
						}
*/
						//---------------------------------------------------
						//缩时处理
						//-----------------------------------------------------
						if(0 == Flag_Data.Short_Time)
						{
							data_temp = 57;
						}
						else
						{
							data_temp = 0;
						}							
						if(++Time_1s_Short_Counter >= data_temp)			//1分钟
						{
							Time_1s_Short_Counter = 0;								
							//-----------------------------------------------------------
							//风门连续45分钟无动作
							//-------------------------------------------------------------			
							if(0 == Flag_Data.Fan_Door_Open_Enable && 0 == Flag_Data.Fan_Door_Close_Enable)
							{
								if(++Fan_Door_No_Act_Time_Counter >= 45)//45
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
							//---------------------------------------------------------
							//压机处理
							//---------------------------------------------------------
							if(0 == Flag_Data.Comp)
							{										
								if(No_Comp_Time_Counter < No_Comp_Time)
								{
									No_Comp_Time_Counter++;
								}
								else
								{									
									Flag_Data.No_Comp_Time_End = 1;
								}	
								if(Comp_Move_Time_Counter >= Comp_Move_Time)
								{
									Flag_Data.Def_Prepare = 1;	
								}											
							}
							else  //压缩机连续工作
							{
                                if(0 == Flag_Data.Jn_Test)
                                {
                                    if(Comp_Move_3min_Counter < 2)
                                    {
                                        Comp_Move_3min_Counter++;
                                    }
                                    else
                                    {
                                        Comp_Move_3min_Counter = 0;
                                        Ld_Differ = Ld_Room_Tem - Comp_Move_Tem;
                                        Lc_Differ = Lc_Room_Tem - Ref_Move_Tem;
                                        if(Ld_Differ >= Lc_Differ)
                                        {
                                            Tem_Differ = Ld_Differ;
                                        }
                                        else
                                        {
                                            Tem_Differ = Lc_Differ;
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
                                                                Freq_Add_Num = 6;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }                                    
                                    }	
                                }
								if(Comp_Continue_Time_Counter < 599)	//压机连续工作10小时BCD-366改成10小时
								{
									Comp_Continue_Time_Counter++;
								}
								else
								{									
									Flag_Data.Def_Prepare = 1;																					
								}								
								if(Comp_Move_Time_Counter < Comp_Move_Time)
								{
									Comp_Move_Time_Counter++;
								}
								else
								{
									Flag_Data.Def_Prepare = 1;	
								}
							}	
							//--------------------------------
							//风机处理
							//----------------------------------------
							if(0 == Flag_Data.Fan_Test)
							{
								if(0 == Flag_Data.Fan)
								{
									if(++Fan_Delay_Time_Counter > Fan_Delay_Time)
									{
										Fan_Delay_Time = 0;
										Fan_Delay_Time_Counter = 0;									
										Flag_Data.Fan_Delay_Time_End = 1;
									}																
								}							
							}
							//-----------------------------------------
							//风门处理
							//---------------------------------------------
							if(0 == Flag_Data.Fan_Door_State)
							{									
								if(++Fan_Door_Delay_Time_Counter >= Fan_Door_Delay_Time)
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
									
							
							//-----------------------------
							//除霜
							//------------------------------------
							if(1 == Flag_Data.Def)
								
							}	
							else
							{			
								Def_Time_Counter = 0;										
							}
							if(Ld_Hs_Room_Tem >= Def_End_Tem && 0 == Flag_Data.Ld_Hs_Tem_Err)
							{									
								Flag_Data.Hs_Err = 0;
							}				
							//----------------------------------------------
							//准备除霜处理
							//----------------------------------
							if(1 == Flag_Data.Def_Prepare)	//准备化霜
							{									
								if(Def_Prepare_Time_Counter < 30)
								{
									Def_Prepare_Time_Counter++;
								}
								else
								{									
									def_begin_deal();
								}
							}
							//----------------------------------------
							//冷藏门灯处理
							//----------------------------------------------------
							if(1 == Flag_Data.Door)
							{
								if(Door_Open_10m_Counter < 10)
								{
									Door_Open_10m_Counter++;	
								}	
								else
								{										
									Flag_Data.Light_Off = 1;									
								}								
							}
							else
							{
								Door_Open_10m_Counter = 0;									
							}												
							//------------------------------------------
							//速冻模式
							//-----------------------------------------					
							if(SD_MS == SET_MS_DATA)
							{
								Def_Prepare_Time_Counter = 0;
								if(++Quick_Fre_Time_Counter >= 300)
								{
									Quick_Fre_Time_Counter = 0;
									Data[6] = Data[6] - Data[2];
									Data[2] = 0;
									Flag_Data.Write_Enable = 1;										
								}
							}								
							if(SL_MS == SET_MS_DATA)
							{
								if(++Quick_Ref_Time_Counter >= 300)
								{
									Quick_Ref_Time_Counter = 0;
									Data[6] = Data[6] - Data[2];
									Data[2] = 0;
									Flag_Data.Write_Enable = 1;										
								}
							}
							//------------------------------------
							//风机故障
							//----------------------------------
							if(1 == Flag_Data.Fan_Err)
							{
								if(++Fan_Err_Time_Counter >= 180)	//3小时
								{
									Fan_Err_Time_Counter = 0;
									Flag_Data.Fan_Test = 1;
								}
							}
/*
							//--------------------------------------------
							//小时
							//---------------------------------------------
							if(++Time_1m_Counter >= 60)
							{
								Time_1m_Counter = 0;
								if(0 == Data[4])	//耗能模式未永久结束
								{
									if(++Power_Time_Counter >= 24)
									{
										Power_Time_Counter = 0;											
										if(++Data[5] >= 28)
										{
											Data[4] = 1;
											Data[6]++;
										}
										Data[6]++;										
										Flag_Data.Write_Enable = 1;				
									}
								}
								if(0 == Flag_Data.Door && Hj_Range == 3 && SET_MS_DATA == 0 && 0 == Flag_Data.Lc_Tem_Err && 0 == Flag_Data.Ld_Tem_Err && 0 == Flag_Data.Ld_Hs_Tem_Err 
	 								 && 0 == Flag_Data.Hs_Err && 0 == Flag_Data.Fan_Err && 0 == Flag_Data.Comm_Err && 1 == Flag_Data.First_Def)		//
								{
									if(++Time_24h_Counter >= 24)
									{
										Time_24h_Counter = 0;
										if(0 == Data[4])
										{
											Flag_Data.Jn_Test = 1;
										}
									}
								}							
							}	
*/						
						}		
					}			
				}		
			}		     
		}
	}
}

	
	
	   