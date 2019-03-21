#include "preview.h"
#include "mainwnd.h"
#include "topwnd.h"
#include "string.h"
#include "opendlg.h"
#include "momo.h"
#include "i18n.h"
#include "smp.h"
#include "smp_menu.h"
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
#include "smp_titlebar.h"

enum {
    PREVIEW_TOOLBAR=1,
    PREVIEW_TITLEBAR
};
static int32 g_timer;
char hmz;
/////////////////////////////////////////////////////////
static void yulanANMWin(int32 data)
{
    int16 i,k,istop,kstop;
    int16 x,y,p,q,w;

    x = (SCREEN_WIDTH>>1) - 8;
    y = (SCREEN_HEIGHT>>1) - 8;

    mrc_drawRect(x,y,16,16,50,101,150);
    switch(hmz)//menuwz在这时用作切换画面帧用
    {
    case 0:
        k=i=0,kstop=istop=15;
        break;
    case 1:
        i=0,istop=15;
        k=16,kstop=31;
        break;
    case 2:
        i=16,istop=31;
        k=0,kstop=15;
        break;
    default:
        k=i=16,kstop=istop=31;
    }
    w=k;
    for(p=0;i<=istop;i++,p++)
    {
        for(q=0,k=w;k<=kstop;k++,q++)
        {
            if(ems_data[k][i])
                mrc_drawPoint(x+q,y+p,0x0000);
            else
                mrc_drawPoint(x+q,y+p,0xffff);
        }
    }

    if(hmz>3)
        hmz=0;
    else 
        hmz++;
    mrc_refreshScreen(x,y,16,16);
}

VOID ShowPREVIEW(WID wndid)
{
	HideTopWindow(wndid, FALSE, FALSE);
	ShowTopWindow(TOPWND_PREVIEW, NULL, (DWORD)wndid);
}

static VOID HidePREVIEW(HWND hwnd)
{
	HideTopWindow(TOPWND_PREVIEW, FALSE, FALSE);
	ShowTopWindow((WID)_USERDATA(hwnd), NULL, 0);
}

LRESULT PREVIEW_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_CREATE:
	{
        HWND hcon;

        _BGCOLOR(hWnd)=0x326596;

        hcon = SGL_CreateWindow(SMP_Toolbar_WndProc, 0,
            _HEIGHT(hWnd) - SMP_ITEM_HEIGHT, 
            _WIDTH(hWnd), SMP_ITEM_HEIGHT, 
            PREVIEW_TOOLBAR, 0, 0);
        SMP_Toolbar_SetStrings(hcon, STR_EMSPRE, RESID_INVALID, STR_BACK, 0);
        SGL_AddChildWindow(hWnd, hcon);

        hcon = SGL_CreateWindow(SMP_Titlebar_WndProc,
            0, 0, SCREEN_WIDTH, SMP_ITEM_HEIGHT,
            PREVIEW_TITLEBAR, 
            SMP_TITLEBAR_STATIC, 0);
        _FGCOLOR(hcon) =  COLOR_lightwhite ;/* 标题文字颜色 */
        SMP_Titlebar_SetContent(hcon, RESID_INVALID, SGL_LoadString(STR_PRETITLE),FALSE);
        SGL_AddChildWindow(hWnd, hcon);

		return 0;
	}
    case WM_HIDE:
    case WM_DESTROY:
    {
        if(g_timer)
        {
            mrc_timerStop(g_timer);
            mrc_timerDelete(g_timer);
            g_timer^=g_timer;
        }
		return 0;
    }
    case WM_SHOW:
    {
        if(!g_timer)
        {
            g_timer = mrc_timerCreate();
        }
        _SET_WINDATA(hWnd,1);
        break;
    }
    case WM_PAINT:
        {  
            if(_WINDATA(hWnd))
            {
                extern void drawEMS(int x, int y, uint8 emsdata[32][32]);
                mrc_timerStop(g_timer);
                drawEMS((SCREEN_WIDTH>>1) - 16,(SCREEN_HEIGHT>>1) - 16,ems_data);
            }
            else 
            {
                hmz=0;
                mrc_timerStart(g_timer,100,0,yulanANMWin,1);
            }
            return 0;
        }
	case WM_KEYUP:	
        switch(wParam)
        {
        case MR_KEY_SOFTRIGHT://按右键
            HidePREVIEW(hWnd);
            return 1;
        case MR_KEY_SOFTLEFT://切换
            _WINDATA(hWnd)=!_WINDATA(hWnd);
            if(_WINDATA(hWnd))
                SMP_Toolbar_SetStrings(SGL_FindChildWindow(hWnd,PREVIEW_TOOLBAR),STR_EMSPRE,RESID_INVALID,STR_BACK,0);
            else
                SMP_Toolbar_SetStrings(SGL_FindChildWindow(hWnd,PREVIEW_TOOLBAR),STR_ANMPRE,RESID_INVALID,STR_BACK,0);
            SGL_UpdateWindow(hWnd);
            return 1;
        }
		break;
	}    
    return 1;
}

