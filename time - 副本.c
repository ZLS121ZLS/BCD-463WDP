/*******************************************************************************
���ƣ�time0_isr.c   
�汾��V1.0_090205
���ܣ�(mega48V)
˵����time0��ʱ�ж�
*******************************************************************************/
#include "common.h"
//------------------------------------------------------------------------------
//T1�����ڶ�ʱ��ʽ
//��ʱ������1MHzϵͳʱ����Ϊ��ʱʱ��
//ÿ��1������2ms�����1��
//------------------------------------------------------------------------------
const uint8 Db_Heat_On_Time[7] = {80,80,63,50,37,20,20};
const uint8 Db_Heat_Off_Time[7] = {20,20,37,50,63,80,80};

uint8 Time_1ms_Counter   = 0;       //1ms��������
uint8 Time_5ms_Counter   = 0;				//5ms
uint8 Time_50ms_Counter  = 0;				//50ms
uint8 Time_100ms_Counter = 0;       //100ms��������
uint8 Time_500ms_Counter = 0;       //500ms����

uint8 Time_1s_Short_Counter;				//1S
//uint8 Time_1m_Counter   = 0;  			//1����
//uint8 Time_24h_Counter;							//24Сʱ

uint8 Err_No_Fan_Door_Time_Counter;	//����ʱ���ű�����ʱ��
uint8 Err_Fan_Door_Time_Counter;		//�����Ƿ��ű�����ʱ��

uint8 No_Normal_Time_Counter; 			//�ǳ���ʱ���ʱ
//uint8 Power_Time_Counter;						//ͨ��ʱ��

uint8 No_Comp_Time_Counter;					//ѹ�����ػ�ʱ��
uint8 No_Comp_Time;									//ѹ����������ʱ


uint8 Err_No_Comp_Time_Counter;			//������ѹ����������ʱ��
uint8 Err_Comp_Time_Counter;				//������ѹ�������п�ʱ��

uint16 Comp_Continue_Time_Counter;	//ѹ������������ʱ��
uint16 Comp_Move_Time_Counter;			//ѹ�����ۼ�����ʱ��
uint16 Comp_Move_Time;							//ѹ��������ʱ��

uint16 Quick_Fre_Time_Counter;			//�ٶ�ʱ���ʱ
uint16 Quick_Ref_Time_Counter;
uint8 Def_Time_Counter;							//��˪ʱ��
//uint8 No_Def_Time_Counter;					//�ǳ�˪ʱ���ʱ
//uint8 No_Def_Time;									//�ǳ�˪ʱ��
uint8 Def_Prepare_Time_Counter;			//��˪׼��ʱ���ʱ

uint8 Comp_Stop_15s_Counter;				//ѹ����ͣ15S
uint8 Comp_Move_30s_Counter;				//ѹ��������30S

uint8 Fan_Delay_Time_Counter;				//�����ʱ��ʱ
uint8 Fan_Delay_Time;								//�����ʱ

//uint8 Fan_3s_Counter;								//���3��
//uint8 No_Fan_3s_Counter;
uint8 Fan_Test_Counter;							//������Դ���
uint8 Fan_Err_Time_Counter;					//�������ʱ���ʱ

uint8 Fan_Door_Delay_Time_Counter;	//������ʱ��ʱ
uint8 Fan_Door_Delay_Time;					//������ʱ

uint8 Door_Open_Time_Counter;				//�Ŵ�ʱ��
uint8 Door_Open_10m_Counter;				//�Ŵ�10����
//uint8 Door1_Open_10m_Counter;
//uint8 Door2_Open_10m_Counter;

//uint8 Flz_On_Time_Counter;					//�����ӿ�ʱ��
//uint8 Flz_Off_Time_Counter;					//�����ӹ�ʱ��
uint8 Db_Heat_On_Time_Counter;			//������ȿ�ʱ��
uint8 Db_Heat_Off_Time_Counter;			//������ȹ�ʱ��
uint8 Fan_Door_No_Act_Time_Counter;	//����û������ʱ
uint8 Comp_Move_90m_Counter;				//ѹ��������90����
uint8 Comp_Move_10m_Counter; 				//ѹ��������10����

int8 Def_End_Tem;										//��˪��ֹ�¶�
//uint8 Def_Tem_Counter;							//��˪�¶ȸ��Ĵ���

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
//ʱ�䴦��
//==============================================================================
void time_deal(void)
{	
	uint16 data_temp;
	if(++Time_1ms_Counter >= 5)
	{
		Time_1ms_Counter      = 0;		
		Flag_Data.Send_Enable = 1;			//ͨ�ŷ���ʹ��									
		//------------------------------------------------------------------
		//10ms��׼��ʱ
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
			//100ms��׼��ʱ
			//-------------------------------------------------------------------------
			if(++Time_50ms_Counter >= 2)        //100ms		
			{
				Time_50ms_Counter = 0;	
				//----------------------------------------------------------
				//�ſ����ж�
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
				//��������
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
				//������ȴ���
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
				//500ms��׼��ʱ
				//-----------------------------------------------------------------------
				if(++Time_100ms_Counter >= 5)      //500ms
                {
			 		Time_100ms_Counter = 0;							
					//--------------------------------------------------
					//ͨ��30sû�н��յ����ݱ�������Ӧ��
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
					//1s��׼ʱ���ʱ
					//---------------------------------------------------------------------
					if(++Time_500ms_Counter >= 2)		//1s
					{
						Time_500ms_Counter = 0;		
                        Flag_Data.Control_Enable = 1;				//����ʹ��
						//---------------------------------------------------------
						//ѹ������
						//---------------------------------------------------------
						if(0 == Flag_Data.Comp)
						{
                            Comp_Move_3min_Counter = 0;
                            Freq_Add_Num = 0;
							Comp_Continue_Time_Counter = 0;		//������������
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
						//�������
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
						//�ſ��ض���
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
						//��ʱ����
						//-----------------------------------------------------
						if(0 == Flag_Data.Short_Time)
						{
							data_temp = 57;
						}
						else
						{
							data_temp = 0;
						}							
						if(++Time_1s_Short_Counter >= data_temp)			//1����
						{
							Time_1s_Short_Counter = 0;								
							//-----------------------------------------------------------
							//��������45�����޶���
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
							//ѹ������
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
							else  //ѹ������������
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
								if(Comp_Continue_Time_Counter < 599)	//ѹ����������10СʱBCD-366�ĳ�10Сʱ
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
							//�������
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
							//���Ŵ���
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
							//��˪
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
							//׼����˪����
							//----------------------------------
							if(1 == Flag_Data.Def_Prepare)	//׼����˪
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
							//����ŵƴ���
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
							//�ٶ�ģʽ
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
							//�������
							//----------------------------------
							if(1 == Flag_Data.Fan_Err)
							{
								if(++Fan_Err_Time_Counter >= 180)	//3Сʱ
								{
									Fan_Err_Time_Counter = 0;
									Flag_Data.Fan_Test = 1;
								}
							}
/*
							//--------------------------------------------
							//Сʱ
							//---------------------------------------------
							if(++Time_1m_Counter >= 60)
							{
								Time_1m_Counter = 0;
								if(0 == Data[4])	//����ģʽδ���ý���
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

	
	
	   