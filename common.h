/* 
 * File:   common.h
 * Author: ZhiGuoXia
 *
 * Created on 2015年5月6日, 下午8:35
 */

#ifndef COMMON_H
#define	COMMON_H


//----------------------------------------
//重新命名数据类型
//----------------------------------------
typedef unsigned char uint8;    //unsigned 8-bit definition
typedef signed char   int8;     //signed 8-bit definition
typedef unsigned int  uint16;   //unsigned 16-bit definition
typedef signed int    int16;    //signed 16-bit definition
typedef unsigned long uint32;   //unsigned 32-bit definition
typedef signed long   int32;    //signed 32-bit definition
//typedef bit			  		bool;

typedef union
{
  uint8 int_data;
  struct
  {
    uint8   bit0:1;
    uint8   bit1:1;
    uint8   bit2:1;
    uint8   bit3:1;
    uint8   bit4:1;
    uint8   bit5:1;
    uint8   bit6:1;
    uint8   bit7:1;
  }byte;
} int_bit;
//----------------------------
//插入文件包
//----------------------------
#include <pic.h>
#include <pic16f1933.h>
#include <math.h>
#include <stdio.h>
#include "init.h"
#include "time.h"
#include "ad.h"
#include "usart.h"
#include "control.h"
#include "initerrupt.h"
//#include "const_data.h"
#include "data.h"
//#include "test.h"

#endif

