#ifndef INPUTSOUTPUTS_INC_INPUTSOUTPUTS_H_
#define INPUTSOUTPUTS_INC_INPUTSOUTPUTS_H_

#include "types.h"
#include "gpio.h"
#include "pinMap.h"

typedef u8_t 	OUTPUT_RETURN_TYPE;
typedef u16_t 	OUTPUT_STATE_TYPE;
typedef u32_t 	INPUT_STATE_TYPE;

#define INPUT_ANALOG_1				0
#define INPUT_ANALOG_2				1
#define INPUT_ANALOG_3				2
#define INPUT_ANALOG_4				3
#define INPUT_ANALOG_5				4
#define INPUT_TEMPERATURE			5
#define INPUT_HUMIDITY				6
#define INPUT_PRESSURE_0			7
#define INPUT_PRESSURE_1			8

#define INPUT_AMOUNT				9
#define INPUT_ANALOG_AMOUNT			5

#define INPUT_ERROR_OK				(0x10000)
#define INPUT_ERROR_ADDRESS			(0x10001)

#define INPUT_ANALOG_ADC			ADC1
#define INPUT_ANALOG_CHANNEL_1		ADC_Channel_3
#define INPUT_ANALOG_CHANNEL_2		ADC_Channel_4
#define INPUT_ANALOG_CHANNEL_3		ADC_Channel_5
#define INPUT_ANALOG_CHANNEL_4		ADC_Channel_6
#define INPUT_ANALOG_CHANNEL_5		ADC_Channel_7

#define INPUTS_IS_INPUT(address)	((address == INPUT_ANALOG_1)||(address == INPUT_ANALOG_2)||(address == INPUT_ANALOG_3)||\
		 	 	 	 	 	 	 	 (address == INPUT_ANALOG_4)||(address == INPUT_ANALOG_5)||(address == INPUT_TEMPERATURE)||\
									 (address == INPUT_HUMIDITY)||(address == INPUT_PRESSURE_0)||(address == INPUT_PRESSURE_1))

#define INPUTS_IS_ANALOG_INPUT(address)	((address == INPUT_ANALOG_1)||(address == INPUT_ANALOG_2)||(address == INPUT_ANALOG_3)||\
		 	 	 	 	 	 	 	 	 (address == INPUT_ANALOG_4)||(address == INPUT_ANALOG_5))

#define OUTPUT_LED_1				0
#define OUTPUT_LED_2				1
#define OUTPUT_LED_3				2
#define OUTPUT_LED_4				3
#define OUTPUT_LED_5				4
#define OUTPUT_LED_6				5
#define OUTPUT_LED_7				6
#define OUTPUT_LED_8				7
#define OUTPUT_LED_9				8
#define OUTPUT_LED_10				9
#define OUTPUT_LED_11				10
#define OUTPUT_LED_12				11
#define OUTPUT_LED_13				12
#define OUTPUT_LED_14				13
#define OUTPUT_LED_15				14
#define OUTPUT_OC_1					15
#define OUTPUT_OC_2					16
#define OUTPUT_OC_3					17
#define OUTPUT_OC_4					18
#define OUTPUT_OC_5					19
#define OUTPUT_OC_6					20
#define OUTPUT_OC_7					21
#define OUTPUT_BUZZER				22
#define OUTPUT_AMOUNT				23

#define OUTPUT_STATE_OFF			0x0000
#define OUTPUT_STATE_ON				0xFF00
#define OUTPUT_STATE_ERROR			3

#define OUTPUT_ERROR_OK				0
#define OUTPUT_ERROR_ADDRESS		1
#define OUTPUT_ERROR_STATE			2

#define OUTPUTS_IS_STATE(state)		((state == OUTPUT_STATE_OFF)||(state == OUTPUT_STATE_ON))
#define OUTPUTS_IS_OUTPUT(address)	((address == OUTPUT_LED_1)||(address == OUTPUT_LED_2)||(address == OUTPUT_LED_3)||\
									 (address == OUTPUT_LED_4)||(address == OUTPUT_LED_5)||(address == OUTPUT_LED_6)||\
									 (address == OUTPUT_LED_7)||(address == OUTPUT_LED_8)||(address == OUTPUT_LED_9)||\
									 (address == OUTPUT_LED_10)||(address == OUTPUT_LED_11)||(address == OUTPUT_LED_12)||\
									 (address == OUTPUT_LED_13)||(address == OUTPUT_LED_14)||(address == OUTPUT_LED_15)||\
									 (address == OUTPUT_OC_1)||(address == OUTPUT_OC_2)||(address == OUTPUT_OC_3)||\
									 (address == OUTPUT_OC_4)||(address == OUTPUT_OC_5)||(address == OUTPUT_OC_6)||\
									 (address == OUTPUT_OC_7)||(address == OUTPUT_BUZZER))

#define OUTPUT_IS_CORRECT_NUMBER_OF_BYTES(numberOfOutputs, numberOfBytes)	(((numberOfOutputs % 8 != 0) && (numberOfBytes == (numberOfOutputs/8)+1))||\
																	 	 	 ((numberOfOutputs % 8 == 0) && (numberOfBytes == (numberOfOutputs/8))))

void INPUTS_Init();
void INPUTS_SetTemperatureValue(u16_t value);
void INPUTS_SetHumidityValue(u16_t value);
void INPUTS_SetPressureValue(u32_t value);
s16_t INPUTS_GetTemperatureValue();
u16_t INPUTS_GetHumidityValue();
u32_t INPUTS_GetPressureValue();
INPUT_STATE_TYPE INPUTS_GetState(u16_t address);
INPUT_STATE_TYPE INPUTS_GetMultipleState(u16_t address, u16_t numberOfInputs, u8_t *data);
INPUT_STATE_TYPE INPUTS_SetMultipleState(u16_t address, u16_t numberOfInputs, u8_t *data);

void OUTPUTS_Init();
OUTPUT_STATE_TYPE OUTPUTS_GetState(u16_t address);
OUTPUT_RETURN_TYPE OUTPUTS_GetMultipleState(u16_t address, u16_t numberOfOutputs, u8_t *data, u8_t *numberOfBytes);
OUTPUT_RETURN_TYPE OUTPUTS_SetState(u16_t address, OUTPUT_STATE_TYPE state);
OUTPUT_RETURN_TYPE OUTPUTS_SetMultipleState(u16_t address, u16_t numberOfOutputs, u8_t *data);

#endif /* INPUTSOUTPUTS_INC_INPUTSOUTPUTS_H_ */
