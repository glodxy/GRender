//
// Created by 田淙宇 on 2019/9/12.
//

#ifndef GRENDERER_BASEGRAPH_H
#define GRENDERER_BASEGRAPH_H

#include <vector>
#include "../../Util/Math/Vec.h"
namespace GRCore{
    class GCanvas;
    class Color;
}

using std::vector;
using namespace GRCore;
namespace GRGraph{

    template <class T>
    class BaseGraph{
    public:
        BaseGraph(){}
        BaseGraph(const BaseGraph<T>&g){
            for(int i=0;i<g.mVertices.size();++i){
                AddVertex((g.mVertices)[i]);
            }
        }
        virtual ~BaseGraph(){
            mVertices.clear();
        }

        virtual void Draw(GCanvas& canvas,const Color& color){}

        void AddVertex(Vec<3,T> v){
            mVertices.push_back(v);
        }


        Vec<3,T>& operator[](int idx){
            return mVertices[idx];
        }

        const Vec<3,T>& operator[](int idx)const {
            return mVertices[idx];
        }

        //判断是否是左右顶点
        bool SideVertex(int idx)const{
            Vec<3,T> last=mVertices[(idx-1+Size())%Size()];
            Vec<3,T> next=mVertices[(idx+1)%Size()];
            Vec<3,T> cur=mVertices[idx%Size()];
            return !((last.Y()>=cur.Y()&&next.Y()>=cur.Y())||(last.Y()<=cur.Y()&&next.Y()<=cur.Y()));
        }

        //是否是顺时针
        bool ClockWise()const{
            int idx=0;
            for(int i=0;i<mVertices.size();++i){
                if(mVertices[i].X()>mVertices[idx].X()){
                    idx=i;
                }
            }
            Vec<3,T> lastV=idx==0?mVertices[mVertices.size()-1]:mVertices[idx-1];
            Vec<3,T> nextV=idx==mVertices.size()-1?mVertices[0]:mVertices[idx+1];

            Vec<3,T> a=mVertices[idx]-lastV;
            Vec<3,T> b=nextV-mVertices[idx];

            return a.Cross(b).Z()<0;
        }

        //反向
        void Reverse(){
            std::reverse(mVertices.begin(),mVertices.end());
        }

        int Size()const{
            return mVertices.size();
        }

        friend std::ostream&operator <<(std::ostream& os,const BaseGraph<T>& g){
            for(int i=0;i<g.mVertices.size();++i){
                os<<((g.mVertices)[i])<<"; ";
            }
            return os;
        }





    protected:
        vector<Vec<3,T>> mVertices;
    };



    typedef GRGraph::BaseGraph<float> OriginGraph;
    typedef GRGraph::BaseGraph<int> ScreenGraph;
}

#endif //GRENDERER_BASEGRAPH_H
