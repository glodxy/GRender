//
// Created by 田淙宇 on 2019/9/26.
//

#ifndef GRENDERER_CIRCLE_H
#define GRENDERER_CIRCLE_H

#include "../BaseGraph.h"

namespace GRGraph {
    class Circle:public ScreenGraph{
    public:
        Circle(int x,int y,int a,int b):mX(x),mY(y),mA(a),mB(b){}

        Circle(const Circle& circle){
            mX=circle.mX;
            mY=circle.mY;
            mA=circle.mA;
            mB=circle.mB;
        }

        void SetA(int a){
            mA=a;
        }

        void SetB(int b){
            mB=b;
        }

        int X()const{
            return mX;
        }

        int Y()const{
            return mY;
        }



        void Draw(GCanvas& canvas,const Color& color);

    protected:
        int mX,mY,mA,mB;
    };
}


#endif //GRENDERER_CIRCLE_H
