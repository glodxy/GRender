//
// Created by 田淙宇 on 2019/9/10.
//

#include <cassert>
#include <windowsx.h>
#include "WinApp.h"

LRESULT CALLBACK
WinMainWndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
    return GRCore::WinApp::GetApp()->MsgProcess(hWnd,msg,wParam,lParam);
}

GRCore::WinApp* GRCore::WinApp::mApp= nullptr;

GRCore::WinApp* GRCore::WinApp::GetApp() {
    return mApp;
}

GRCore::WinApp::WinApp(HINSTANCE hInstance) :mAppInstance(hInstance){
    assert(mApp== nullptr);
    mApp=this;
}

HINSTANCE GRCore::WinApp::AppInstance() const {
    return mAppInstance;
}

HWND GRCore::WinApp::MainWnd() const {
    return mMainWindow;
}

bool GRCore::WinApp::Initialize(int width,int height) {
    mWidth=width;
    mHeight=height;
    if(!InitWindow())
        return false;

    OnResize();
    return true;
}

LRESULT GRCore::WinApp::MsgProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg){
        //激活或非激活时
        case WM_ACTIVATE:{
            //激活时启动，否则暂停
            if(LOWORD(wParam)==WA_INACTIVE){
                mAppPaused=true;
                mTimer.Stop();
            }
            else{
                mAppPaused=false;
                mTimer.Start();
            }
            return 0;
        }

            //拖动改变窗口大小时
        case WM_ENTERSIZEMOVE:{
            mAppPaused=true;
            mResizing=true;
            mTimer.Stop();
            return 0;
        }
            //停止改变窗口大小时
        case WM_EXITSIZEMOVE:{
            mAppPaused=false;
            mResizing=false;
            mTimer.Start();
            OnResize();
            return 0;
        }
            //窗口关闭时
        case WM_DESTROY:{
            PostQuitMessage(0);
            return 0;
        }
            //用户点击窗口按键时
        case WM_MENUCHAR:{
            return MAKELRESULT(0,MNC_CLOSE);
        }
            //鼠标按键
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:{
            OnMouseDown(wParam,GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
            return 0;
        }
            //鼠标松开
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }
            //鼠标移动
        case WM_MOUSEMOVE: {
            OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }
        //按键事件
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:{
            OnKeyDown(wParam);
            return 0;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:{
            OnKeyUp(wParam);
            return 0;
        }
    }
    return DefWindowProc(hWnd,msg,wParam,lParam);
}

bool GRCore::WinApp::InitWindow() {
    ResetScreenParam();
    WNDCLASS wc;
    wc.style=CS_BYTEALIGNCLIENT;
    wc.lpfnWndProc=WinMainWndProc;
    wc.cbClsExtra=0;
    wc.cbWndExtra=0;
    wc.hInstance=mAppInstance;
    wc.hIcon=LoadIcon(0,IDI_APPLICATION);
    wc.hCursor=LoadCursor(0,IDC_ARROW);
    wc.hbrBackground=(HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszMenuName=0;
    wc.lpszClassName="WMainWnd";

    BITMAPINFO bmpInfo={{sizeof(BITMAPINFOHEADER),mWidth,-mHeight,1,32,BI_RGB,(DWORD)mWidth*mHeight*4,0,0,0,0}};

    if(!RegisterClass(&wc)){
        MessageBox(0,"Regist Class Failed",0,0);
        return false;
    }

    RECT rect={0,0,mWidth,mHeight};

    mMainWindow=CreateWindow("WMainWnd",mMainWindowCaption.c_str(),WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
            CW_USEDEFAULT,CW_USEDEFAULT,0,0,0,0,mAppInstance,0);
    if(!mMainWindow){
        MessageBox(0,"Create Window Failed",0,0);
        return false;
    }

    LPVOID ptr;
    HDC hDC=GetDC(mMainWindow);
    mScreenDc=CreateCompatibleDC(hDC);
    ReleaseDC(mMainWindow,hDC);

    mScreenBmp=CreateDIBSection(mScreenDc,&bmpInfo,DIB_RGB_COLORS,&ptr,0,0);
    if(!mScreenBmp){
        MessageBox(0,"Create ScreenBMP Failed",0,0);
        return false;
    }

    mScreenOldBmp=(HBITMAP)SelectObject(mScreenDc,mScreenBmp);
    mBuffer=(uint32_t*)ptr;

    AdjustWindowRect(&rect,GetWindowLong(mMainWindow,GWL_STYLE),0);
    int w=rect.right-rect.left;
    int h=rect.bottom-rect.top;
    int sx=(GetSystemMetrics(SM_CXSCREEN)-w)/2;
    int sy=(GetSystemMetrics(SM_CYSCREEN)-h)/2;
    if(sy<0)sy=0;
    SetWindowPos(mMainWindow,0,sx,sy,w,h,(SWP_NOCOPYBITS|SWP_NOZORDER|SWP_SHOWWINDOW));
    SetForegroundWindow(mMainWindow);

    ShowWindow(mMainWindow,SW_NORMAL);

    return true;
}

void GRCore::WinApp::DrawCanvas(const GRCore::GCanvas &canva) {
    //将数据拷入
    memcpy(mBuffer,canva.Data(), sizeof(uint32_t)*canva.Width()*canva.Height());
    HDC dc=GetDC(mMainWindow);
    BitBlt(dc,0,0,mWidth,mHeight,mScreenDc,0,0,SRCCOPY);
    ReleaseDC(mMainWindow,dc);

}

void GRCore::WinApp::DrawCanvas(const std::vector<GCanvas> &canva) {
    int width=canva[0].Width();
    int height=canva[0].Height();
    int size=canva.size()-1;
    std::cout<<"width:"<<width<<"height:"<<height<<"size:"<<size<<std::endl;
    for(int i=0;i<width;++i){
        for(int j=0;j<height;++j){
            uint32_t color;
            for(int m=size;m>=0;--m){

                color=canva[m].GetPixel(i,j);
                if(m==0){
                    mBuffer[i+width*j]=color;
                }
                else if((color)!=0){
                    mBuffer[i+width*j]=color;
                    break;
                }
            }
        }
    }
    HDC dc=GetDC(mMainWindow);
    BitBlt(dc,0,0,mWidth,mHeight,mScreenDc,0,0,SRCCOPY);
    ReleaseDC(mMainWindow,dc);
}

void GRCore::WinApp::ResetScreenParam() {
    if(mScreenDc){
        if(mScreenOldBmp){
            SelectObject(mScreenDc,mScreenOldBmp);
            mScreenOldBmp= nullptr;
        }
        DeleteDC(mScreenDc);
        mScreenDc= nullptr;
    }
    if(mScreenBmp){
        DeleteObject(mScreenBmp);
        mScreenBmp=nullptr;
    }
    if(mMainWindow){
        CloseWindow(mMainWindow);
        mMainWindow= nullptr;
    }
}

int GRCore::WinApp::Run() {
    MSG msg={0};

    mTimer.Reset();

    while(msg.message!=WM_QUIT){
        if(PeekMessage(&msg,0,0,0,PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else{
            mTimer.Tick();

            if(!mAppPaused){
                CalculateFrameStats();
                Update(mTimer);
                Draw(mTimer);
                while(mTimer.TotalTime()-mLastTime>=mFixedTime){
                    FixedUpdate();
                    mLastTime+=mFixedTime;
                }
            }
            else{
                Sleep(100);
            }
        }
    }
    return (int)msg.wParam;
}

void GRCore::WinApp::CalculateFrameStats() {
    static int frameCount=0;
    static float timeElapsed=0.0f;

    frameCount++;

    if((mTimer.TotalTime()-timeElapsed)>=1.0f){
        float fps=(float)frameCount;
        float mspf=1000.0f/fps;

        std::string fpsStr=std::to_string(fps);
        std::string mspfStr=std::to_string(mspf);

        std::string title=mMainWindowCaption+"    fps:"+fpsStr+"     mspf:"+mspfStr+"     mode:"+mModeName;

        SetWindowText(mMainWindow,title.c_str());

        frameCount=0;
        timeElapsed+=1.0f;
    }
}

void GRCore::WinApp::SetWindowTitle(const std::string &title) {
    SetWindowText(mMainWindow,title.c_str());
}

std::string GRCore::WinApp::GetWindowTitle() {
    char temp[100];
    GetWindowText(mMainWindow,temp, sizeof(temp));
    return std::string(temp);

}

