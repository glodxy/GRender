//
// Created by 田淙宇 on 2019/9/26.
//
#include "Line.h"
#include "../../GCanvas.h"
using namespace GRGraph;
using namespace GRCore;
void Line::Draw(GCanvas &canvas,const Color& color) {
    canvas.BresenhamLine(mVertices[0].X(),mVertices[0].Y(),mVertices[1].X(),mVertices[1].Y(),color);
}
