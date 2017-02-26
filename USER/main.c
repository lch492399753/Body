#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "exti.h"
#include "timer.h"
#include "UltrasonicWave.h"
#include "i2c.h"
#include "CmdPrase.h"
#include "Posture.h"
#include "eye.h"
#include "ADC.h"

int main(void)
{
	int8_t count = 0;
	uint8_t index = 0;
	uint32_t u32ToggleTm = 0;
	delay_init();	          						//��ʱ��ʼ��
	NVIC_Configuration();
	//bsp_InitUart();
	
	PostureInit();
	TIM4_Int_Init(49,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
	LED_Init();		            					//LED�˿ڳ�ʼ��
	//KEY_Init();               						//�����˿ڳ�ʼ��
	//I2C_Configuration();							//IIC��ʼ��
	//Timerx_Init(5000,7199);							//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	UltrasonicWave_Configuration();					//�Գ�����ģ���ʼ��

	EYE_GPIO_Configuration();

	Init_All_Periph();
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	MotorInit();
//	R_ARM_SPEED(200) ;
//	R_ARM_TURN_UP();
	ArmRightAngel.StopFlg = 1;
	ArmRightAngel.DirFlg = 2;
	
	
	GetStartAngel();
//	ObstDisFLASHCheck();
	AngelFLASHCheckAndGet();
	uart_init(115200);
	while(1)
	{
		ExpressShow(g_u8ExpressID);
		CMDPrase();
		sonar_heartbeat();
		if( (HeadAngel.ErrFlg & (FLASH_READ_ERR)) + 
			(WaistAngel.ErrFlg & (FLASH_READ_ERR) )+
			(ArmLeftAngel.ErrFlg & (FLASH_READ_ERR) ) + 
		    (ArmRightAngel.ErrFlg & (FLASH_READ_ERR) ) >0)
		{
			if( (getms() - u32ToggleTm) >500 )
			{
				u32ToggleTm = getms();
				ARStop(AR_ALL);
				TestLEDToggle();
			}
			
		}
		else
		{
			ARRun();
			
			if( (g_u8ObstFlg &0x07) != 0)
			{
				TEST_LED = 0;
			}
			else
			{
				TEST_LED = 1;
			}
		}
		
			
		
		
		
		
		
	}
}
