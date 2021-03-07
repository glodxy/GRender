//
// Created by 田淙宇 on 2019/9/4.
//

#ifndef GRENDERER_GCOLOR_H
#define GRENDERER_GCOLOR_H

#include <ctime>
#include "../Util/Math/Vec.h"

namespace GRCore {
    class Color {

    public:
        Color():mR(0),mG(0),mB(0),mA(0){

        }
        Color(int r,int g,int b,int a):mR(r),mG(g),mB(b),mA(a){

        }
        Color(const Vec3i& v){
            mR=v[0];
            mG=v[1];
            mB=v[2];
            mA=255;
        }

        Color(const Vec4i& v){
            mR=v[0];
            mG=v[1];
            mB=v[2];
            mA=v[3];
        }
        Color(const Vec3f& v){
            mR=v[0]*255;
            mG=v[1]*255;
            mB=v[2]*255;
            mA=255;
        }
        Color(const Vec4f& v){
            mR=v[0]*255;
            mG=v[1]*255;
            mB=v[2]*255;
            mA=v[3]*255;
        }

        Color(uint32_t value){
#ifndef _WIN
            mA=value>>24;
            mB=(value>>16)^(mA<<8);
            mG=(value>>8)^(mB<<8)^(mA<<16);
            mR=(value)^(mG<<8)^(mB<<16)^(mA<<24);
#else
            mA=value>>24;
            mR=(value>>16)^(mA<<8);
            mG=(value>>8)^(mR<<8)^(mA<<16);
            mB=(value)^(mG<<8)^(mR<<16)^(mA<<24);
#endif
        }

        uint32_t  ToUint32(){
#ifndef _WIN
            return mR|(mG<<8)|(mB<<16)|(mA<<24);
#else
            return mB|(mG<<8)|(mR<<16)|(mA<<24);
#endif
        }

        friend std::ostream &operator<<(std::ostream& os,const Color& r){
            os<<"Color("<<r.mR<<","<<r.mG<<","<<r.mB<<","<<r.mA<<")"<<std::endl;
            return os;
        }

    private:
        int mR,mG,mB,mA;
    };

    static Color GetRandColor(){
        srand(time(0));
        int r=rand()%255;
        int g=rand()%255;
        int b=rand()%255;
        return Color(r,g,b,255);
    }

#define WHITE Color(255,255,255,255)
#define BLACK Color(0,0,0,255)
#define GREEN Color(0,255,0,255)
#define BLUE Color(0,0,255,255)
#define RED Color(255,0,0,255)
}
#endif //GRENDERER_GCOLOR_H
