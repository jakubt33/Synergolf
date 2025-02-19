#include "ADC.h"

extern volatile Data globalData;

void initializeADC()
{
	// Battery ADC
	GPIO_InitTypeDef adcgpio;
	adcgpio.GPIO_Pin = ADC_BATTERY_PIN;
	adcgpio.GPIO_Speed = GPIO_Speed_2MHz;
	adcgpio.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_GPIO, &adcgpio);

	RCC_ADCCLKConfig(RCC_PCLK2_Div8); // 72 / 8 = 9 MHz

	ADC1->CR2 = ADC_CR2_ADON | ADC_CR2_CONT; // continuous
	ADC1->SQR1 = 0; // one channel
	ADC1->SQR3 = 10; // 10th channel
	ADC1->SMPR1 |= 0b111; // the longest cycle for 10th channel
	ADC1->CR1 |= ADC_CR1_EOCIE; // enable interrupt

	// calibration
	ADC1->CR2 |= ADC_CR2_RSTCAL;
	while(ADC1->CR2 & ADC_CR2_RSTCAL);
	ADC1->CR2 |= ADC_CR2_CAL;
	while(ADC1->CR2 & ADC_CR2_CAL);

	ADC1->CR2 |= ADC_CR2_ADON; // start conversion
}

void ADC1_2_IRQHandler(void){
	if(ADC1->SR & ADC_SR_EOC)
	{
		// it should be done via DMA (with multichannel)
		globalData.battery_level_sum += (ADC1->DR >> 4);
		globalData.sample++;

		if (globalData.sample == 255)
		{
			globalData.sample = 0;
			globalData.battery_level = globalData.battery_level_sum >> 8;
			globalData.battery_level_sum = 0;
		}
	}
}

void initializeBatteryTimer(){
    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 35999; // 1Hz - period=2000
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 1999;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &timerInitStructure);

	TIM_ClearFlag( TIM1, TIM_FLAG_Update ); // czy�ci flag� aktualizacji TIM1
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE); // w��cza przerwanie aktualizacji TIM1
    TIM_Cmd(TIM1, ENABLE);
}

void TIM1_CC_IRQHandler(void){
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		if(globalData.battery_level>ADC_MIN_VALUE){
			LED_ADC_RED_OFF;
			TIM1->ARR = 99+((globalData.battery_level-ADC_MIN_VALUE)<<7); //199 -> f=10hz
		}
		else{
			TIM1->ARR = 99;
			LED_ADC_RED_ON;
		}
        LED_ADC_GREEN_GPIO->ODR ^= LED_ADC_GREEN_PIN;
	}
}
