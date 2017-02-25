#include "eye.h"

u8 const lie1[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
u8 const lie2[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
u8 const hang9[] = {0xFF,0xFF,0xFF,0x3F,0x3F,0x3C,0x3C,0x38,0x31,0x23,0x07,0x0F,0x1F,0x3F,0xFF,0xFF,};//叉号
u8 const hang8[] = {0xFF,0xFF,0xFF,0xFF,0xF1,0xE0,0xC0,0x81,0x03,0x81,0xC0,0xE0,0xF1,0xFF,0xFF,0xFF,};//心
u8 const hang7[] = {0xEF,0xFD,0xEB,0xC6,0xC1,0xBD,0x66,0x42,0x66,0xBD,0xC1,0xC6,0xEB,0xFD,0xEF,0xFF,};
u8 const hang6[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xE7,0xDB,0xA5,0xA5,0xDB,0xE7,0xFF,0xFF,0xFF,0xFF,0xFF,};
u8 const hang5[] = {0xFF,0xFF,0xFF,0xFF,0x3F,0x9F,0xCF,0xE7,0xF1,0xF8,0xFC,0xFE,0xFF,0xFF,0xFF,0xFF,};
u8 const hang4[] = {0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xF8,0xF1,0xE7,0xCF,0x9F,0x3F,0xFF,0xFF,0xFF,0xFF,};
u8 const hang3[] = {0xFF,0xFF,0xEF,0xFD,0xAB,0xC6,0x83,0x81,0x98,0x99,0x82,0xC5,0xFB,0xFF,0xFF,0xFF,};   //原始/
u8 const hang2[] = {0xFF,0xFF,0xFF,0xEF,0xEF,0xEF,0xF7,0xF7,0xF7,0xF7,0xEF,0xEF,0xFF,0xFF,0xFF,0xFF,};
u8 const hang1[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,};

u8 a=0;
uint8_t g_u8ExpressID = 1;

void biaoqing1(void);
void biaoqing2(void);
void biaoqing3(void);
void biaoqing4(void);
void biaoqing5(void);
void biaoqing6(void);
void biaoqing7(void);
void biaoqing8(void);
void biaoqing9(void);
void biaoqing10(void);


void EYE_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);						 //PA.8 输出高
}

void WR_595(unsigned char segdata) 
{
	unsigned char i; 
	for(i=0;i<8;i++)           
	{        
		if((segdata&0x80)==0x80) 
		{
			DS(1);
		}
		else
		{
			DS(0);

		}
	 
		CLK(1);     
		segdata=segdata<<1;
		CLK(0);
	}
}

void OUT_595(void)      //?????????????
{                
	RCK(0);
	//  Delay(10);
	RCK(1);
}  

void WR_5951(unsigned char segdata) //???????????595
{
	unsigned char i; 
	for(i=0;i<8;i++)           //????8???,
	{        
		if((segdata&0x80)==0x80)          // ???????
		{
			DS1(1);
		}
		else
		{
			DS1(0);

		}
	 
		CLK1(1);     //??????????595
		segdata=segdata<<1;
		//  Delay(10);
		CLK1(0);
	}
}

void OUT_5951(void)      //?????????????
{ 
                  
	RCK1(0);
	//  Delay(10);
	RCK1(1);
}

void ExpressShow(uint8_t _expressID)
{
	switch(_expressID)
	{
		case EXPRESS1:
		{
			biaoqing1();
			break;
		}
		case EXPRESS2:
		{
			biaoqing2();
			break;
		}
		case EXPRESS3:
		{
			biaoqing3();
			break;
		}
		case EXPRESS4:
		{
			biaoqing4();
			break;
		}
		case EXPRESS5:
		{
			biaoqing5();
			break;
		}
		case EXPRESS6:
		{
			biaoqing6();
			break;
		}
		case EXPRESS7:
		{
			biaoqing7();
			break;
		}
		case EXPRESS8:
		{
			biaoqing8();
			break;
		}
		case EXPRESS9:
		{
			biaoqing9();
			break;
		}
		case EXPRESS10:
		{
			biaoqing10();
			break;
		}
		default:
			break;
	}
}

void biaoqing1()
{

    u8 i;
    if(a==1)
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang3[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang3[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang3[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang3[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }

        //delay_ms(500);
    }
    else
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang2[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang2[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang2[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang2[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }
    }

}
void biaoqing2()
{

    u8 i;
    if(a==1)
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang4[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang4[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang4[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang4[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }

        //delay_ms(500);
    }
    else
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang1[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang1[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang1[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang1[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }
    }

}
void biaoqing3()
{

    u8 i;
    if(a==1)
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang5[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang5[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang5[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang5[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }

        //delay_ms(500);
    }
    else
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang1[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang1[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang1[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang1[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }
    }

}
void biaoqing4()
{

    u8 i;
    if(a==1)
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang6[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang6[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang6[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang6[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }

        //delay_ms(500);
    }
    else
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang2[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang2[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang2[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang2[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }
    }

}
void biaoqing5()
{

    u8 i;
    if(a==1)
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang7[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang7[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang7[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang7[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }

        //delay_ms(500);
    }
    else
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang2[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang2[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang2[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang2[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }
    }

}
void biaoqing6()
{

    u8 i;
    if(a==1)
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang8[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang8[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang8[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang8[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }

        //delay_ms(500);
    }
    else
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang1[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang1[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang1[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang1[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }
    }

}
void biaoqing7()
{

    u8 i;
    if(a==1)
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang9[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang9[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang9[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang9[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }

        //delay_ms(500);
    }
    else
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang1[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang1[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang1[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang1[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }
    }

}
void biaoqing8()
{
    u8 i;
    for(i=0; i<8; i++)
    {
        WR_595(hang2[i]);
        WR_595(0x00);
        WR_595(lie1[i]);
        OUT_595();
    }
    for(i=0; i<8; i++)
    {
        WR_595(hang2[i+8]);
        WR_595(lie1[i]);
        WR_595(0x00);
        OUT_595();
    }

    for(i=0; i<8; i++)
    {
        WR_5951(hang2[i]);
        WR_5951(lie2[i]);
        WR_5951(0x00);
        OUT_5951();
    }
    for(i=0; i<8; i++)
    {
        WR_5951(hang2[i+8]);
        WR_5951(0x00);
        WR_5951(lie2[i]);
        OUT_5951();
    }





}
void biaoqing9()
{

    u8 i;
    if(a==1)
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang3[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang3[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang3[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang3[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }

        //delay_ms(500);
    }
    else
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang2[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang2[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang2[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang2[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }
    }

}
void biaoqing10()
{

    u8 i;
    if(a==1)
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang3[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang3[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang3[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang3[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }

        //delay_ms(500);
    }
    else
    {
        for(i=0; i<8; i++)
        {
            WR_595(hang2[i]);
            WR_595(0x00);
            WR_595(lie1[i]);
            OUT_595();
        }
        for(i=0; i<8; i++)
        {
            WR_595(hang2[i+8]);
            WR_595(lie1[i]);
            WR_595(0x00);
            OUT_595();
        }

        for(i=0; i<8; i++)
        {
            WR_5951(hang2[i]);
            WR_5951(lie2[i]);
            WR_5951(0x00);
            OUT_5951();
        }
        for(i=0; i<8; i++)
        {
            WR_5951(hang2[i+8]);
            WR_5951(0x00);
            WR_5951(lie2[i]);
            OUT_5951();
        }
    }

}

