#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "ring_buffer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/5/27
//�汾��V1.3
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
////////////////////////////////////////////////////////////////////////////////// 

#define USART4_BUFF_SIZE 64

extern char g_cUSART4RxBuffer[USART4_BUFF_SIZE];

extern RINGBUFF_T g_tUSART4RxRingBuf;

	

void uart_init(unsigned int bound);
void USART4_SendStrings(unsigned char* str,uint8_t len);
#endif
