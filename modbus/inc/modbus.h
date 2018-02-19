/*
* modbus.h
 *
 *  Created on: 17 янв. 2017 г.
 *      Author: misaka10032
 */

#ifndef MODBUS_H_
#define MODBUS_H_

#include "types.h"

typedef enum
{
	ModbusProtocol_RTU = 0x01,
	ModbusProtocol_TCP = 0x02,
	ModbusProtocol_ASCII = 0x03,
} ModbusProtocol;

typedef enum
{
	ModbusDataSize_TCP_Min_ADU_Size = 7,
	ModbusDataSize_TCP_Max_ADU_Size = 260,
	ModbusDataSize_TCP_Function_Position = 7,
	ModbusDataSize_RTU_Min_ADU_Size = 4,
	ModbusDataSize_RTU_Max_ADU_Size = 256,
	ModbusDataSize_RTU_Function_Position = 1,
	ModbusDataSize_Max_ADU_Size = 260,
	ModbusDataSize_Max_Data_Size = 253,
} ModbusDataSize;

typedef enum
{
	ModbusAddress_User_First_Min = 65,
	ModbusAddress_User_First_Max = 72,
	ModbusAddress_User_Second_Min = 100,
	ModbusAddress_User_Second_Max = 110,
} ModbusAddress;

typedef enum
{
	ModbusRTUParameter_Parity_None,
	ModbusRTUParameter_Parity_Even,
	ModbusRTUParameter_Parity_Odd,
	ModbusRTUParameter_Stop_One,
	ModbusRTUParameter_Stop_One_Point_Five,
	ModbusRTUParameter_Stop_Two,
} ModbusRTUParameter;

typedef enum
{
	ModbusStandartFunction_Read_Coils 					 = 0x01,
	ModbusStandartFunction_Read_Discrete_Inputs 		 = 0x02,
	ModbusStandartFunction_Read_Holding_Registers 		 = 0x03,
	ModbusStandartFunction_Read_Input_Registers 		 = 0x04,
	ModbusStandartFunction_Write_Single_Coil 			 = 0x05,
	ModbusStandartFunction_Write_Single_Register 		 = 0x06,
	ModbusStandartFunction_Write_Multiple_Coils 		 = 0x0F,
	ModbusStandartFunction_Write_Multiple_Registers 	 = 0x10,
	ModbusStandartFunction_Report_Server_ID 			 = 0x11,		//Serial Line only
	ModbusStandartFunction_Read_Write_Multiple_Registers = 0x17
} ModbusStandartFunction;

typedef enum
{
	ModbusReturnErrorCode_Error_OK = 0x00,
	ModbusReturnErrorCode_Illegal_Protocol = 0x01,
	ModbusReturnErrorCode_Error_Length = 0x02,
} ModbusReturnErrorCode;

typedef enum
{
	MODBUS_ILLEGAL_FUNCTION 						= 0x01,
	MODBUS_ILLEGAL_DATA_ADDRESS 					= 0x02,
	MODBUS_ILLEGAL_DATA_VALUE 						= 0x03,
	MODBUS_SERVER_DEVICE_FAILURE 					= 0x04,
	MODBUS_ACKNOWLEDGE								= 0x05,
	MODBUS_SERVER_DEVICE_BUSY 						= 0x06,
	MODBUS_MEMORY_PARITY_ERROR 						= 0x08,
	MODBUS_GATEWAY_PATH_UNAVAILABLE 			    = 0x0A,
	MODBUS_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND  = 0x0B,
} ModbusResponseErrorCode;


typedef enum
{
	MODBUS_COIL_MAX_READ 		  = 0x07D0,
	MODBUS_COIL_MAX_WRITE 		  = 0x07B0,
	MODBUS_COIL_MIN_START_ADDRESS = 0x0000,
	MODBUS_COIL_MAX_START_ADDRESS = 0xFFFF,
} ModbusCoilParam;

typedef enum
{
	MODBUS_HOLDING_REGISTER_MAX_READ		  = 0x007D,
	MODBUS_HOLDING_REGISTER_MAX_WRITE 		  = 0x007B,
	MODBUS_HOLDING_REGISTER_MIN_START_ADDRESS = 0x0000,
	MODBUS_HOLDING_REGISTER_MAX_START_ADDRESS = 0xFFFF,
} ModbusHoldingRegisterParam;

typedef enum
{
	MODBUS_DISCRETE_INPUT_MAX_READ 		    = 0x07D0,
	MODBUS_DISCRETE_INPUT_MIN_START_ADDRESS = 0x0000,
	MODBUS_DISCRETE_INPUT_MAX_START_ADDRESS = 0xFFFF,
} ModbusDiscreteInputParam;

typedef enum
{
	MODBUS_INPUT_REGISTER_MAX_READ 			= 0x007D,
	MODBUS_INPUT_REGISTER_MIN_START_ADDRESS = 0x0000,
	MODBUS_INPUT_REGISTER_MAX_START_ADDRESS = 0xFFFF,
} ModbusInputRegisterParam;

typedef struct
{
	u8_t  data[ModbusDataSize_Max_ADU_Size];
	u16_t length;
	u8_t  protocol;
} ModbusADU;

typedef struct
{
	u8_t  protocol;
	u8_t  slaveAddress;
	u8_t  functionCode;
	u16_t startAddress;
	u16_t quantityOfCoils;
	u16_t quantityOfInputs;
	u16_t quantityOfRegister;
	u16_t byteCount;
	u8_t  data[ModbusDataSize_Max_Data_Size];
} ModbusPDU;

typedef struct
{
	u16_t transactionID;
	u16_t protocolID;
	u16_t length;
	u8_t slaveAddress;
} ModbusMBAP;

typedef struct
{
	u8_t  slaveAddress;
	u32_t baudrate;
	u8_t  stop;
	u8_t  parity;
} ModbusRTU_InitTypeDef;

typedef int (*modbus_user_func_process)(ModbusPDU* pdu);

typedef struct
{
	u8_t functionCode;
	modbus_user_func_process processFunction;
} ModbusFunction;

#define MODBUS_IS_PROTOCOL(PROTOCOL) ((PROTOCOL == ModbusProtocol_RTU) || (PROTOCOL == ModbusProtocol_TCP) || (PROTOCOL == ModbusProtocol_ASCII))

#define MODBUS_RTU_CHECK_SIZE(SIZE) ((SIZE >= ModbusDataSize_RTU_Min_ADU_Size) && (SIZE <= ModbusDataSize_RTU_Max_ADU_Size))
#define MODBUS_TCP_CHECK_SIZE(SIZE) ((SIZE >= ModbusDataSize_TCP_Min_ADU_Size) && (SIZE <= ModbusDataSize_TCP_Max_ADU_Size))
#define MODBUS_CHECK_SIZE(PROTOCOL, SIZE) (((PROTOCOL == ModbusProtocol_RTU) && MODBUS_RTU_CHECK_SIZE(SIZE)) || \
										   ((PROTOCOL == ModbusProtocol_TCP) && MODBUS_TCP_CHECK_SIZE(SIZE)))

ModbusReturnErrorCode Modbus_ParsingADU(ModbusADU* adu, ModbusMBAP* mbap, ModbusPDU* pdu);

#endif /* MODBUS_H_ */
