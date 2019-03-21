#ifndef _SMP_HSCROLLBAR_H
#define _SMP_HSCROLLBAR_H

#include "window.h"

/* 滑块条

若要用滑块条：
   - 创建滑块条窗口
   - 设置滑块条窗口的信息
   - 添加到父窗口
   - 响应消息的通知

  \code
  	//创建滑块条
  	hHScrbar = SGL_CreateWindow(SMP_HScrollbar_WndProc, ...);
  	SMP_HScrollbar_SetInfo(hHScrbar);
  	SGL_AddChildWindow(hWnd, hHScrbar);

  	//响应消息的通知
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);
		if(id == "the Hscrollbar id" && code == SMP_HSCRBARN_VALUECHANGED)
		{
			int index = (int)lParam; //当前滑块条值
		}	
  \endcode


 *
 * \参数index   滚动条的当前值
 */
#define SMP_HSCRBARN_VALUECHANGED		0x0001


//滚动条的当前值设置到了最后
#define SMP_HSCRBARN_LAST  2

//固定高度
#define SMP_HSCRBARN_HEIGHT 25

/** 
 * \设置滑块条的信息
 *
 * \参数hWnd        窗口句柄
 * \参数min         最低值
 * \参数max         最大值
 * \参数pagesize    响应鼠标事件时每页大小
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_HScrollbar_SetInfo(HWND hWnd, int min, int max, int pagesize);

/**
 * \获得滑块条信息
 *
 * \参数hWnd       窗口句柄
 * \参数min        最低值
 * \参数max        最大值
 * \参数pagesize   每页大小
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_HScrollbar_GetInfo(HWND hWnd, int* min, int* max, int* pagesize);

/**
 * \设置滑块条的步长
 *
 * \参数hWnd    窗口句柄
 * \参数step    单步值
 */
VOID SMP_HScrollbar_SetStep(HWND hWnd, int step);

/**
 * \设置滑块条的新位置
 *
 * \参数hWnd        窗口句柄
 * \参数value       新值
 * \参数redraw      是否重绘窗口
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_HScrollbar_SetValue(HWND hWnd, int value, BOOL redraw);

/**
 * \得到滑块条的当前位置
 *
 * \参数hWnd        窗口句柄
 * \返回当前位置值
 */
int SMP_HScrollbar_GetValue(HWND hWnd);

/**
 * \向右滚动
 *
 * \参数hWnd        窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_HScrollbar_PageLeft(HWND hWnd);

/**
 * \向左滚动
 *
 * \参数hWnd        窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_HScrollbar_PageRight(HWND hWnd);

/**
 * \按步长向右滚动
 *
 * \参数hWnd     窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_Scrollbar_LineRight(HWND hWnd);

/**
 * \按步长向左滚动
 *
 * \参数hWnd     窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_Scrollbar_LineLeft(HWND hWnd);

/** 
窗口程序

 * \滑块条的窗口过程
 *
 * \参数hWnd        窗口句柄
 * \参数Msg         窗口消息
 * \参数wParam      第一个参数
 * \参数lParam      第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_HScrollbar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif /* _SMP_HSCROLLBAR_H */

