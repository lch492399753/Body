#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define CHEST_UP();			{PEout(10)=0;PEout(11)=1;}
#define CHEST_DOWN();		{PEout(10)=1;PEout(11)=0;}
#define CHEST_STOP();		{PEout(10)=1;PEout(11)=1;}
#define CHEST_BRK();		{PEout(10)=0;PEout(11)=0;}

#define TRAY_UP();			{PEout(13)=1;PEout(14)=0;}
#define TRAY_DOWN();		{PEout(13)=0;PEout(14)=1;}
#define TRAY_STOP();		{PEout(13)=1;PEout(14)=1;}
#define TRAY_BRK();			{PEout(13)=0;PEout(14)=0;}

#define HEAD_TURN_LEFT();	{PAout(4)=1;PAout(5)=0;}
#define HEAD_TURN_RIGHT();	{PAout(4)=0;PAout(5)=1;}
#define HEAD_STOP();		{PAout(4)=1;PAout(5)=1;}
#define HEAD_BRK();			{PAout(4)=0;PAout(5)=0;}

#define L_ARM_TURN_UP();	{PCout(4)=1;PCout(5)=0;}
#define L_ARM_TURN_DOWN();	{PCout(4)=0;PCout(5)=1;}
#define L_ARM_STOP();		{PCout(4)=1;PCout(5)=1;}
#define L_ARM_BRK();		{PCout(4)=0;PCout(5)=0;}

#define R_ARM_TURN_UP();	{PBout(2)=1;PEout(7)=0;}
#define R_ARM_TURN_DOWN();	{PBout(2)=0;PEout(7)=1;}
#define R_ARM_STOP();		{PBout(2)=1;PEout(7)=1;}
#define R_ARM_BRK();		{PBout(2)=0;PEout(7)=0;}

#define WAIST_TURN_LEFT();	{PEout(8)=1;PEout(9)=0;}
#define WAIST_TURN_RIGHT();	{PEout(8)=0;PEout(9)=1;}
#define WAIST_STOP();   	{PEout(8)=1;PEout(9)=1;}
#define WAIST_BRK();   		{PEout(8)=0;PEout(9)=0;}

#define TRAY_SPEED(X)  		TIM_SetCompare2(TIM2,X)
#define CHEST_SPEED(X)  	TIM_SetCompare1(TIM2,X)

#define WAIST_SPEED(X)  	TIM_SetCompare4(TIM3,X) 
#define R_ARM_SPEED(X)  	TIM_SetCompare3(TIM3,X)
#define L_ARM_SPEED(X)  	TIM_SetCompare2(TIM3,X)
#define HEAD_SPEED(X)  		TIM_SetCompare1(TIM3,X)

#define CHEST_DOWN_LIMT		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define CHEST_UP_LIMT		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define TRAY_OUT_LIMT		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define TRAY_IN_LIMT		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)

#define POS_ARIVEED	1
#define POS_NOARIVE	0


void OpenChest(void);
void CloseChest(void);
void MotorInit(void);

#endif
