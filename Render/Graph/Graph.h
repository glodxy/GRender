//
// Created by 田淙宇 on 2019/9/12.
//

#ifndef GRENDERER_GRAPH_H
#define GRENDERER_GRAPH_H

#include <algorithm>
#include "BaseGraph.h"
#include "Triangle.h"
#include "ScreenGraph/Line.h"
#include "ScreenGraph/Circle.h"
#include "ScreenGraph/Polygon.h"

#define RIGHT 8
#define LEFT 4
#define TOP 2
#define BOTTOM 1


namespace GRGraph{
    //填充用的边表节点
    struct Edge{
        double x;
        double dx;
        int ymax;
    };

    //交点
    template<class T>
    struct Intersection{
        Vec<3,T> point;
        bool out;//是否是出点
    };

    template<class T>
    struct Bound{
        T bottom;
        T top;
        T left;
        T right;
        T back;
        T front;


    public:
        T&operator[](int idx){
            switch(idx){
                case 0:return bottom;
                case 1:return top;
                case 2:return left;
                case 3:return right;
                case 4:return back;
                case 5:return front;
                default:{
                    std::cerr<<"get bound over index";
                    exit(0);
                }
            }
        }

        const T&operator[](int idx)const{
            switch(idx){
                case 0:return bottom;
                case 1:return top;
                case 2:return left;
                case 3:return right;
                case 4:return back;
                case 5:return front;
                default:{
                    std::cerr<<"get bound over index";
                    exit(0);
                }
            }
        }
    };

    typedef Bound<int> BoundI;
    typedef Bound<float> BoundF;
    //查找边界
    template<class T>
    static Bound<T> FindBound2D(const BaseGraph<T>& g){
#if defined(DEBUG)
#endif
        T b=(g[0]).Y();
        T t=(g[0]).Y();
        T l=g[0].X();
        T r=g[0].X();
        for(int i=0;i<g.Size();++i){
            if(g[i].Y()<b)
                b=g[i].Y();
            if(g[i].Y()>t)
                t=g[i].Y();
            if(g[i].X()>r)
                r=g[i].X();
            if(g[i].X()<l)
                l=g[i].X();
        }
        return Bound<T>{b,t,l,r,0,0};
    }

    //判断顶点是否在边界内
    template<class T>
    static bool Inside(const Vec<3,T>&v,const Bound<T>& bound, int idx=-1){
        switch(idx){
            case -1:return bound.top >= v.Y() && bound.bottom <= v.Y() && bound.left <= v.X() && bound.right >= v.X()&&bound.front>=v.Z()&&bound.back<=v.Z();
            case 0:return v.Y()>=bound.bottom;
            case 1:return v.Y()<=bound.top;
            case 2:return v.X()>=bound.left;
            case 3:return v.X()<=bound.right;
            case 4:return v.Z()>=bound.back;
            case 5:return v.Z()<=bound.front;
        }

    }

    //通用3D计算线段与平面交点
    template<class T>
    static Vec<3,T> Intersect3D(const Vec<3,T>&v0,const Vec<3,T>& v1,const Vec<3,T>&norm/*平面法线*/,const Vec<3,T>& point/*平面上的一个点*/){
        Vec<3,T> vec0=point-v0;
        Vec<3,T> n=norm.Normalized();//单位法向量

        Vec<3,T> direction=(v1-v0).Normalized();//线段方向单位向量

        T distance=vec0.Dot(n);//点离平面距离

        T intensity=distance/(direction.Dot(n));//线段长度

        Vec<3,T> result=v0+direction*intensity;
        return result;
    }


    //*/////////////////////////////////2d交点
    //排斥实验
    template <class T>
    bool IsRectCross(const Vec<2,T> &p1,const Vec<2,T> &p2,const Vec<2,T> &q1,const Vec<2,T> &q2)
    {
        bool ret = std::min(p1.X(),p2.X()) <= std::max(q1.X(),q2.X()) &&
                   std::min(q1.X(),q2.X()) <= std::max(p1.X(),p2.X()) &&
                   std::min(p1.Y(),p2.Y()) <= std::max(q1.Y(),q2.Y()) &&
                   std::min(q1.Y(),q2.Y()) <= std::max(p1.Y(),p2.Y());
        return ret;
    }

    //跨立判断
    template<class T>
    bool IsLineSegmentCross(const Vec<2,T> &P1,const Vec<2,T> &P2,const Vec<2,T> &Q1,const Vec<2,T> &Q2)
    {
        Vec<3,T> tp1=P1.Embed();
        Vec<3,T> tp2=P2.Embed();
        Vec<3,T> tq1=Q1.Embed();
        Vec<3,T> tq2=Q2.Embed();
        std::cout<<"cross result:"<<(tp1-tq1).Cross(tq2-tq1)<<";"<<(tq2-tq1).Cross(tp2-tq1)<<";"<<(tq1-tp1).Cross(tp2-tp1)<<";"<<(tp2-tp1).Cross(tq2-tp1)<<";"<<std::endl;
        long long tempa=(long long)((tp1-tq1).Cross(tq2-tq1)).Z()*((tq2-tq1).Cross(tp2-tq1)).Z();
        long long tempb=(long long)((tq1-tp1).Cross(tp2-tp1)).Z()*((tp2-tp1).Cross(tq2-tp1)).Z();
        std::cout<<"dot result:"<<tempa<<";"<<tempb<<std::endl;
        return tempa>0 &&
                tempb>0;
    }


    template <class T>
    static bool Intersect2D(const Vec<2,T>&v0,const Vec<2,T>&v1,const Vec<2,T>& nv0,const Vec<2,T>& nv1,Vec<2,T>& out){
        if(IsRectCross(v0,v1,nv0,nv1)){
            std::cout<<"line:"<<v0<<";"<<v1<<"; line 2:"<<nv0<<";"<<nv1<<"; passed rect"<<std::endl;
            if(IsLineSegmentCross(v0,v1,nv0,nv1)){
                std::cout<<"line:"<<v0<<";"<<v1<<"; line 2:"<<nv0<<";"<<nv1<<"; passed cross"<<std::endl;
                double d= (v1.Y() - v0.Y())*(nv1.X() - nv0.X()) - (v0.X() - v1.X())*(nv0.Y() - nv1.Y());

                T x = ( (v1.X() - v0.X()) * (nv1.X() - nv0.X()) * (nv0.Y() - v0.Y())
                      + (v1.Y() - v0.Y()) * (nv1.X() - nv0.X()) * v0.X()
                      - (nv1.Y() - nv0.Y()) * (v1.X() - v0.X()) * nv0.X() ) / d;

                T y =-( (v1.Y() - v0.Y()) * (nv1.Y() - nv0.Y()) * (nv0.X() - v0.X())
                       + (v1.X() - v0.X()) * (nv1.Y() - nv0.Y()) * v0.Y()
                       - (nv1.X() - nv0.X()) * (v1.Y() - v0.Y()) * nv0.Y() ) / d;

//                T k0=(v1.Y()-v0.Y())/(v1.X()-v0.X());
//                T k1=(nv1.Y()-nv0.Y())/(nv1.X()-nv0.X());
//
//                T x=nv0.Y()-k1*nv0.X()-v0.Y()+k0*v0.X();
//                x=(double)x/(k0-k1);
//                T y=k0*x+(v0.Y()-k0*v0.X());
                out=Vec<2,T>(x,y);
                return true;
            }
        }
        return false;
    }

    template <class T>
    static double Distance(const Vec<2,T>&p0,const Vec<2,T>&p1){
        return std::sqrt(std::pow(p0.X()-p1.X(),2)+std::pow(p0.Y()-p1.Y(),2));
    }




    //求2D线段与某边界的交点
    template<class T>
    static Vec<2,T> IntersectBound2D(const Vec<3,T>& v0,const Vec<3,T>&v1,T bound,bool vertical=false){
        Vec<2,T> result;
        float dx=v1.X()-v0.X();
        float dy=v1.Y()-v0.Y();

        //边界是水平的
        if(!vertical){
            result[1]=bound;
            result[0]=(T)(v0.X()+((float)bound-v0.Y())*(dx/dy));
        }
        //垂直
        else{
            result[0]=bound;
            result[1]=(T)(v0.Y()+((float)bound-v0.X())*(dy/dx));
        }
        return result;
    }

    //求2D线段与边界的交点(Nicholl-Lee-Nicholl裁剪)
    template<class T>
    static Intersection<T> IntersectBound2D(const Vec<3,T>& v0,const Vec<3,T>&v1,const Bound<T>& bound){

    }

    //3D线段与某边界面的交点
    template<class T>
    static Vec<3,T> IntersectBound3D(const Vec<3,T>&v0,const Vec<3,T>& v1,const Bound<T>& bound,int idx){
        switch(idx){
            //bottom
            case 0:{
                return Intersect3D(v0,v1,Vec<3,T>(0,-1,0),Vec<3,T>(0,bound.bottom,0));
            }
            //top
            case 1:{
                return Intersect3D(v0,v1,Vec<3,T>(0,1,0),Vec<3,T>(0,bound.top,0));
            }
            //left
            case 2:{
                return Intersect3D(v0,v1,Vec<3,T>(-1,0,0),Vec<3,T>(bound.left,0,0));
            }
            //right
            case 3:{
                return Intersect3D(v0,v1,Vec<3,T>(1,0,0),Vec<3,T>(bound.right,0,0));
            }
            //back
            case 4:{
                return Intersect3D(v0,v1,Vec<3,T>(0,0,-1),Vec<3,T>(0,0,bound.back));
            }
            //front
            case 5:{
                return Intersect3D(v0,v1,Vec<3,T>(0,0,1),Vec<3,T>(0,0,bound.front));
            };
        }
    }

    //求重心坐标
    static Vec3f Barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
        Vec3f s[2];
        for (int i=2; i--; ) {
            s[i][0] = C[i]-A[i];
            s[i][1] = B[i]-A[i];
            s[i][2] = A[i]-P[i];
        }
        Vec3f u =s[0].Cross( s[1]);
        if (std::abs(u[2])>1e-2) // 即非退化三角形
            return Vec3f(1.f-(u.X()+u.Y())/u.Z(), u.Y()/u.Z(), u.X()/u.Z());
        return Vec3f(-1,1,1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
    }

//*///////////////////////////////////cohen-sutherland线段裁剪
    //顶点编码
    template<class T>
    static int EncodePoint2D(const Vec<2,T>& p,const Bound<T>& b){
        int i=0;
        if(p.Y()>b.top){
            i|=TOP;
        }
        else if(p.Y()<b.bottom){
            i|=BOTTOM;
        }
        if(p.X()>b.right){
            i|=RIGHT;
        }
        else if(p.X()<b.left){
            i|=LEFT;
        }
        return i;
    }

    static bool CohenSutherLandClipLine2D(const Line& line,const BoundI& bound,Line& outLine){
        Vec2i p0=line[0].Embed<2>(0);
        Vec2i p1=line[1].Embed<2>(0);
        int c0,c1;
        c1=EncodePoint2D<int>(p1,bound);
        while((c0=EncodePoint2D<int>(p0,bound))!=0||c1!=0){
            if(c0==0){
                std::swap(p0,p1);
                std::swap(c0,c1);
            }else {
                if ((c0 & c1) != 0) {
                    return false;
                }
            }
            for(int i=0;i<4;++i){
                if((c0&(1<<i))!=0){
                    p0=IntersectBound2D(p0.Embed<3>(0),p1.Embed<3>(0),bound[i],i>=2);
                    break;
                }
            }
        }
        outLine[0]=p0.Embed<3>(0);
        outLine[1]=p1.Embed<3>(0);
        return true;
    }
//*////////////////////////////////////////


    //SutherLandHodgman多边形单边裁剪
    static ScreenGraph SutherLandHodgmanPolygonClipEdge2D(const ScreenGraph& graph,const BoundI& bound,int idx){
        ScreenGraph result;
        for(int i=0;i<graph.Size();++i){
            Vec3i startVertex=graph[i];
            Vec3i endVertex=graph[(i+1)%graph.Size()];
            //如果结束点在边界内
            if(Inside(endVertex,bound,idx)){
                //如果起始点在边界内
                if(Inside(startVertex,bound,idx)){
                    result.AddVertex(endVertex);
                }
                //起始点在边界外
                else{
                    Vec2i intersectVertex=IntersectBound2D(startVertex,endVertex,bound[idx],idx>=2);
                    result.AddVertex(intersectVertex.Embed<3>(0));
                    result.AddVertex(endVertex);
                }
            }

            else{
                if(Inside(startVertex,bound,idx)){
                    Vec2i intersectVertex=IntersectBound2D(startVertex,endVertex,bound[idx],idx>=2);
                    result.AddVertex(intersectVertex.Embed<3>(0));
                }
            }
        }
        return result;
    }

    //SutherLand多边形单面裁剪
    static OriginGraph SutherLandHodgmanPolygonClipEdge3D(const OriginGraph& graph,const BoundF& bound,int idx){
        OriginGraph result;
        for(int i=0;i<graph.Size();++i){
            Vec3f startVertex=graph[i];
            Vec3f endVertex=graph[(i+1)%graph.Size()];
            //如果结束点在边界内
            if(Inside(endVertex,bound,idx)){
                //如果起始点在边界内
                if(Inside(startVertex,bound,idx)){
                    result.AddVertex(endVertex);
                }
                    //起始点在边界外
                else{
                    Vec3f intersectVertex=IntersectBound3D(startVertex,endVertex,bound,idx);
                    result.AddVertex(intersectVertex);
                    result.AddVertex(endVertex);
                }
            }

            else{
                if(Inside(startVertex,bound,idx)){
                    Vec3f intersectVertex=IntersectBound3D(startVertex,endVertex,bound,idx);
                    result.AddVertex(intersectVertex);
                }
            }
        }
        return result;
    }

    //SutherLand&Hodgman多边形裁剪
    static ScreenGraph SutherLandHodgmanPolygonClip2D(const ScreenGraph& graph,const BoundI& bound){
        ScreenGraph tempGraph=graph;
        //对于四个边界进行处理
        for(int i=0;i<4;++i){
            tempGraph=SutherLandHodgmanPolygonClipEdge2D(tempGraph,bound,i);
        }
#if defined(DEBUG)
        std::cout<<"cliped graph:"<<tempGraph<<std::endl;
#endif
        return tempGraph;
    }

    //3D SutherLand多边形裁剪
    static OriginGraph SutherLandHodgmanPolygonClip3D(const OriginGraph& graph,const BoundF& bound){
        OriginGraph tempGraph=graph;
        //对于四个边界进行处理
        for(int i=0;i<4;++i){
            tempGraph=SutherLandHodgmanPolygonClipEdge3D(tempGraph,bound,i);
        }
#if defined(DEBUG)
        std::cout<<"3D cliped graph:"<<tempGraph<<std::endl;
#endif
        return tempGraph;
    }



//*////////////////////////////////////////////
    typedef struct _wapoint{
        Vec2i pos;
        int state;//0为入，1为出
        Vec2i idx;//0为被裁减，1为窗口
        bool used=false;
        bool intersect;//是交点还是多边形上的点

        bool operator==(const _wapoint& p){
            return p.pos==pos&&p.idx==idx&&p.intersect==intersect;
        }

        bool operator!=(const _wapoint&p){
            return !((*this)==p);
        }
    }WAPoint;

    typedef struct{
        double distance;//到起始点的距离
        int idx;//索引
    }WAIndex;

    static void InsertWAPoints(const ScreenGraph& graph,const vector<WAPoint>&intersects,int graphIdx,vector<WAPoint>& culled){
        vector<WAIndex> tempIntersects;//用于存储当前边上的交点
        //遍历每条边，将交点插入被裁减多边形
        for(int idx=0;idx<graph.Size();++idx){
            Vec2i p=graph[idx].Embed<2>(0);
            WAPoint point{};
            point.pos=p;
            point.idx[graphIdx]=idx;
            point.intersect=false;
            point.state=-1;
            culled.push_back(point);

            //找到所有交点
            for(int n=0;n<intersects.size();++n){
                if(intersects[n].idx[graphIdx]==idx){
                    double dis=Distance(p,intersects[n].pos);
                    int index=n;
                    WAIndex temp{};
                    temp.distance=dis;
                    temp.idx=index;
                    tempIntersects.push_back(temp);
                }
            }
            //将交点按距离排序
            std::sort(tempIntersects.begin(),tempIntersects.end(),[](const WAIndex&a,const WAIndex&b){return a.distance<b.distance;});
            for(int n=0;n<tempIntersects.size();++n){
                culled.push_back(intersects[tempIntersects[n].idx]);
            }
            tempIntersects.clear();
        }
    }


    static void InitWAPoints(const ScreenGraph& graph, const ScreenGraph& bound,vector<WAPoint>& culled,vector<WAPoint>& culling){
        vector<WAPoint> intersects;
        //遍历被裁减的每一条边,创建交点序列
        for(int i=0;i<graph.Size();++i){
            int nIdx=(i+1)%graph.Size();
            Vec2i p0=graph[i].Embed<2>(0);
            Vec2i p1=graph[nIdx].Embed<2>(0);

            for(int j=0;j<bound.Size();++j){
                int bnIdx=(j+1)%bound.Size();
                Vec2i q0=bound[j].Embed<2>(0);
                Vec2i q1=bound[bnIdx].Embed<2>(0);
                Vec2i intersect;
                WAPoint temp;
                std::cout<<"start bool intersect:"<<"line 0:"<<p0<<";"<<p1<<";  line 1:"<<q0<<";"<<q1<<";"<<std::endl;
                //如果有相交
                if(Intersect2D(p0,p1,q0,q1,intersect)){
                    //叉乘小于0为出点
                    if((((p1-p0).Embed<3>(0)).Cross((q1-q0).Embed<3>(0))).Z()<0){
                        temp.state=1;
                    }else{
                        temp.state=0;
                    }
                    temp.pos=intersect;
                    temp.intersect=true;
                    temp.idx[0]=i;
                    temp.idx[1]=j;
                    intersects.push_back(temp);
                }
            }
        }
        std::cout<<"create intersects over:"<<intersects.size()<<std::endl;

        //将交点插入graph
        InsertWAPoints(graph,intersects,0,culled);
        //将交点插入裁剪窗口
        InsertWAPoints(bound,intersects,1,culling);
    }


    static void PushWAPoints(int& i,int endState,vector<WAPoint>&array,vector<WAPoint>& out){
        while(array[i].state!=endState){
            out.push_back(array[i]);
            std::cout<<"add:"<<array[i].pos<<std::endl;
            i=(i+1)%array.size();
        }
    }

    //weiler-atherton多边形裁剪
    static void WeilerAthertonPolygonClip2D(const ScreenGraph& graph,const ScreenGraph& bound,std::vector<ScreenGraph>& outGraph){
        ScreenGraph realGraph=graph;
        if(!realGraph.ClockWise()){
            realGraph.Reverse();
        }
        vector<WAPoint> culled;
        vector<WAPoint> culling;
        InitWAPoints(realGraph,bound,culled,culling);
        //如果没有交点
        if(culled.size()==realGraph.Size()){
            //判断在内部还是在外部
            Vec3i tv0=realGraph[0];
            for(int n=0;n<bound.Size();++n) {
                Vec3i tp0 = bound[n];
                Vec3i tp1 = bound[(n+1)%(bound.Size())];
                //叉乘小于0，顺时针，为边界内
                if ((tp0 - tv0).Cross((tp1 - tv0)).Z() > 0) {
                    return;
                }
            }
            outGraph.push_back(realGraph);
            return;
        }

        std::cout<<"culled:"<<std::endl;
        for(int i=0;i<culled.size();++i){
            std::cout<<culled[i].pos<<","<<culled[i].state<<"; ";
        }
        std::cout<<std::endl;
        std::cout<<"culling:"<<std::endl;
        for(int i=0;i<culling.size();++i){
            std::cout<<culling[i].pos<<","<<culling[i].state<<"; ";
        }
        std::cout<<std::endl;

        WAPoint start;
        bool find=false;
        bool first=true;
        vector<WAPoint> result;
        int i=0;


        while(i<culled.size()) {
            //找到起始点
            for (; i < culled.size(); ++i) {
                if (culled[i].intersect && culled[i].state == 0&&!culled[i].used) {
                    start = culled[i];
                    find=true;
                    break;
                }
            }
            if (!find) {
                return;
            }
            find=false;
            first=true;
            do{
                if(!first) {
                    for (int m = 0; m < culled.size(); ++m) {
                        if (culling[i] == culled[m]) {
                            i = m;
                            break;
                        }
                    }
                }
                result.push_back(culled[i]);
                std::cout<<"in:"<<culled[i].pos<<std::endl;
                culled[i].used=true;
                i = (i + 1) % culled.size();
                PushWAPoints(i, 1, culled, result);
                for(int m=0;m<culling.size();++m){
                    if(culled[i]==culling[m]){
                        i=m;
                        break;
                    }
                }
                PushWAPoints(i, 0, culling, result);
                first=false;
            }while(culling[i]!=start);
            ScreenGraph temp;
            for (int j = 0; j < result.size(); ++j) {
                temp.AddVertex(Vec3i(result[j].pos.X(), result[j].pos.Y(), 0));
            }
            result.clear();
            outGraph.push_back(temp);
        }
    }

    static void WeilerAthertonPolygonClip2D(const ScreenGraph& graph,const BoundI& bound,std::vector<ScreenGraph>& outGraph){
        std::cout<<"ori graph:"<<std::endl;
        for(int i=0;i<graph.Size();++i){
            std::cout<<graph[i]<<"; ";
        }
        std::cout<<"end graph"<<std::endl;
        ScreenGraph temp;
        temp.AddVertex(Vec3i(bound.left,bound.top,0));
        temp.AddVertex(Vec3i(bound.right,bound.top,0));
        temp.AddVertex(Vec3i(bound.right,bound.bottom,0));
        temp.AddVertex(Vec3i(bound.left,bound.bottom,0));
        WeilerAthertonPolygonClip2D(graph,temp,outGraph);
    }

//*/////////////////////////////////////////////
    static OriginGraph CullingGraph(const OriginGraph& graph,BoundF bound,double nearFace,double farFace){

    }


}

#endif //GRENDERER_GRAPH_H
