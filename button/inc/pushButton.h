#ifndef BUTTON_INC_PUSHBUTTON_H_
#define BUTTON_INC_PUSHBUTTON_H_

#include "types.h"
#include "pinMap.h"
#include "stm32f107.h"
#include "gpio.h"
#include "GUI.h"

#define BUTTON_DEBOUNCE	50
#define PushButton_IRQHandler TIM3_IRQHandler

void PushButton_Init();

#endif /* BUTTON_INC_PUSHBUTTON_H_ */
