#ifndef _SMP_ADVBAR_H
#define _SMP_ADVBAR_H

#include "window.h"
//#include "mrc_advbar.h"




/*
�������mrc_appPause�����mrc_advStopDraw();

    ����Ҫʹ�ù��ؼ��Ĵ���WM_CREATE��WM_SHOW��֪ͨ���������´���
    SMP_Advbar_AddToParent(...);
    SMP_Advbar_SetInfo(...);

���֪ͨ��Ӧ
typedef enum {
    NOTIFY_TYPE_DOWNLOAD,
    NOTIFY_TYPE_BROWSER
}NOTIFY_TYPE;

 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the advbar id" && code == NOTIFY_TYPE)
 *		{
 *			HWND hAdvBar = (HWND)lParam;
 * 			//������֪ͨ��Ϣ
 *		}
 * \endcode


������ܻ���Ҫ�ڸ����������ӣ�
    case WM_MENUSHOW:
    case WM_MODALSHOW:
        mrc_advStopDraw();//���û����䣬�˵���ʾʱ�����²˵�������

*/

extern HWND hAdvBar;

#define  SMP_Advbar_Hide() \
    mrc_advStopDraw()

#define  SMP_Advbar_Show() \
    mrc_advStartDraw()


HWND SMP_Advbar_AddToParent(HWND hParent, int x, int y, int w, WID id, HWND hListener);

void SMP_Advbar_SetInfo(uint32 advChunkID, int8 IsSmallFont);


#endif
