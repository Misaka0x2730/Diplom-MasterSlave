#ifndef __HWFUNCTIONS_H
#define __HWFUNCTIONS_H

#include "stm32f107.h"
#include "eeprom.h"
#include "inputsOutputs.h"
#include "sensors.h"
#include "device.h"
#include "pushButton.h"

#define RCC_AFIO_ENABLE 				do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);}while(0)

#define    DWT_CYCCNT       			*(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   				*(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR        			*(volatile unsigned long *)0xE000EDFC

void BKP_Init(void);
void ClearConfiguration_Init();
void ETH_UpdateLinkStatus();
void ETH_Timer();
void HW_Init();
void HW_ReInit_Timer();
void HW_StartDWT();
u32_t HW_GetDWT();
#endif //__HWFUNCTIONS_H
