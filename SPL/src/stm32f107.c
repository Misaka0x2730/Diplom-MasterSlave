#include "stm32f107.h"

static unsigned long *UID = (uint32_t *)0x1FFFF7E8;

void delay_us (unsigned int delay)
{
    TIM2->PSC = F_APB1/1000000-1;
    TIM2->ARR = delay*2;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->CR1 = TIM_CR1_CEN|TIM_CR1_OPM;
    while ((TIM2->CR1 & TIM_CR1_CEN)!=0);
}

void delay_ms (unsigned int delay)
{
    TIM2->PSC = F_APB1/1000-1;
    TIM2->ARR = delay*2;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->CR1 = TIM_CR1_CEN|TIM_CR1_OPM;
    while ((TIM2->CR1 & TIM_CR1_CEN)!=0);
}
