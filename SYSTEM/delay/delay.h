#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//使用SysTick的普通计数模式对延迟进行管理
//包括delay_us,delay_ms
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/5/27
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.2修改说明
//修正了中断中调用出现死循环的错误
//防止延时不准确,采用do while结构!

//////////////////////////////////////////////////////////////////////////////////
#define CPU_FREQ 72000000L
#define SYSTICK_1MS_TICKS    (CPU_FREQ/1000)

typedef int8_t         _s8;
typedef uint8_t        _u8;

typedef int16_t        _s16;
typedef uint16_t       _u16;

typedef int32_t        _s32;
typedef uint32_t       _u32;

typedef int64_t        _s64;
typedef uint64_t       _u64;


void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void softdelay_calibrate(void);
void _delay_us(volatile uint32_t us);
uint32_t getms(void);
uint64_t getus(void);
void _delay_us(volatile uint32_t us);
#endif





























