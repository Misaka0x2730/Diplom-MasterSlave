#include "UI_MainWindow.h"

static const GUI_WIDGET_CREATE_INFO mainMenuDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect,	"Main Menu",			WIDGET_FRAME_MAIN_ID,		0,		0,						XSIZE_PHYS, YSIZE_PHYS,  0, 0 },
	{ TEXT_CreateIndirect,		"Current Mode:",		0,							LEFT_X, TOP_Y + 1 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Current State:",		0,							LEFT_X, TOP_Y + 2 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Temperature:",			0,							LEFT_X, TOP_Y + 3 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Pressure:",			0,							LEFT_X, TOP_Y + 4 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Humidity:",			0,							LEFT_X, TOP_Y + 5 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Received:",			0,							LEFT_X, TOP_Y + 6 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Transmitted:",			0,							LEFT_X, TOP_Y + 7 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Free Bytes:",			0,							LEFT_X, TOP_Y + 8 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },

	{ EDIT_CreateIndirect,		NULL,					WIDGET_TEXT_MODE_ID,		170, TOP_Y + 1 * INC_YVALUE,	80,		FONT_HEIGHT, 0, 0 },
	{ EDIT_CreateIndirect,		NULL,					WIDGET_TEXT_STATE_ID,		170, TOP_Y + 2 * INC_YVALUE,	80,		FONT_HEIGHT, 0, 0 },
	{ EDIT_CreateIndirect,		NULL,					WIDGET_TEXT_TEMP_ID,		170, TOP_Y + 3 * INC_YVALUE,	80,		FONT_HEIGHT, 0, 0 },
	{ EDIT_CreateIndirect,		NULL,					WIDGET_TEXT_PRESSURE_ID,	170, TOP_Y + 4 * INC_YVALUE,	80,		FONT_HEIGHT, 0, 0 },
	{ EDIT_CreateIndirect,		NULL,					WIDGET_TEXT_RH_ID,			170, TOP_Y + 5 * INC_YVALUE,	80,		FONT_HEIGHT, 0, 0 },
	{ EDIT_CreateIndirect,		NULL,					WIDGET_TEXT_RECV_ID,		170, TOP_Y + 6 * INC_YVALUE,	80,		FONT_HEIGHT, 0, 0 },
	{ EDIT_CreateIndirect,		NULL,					WIDGET_TEXT_TRANSM_ID,		170, TOP_Y + 7 * INC_YVALUE,	80,		FONT_HEIGHT, 0, 0 },
	{ EDIT_CreateIndirect,		NULL,					WIDGET_TEXT_FREE_BYTES_ID,	170, TOP_Y + 8 * INC_YVALUE,	80,		FONT_HEIGHT, 0, 0 },

	{ TEXT_CreateIndirect,		"C",					0,							270, TOP_Y + 3 * INC_YVALUE,	40,			FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"kPa",					0,							270, TOP_Y + 4 * INC_YVALUE,	40,			FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"%",					0,							270, TOP_Y + 5 * INC_YVALUE,	40,			FONT_HEIGHT, 0, 0 },
};


void UI_DisplayMainMenu(WM_HWIN parentWindow)
{
	Device_SaveConfig();
	TEXT_SetDefaultFont(&MAIN_FONT);
	WM_HWIN mainWindow = WM_CreateWindowAsChild(0, 0, LCD_GetXSize(), LCD_GetYSize(), parentWindow, WM_CF_SHOW, UI_MainMenuWindowCallback, 0);
	WM_HWIN dialog = GUI_CreateDialogBox(mainMenuDialogCreate, GUI_COUNTOF(mainMenuDialogCreate), UI_MainMenuDialogCallback, mainWindow, 0, 0);
	WM_SetFocus(dialog);
	WM_SendMessageNoPara(mainWindow, MESSAGE_CREATE);
}

static void UI_MainMenuWindowCallback(WM_MESSAGE *pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	switch (pMsg->MsgId)
	{
		case MESSAGE_CREATE:
		{
			WM_HTIMER timer = WM_CreateTimer(hWin, 0, 500, 0);
			break;
		}
		case WM_TIMER:
		{
			UI_MainMenuSetValue(hWin);
			WM_RestartTimer((WM_HTIMER)(pMsg->Data.v), 500);
			break;
		}
		case WM_KEY:
		{
			WM_KEY_INFO key = *(WM_KEY_INFO*)(pMsg->Data.p);
			if (key.PressedCnt == 0)
			{
				switch (key.Key)
				{
					case GUI_KEY_RIGHT:
					{
						if (Device_GetState() != STATE_RUN)
						{
							WM_MESSAGE messageToParent;
							messageToParent.MsgId = MESSAGE_MENU_CHANGED_NEXT;
							messageToParent.Data.v = MAIN_MENU;
							WM_SendToParent(hWin, &messageToParent);
						}
						break;
					}
					case GUI_KEY_ENTER:
					{
						Device_SwitchState();
						UI_MainMenuSetValue(hWin);
						break;
					}
				}
			}
			break;
		}
		case MESSAGE_MENU_CHANGED_NEXT:
		case MESSAGE_MENU_CHANGED_PREV:
		{
			WM_MESSAGE messageToParent;
			messageToParent.MsgId = pMsg->MsgId;
			messageToParent.Data.v = pMsg->Data.v;
			WM_SendToParent(hWin, &messageToParent);
			break;
		}
		default:
		{
			WM_DefaultProc(pMsg);
			break;
		}
	}
}

static void UI_MainMenuDialogCallback(WM_MESSAGE *pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	switch (pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
		{
			FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER);
			FRAMEWIN_SetFont(hWin, &MAIN_FONT);
			UI_MainMenuInitEdit(hWin);
			UI_MainMenuSetValue(hWin);
			break;
		}
		case WM_NOTIFY_PARENT:
		{
			int ID = WM_GetId(pMsg->hWinSrc);
			int Ncode = pMsg->Data.v;
			break;
		}
		default:
		{
			WM_DefaultProc(pMsg);
			break;
		}
	}
}

static void UI_MainMenuInitEdit(WM_HWIN hWin)
{
	WM_HWIN hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_MODE_ID);
	EDIT_SetFont(hItem, &MAIN_FONT);
	EDIT_SetTextMode(hItem);
	EDIT_SetFocussable(hItem, 0);

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_STATE_ID);
	EDIT_SetFont(hItem, &MAIN_FONT);
	EDIT_SetTextMode(hItem);
	EDIT_SetFocussable(hItem, 0);

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_TEMP_ID);
	EDIT_SetFont(hItem, &MAIN_FONT);
	EDIT_SetDecMode(hItem, Device_GetTemp(), -999, 999, 1, GUI_EDIT_SIGNED);
	EDIT_SetMaxLen(hItem, 5);
	EDIT_SetFocussable(hItem, 0);

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_PRESSURE_ID);
	EDIT_SetFont(hItem, &MAIN_FONT);
	EDIT_SetDecMode(hItem, Device_GetPressure(), 0, 2000, 1, GUI_EDIT_NORMAL);
	EDIT_SetFocussable(hItem, 0);

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_RH_ID);
	EDIT_SetFont(hItem, &MAIN_FONT);
	EDIT_SetDecMode(hItem, Device_GetRH(), 0, 99, 0, GUI_EDIT_NORMAL);
	EDIT_SetMaxLen(hItem, 2);
	EDIT_SetFocussable(hItem, 0);

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_RECV_ID);
	EDIT_SetFont(hItem, &MAIN_FONT);
	EDIT_SetDecMode(hItem, Device_GetReceived(), 0, 1000000, 0, GUI_EDIT_NORMAL);
	EDIT_SetMaxLen(hItem, 7);
	EDIT_SetFocussable(hItem, 0);

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_TRANSM_ID);
	EDIT_SetFont(hItem, &MAIN_FONT);
	EDIT_SetDecMode(hItem, Device_GetTransmitted(), 0, 1000000, 0, GUI_EDIT_NORMAL);
	EDIT_SetMaxLen(hItem, 7);
	EDIT_SetFocussable(hItem, 0);

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_FREE_BYTES_ID);
	EDIT_SetFont(hItem, &MAIN_FONT);
	EDIT_SetDecMode(hItem, GUI_ALLOC_GetNumFreeBytes(), 0, 1000000, 0, GUI_EDIT_NORMAL);
	EDIT_SetMaxLen(hItem, 7);
	EDIT_SetFocussable(hItem, 0);
}

static void UI_MainMenuSetValue(WM_HWIN hWin)
{
	GUI_ConstString modeArray[] = { "Slave", "Master" };
	GUI_ConstString stateArray[] = { "Stop", "Run" };

	WM_HWIN hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_MODE_ID);
	EDIT_SetText(hItem, modeArray[Device_GetMode()]);

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_STATE_ID);
	EDIT_SetText(hItem, stateArray[Device_GetState()]);

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_TEMP_ID);
	EDIT_SetValue(hItem, Device_GetTemp());

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_PRESSURE_ID);
	EDIT_SetValue(hItem, Device_GetPressure());

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_RH_ID);
	EDIT_SetValue(hItem, Device_GetRH());

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_RECV_ID);
	EDIT_SetValue(hItem, Device_GetReceived());

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_TRANSM_ID);
	EDIT_SetValue(hItem, Device_GetTransmitted());

	hItem = WM_GetDialogItem(hWin, WIDGET_TEXT_FREE_BYTES_ID);
	EDIT_SetValue(hItem, GUI_ALLOC_GetNumFreeBytes());
}
