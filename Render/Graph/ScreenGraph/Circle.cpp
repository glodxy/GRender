//
// Created by 田淙宇 on 2019/9/26.
//

#include "Circle.h"
#include "../../GCanvas.h"
using namespace GRCore;
using namespace GRGraph;

void Circle::Draw(GCanvas &canvas, const Color &color) {
    if(mA==mB){
        canvas.BresenhamCircle(mX+mA/2,mY+mA/2,mA,color);
    }else{
        canvas.Ellipse(mX+mA/2,mY+mB/2,mA,mB,color);
    }
}

