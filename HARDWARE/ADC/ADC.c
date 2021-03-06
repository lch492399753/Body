#include "delay.h"
#include "sys.h"
#include "ADC.h"

__IO uint16_t AD_Value[N][M];
uint16_t After_filter[M];
int nn;

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //PC0/1/2/3/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;

    RCC_DeInit(); //RCC ????
    RCC_HSEConfig(RCC_HSE_ON);   //??HSE
    HSEStartUpStatus = RCC_WaitForHSEStartUp(); //??HSE???
    if(HSEStartUpStatus == SUCCESS)
    {
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //Enable Prefetch Buffer
        FLASH_SetLatency(FLASH_Latency_2); //Set 2 Latency cycles
        RCC_HCLKConfig(RCC_SYSCLK_Div1); //AHB clock = SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1); //APB2 clock = HCLK
        RCC_PCLK1Config(RCC_HCLK_Div2); //APB1 clock = HCLK/2
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6); //PLLCLK = 12MHz * 6 = 72 MHz
        RCC_PLLCmd(ENABLE); //Enable PLL
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //Wait till PLL is ready
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //Select PLL as system clock source
        while(RCC_GetSYSCLKSource() != 0x08); //Wait till PLL is used as system clock source

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB
                               | RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO |RCC_APB2Periph_USART1, ENABLE);  //??ADC1????,??????

        RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC????????14M
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //??DMA??

    }
}
void ADC1_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    ADC_DeInit(ADC1); //??? ADC1 ????????????


    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC????:ADC1?ADC2???????
    ADC_InitStructure.ADC_ScanConvMode =ENABLE; //???????????
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //?????????????
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //????????
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC?????
    ADC_InitStructure.ADC_NbrOfChannel = M; //?????????ADC?????
    ADC_Init(ADC1, &ADC_InitStructure); //??ADC_InitStruct???????????ADCx????




    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5);


    // ??ADC?DMA??(???DMA??,??????DMA?????)
    ADC_DMACmd(ADC1, ENABLE);


    ADC_Cmd(ADC1, ENABLE); //?????ADC1

    ADC_ResetCalibration(ADC1); //?????ADC1??????

    while(ADC_GetResetCalibrationStatus(ADC1)); //??ADC1??????????,???????


    ADC_StartCalibration(ADC1); //????ADC1?????

    while(ADC_GetCalibrationStatus(ADC1)); //????ADC1?????,???????


}


void DMA_Configuration(void)
{

    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel1); 										//
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; 		//
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; 			//DMA?????
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 				//????????????
    DMA_InitStructure.DMA_BufferSize = N*M; 						//DMA???DMA?????
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //?????????
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 		//?????????
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //?????16?
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //?????16?
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 				//?????????
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; 			//DMA?? x??????
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 					//DMA??x????????????
    DMA_Init(DMA1_Channel1, &DMA_InitStructure); 					//??DMA_InitStruct?????????DMA???

}

void Init_All_Periph(void)
{

    RCC_Configuration();

    GPIO_Configuration();

    ADC1_Configuration();

    DMA_Configuration();


    //USART1_Configuration();
    //USART_Configuration(9600);


}

u16 GetVolt(u16 advalue)
{
    return (u16)(advalue * 330 / 4096); //???????100?,????????
}

void filter(void)
{
    int sum = 0;
    u8 count;
	uint16_t u16max=0,u16min=0;
    for(nn=0; nn<4; nn++)
    {
		u16max = AD_Value[0][nn];
		u16min = AD_Value[0][nn];
        for(count=0; count<N; count++)
        {
			if(AD_Value[count][nn] > u16max)
			{
				u16max = AD_Value[count][nn];
			}
			if(AD_Value[count][nn] < u16min)
			{
				u16min = AD_Value[count][nn];
			}
			
            sum += AD_Value[count][nn];

        }
		sum -= u16max;
		sum -= u16min;
        After_filter[nn]=sum/(N-2);

        sum=0;
    }

}
