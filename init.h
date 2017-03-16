/* 
 * File:   init.h
 * Author: ZhiGuoXia
 *
 * Created on 2015年5月6日, 下午8:46
 */

#ifndef INIT_H
#define	INIT_H

#define NEW_POWER   1


#define PR_FREQ_100      77      //85 2550
#define CCPR_FREQ_100    38      //
#define PR_FREQ_125     62
#define CCPR_FREQ_125   31

#define wdt_init()	WDTCON = 0b00010101//1s
#define clr_wdt()	asm("clrwdt") //清看门狗

    //--------------------------------------------------------------------

typedef struct
{
	uint8 System_Clock : 1;
    //----------------------------------------------
    uint8 Read_Enable : 1;//读使能
    uint8 Write_Enable : 1; //写使能
    uint8 Def_Freq_State : 1;//默认输出频率
	uint8 Key : 1;//按键使能
	uint8 Key_Temp : 1;
	uint8 Key_Old : 1;
	uint8 Jn_Test : 1;
	uint8 Lc_Tem_Err : 1;  //冷藏传感器故障
    uint8 Ld_Tem_Err : 1;//冷冻传感器故障
	uint8 Ld_Hs_Tem_Err : 1;//化霜传感器故障
	uint8 Hj_Tem_Err : 1;//化霜加热故障
	uint8 Hs_Err : 1;//化霜故障
	uint8 Fan_Err : 1;//风机故障
    uint8 Comm_Err : 1;	//压缩机故障
	uint8 Fan_Door_State : 1;//判定风门状态  0 关闭  1打开
	//uint8 Flz_Out;
	uint8 Def_Out   : 1;
//	uint8 Comp_140hz_Enable : 1;
	uint8 Led_All_Off:1;
//    uint8 First_Def : 1;
//	uint8 Data_Save : 1;
	uint8 Data_Change : 1;//数据改变

	uint8 Short_Time : 1;
	//uint8 Light_Delay : 1;
    uint8 Forst_Freq_S6 : 1;
//	uint8 Buzzer_Clock : 1;
//	uint8 Buzzer_Power : 1;
//	uint8 Buzzer_Control : 1;

	
	uint8 Comm_Enable : 1;
	uint8 Receive_End : 1;

	uint8 Comm_Err_Temp : 1;
	uint8 Receive_Enable    : 1;

    uint8 First_Comp_End   : 1;

  uint8 Normal_Work : 1;  //正常工作
// 
//  uint8 Alarm : 1;
//  uint8 Alarm_End : 1;

	uint8 Door : 1;
	uint8 Door_Old : 1;
	uint8 Door_Temp : 1;
	//uint8 Flz : 1;

	/*
	uint8 Door1 : 1;
	uint8 Door1_Old : 1;
	uint8 Door1_Temp : 1;
	uint8 Door2 : 1;
	uint8 Door2_Old : 1;
	uint8 Door2_Temp : 1;
*/
	uint8 Def_Prepare : 1;
	uint8 Def : 1;
	uint8 Comp : 1;
	uint8 Fan : 1;
	uint8 No_Comp_Time_End : 1;
	uint8 Db_Heat : 1;
	uint8 Light : 1;//开灯 0关闭 1打开
	//uint8 Light1 : 1;
	//uint8 Light2 : 1; 
	uint8 Fan_Door_Heat : 1;
	uint8 Light_Off : 1;//判定灯是否关闭
	//uint8 Light1_Off : 1;
	//uint8 Light2_Off : 1;
	uint8 Control_Enable : 1;
	uint8 Fan_Delay_Time_End : 1;
	uint8 Fan_Door_Delay_Time_End : 1;//判定风门延时是否结束
	uint8 Fan_Fb : 1;
	uint8 Fan_Fb_Old : 1;
	//uint8 Fan_Fb_Enable : 1;
	//uint8 No_Fan_Fb_Enable : 1;
	uint8	Fan_Err_Temp : 1; 
	uint8 Fan_Test : 1;
	uint8 Fan_Door : 1;//判定风门 1关闭处理  0打开处理
	uint8 Fan_Door_Open_Enable : 1;
	uint8 Fan_Door_Close_Enable : 1;
    uint8 Door_Open_70sec       : 1;
	uint8 Power_On_End : 1;
    uint8 Comp_Freq_State;
    uint8 Fan_Door_Clock    : 1;
    uint8 Light_Enable      : 1;
    uint8 Door_Key_Clock    : 1;
    uint8 Close_Jn_Test_Enable  : 1;

  } flag_bit;



extern flag_bit Flag_Data;
extern void system_init(void);

#endif

