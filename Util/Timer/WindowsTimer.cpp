//
// Created by 田淙宇 on 2019/9/9.
//


#include "WindowsTimer.h"

GRCore::WindowsTimer::WindowsTimer():mSecondsPerCount(0.0),mDeltaTime(-1.0),mBaseTime(0),mPausedTime(0),
mPrevTime(0),mCurTime(0),mStopped(false)
{
    __int64 countsPerSec;
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    mSecondsPerCount=1.0/(double)countsPerSec;
}

void GRCore::WindowsTimer::Tick() {
    if(mStopped){
        mDeltaTime=0.0;
        return;
    }

    //获取本帧开始显示时间
    __int64 curTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
    mCurTime=curTime;

    //计算与上帧的时间差
    mDeltaTime=(mCurTime-mPrevTime)*mSecondsPerCount;
    mPrevTime=mCurTime;

    if(mDeltaTime<0.0){
        mDeltaTime=0.0;
    }
}

float GRCore::WindowsTimer::DeltaTime() const {
    return (float)mDeltaTime;
}

float GRCore::WindowsTimer::TotalTime() const {
    if(mStopped){
        return (float)(((mStopTime-mPausedTime)-mBaseTime)*mSecondsPerCount);
    }
    else{
        return (float)(((mCurTime-mPausedTime)-mBaseTime)*mSecondsPerCount);
    }
}

void GRCore::WindowsTimer::Reset() {
    __int64 curTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

    mBaseTime=curTime;
    mPrevTime=curTime;
    mStopTime=0;
    mStopped=false;
}

void GRCore::WindowsTimer::Stop() {
    if(!mStopped){
        __int64 curTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

        mStopTime=curTime;
        mStopped= true;
    }
}

void GRCore::WindowsTimer::Start() {
    __int64 startTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

    if(mStopped){
        mPausedTime+=(startTime-mStopTime);

        mPrevTime=startTime;

        mStopTime=0;
        mStopped=false;
    }
}
