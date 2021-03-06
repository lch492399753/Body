#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "ring_buffer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/5/27
//版本：V1.3
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
////////////////////////////////////////////////////////////////////////////////// 

#define USART4_BUFF_SIZE 64

extern char g_cUSART4RxBuffer[USART4_BUFF_SIZE];

extern RINGBUFF_T g_tUSART4RxRingBuf;

	

void uart_init(unsigned int bound);
void USART4_SendStrings(unsigned char* str,uint8_t len);
#endif
