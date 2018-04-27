#pragma once

#include "LCDConf_FlexColor_Template.h"
#include "GUI.h"
#include "SPINBOX.h"
#include "TEXT.h"
#include "DROPDOWN.h"
#include "FRAMEWIN.h"
#include "BUTTON.h"
#include "EDIT.h"
#include "UI_Struct.h"
#include "device.h"

void		UI_DisplayMainMenu(WM_HWIN parentWindow);
static void UI_MainMenuWindowCallback(WM_MESSAGE *pMsg);
static void UI_MainMenuDialogCallback(WM_MESSAGE *pMsg);
static void	UI_MainMenuInitEdit(WM_HWIN hWin);
static void	UI_MainMenuSetValue(WM_HWIN hWin);
