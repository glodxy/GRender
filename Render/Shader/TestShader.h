//
// Created by 田淙宇 on 2019/9/21.
//

#ifndef GRENDERER_TESTSHADER_H
#define GRENDERER_TESTSHADER_H


#include "BaseShader.h"
#include "../../Util/Math/Matrix.h"
#include "../GColor.h"
#include "../Camera/BaseCamera.h"

class TestShader:public BaseShader{
public:
    void Vert(const Vec3f &vertex, Vec4f &outVertex){
        Vec3f color;
        Vec2f uv;
        Matrix4f pers,view,model;
        pers=Get<Matrix4f>("perspective");
        view=Get<Matrix4f>("lookat");
        //model=Get<Matrix4f>("model");
        Read(uv,"uv",Vec2f);
        Vec4f temp=vertex.Embed<4>(1);
        outVertex=pers*view*temp;
        Out(uv,"uv",Vec2f);
        //outVertex=Projection(-1.f/(3))*temp;
        //Read(color,"color",Vec3f);
        //std::cout<<"read color:"<<color<<std::endl;
        //Out(color,"color",Vec3f);
        }

    bool Fragment(const Vec3f &interpolation, Color &outColor){
        Color color;
        Vec2f uv;
        string texName;
        texName=Get<string>("texture0");
        In(uv,"uv",Vec2f);
        //In(color,"color",Vec3f);
        //std::cout<<"inter "<<interpolation<<",frag color:"<<color<<std::endl;
        Tex(color,texName,uv);
        outColor=color;

        return false;
    }

    void SetCamera(BaseCamera*c){
        mCamera=c;
    }

protected:
    BaseCamera* mCamera;
};
#endif //GRENDERER_TESTSHADER_H
