//
// Created by 田淙宇 on 2019/9/17.
//

#ifndef GRENDERER_BASESHADER_H
#define GRENDERER_BASESHADER_H

#include "../Graph/Graph.h"
#include "../Pipeline/Pipeline.h"
#include <unordered_map>

using std::unordered_map;
using std::string;
using GRCore::Pipeline;
using GRCore::Buffer;

namespace GRCore {
    class BaseShader {
    public:
        virtual void Vert(const Vec3f &vertex, Vec4f &outVertex) = 0;

        bool Frag(const Vec3f& interpolation,const Vec3i& face,Color& outColor){
            curFace=face;
//            std::cout<<"curFace"<<curFace<<std::endl;
//            char* s=valueMap["color"];
//            std::cout<<"origin Color:"<<*((Vec3f*)(s+ sizeof(Vec3f)*curFace[0]))<<";"<<*((Vec3f*)valueMap["color"]+ ((size_t)curFace[1]))<<";"<<*((Vec3f*)valueMap["color"]+ ((size_t)curFace[2]))<<std::endl;
            return Fragment(interpolation,outColor);
        }

        virtual bool Fragment(const Vec3f &interpolation, Color &outColor) = 0;

        void ResetAll(){
            mCount=mCurCount=0;
            for(auto iter=valueMap.begin();iter!=valueMap.end();++iter){
                delete[] (iter->second);
            }
            valueMap.clear();
        }

        template<class T>
        void Set(T a,const string& name){
            T *result;
            if(staticMap.find(name)!=staticMap.end()){
                result=reinterpret_cast<T*>(staticMap[name]);
                if(a==(*result)){
                    return;
                }
                delete[] staticMap[name];
            }
            result=new T(a);
            staticMap[name]= reinterpret_cast<char*>(result);
        }

        template<class T>
        T Get(const string& name){
            T* result;
            if(staticMap.find(name)==staticMap.end()){
                std::cerr<<"error! shader cannot get the value of "<<name<<std::endl;
                exit(0);
            }
            result= reinterpret_cast<T*>(staticMap[name]);
            return *result;
        }


        int mCount;//总顶点数
        int mCurCount;//当前顶点位置

        Vec3i curFace;
        Pipeline *pipeline;
    protected:
        unordered_map<string,char*> valueMap;//每帧的临时数据，渲染后删除
        unordered_map<string,char*> staticMap;//不会被删除
    };

#define Read(Out,Name,Type) {\
    Buffer buffer=pipeline->GetBuffer(Name);\
    int idx=pipeline->GetIndex(Name,mCurCount);\
    Out=*(((Type*)buffer.buffer)+((size_t)idx));\
}

#define In(Out,Name,Type) {\
        char* s=valueMap[Name];\
        Type v0=*((Type*)s+ ((size_t)curFace[0]));\
        Type v1=*((Type*)s+ ((size_t)curFace[1]));\
        Type v2=*((Type*)s+ ((size_t)curFace[2]));\
        Out=v0*interpolation[0]+v1*interpolation[1]+v2*interpolation[2];\
    }

#define Out(Data,Name,Type) {\
        if(valueMap.find(Name)==valueMap.end()){\
            char* buf=new char[sizeof(Type)*mCount];\
            valueMap[Name]=buf;\
        }\
        char* s=valueMap[Name];\
        memcpy(s+ sizeof(Type)*mCurCount,&Data, sizeof(Type));\
    }

#define Tex(outColor,name,pos) {\
        Texture* tex=pipeline->GetTexture(name);\
        outColor=tex->Get(pos);\
    }

#define ToName(a) #a
}
#endif //GRENDERER_BASESHADER_H
