#include "convert.h"
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


#define CONVERTISR  0//红掩码转换
#define CONVERTISG  1
#define CONVERTISB  2
#define CONVERT_HEIGHT 36//转换显示框大小
#define DECODEPLAT  0//图片解码格式
#define DECODESKY   1
#define DECODEBMP   2
enum {
    CONVERT_TOOLBAR=1,
    CONVERT_TITLEBAR,
    CONVERT_OPTMENU
};
static const DWORD miOptions[] = 
{
    STR_OPENFILE,    
    STR_CONVERTIMGDECODE,   
    STR_COMPLETE	   
};
static const DWORD decOptions[] = 
{
    STR_PLATDEC,//plat解码
    STR_SKYDEC,//平台解码
    STR_STR1//直接绘制
};
struct {
    int RY,GY,BY;
    int leftx,rightx;
    int w,h,clienty,clienth;
}ctrlData;

uint16 imgdata[32][32];
uint8 convertEms[32][32];
static HWND hToolbar;
char decodeMode;//图片解码模式
uint8 r,g,b,convertMode,isRepeat;//转换模式1为R,2为G,3为B
int32 mouseTimer;
/////////////////////////////////////////////////////////


static void convert(void)
{
    uint8 x,y;
    uint8 icolor;

    for(y=0; y<=31; y++)
    {
        for(x=0; x<=31; x++)
        {
            switch(convertMode)
            {
            case CONVERTISR: icolor=PIXEL565RED(imgdata[x][y]);
                convertEms[y][x]=icolor<r; break;                
            case CONVERTISG: icolor=PIXEL565GREEN(imgdata[x][y]); 
                convertEms[y][x]=icolor<g; break;                
            case CONVERTISB: icolor=PIXEL565BLUE(imgdata[x][y]); 
                convertEms[y][x]=icolor<b; break;               
            }
        }
    }
}
static void drawCtrl(void)
{
    int x,w;
    uint32 rcolor,gcolor,bcolor;
    
    rcolor=gcolor=bcolor=COLOR_lightwhite;
    x=ctrlData.leftx+ctrlData.w;
    w=SCREEN_WIDTH-SMP_ITEM_MARGIN*2-ctrlData.w*2;
    mrc_drawRect(x, ctrlData.RY, w, ctrlData.h, 200,200,200);//R控制器背景
    mrc_drawRect(x, ctrlData.GY, w, ctrlData.h, 200,200,200);//R控制器背景
    mrc_drawRect(x, ctrlData.BY, w, ctrlData.h, 200,200,200);//R控制器背景

    mrc_drawRect(x, ctrlData.RY+2, w*r/255, ctrlData.h-4, 200,2,2);//R控制器前景
    mrc_drawRect(x, ctrlData.GY+2, w*g/255, ctrlData.h-4, 2,200,2);//R控制器前景
    mrc_drawRect(x, ctrlData.BY+2, w*b/255, ctrlData.h-4, 2,2,200);//R控制器前景
    
    switch(convertMode)
    {
    case CONVERTISR:
        rcolor=COLOR_focus;
        break;
    case CONVERTISG:
        gcolor=COLOR_focus;
        break;
    case CONVERTISB:
        bcolor=COLOR_focus;
        break;
    }
    GAL_Rectangle(2,ctrlData.RY-2,SCREEN_WIDTH-4,ctrlData.h+4,rcolor);
    GAL_Rectangle(2,ctrlData.GY-2,SCREEN_WIDTH-4,ctrlData.h+4,gcolor);
    GAL_Rectangle(2,ctrlData.BY-2,SCREEN_WIDTH-4,ctrlData.h+4,bcolor);
}
static void drawCtrlLEFT(int y)
{
    int a,b;
    a=y+ctrlData.h/2;
    b=ctrlData.leftx+ctrlData.w;

    GAL_Rectangle(ctrlData.leftx,y,ctrlData.w,ctrlData.h,0);
    mrc_drawLine(ctrlData.leftx,a,b,y,0,0,0);
    mrc_drawLine(ctrlData.leftx,a,b,y+ctrlData.h-1,0,0,0);
}
static void drawCtrlRIGHT(int y)
{
    int a,b;
    a=y+ctrlData.h/2;
    b=ctrlData.rightx+ctrlData.w-2;

    GAL_Rectangle(ctrlData.rightx,y,ctrlData.w,ctrlData.h,0);
    mrc_drawLine(ctrlData.rightx,y,b,a,0,0,0);
    mrc_drawLine(ctrlData.rightx,y+ctrlData.h-1,b,a,0,0,0);
}
static void drawUI(int is_onlyRefems)
{
    extern void drawEMS(int x, int y, uint8 emsdata[32][32]);

    if(FALSE==is_onlyRefems){
        drawCtrlLEFT(ctrlData.RY);
        drawCtrlLEFT(ctrlData.GY);
        drawCtrlLEFT(ctrlData.BY);
        drawCtrlRIGHT(ctrlData.RY);
        drawCtrlRIGHT(ctrlData.GY);
        drawCtrlRIGHT(ctrlData.BY);

        DrawShadeRect(SMP_ITEM_MARGIN,SMP_ITEM_HEIGHT+SMP_ITEM_MARGIN,SCREEN_WIDTH-SMP_ITEM_MARGIN*2,CONVERT_HEIGHT,0x3A9FFF,0x0762B4,SHADE_UPDOWN);
//         GAL_FillBox(SMP_ITEM_MARGIN+1,SMP_ITEM_HEIGHT+SMP_ITEM_MARGIN+1,34,34,0xffffff);
        GAL_Rectangle(SMP_ITEM_MARGIN+1,SMP_ITEM_HEIGHT+SMP_ITEM_MARGIN+1,34,34,0xff0202);//图片框
        mrc_drawText("图片(32*32)",SMP_ITEM_MARGIN*2+CONVERT_HEIGHT,(uint16)(SMP_ITEM_HEIGHT+SMP_ITEM_MARGIN+DIV(CONVERT_HEIGHT-ctrlData.clienth,2)),255,255,255,FALSE,MR_FONT_MEDIUM);
        mrc_bitmapShowEx((uint16*)imgdata,SMP_ITEM_MARGIN+2,SMP_ITEM_HEIGHT+SMP_ITEM_MARGIN+2,32,32,32,BM_COPY,0,0);

        DrawShadeRect(SMP_ITEM_MARGIN,SMP_ITEM_HEIGHT+SMP_ITEM_MARGIN+CONVERT_HEIGHT,SCREEN_WIDTH-SMP_ITEM_MARGIN*2,CONVERT_HEIGHT,0x3A9FFF,0x0762B4,SHADE_UPDOWN);
//         GAL_FillBox(SMP_ITEM_MARGIN+1,SMP_ITEM_HEIGHT+SMP_ITEM_MARGIN+CONVERT_HEIGHT+1,34,34,0xffffff);
        GAL_Rectangle(SMP_ITEM_MARGIN+1,SMP_ITEM_HEIGHT+SMP_ITEM_MARGIN+CONVERT_HEIGHT+1,34,34,0xff0202);//ems框
        mrc_drawText("转换后ems",SMP_ITEM_MARGIN*2+CONVERT_HEIGHT,(uint16)(SMP_ITEM_HEIGHT+CONVERT_HEIGHT+SMP_ITEM_MARGIN+DIV(CONVERT_HEIGHT-ctrlData.clienth,2)),255,255,255,FALSE,MR_FONT_MEDIUM);
        
    }
    convert();
    drawCtrl();
    drawEMS(SMP_ITEM_MARGIN+2,SMP_ITEM_HEIGHT+SMP_ITEM_MARGIN+CONVERT_HEIGHT+2,convertEms);
}
static void showEvent(void)//将图片戴图为32*32
{
    uint16 *scrbuf,x,y;

    scrbuf=w_getScreenBuffer();
    if(mrc_fileState(SMP_DIALOG_PATH) != MR_IS_FILE) return;
    mrc_clearScreen(0,0,0);
    switch(decodeMode)
    {
    case DECODEPLAT:
        DrawIMG(SMP_DIALOG_PATH,0,0);
        break;
    case DECODESKY:
        {
            T_DRAW_DIRECT_REQ input;
            int32 len,f;
            char *buf;

            len=mrc_getLen(SMP_DIALOG_PATH);
            if(len>20*1024) len=20*1024;//最大20K
            f=mrc_open(SMP_DIALOG_PATH,MR_FILE_RDONLY);
            if(!f) return;
            buf=mrc_malloc(len);
            if(!buf){mrc_close(f);return;}
            mrc_read(f,buf,len);
            mrc_close(f);
            input.src_type = SRC_STREAM;
            input.ox = 0;
            input.oy = 0;
            input.w = 32;
            input.h = 32;
            input.src=buf;
            input.src_len=len;
            mrc_drawJpegToFrameBuffer(&input);
            mrc_free(buf);
        }
        break;
    case DECODEBMP:
        {
            int32 f;
            char *buf;

            f=mrc_open(SMP_DIALOG_PATH,MR_FILE_RDONLY);
            if(!f) return;
            buf=mrc_malloc(32*32*2);
            if(!buf){mrc_close(f);return;}
            mrc_read(f,buf,32*32*2);
            mrc_close(f);
            mrc_bitmapShowEx((uint16*)buf,0,0,32,32,32,BM_COPY,0,0);
            mrc_free(buf);
        }
        break;
    }
    for(y=0; y<=31; y++)
    {
        for(x=0; x<=31; x++)
        {
            imgdata[y][x]=(uint16)*(scrbuf+x+y*SCREEN_WIDTH);
        }
    }
}
static void MouseRepeatCallback(int32 data)
{
    CONVERT_WndProc(0, WM_KEYDOWNREPEAT, data, 0);//因为不涉及窗口操作所以HWND不需要
}

static void MouseRepeatStartCallback(int32 data)
{
    mrc_timerStart(mouseTimer, SGL_KEYREPEAT_INTERVAL, data, MouseRepeatCallback, TRUE);
}

static void MouseRepeat(int32 data)
{
    CONVERT_WndProc(0, WM_KEYDOWNREPEAT, data, 0);
    if(!mouseTimer)
        mouseTimer = mrc_timerCreate();
    mrc_timerStart(mouseTimer, SGL_KEYREPEAT_START_INTERVAL, data, MouseRepeatStartCallback, FALSE);
    isRepeat = TRUE;
}
LRESULT CONVERT_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
    case WM_CREATE:
        {
            int margin;

            ctrlData.clienty = SMP_ITEM_HEIGHT+SMP_ITEM_MARGIN*2+CONVERT_HEIGHT*2;
            ctrlData.clienth = SCREEN_HEIGHT-(SMP_ITEM_HEIGHT*2+SMP_ITEM_MARGIN*3+CONVERT_HEIGHT*2);
            ctrlData.leftx=SMP_ITEM_MARGIN;//控制器左边
            ctrlData.h=(ctrlData.clienth/2)/3+2;//控制器高度
            ctrlData.w=ctrlData.h-5;//控制器宽度

            margin = (ctrlData.clienth-ctrlData.h*3)/4;//控制器间隔
            ctrlData.RY=ctrlData.clienty+margin;
            ctrlData.GY=ctrlData.clienty+margin*2+ctrlData.h;
            ctrlData.BY=ctrlData.clienty+margin*3+ctrlData.h*2;

            ctrlData.rightx=SCREEN_WIDTH-SMP_ITEM_MARGIN-ctrlData.w;

            GetMediumFontWH(NULL,(int32*)&ctrlData.clienth,MR_FONT_MEDIUM);
#ifdef WIN32
            decodeMode=DECODEBMP;
#else
            decodeMode=readCfg(CFG_IMGDECODEMODE);
#endif // WIN32

            hToolbar = SGL_CreateWindow(SMP_Titlebar_WndProc,
                0, 0, SCREEN_WIDTH, SMP_ITEM_HEIGHT,
                CONVERT_TITLEBAR, 
                SMP_TITLEBAR_STATIC, 0);
            _FGCOLOR(hToolbar) =  COLOR_lightwhite ;/* 标题文字颜色 */
            SMP_Titlebar_SetContent(hToolbar, RESID_INVALID, SGL_LoadString(STR_CONVERTIMG),FALSE);
            SGL_AddChildWindow(hWnd, hToolbar);

            hToolbar = SGL_CreateWindow(SMP_Toolbar_WndProc, 0,
                _HEIGHT(hWnd) - SMP_ITEM_HEIGHT, 
                _WIDTH(hWnd), SMP_ITEM_HEIGHT, 
                CONVERT_TOOLBAR, 0, 0);
            SMP_Toolbar_SetStrings(hToolbar, STR_MENU, RESID_INVALID, STR_BACK, 0);
            SGL_AddChildWindow(hWnd, hToolbar);
        }
        return 0;
    case WM_DESTROY:
        {
            if(mouseTimer) mrc_timerDelete(mouseTimer);
        }
        return 0;
    case WM_SHOW:
        {
            showEvent();//hWnd,Msg,wParam,lParam
            return 0;
        }
    case WM_PAINT:
        {  
            drawUI(FALSE);
            return 0;
        }
    case WM_MOUSEDOWN:
        {
            if((int)lParam>ctrlData.BY) convertMode=CONVERTISB;
            else if((int)lParam>ctrlData.GY) convertMode=CONVERTISG;
            else if((int)lParam>ctrlData.RY) convertMode=CONVERTISR;

            if(((int)wParam > ctrlData.leftx) && ((int)wParam<ctrlData.leftx+ctrlData.w)) 
            {
                MouseRepeat(MR_KEY_LEFT);
                return 0;
            }
            else if(((int)wParam>ctrlData.rightx) && ((int)wParam<ctrlData.rightx+ctrlData.w)) 
            {
                MouseRepeat(MR_KEY_RIGHT);
                return 0;
            }
            goto next0;
        }
        return 0;
    case WM_MOUSEUP:
        {
            if(isRepeat)
            {
                mrc_timerStop(mouseTimer);
                isRepeat = FALSE;
            }
        }
        return 0;
    case WM_KEYDOWN:
    case WM_KEYDOWNREPEAT:
        {
            uint8 *ptr=&r;
            switch(convertMode)
            {
            case CONVERTISG: ptr=&g; break;                
            case CONVERTISB: ptr=&b; break;               
            }
            switch(wParam)
            {
            case MR_KEY_UP:
                if(convertMode>CONVERTISR) convertMode--;
                else convertMode=CONVERTISB;
                goto next0;
            case MR_KEY_DOWN:
                if(convertMode<CONVERTISB) convertMode++;
                else convertMode=CONVERTISR;
                goto next0;
            case MR_KEY_LEFT:
                (*ptr)--;
                goto next0;
            case MR_KEY_RIGHT:
                (*ptr)++;
                goto next0;
            }
goto next1;
            next0:
            drawUI(TRUE);
            mrc_refreshScreen(0,SMP_ITEM_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT-SMP_ITEM_HEIGHT*2);
            return 0;
        }
next1:
        break;
    case WM_KEYUP:	
        switch(wParam)
        {
        case MR_KEY_SOFTRIGHT://按右键
            HideTopWindow(TOPWND_CONVERT, FALSE, FALSE);
            ShowTopWindow(TOPWND_START, NULL, 0);
            return 1;
        case MR_KEY_SOFTLEFT://
            {
                int index=3;
                SMP_Menu_ClearMenuItems();
                SMP_Menu_SetMenuItem2(0, miOptions, TABLESIZE(miOptions));
                SMP_Menu_SetMenuItem2(TABLESIZE(miOptions), decOptions, TABLESIZE(decOptions));

                switch(decodeMode)
                {
                case DECODESKY:
                    index=4;
                    break;
                case DECODEBMP:
                    index=5;
                    break;
                }
                SMP_Menu_CheckMenuItem(index,TRUE);
                SMP_Menu_SetSubMenu(1,TABLESIZE(miOptions));
                SMP_Menu_Popup(CONVERT_OPTMENU, SMP_MENUS_BOTTOMLEFT, hWnd, 0, _HEIGHT(hWnd) - SMP_ITEM_HEIGHT, NULL);
            }
            return 1;
        }
        break;
    case WM_COMMAND:
        {
            WID id = LOWORD(wParam);
            WORD code = HIWORD(wParam);

            switch(id)
            {
            case CONVERT_OPTMENU:
                switch(code)
                {
                case STR_OPENFILE:
                    ShowOpenDlg(STR_OPEN,TOPWND_CONVERT, DIALOG_SELECTFILE);
                    break;
                case STR_PLATDEC://plat解码
                    decodeMode=DECODEPLAT;//0
                    goto label1;
                case STR_SKYDEC://平台解码
                    decodeMode=DECODESKY;//1
                    goto label1;
                case STR_STR1://直接绘制
                    decodeMode=DECODEBMP;//2
                label1:
                    writeCfg(CFG_IMGDECODEMODE,decodeMode);
                    showEvent();
                    SGL_UpdateWindow(hWnd);
                    break;
                case STR_COMPLETE://完成
                    mrc_memcpy(ems_data,convertEms,1024);
                    _FUNC_SET_STYLE(FUNC_INITSUC);//防止出现打开temp.ems的情况
                    HideTopWindow(TOPWND_CONVERT, FALSE, FALSE);
                    ShowTopWindow(TOPWND_MAINWND, NULL, 0);
                    return 0;
                }
                break;
            }
        }

        return 0;
    }
    return SMP_MenuWnd_WndProc(hToolbar,hWnd, Msg, wParam, lParam);
}

