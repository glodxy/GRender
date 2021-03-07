//
// Created by 田淙宇 on 2019/9/10.
//

#ifndef GRENDERER_WINAPP_H
#define GRENDERER_WINAPP_H

#include "../../../Util/Timer/GTimer.h"
#include "../../GApp.h"

namespace GRCore {
    typedef GTimer GameTimer;
    class WinApp:public GApp{
    protected:
        WinApp(HINSTANCE hInstance);

        WinApp(const WinApp&)=delete;
        WinApp& operator=(const WinApp&)=delete;
        virtual ~WinApp()=default;

    public:
        static WinApp* GetApp();
        HINSTANCE AppInstance()const;
        HWND MainWnd()const;

        int Run()override;

        bool Initialize(int width,int height)override ;

        virtual LRESULT MsgProcess(HWND hWnd,UINT msg, WPARAM wParam,LPARAM lParam);

    protected:

        void DrawCanvas(const GCanvas& canvas)override ;
        void DrawCanvas(const std::vector<GCanvas>& canva);

        void OnResize()override{}

        virtual void OnMouseDown(WPARAM wParam,int x,int y){}
        virtual void OnMouseUp(WPARAM wParam, int x, int y) {}
        virtual void OnMouseMove(WPARAM wParam, int x, int y) {}

        virtual void OnKeyDown(WPARAM wParam){}
        virtual void OnKeyUp(WPARAM wParam){}

        virtual void FixedUpdate() override{}

        std::string GetWindowTitle();
        void SetWindowTitle(const std::string& title);

    protected:
        void ResetScreenParam();

        bool InitWindow();

        void CalculateFrameStats()override;

    protected:
        static WinApp* mApp;
        std::string mModeName;

        uint32_t *mBuffer;

        HINSTANCE  mAppInstance= nullptr;
        HWND mMainWindow=nullptr;

        HDC mScreenDc;
        HBITMAP mScreenBmp;
        HBITMAP mScreenOldBmp;

    };
}


#endif //GRENDERER_WINAPP_H
