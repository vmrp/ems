/*
最后更新 20120223
*/

#include "help.h"
#include "mainwnd.h"

#include "i18n.h"
#include "smp.h"
#include "smp_toolbar.h"
#include "smp_textinfo.h"
#include "smp_menuwnd.h"
#include "smp_edit.h"
#include "smp_button.h"
#include "smp_msgbox.h"
#include "bmp.h"
#include "smp_label.h"
#include "smp_progbar.h"
#include "smp_combobox.h"
#include "smp_list.h"
#include "topwnd.h"
#include "string.h"
#include "smp_titlebar.h"

//子窗口ID
enum
{
    HELP_TITLEBAR = 1,
	HELP_TOOLBAR,
    HELP_TEXTINFO
};

/////////////////////////////////////////////////////////

VOID ShowHELP(WID wndid, DWORD str)
{
    HWND hwnd;
    HideTopWindow(wndid, FALSE, FALSE);
    hwnd=ShowTopWindow(TOPWND_HELP, NULL, (DWORD)str);
    _SET_WINDATA(hwnd,wndid);	
}

static VOID HideHELP(HWND hwnd)
{
	HideTopWindow(TOPWND_HELP, TRUE, FALSE);
	ShowTopWindow(_GET_WINDATA(hwnd,WID), NULL, 0);
}


LRESULT HELP_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	
	switch(Msg)
	{
	case WM_CREATE:
	{
	    HWND hControl;

        //标题栏
		hControl = SGL_CreateWindow(SMP_Titlebar_WndProc,
			0, 0, SCREEN_WIDTH, SMP_ITEM_HEIGHT,
			HELP_TITLEBAR, 
			SMP_TITLEBAR_STATIC, 0);
		_FGCOLOR(hControl) = COLOR_lightwhite;
		SMP_Titlebar_SetContent(hControl, RESID_INVALID, SGL_LoadString(STR_HELP),FALSE);
		SGL_AddChildWindow(hWnd, hControl);

		//创建工具栏
		hControl = SGL_CreateWindow(SMP_Toolbar_WndProc, 
			0,  _HEIGHT(hWnd) - SMP_ITEM_HEIGHT, 
			_WIDTH(hWnd), SMP_ITEM_HEIGHT,
			HELP_TOOLBAR,
			0, 0);
		SMP_Toolbar_SetStrings(hControl,  RESID_INVALID, RESID_INVALID, STR_BACK, FALSE);
		SGL_AddChildWindow(hWnd, hControl);

        //文本框
        hControl = SGL_CreateWindow(SMP_TextInfo_WndProc,
            0,SMP_ITEM_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT - SMP_ITEM_HEIGHT*2,
            HELP_TEXTINFO,0,0);
		SGL_AddChildWindow(hWnd, hControl);
		break;
	}
	case WM_INITFOCUS:
	{
        SGL_SetFocusWindow(hWnd,SGL_FindChildWindow(hWnd,HELP_TEXTINFO));
		break;
	}
    case WM_SHOW:
    {   
        SMP_TextInfo_SetContent(SGL_FindChildWindow(hWnd,HELP_TEXTINFO),SGL_LoadString(_USERDATA(hWnd)));
        break;
    }
	case WM_KEYUP:
	{
		switch(wParam)
		{
		case MR_KEY_SOFTRIGHT:
            HideHELP(hWnd);
			return 1;
		}
		break;
	}
	}
	return 0;
}

