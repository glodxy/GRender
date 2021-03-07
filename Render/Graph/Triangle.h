//
// Created by 田淙宇 on 2019/9/12.
//

#ifndef GRENDERER_TRIANGLE_H
#define GRENDERER_TRIANGLE_H


#include "BaseGraph.h"

namespace GRGraph {
    template <class T>
    class Triangle:public BaseGraph<T> {
    public:
        Triangle(const Vec<3,T>&p0,const Vec<3,T>&p1,const Vec<3,T>&p2) {
#if defined(DEBUG)
            //std::cout<<"triangle add:"<<" p0:"<<p0<<" p1:"<<p1<<" p2:"<<p2<<std::endl;
#endif
            AddVertex(p0);
            AddVertex(p1);
            AddVertex(p2);
        }

        Triangle(const Triangle& r):Triangle(r[0],r[1],r[2]){}

        Triangle& operator=(const Triangle& r){
            (this->mVertices)[0]=r[0];
            (this->mVertices)[1]=r[1];
            (this->mVertices)[2]=r[2];
            return *this;
        }

    protected:

    };
}


#endif //GRENDERER_TRIANGLE_H
