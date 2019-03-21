#include "gal.h"
#include "smp.h"
#include "window.h"
#include "bmp.h"


VOID SMP_DrawCheckBox(int x, int y, BOOL checked)
{
	GAL_Rectangle(x, y, SMP_CHECKBOX_SIZE, SMP_CHECKBOX_SIZE, COLOR_border);	

	if(checked)
	{
		x += DIV(SMP_CHECKBOX_SIZE-7, 2);
		GAL_Line(x, y + DIV(SMP_CHECKBOX_SIZE, 2), x + 3, y + DIV(SMP_CHECKBOX_SIZE*2, 3), COLOR_focus);
		GAL_Line(x + 3, y + DIV(SMP_CHECKBOX_SIZE*2, 3), x + 7, y + DIV(SMP_CHECKBOX_SIZE, 3), COLOR_focus);
	}
}

VOID SMP_DrawMsgBoxFrame(int x, int y, int w, int h)
{

    DrawShadeRect(x,y,w,SMP_ITEM_HEIGHT,0xD52828,0x9B1111,SHADE_UPDOWN);

    y+=SMP_ITEM_HEIGHT;
    h-=SMP_ITEM_HEIGHT;
    GAL_Rectangle(x,y,w,h,0x9B1111);
    GAL_Rectangle(x+1,y+1,w-2,h-2,0xB55959);
    GAL_Rectangle(x+2,y+2,w-4,h-4,0x9B1111);

}

/*
VOID SMP_DrawWndHeader(int x, int y, int w, int h, Uint32 bgcolor, Uint32 fgcolor, DWORD bmpID, PCWSTR str)
{
	int wBmp = 0, hBmp = 0;
	int32 wText, hText;
	mr_screenRectSt rect;
	mr_colourSt color={0,};
	HBITMAP bmp;

	//绘制背景
	GAL_FillBox(PHYSICALGC, x, y, w, h, bgcolor);

	//绘制位图
	wBmp = 0;
	if(bmpID < RES_BITMAP_COUNT)
	{
		bmp = SGL_LoadBitmap(bmpID, &wBmp, &hBmp);
		mrc_bitmapShowEx(bmp, SMP_ITEM_CONTENT_MARGIN, (int16)DIV(h - hBmp, 2), (int16)wBmp, (int16)wBmp, (int16)hBmp, BM_TRANSPARENT, 0, 0);
		wBmp += SMP_ITEM_CONTENT_MARGIN;
	}

	//绘制标题
	if(str)
	{
		HFONT font = SGL_GetSystemFont();
		rect.x = (int16)(SMP_ITEM_CONTENT_MARGIN + wBmp);
		mrc_textWidthHeight((PSTR)str, TRUE, (uint16)font, &wText, &hText);
		rect.y = (int16)DIV(h - hText, 2); rect.h = h; rect.w = (uint16)(SCREEN_WIDTH - rect.x - SMP_ITEM_CONTENT_MARGIN);

		color.r = PIXEL888RED(fgcolor);
		color.g = PIXEL888GREEN(fgcolor);
		color.b = PIXEL888BLUE(fgcolor);
		mrc_drawTextEx((PSTR)str, rect.x, rect.y, rect, color, 1, (uint16)font);
	}
}
*/