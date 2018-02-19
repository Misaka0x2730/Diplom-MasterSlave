#include <stddef.h>
#include "modbus.h"

ModbusFunction ModbusFunction_List[4];
ModbusRTU_InitTypeDef ModbusRTU_InitStructure;

void Modbus_Main(ModbusADU* adu)
{
	u8_t errorCode = Modbus_Parsing(adu);
	if(errorCode != ModbusReturnErrorCode_Error_OK)
	{

	}
	else
	{

	}
}

ModbusReturnErrorCode Modbus_Parsing(ModbusADU* adu)
{
	u8_t errorCode = 0;
	if(!MODBUS_IS_PROTOCOL(adu->protocol))
		return ModbusReturnErrorCode_Illegal_Protocol;
	else
	{
		ModbusMBAP* mbap;
		ModbusPDU*  pdu;
		errorCode = Modbus_ParsingADU(adu, mbap, pdu);
		if(errorCode == ModbusReturnErrorCode_Error_OK)
		{

		}
		else
			return errorCode;
	}
}

ModbusReturnErrorCode Modbus_ParsingADU(ModbusADU* adu, ModbusMBAP* mbap, ModbusPDU* pdu)
{
	if(MODBUS_CHECK_SIZE(adu->protocol, adu->length))
	{
		u16_t currentPosition = 0;
		u16_t i = 0;
		if(adu->protocol == ModbusProtocol_TCP)
		{
			mbap->transactionID = (adu->data[0]<<8) + adu->data[1];
			mbap->protocolID = (adu->data[2]<<8) + adu->data[3];
			mbap->length = (adu->data[4]<<8) + adu->data[5];
			mbap->slaveAddress = adu->data[6];
			currentPosition = ModbusDataSize_TCP_Function_Position;
			if(mbap->protocolID != 0x0000)
				return ModbusReturnErrorCode_Illegal_Protocol;
		}
		else
		{
			pdu->slaveAddress = adu->data[0];
			currentPosition = ModbusDataSize_RTU_Function_Position;
		}

		pdu->functionCode = adu->data[currentPosition];

		for(i = currentPosition + 1; i < adu->length; i++)
			pdu->data[i] = adu->data[i];

		return ModbusReturnErrorCode_Error_OK;
	}
	else
		return ModbusReturnErrorCode_Error_Length;
}
