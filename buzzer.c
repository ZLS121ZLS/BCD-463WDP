#include "common.h"

uint8 Buzzer_State;
uint8 Buzzer_Power_Time_Counter;
uint8 Buzzer_Type;

void buzzer_power_deal(void)
{
	if(1 == Flag_Data.Buzzer_Clock)	//5ms
	{
		Flag_Data.Buzzer_Clock = 0;			
		switch(Buzzer_Type)
		{		
		case POWER_BUZZ:
			if(1 == Flag_Data.Buzzer_Power)
			{
				if(++Buzzer_Power_Time_Counter >= 5)
				{
					Buzzer_Power_Time_Counter = 0;
					Flag_Data.Buzzer_Power    = 0;
				}
			}
			else
			{
				if(++Buzzer_Power_Time_Counter >= 45)
				{
					Buzzer_Power_Time_Counter = 0;
					if(Buzzer_State <= 1)
					{
						Buzzer_State++;
						Flag_Data.Buzzer_Power = 1;						
					}					
				}
			}		
			break;
/*
			case KEY_BUZZ:
			//case DOOR_BUZZ:
			switch(Buzzer_State)
			{
			case 0:
				if(++Buzzer_Power_Time_Counter >= 8)
				{
					Buzzer_Power_Time_Counter = 0;
				}
				Buzzer_State = 1;
				Flag_Data.Buzzer_Power = 1;	
				break;
			case 1:
				if(++Buzzer_Power_Time_Counter >= 17)
				{
					Buzzer_Power_Time_Counter = 0;
				}
				Buzzer_State = 2;
				Flag_Data.Buzzer_Power = 1;	
				break;
			case 2:
				if(++Buzzer_Power_Time_Counter >= 7)
				{
					Buzzer_Power_Time_Counter = 0;
				}
				Buzzer_State = 3;
				Flag_Data.Buzzer_Power = 1;	
				break;
			case 3:
				if(++Buzzer_Power_Time_Counter >= 17)
				{
					Buzzer_Power_Time_Counter = 0;
				}
				Buzzer_State = 4;
				Flag_Data.Buzzer_Power = 0;	
				break;
			case 4:
				if(++Buzzer_Power_Time_Counter >= 7)
				{
					Buzzer_Power_Time_Counter = 0;
				}
				Buzzer_State = 5;
				Flag_Data.Buzzer_Power = 1;	
				break;
			case 5:
				if(++Buzzer_Power_Time_Counter >= 197)
				{
					Buzzer_Power_Time_Counter = 0;
				}				
				Flag_Data.Buzzer_Power = 0;	
				break;				
			}
			break;*/
		case DOOR_BUZZ:
		case KEY_BUZZ:
			if(1 == Flag_Data.Buzzer_Power)
			{
				if(++Buzzer_Power_Time_Counter >= 5)
				{
					Buzzer_Power_Time_Counter = 0;
					Flag_Data.Buzzer_Power    = 0;
				}
			}
			else
			{
				if(++Buzzer_Power_Time_Counter >= 35)				
				{
					Buzzer_Power_Time_Counter = 0;					
					if(0 == Buzzer_State)
					{
						Buzzer_State           = 1;
						Flag_Data.Buzzer_Power = 1;	
					}				
				}
			}	
			break;			
		}	
		PORT_BUZZ_POWER_OUT = Flag_Data.Buzzer_Power;
	}	
}


