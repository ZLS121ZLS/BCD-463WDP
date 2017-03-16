/*************************************************************
���ƣ�interrupt.c
�汾��V1.0_100601
���ܣ�
˵����
 ****************************************************************/
#include "common.h"
//==============================================================
uint8 Receive_Data_Temp;

void interrupt ISR(void) //�жϷ������
{
	if(TMR1IE && TMR1IF) 		//��TMR1 �ж�
	{	
		TMR1IF = 0; 					//���TMR1 �жϱ�־
		TMR1ON = 0;						//TMR1��ͣ
		TMR1L  = 0x0C;			//TMR1����ֵ//�����ڲ�ʱ�ӣ�FOSC/4����1MHz
		TMR1H  = 0xFE;			//1ms�Ķ�ʱΪ65536-1000=64536��1000*1us=1ms�ж�һ��
		//TMR1L  = 0x65;			//TMR1����ֵ//�����ڲ�ʱ�ӣ�FOSC/4����1MHz
		//TMR1H  = 0xFD;			//1ms�Ķ�ʱΪ65536-1000=64536��1000*1us=1ms�ж�һ��
		TMR1ON = 1; 
        Flag_Data.System_Clock = 1;
	} 

    if(RCIF && RCIE) //˵��:RCIF ��Ӳ�������Ƿ�����,����������
    {
        if (1 == FERR) //֡����?
        {
            Receive_Data_Temp = RCREG; //��ȡ��ǰ��������Ľ��ս��,������
        } 
		else 
		{					
			Receive_Data_Temp = RCREG ; //��ȡ���ս��			
			if(0 == Flag_Data.Receive_Enable)
			{								
				if(0x5A == Receive_Data_Temp)//���յ�֡ͷ����δ���յ�����
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
        if (1 == OERR) //�������?
        {
			CREN = 0;
			CREN = 1; //���¸�λ����ģ��,��OERR ��־����
        }
    } 
}