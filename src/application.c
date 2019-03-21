#include "application.h"
#include "topwnd.h"
#include "mrc_base.h"
#include "momo.h"
#include "mrc_base_i.h"
#include "mainwnd.h"
#include "smp.h"
#include "mrc_skyfont.h"
#include "basic.h"

void drawEMS(int x, int y, uint8 emsdata[32][32])
{
    int8 i,k;

    for(i=0;i<=31;i++)//画预览ems图片
    {
        for(k=0;k<=31;k++)
        {
            if(emsdata[k][i])
                mrc_drawPoint(x+k,y+i,0x0000);
            else
                mrc_drawPoint(x+k,y+i,0xffff);
        }
    }
}
int drawSmsImg(char *FiName, int x, int y)
{
    int32 handle;
    uint16 color;
    uint32 w,h,b=0;
    uint8 n,i,a=0,p,aa=0,bb=0,cc,dd;
    uint8 data[140];

    h=mrc_getLen(FiName);
    handle = mrc_open(FiName,MR_FILE_RDONLY);
    if(!handle) return -1;
    if(137==h)//是EMS格式
    {
        mrc_read(handle,data,137);
        i=9;
        for(h=0;h<=31;h++)//数据转换成二进制
        {
            for(a=1;a<=4;a++)
            {
                w=a*8-1;
                n=data[i++];	
                for(b=1;b<=8;b++)
                {
                    if(n%2)
                        color=0x0000;
                    else
                        color=0xffff;
                    mrc_drawPoint((int16)(x+w),(int16)(y+h),color);
                    n=n>>1;
                    w--;
                }
            }
        }

    }
    else if(140==h)//是ANM格式
    {
        mrc_read(handle,data,140);
        i=12;

        for(p=0;p<4;p++)
        {
            switch(p)
            {
            case 0:
                aa=0,a=15;
                bb=1,b=2;
                break;
            case 1:
                aa=0,a=15;
                bb=3,b=4;
                break;
            case 2:
                aa=16,a=31;
                bb=1,b=2;
                break;
            case 3:
                aa=16,a=31;
                bb=3,b=4;
            }
            for(h=aa;h<=a;h++)//数据转换成二进制
            {
                for(cc=bb;cc<=b;cc++)
                {
                    w=cc*8-1;
                    n=data[i++];	
                    for(dd=0;dd<=7;dd++)
                    {
                        if(n%2)
                            color=0x0000;
                        else
                            color=0xffff;
                        mrc_drawPoint((int16)(x+w),(int16)(y+h),color);
                        n=n>>1;
                        w--;
                    }
                }
            }
        }
    }
    else
    {
        mrc_close(handle);
        return -1;
    }
    mrc_close(handle);
    return 0;
}
int InitApplication(VOID)
{	    
//     mrc_clearScreen(0,0,0);
// 
//     mrc_refreshScreen(0,0,240,320);
//     return 0;
#ifndef WIN32
#if 1
    BMPScreen("logo.bmp",137,136,BM_COPY,0x0000);
    mrc_sleep(2000);
#endif
#endif 
#if 1
    if(TRUE != readCfg(CFG_FIRSTRUN))
    {
        uint8 *buf=NULL;
        int32 len;

        writeCfg(CFG_FIRSTRUN,TRUE);
        if(mrc_readFileFromMrpEx(NULL,"bat",&buf,&len,0))
        {
            mrc_exit();
            return -1;
        }
        StartBasic((char*)buf);
        mrc_free(buf);
    }
#endif
    InitTopWindow();
    ShowTopWindow(TOPWND_START, 0, 0);
//     ShowTopWindow(TOPWND_MAINWND, 0, 0);
	return 0;
}


int ExitApplication(VOID)
{
    mrc_clearScreen(0,0,0);
    TextMiddle("正在退出",0,1);
    if(_FUNC_IS_SET_ANY(FUNC_INITSUC)) //是否成功初始化,如果否,则不能释放资源
    {
        if(!_FUNC_IS_SET_ANY(FUNC_EXIT))//非正常退出
            SaveEms((char*)TEMPEMS);
        HideTopWindow(TOPWND_MAINWND,TRUE,FALSE);
        HideTopWindow(TOPWND_OPENDLG,TRUE,FALSE);
        HideTopWindow(TOPWND_START,TRUE,FALSE);
        momo_exit();
    }
	return 0;
}
int PauseApplication(VOID)
{
	return 0;
}


int ResumeApplication(VOID)
{
	return 0;
}

