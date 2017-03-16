/*************************************************************
名称：control.h  
版本：V1.0_130314
功能：
说明：初始化
****************************************************************/
#ifndef CONTROL_H
#define CONTROL_H

#define REF_UP_RETURN       15
#define REF_DOWN_RETURN     -15
#define COMP_UP_RETURN      20
#define COMP_DOWN_RETURN    -20


#define FREQ_45         0
#define FREQ_55         1
#define FREQ_65         2
#define FREQ_75         3
#define FREQ_80         4
#define FREQ_90         5
#define FREQ_100		6
#define FREQ_115		7
#define FREQ_125		8

#define FREQ_S0	 FREQ_45
#define FREQ_S1	 FREQ_55
#define FREQ_S2	 FREQ_65
#define FREQ_S3	 FREQ_75
#define FREQ_S4	 FREQ_80
#define FREQ_S5	 FREQ_90
#define FREQ_S6	 FREQ_100
#define FREQ_S7	 FREQ_115
#define FREQ_S8	 FREQ_125

#define DEF_END_TEM     80

#define PORT_FLZ  			RA7
#define PORT_DOOR2          RA6
#define PORT_COMP_PWM		RA4
#define PORT_DB_HEAT 		RA3
#define PORT_DEF			RA2	
#define PORT_COMP 			RA1

#define PORT_LIGHT2             RB5
#define PORT_FAN_DOOR_HEAT      RB1
#define PORT_LIGHT				RB0

#define PORT_KEY                RB6

#define PORT_B					RC5
#define PORT_A					RC4
#define PORT_C					RB7

#define PORT_DOOR 		RC3
#define PORT_LIGHT1     RC2
#define PORT_FAN   		RC1
#define PORT_DOOR1  	RC0

#define PORT_FAN_FB		RA0




#define TRIS_LIGHT			TRISB0
#define TRIS_FAN			TRISC1
#define TRIS_COMP			TRISA4
#define TRIS_LIGHT1			TRISC2
#define TRIS_LIGHT2			TRISB5

#define CCPR_LIGHT			CCPR4L
#define CCPR_FAN			CCPR2L
#define CCPR_LIGHT1			CCPR1L
#define CCPR_LIGHT2			CCPR3L

#define PR_COMP     PR4
#define CCPR_COMP   CCPR5L

#define OUT 0
#define IN	1

#define TIME_2SEC       1
#define TIME_3SEC       2
#define TIME_4SEC       3
#define TIME_5SEC       4
#define TIME_6SEC       5
#define TIME_7SEC       6
#define TIME_8SEC       7
#define TIME_15SEC      14
#define TIME_30SEC      29
#define TIME_60SEC      59
#define TIME_80SEC      79
#define TIME_90SEC      89
#define TIME_120SEC     119

#define TIME_1MIN       59
#define TIME_2MIN       119
#define TIME_3MIN       179
#define TIME_8MIN       479    
#define TIME_10MIN      599
#define TIME_14MIN      839
#define TIME_15MIN      899     //1秒
#define TIME_16MIN      959
#define TIME_30MIN      1799    //1800  //1秒
#define TIME_45MIN      2699
#define TIME_120MIN     7199

#define TIME_2HOUR      119
#define TIME_4HOUR      239
#define TIME_5HOUR      299
#define TIME_7HOUR      419
#define TIME_10HOUR     599     //基准1分钟
#define TIME_12HOUR     719
#define TIME_24HOUR     1439
#define TIME_32HOUR     1919
#define TIME_50HOUR     2999
#define TIME_72HOUR     4319

#define PREPARE_TIME_30MIN  29
#define FAN_ERR_3HOUR   10799
#define MOVE_TIME_120MIN    119     //单位1分钟   

#define NUM_3   2

extern uint16 Comp_Move_Time;

extern void fandoor_open_deal(void);//风门打开处理
extern void fandoor_close_deal(void);//风门关闭处理
extern void def_begin_deal(void);
extern void comp_close_deal(void);
extern void control_deal(void);
extern void fan_door_step_deal(void);
extern void fan_fb_deal(void);
extern void light_deal(void);
extern void door_key_deal(void);
#endif
