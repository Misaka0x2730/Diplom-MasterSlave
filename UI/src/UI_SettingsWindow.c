#include "UI_SettingsWindow.h"

static const GUI_WIDGET_CREATE_INFO settingsSlaveDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect,	"Settings",				WIDGET_FRAME_SLAVE_ID,		0,		0,						XSIZE_PHYS, YSIZE_PHYS,  0, 0 },
	{ TEXT_CreateIndirect,		"Mode:",				0,							LEFT_X, TOP_Y + 1 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Address:",				0,							LEFT_X, TOP_Y + 2 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Baud:",				0,							LEFT_X, TOP_Y + 3 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Waiting time:",		0,							LEFT_X, TOP_Y + 4 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ LISTBOX_CreateIndirect,	NULL,					WIDGET_MODE_ID,				170,	TOP_Y + 1 * INC_YVALUE, 120,		FONT_HEIGHT, 0, 0 },
	{ SPINBOX_CreateIndirect,	NULL,					WIDGET_ADDRESS_ID,			170,	TOP_Y + 2 * INC_YVALUE, 120,		FONT_HEIGHT, 0, 0 },
	{ LISTBOX_CreateIndirect,	NULL,					WIDGET_BAUD_ID,				170,	TOP_Y + 3 * INC_YVALUE,	120,		FONT_HEIGHT, 0, 0 },
	{ SPINBOX_CreateIndirect,	NULL,					WIDGET_WAITING_TIME_ID,		170,	TOP_Y + 4 * INC_YVALUE,	120,		FONT_HEIGHT, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO settingsMasterDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect,	"Settings",				WIDGET_FRAME_MASTER_ID,		0,		0,						XSIZE_PHYS, YSIZE_PHYS,  0, 0 },
	{ TEXT_CreateIndirect,		"Mode:",				0,							LEFT_X, TOP_Y + 1 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Address:",				0,							LEFT_X, TOP_Y + 2 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Baud:",				0,							LEFT_X, TOP_Y + 3 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ TEXT_CreateIndirect,		"Interview period:",	0,							LEFT_X, TOP_Y + 4 * INC_YVALUE, 140,		FONT_HEIGHT, 0, 0 },
	{ LISTBOX_CreateIndirect,	NULL,					WIDGET_MODE_ID,				170,	TOP_Y + 1 * INC_YVALUE, 120,		FONT_HEIGHT, 0, 0 },
	{ SPINBOX_CreateIndirect,	NULL,					WIDGET_ADDRESS_ID,			170,	TOP_Y + 2 * INC_YVALUE, 120,		FONT_HEIGHT, 0, 0 },
	{ LISTBOX_CreateIndirect,	NULL,					WIDGET_BAUD_ID,				170,	TOP_Y + 3 * INC_YVALUE,	120,		FONT_HEIGHT, 0, 0 },
	{ SPINBOX_CreateIndirect,	NULL,					WIDGET_PERIOD_ID,			170,	TOP_Y + 4 * INC_YVALUE,	120,		FONT_HEIGHT, 0, 0 }
};

void UI_DisplaySettingsMenu(WM_HWIN parentWindow)
{
	TEXT_SetDefaultFont(&MAIN_FONT);
	WM_HWIN settingsWindow = WM_CreateWindowAsChild(0, 0, LCD_GetXSize(), LCD_GetYSize(), parentWindow, WM_CF_SHOW, UI_SettingsWindowCallback, 0);
	if(Device_GetMode() == MODE_SLAVE)
		UI_DisplaySettingsSlaveMenu(settingsWindow);
	else
		UI_DisplaySettingsMasterMenu(settingsWindow);
}

static void UI_DisplaySettingsSlaveMenu(WM_HWIN parentWindow)
{
	WM_HWIN dialog = GUI_CreateDialogBox(settingsSlaveDialogCreate, GUI_COUNTOF(settingsSlaveDialogCreate), UI_SettingsDialogCallback, parentWindow, 0, 0);
	WM_MESSAGE messageToDialog;
	messageToDialog.MsgId = MESSAGE_CREATE;
	messageToDialog.Data.v = MODE_SLAVE;
	WM_SendMessage(WM_GetClientWindow(dialog), &messageToDialog);
}

static void UI_DisplaySettingsMasterMenu(WM_HWIN parentWindow)
{
	WM_HWIN dialog = GUI_CreateDialogBox(settingsMasterDialogCreate, GUI_COUNTOF(settingsMasterDialogCreate), UI_SettingsDialogCallback, parentWindow, 0, 0);
	WM_MESSAGE messageToDialog;
	messageToDialog.MsgId = MESSAGE_CREATE;
	messageToDialog.Data.v = MODE_MASTER;
	WM_SendMessage(WM_GetClientWindow(dialog), &messageToDialog);
}

static void UI_SettingsWindowCallback(WM_MESSAGE *pMsg)
{
	WM_HWIN hWin = pMsg->hWin;

	switch (pMsg->MsgId)
	{
		case MESSAGE_SETTINGS_MODE_CHANGED:
		{
			u8_t mode = pMsg->Data.v;
			WM_DeleteWindow(pMsg->hWinSrc);
			if (mode == MODE_SLAVE)
				UI_DisplaySettingsSlaveMenu(hWin);
			else
				UI_DisplaySettingsMasterMenu(hWin);
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

static void UI_SettingsDialogCallback(WM_MESSAGE *pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	GUI_ConstString modeArray[] = { "Slave", "Master" };
	GUI_ConstString baudArray[] = { "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200" };
	static u8_t changeFlag = 0;
	switch (pMsg->MsgId)
	{
		case MESSAGE_CREATE:
		{
			WM_HWIN hItem = WM_GetDialogItem(hWin, WIDGET_MODE_ID);
			for (u8_t i = 0; i < GUI_COUNTOF(modeArray); i++)
				LISTBOX_AddString(hItem, modeArray[i]);
			LISTBOX_SetFont(hItem, &MAIN_FONT);
			changeFlag = 1;
			LISTBOX_SetSel(hItem, pMsg->Data.v);

			hItem = WM_GetDialogItem(hWin, WIDGET_ADDRESS_ID);
			SPINBOX_SetRange(hItem, MODBUS_RTU_MIN_ADDRESS, MODBUS_RTU_MAX_ADDRESS);
			SPINBOX_SetFont(hItem, &MAIN_FONT);
			SPINBOX_SetValue(hItem, Device_GetAddress());

			hItem = WM_GetDialogItem(hWin, WIDGET_BAUD_ID);
			for (u8_t i = 0; i < GUI_COUNTOF(baudArray); i++)
				LISTBOX_AddString(hItem, baudArray[i]);
			LISTBOX_SetFont(hItem, &MAIN_FONT);
			LISTBOX_SetSel(hItem, Device_GetBaud());

			if (pMsg->Data.v == MODE_SLAVE)
			{
				hItem = WM_GetDialogItem(hWin, WIDGET_WAITING_TIME_ID);
				SPINBOX_SetRange(hItem, 1, 999);
				SPINBOX_SetFont(hItem, &MAIN_FONT);
				SPINBOX_SetValue(hItem, Device_GetWaitingTime());
			}
			else
			{
				hItem = WM_GetDialogItem(hWin, WIDGET_PERIOD_ID);
				SPINBOX_SetRange(hItem, 1, 999);
				SPINBOX_SetFont(hItem, &MAIN_FONT); SPINBOX_SetValue(hItem, Device_GetInterviewPeriod());
				SPINBOX_SetValue(hItem, Device_GetInterviewPeriod());
			}

			FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER);
			FRAMEWIN_SetFont(hWin, &MAIN_FONT);

			break;
		}
		case WM_NOTIFY_PARENT:
		{
			int ID = WM_GetId(pMsg->hWinSrc);
			int Ncode = pMsg->Data.v;
			switch (ID)
			{
				case WIDGET_MODE_ID:
				{
					switch (Ncode)
					{
						case WM_NOTIFICATION_SEL_CHANGED:
						{
							if (!changeFlag)
							{
								WM_MESSAGE messageToParent;
								messageToParent.MsgId = MESSAGE_SETTINGS_MODE_CHANGED;
								messageToParent.Data.v = LISTBOX_GetSel((LISTBOX_Handle)(pMsg->hWinSrc));
								WM_SendToParent(hWin, &messageToParent);
							}
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case WM_KEY:
		{
			WM_KEY_INFO key = *(WM_KEY_INFO*)(pMsg->Data.p);
			if (key.PressedCnt == 0)
			{
				switch (key.Key)
				{
					case GUI_KEY_ENTER:
					{
						GUI_SendKeyMsg(GUI_KEY_TAB, 1);
						break;
					}
					case GUI_KEY_LEFT:
					{
						WM_HWIN hItem = WM_GetDialogItem(hWin, WIDGET_MODE_ID);
						Device_SetMode(LISTBOX_GetSel(hItem));
						
						hItem = WM_GetDialogItem(hWin, WIDGET_ADDRESS_ID);
						Device_SetAddress(SPINBOX_GetValue(hItem));

						hItem = WM_GetDialogItem(hWin, WIDGET_BAUD_ID);
						Device_SetBaud(LISTBOX_GetSel(hItem));

						if (Device_GetMode() == MODE_SLAVE)
						{
							hItem = WM_GetDialogItem(hWin, WIDGET_WAITING_TIME_ID);
							Device_SetWaitingTime(SPINBOX_GetValue(hItem));
						}
						else
						{
							hItem = WM_GetDialogItem(hWin, WIDGET_PERIOD_ID);
							Device_SetInterviewPeriod(SPINBOX_GetValue(hItem));
						}

						WM_MESSAGE messageToParent;
						messageToParent.MsgId = MESSAGE_MENU_CHANGED_PREV;
						messageToParent.Data.v = SETTINGS_MENU;
						WM_SendToParent(hWin, &messageToParent);
						break;
					}
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
	changeFlag = 0;
}
