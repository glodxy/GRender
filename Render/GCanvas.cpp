//
// Created by 田淙宇 on 2019/9/4.
//


#include <cassert>
#include <stack>
#include "GCanvas.h"
using GRCore::GCanvas;
using namespace GRGraph;

GCanvas::GCanvas(int w, int h, int pixelSize):mWidth(w),mHeight(h),mPixelSize(pixelSize) {
    mBuffer=new uint32_t[mWidth*mHeight];
}


//bresenham line
void GCanvas::BresenhamLine(int x0, int y0, int x1, int y1, GRCore::Color color) {
    bool swap=false;//代表是否交换x和y
    //选择变化较大的一向作为遍历向
    if(std::abs(x0-x1)<std::abs(y0-y1)){
        std::swap(x0,y0);
        std::swap(x1,y1);
        swap=true;
    }
    //选择较小的为遍历起点
    if(x0>x1){
        std::swap(x0,x1);
        std::swap(y0,y1);
    }

    int dx=x1-x0;
    int dy=y1-y0;
    int i=dy>0?1:-1;//根据dy判断该直线是上升还是下降
    int step=std::abs(dy)*2;//每次check所添加的数值
    int check=0;//判断是否上升
    int y=y0;

    //开始遍历
    for(int x=x0;x<=x1;++x){
        if(swap){
            SetPixel(y,x,color);
        }
        else{
            SetPixel(x,y,color);
        }

        check+=step;
        if(check>dx){
            y+=i;
            check-=2*dx;
        }
    }
}

void GCanvas::DDALine(int x0, int y0, int x1, int y1, Color color) {
    bool swap=false;//代表是否交换x和y
    //选择变化较大的一向作为遍历向
    if(std::abs(x0-x1)<std::abs(y0-y1)){
        std::swap(x0,y0);
        std::swap(x1,y1);
        swap=true;
    }
    //选择较小的为遍历起点
    if(x0>x1){
        std::swap(x0,x1);
        std::swap(y0,y1);
    }

    int dx=x1-x0;
    int dy=y1-y0;
    int i=dy>0?1:-1;//根据dy判断该直线是上升还是下降
    float y=y0;
    float k=(float)dy/dx;

    for(int x=x0;x<=x1;++x){
        if(swap){
            SetPixel((int)(y+0.5),x,color);
        }
        else{
            SetPixel(x,(int)(y+0.5),color);
        }
        y+=k;
    }
}

void GCanvas::MidPointLine(int x0, int y0, int x1, int y1, Color color) {
    bool swap=false;//代表是否交换x和y
    //选择变化较大的一向作为遍历向
    if(std::abs(x0-x1)<std::abs(y0-y1)){
        std::swap(x0,y0);
        std::swap(x1,y1);
        swap=true;
    }
    //选择较小的为遍历起点
    if(x0>x1){
        std::swap(x0,x1);
        std::swap(y0,y1);
    }

    int a=-std::abs(y0-y1);
    int b=std::abs(x1-x0);
    int step=2*a;
    int check=2*a+b;
    int y=y0;
    int i=(y0-y1)>0?-1:1;


    for(int x=x0;x<=x1;++x){
#if defined(DEBUG)
        std::cout<<"x0:"<<x<<", y0:"<<y<<std::endl;
#endif
        if(swap){
            SetPixel(y,x,color);
        }
        else{
            SetPixel(x,y,color);
        }
        check+=step;
        if(check<0){
            y+=i;
            check+=2*b;
        }
    }
}

void GCanvas::Circle(int x0, int y0, int radius, GRCore::Color color) {
    int p=1-radius;

    for(int x=0,y=radius;x<=y;++x){
        //画八个方向的点
        CirclePlotPoint(x,y,x0,y0,color);

        p+=2*x+3;
        if(p>0){
            --y;
            p-=2*y;
        }
    }
}

void GCanvas::BresenhamCircle(int x0, int y0, int radius, Color color) {
    int p0=2*(1-radius);//代表右下点的判断

    for(int x=0,y=radius;x<=y;){
        CirclePlotPoint(x,y,x0,y0,color);

        if(p0<0){
            if(2*(p0+y)-1<0){
                ++x;
                p0+=(x+x+3);
                continue;
            }
        }
        else if(p0>0){
            if(2*(p0-x)-1>0){
                --y;
                p0+=3-y-y;
                continue;
            }
        }
        ++x;
        --y;
        p0+=x+x-y-y+6;
    }
}

void GCanvas::Ellipse(int x0, int y0, int rx, int ry, GRCore::Color color) {
    bool swap=false;
    int a=rx;
    int b=ry;
    if(rx>ry){
        std::swap(a,b);
        swap=true;
    }

    int p=std::round(b*b+a*a*0.25-a*a*b);
    int m=2*b*b;//2*b*b*(xi+1)
    int n=2*a*a*(b-1);//2*a*a*(yi-1)

    int x=0;
    int y=b;

    //区域1,即斜率小于1：2*x/(a*a)<2*y/(b*b)
    for(x=0;m-2*a*a<n+2*b*b;++x){
        if(swap){
            EllipsePlotPoint(y,x,x0,y0,color);
        }
        else{
            EllipsePlotPoint(x,y,x0,y0,color);
        }

        p+=m;
        m+=2*b*b;
        if(p>=0){
            --y;
            p-=n;
            n-=2*a*a;
        }
    }
    //区域2
    p=std::round(b*b*(x+0.5)*(x+0.5)+a*a*(y-1)*(y-1)-a*a*b*b);
    m=-(a*a*(2*y-3));
    n=2*b*b*(x+1);
    for(;y>=0;--y){
        if(swap){
            EllipsePlotPoint(y,x,x0,y0,color);
        }
        else{
            EllipsePlotPoint(x,y,x0,y0,color);
        }

        p+=m;
        m+=2*a*a;
        if(p<=0){
            ++x;
            p+=n;
            n+=2*b*b;
        }

    }
}
//*//////////////////////////////种子填充

void GCanvas::FillBySeedScanLine(int x0, int y0, Color color) {
    uint32_t ori=GetPixel(GetPixelIdx(x0,y0));
    std::stack<Vec2i> points;
    points.push(Vec2i(x0,y0));
    Vec2i temp;
    while(!points.empty()) {
        temp=points.top();
        std::cout<<"get seed:"<<temp<<std::endl;
        points.pop();
        Vec2i left;
        Vec2i right;
        ScanFill(temp, left, right, color, ori);
        std::cout<<"left:"<<left<<";right:"<<right<<std::endl;
        if(left.Y()+1<mHeight){
            SearchNextSeed(left.X(),right.X(),left.Y()+1,ori,points);
        }
        if(left.Y()-1>=0){
            SearchNextSeed(left.X(),right.X(),left.Y()-1,ori,points);
        }

    }
}

void GCanvas::ScanFill(const Vec2i &start, Vec2i &left, Vec2i &right,const Color& color,uint32_t oriColor) {
    int idx;
    for(idx=start.X();idx<mWidth&&GetPixel(GetPixelIdx(idx,start.Y()))==oriColor;++idx){
        SetPixel(idx,start.Y(),color);
    }
    right=Vec2i(idx-1,start.Y());
    for(idx=start.X()-1;idx>=0&&GetPixel(GetPixelIdx(idx,start.Y()))==oriColor;--idx){
        SetPixel(idx,start.Y(),color);
    }
    left=Vec2i(idx+1,start.Y());
}

void GCanvas::SearchNextSeed(int l, int r, int y, uint32_t oriColor, std::stack<Vec2i> &s) {
    bool needFill=false;
    int x=l;
    while (x<r)
    {
        needFill = false;
        while (x<mWidth-1&&GetPixel(GetPixelIdx(x, y)) == oriColor)
        {
            needFill = true;
            x++;
        }
        if (needFill)
        {
            s.push(Vec2i(x-1,y));
            needFill = false;
        }
        while (GetPixel(GetPixelIdx(x, y)) != oriColor  && x<r)
        {
            x++;
        }
    }
}

//*//////////////////////////////////////////////扫描线填充
void GCanvas::DrawPolyGon(const GRGraph::ScreenGraph &graph,GRCore::Color color) {
    BoundI b={0,mHeight,0,mWidth};
    ScreenGraph t=SutherLandHodgmanPolygonClip2D(graph,b);
    BoundI bound=GRGraph::FindBound2D(t);
    std::vector<std::list<Edge>> NET(bound.top-bound.bottom+1);
    std::list<Edge> AET;
#if defined(DEBUG)
    std::cout<<"top:"<<bound.top<<" bot:"<<bound.bottom<<" l:"<<bound.left<<" r:"<<bound.right<<std::endl;
#endif
    InitNET(NET,t,bound.bottom);

    for(int i=bound.bottom;i<=bound.top;++i){
        InsertAET(AET,NET[i-bound.bottom]);
        ExecuteAET(AET,i,color);
        RemoveAET(AET,i);
        UpdateAET(AET);
    }

}

void GCanvas::InitNET(std::vector<std::list<Edge>> &NET, const GRGraph::ScreenGraph &g, int ymin) {
    for(int i=0;i<g.Size();++i){
        Vec3i start=g[i];//边的起点
        Vec3i end=i==g.Size()-1?g[0]:g[i+1];//边的终点
#if defined(DEBUG)
        std::cout<<" line "<<i<<": start:"<<start<<" end:"<<end<<std::endl;
#endif
        //如果不是水平边,则加入边表
        if(start.Y()!=end.Y()){
            //斜率倒数
            double dx=((double)end.X()-start.X())/(end.Y()-start.Y());
            int maxY;
            int minY;
            //低处的x
            double lowX;
#if defined(DEBUG)
            std::cout<<"line "<<i<<" calc dx suc:"<<dx<<std::endl;
#endif
            if(start.Y()>end.Y()){
                maxY=g.SideVertex(i)?start.Y()-1:start.Y();
                minY=end.Y();
                lowX=end.X();
            }
            else{
                maxY=g.SideVertex(i+1)?end.Y()-1:end.Y();
                minY=start.Y();
                lowX=start.X();
            }
#if defined(DEBUG)
            std::cout<<"line "<<i<<" calc maxy&x suc:"<<dx<<std::endl;
#endif
            Edge temp{lowX,dx,maxY};
            NET[minY-ymin].push_back(temp);
#if defined(DEBUG)
            std::cout<<"line "<<i<<" : x:"<<lowX<<" dx:"<<dx<<" maxY:"<<maxY<<std::endl<<std::endl;
#endif
        };
    }
#if defined(DEBUG)
    std::cout<<" init NET suc"<<std::endl;
#endif
}

void GCanvas::InsertAET(std::list<Edge> &AET, const std::list<Edge> &srcET) {
    if(!srcET.empty()){
        for(auto iter=srcET.begin();iter!=srcET.end();++iter){
            AET.push_back(*iter);
        }
        AET.sort([](const Edge& a,const Edge& b){return (a.x<b.x)||(a.x==b.x&&a.dx<=b.dx);});
    }
}

void GCanvas::ExecuteAET(std::list<Edge> &AET, int y, GRCore::Color color) {
    for(auto iter=AET.begin();iter!=AET.end();++iter){
        int x0=(*iter).x;
        ++iter;
        int x1=(*iter).x;
        SetPixelRow(x0,x1,y,color);
    }
}

void GCanvas::RemoveAET(std::list<Edge> &AET, int y) {
    for(auto iter=AET.begin();iter!=AET.end();){
        if((*iter).ymax==y)
            AET.erase(iter++);
        else
            ++iter;
    }
}

void GCanvas::UpdateAET(std::list<Edge> &AET) {
    for(auto iter=AET.begin();iter!=AET.end();++iter){
        (*iter).x+=(*iter).dx;
    }
}
//*///////////////////////////////
