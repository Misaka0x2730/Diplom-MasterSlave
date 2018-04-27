#include "hwfunctions.h"
#include "sensors.h"
#include "sht21.h"
#include "GUI.h"
#include "MainTask.h"
#include "ModbusSlave.h"

extern GUI_TIMER_TIME OS_TimeMS;

void main()
{
	RCC_ClocksTypeDef RCC_Clock;
	RCC_GetClocksFreq(&RCC_Clock);
	SysTick_Config(SystemCoreClock / 1000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	HW_Init();
	GUI_Init();
	MainTask();
	__enable_irq();

	u32_t time = 0;
	while(1)
	{
		Sensor_Exec();
		GUI_Exec();
	}
}

void HardFault_Handler(void)
{
  /* Go into an infinite loop. */
  while (1)
  {
  }
}

void SysTick_Handler(void)
{
  OS_TimeMS ++;
}
