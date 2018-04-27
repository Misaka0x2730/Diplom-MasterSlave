#include "sensors.h"

u8_t timerFlag = 1;

void Sensors_Init()
{
	SHT21_Init(SENSORS_I2C_MODULE);
	BMP085_Init(SENSORS_I2C_MODULE);

	TIM_TimeBaseInitTypeDef TIM_InitStructure;

	TIM_DeInit(TIM2);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructInit(&TIM_InitStructure);
	TIM_InitStructure.TIM_Prescaler = ((u16_t)(F_APB1/1000))-1;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 2000 - 1;
	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);

	//TIM_Cmd(TIM2, ENABLE);
}

void Sensors_StartMeasure()
{
	SHT21_StartMeasureTemperature(SENSORS_I2C_MODULE);
	BMP085_StartMeasureTemperature(SENSORS_I2C_MODULE);

	timerFlag = 0;
	TIM_SetAutoreload(TIM2, 100-1);
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);
}

void Sensors_ReadIntermediateResult()
{
	BMP085_ReadUT(SENSORS_I2C_MODULE);
	BMP085_StartMeasurePressure(SENSORS_I2C_MODULE);

	SHT21_CalculateTemp(SENSORS_I2C_MODULE);
	INPUTS_SetTemperatureValue(SHT21_GetTemp());
	SHT21_StartMeasureRH(SENSORS_I2C_MODULE);

	timerFlag = 0;
	TIM_SetAutoreload(TIM2, 40-1);
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);
}

void Sensors_ReadFinalResult()
{
	BMP085_ReadUP(SENSORS_I2C_MODULE);
	BMP085_CalculatePressure();
	INPUTS_SetPressureValue(BMP085_GetPressure());

	SHT21_CalculateRH(SENSORS_I2C_MODULE);
	INPUTS_SetHumidityValue(SHT21_GetRH());

	timerFlag = 0;
	TIM_SetAutoreload(TIM2, 2000-1);
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);
}

void Sensor_Exec()
{
	static u8_t status = 0;
	if(Device_GetMode() == MODE_SLAVE)
	{
		switch(status)
		{
			case 0:
			{
				if(timerFlag)
				{
					Sensors_StartMeasure();
					++status;
				}
				break;
			}
			case 1:
			{
				if(timerFlag)
				{
					Sensors_ReadIntermediateResult();
					++status;
				}
				break;
			}
			case 2:
			{
				if(timerFlag)
				{
					Sensors_ReadFinalResult();
					status = 0;
				}
				break;
			}
		}
	}
}

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIM_Cmd(TIM2, DISABLE);
		timerFlag = 1;
	}
}
