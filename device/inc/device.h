#ifndef DEVICE_INC_DEVICE_H_
#define DEVICE_INC_DEVICE_H_

#include "types.h"
#include "eeprom.h"
#include "ModbusCommon.h"
#include "inputsOutputs.h"
#include "ModbusSlave_Init.h"
#include "ModbusMaster_Init.h"
#include <string.h>

#define CONFIG_ADDRESS (0)

typedef enum
{
	MODE_SLAVE = 0,
	MODE_MASTER = 1
}DEVICE_MODES;

typedef enum
{
	STATE_STOP = 0,
	STATE_RUN = 1
}DEVICE_STATES;

typedef enum
{
	MENU_NONE = 0,
	MAIN_MENU = 1,
	SETTINGS_MENU = 2
}KEY_STATES;

typedef enum
{
	MENU_NEXT = 0,
	MENU_PREV = 1,
	MENU_STAY = 2
}MENU_SWITCH;

typedef struct __attribute__((packed, aligned(1)))
{
	u8_t deviceMode;
	u8_t deviceState;
	u8_t address;
	u8_t baud;
	u16_t waitingTime;
	u16_t interviewPeriod;
	u32_t recv;
	u32_t transm;
}DEVICE_SettingsStruct;

void Device_ReadConfig();
u8_t Device_SaveConfig();
void Device_ClearConfig();

void Device_NullStats();
u32_t Device_GetReceived();
u32_t Device_GetTransmitted();
void Device_IncRecv();
void Device_IncTransm();

u8_t Device_GetAddress();
void Device_SetAddress(u8_t address);
u8_t Device_GetBaud();
void Device_SetBaud(u8_t baud);
u16_t Device_GetWaitingTime();
void Device_SetWaitingTime(u16_t waitingTime);
u16_t Device_GetInterviewPeriod();
void Device_SetInterviewPeriod(u16_t interviewPeriod);

s32_t Device_GetTemp();
u32_t Device_GetPressure();
u16_t Device_GetRH();

u16_t Device_GetMode();
void Device_SetMode(u8_t mode);

u16_t Device_GetState();
void Device_SetState(u8_t state);
void Device_SwitchState();

#endif /* DEVICE_INC_DEVICE_H_ */
