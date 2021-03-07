//
// Created by 田淙宇 on 2019/10/10.
//

#ifndef GRENDERER_POLYGON_H
#define GRENDERER_POLYGON_H


#include "../BaseGraph.h"
#include "../../GColor.h"

namespace GRGraph {
    class Polygon:public ScreenGraph {
    public:
        Polygon(const Vec2i& start,bool fill=false):ScreenGraph(){
            this->fill=fill;
            finished=false;
            PushVertex(start);
            temp=start;
        }

        Polygon(const ScreenGraph& g,bool fill=false);

        void PushVertex();//将临时点加入

        void PushVertex(int x,int y);

        void PushVertex(const Vec2i& vtx);//添加顶点

        void PopVertex();//去除顶点

        void Finish();//结束绘制

        void SetCurVertex(const Vec2i& vtx);//设置当前顶点位置

        void Draw(GCanvas& canvas,const Color& color);

        bool Fill()const;

    private:
        Vec2i temp;
        bool fill;
        bool finished;
    };
}


#endif //GRENDERER_POLYGON_H
