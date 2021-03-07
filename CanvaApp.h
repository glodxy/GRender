//
// Created by 田淙宇 on 2019/9/26.
//

#ifndef GRENDERER_CANVAAPP_H
#define GRENDERER_CANVAAPP_H

#include "Render/Platform/Win/WinApp.h"
#include <DirectXColors.h>

using namespace GRCore;
using GRGraph::Triangle;
using GRGraph::Line;
using GRGraph::Circle;

enum PaintMode{
    LINE,CIRCLE,POLYGON,POLYGON_FILL,FILL
};

class TestWinApp:public WinApp{
public:
    TestWinApp(HINSTANCE h):WinApp(h){

    }

    ~TestWinApp(){
        delete mFront;
        delete mCanvas;
        if(tempLine!= nullptr)
            delete tempLine;
    }
    bool Initialize(int width=800,int height=600)override{

        if(!WinApp::Initialize(width,height)){
            return false;
        }
        mCanvas=new GCanvas(mWidth,mHeight,4);
        mCanvas->Clear();
        mFront=new GCanvas(mWidth,mHeight,4);
        mFront->Clear();

        bound.left=50;
        bound.right=mWidth-50;
        bound.bottom=50;
        bound.top=mHeight-50;
        std::cout<<"test app create suc"<<std::endl;
        return true;
    }

    void Update(const GameTimer& gt)override{
        mFront->Clear();
        if(tempLine!= nullptr) {
            tempLine->Draw(*mFront,RED);
        }
        Line a(Vec3i(bound.left,bound.top,0),Vec3i(bound.right,bound.top,0));
        Line b(Vec3i(bound.right,bound.top,0),Vec3i(bound.right,bound.bottom,0));
        Line c(Vec3i(bound.right,bound.bottom,0),Vec3i(bound.left,bound.bottom,0));
        Line d(Vec3i(bound.left,bound.bottom,0),Vec3i(bound.left,bound.top,0));
        a.Draw(*mFront,WHITE);
        b.Draw(*mFront,WHITE);
        c.Draw(*mFront,WHITE);
        d.Draw(*mFront,WHITE);
#if defined(DEBUG)
        std::cout<<"update finish"<<std::endl;
#endif
    }

    void Draw(const GameTimer&gt)override{
        vector<GCanvas> cs;

        cs.push_back(*mCanvas);
        cs.push_back(*mFront);
        DrawCanvas(cs);
#if defined(DEBUG)
        std::cout<<"draw finish"<<std::endl;
#endif
    }

    void OnKeyDown(WPARAM wParam){
        if(wParam==VK_SHIFT){
            locked=true;
        }
        if(wParam==VK_NUMPAD1){
            if(tempLine== nullptr)
            {
                mode=CIRCLE;
                mModeName="cirlce mode";
            }
        }
        if(wParam==VK_NUMPAD0){
            if(tempLine== nullptr){
                mode=LINE;
                mModeName="line mode";
            }

        }
        if(wParam==VK_NUMPAD2){
            if(tempLine== nullptr){
                mode=POLYGON;
                mModeName="polygon no fill";
            }
        }
        if(wParam==VK_NUMPAD3){
            if(tempLine== nullptr){
                mode=POLYGON_FILL;
                mModeName="polygon fill";
            }
        }
        if(wParam==VK_NUMPAD4){
            if(tempLine== nullptr){
                mode=FILL;
                mModeName="fill color";
            }
        }
    }
    void OnKeyUp(WPARAM wParam){
        if(wParam==VK_SHIFT){
            locked=false;
        }
        if(wParam==VK_SPACE){
            if(tempLine!= nullptr&&(mode==POLYGON||mode==POLYGON_FILL)){
                GRGraph::Polygon * p= dynamic_cast<GRGraph::Polygon *>(tempLine);
                p->Finish();
                vector<ScreenGraph> temp;
                std::cout<<"start culling"<<std::endl;
                GRGraph::WeilerAthertonPolygonClip2D(*p,bound,temp);
                bool f=p->Fill();
                std::cout<<"start draw polygons"<<std::endl;
                delete p;
                for(int i=0;i<temp.size();++i) {
                    p=new GRGraph::Polygon(temp[i],f);
                    std::cout<<"start draw polygon:"<<i<<std::endl;
                    for(int m=0;m<p->Size();++m){
                        std::cout<<(*p)[m]<<";";
                    }
                    std::cout<<std::endl<<"end draw polygon"<<i<<std::endl;
                    p->Draw(*mCanvas,RED);
                    delete p;
                }
                tempLine= nullptr;
            }
        }
    }

    void OnMouseDown(WPARAM wParam,int x,int y){
        //左键事件
        if(wParam==MK_LBUTTON) {
            //当当前无操控对象时，创建对象
            if(tempLine== nullptr) {
                lx = x;
                if (mode == CIRCLE) {
                    tempLine = new Circle(x, mHeight - y - 1, 1, 1);
                } else if (mode == LINE) {
                    tempLine = new Line(Vec3i(x, mHeight - 1 - y, 0), Vec3i(x, mHeight - y - 1, 0));
                } else if (mode == POLYGON) {
                    tempLine = new GRGraph::Polygon(Vec2i(x,mHeight-1-y));
                } else if (mode==POLYGON_FILL){
                    tempLine = new GRGraph::Polygon(Vec2i(x,mHeight-1-y),true);
                }else if(mode==FILL){
                    mCanvas->FillBySeedScanLine(x,mHeight-1-y,GREEN);
                }
            }
            else{
                if(mode==POLYGON||mode==POLYGON_FILL){
                    GRGraph::Polygon * p= dynamic_cast<GRGraph::Polygon *>(tempLine);
                    p->PushVertex();
                }
            }
            clicked = true;
        }
        else if(wParam==MK_RBUTTON){
            if(tempLine!= nullptr){
                if(mode==POLYGON||mode==POLYGON_FILL){
                    GRGraph::Polygon * p= dynamic_cast<GRGraph::Polygon *>(tempLine);
                    p->PopVertex();
                }
            }
        }
    }
    void OnMouseUp(WPARAM wParam, int x, int y) {
        clicked=false;
        if(mode==LINE||mode==CIRCLE) {
            if(mode==LINE){
                Line* l= dynamic_cast<Line*>(tempLine);
                Line temp(Vec3i(0,0,0),Vec3i(1,0,0));
                if(GRGraph::CohenSutherLandClipLine2D(*l,bound,temp)) {
                    temp.Draw(*mCanvas, RED);
                }
            }else {
                tempLine->Draw(*mCanvas, RED);
            }
            delete tempLine;
            tempLine = nullptr;
        }
    }
    void OnMouseMove(WPARAM wParam, int x, int y) {
        int ry=mHeight-1-y;
        if((mode==POLYGON||mode==POLYGON_FILL)&&tempLine!=nullptr){
            GRGraph::Polygon * p= dynamic_cast<GRGraph::Polygon *>(tempLine);
            p->SetCurVertex(Vec2i(x,ry));
            return;
        }
        if(clicked&&tempLine!=nullptr){
            if(mode==LINE)
                (*tempLine)[1]=Vec3i(x,mHeight-1-y,0);
            else if(mode==CIRCLE) {

                Circle *c = dynamic_cast<Circle *>(tempLine);
                if(locked){
                    c->SetA((x-c->X())/2);
                    c->SetB((x-c->X())/2);
                }
                else{
                    c->SetA((x-c->X())/2);
                    c->SetB((ry-c->Y())/2);
                }

            }
        }
    }
private:
    GRGraph::BoundI bound;
    GCanvas* mFront,*mCanvas;
    int lx,ly;
    ScreenGraph* tempLine= nullptr;
    bool clicked=false;
    bool locked=false;//锁定比例
    PaintMode mode=CIRCLE;//画笔模式
};
#endif //GRENDERER_CANVAAPP_H
