/*************************************************************
名称：display.h   
版本：V1.0_130314
功能：
说明：
****************************************************************/
#ifndef USART_H
#define USART_H

#define SEND_iCNT       10
#define SEND_iCNT_1     9

#define RECEIVE_iCNT    9
#define RECEIVE_iCNT_1  8

#define SL_MS	4
#define SD_MS	3
#define ZN_MS	2
#define JN_MS	1

extern uint8 Hj_Range;
extern uint8 Test_Mode_Data;

extern uint8 Receive_Num;

extern uint8 Receive_Data[RECEIVE_iCNT];
extern uint8 No_Receive_Time_Counter;

extern void usart_init(void);
extern void send_deal(void);
extern void comm_deal(void);

#endif
