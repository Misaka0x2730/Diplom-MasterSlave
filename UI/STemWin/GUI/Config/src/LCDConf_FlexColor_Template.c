/*********************************************************************
*          Portions COPYRIGHT 2016 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.32 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf_FlexColor_Template.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "LCDConf_FlexColor_Template.h"
#include "stm32f10x_conf.h"
#include "pinMap.h"
#include "delay.h"
#include "gpio.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/

//
// Physical display size
//

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif


void LCD_SendByte(uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data);
}

void LCD_SendCmd(uint8_t cmd)
{
	GPIO_xResetBit(PIN_LCD_RS);
	GPIO_xResetBit(PIN_LCD_CS);
	delay_us(1);
	LCD_SendByte(cmd);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)!=RESET);
	delay_us(1);
	GPIO_xSetBit(PIN_LCD_CS);
}

void LCD_SendData(uint8_t data)
{
	GPIO_xSetBit(PIN_LCD_RS);
	GPIO_xResetBit(PIN_LCD_CS);
	delay_us(1);
	LCD_SendByte(data);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)!=RESET);
	delay_us(1);
	GPIO_xSetBit(PIN_LCD_CS);
}

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
/********************************************************************
*
*       LcdWriteReg
*
* Function description:
*   Sets display register
*/
static void LcdWriteReg(U8 Data)
{
	GPIO_xResetBit(PIN_LCD_RS);
	GPIO_xResetBit(PIN_LCD_CS);
	SPI_I2S_SendData(SPI1, Data);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);
	GPIO_xSetBit(PIN_LCD_CS);
}

/********************************************************************
*
*       LcdWriteData
*
* Function description:
*   Writes a value to a display register
*/
static void LcdWriteData(U8 Data)
{
	GPIO_xSetBit(PIN_LCD_RS);
	GPIO_xResetBit(PIN_LCD_CS);
	SPI_I2S_SendData(SPI1, Data);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);
	GPIO_xSetBit(PIN_LCD_CS);
}

/********************************************************************
*
*       LcdWriteDataMultiple
*
* Function description:
*   Writes multiple values to a display register.
*/
static void LcdWriteDataMultiple(U8 * pData, int NumItems)
{
	GPIO_xSetBit(PIN_LCD_RS);
	GPIO_xResetBit(PIN_LCD_CS);
	while (NumItems--)
	{
		SPI_I2S_SendData(SPI1, *(pData++));
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	}
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);
	GPIO_xSetBit(PIN_LCD_CS);
}

/********************************************************************
*
*       LcdReadDataMultiple
*
* Function description:
*   Reads multiple values from a display register.
*/
static void LcdReadDataMultiple(U8 * pData, int NumItems)
{
	GPIO_xSetBit(PIN_LCD_RS);
	GPIO_xResetBit(PIN_LCD_CS);
	while (NumItems--)
	{
	  SPI_I2S_SendData(SPI1, 0xFF);
	  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	  *(pData++) = SPI_I2S_ReceiveData(SPI1);
	}
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);
	GPIO_xSetBit(PIN_LCD_CS);
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  //
  // Set display driver and color conversion
  //
  pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetSizeEx (0, YSIZE_PHYS , XSIZE_PHYS);
  LCD_SetVSizeEx(0, VYSIZE_PHYS, VXSIZE_PHYS);
  //
  // Orientation
  //
  Config.Orientation = GUI_SWAP_XY;
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set controller and operation mode
  //
  /*PortAPI.pfWrite8_A0  = LcdWriteReg;
  PortAPI.pfWrite8_A1  = LcdWriteData;
  PortAPI.pfWriteM8_A1 = LcdWriteDataMultiple;
  PortAPI.pfReadM8_A1  = LcdReadDataMultiple;
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B8);*/
  PortAPI.pfWrite8_A0  = LcdWriteReg;
  PortAPI.pfWrite8_A1  = LcdWriteData;
  PortAPI.pfWriteM8_A1 = LcdWriteDataMultiple;
  PortAPI.pfReadM8_A1  = LcdReadDataMultiple;
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B8);
}


/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;
  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER:
  {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_xInit(PIN_LCD_MOSI, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
	GPIO_xInit(PIN_LCD_MISO, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
	GPIO_xInit(PIN_LCD_SCK, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);

	GPIO_xInit(PIN_LCD_RST, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIO_xInit(PIN_LCD_CS, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIO_xInit(PIN_LCD_RS, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIO_xInit(PIN_LCD_BL, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIO_xSetBit(PIN_LCD_BL);

	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
	SPI_SSOutputCmd(SPI1, DISABLE);

	GPIO_xResetBit(PIN_LCD_RST);
	delay_ms(100);
	GPIO_xSetBit(PIN_LCD_RST);

	//GPIO_xResetBits(GPIOE, GPIO_Pin_2);
	LCD_SendCmd(LCD_POWERA);
	LCD_SendData(0x39);
	LCD_SendData(0x2C);
	LCD_SendData(0x00);
	LCD_SendData(0x34);
	LCD_SendData(0x02);
	LCD_SendCmd(LCD_POWERB);
	LCD_SendData(0x00);
	LCD_SendData(0xC1);
	LCD_SendData(0x30);
	LCD_SendCmd(LCD_DTCA);
	LCD_SendData(0x85);
	LCD_SendData(0x00);
	LCD_SendData(0x78);
	LCD_SendCmd(LCD_DTCB);
	LCD_SendData(0x00);
	LCD_SendData(0x00);
	LCD_SendCmd(LCD_POWER_SEQ);
	LCD_SendData(0x64);
	LCD_SendData(0x03);
	LCD_SendData(0x12);
	LCD_SendData(0x81);
	LCD_SendCmd(LCD_PRC);
	LCD_SendData(0x20);
	LCD_SendCmd(LCD_POWER1);
	LCD_SendData(0x23);
	LCD_SendCmd(LCD_POWER2);
	LCD_SendData(0x10);
	LCD_SendCmd(LCD_VCOM1);
	LCD_SendData(0x3E);
	LCD_SendData(0x28);
	LCD_SendCmd(LCD_VCOM2);
	LCD_SendData(0x86);
	LCD_SendCmd(LCD_MAC);
	LCD_SendData(0x48);
	LCD_SendCmd(LCD_PIXEL_FORMAT);
	LCD_SendData(0x55);
	LCD_SendCmd(LCD_FRMCTR1);
	LCD_SendData(0x00);
	LCD_SendData(0x18);
	LCD_SendCmd(LCD_DFC);
	LCD_SendData(0x08);
	LCD_SendData(0x82);
	LCD_SendData(0x27);
	LCD_SendCmd(LCD_3GAMMA_EN);
	LCD_SendData(0x00);
	LCD_SendCmd(LCD_COLUMN_ADDR);
	LCD_SendData(0x00);
	LCD_SendData(0x00);
	LCD_SendData(0x00);
	LCD_SendData(0xEF);
	LCD_SendCmd(LCD_PAGE_ADDR);
	LCD_SendData(0x00);
	LCD_SendData(0x00);
	LCD_SendData(0x01);
	LCD_SendData(0x3F);
	LCD_SendCmd(LCD_GAMMA);
	LCD_SendData(0x01);
	LCD_SendCmd(LCD_PGAMMA);
	LCD_SendData(0x0F);
	LCD_SendData(0x31);
	LCD_SendData(0x2B);
	LCD_SendData(0x0C);
	LCD_SendData(0x0E);
	LCD_SendData(0x08);
	LCD_SendData(0x4E);
	LCD_SendData(0xF1);
	LCD_SendData(0x37);
	LCD_SendData(0x07);
	LCD_SendData(0x10);
	LCD_SendData(0x03);
	LCD_SendData(0x0E);
	LCD_SendData(0x09);
	LCD_SendData(0x00);
	LCD_SendCmd(LCD_NGAMMA);
	LCD_SendData(0x00);
	LCD_SendData(0x0E);
	LCD_SendData(0x14);
	LCD_SendData(0x03);
	LCD_SendData(0x11);
	LCD_SendData(0x07);
	LCD_SendData(0x31);
	LCD_SendData(0xC1);
	LCD_SendData(0x48);
	LCD_SendData(0x08);
	LCD_SendData(0x0F);
	LCD_SendData(0x0C);
	LCD_SendData(0x31);
	LCD_SendData(0x36);
	LCD_SendData(0x0F);
	LCD_SendCmd(LCD_SLEEP_OUT);

	delay_ms(100);
	LCD_SendCmd(LCD_DISPLAY_ON);
	LCD_SendCmd(LCD_GRAM);
	//GPIO_SetBits(GPIOE, GPIO_Pin_2);
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/

