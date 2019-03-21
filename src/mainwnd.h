#ifndef _MAINWND_H
#define _MAINWND_H

#include "window.h"



/*
图像颜色用二进制位来描述，1描述黑色，0描述白色。

ANM单色动画格式（以16*16像素为例）：
0401 0004 0000 //文件头（可能还包括有其它信息）
1000 //图像宽
1000 //图像高
8000 //图像数据总大小
下面是图像数据
12-44 //第一帧图像
44-76 //第二帧图像
76-108 //第三帧图像
108-140 //第四帧图像
=============================================
EMS单色图片格式（以32*32像素为例）：
0201 00 //文件头
2000 //图像宽
2000 //图像高
8000 //图像数据总大小
下面是图像数据
9-137 //图像数据
*/

extern const char TEMPEMS[];
extern const char IMAGES[];
extern const char ANMDIR[];
extern const char EMSDIR[];


#define DrawEmsEditPointEx(wx, wy, color) \
    do{\
        drawColor=color?0:255;\
        mrc_drawRect((int16)(9+(wx)*g_wgsize),(int16)(9+(wy)*g_wgsize),fangkuaiaa,fangkuaiaa,drawColor,drawColor,drawColor);\
    }while(0)

#define DrawEmsEditPoint(x,y)	DrawEmsEditPointEx((x),(y),ems_data[(x)][(y)]);




int32 SaveAnm(char *FiName);
int32 SaveEms(char *FiName);





extern uint8 ems_data[32][32]; 






LRESULT MAINWND_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif
