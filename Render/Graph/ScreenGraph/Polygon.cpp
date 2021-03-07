//
// Created by 田淙宇 on 2019/10/10.
//

#include "Polygon.h"
#include "../../GCanvas.h"
using namespace GRGraph;

Polygon::Polygon(const GRGraph::ScreenGraph &g,bool fill):BaseGraph<int>(g){
    this->fill=fill;
    finished=true;
}

void Polygon::PushVertex() {
    PushVertex(temp);
}

void Polygon::PushVertex(const Vec2i &vtx) {
    AddVertex(vtx.Embed<3>(0));
}

void Polygon::PushVertex(int x, int y) {
    PushVertex(Vec2i(x,y));
}

void Polygon::Finish() {
    finished=true;
}

void Polygon::PopVertex() {
    if(mVertices.size()>1) {
        temp = mVertices.back().Embed<2>(0);
        mVertices.pop_back();
    }
}

void Polygon::SetCurVertex(const Vec2i &vtx) {
    temp=vtx;
}

bool Polygon::Fill() const {
    return fill;
}

void Polygon::Draw(GCanvas &canvas, const Color &color) {
    if(finished){
        if(fill){
            canvas.DrawPolyGon(*this,color);
        }
        else{
            for(int i=0;i<mVertices.size();++i){
                int r=(i+1)%mVertices.size();
                canvas.BresenhamLine(mVertices[i].X(),mVertices[i].Y(),mVertices[r].X(),mVertices[r].Y(),color);
            }
        }
    }
    else{
        int size=mVertices.size();
        for(int i=0;i<size-1;++i){
            canvas.BresenhamLine(mVertices[i].X(),mVertices[i].Y(),mVertices[i+1].X(),mVertices[i+1].Y(),color);
        }
        canvas.BresenhamLine(mVertices[size-1].X(),mVertices[size-1].Y(),temp.X(),temp.Y(),color);
    }
}