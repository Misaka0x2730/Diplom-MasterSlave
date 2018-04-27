#include "inputsOutputs.h"

static const GPIO_PIN_TYPE outputPinList[OUTPUT_AMOUNT] = {
							PIN_LED_1,PIN_LED_2,PIN_LED_3,PIN_LED_4,PIN_LED_5,
							PIN_LED_6,PIN_LED_7,PIN_LED_8,PIN_LED_9,PIN_LED_10,
							PIN_LED_11,PIN_LED_12,PIN_LED_13,PIN_LED_14,PIN_LED_15,
							PIN_OC_1,PIN_OC_2,PIN_OC_3,PIN_OC_4,PIN_OC_5,PIN_OC_6,
							PIN_OC_7,PIN_BUZZER
};

static OUTPUT_STATE_TYPE outputPinState[OUTPUT_AMOUNT] = {OUTPUT_STATE_OFF};

static const GPIO_PIN_TYPE inputAnalogPinList[INPUT_ANALOG_AMOUNT] = {
							PIN_ANALOG_1,PIN_ANALOG_2,PIN_ANALOG_3,
							PIN_ANALOG_4,PIN_ANALOG_5
};

static INPUT_STATE_TYPE	inputState[INPUT_AMOUNT] = {0};

void INPUTS_Init()
{
	for(u8_t i = 0; i < INPUT_ANALOG_AMOUNT; i++)
		GPIO_xInit(inputAnalogPinList[i], GPIO_Speed_2MHz, GPIO_Mode_AIN);

	/*IR_SAMPLE_TIMER_RCC_ENABLE;

	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_DeInit(IR_SAMPLE_TIMER);
	TIM_TimeBaseStructInit(&TIM_InitStructure);
	TIM_InitStructure.TIM_Prescaler = IR_SAMPLE_TIMER_PRESCALER;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV2;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = IR_SAMPLE_TIMER_PERIOD;
	TIM_TimeBaseInit(IR_SAMPLE_TIMER, &TIM_InitStructure);

	TIM_ARRPreloadConfig(IR_SAMPLE_TIMER, ENABLE);
	TIM_SelectOutputTrigger(IR_SAMPLE_TIMER, TIM_TRGOSource_Update);

	IR_ADC_RCC_ENABLE;

	ADC_InitTypeDef ADC_InitStructure;
	ADC_DeInit(INPUT_ANALOG_ADC);
	ADC_InitStructure.ADC_Mode = ADC_Mode_InjecSimult;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel = INPUT_ANALOG_AMOUNT;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Init(INPUT_ANALOG_ADC, &ADC_InitStructure);

	ADC_InjectedSequencerLengthConfig(INPUT_ANALOG_ADC, INPUT_ANALOG_AMOUNT);

	ADC_InjectedChannelConfig(INPUT_ANALOG_ADC, IR_IN1_CHANNEL, 1, ADC_SampleTime_239Cycles5);
	ADC_InjectedChannelConfig(INPUT_ANALOG_ADC, IR_IN2_CHANNEL, 2, ADC_SampleTime_239Cycles5);
	ADC_InjectedChannelConfig(INPUT_ANALOG_ADC, IR_REF_CHANNEL, 3, ADC_SampleTime_239Cycles5);
	ADC_InjectedDiscModeCmd(INPUT_ANALOG_ADC, ENABLE);

	ADC_ExternalTrigInjectedConvConfig(INPUT_ANALOG_ADC, ADC_ExternalTrigInjecConv_T1_TRGO);
	ADC_ExternalTrigInjectedConvCmd(INPUT_ANALOG_ADC, ENABLE);

	ADC_Cmd(INPUT_ANALOG_ADC, ENABLE);

	NVIC_InitTypeDef  NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	ADC_ResetCalibration(INPUT_ANALOG_ADC);
	while(ADC_GetResetCalibrationStatus(INPUT_ANALOG_ADC) != RESET);

	ADC_StartCalibration(INPUT_ANALOG_ADC);
	while(ADC_GetCalibrationStatus(INPUT_ANALOG_ADC) != RESET);

	IR_StartConv();*/
}

INPUT_STATE_TYPE INPUTS_SetAnalogInputValue(u8_t address, u16_t value)
{
	if(!INPUTS_IS_ANALOG_INPUT(address))
		return INPUT_ERROR_ADDRESS;
	inputState[address] = value;
	return INPUT_ERROR_OK;
}

void INPUTS_SetTemperatureValue(u16_t value)
{
	inputState[INPUT_TEMPERATURE] = (u16_t)(value);
}

void INPUTS_SetHumidityValue(u16_t value)
{
	inputState[INPUT_HUMIDITY] = value;
}

void INPUTS_SetPressureValue(u32_t value)
{
	inputState[INPUT_PRESSURE_0] = (value>>16) & 0xFFFF;
	inputState[INPUT_PRESSURE_1] = value & 0xFFFF;
}

s16_t INPUTS_GetTemperatureValue()
{
	return (s16_t)(inputState[INPUT_TEMPERATURE]);
}

u16_t INPUTS_GetHumidityValue()
{
	return inputState[INPUT_HUMIDITY];
}

u32_t INPUTS_GetPressureValue()
{
	return ((inputState[INPUT_PRESSURE_0]<<16)+inputState[INPUT_PRESSURE_1]);
}

INPUT_STATE_TYPE INPUTS_GetState(u16_t address)
{
	if(!INPUTS_IS_INPUT(address))
		return INPUT_ERROR_ADDRESS;
	return inputState[address];
}

INPUT_STATE_TYPE INPUTS_SetState(u16_t address, u16_t value)
{
	if(!INPUTS_IS_INPUT(address))
		return INPUT_ERROR_ADDRESS;
	inputState[address] = value;
	return INPUT_ERROR_OK;
}

INPUT_STATE_TYPE INPUTS_GetMultipleState(u16_t address, u16_t numberOfInputs, u8_t *data)
{
	for(u16_t i = 0; i < numberOfInputs; i++)
	{
		if(!INPUTS_IS_INPUT(address+i))
			return INPUT_ERROR_ADDRESS;
	}
	for(u16_t i = 0; i < numberOfInputs; i++)
	{
		INPUT_STATE_TYPE value = INPUTS_GetState(address+i);
		data[(i*2)] = (value & 0xFF);
		data[(i*2)+1] = ((value>>8) & 0xFF);
	}
	return INPUT_ERROR_OK;
}

INPUT_STATE_TYPE INPUTS_SetMultipleState(u16_t address, u16_t numberOfInputs, u8_t *data)
{
	for(u16_t i = 0; i < numberOfInputs; i++)
	{
		if(!INPUTS_IS_INPUT(address+i))
			return INPUT_ERROR_ADDRESS;
	}
	for(u16_t i = 0; i < numberOfInputs; i++)
	{
		INPUT_STATE_TYPE value = (data[(i*2)]<<8) + data[(i*2)+1];
		INPUTS_SetState(address+i, value);
	}
	return INPUT_ERROR_OK;
}

void OUTPUTS_Init()
{
	for(u8_t i = 0; i < OUTPUT_AMOUNT; i++)
	{
		GPIO_xInit(outputPinList[i], GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
		OUTPUTS_SetState(i, OUTPUT_STATE_OFF);
	}
}

OUTPUT_STATE_TYPE OUTPUTS_GetState(u16_t address)
{
	if(!OUTPUTS_IS_OUTPUT(address))
		return OUTPUT_STATE_ERROR;
	return outputPinState[address];
}

OUTPUT_RETURN_TYPE OUTPUTS_GetMultipleState(u16_t address, u16_t numberOfOutputs, u8_t *data, u8_t *numberOfBytes)
{
	for(u16_t i = 0; i < numberOfOutputs; i++)
	{
		if(!OUTPUTS_IS_OUTPUT(address+i))
			return OUTPUT_ERROR_ADDRESS;
	}

	for(u16_t i = 0; i < numberOfOutputs; i++)
	{
		if(OUTPUTS_GetState(address+i) == OUTPUT_STATE_ON)
			data[i/8] |= (1<<(i%8));
		else
			data[i/8] &= ~(1<<(i%8));
	}

	if((numberOfOutputs % 8) != 0)
		(*numberOfBytes) = ((numberOfOutputs / 8) + 1);
	else
		(*numberOfBytes) = (numberOfOutputs / 8);

	return OUTPUT_ERROR_OK;
}


OUTPUT_RETURN_TYPE OUTPUTS_SetState(u16_t address, OUTPUT_STATE_TYPE state)
{
	if(!OUTPUTS_IS_OUTPUT(address))
		return OUTPUT_ERROR_ADDRESS;
	if(!OUTPUTS_IS_STATE(state))
		return OUTPUT_ERROR_STATE;

	outputPinState[address] = state;
	if(state == OUTPUT_STATE_ON)
		GPIO_xSetBit(outputPinList[address]);
	else
		GPIO_xResetBit(outputPinList[address]);
	return OUTPUT_ERROR_OK;
}

OUTPUT_RETURN_TYPE OUTPUTS_SetMultipleState(u16_t address, u16_t numberOfOutputs, u8_t *data)
{
	for(u16_t i = 0; i < numberOfOutputs; i++)
	{
		if(!OUTPUTS_IS_OUTPUT(address+i))
			return OUTPUT_ERROR_ADDRESS;
	}

	for(u16_t i = 0; i < numberOfOutputs; i++)
	{
		if((data[i/8]>>i)&0x01 == 1)
			OUTPUTS_SetState(address+i, OUTPUT_STATE_ON);
		else
			OUTPUTS_SetState(address+i, OUTPUT_STATE_ON);
	}

	return OUTPUT_ERROR_OK;
}
