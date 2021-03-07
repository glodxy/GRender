//
// Created by 田淙宇 on 2019/9/24.
//

#ifndef GRENDERER_ROUNDCAMERA_H
#define GRENDERER_ROUNDCAMERA_H

#include "BaseCamera.h"

namespace GRCore {
    class RoundCamera :public BaseCamera{
    public:
        RoundCamera(const Vec3f& target,float dis);
        void Rotate(float angle);
        void SetDistance(float dis);
    protected:
        Vec3f mTarget;
        float mDistance;
    };
}

#endif //GRENDERER_ROUNDCAMERA_H
