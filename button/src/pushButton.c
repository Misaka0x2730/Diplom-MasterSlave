#include "pushButton.h"

void PushButton_Init()
{
	GPIO_xInit(PIN_BUTTON_ENTER, GPIO_Speed_2MHz, GPIO_Mode_IPU);
	GPIO_xInit(PIN_BUTTON_LEFT, GPIO_Speed_2MHz, GPIO_Mode_IPU);
	GPIO_xInit(PIN_BUTTON_RIGHT, GPIO_Speed_2MHz, GPIO_Mode_IPU);
	GPIO_xInit(PIN_BUTTON_UP, GPIO_Speed_2MHz, GPIO_Mode_IPU);
	GPIO_xInit(PIN_BUTTON_DOWN, GPIO_Speed_2MHz, GPIO_Mode_IPU);

	TIM_TimeBaseInitTypeDef TIM_InitStructure;

	TIM_DeInit(TIM3);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseStructInit(&TIM_InitStructure);
	TIM_InitStructure.TIM_Prescaler = ((u16_t)(F_APB1/1000000))-1;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 1000 - 1;	//36MHz/36 = 1MHz/1000 = 1kHz
	TIM_TimeBaseInit(TIM3, &TIM_InitStructure);

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM3_IRQn);
	TIM_Cmd(TIM3, ENABLE);
}

void PushButton_IRQHandler()
{
	static u8_t enter = 0, left = 0, right = 0,
				up = 0, down = 0;
	static u8_t enterState = 1, leftState = 1, rightState = 1,
				upState = 1, downState = 1;

	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		if(enterState == 1)
		{
			if(GPIO_xReadBit(PIN_BUTTON_ENTER) == Bit_RESET)
			{
				enter++;
				if(enter > BUTTON_DEBOUNCE)
				{
					enter = 0;
					enterState = 0;
					GUI_StoreKeyMsg(GUI_KEY_ENTER, 1);
				}
			}
			else
			{
				enter = 0;
			}
		}
		else
		{
			if(GPIO_xReadBit(PIN_BUTTON_ENTER) == Bit_SET)
			{
				enter++;
				if(enter > BUTTON_DEBOUNCE)
				{
					enter = 0;
					enterState = 1;
					GUI_StoreKeyMsg(GUI_KEY_ENTER, 0);
				}
			}
			else
			{
				enter = 0;
			}
		}

		if(leftState == 1)
		{
			if(GPIO_xReadBit(PIN_BUTTON_LEFT) == Bit_RESET)
			{
				left++;
				if(left > BUTTON_DEBOUNCE)
				{
					left = 0;
					leftState = 0;
					GUI_StoreKeyMsg(GUI_KEY_LEFT, 1);
				}
			}
			else
			{
				left = 0;
			}
		}
		else
		{
			if(GPIO_xReadBit(PIN_BUTTON_LEFT) == Bit_SET)
			{
				left++;
				if(left > BUTTON_DEBOUNCE)
				{
					left = 0;
					leftState = 1;
					GUI_StoreKeyMsg(GUI_KEY_LEFT, 0);
				}
			}
			else
			{
				left = 0;
			}
		}

		if(rightState == 1)
		{
			if(GPIO_xReadBit(PIN_BUTTON_RIGHT) == Bit_RESET)
			{
				right++;
				if(right > BUTTON_DEBOUNCE)
				{
					right = 0;
					rightState = 0;
					GUI_StoreKeyMsg(GUI_KEY_RIGHT, 1);
				}
			}
			else
			{
				right = 0;
			}
		}
		else
		{
			if(GPIO_xReadBit(PIN_BUTTON_RIGHT) == Bit_SET)
			{
				right++;
				if(right > BUTTON_DEBOUNCE)
				{
					right = 0;
					rightState = 1;
					GUI_StoreKeyMsg(GUI_KEY_RIGHT, 0);
				}
			}
			else
			{
				right = 0;
			}
		}

		if(upState == 1)
		{
			if(GPIO_xReadBit(PIN_BUTTON_UP) == Bit_RESET)
			{
				up++;
				if(up > BUTTON_DEBOUNCE)
				{
					up = 0;
					upState = 0;
					GUI_StoreKeyMsg(GUI_KEY_UP, 1);
				}
			}
			else
			{
				up = 0;
			}
		}
		else
		{
			if(GPIO_xReadBit(PIN_BUTTON_UP) == Bit_SET)
			{
				up++;
				if(up > BUTTON_DEBOUNCE)
				{
					up = 0;
					upState = 1;
					GUI_StoreKeyMsg(GUI_KEY_UP, 0);
				}
			}
			else
			{
				up = 0;
			}
		}

		if(downState == 1)
		{
			if(GPIO_xReadBit(PIN_BUTTON_DOWN) == Bit_RESET)
			{
				down++;
				if(down > BUTTON_DEBOUNCE)
				{
					down = 0;
					downState = 0;
					GUI_StoreKeyMsg(GUI_KEY_DOWN, 1);
				}
			}
			else
			{
				down = 0;
			}
		}
		else
		{
			if(GPIO_xReadBit(PIN_BUTTON_DOWN) == Bit_SET)
			{
				down++;
				if(down > BUTTON_DEBOUNCE)
				{
					down = 0;
					downState = 1;
					GUI_StoreKeyMsg(GUI_KEY_DOWN, 0);
				}
			}
			else
			{
				down = 0;
			}
		}
	}
}
