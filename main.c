//====================================================================
//11.4修改新的功能表
//====================================================================
#include "common.h"


__CONFIG(FOSC_INTOSC & WDTE_ON & PWRTE_ON & MCLRE_ON & CP_ON & CPD_ON & BOREN_ON & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
__CONFIG(WRT_OFF & VCAPEN_OFF & PLLEN_OFF & STVREN_OFF & BORV_25 & LVP_ON);

__EEPROM_DATA(4,-18,2,0,0,0,153,0);

void main()
{	
	system_init();	
	while(1)
	{
		if(1 == Flag_Data.System_Clock)
		{
			Flag_Data.System_Clock = 0;
			if(3 != Test_Mode_Data)
			{
				clr_wdt();
			}
			data_deal();
            fan_door_step_deal();
            fan_fb_deal();
            light_deal();		
            door_key_deal();
			ad_deal();
			time_deal();			
			comm_deal();			
			control_deal();			
		}                
	}
}

   
