#ifndef __BUZZER_H
#define __BUZZER_H
 
#define POWER_BUZZ	0
#define KEY_BUZZ		1
#define DOOR_BUZZ		2

#define PORT_BUZZ_POWER_OUT			RC4
#define PORT_BUZZ_CONTROL_OUT		RC5


#define DATA_3K_L		0xB3
#define DATA_3K_H		0xFE

#define DATA_3_5K_L	0xE2
#define DATA_3_5K_H	0xFE

#define DATA_4K_L		0x06
#define DATA_4K_H		0xFF
/*
#define DATA_3K_L		0xCC
#define DATA_3K_H		0xFE

#define DATA_3_5K_L	0xF5
#define DATA_3_5K_H	0xFE

#define DATA_4K_L		0x15
#define DATA_4K_H		0xFF
 */

extern uint8 Buzzer_Type;
extern uint8 Buzzer_State;
extern uint8 Buzzer_Power_Time_Counter;

extern void buzzer_power_deal(void);
 
#endif /* __BUZZER_H */

