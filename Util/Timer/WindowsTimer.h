//
// Created by 田淙宇 on 2019/9/9.
//

#ifndef GRENDERER_WINDOWSTIMER_H
#define GRENDERER_WINDOWSTIMER_H
#define NOMINMAX
#include <Windows.h>

namespace GRCore {
    class _declspec(dllexport) WindowsTimer {
    public:
        WindowsTimer();

        float TotalTime() const;

        float DeltaTime() const;

        void Reset();

        void Start();

        void Stop();

        void Tick();

    private:
        double mSecondsPerCount;
        double mDeltaTime;

        __int64 mBaseTime;
        __int64 mPausedTime;
        __int64 mStopTime;
        __int64 mPrevTime;
        __int64 mCurTime;

        bool mStopped;
    };

}

#endif //GRENDERER_WINDOWSTIMER_H
