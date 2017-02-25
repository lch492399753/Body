/*
*********************************************************************************************************
* Copyright (c) 2016, XXXX公司
* All rights reserved.
*
* 文件名称: UltrasonicWave.h
* 文件标识:
* 摘    要: 3路超声波雷达距离检测
*
* 版    本: Ver 1.00
* 原 作 者:	IWen.Liu
* 完成日期: 2016.12.20
*
* 版    本:
* 作    者:
* 完成日期:
* 更改说明:
*
*********************************************************************************************************
*/

#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "sys.h"
#include "stdint.h"
#include "string.h"

#define HIGH 1
#define LOW 0


#define CONFIG_SONAR_COE_A              331.45  /*< Sonar distance coefficient A. */
#define CONFIG_SONAR_COE_B              0.607   /*< Sonar distance coefficient B. */
#define CONFIG_SONAR_COE_T              20      /*< Sonar distance coefficient T at 20. */

#define CONFIG_SONAR_CHANNEL_NUM 3				//超声波雷达通道数

#define CONFIG_SONAR_SAMPLE_SIZE 3
#define CONFIG_SONAR_TIMEOUT_MS 50


/*雷达控制引脚定义*/
#define	TRIG_PORT1 	GPIOD
#define	TRIG_PORT2 	GPIOD
#define	TRIG_PORT3 	GPIOD

#define TRIG_PORT	GPIOD
#define ECHO_PORT	GPIOD

#define	ECHO_PORT1	GPIOD		
#define	ECHO_PORT2	GPIOD
#define	ECHO_PORT3	GPIOD

#define	TRIG_PIN1	GPIO_Pin_9	//TRIG1 
#define	TRIG_PIN2	GPIO_Pin_10	//TRIG2   
#define	TRIG_PIN3	GPIO_Pin_11	//TRIG3   

#define	ECHO_PIN1	GPIO_Pin_12	//ECHO1   
#define	ECHO_PIN2	GPIO_Pin_13	//ECHO2 
#define	ECHO_PIN3	GPIO_Pin_14	//ECHO3 

#define ECHO_PIN1_EXIT_NO 	12
#define ECHO_PIN2_EXIT_NO 	13
#define ECHO_PIN3_EXIT_NO 	14

#define EXTI_LINE(n)        (EXTI_Line0 << (n))

#define SONAR_TIMER         TIM6

/**
 @brief sonar channel info structure.
 */
typedef struct _sonar_channel {
    uint8_t  state;                             /**< Sonar sensor state. */
    uint8_t  id;                                /**< Sonar smaple buffer id. */
    uint8_t  cnt;                               /**< Sonar smaple count. */
    uint16_t sample[CONFIG_SONAR_SAMPLE_SIZE];  /**< Sonar sample buffer. */
    uint32_t ticks;                             /**< Sonar sample ticks. */
    uint32_t distance;                          /**< Sonar sampel result. */
	uint8_t	 obsflg;
	uint16_t obsdistance;
} sonar_channel_t;

/**
 @brief sonar channel configuration.
 */
typedef struct _sonar_cfg {
    GPIO_TypeDef *trig_port;        /**< Sonar channel trigger IO port. */
    uint16_t      trig_pin;         /**< Sonar channel tirgger IO pin. */
    GPIO_TypeDef *echo_port;        /**< Sonar channel echo input IO port. */
    uint16_t      echo_pin;         /**< Sonar channel echo input IO pin. */
    uint32_t      exti_line;        /**< Sonar channel echo input interrupt line. */
} sonar_cfg_t;

/**
 @brief Sonar sensor states.
 */
enum _sonar_state {
    SONAR_IDLE = 0,
    SONAR_INIT,
    SONAR_MEASURE,
    SONAR_DONE,
    SONAR_EXIT,
};




extern uint8_t g_u8ObstFlg;
extern uint8_t g_u8ObstCheckFlg;
extern sonar_channel_t g_sonar[CONFIG_SONAR_CHANNEL_NUM];

void UltrasonicWave_Configuration(void);               //对超声波模块初始化
void UltrasonicWave_StartMeasure1(void);                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
void UltrasonicWave_StartMeasure2(void); 
void UltrasonicWave_StartMeasure3(void); 
void sonar_heartbeat(void);
void SetObstDis(uint8_t dis1, uint8_t dis2, uint8_t dis3);
void WriteObstDisToFlash(uint16_t dis1, uint16_t dis2, uint16_t dis3);
void ObstDisFLASHCheck(void);
void WriteObstCheckToFlash(uint16_t _check_bit);
#endif /* __UltrasonicWave_H */

/***********************************End Of File*********************************************************/
