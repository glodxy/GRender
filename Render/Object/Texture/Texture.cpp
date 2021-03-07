//
// Created by 田淙宇 on 2019/9/27.
//

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../../Library/stb_image.h"
using namespace GRCore;

Texture::Texture(const char *fileName,bool flip) :mFlip(flip){
    mData=stbi_load(fileName,&mWidth,&mHeight,&mChannel,0);
}

Color Texture::Get(int x, int y) const {
    if(mFlip){
        y=mHeight-1-y;
    }
    if(mChannel==3){
        return Color(_get3i(x,y));
    }
    else if(mChannel==4){
        return Color(_get4i(x,y));
    }
}

Color Texture::Get(const GRCore::Vec2f &uv) const {
    int x=(int)(uv[0]*(float)mWidth);
    int y=(int)(uv[1]*(float)mHeight);
    return Get(x,y);
}

