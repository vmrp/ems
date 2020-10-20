/*
** platform.c: implements of platform.h
** 
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** Create date: 2007-12-27 by wandonglin
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#include "window.h"
#include "i18n.h"
#include "mrc_network.h"
#include "platform.h"
#include "momo.h"
//#include "mrc_advbar.h"


int32 MRC_EXT_INIT(void)
{
	return momo_init();
}

int32 mrc_appEvent(int32 code, int32 param0, int32 param1)
{
	switch(code)
	{
	case MR_MOUSE_DOWN:
		SGL_DispatchEvents(WM_MOUSEDOWN, param0, param1);
		break;
	case MR_MOUSE_UP:
//         advNext();
		SGL_DispatchEvents(WM_MOUSEUP, param0, param1);
		break;
	case MR_KEY_PRESS:
		SGL_DispatchEvents(WM_KEYDOWN, param0, param1);
		break;
	case MR_KEY_RELEASE:
//         advNext();
		SGL_DispatchEvents(WM_KEYUP, param0, param1);
		break;
	case MR_MOUSE_MOVE:
		SGL_DispatchEvents(WM_MOUSEMOVE, param0, param1);
		break;
	default:
		SGL_DispatchEvents(code, param0, param1);
		break;
	}
	return 0;
}

int32 mrc_appPause(void)
{	
//    mrc_advStopDraw();
	SGL_SuspendDrawing();
#ifdef ENABLE_SGL_KEYREPEAT		
	SGL_StopKeyRepeat();
#endif
	//用户应用程序指定的函数
	return PauseApplication();
}

int32 mrc_appResume(void)
{
	HWND hTopWnd = SGL_GetNextChild(HWND_DESKTOP, NULL);
	SGL_UnsuspendDrawing();
	if(hTopWnd)
		SGL_UpdateWindow(hTopWnd);
	//用户应用程序指定的函数
	return ResumeApplication();
}

int32 MRC_EXT_EXIT(void)
{
	//调用用户应用功能
	int r;

	SGL_SuspendDrawing();
	r = ExitApplication();
	
	//终止sky GUI库
	SGL_Terminate();

#if !OMIT_NETWORK_MODULE
	//终止网络模块

	mrc_Socket_Terminate();
	mrc_Http_Terminate();
#endif

	return r;
}

#define _EXT_RELEASE
#ifndef _EXT_RELEASE
void sgl_assert(const char* file, int line, int exp)
{
	if(!exp)
	{
	//	SGL_TRACE("assert failed %s:%d\n", file, line);
		mrc_exit();
	}
}

#endif


#ifdef WIN32
#include "mrc_bmp.h"
int32 mrc_Img2bmp(uint8 *pImg, int32 ImgSize, MRC_IMG_TYPE ImgType, uint16 **bmp, int32 *w, int32 *h)
{
	return MR_FAILED;
}
#endif