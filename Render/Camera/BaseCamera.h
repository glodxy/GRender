//
// Created by 田淙宇 on 2019/9/24.
//

#ifndef GRENDERER_BASECAMERA_H
#define GRENDERER_BASECAMERA_H

#include "../../Util/Math/Vec.h"
#include "../../Util/Math/Matrix.h"

namespace GRCore {
    class BaseCamera {
    public:
        BaseCamera():mDirection(Vec3f(0,0,-1)),mUp(Vec3f(0,1,0)),mFOV(60),mAspect(800.f/600.f),mNear(-1.f),mFar(-100.f){

        }
        Matrix4f GetPerspectiveMatrix()const {
            return Perspective(mFOV,mAspect,mNear,mFar);
        }

        void SetDirection(const Vec3f& d){
            mDirection=d.Normalized();
        }

        void SetPosition(const Vec3f& p){
            mPosition=p;
        }

        Vec3f Position()const{
            return mPosition;
        }

        Matrix4f GetLookAtMatrix()const{
            return Lookat(mPosition,mPosition+mDirection.Normalized(),mUp);
        }
    protected:
        Vec3f mPosition;
        Vec3f mDirection;
        Vec3f mUp;
        int mFOV;
        float mAspect;
        float mNear;
        float mFar;
    };
}

#endif //GRENDERER_BASECAMERA_H
