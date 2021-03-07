//
// Created by 田淙宇 on 2019/9/24.
//

#include "RoundCamera.h"
using GRCore::RoundCamera;

RoundCamera::RoundCamera(const GRCore::Vec3f &target, float dis):BaseCamera() {
    mTarget=target;
    mDistance=dis;
    mPosition=mTarget-mDirection*mDistance;

}

void RoundCamera::Rotate(float angle) {
    Vec4f temp=Vec4f({0,0,0,0})-(mDirection.Embed<4>(1));
    std::cout<<" origin dir:"<<mDirection;
    temp=GRCore::Rotate(Vec3f(0,1,0),angle)*temp;
    mDirection=Vec3f(0,0,0)-temp.Embed<3>(0);
    std::cout<<"; dir:"<<mDirection<<"; origin pos:"<<mPosition<<"; cur Pos:";
    mPosition=(mTarget-(mDirection*mDistance));
    std::cout<<mPosition<<std::endl;
}

void RoundCamera::SetDistance(float dis) {
    mDistance=dis;
    mPosition=mTarget-mDirection*mDistance;
}