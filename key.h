/**********************************************************
名称：key.h
版本：V1.0_130314
功能：按键处理
说明：
**************************************************************/ 
#ifndef KEY_DEAL_H
#define KEY_DEAL_H




	#define PORT_KEY	RB6
//	#define PORT_KEY	RB7 

//==============================================================================
#define KEY_NO_VALUE          0x00
//------------------------------------------------------------------------------


#define NO_KEY_STATE            0
#define KEY_STATE               1
#define KEY_KEY_STATE           2

extern void read_key(void);
extern void key_deal(void);

#endif
