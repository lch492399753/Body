#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
//����delay_us,delay_ms
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/5/27
//�汾��V1.2
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.2�޸�˵��
//�������ж��е��ó�����ѭ���Ĵ���
//��ֹ��ʱ��׼ȷ,����do while�ṹ!

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





























