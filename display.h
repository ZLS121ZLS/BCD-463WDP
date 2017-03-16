/*************************************************************
名称：display.h   
版本：V1.0_130314
功能：
说明：
****************************************************************/
#ifndef DISPLAY_H
#define DISPLAY_H

#define   SEG_A		10		/*A  10*/
#define   SEG_b   11    /*b  11*/
#define   SEG_C   12    /*C  12*/
#define   SEG_d   13    /*d  13*/
#define   SEG_E	  14		/*E	 14*/
#define   SEG_F   15    /*F  15*/
#define   SEG_H   16    /*H  17*/
#define   SEG_J   17    /*J  19*/
#define   SEG_o   18    /*o  22*/
#define   SEG_P   19    /*P  29*/
#define   SEG_W   20    /*W  32*/
#define   SEG_X   21    /*X  33*/
#define   SEG__8  22    /*全显 38*/
#define   SEG__   23    /*不显  39*/



#define Led_Lc_C				Led_Data.byte.bit0
#define Led_Lc_Off			Led_Data.byte.bit1
#define Led_Ld_C				Led_Data.byte.bit2
#define Led_Ld_Neg			Led_Data.byte.bit3
#define Led_Sl          Led_Data.byte.bit4
#define Led_Sd          Led_Data.byte.bit5
#define Led_Zn          Led_Data.byte.bit6
#define Led_Jn          Led_Data.byte.bit7

#define Led_L1          Led_2_Data.byte.bit0
#define Led_L2_13				Led_2_Data.byte.bit1
#define Led_L4_14				Led_2_Data.byte.bit2
#define Led_L3          Led_2_Data.byte.bit3
#define Led_L5_6        Led_2_Data.byte.bit4
#define Led_L7_8        Led_2_Data.byte.bit5
#define Led_L9_10       Led_2_Data.byte.bit6
#define Led_L11_12      Led_2_Data.byte.bit7


extern int_bit Led_Data;
extern int_bit Led_2_Data;
extern uint8 Dig_Data[4];
extern uint8 Lc_Refurbish_Time;
extern uint8 Bw_Refurbish_Time;
extern uint8 Ld_Refurbish_Time;
extern uint8 Test_Mode_Data;//测试模式

extern void seg_deal(void);
extern void display_deal(void);


#endif
