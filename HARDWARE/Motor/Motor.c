#include "Motor.h"

/*
*
*/
static void Head_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);				
	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5);
}

static void Arm_Left_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5);						
	
}

static void Arm_Right_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	GPIO_SetBits(GPIOB,GPIO_Pin_2);	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 
	GPIO_SetBits(GPIOE,GPIO_Pin_7);	

}

static void Chest_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 
	GPIO_SetBits(GPIOE,GPIO_Pin_10|GPIO_Pin_11);	
}

static void Waist_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 
	GPIO_SetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9);
}

static void LIMIT_SENSOR(void)
{ 
	/*
	*	�ز� ����λ	-> PB12 
	*	�ز� ����λ -> PB13 
	*	���� ����λ -> PB14 
	*	���� ����λ -> PB15 
	*/
	
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	
}

static void PWM_Init(u16 arr,u16 psc)
{
	/*
	*	Head_PWM  -> PA6
	*	Waist_PWM -> PB1
	*	Chest_PWM -> PA0
	*	LARM_PWM  -> PA7
	*	RARM_PWM  -> PB0
	*	TRAY_PWM  -> PA1
	*/
	GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_DeInit(TIM3);
	TIM_DeInit(TIM2);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2, ENABLE);    //ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	
    //��ʼ��TIM3
    TIM_TimeBaseStructure.TIM_Period = arr; 						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =psc; 						//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    //��ʼ��TIM3 Channel/2/3/4 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2

}

/*����*/
void OpenChest()
{
    if( (CHEST_DOWN_LIMT == POS_NOARIVE) && (TRAY_OUT_LIMT == POS_NOARIVE) )
    {
        CHEST_DOWN();
        TRAY_STOP();
    }

    if( (CHEST_DOWN_LIMT == POS_ARIVEED) && (TRAY_OUT_LIMT == POS_NOARIVE) )
    {
        TRAY_DOWN();
        CHEST_STOP();
    }
    if( (CHEST_DOWN_LIMT == POS_ARIVEED) && (TRAY_OUT_LIMT == POS_ARIVEED) )
    {
        CHEST_STOP();
        TRAY_STOP();
    }

}

/*�ظ�*/
void CloseChest()
{
    if( (CHEST_UP_LIMT == POS_NOARIVE) && (TRAY_IN_LIMT == POS_NOARIVE) )
    {
        TRAY_UP();
        CHEST_STOP();
    }
    if( (CHEST_UP_LIMT == POS_NOARIVE) && (TRAY_IN_LIMT == POS_ARIVEED) )
    {
		CHEST_UP();	
        TRAY_STOP();
    }
    if( (CHEST_UP_LIMT == POS_ARIVEED) && (TRAY_IN_LIMT == POS_ARIVEED) )
    {
        TRAY_STOP();
        CHEST_STOP();
    }
}

void MotorInit()
{
	Head_GPIO_Init();
	Arm_Left_GPIO_Init();
	Arm_Right_GPIO_Init();
	Chest_GPIO_Init();
	Waist_GPIO_Init();
	LIMIT_SENSOR();
	PWM_Init(719,99);
}
