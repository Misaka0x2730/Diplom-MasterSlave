#include "ModbusMaster.h"

extern Modbus_Master_State masterState;
static u8_t readInputs[] = {MODBUS_READ_INPUT_REGISTERS, 0x00, INPUT_TEMPERATURE, 0x00, 0x04};

void ModbusMaster_ParsingPdu(ModbusPdu* pdu, ModbusAdu* adu)
{
	u16_t i = 0, j = 0;
	u16_t originState = 1;
	u16_t endState = 2; // Modbus RTU: last 2 bytes - CRC-16
	u16_t maxLength = MODBUS_RTU_ADU_MAX_SIZE;

	pdu->slaveAddress = adu->bytes[0];

	for(i = 0; ((i < adu->length - originState - endState) && ((i+originState) < maxLength)); i++)
	{
		switch(i)
		{
			case 0:
			{
				pdu->cmd = adu->bytes[i+originState];
				break;
			}
			case 1:
			{
				if((pdu->cmd == MODBUS_READ_COILS) || (pdu->cmd == MODBUS_READ_DISCRETE_INPUTS) || (pdu->cmd == MODBUS_READ_HOLDING_REGISTERS) || (pdu->cmd == MODBUS_READ_INPUT_REGISTERS))
				{
					pdu->numberOfBytes = (adu->bytes[i+originState]);
					if((pdu->cmd == MODBUS_READ_HOLDING_REGISTERS) || (pdu->cmd == MODBUS_READ_INPUT_REGISTERS))
					{
						pdu->numberOfOutputs = pdu->numberOfBytes/2;
					}
				}
				if((pdu->cmd == MODBUS_SET_ONE_COIL)||(pdu->cmd == MODBUS_WRITE_ONE_HOLDING_REGISTER)||(pdu->cmd == MODBUS_WRITE_MULTIPLE_HOLDING_REGISTERS)||(pdu->cmd == MODBUS_SET_MULTIPLE_COILS))
					pdu->address = (adu->bytes[i+originState]<<8);
				break;
			}
			case 2:
			{
				if((pdu->cmd == MODBUS_SET_ONE_COIL)||(pdu->cmd == MODBUS_WRITE_ONE_HOLDING_REGISTER)||(pdu->cmd == MODBUS_WRITE_MULTIPLE_HOLDING_REGISTERS)||(pdu->cmd == MODBUS_SET_MULTIPLE_COILS))
					pdu->address += (adu->bytes[i+originState]);
				else
				{
					pdu->data[0] = adu->bytes[i+originState];
					pdu->length = 1;
				}
				break;
			}
			default:
			{
				for(j = i; ((j < adu->length - originState - endState) && ((j+originState) < maxLength) && (pdu->length < MODBUS_MAX_DATA_SIZE)); j++)
					pdu->data[pdu->length++] = adu->bytes[j+originState];
				return;
			}
		}
	}
}

u8_t ModbusMaster_Parsing(u16_t slaveAddress, ModbusAdu* adu)
{
	u16_t i = 0;
	ModbusPdu pdu;
	u8_t errorCode = MODBUS_ERROR_OK;

	if(adu->length < MODBUS_RTU_MIN_DATA_SIZE)
		return MODBUS_UNDEFINED_ERROR;

	ModbusCommon_InitPdu(&pdu);
	ModbusMaster_ParsingPdu(&pdu, adu);

	if(ModbusCommon_CheckCRC16(adu) == MODBUS_ERROR_CRC)
		return ModbusMaster_Send(NULL);
	else
	{
		if((pdu.slaveAddress != slaveAddress) && (pdu.slaveAddress != MODBUS_RTU_BROADCAST_ADDRESS))
			return ModbusMaster_Send(NULL);
	}

	switch(pdu.cmd)
	{
		/*case MODBUS_READ_COILS:
		{
			if((pdu.numberOfOutputs > 0) && (pdu.numberOfOutputs <= COILS_MAX_AMOUNT_READ))
			{
				if(OUTPUTS_GetMultipleState(pdu.address, pdu.numberOfOutputs, pdu.data, (&pdu.numberOfBytes)) != OUTPUT_ERROR_OK)
					errorCode = MODBUS_ILLEGAL_DATA_ADDRESS;
				else
					errorCode = MODBUS_ERROR_OK;
			}
			else
				errorCode = MODBUS_ILLEGAL_DATA_VALUE;
			break;
		}
		case MODBUS_SET_ONE_COIL:
		{
			if(pdu.length == 2)
			{
				u8_t code = OUTPUTS_SetState(pdu.address, ((pdu.data[0]<<8)+pdu.data[1]));
				switch(code)
				{
					case OUTPUT_ERROR_OK:
					{
						errorCode = MODBUS_ERROR_OK;
						break;
					}
					case OUTPUT_ERROR_ADDRESS:
					{
						errorCode = MODBUS_ILLEGAL_DATA_ADDRESS;
						break;
					}
					case OUTPUT_ERROR_STATE:
					{
						errorCode = MODBUS_ILLEGAL_DATA_VALUE;
						break;
					}
				}
			}
			else
				errorCode = MODBUS_ILLEGAL_DATA_VALUE;
			break;
		}
		case MODBUS_SET_MULTIPLE_COILS:
		{
			if((pdu.numberOfOutputs > 0)&&(pdu.numberOfOutputs <= COILS_MAX_AMOUNT_WRITE)&&
				OUTPUT_IS_CORRECT_NUMBER_OF_BYTES(pdu.numberOfOutputs, pdu.numberOfBytes))
			{

				if(OUTPUTS_SetMultipleState(pdu.address, pdu.numberOfOutputs, pdu.data) == OUTPUT_ERROR_ADDRESS)
					errorCode = MODBUS_ILLEGAL_DATA_ADDRESS;
				else
					errorCode = MODBUS_ERROR_OK;
			}
			else
				errorCode = MODBUS_ILLEGAL_DATA_VALUE;
			break;
		}*/
		case MODBUS_READ_INPUT_REGISTERS:
		{
			if((pdu.numberOfOutputs > 0) && (pdu.numberOfOutputs <= INPUT_AMOUNT))
			{
				if(INPUTS_SetMultipleState(INPUT_TEMPERATURE, pdu.numberOfOutputs, pdu.data) != INPUT_ERROR_OK)
					errorCode = MODBUS_ILLEGAL_DATA_ADDRESS;
				else
					errorCode = MODBUS_ERROR_OK;
			}
			break;
		}
		default:
		{
			errorCode = MODBUS_ILLEGAL_FUNCTION;
			break;
		}
	}
	return errorCode;
}

void ModbusMaster_SendReadInputMessage()
{
	ModbusAdu data;
	u16_t crc16 = 0;
	data.bytes[0] = masterState.slaveAddress;
	memcpy(&(data.bytes[1]), readInputs, sizeof(readInputs));
	data.length = sizeof(readInputs) + 1 + 2;
	crc16 = ModbusCommon_CalculateCRC16(&data);
	data.bytes[data.length-2] = (crc16 & 0xFF);
	data.bytes[data.length-1] = ((crc16>>8) & 0xFF);
	ModbusMaster_Send(&data);
}
