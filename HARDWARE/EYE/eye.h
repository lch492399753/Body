#ifndef __EYE_H
#define __EYE_H

#include "sys.h"

#define RCK1(x)	GPIOB->ODR = (GPIOB->ODR&~GPIO_Pin_3)	| (x ? GPIO_Pin_3 : 0)
#define CLK1(x)	GPIOB->ODR = (GPIOB->ODR&~GPIO_Pin_4)	| (x ? GPIO_Pin_4 : 0)
#define DS1(x)	GPIOB->ODR = (GPIOB->ODR&~GPIO_Pin_5)	| (x ? GPIO_Pin_5 : 0)

#define RCK(x)	GPIOB->ODR = (GPIOB->ODR&~GPIO_Pin_6)	| (x ? GPIO_Pin_6 : 0)
#define CLK(x)	GPIOB->ODR = (GPIOB->ODR&~GPIO_Pin_7)	| (x ? GPIO_Pin_7 : 0)
#define DS(x)	GPIOB->ODR = (GPIOB->ODR&~GPIO_Pin_8)	| (x ? GPIO_Pin_8 : 0)

void ExpressShow(uint8_t _expressID);
void EYE_GPIO_Configuration(void);
extern uint8_t g_u8ExpressID;

enum _expressid
{
	EXPRESS1 = 1,
	EXPRESS2,
	EXPRESS3,
	EXPRESS4,
	EXPRESS5,
	EXPRESS6,
	EXPRESS7,
	EXPRESS8,
	EXPRESS9,
	EXPRESS10,
};

#endif
