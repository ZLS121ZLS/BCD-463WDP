/*************************************************************
名称：interrupt.c
版本：V1.0_100601
功能：
说明：
 ****************************************************************/
#include "common.h"
//==============================================================
uint8 Receive_Data_Temp;

void interrupt ISR(void) //中断服务程序
{
	if(TMR1IE && TMR1IF) 		//判TMR1 中断
	{	
		TMR1IF = 0; 					//清除TMR1 中断标志
		TMR1ON = 0;						//TMR1暂停
		TMR1L  = 0x0C;			//TMR1付初值//采用内部时钟（FOSC/4）即1MHz
		TMR1H  = 0xFE;			//1ms的定时为65536-1000=64536即1000*1us=1ms中断一次
		//TMR1L  = 0x65;			//TMR1付初值//采用内部时钟（FOSC/4）即1MHz
		//TMR1H  = 0xFD;			//1ms的定时为65536-1000=64536即1000*1us=1ms中断一次
		TMR1ON = 1; 
        Flag_Data.System_Clock = 1;
	} 

    if(RCIF && RCIE) //说明:RCIF 由硬件决定是否清零,不必软件清除
    {
        if (1 == FERR) //帧错误?
        {
            Receive_Data_Temp = RCREG; //读取当前发生错误的接收结果,并丢弃
        } 
		else 
		{					
			Receive_Data_Temp = RCREG ; //读取接收结果			
			if(0 == Flag_Data.Receive_Enable)
			{								
				if(0x5A == Receive_Data_Temp)//接收到帧头并且未接收到数据
				{	
					No_Receive_Time_Counter = 0;
					Flag_Data.Receive_Enable = 1; 
					Receive_Data[0] = 0x5A;
					Receive_Num = 1;
				}			
			}
			else
			{
                if(0 == Flag_Data.Receive_End)
				{        
                    Receive_Data[Receive_Num] = Receive_Data_Temp;
                    if(++Receive_Num >= RECEIVE_iCNT) 
					{
                        Flag_Data.Receive_End = 1;
                    }
                }
			}			
		}
        if (1 == OERR) //溢出错误?
        {
			CREN = 0;
			CREN = 1; //重新复位接收模块,令OERR 标志清零
        }
    } 
}