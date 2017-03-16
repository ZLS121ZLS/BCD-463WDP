/*************************************************************
名称：ad.h  
版本：V1.0_130314
功能：石塚热敏电阻
说明：初始化
****************************************************************/
#ifndef AD_H
#define AD_H

//#define Ld_Hs_Tem_Data Ld_Hs_Room_Tem
#define NTC_iMIN   0
#define NTC_iMAX   80 
#define NTC_iCNT   81

#define NTC_tMIN		-400

#define NTC_AD_MAX 15933        //63735      	//>100
#define NTC_AD_MIN 441          //1767       	//<-50  大约

#define LC_AD_ADDRESS	0b00101101		//AN11  LC_AD


//uint16 Sampling_Times[6]={64,128,256,512,1024,2048};
#define SAMPLING_NUM    3//4//2//0//7//6//5//0// 
#define SAMPLING_DATA   512//1024//256//64//8192//4096//2048//64//256//

extern int8 Lc_Integer_Tem;
extern int8 Ld_Hs_Integer_Tem;
extern int8 Ld_Integer_Tem;					//冷冻室温度整数

extern int16 Lc_Room_Tem;
extern int16 Ld_Room_Tem;
extern int16 Ld_Hs_Room_Tem;


extern void ad_deal(void);

#endif
