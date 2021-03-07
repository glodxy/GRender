//
// Created by 田淙宇 on 2019/9/27.
//

#ifndef GRENDERER_TEXTURE_H
#define GRENDERER_TEXTURE_H

#include "../../../Util/Math/Vec.h"
#include "../../GColor.h"

namespace GRCore {
    class Texture {
    public:
        Texture(const char *fileName, bool flip=false);

        int Channel()const{
            return mChannel;
        }

        int Width()const{
            return mWidth;
        }

        int Height()const{
            return mHeight;
        }

        int Size()const{
            return mWidth*mHeight;
        }

        Color Get(int x,int y)const;
        Color Get(const Vec2f& uv)const;




    private:
        int mWidth, mHeight, mChannel;
        bool mFlip;//是否反向
        unsigned char *mData;

        Vec3i _get3i(int x, int y)const{
            unsigned char r=mData[y*mChannel*mWidth+x*mChannel+0];
            unsigned char g=mData[y*mChannel*mWidth+x*mChannel+1];
            unsigned char b=mData[y*mChannel*mWidth+x*mChannel+2];
            return Vec3i(r,g,b);
        }

        Vec4i _get4i(int x,int y)const{
            unsigned char r=mData[y*mChannel*mWidth+x*mChannel+0];
            unsigned char g=mData[y*mChannel*mWidth+x*mChannel+1];
            unsigned char b=mData[y*mChannel*mWidth+x*mChannel+2];
            unsigned char a=mData[y*mChannel*mWidth+x*mChannel+3];
            return Vec4i(r,g,b,a);
        }
    };
}

#endif //GRENDERER_TEXTURE_H
