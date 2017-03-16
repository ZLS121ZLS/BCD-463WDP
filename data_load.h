/*************************************    
���ƣ�data_load.h
�汾��V1.0_090705
���ܣ����ݴ�ȡ
˵����
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
