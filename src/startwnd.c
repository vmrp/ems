#include "startwnd.h"
#include "mainwnd.h"
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
#include "plt_msgbox.h"
#include "smp_progbar.h"
#include "smp_combobox.h"
#include "smp_list.h"
#include "topwnd.h"
#include "string.h"
#include "smp_titlebar.h"
#include "opendlg.h"
#include "smp_advbar.h"
#include "help.h"
#include "convert.h"
//#include "mrc_freecurrency.h"
#include "momo.h"


//本次交易收取您10令牌
#define TEXT_BRIEF "\x67\x2c\x6b\x21\x4e\xa4\x66\x13\x65\x36\x53\xd6\x60\xa8\x0\x31\x0\x30\x4e\xe4\x72\x4c\x0\x0"
//因为开发这个软件的人长得太帅了，不然对不起他逝去的青春啊，你说是吧！？
#define TEXT_DETAIL	"\x56\xe0\x4e\x3a\x5f\x0\x53\xd1\x8f\xd9\x4e\x2a\x8f\x6f\x4e\xf6\x76\x84\x4e\xba\x95\x7f\x5f\x97\x59\x2a\x5e\x5\x4e\x86\xff\xc\x4e\xd\x71\x36\x5b\xf9\x4e\xd\x8d\x77\x4e\xd6\x90\x1d\x53\xbb\x76\x84\x97\x52\x66\x25\x55\x4a\xff\xc\x4f\x60\x8b\xf4\x66\x2f\x54\x27\xff\x1\xff\x1f\x0\x0"

//子窗口ID
enum
{
	START_BUTTON_START = 1,
    START_BUTTON_CONVERT,
    START_BUTTON_HELP,
    START_BUTTON_EXIT,
    START_ADVBAR,
    START_MSG_CHARGE
};

/////////////////////////////////////////////////////////
static void __chargeResultCB(int32 result){
    if (MR_SUCCESS == result) {
        int32 data;

        data=readCfg(CFG_CHARGE)+1;//每次加1
        writeCfg(CFG_CHARGE,data);
        HideTopWindow(TOPWND_START, FALSE, FALSE);
        ShowTopWindow(TOPWND_CONVERT, NULL, 0);
    }
//    else SMP_Advbar_Show();
}
#ifndef WIN32
// static void __prop(int32 data)
// {
//     mrc_freecurrency_prop_pay(1, 10, TEXT_BRIEF, TEXT_DETAIL, __chargeResultCB);
//     mrc_timerDelete(data);
// }
// static void __loadCB(int32 result){
//     if(!_FUNC_IS_SET_ANY(FUNC_CHARGEINIT)) SGL_UnsuspendDrawing();
//     if (MR_SUCCESS == result) {
//         int32 timer;
//         timer=mrc_timerCreate();
//         mrc_timerStart(timer,200,timer,__prop,0);
//         _FUNC_SET_STYLE(FUNC_CHARGEINIT);
//     }
// }
#endif // WIN32
LRESULT START_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{	
	switch(Msg)
	{
	case WM_CREATE:
	{
        HWND hContorl;
        int x,y,w;

        GetMediumFontWH((int32*)&w,NULL,1);
        w=w*6;
        x=(SCREEN_WIDTH-w)/2;
        y=SCREEN_HEIGHT-x-SMP_ITEM_HEIGHT;


        // SMP_Advbar_AddToParent(hWnd,0,0,SCREEN_WIDTH,START_ADVBAR,NULL);
        // SMP_Advbar_SetInfo(9000022,FALSE);

        hContorl = SGL_CreateWindow(SMP_Button_WndProc,//退出
            x, y, w, SMP_ITEM_HEIGHT, 
            START_BUTTON_EXIT, SMP_BUTTONS_HCENTER|SMP_BUTTONS_VCENTER|WS_TABSTOP|WS_BORDER, 0);
        SMP_Button_SetTitle(hContorl,SGL_LoadString(STR_EXIT));
        SGL_AddChildWindow(hWnd,hContorl);

        y=y-SMP_ITEM_HEIGHT-SMP_ITEM_MARGIN*2;
        hContorl = SGL_CreateWindow(SMP_Button_WndProc,//帮助
            x, y, w, SMP_ITEM_HEIGHT, 
            START_BUTTON_HELP, SMP_BUTTONS_HCENTER|SMP_BUTTONS_VCENTER|WS_TABSTOP|WS_BORDER, 0);
        SMP_Button_SetTitle(hContorl,SGL_LoadString(STR_HELP));
        SGL_AddChildWindow(hWnd,hContorl);

        y=y-SMP_ITEM_HEIGHT-SMP_ITEM_MARGIN*2;
        hContorl = SGL_CreateWindow(SMP_Button_WndProc,//转换
            x, y, w, SMP_ITEM_HEIGHT, 
            START_BUTTON_CONVERT, SMP_BUTTONS_HCENTER|SMP_BUTTONS_VCENTER|WS_TABSTOP|WS_BORDER, 0);
        SMP_Button_SetTitle(hContorl,SGL_LoadString(STR_CONVERTIMG));
        SGL_AddChildWindow(hWnd,hContorl);

        y=y-SMP_ITEM_HEIGHT-SMP_ITEM_MARGIN*2;
        hContorl = SGL_CreateWindow(SMP_Button_WndProc,//开始
            x, y, w, SMP_ITEM_HEIGHT, 
            START_BUTTON_START, SMP_BUTTONS_HCENTER|SMP_BUTTONS_VCENTER|WS_TABSTOP|WS_BORDER, 0);
        SMP_Button_SetTitle(hContorl,SGL_LoadString(STR_START));
        SGL_AddChildWindow(hWnd,hContorl);

		break;
	}
    case WM_DESTROY:
    {
#ifndef WIN32
        // if(_FUNC_IS_SET_ANY(FUNC_CHARGEINIT))
        //     mrc_freecurrency_unload();
#endif // WIN32
		break;
    }
	case WM_INITFOCUS:
	{
        SGL_FocusNext(hWnd,TRUE);
		break;
	}
    case WM_SHOW:
    {   
        break;
    }
	case WM_PAINT:
	{   
		break;
	}
	case WM_KEYDOWN:
	case WM_KEYDOWNREPEAT:
	{
        switch(wParam)
        {
        case MR_KEY_UP:
            SGL_FocusNext(hWnd,TRUE);
            return 1;
        case MR_KEY_DOWN:            
            SGL_FocusNext(hWnd,FALSE);
            return 1;
        }

		break;
	}	
    case WM_COMMAND:
        {
            WID id = LOWORD(wParam);
            WORD code = HIWORD(wParam);

            switch(id)
            {
            case START_BUTTON_EXIT:
                if(code==SMP_BUTTONN_CLICKED)
                {
                    mrc_exit();
                }
                break;
            case START_BUTTON_START://直接开始
                HideTopWindow(TOPWND_START, FALSE, FALSE);
                ShowTopWindow(TOPWND_MAINWND, NULL, 0);
                break;
            case START_BUTTON_HELP:
                ShowHELP(TOPWND_START,STR_HELPCONTENT);
                break;
            case START_BUTTON_CONVERT:
                {
                    if(10 == readCfg(CFG_CHARGE))
                    {
                        HideTopWindow(TOPWND_START, FALSE, FALSE);
                        ShowTopWindow(TOPWND_CONVERT, NULL, 0);
                    }
                    else SMP_MsgBox(START_MSG_CHARGE,hWnd,NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_CHARGEMSG),ID_OK|ID_CANCEL,NULL);
                }
                break;
            case START_MSG_CHARGE:
                if(code==ID_OK)
                {
//                    SMP_Advbar_Hide();
#ifndef WIN32
                    // if(!_FUNC_IS_SET_ANY(FUNC_CHARGEINIT))
                    // {
                    //     SGL_SuspendDrawing();
                    //     mrc_freecurrency_init(TRUE, FALSE, __loadCB);
                    // }
                    // else
                    //     __loadCB(MR_SUCCESS);
#else
// __chargeResultCB(MR_SUCCESS );

#endif // WIN32
__chargeResultCB(MR_SUCCESS );
                }
            }
        }
        break;
	}
	return 0;
}

