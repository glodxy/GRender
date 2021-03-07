//
// Created by 田淙宇 on 2019/9/10.
//

#ifndef GRENDERER_GAPP_H
#define GRENDERER_GAPP_H

#include <string>
#include "../Util/Timer/GTimer.h"
#include "GCanvas.h"

namespace GRCore {
    typedef GRCore::GTimer GameTimer;
    class GApp {
    public:
        float AspectRatio() const{
            return static_cast<float>(mWidth)/mHeight;
        }

        virtual int Run()=0;

        virtual bool Initialize(int width=800,int height=600)=0;

        virtual std::string GetWindowTitle()=0;
        virtual void SetWindowTitle(const std::string& title)=0;


    protected:
        virtual void CalculateFrameStats()=0;

        virtual void OnResize()=0;

        virtual void Update(const GameTimer &gt) = 0;

        virtual void FixedUpdate()=0;

        virtual void Draw(const GameTimer &gt) = 0;

        virtual void DrawCanvas(const GCanvas&cv)=0;

    protected:
        bool mAppPaused = false;//是否暂停
        bool mMinimized = false;//是否最小化
        bool mMaxiMized = false;//是否最大化
        bool mResizing = false;//是否在调整大小
        bool mFullScreen = false;//是否全屏

        GameTimer mTimer;


    protected:
        int mWidth = 800;
        int mHeight = 600;
        float mFixedTime=1.f;
        float mLastTime=0;
        std::string mMainWindowCaption = "D3D";
    };
}



#endif //GRENDERER_GAPP_H
