/*************************************    
名称：data.h
版本：V1.0_130314
功能：数据存取
说明：
****************************************/
#ifndef DATA_H
#define DATA_H

#define SET_LC_TEM  Data[0]
#define SET_LD_TEM  Data[1]
#define SET_MS_DATA	Data[2]//设置模式数据
#define SET_FLZ_DATA Data[3]
//#define JN_TSET_FLAG Data[4]

extern int8 Data[7];//
extern void data_deal(void);

#endif
