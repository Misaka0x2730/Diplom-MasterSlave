#include "device.h"

DEVICE_SettingsStruct settings;

extern void ModbusSlave_Start();
extern void ModbusSlave_Stop();

u8_t Device_SaveConfig()
{
	EEP_WritePage(CONFIG_ADDRESS, (u8_t*)(&settings), sizeof(DEVICE_SettingsStruct));
	return ERROR_OK;
}

void Device_ReadConfig()
{
	EEP_ReadPage(CONFIG_ADDRESS, (u8_t*)(&settings), sizeof(DEVICE_SettingsStruct));
	delay_ms(5);
}

void Device_ClearConfig()
{
	memset((u8_t*)(&settings), 0, sizeof(DEVICE_SettingsStruct));
	EEP_WritePage(CONFIG_ADDRESS, (u8_t*)(&settings), sizeof(DEVICE_SettingsStruct));
}

u16_t Device_GetMode()
{
	return settings.deviceMode;
}

void Device_SetMode(u8_t mode)
{
	settings.deviceMode = mode;
}

u16_t Device_GetState()
{
	return settings.deviceState;
}

void Device_SetState(u8_t state)
{
	settings.deviceState = state;
	if(settings.deviceState == STATE_RUN)
	{
		if(settings.deviceMode == MODE_SLAVE)
		{
			ModbusSlave_Start();
		}
		else
		{
			ModbusMaster_Start();
		}
	}
	else
	{
		if(settings.deviceMode == MODE_SLAVE)
		{
			ModbusSlave_Stop();
		}
		else
		{
			ModbusMaster_Stop();
		}
	}
}

void Device_SwitchState()
{
	if (Device_GetState() == STATE_RUN)
		Device_SetState(STATE_STOP);
	else
		Device_SetState(STATE_RUN);
}

void Device_NullStats()
{
	settings.recv = 0;
	settings.transm = 0;
}

u32_t Device_GetReceived()
{
	return settings.recv;
}

u32_t Device_GetTransmitted()
{
	return settings.transm;
}

void Device_IncRecv()
{
	settings.recv++;
}

void Device_IncTransm()
{
	settings.transm++;
}

u8_t Device_GetAddress()
{
	return settings.address;
}

void Device_SetAddress(u8_t address)
{
	settings.address = address;
}

u8_t Device_GetBaud()
{
	return settings.baud;
}

void Device_SetBaud(u8_t baud)
{
	settings.baud = baud;
}

u16_t Device_GetWaitingTime()
{
	return settings.waitingTime;
}

void Device_SetWaitingTime(u16_t waitingTime)
{
	settings.waitingTime = waitingTime;
}

u16_t Device_GetInterviewPeriod()
{
	return settings.interviewPeriod;
}
void Device_SetInterviewPeriod(u16_t interviewPeriod)
{
	settings.interviewPeriod = interviewPeriod;
}

s32_t Device_GetTemp()
{
	return INPUTS_GetTemperatureValue();
}

u32_t Device_GetPressure()
{
	return INPUTS_GetPressureValue()/100;
}

u16_t Device_GetRH()
{
	return INPUTS_GetHumidityValue()/10;
}
