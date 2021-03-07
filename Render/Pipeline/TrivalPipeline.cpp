//
// Created by 田淙宇 on 2019/9/19.
//

#include "TrivalPipeline.h"
#include "../Shader/BaseShader.h"
#include <map>
#include <climits>
using GRCore::BaseShader;
using std::map;

void TrivalPipeline::ExecuteVertex() {
    if(mTempVertices.find(mVertexBufferID)==mTempVertices.end()){
        InitializeForExecute(false);
        mFirstExecuteFlag=false;
    }

    mShader->mCount=mTempVertices[mVertexBufferID].size();
    mShader->mCurCount=0;
    //顶点着色
    for(size_t i=0;i<mTempVertices[mVertexBufferID].size();++i){
        Vec3f temp=mTempVertices[mVertexBufferID][i];
        //std::cout<<"vertex "<<i<<":"<<temp<<std::endl;
        Vec4f temp4f;
        mShader->Vert(temp,temp4f);
        //std::cout<<"vertex shaded "<<i<<":"<<temp4f<<std::endl;
        mVertices.push_back(temp4f);
        mShader->mCurCount++;
    }

    std::cout<<"load vertices from buffer suc"<<std::endl;

    //转化为屏幕坐标
    for(int i=0;i<mVertices.size();++i){
        Vec4f temp;
        ScreenTranform(mVertices[i],temp);
        mScreenVertices.push_back(temp);
        //std::cout<<"vertex "<<i<<":"<<"origin :"<<mVertices[i]<<" result:"<<temp<<std::endl;
    }
    std::cout<<"trans to screen suc"<<std::endl;

    //装配三角形
    PrimitiveAssebly(mVertices,mTriangles);

    std::cout<<"setup triangle suc, triangle size:"<<mTriangles.size()<<std::endl;
    CheckCanvasSize();
    //光栅化
    for(int i=0;i<mTriangles.size();++i){
        //std::cout<<"start rasterization"<<std::endl;
        Rasterization(mTriangles[i]);
    }
    PostProcess();
    ResetAll();
}

void TrivalPipeline::ExecuteIndex() {
    if(mTempVertices.find(mVertexBufferID)==mTempVertices.end()){
        InitializeForExecute(true);
        mFirstExecuteFlag=false;
    }

    mShader->mCount=mTempVertices[mVertexBufferID].size();
    mShader->mCurCount=0;
    //顶点着色
    for(size_t i=0;i<mTempVertices[mVertexBufferID].size();++i){
        Vec3f temp=mTempVertices[mVertexBufferID][i];
        //std::cout<<"vertex "<<i<<":"<<temp<<std::endl;
        Vec4f temp4f;
        mShader->Vert(temp,temp4f);
        //std::cout<<"vertex shaded "<<i<<":"<<temp4f<<std::endl;
        mVertices.push_back(temp4f);
        mShader->mCurCount++;
    }
    mShader->mCurCount=0;
    std::cout<<"load vertices from buffer suc"<<std::endl;

    //转化为屏幕坐标
    for(int i=0;i<mVertices.size();++i){
        Vec4f temp;
        ScreenTranform(mVertices[i],temp);
        mScreenVertices.push_back(temp);
        //std::cout<<"vertex "<<i<<":"<<"origin :"<<mVertices[i]<<" result:"<<temp<<std::endl;
    }
    std::cout<<"trans to screen suc"<<std::endl;

    //装配三角形
    PrimitiveAssebly(mVertices,mTriangles);

    std::cout<<"setup triangle suc, triangle size:"<<mTriangles.size()<<std::endl;
    CheckCanvasSize();
    //光栅化
    for(int i=0;i<mTriangles.size();++i){
        //std::cout<<"start rasterization"<<std::endl;
        Rasterization(mTriangles[i]);
    }
    PostProcess();
    ResetAll();
}

void TrivalPipeline::ScreenTranform(const Vec4f &srcData, Vec4f &dstData) {
    for(int i=0;i<3;++i){
        dstData[i]=srcData[i]/srcData[3];
    }
    dstData[3]=1/srcData[3];
    dstData[0]=(dstData[0]+1.0)*mWidth/2;
    dstData[1]=(dstData[1]+1.0)*mHeight/2;
}

void TrivalPipeline::PrimitiveAssebly(vector<Vec4f> &srcData, vector<IndexTriangle> &triangles) {
    int size=srcData.size()/3;
    for(int i=0;i<size;++i){
        IndexTriangle triangle(3*i,1+3*i,2+3*i);
        triangles.push_back(triangle);
    }
}

void TrivalPipeline::PrimitiveAsseblyByIdx(vector<int> &idxData,
                                           vector<IndexTriangle> &triangles) {
    int size=idxData.size()/3;
    for(int i=0;i<size;++i){
        IndexTriangle triangle(idxData[3*i],idxData[1+3*i],idxData[2+3*i]);
        triangles.push_back(triangle);
    }
}

void TrivalPipeline::Rasterization(const IndexTriangle &triangle) {

    Vec2f v0=mScreenVertices[triangle[0]].Embed<2>(0);
    Vec2f v1=mScreenVertices[triangle[1]].Embed<2>(0);
    Vec2f v2=mScreenVertices[triangle[2]].Embed<2>(0);

    float z0=mScreenVertices[triangle[0]].Z();
    float z1=mScreenVertices[triangle[1]].Z();
    float z2=mScreenVertices[triangle[2]].Z();

    if(mLineMode){
        std::cout<<"lined vertex:"<<v0<<";"<<v1<<";"<<v2<<std::endl;
        mCanvas->BresenhamLine(v0[0],v0[1],v1[0],v1[1],GREEN);
        mCanvas->BresenhamLine(v1[0],v1[1],v2[0],v2[1],GREEN);
        mCanvas->BresenhamLine(v2[0],v2[1],v0[0],v0[1],GREEN);
        std::cout<<"lined over"<<std::endl;
        return;
    }

    Triangle<float> vTriangle(v0.Embed<3>(z0),v1.Embed<3>(z1),v2.Embed<3>(z2));
    BoundF bound=GRGraph::FindBound2D<float>(vTriangle);
    //std::cout<<"rasterization bound:"<<bound.left<<","<<bound.right<<","<<bound.top<<","<<bound.bottom<<std::endl;
    Vec2i P;
    for(P[0]=bound.left;P[0]<=bound.right;P[0]++){
        for(P[1]=bound.bottom;P[1]<=bound.top;P[1]++){
            Vec3f bc=GRGraph::Barycentric(v0,v1,v2,P);//求重心坐标
            //std::cout<<"origin:"<<P<<" bc:"<<bc<<std::endl;

            Vec3f interpolation=Vec3f(bc[0]/z0,bc[1]/z1,bc[2]/z2);

            float depth=1/(interpolation.Dot(Vec3f(1.0f,1.0f,1.0f)));
            interpolation=interpolation/(interpolation[0]+interpolation[1]+interpolation[2]);

            if (bc.X()<0 || bc.Y()<0 || bc.Z()<0 || zbuffer[mCanvas->GetPixelIdx(P.X(),P.Y())]>depth) continue;
            Color color;
            bool discard = mShader->Frag(interpolation,triangle,color);
            if (!discard) {
                zbuffer[mCanvas->GetPixelIdx(P.X(),P.Y())] = depth;
                mCanvas->SetPixel(P.X(),P.Y(),color);
            }
        }
    }
}

void TrivalPipeline::OnCanvasChanged(int ow,int oh,int nw,int nh) {
    if(zbuffer!= nullptr){
        delete[] zbuffer;
    }
    zbuffer=new float[nw*nh];
    for(int i=0;i<nw*nh;++i){
        zbuffer[i]=-std::numeric_limits<float>::max();
    }
    mWidth=nw;
    mHeight=nh;
}

void TrivalPipeline::ClearZBuffer() {
    for(int i=0;i<mWidth*mHeight;++i){
        zbuffer[i]=-std::numeric_limits<float>::max();
    }
}

void TrivalPipeline::ResetAll() {
    mVertices.clear();
    mTriangles.clear();
    mScreenVertices.clear();
    mShader->ResetAll();

}
