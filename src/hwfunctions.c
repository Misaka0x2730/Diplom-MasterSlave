#include "hwfunctions.h"

void assert_failed(uint8_t* file, uint32_t line)
{
	while(1);
}

void BKP_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);
	PWR_BackupAccessCmd(ENABLE);
}

void IntervalTimer_Init()
{
	//INTERVAL_TIM_RCC_ENABLE;

	TIM_TimeBaseInitTypeDef TIM_InitStructure;

	TIM_TimeBaseStructInit(&TIM_InitStructure);
	TIM_InitStructure.TIM_Prescaler = (F_APB1/1000)-1;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//TIM_InitStructure.TIM_Period = INTERVAL_TIM_PERIOD;
	//TIM_TimeBaseInit(INTERVAL_TIM, &TIM_InitStructure);

	//TIM_Cmd(INTERVAL_TIM, ENABLE);
}

void HW_Init()
{
	RCC_AFIO_ENABLE;
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	INPUTS_Init();
	OUTPUTS_Init();
	IntervalTimer_Init();
	BKP_Init();
	EEP_Init();
	Sensors_Init();
	PushButton_Init();
	Device_ReadConfig();
}

void HW_StartDWT()
{
	SCB_DEMCR |= 0x01000000;
	DWT_CONTROL|= 1; // enable the counter
	DWT_CYCCNT  = 0;
}

u32_t HW_GetDWT()
{
	return DWT_CYCCNT;
}
