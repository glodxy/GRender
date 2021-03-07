//
// Created by 田淙宇 on 2019/9/19.
//

#ifndef GRENDERER_LINE_H
#define GRENDERER_LINE_H
#include "../BaseGraph.h"
#include "../../GColor.h"

namespace GRGraph {
    class Line:public ScreenGraph {
    public:
        Line(const Vec3i&p0,const Vec3i&p1) {
            AddVertex(p0);
            AddVertex(p1);
        }

        Line(const Line& r):Line(r[0],r[1]){}

        Line& operator=(const Line& r){
            (this->mVertices)[0]=r[0];
            (this->mVertices)[1]=r[1];
            return *this;
        }

        void Draw(GCanvas& canvas,const Color& color);

    protected:

    };
}
#endif //GRENDERER_LINE_H
