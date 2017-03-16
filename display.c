/*************************************************************
名称：dispaly.c   
版本：V1.0_130314
功能：
说明：
 ****************************************************************/
#include "common.h"

uint8 Display_Data[4];
uint8 Dig_Data[4];
int_bit Led_Data;
int_bit Led_2_Data;

int8 Lc_Display_Tem;
int8 Ld_Display_Tem;


uint8 Lc_Refurbish_Time;
uint8 Bw_Refurbish_Time;
uint8 Ld_Refurbish_Time;

uint8 Test_Mode_Data;//测试模式
//==============================================================
//数码管显示移至中断中去了
//==============================================================
//=================================================================================
//显示段码更新
//================================================================================

void seg_deal(void)
{
	if(1 == Flag_Data.Seg_Enable)
  {
    Flag_Data.Seg_Enable = 0;				
    //--------------------------------------------------
    //冷藏
    //--------------------------------------------------
   	if (1 == Flag_Data.Lc_Flash_Enable)
    {
      if (1 == Flag_Data.Display_Flash)
      {
        Dig_Data[1] = 0x00;
				Dig_Data[0] = 0x00;
      }
      else
      {
      	Dig_Data[1] = Seg_Tbl[Display_Data[2]];
        Dig_Data[0] = Seg_Tbl[Display_Data[3]];
      }
    }
    else
    {
      Dig_Data[1] = Seg_Tbl[Display_Data[2]];
      Dig_Data[0] = Seg_Tbl[Display_Data[3]];
    }
    Led_L2_13 = Led_Lc_C;
    Led_L1 = Led_Lc_Off;
    //------------------------------------------------------
    //速冻处理
    //-------------------------------------------------------
    if (1 == Flag_Data.Ld_Flash_Enable)
    {
      if (1 == Flag_Data.Display_Flash)
     	{
        Dig_Data[3] = 0x00;
        Dig_Data[2] = 0x00;
				Led_L3      = 0;
      }
      else
      {
        Dig_Data[3] = Seg_Tbl[Display_Data[0]];
        Dig_Data[2] = Seg_Tbl[Display_Data[1]];
				Led_L3 = Led_Ld_Neg;
      }
    }
    else
    {
      Dig_Data[3] = Seg_Tbl[Display_Data[0]];
      Dig_Data[2] = Seg_Tbl[Display_Data[1]];
			Led_L3 = Led_Ld_Neg;
    }        
    Led_L4_14 = Led_Ld_C;
    
    //-------------------------------------------------------------
    //状态指示
    //------------------------------------------------------
    if (1 == Flag_Data.Quick_Ref_Flash_Enable)
    {
      if (1 == Flag_Data.Display_Flash)
      {
        Led_L5_6 = 0;
      } 
      else
      {
        Led_L5_6 = 1;
      }
    } 
    else
    {
      Led_L5_6 = Led_Sl;
    }
    if (1 == Flag_Data.Quick_Fre_Flash_Enable)
    {
      if (1 == Flag_Data.Display_Flash)
      {
        Led_L7_8 = 0;
      }
      else
      {
        Led_L7_8 = 1;
      }
		}
    else
    {
      Led_L7_8 = Led_Sd;
    }
    if (1 == Flag_Data.Intell_Flash_Enable)
    {
    	if (1 == Flag_Data.Display_Flash)
      {
      	Led_L9_10 = 0;
      }
      else
      {
      	Led_L9_10 = 1;
      }
    }
    else
    {
      Led_L9_10 = Led_Zn;
    }
    if (1 == Flag_Data.Econ_Flash_Enable)
    {
      if (1 == Flag_Data.Display_Flash)
      {
        Led_L11_12 = 0;
      }
      else
      {
        Led_L11_12 = 1;
      }
    }
    else
    {
      Led_L11_12 = Led_Jn;
    }
  }
}
//===============================================
//冷藏室显示温度显示
//===============================================
void lc_tem_display(int8 display_temp)
{
	if (display_temp < 0)
  {
  	Display_Data[3] = 0;
    Display_Data[2] = 0;
  }
  else
  {
    Display_Data[3] = display_temp / 10;
    Display_Data[2] = display_temp % 10;
  }
}
//===============================================
//冷冻室显示温度显示
//===============================================
void ld_tem_display(int8 display_temp)
{
  uint8 display_data;
  if (display_temp < 0)
  {
    display_data = 0 - display_temp;
    Display_Data[1] = display_data / 10;
    Display_Data[0] = display_data % 10;
    Led_Ld_Neg = 1;
  } 
  else
  {
    Display_Data[1] = display_temp / 10;
  	Display_Data[0] = display_temp % 10;
  }
}
//======================================================
//正常状态显示
//======================================================
void normal_display(void) 
{
	static uint8 num1, num2, temp;
  static int_bit data;
	switch(Ms_Set_Data)
	{
	case 4:
		Led_Sl = 1;
		break;
	case 3:
		Led_Sd = 1;
		break;
	case 2:
		Led_Zn = 1;
		break;
	case 1:
		Led_Jn = 1;
		break;
	default:
		break;
	}  
	if(1 == Flag_Data.Lc_Tem_Err)
	{
		Flag_Data.Lc_Flash_Enable = 1;
		Flag_Data.Ld_Flash_Enable = 1;	
		Display_Data[3] = SEG__;
    Display_Data[2] = SEG_E;
		Display_Data[1] = SEG__;
    Display_Data[0] = 1;
	}
	else if(1 == Flag_Data.Ld_Tem_Err)
	{
		Flag_Data.Lc_Flash_Enable = 1;
		Flag_Data.Ld_Flash_Enable = 1;	
		Display_Data[3] = SEG__;
    Display_Data[2] = SEG_E;
		Display_Data[1] = SEG__;
    Display_Data[0] = 2;
	}
	else if(1 == Flag_Data.Ld_Hs_Tem_Err)
	{
		Flag_Data.Lc_Flash_Enable = 1;
		Flag_Data.Ld_Flash_Enable = 1;		
		Display_Data[3] = SEG__;
    Display_Data[2] = SEG_E;
		Display_Data[1] = SEG__;
    Display_Data[0] = 3;
	}
	else if(1 == Flag_Data.Hj_Tem_Err)
	{
		Flag_Data.Lc_Flash_Enable = 1;
		Flag_Data.Ld_Flash_Enable = 1;		
		Display_Data[3] = SEG__;
    Display_Data[2] = SEG_E;
		Display_Data[1] = SEG__;
    Display_Data[0] = 4;
	}
	else if(1 == Flag_Data.Hs_Err)
	{
		Flag_Data.Lc_Flash_Enable = 1;
		Flag_Data.Ld_Flash_Enable = 1;		
		Display_Data[3] = SEG__;
    Display_Data[2] = SEG_E;
		Display_Data[1] = SEG__;
    Display_Data[0] = 5;
	}
	else if(1 == Flag_Data.Fan_Err)	
	{
		Flag_Data.Lc_Flash_Enable = 1;
		Flag_Data.Ld_Flash_Enable = 1;		
		Display_Data[3] = SEG__;
    Display_Data[2] = SEG_E;
		Display_Data[1] = SEG__;
    Display_Data[0] = SEG_F;
	}
	else if(1 == Flag_Data.Comm_Err)
	{
		Flag_Data.Lc_Flash_Enable = 1;
		Flag_Data.Ld_Flash_Enable = 1;	
		Display_Data[3] = SEG__;
    Display_Data[2] = SEG_C;
		Display_Data[1] = SEG__;
    Display_Data[0] = 0;
	}
	else
	{
		if(9 != Lc_Set_Tem)
		{
			Led_Lc_C = 1;
			//Lc_Display_Tem = Test_Data;
      lc_tem_display(Lc_Display_Tem); //冷藏正常温度显示
		}
		else
		{		
			Led_Lc_Off = 1;	
			Display_Data[3] = SEG_o;
    	Display_Data[2] = SEG_F;
		}
		Led_Ld_C = 1;
    ld_tem_display(Ld_Display_Tem); //冷冻正常温度显示
	}
}
//===============================================================
//冷藏室温度显示处理
//===============================================================
void lc_tem2display(void)
{
	int8 set_data;
  set_data = Lc_Set_Tem; 
  if (Lc_Display_Tem < set_data) 
	{
		if(Lc_Refurbish_Time >= 180) 
		{
    	Lc_Refurbish_Time = 0;
      Lc_Display_Tem++;
        
		}
  } 
	else if (Lc_Display_Tem > set_data) 
	{
		if (Lc_Refurbish_Time >= 60) 
		{
    	Lc_Refurbish_Time = 0;
      Lc_Display_Tem--;
    }
  }
  else 
	{
  	Lc_Refurbish_Time = 0;
  }
}
//===============================================================
//冷冻室温度显示处理
//===============================================================
void ld_tem2display(void) 
{
	int8 set_data;
	set_data = Ld_Set_Tem;
  if (Ld_Display_Tem < set_data) 
	{
		if(Ld_Refurbish_Time >= 180) 
		{
    	Ld_Refurbish_Time = 0;
      Ld_Display_Tem++;
        
		}
  } 
	else if (Ld_Display_Tem > set_data) 
	{
		if (Ld_Refurbish_Time >= 60) 
		{
    	Ld_Refurbish_Time = 0;
      Ld_Display_Tem--;
    }
  }
  else 
	{
  	Ld_Refurbish_Time = 0;
  }
}
//============================================================
//冷藏室温度设置显示
//============================================================
void lc_tem_set_display(void)
{       
	Flag_Data.Lc_Flash_Enable = 1;  
  switch(Ms_Set_Data)
	{
	case 4:
		Led_Sl = 1;
		break;
	case 3:
		Led_Sd = 1;
		break;
	case 2:
		Led_Zn = 1;
		break;
	case 1:
		Led_Jn = 1;
		break;
	default:
		break;
	} 
	Led_Ld_C = 1;
  ld_tem_display(Ld_Display_Tem);                     //冷冻正常温度显示  
  if(9 == Lc_Set_Tem_Temp)
  {       
    Led_Lc_Off = 1;	
		Display_Data[3] = SEG_o;
		Display_Data[2] = SEG_F; 
  }
  else
  { 		  
  	Led_Lc_C   = 1;   
    lc_tem_display(Lc_Set_Tem_Temp);    //显示冷藏设置临时数据        
  }
} 
//============================================================
//冷冻室温度设置显示
//============================================================
void ld_tem_set_display(void)
{       
	Flag_Data.Ld_Flash_Enable = 1;  
  switch(Ms_Set_Data)
	{
	case 4:
		Led_Sl = 1;
		break;
	case 3:
		Led_Sd = 1;
		break;
	case 2:
		Led_Zn = 1;
		break;
	case 1:
		Led_Jn = 1;
		break;
	default:
		break;
	} 	
	if(9 != Lc_Set_Tem)
	{
		Led_Lc_C = 1;
    lc_tem_display(Lc_Display_Tem); //冷藏正常温度显示
	}
	else
	{		
		Led_Lc_Off = 1;
		Display_Data[3] = SEG_o;
   	Display_Data[2] = SEG_F;
	}    
  Led_Ld_C   = 1;   
  ld_tem_display(Ld_Set_Tem_Temp);    //显示冷藏设置临时数据        
} 
//=============================================================
//模式设置显示
//===============================================================
void ms_data_set_display(void)
{
	if(9 != Lc_Set_Tem)
	{
		Led_Lc_C = 1;
    lc_tem_display(Lc_Display_Tem); //冷藏正常温度显示
	}
	else
	{		
		Led_Lc_Off = 1;
		Display_Data[3] = SEG_o;
   	Display_Data[2] = SEG_F;
	} 
  Led_Ld_C   = 1;   
  ld_tem_display(Ld_Display_Tem);    //显示冷冻设置临时数据   	
	switch(Ms_Set_Data_Temp)
	{
	case 4:
		Flag_Data.Quick_Ref_Flash_Enable = 1;
		break;
	case 3:
		Flag_Data.Quick_Fre_Flash_Enable = 1;
		break;
	case 2:
		Flag_Data.Intell_Flash_Enable = 1;
		break;
	case 1:
		Flag_Data.Econ_Flash_Enable = 1;
		break;
	default:
		break;		
	}
}
//=============================================================
//测试状态显示
//===============================================================
void test_display(void)
{
	if(1 == Test_Mode_Data)
	{
		Led_Lc_Off = 1;
		Display_Data[3] = SEG__;
   	Display_Data[2] = SEG__;
		Display_Data[1] = SEG_P;
		Display_Data[0] = 1;
	}
	else if(2 == Test_Mode_Data)
	{
		Led_Lc_Off = 1;
		Display_Data[3] = SEG__;
   	Display_Data[2] = SEG__;
		Display_Data[1] = SEG_P;
		Display_Data[0] = 2;
	}		
}
//=============================================================
//缩时显示
//===============================================================
void short_time_display(void)
{
	Flag_Data.Lc_Flash_Enable = 1;
	Flag_Data.Ld_Flash_Enable = 1;
	Flag_Data.Quick_Ref_Flash_Enable = 1;	
	Flag_Data.Quick_Fre_Flash_Enable = 1;	
	Flag_Data.Intell_Flash_Enable = 1;	
	Flag_Data.Econ_Flash_Enable = 1;
	normal_display();
}

//=========================================================
//显示处理
//==========================================================
void display_deal(void)
{	
	if(1 == Flag_Data.Display_Enable)		//100ms扫描一次
	{
		Flag_Data.Display_Enable = 0;

		Flag_Data.Lc_Flash_Enable = 0;
		Flag_Data.Ld_Flash_Enable = 0;
		Flag_Data.Quick_Ref_Flash_Enable = 0;	
		Flag_Data.Quick_Fre_Flash_Enable = 0;	
		Flag_Data.Intell_Flash_Enable = 0;	
		Flag_Data.Econ_Flash_Enable = 0;
		Led_Lc_C = 0;
		Led_Lc_Off = 0;
		Led_Ld_C = 0;
		Led_Ld_Neg = 0;
		Led_Sl = 0;
		Led_Sd = 0;
		Led_Zn = 0;
		Led_Jn = 0;
		Display_Data[0] = SEG__;
		Display_Data[1] = SEG__;
		Display_Data[2] = SEG__;
		Display_Data[3] = SEG__;
	
		if(1 == Flag_Data.Normal_Work)
		{		
			lc_tem2display();			
			ld_tem2display();
			if(0 == Flag_Data.Led_All_Off)
			{
				switch(Controller_State)
				{
				case NORMAL_STATE:				
					normal_display();
					break;	
				case LC_SET_STATE:
					lc_tem_set_display();
					break;				
				case LD_SET_STATE:
					ld_tem_set_display();
					break;	
				case MS_SET_STATE:
					ms_data_set_display();
					break;
				case SHORT_TIME_STATE:
					short_time_display();
					break;	
				case TEST_STATE:
					test_display();
					break;		
				}					
			}			
		}			
		else
		{				
			Display_Data[0] = SEG__8;
			Display_Data[1] = SEG__8;		
			Display_Data[2] = SEG__8;	
			Display_Data[3] = SEG__8;
			Led_Lc_C = 1;
			Led_Lc_Off = 1;
			Led_Ld_C = 1;
			Led_Ld_Neg = 1;
			Led_Sl = 1;
			Led_Sd = 1;
			Led_Zn = 1;
			Led_Jn = 1;
		}
	}
}