//
// Created by 田淙宇 on 2019/9/26.
//

#ifndef GRENDERER_3DAPP_H
#define GRENDERER_3DAPP_H

#include "Render/Platform/Win/WinApp.h"
#include "Render/Pipeline/TrivalPipeline.h"
#include "Render/Shader/TestShader.h"
#include "Render/Object/obj/ObjLoader.h"
#include "Render/Camera/RoundCamera.h"
#include <DirectXColors.h>

using namespace GRCore;
using GRGraph::Triangle;
using GRGraph::Line;
using GRGraph::Circle;
using GRCore::TrivalPipeline;

GRObject obj;

void TestBuffer(){
    TrivalPipeline pipeline;
    float* temp=new float[100];
    for(int i=0;i<100;++i){temp[i]=i;}
    pipeline.LoadBuffer<float>(temp, sizeof(float),50,0,"vertex");
    delete[] temp;
    Buffer buffer=pipeline.GetBuffer("vertex");
    std::cout<<"test buffer:";
    for(int i=0;i<buffer.count;++i){
        std::cout<<*((float*)buffer.buffer+i)<<" ";
    }
    std::cout<<std::endl;
}

float testv[]{
        0.5,0.8,-0.1,0.5,0.1,-0.1,0.2,0.1,-0.1,0.2,0.8,-0.1,
        0.5,0.8,-0.6,0.5,0.1,-0.6,0.2,0.1,-0.6,0.2,0.8,-0.6
};

int testi[]{
        0,1,2,2,3,0,4,5,6,6,7,4,4,0,3,3,7,4,5,1,2,2,6,5,4,0,1,1,5,4,7,3,2,2,6,7
};

Vec3f testc[]{
        Vec3f(1.0f,0.f,0.f),Vec3f(1.f,0.f,0.f),Vec3f(1.f,0.f,0.0f),Vec3f(1.f,0.f,0.f),
        Vec3f(0.f,1.f,0.f),Vec3f(0.f,1.f,0.f),Vec3f(0.f,1.f,0.f),Vec3f(0.f,1.f,0.f)
};


float vertices[] = {
        // Positions          // Colors           // Texture Coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
};
int indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
};

#ifdef DX
using namespace DirectX;
#include "Render/DX/D3DApp.h"
//class TestApp:public D3DApp{
//public:
//    TestApp(HINSTANCE h):D3DApp(h){}
//    ~TestApp(){
//        delete canvas;
//    }
//
//    virtual bool Initialize()override{
//        if(!D3DApp::Initialize()){
//            return false;
//        }
//        canvas=new GCanvas(mWidth,mHeight,4);
//        return true;
//    }
//
//private:
//    virtual void OnResize()override {
//        D3DApp::OnResize();
//    }
//    virtual void Update(const GameTimer& gt)override{
//
//        canvas->Line(20,20,300,300,Color(0,0,0,255));
//    }
//    virtual void Draw(const GameTimer& gt)override{
//        ThrowIfFailed(mCommandAllocator->Reset());
//
//        ThrowIfFailed(mCommandList->Reset(mCommandAllocator.Get(), nullptr));
//
//        mCommandList->ResourceBarrier(1,&CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),D3D12_RESOURCE_STATE_PRESENT,D3D12_RESOURCE_STATE_RENDER_TARGET));
//        mCommandList->RSSetViewports(1,&mScreenViewPort);
//        mCommandList->RSSetScissorRects(1,&mScissorRect);
//
//
//        mCommandList->ClearRenderTargetView(CurrentBackBufferView(),Colors::LightSteelBlue,0, nullptr);
//        mCommandList->ClearDepthStencilView(DepthStencilView(),D3D12_CLEAR_FLAG_DEPTH|D3D12_CLEAR_FLAG_STENCIL,1.0f,0,0,
//                                            nullptr);
//        mCommandList->OMSetRenderTargets(1,&CurrentBackBufferView(),true,&DepthStencilView());
//
//        mCommandList->ResourceBarrier(1,&CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),D3D12_RESOURCE_STATE_RENDER_TARGET,D3D12_RESOURCE_STATE_PRESENT));
//
//        DrawCanvas(*canvas);
//
//        ThrowIfFailed(mCommandList->Close());
//
//        ID3D12CommandList* cmdLists[]={mCommandList.Get()};
//        mCommandQueue->ExecuteCommandLists(_countof(cmdLists),cmdLists);
//
//        ThrowIfFailed(mSwapChain->Present(0,0));
//        mCurrentBackBuffer=(mCurrentBackBuffer+1)%mSwapChainBufferCount;
//
//        FlushCommandQueue();
//    }
//};
#else
class Win3DApp:public WinApp{
public:
    Win3DApp(HINSTANCE h):WinApp(h){

    }

    ~Win3DApp(){

    }
    bool Initialize(int width=800,int height=600)override{

        if(!WinApp::Initialize(width,height)){
            return false;
        }
        mCamera=new RoundCamera(Vec3f(0,0,0),2.f);
        mCamera->SetPosition(mCamera->Position()+Vec3f(.1f,0,0));
//        mCamera->SetDirection(Vec3f(-1,-1,-3));
//        mCamera->SetPosition(Vec3f(.5,.5,1.5));

        mShader=new TestShader();

        pipeline=new TrivalPipeline();
        std::cout<<"pipeline created"<<std::endl;
        pipeline->LoadBuffer<float>(&obj.Vert(0),sizeof(float),obj.VertCount()*3,0,"vertex");
        pipeline->LoadBuffer<float>(&obj.UV(0), sizeof(float),obj.UVCount()*2,0,"uv");
        pipeline->LoadIndex((int*)&obj.Face(0),3,obj.FaceCount()*3,0,"vertex");
        pipeline->LoadIndex((int*)&obj.Face(0),3,obj.FaceCount()*3,1,"uv");

        pipeline->LoadTexture("african_head/african_head_diffuse.tga","diffuse",true);

//        pipeline->LoadBuffer<float,3>(vertices,sizeof(float)*8,4,0,"vertex");
//        pipeline->LoadBuffer<float,2>(vertices,sizeof(float)*8,4,sizeof(float)*6,"uv");
//        pipeline->LoadIndex(indices,1,6,0,"vertex");
//        pipeline->LoadTexture("container.jpg","container");

//        pipeline->LoadBuffer<float>(testv,sizeof(float),sizeof(testv)/sizeof(float),0,"vertex");
//        pipeline->LoadIndex(testi,1,sizeof(testi)/sizeof(int),0,"vertex");
//        pipeline->LoadBuffer<Vec3f>(testc,sizeof(Vec3f), sizeof(testc)/ sizeof(Vec3f),0,"color");
//        pipeline->LoadIndex(testi,1,sizeof(testi)/sizeof(int),0,"color");
        std::cout<<"pipeline buffer loaded"<<std::endl;
        pipeline->SetVertexBuffer("vertex");
        pipeline->SetIndexBuffer("vertex");
        std::cout<<"set buffer idx suc"<<std::endl;
        pipeline->SetShader(mShader);
        std::cout<<"set shader suc"<<std::endl;

        //pipeline->mLineMode=true;
        mCanvas=new GCanvas(mWidth,mHeight,4);
        mCanvas->Clear();
        pipeline->SetCanvas(mCanvas);
        mShader->SetCamera(mCamera);
        std::cout<<"test app create suc"<<std::endl;
        return true;
    }

    void Update(const GameTimer& gt)override{
        mCanvas->Clear();
        pipeline->ClearZBuffer();
        mShader->Set(mCamera->GetPerspectiveMatrix(),"perspective");
        mShader->Set(mCamera->GetLookAtMatrix(),"lookat");
        mShader->Set(string("diffuse"),"texture0");
        pipeline->ExecuteIndex();
        std::cout<<"update finished"<<std::endl;
#if defined(DEBUG)
        std::cout<<"update finish"<<std::endl;
#endif
    }

    void FixedUpdate()override{
        //mCamera->Rotate(10.f);
        std::cout<<"fixed update called"<<std::endl;
    }

    void Draw(const GameTimer&gt)override{
        DrawCanvas(*mCanvas);
#if defined(DEBUG)
        std::cout<<"draw finish"<<std::endl;
#endif
    }

    void OnKeyDown(WPARAM wParam){
        if(wParam==VK_UP){
            mCamera->SetPosition(mCamera->Position()+Vec3f(0,0,-0.01f));
        }
        if(wParam==VK_DOWN){
            mCamera->SetPosition(mCamera->Position()+Vec3f(0,0,0.01f));
        }
        if(wParam==VK_LEFT){
            mCamera->SetPosition(mCamera->Position()+Vec3f(-.01f,0,0));
        }
        if(wParam==VK_RIGHT){
            mCamera->SetPosition(mCamera->Position()+Vec3f(.01f,0,0));
        }
    }
    void OnKeyUp(WPARAM wParam){

    }

    void OnMouseDown(WPARAM wParam,int x,int y){
        lx=x;
        ly=y;
        clicked=true;
    }
    void OnMouseUp(WPARAM wParam, int x, int y) {
        clicked=false;

    }
    void OnMouseMove(WPARAM wParam, int x, int y) {
        if(clicked){

        }
    }
private:
    GCanvas *mCanvas;
    int lx,ly;
    RoundCamera *mCamera;
    TestShader* mShader;
    TrivalPipeline* pipeline;
    bool clicked=false;
    bool locked=false;
};
#endif

#endif //GRENDERER_3DAPP_H
