#include "smp.h"
#include "smp_hscrollbar.h"


#define HSCRBAR_MIN_DEFAULT 	    0
#define HSCRBAR_MAX_DEFAULT	        99
#define HSCRBAR_STEP_DEFAULT	    1
#define HSCRBAR_PAGE_DEFAULT	(HSCRBAR_MAX_DEFAULT - HSCRBAR_MIN_DEFAULT + 1) / 10

#define HSCRBAR_SMP_HEADER_HEIGHT	12
#define CURSOR_WIDTH		5

//内部使用
#define GET_HSCRBAR_DATA(hWnd) \
	_GET_WINDATA(hWnd, PHSBDATA)




///////////////////////////////////////////////////////////////////////////////////////////////

typedef struct SMP_HScrBarData
{
	int min;     //最小值
	int max;     //最大值
	int cur;     //当前位置
	int page;    //页规格
	int step;    //行规格
}HSBDATA, *PHSBDATA;

/////////////////////////////////////////////////////////////////////////////////////////////

//函数获取光标信息
static VOID SMP_HScrollbar_GetCursorInfo(PHSBDATA pHScrbar, int w, int* cursor_x)
{
	//计算光标位置
	int maxSteps = pHScrbar->max - pHScrbar->min + 1 - pHScrbar->page;

	if(maxSteps < 1) maxSteps = 1; 	
	*cursor_x = (w* (pHScrbar->cur - pHScrbar->min)) / maxSteps;

	//当高度在菜单的最后一项，设置光标移动到滚动条的底部
	if(pHScrbar->cur == pHScrbar->max) 
		*cursor_x = w;
}


BOOL SMP_HScrollbar_SetInfo(HWND hWnd, int min, int max, int pagesize)
{
	PHSBDATA pHScrbar;

	if(!IS_NORMAL_WINDOW(hWnd) 
		|| pagesize < 0 || min > max)
		return FALSE;

	pHScrbar = GET_HSCRBAR_DATA(hWnd);
	pHScrbar->min = min;
	pHScrbar->max = max;
	pHScrbar->page = pagesize;
	return TRUE;
}


BOOL SMP_HScrollbar_GetInfo(HWND hWnd, int* min, int* max, int* pagesize)
{
	PHSBDATA pHScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pHScrbar = GET_HSCRBAR_DATA(hWnd);
	if(min) *min = pHScrbar->min;
	if(max) *max = pHScrbar->max;
	if(pagesize) *pagesize = pHScrbar->page;
	return TRUE;
}


VOID SMP_HScrollbar_SetStep(HWND hWnd, int step)
{
	PHSBDATA pHScrbar = GET_HSCRBAR_DATA(hWnd);
	pHScrbar->step = step;
}


BOOL SMP_HScrollbar_SetValue(HWND hWnd, int index, BOOL redraw)
{
	PHSBDATA pHScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pHScrbar = GET_HSCRBAR_DATA(hWnd);
	if(index == pHScrbar->cur || index < pHScrbar->min || index > MAX(pHScrbar->max - pHScrbar->page + 1, 0))
		return FALSE;

	pHScrbar->cur = index;
	if(redraw)
		SGL_UpdateWindow(hWnd);
	return TRUE;
}


int SMP_HScrollbar_GetValue(HWND hWnd)
{
	PHSBDATA pHScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return 0;
	
	pHScrbar = GET_HSCRBAR_DATA(hWnd);
	return pHScrbar->cur;
}


BOOL SMP_HScrollbar_PageLeft(HWND hWnd)
{
	PHSBDATA pHScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pHScrbar = GET_HSCRBAR_DATA(hWnd);
	if(pHScrbar->cur > pHScrbar->min)
    {
		if(SMP_HScrollbar_SetValue(hWnd, MAX(pHScrbar->cur - pHScrbar->page, pHScrbar->min), TRUE))
            return SMP_HSCRBARN_LAST;
        else
	        return FALSE;
    }
	return TRUE;
}

BOOL SMP_HScrollbar_PageRight(HWND hWnd)
{
	PHSBDATA pHScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pHScrbar = GET_HSCRBAR_DATA(hWnd);
	if(pHScrbar->cur < pHScrbar->max - pHScrbar->page + 1)
    {
		if(SMP_HScrollbar_SetValue(hWnd, MIN( pHScrbar->cur + pHScrbar->page, pHScrbar->max - pHScrbar->page + 1), TRUE))
            return SMP_HSCRBARN_LAST;
        else
	        return FALSE;
    }
	return TRUE;
}


BOOL SMP_HScrollbar_LineLeft(HWND hWnd)
{
	PHSBDATA pHScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pHScrbar = GET_HSCRBAR_DATA(hWnd);
	if(pHScrbar->cur > pHScrbar->min)
    {
        if(SMP_HScrollbar_SetValue(hWnd, MAX(pHScrbar->cur - pHScrbar->step, pHScrbar->min), TRUE))
            return SMP_HSCRBARN_LAST;
        else
            return FALSE;
    }
	return TRUE;
}


BOOL SMP_HScrollbar_LineRight(HWND hWnd)
{
	PHSBDATA pHScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pHScrbar = GET_HSCRBAR_DATA(hWnd);
	if(pHScrbar->cur < pHScrbar->max - pHScrbar->page + 1)
    {
        if(SMP_HScrollbar_SetValue(hWnd, MIN(pHScrbar->cur + pHScrbar->step, pHScrbar->max - pHScrbar->page + 1), TRUE))
            return SMP_HSCRBARN_LAST;
        else
            return FALSE;
    }
	return TRUE;
}


LRESULT SMP_HScrollbar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    PHSBDATA pHScrbar = GET_HSCRBAR_DATA(hWnd);

	switch(Msg)
	{
	case WM_CREATE:
	{
		pHScrbar = SGL_MALLOC(sizeof(HSBDATA));
		if(!pHScrbar)
			return 1;		
		SGL_MEMSET(pHScrbar, 0, sizeof(HSBDATA));
		pHScrbar->cur = pHScrbar->min = HSCRBAR_MIN_DEFAULT;
		pHScrbar->max = HSCRBAR_MAX_DEFAULT;
		pHScrbar->page = HSCRBAR_PAGE_DEFAULT;
		pHScrbar->step = HSCRBAR_STEP_DEFAULT; // 默认步骤

		_SET_WINDATA(hWnd, pHScrbar);
		_BGCOLOR(hWnd) = 0;
		_FGCOLOR(hWnd) = COLOR_controlhili;
		return 0;
	}

	case WM_DESTROY:
	{
		if(pHScrbar) SGL_FREE(pHScrbar);
		return 0;
	}

	case WM_PAINT:
	{
		int x=0, y=0, cursor_x;
        int j,k;

		SGL_WindowToScreen(hWnd, &x, &y);
        SMP_HScrollbar_GetCursorInfo(pHScrbar, _WIDTH(hWnd)-CURSOR_WIDTH-SMP_ITEM_SPACE*2, &cursor_x);


        //背景
        mrc_drawRect(x,y,_WIDTH(hWnd), _HEIGHT(hWnd),225,225,225);
        if(_IS_SET_ANY(hWnd, WS_FOCUSED))//焦点状态
            GAL_Rectangle(x, y, _WIDTH(hWnd), _HEIGHT(hWnd), COLOR_focus);

		//画光标
        x+=SMP_ITEM_SPACE;
        k=x+_WIDTH(hWnd)-5;

        j=y+11;
        mrc_drawLine(x,j,k,j,176,176,176);//上线 0xB0B0B0        
        j=j+1;
        mrc_drawLine(x,j,k,j,231,234,234);//中线 0xE7EAEA      
        j=j+1;
        mrc_drawLine(x,j,k,j,252,252,252);//下线 0xFCFCFC

        x+=cursor_x;
        mrc_drawRect(x,y+5,CURSOR_WIDTH,15,17,17,17);//滑块
		return 0;
	}

/*
    case WM_MOUSEDOWN:
    {
        int cursor_x;

        SMP_HScrollbar_GetCursorInfo(pHScrbar, _WIDTH(hWnd)-CURSOR_WIDTH-SMP_ITEM_SPACE, &cursor_x);
        if((int)wParam < cursor_x)
            SMP_HScrollbar_PageLeft(hWnd);
        else if((int)wParam > cursor_x + CURSOR_WIDTH)
            SMP_HScrollbar_PageRight(hWnd);
        return 0;
    }

	case WM_KEYDOWN:
	case WM_KEYDOWNREPEAT:
	{
		if(wParam == MR_KEY_LEFT)
		{
            SMP_HScrollbar_PageLeft(hWnd);
            SGL_NotifyParent(hWnd, SMP_HSCRBARN_VALUECHANGED, pHScrbar->cur);
			return 1;
		}
        else if(wParam == MR_KEY_RIGHT)
        {
            SMP_HScrollbar_PageRight(hWnd);
            SGL_NotifyParent(hWnd, SMP_HSCRBARN_VALUECHANGED, pHScrbar->cur);
			return 1;
		}
		break;
	}
    case WM_KILLFOCUS:
    case WM_SETFOCUS:
    {
        SGL_UpdateWindow(hWnd);
        return 0;
    }
*/

	}
	return 0;
}


