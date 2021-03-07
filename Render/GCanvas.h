//
// Created by 田淙宇 on 2019/9/4.
//

#ifndef GRENDERER_GCANVAS_H
#define GRENDERER_GCANVAS_H

#include <fstream>
#include <vector>
#include "GColor.h"
#include "Graph/Graph.h"
#include "../Util/Math/Vec.h"
#include <list>
#include <stack>


using GRGraph::ScreenGraph;
using GRGraph::Edge;

namespace GRCore {
class _declspec(dllexport) GCanvas {

public:
    GCanvas(int w,int h,int pixelSize);

    GCanvas(const GCanvas& c):GCanvas(c.Width(),c.Height(),c.PixelSize()){
        memcpy(mBuffer,c.mBuffer, sizeof(uint32_t)*mWidth*mHeight);
    }

    ~GCanvas(){
        delete[] mBuffer;
    }

    int Width()const{
        return mWidth;
    }
    int Height()const{
        return mHeight;
    }
    int PixelSize()const{
        return mPixelSize;
    }
    uint32_t *Data()const{
        return mBuffer;
    }

    void Clear(){
        memset(mBuffer,0, sizeof(uint32_t)*mWidth*mHeight);
    }

    void BresenhamLine(int x0,int y0,int x1,int y1,Color color);
    void DDALine(int x0,int y0,int x1,int y1,Color color);
    void MidPointLine(int x0,int x1,int y0,int y1,Color color);

    void Circle(int x0,int y0,int radius,Color color);
    void BresenhamCircle(int x0,int y0,int radius,Color color);
    void Ellipse(int x0,int y0,int rx,int ry,Color color);

    //扫描线种子填充
    void FillBySeedScanLine(int x0,int y0,Color color);

    //绘制屏幕坐标的图元
    void DrawPolyGon(const ScreenGraph& graph,Color color);


    //绘制单个像素
    inline void SetPixel(int x,int y,Color color){
        if(x>=0&&x<mWidth&&y>=0&&y<mHeight) {
            mBuffer[GetPixelIdx(x,y)] = color.ToUint32();
        }
    }

    uint32_t GetPixel(int i,int j)const{
        return mBuffer[i+mWidth*j];
    }

    uint32_t GetPixel(int idx)const{
        return mBuffer[idx];
    }
    //绘制一行像素
    inline void SetPixelRow(int x0,int x1,int y,Color color){
        for(int i=x0;i<=x1;++i){
            SetPixel(i,y,color);
        }
    }

    inline int GetPixelIdx(int x,int y){
        return x + (mHeight - 1 - y) * mWidth;
    }


protected:
    inline void CirclePlotPoint(int x,int y,int x0,int y0,Color color){
        SetPixel(x0+x,y0+y,color);
        SetPixel(x0-x,y0+y,color);
        SetPixel(x0+x,y0-y,color);
        SetPixel(x0-x,y0-y,color);
        SetPixel(x0+y,y0+x,color);
        SetPixel(x0-y,y0+x,color);
        SetPixel(x0+y,y0-x,color);
        SetPixel(x0-y,y0-x,color);
    }

    inline void EllipsePlotPoint(int x,int y,int x0,int y0,Color color){
        SetPixel(x0+x,y0+y,color);
        SetPixel(x0-x,y0+y,color);
        SetPixel(x0-x,y0-y,color);
        SetPixel(x0+x,y0-y,color);
    }

protected:
    int mWidth,mHeight;
    int mPixelSize;
    uint32_t * mBuffer;

private:
    //填充某行，并获取两侧点
    void ScanFill(const Vec2i& start,Vec2i& left,Vec2i&right,const Color& color,uint32_t oriColor);
    //查询下一个种子点
    void SearchNextSeed(int l,int r,int y,uint32_t oriColor,std::stack<Vec2i>& s);

    //初始化边表
    void InitNET(std::vector<std::list<Edge>>& NET,const ScreenGraph& g,int ymin);
    //插入活动边表
    void InsertAET(std::list<Edge>& AET,const std::list<Edge>& srcET);
    //移除边表
    void RemoveAET(std::list<Edge>& AET,int y);
    //更新活动边表
    void UpdateAET(std::list<Edge>&AET);
    //填充活动边表
    void ExecuteAET(std::list<Edge>& AET,int y,Color color);
};

}

#endif //GRENDERER_GCANVAS_H
