//
// Created by 田淙宇 on 2019/9/19.
//

#ifndef GRENDERER_TRIVALPIPELINE_H
#define GRENDERER_TRIVALPIPELINE_H

#include "Pipeline.h"

using GRGraph::Triangle;
using GRGraph::BoundF;
using GRCore::Color;
namespace GRCore {
    typedef Vec3i IndexTriangle;
    class _declspec(dllexport) TrivalPipeline : public Pipeline {
    public:
        virtual ~TrivalPipeline(){
            if(zbuffer!=nullptr)
                delete[] zbuffer;
        }
        void ExecuteVertex()override;
        void ExecuteIndex()override;

        void ClearZBuffer();

        bool mLineMode=false;
    protected:
        void ResetAll()override ;
        void Clip(vector<Vec4f>& srcData){}//齐次裁剪
        void PrimitiveAssebly(vector<Vec4f>&srcData,vector<IndexTriangle>& triangles);//图元装配
        void PrimitiveAsseblyByIdx(vector<int>& idxData,vector<IndexTriangle>&triangles);//根据索引构建三角
        void ScreenTranform(const Vec4f& srcData,Vec4f& dstData);//透视除法+屏幕映射

        void Rasterization(const IndexTriangle& triangle);//光栅化,并进行像素着色

        void PostProcess(){}//后处理

        void OnCanvasChanged(int ow,int oh,int nw,int nh)override;






        vector<Vec4f> mVertices;//经过顶点着色器后的顶点序列
        vector<Vec4f> mScreenVertices;//经过透视除法与屏幕映射后的顶点序列
        vector<IndexTriangle> mTriangles;//构成三角形的索引序列

        bool mFirstExecuteFlag=true;


        float* zbuffer=nullptr;
    };
}


#endif //GRENDERER_TRIVALPIPELINE_H
