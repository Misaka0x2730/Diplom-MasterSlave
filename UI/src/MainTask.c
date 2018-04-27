/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2004  SEGGER Microcontroller Systeme GmbH        *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : BASIC_HelloWorld.c
Purpose     : Simple demo drawing "Hello world"
----------------------------------------------------------------------
*/

#include "MainTask.h"
#include "UI_Struct.h"
#include "UI_MainWindow.h"
#include "UI_SettingsWindow.h"
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/

void UI_MenuTask()
{
	FRAMEWIN_SetDefaultSkinClassic();
	FRAMEWIN_SetDefaultTextColor(1, GUI_WHITE);
	GUI_Clear();
	WM_HWIN window = WM_CreateWindow(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_CF_SHOW, UI_MainWindowCallback, 16);
	WM_SendMessageNoPara(window, MESSAGE_CREATE);
}

static void UI_MainWindowCallback(WM_MESSAGE *pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	static u8_t currentMenu = MAIN_MENU;
	switch (pMsg->MsgId)
	{
		case MESSAGE_CREATE:
		{
			UI_DisplayMainMenu(hWin);
			break;
		}
		case MESSAGE_MENU_CHANGED_NEXT:
		{
			u8_t menu = pMsg->Data.v;
			switch (menu)
			{
				case MAIN_MENU:
				{
					WM_DeleteWindow(pMsg->hWinSrc);
					UI_DisplaySettingsMenu(hWin);
					break;
				}
			}
			break;
		}
		case MESSAGE_MENU_CHANGED_PREV:
		{
			u8_t menu = pMsg->Data.v;
			switch (menu)
			{
				case SETTINGS_MENU:
				{
					WM_DeleteWindow(pMsg->hWinSrc);
					UI_DisplayMainMenu(hWin);
					break;
				}
			}
			break;
		}
		default:
		{
			WM_DefaultProc(pMsg);
			break;
		}
	}
}

void MainTask(void) 
{
	//GUI_Init();
	//GUI_Clear();
	UI_MenuTask();
}

/*************************** End of file ****************************/
