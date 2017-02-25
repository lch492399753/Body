#ifndef __ADC_H
#define __ADC_H	 
#include "sys.h"

#define N 50 
#define M 4 
extern __IO uint16_t AD_Value[N][M]; 
extern uint16_t After_filter[M]; 

void filter(void);
void Init_All_Periph(void);
u16 GetVolt(u16 advalue);
	

#endif
