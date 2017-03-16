/*************************************    
名称：data_load.h
版本：V1.0_090705
功能：数据存取
说明：
****************************************/
#ifndef DATA_LOAD_H
#define DATA_LOAD_H

#define UINT8_EEPROM_DATA_NUM     33         
#define UINT8_EEPROM_DATA_NUM_1   32

extern void data_clear(uint8 *data_n,uint8 *data_sum);
extern void data_read(void);
extern void data_write(void);
extern void data_deal(void);
extern void data_resume(void);
#endif
