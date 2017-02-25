/*
*********************************************************************************************************
* Copyright (c) 2016, XXXX公司
* All rights reserved.
*
* 文件名称: UltrasonicWave.c
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
#include "UltrasonicWave.h"
#include "usart.h"
#include "timer.h"
#include "delay.h"
#include "led.h"
#include "stmflash.h"
#include "Posture.h"
#include "User_Config.h"

static uint8_t g_sonar_ch = 0;  /**< Active sonar channel index. */

sonar_channel_t g_sonar[CONFIG_SONAR_CHANNEL_NUM];

static const sonar_cfg_t g_sonar_cfg[CONFIG_SONAR_CHANNEL_NUM] =
{
    {TRIG_PORT1, TRIG_PIN1, ECHO_PORT1, ECHO_PIN1, ECHO_PIN1_EXIT_NO},
    {TRIG_PORT2, TRIG_PIN2, ECHO_PORT1, ECHO_PIN2, ECHO_PIN2_EXIT_NO},
    {TRIG_PORT3, TRIG_PIN3, ECHO_PORT1, ECHO_PIN3, ECHO_PIN3_EXIT_NO},

};


/*雷达报警标记*/
uint8_t g_u8ObstFlg = 0;

uint8_t g_u8ObstCheckFlg = 0x07;

uint8_t g_u8CmdBuf[5]={0};

static void SONAR_TRIG(uint8_t ch, uint8_t level);

/*
*********************************************************************************************************
*
* 函数名称:	void UltrasonicWave_Configuration(void)
* 作 用 域:
* 功能描述:	超声波雷达初始化
* 输入参数: 
* 输出参数:
* 全局变量: 
* 返 回 值:
* 备    注：
* 版    本: Ver 1.0
* 原 作 者: Iwen.Liu
* 完成日期: 2016.12.20
*
*********************************************************************************************************
*/
void UltrasonicWave_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);			//关闭jtag
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = TRIG_PIN1 | TRIG_PIN2 | TRIG_PIN3;	//PC8接TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     		//设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	                 		//初始化外设GPIO 

	GPIO_InitStructure.GPIO_Pin = ECHO_PIN1 | ECHO_PIN2 | ECHO_PIN3;	//PC7接ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 				//设为输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	         
	GPIO_Init(ECHO_PORT,&GPIO_InitStructure);						 	//初始化GPIOA

	//GPIOE.11	  中断线以及中断初始化配置
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource11);

//	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
//	
//	//GPIOE.13
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource13);

//	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
//	
//	//GPIOE.15
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource15);

//	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	SONAR_TRIG(0, LOW);
	SONAR_TRIG(1, LOW);
	SONAR_TRIG(2, LOW);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    memset(g_sonar, 0, sizeof(g_sonar));
}

/**
 @brief Sonar trigger function.
 @param ch    - sonar channel index, 1 ~ CONFIG_SONAR_CHANNEL_NUM-1.
 @param level - sonar trigger level.
 @return none.
 */
static void SONAR_TRIG(uint8_t ch, uint8_t level)
{
    if (level == HIGH)
    {
        GPIO_SetBits(g_sonar_cfg[ch].trig_port, g_sonar_cfg[ch].trig_pin);
    }
    else
    {
        GPIO_ResetBits(g_sonar_cfg[ch].trig_port, g_sonar_cfg[ch].trig_pin);
    }
}

/**
 @brief Shutdown a sonar channel.
 @param ch - channel number to be shutdown, 0 ~ CONFIG_SONAR_CHANNEL_NUM.
 @return none.

 */
static void sonar_shutdown(uint8_t ch)
{
    EXTI_InitTypeDef exti;

    if (ch >= CONFIG_SONAR_CHANNEL_NUM)
    {
        return ;
    }

    SONAR_TRIG(ch, LOW);

    /* Configure EXTI lines. */
    exti.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    exti.EXTI_LineCmd = DISABLE;
    exti.EXTI_Line    = EXTI_LINE(g_sonar_cfg[ch].exti_line);
    EXTI_Init(&exti);
    return ;
}

/**
 @brief Trigger a sonar channel.
 @param ch - channel number to be triggered, 1 ~ CONFIG_SONAR_CHANNEL_NUM.
 @return none.

 */
static void sonar_trigger(uint8_t ch)
{
    EXTI_InitTypeDef exti;

    if (ch >= CONFIG_SONAR_CHANNEL_NUM)
    {
        return ;
    }

    /* Send trigger wave. */
    SONAR_TRIG(ch, HIGH);
    _delay_us(20);
    SONAR_TRIG(ch, LOW);

    /* Prepare for echo rising edge interrupt. */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, g_sonar_cfg[ch].exti_line);

    /* Configure EXTI lines. */
    exti.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    exti.EXTI_LineCmd = ENABLE;
    exti.EXTI_Line    = EXTI_LINE(g_sonar_cfg[ch].exti_line);
    EXTI_Init(&exti);
    return ;
}

static uint8_t SONAR_ECHO(uint8_t ch)
{
    return GPIO_ReadInputDataBit(g_sonar_cfg[ch].echo_port, g_sonar_cfg[ch].echo_pin);
}


/*
*********************************************************************************************************
*
* 函数名称:	void EXTI15_10_IRQHandler(void)
* 作 用 域:
* 功能描述:	
* 输入参数: 
* 输出参数:
* 全局变量: 
* 返 回 值:
* 备    注：
* 版    本: Ver 1.0
* 原 作 者: Iwen.Liu
* 完成日期: 2016.12.20
*
*********************************************************************************************************
*/
uint8_t g_u8ObsCount = 0;
void EXTI15_10_IRQHandler(void)
{
	//delay_us(10);		                      //延时10us
	//g_s8ObstFlg = 0;
	uint8_t  id;
    uint8_t  status;
    uint32_t line;
    TIM_TimeBaseInitTypeDef tim_base;

    /* Get EXTI for the active channel. */
    line = EXTI_LINE(g_sonar_cfg[g_sonar_ch].exti_line);
    if (EXTI_GetITStatus(line) != RESET)
    {
        EXTI_ClearITPendingBit(line);

        status = SONAR_ECHO(g_sonar_ch);
        if (status == HIGH)
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
            /* Start counter timer. */
            tim_base.TIM_Period = 60000;
            tim_base.TIM_Prescaler = (SYSTICK_1MS_TICKS/1000-1);
            tim_base.TIM_ClockDivision = 0;
            tim_base.TIM_CounterMode = TIM_CounterMode_Up;

            TIM_TimeBaseInit(SONAR_TIMER, &tim_base);
            TIM_SetCounter(SONAR_TIMER, 0);
            TIM_Cmd(SONAR_TIMER, ENABLE);
        }
        else
        {
            /* Stop counter timer. */
            TIM_Cmd(SONAR_TIMER, DISABLE);

            id = g_sonar[g_sonar_ch].id;
            if (id >= CONFIG_SONAR_SAMPLE_SIZE)
            {
                id = 0;
            }
            g_sonar[g_sonar_ch].sample[id++] = TIM_GetCounter(SONAR_TIMER);
            g_sonar[g_sonar_ch].state++;    /* Move to next state. */
            g_sonar[g_sonar_ch].id = id;
            if (g_sonar[g_sonar_ch].cnt < CONFIG_SONAR_SAMPLE_SIZE)
            {
                g_sonar[g_sonar_ch].cnt++;
            }
        }
    }
	
	
}

/*
@brief Calculate sonar distance by sample value.
 @param ch - sonar channel, 0 ~ CONFIG_SONAR_CHANNEL_NUM
 @return none.

 This distance is in cm.
*/
static void sonar_distance(uint8_t ch)
{
    uint8_t  i;
    uint32_t avg;
    float    d;

    if (ch >= CONFIG_SONAR_CHANNEL_NUM)
    {
        return ;
    }

    avg = 0;
    for (i = 0; i < g_sonar[ch].cnt; i++)
    {
        avg += g_sonar[g_sonar_ch].sample[i];
    }
    if (i > 0)
    {
        avg /= i;
    }
    else
    {
        avg = 0;
    }

    d = CONFIG_SONAR_COE_A + CONFIG_SONAR_COE_B * CONFIG_SONAR_COE_T;
    d *= avg / 2 / 10000.0;

    g_sonar[g_sonar_ch].distance = (uint32_t)d;
	
	if(g_sonar[g_sonar_ch].distance < g_sonar[g_sonar_ch].obsdistance)
	{
		g_sonar[g_sonar_ch].obsflg = 1;
		g_u8ObstFlg |= 1<< g_sonar_ch;
	}
	else
	{
		g_sonar[g_sonar_ch].obsflg = 0;
		g_u8ObstFlg &= ~(1<< g_sonar_ch);
	}
	
	if(g_sonar_ch == 2)
	{
		
	}
	

}

/**
 @brief ultrasonic sonar module heartbeat.
 @param None.
 @return None.
            ___                                                         ___
 TRIG   ___|   |_______________________________________________________|   |____
            >10us _   _   _   _   _   _   _   _          at least 10ms wait
 SENSOR _________| |_| |_| |_| |_| |_| |_| |_| |________________________________
                      Send 8 40KHz wave
                                                   ________
 ECHO   __________________________________________|        |____________________
                                                  10us ~ 18ms
 */
void sonar_heartbeat(void)
{
    if (g_sonar_ch >= CONFIG_SONAR_CHANNEL_NUM)
    {
        g_sonar_ch = 0;
        g_sonar[g_sonar_ch].state = SONAR_INIT;
    }

    switch (g_sonar[g_sonar_ch].state)
    {
        case SONAR_INIT:
            sonar_trigger(g_sonar_ch);
            g_sonar[g_sonar_ch].state++;    /* Move to next state. */
            g_sonar[g_sonar_ch].ticks = getms();
            break;
        case SONAR_MEASURE: /* Wait until measurement done. */
            if (getms() - g_sonar[g_sonar_ch].ticks > CONFIG_SONAR_TIMEOUT_MS)
            {
                /* Timeout. Abort measurement and move to next channel. */
                TIM_Cmd(SONAR_TIMER, DISABLE);
                sonar_shutdown(g_sonar_ch);
                g_sonar[g_sonar_ch].ticks = getms();
                g_sonar[g_sonar_ch].state = SONAR_IDLE;
                memset(g_sonar[g_sonar_ch].sample, 0, sizeof(g_sonar[g_sonar_ch].sample));
                g_sonar_ch++;
                if (g_sonar_ch >= CONFIG_SONAR_CHANNEL_NUM)
                {
                    g_sonar_ch = 0;
                }
                //drv_led_set(0, 0, 0);
            }
            break;
        case SONAR_DONE:    /* Measurement is done. */
            if (g_sonar[g_sonar_ch].cnt == 0)
            {
                break;
            }
            if (g_sonar[g_sonar_ch].cnt >= CONFIG_SONAR_SAMPLE_SIZE)
            {
                g_sonar[g_sonar_ch].cnt = CONFIG_SONAR_SAMPLE_SIZE;
            }
            sonar_distance(g_sonar_ch);
            g_sonar[g_sonar_ch].state++;
            /* Blink the led by channel. */
            //drv_led_set((g_sonar_ch%3)==0?0x10:0, (g_sonar_ch%3)==1?0x10:0, (g_sonar_ch%3)==2?0x10:0);
            //sonar_dbg("ch %d, distance %d\n", g_sonar_ch, g_sonar[g_sonar_ch].distance);
            break;
        case SONAR_EXIT:    /* Channel measurement is done. Move to next. */
            sonar_shutdown(g_sonar_ch);
            g_sonar[g_sonar_ch].ticks = getms();
            g_sonar[g_sonar_ch].state = SONAR_IDLE;
            g_sonar_ch++;
            if (g_sonar_ch >= CONFIG_SONAR_CHANNEL_NUM)
            {
                g_sonar_ch = 0;
            }
            break;
        default:
            g_sonar[g_sonar_ch].state = SONAR_INIT;
            break;
    }

    return ;
}

void SetObstDis(uint8_t dis1, uint8_t dis2, uint8_t dis3)
{
	g_sonar[0].obsdistance = dis1;
	g_sonar[1].obsdistance = dis2;
	g_sonar[2].obsdistance = dis3;
}

void WriteObstDisToFlash(uint16_t dis1, uint16_t dis2, uint16_t dis3)
{
	STMFLASH_Write(OBST0_DIS_ADDR,&dis1,1);
	STMFLASH_Write(OBST1_DIS_ADDR,&dis2,1);
	STMFLASH_Write(OBST2_DIS_ADDR,&dis3,1);
}

void WriteObstCheckToFlash(uint16_t _check_bit)
{
	STMFLASH_Write(OBST_BIT_CHECK_ADDR,&_check_bit,1);
}

void ObstDisFLASHCheck()
{
	if( STMFLASH_ReadHalfWord(OBST0_DIS_ADDR) < 100)
	{
		g_sonar[0].obsdistance = (uint8_t)STMFLASH_ReadHalfWord(OBST0_DIS_ADDR);
	}
	else
	{
		g_sonar[0].obsdistance = OBST_BASE_DIS;
	}
	
	if( STMFLASH_ReadHalfWord(OBST1_DIS_ADDR) < 100)
	{
		g_sonar[0].obsdistance = (uint8_t)STMFLASH_ReadHalfWord(OBST1_DIS_ADDR);
	}
	else
	{
		g_sonar[0].obsdistance = OBST_BASE_DIS;
	}
	
	if( STMFLASH_ReadHalfWord(OBST2_DIS_ADDR) < 100)
	{
		g_sonar[0].obsdistance = (uint8_t)STMFLASH_ReadHalfWord(OBST2_DIS_ADDR);
	}
	else
	{
		g_sonar[0].obsdistance = OBST_BASE_DIS;
	}
	
	if( (STMFLASH_ReadHalfWord(OBST_BIT_CHECK_ADDR) & 0x0f) < 7)
	{
		g_u8ObstCheckFlg = (uint8_t)STMFLASH_ReadHalfWord(OBST_BIT_CHECK_ADDR);
	}
	else
	{
		g_u8ObstCheckFlg = 0x07;
	}
}

/*****************************************************End Of Flie***************************************/
