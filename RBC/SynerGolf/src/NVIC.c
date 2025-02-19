#include "NVIC.h"

void initializeNVIC()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitTypeDef nvic_usart;
	nvic_usart.NVIC_IRQChannel = USART3_IRQn;
	nvic_usart.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_usart.NVIC_IRQChannelSubPriority = 1;
	nvic_usart.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_usart);

	NVIC_InitTypeDef nvic_adc;
	nvic_adc.NVIC_IRQChannel = ADC1_2_IRQn;
	nvic_adc.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_adc.NVIC_IRQChannelSubPriority = 2;
	nvic_adc.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_adc);

	NVIC_InitTypeDef nvic_tim1;
	nvic_tim1.NVIC_IRQChannel =  TIM1_CC_IRQn; // wyb�r linii przerwania
	nvic_tim1.NVIC_IRQChannelCmd = ENABLE; // w��czenie linii
	nvic_tim1.NVIC_IRQChannelPreemptionPriority = 0; // priorytet
	nvic_tim1.NVIC_IRQChannelSubPriority = 3; // podgrupa
	NVIC_Init(&nvic_tim1); // inicjalizacja linii przerwania

	NVIC_InitTypeDef nvic_tim2;
	nvic_tim2.NVIC_IRQChannel =  TIM2_IRQn; // wyb�r linii przerwania
	nvic_tim2.NVIC_IRQChannelCmd = ENABLE; // w��czenie linii
	nvic_tim2.NVIC_IRQChannelPreemptionPriority = 0; // priorytet
	nvic_tim2.NVIC_IRQChannelSubPriority = 4; // podgrupa
	NVIC_Init(&nvic_tim2); // inicjalizacja linii przerwania
}
