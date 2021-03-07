//
// Created by 田淙宇 on 2019/9/8.
//

#include "D3DApp.h"
#include <windowsx.h>

LRESULT CALLBACK
MainWndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
    return GRCore::D3DApp::GetApp()->MsgProcess(hWnd,msg,wParam,lParam);
}

GRCore::D3DApp* GRCore::D3DApp::mApp= nullptr;
GRCore::D3DApp* GRCore::D3DApp::GetApp() {
    return mApp;
}

GRCore::D3DApp::D3DApp(HINSTANCE hInstance):mAppInstance(hInstance) {
    assert(mApp== nullptr);
    mApp=this;
}

GRCore::D3DApp::~D3DApp() {
    if(mDevice!= nullptr){
        FlushCommandQueue();
    }
}

HINSTANCE GRCore::D3DApp::AppInstance() const {
    return mAppInstance;
}

HWND GRCore::D3DApp::MainWnd() const {
    return mMainWindow;
}

bool GRCore::D3DApp::Initialize(int width,int height) {
    if(!InitWindow())
        return false;
    if(!InitDirectx())
        return false;

    OnResize();
    return true;
}

LRESULT GRCore::D3DApp::MsgProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg){
        //激活或非激活时
        case WM_ACTIVATE:{
            //激活时启动，否则暂停
            if(LOWORD(wParam)==WA_INACTIVE){
                mAppPaused=true;
                mTimer.Stop();
            }
            else{
                mAppPaused=false;
                mTimer.Start();
            }
            return 0;
        }

        case WM_SIZE:{
            mWidth=LOWORD(lParam);
            mHeight=HIWORD(lParam);
            if(mDevice){
                //最小化时
                if(wParam==SIZE_MINIMIZED){
                    mAppPaused=true;
                    mMinimized=true;
                    mMaxiMized=false;
                }
                //最大化时
                else if(wParam==SIZE_MAXIMIZED){
                    mAppPaused=false;
                    mMinimized=false;
                    mMaxiMized=true;
                    OnResize();
                }
                //重设大小
                else if(wParam==SIZE_RESTORED){
                    //从最小化改变大小
                    if(mMinimized){
                        mAppPaused=false;
                        mMinimized=false;
                        OnResize();
                    }
                    //从最大化变化
                    else if(mMaxiMized){
                        mAppPaused=false;
                        mMaxiMized=false;
                        OnResize();
                    }
                    else if(mResizing){

                    }
                    else{
                        OnResize();
                    }
                }
            }
            return 0;
        }
        //拖动改变窗口大小时
        case WM_ENTERSIZEMOVE:{
            mAppPaused=true;
            mResizing=true;
            mTimer.Stop();
            return 0;
        }
        //停止改变窗口大小时
        case WM_EXITSIZEMOVE:{
            mAppPaused=false;
            mResizing=false;
            mTimer.Start();
            OnResize();
            return 0;
        }
        //窗口关闭时
        case WM_DESTROY:{
            PostQuitMessage(0);
            return 0;
        }
        //用户点击窗口按键时
        case WM_MENUCHAR:{
            return MAKELRESULT(0,MNC_CLOSE);
        }
        //鼠标按键
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:{
            OnMouseDown(wParam,GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
            return 0;
        }
        //鼠标松开
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }
        //鼠标移动
        case WM_MOUSEMOVE: {
            OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }
        //按键
        case WM_KEYUP:{
            return 0;
        }
    }
    return DefWindowProc(hWnd,msg,wParam,lParam);
}

bool GRCore::D3DApp::InitWindow() {
    WNDCLASS wc;
    wc.style=CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc=MainWndProc;
    wc.cbClsExtra=0;
    wc.cbWndExtra=0;
    wc.hInstance=mAppInstance;
    wc.hIcon=LoadIcon(0,IDI_APPLICATION);
    wc.hCursor=LoadCursor(0,IDC_ARROW);
    wc.hbrBackground=(HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszMenuName=0;
    wc.lpszClassName="MainWnd";

    if(!RegisterClass(&wc)){
        MessageBox(0,"Regist class failed",0,0);
        return false;
    }

    RECT rect={0,0,mWidth,mHeight};
    AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,false);
    int width=rect.right-rect.left;
    int height=rect.bottom-rect.top;

    mMainWindow=CreateWindow("MainWnd",mMainWindowCaption.c_str(),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,width,height,0,0,mAppInstance,0);

    if(!mMainWindow){
        MessageBox(0,"Create Window Failed",0,0);
        return false;
    }

    ShowWindow(mMainWindow,SW_SHOW);
    UpdateWindow(mMainWindow);

    return true;
}

//绘制canvas
void GRCore::D3DApp::DrawCanvas(const GRCore::GCanvas &canvas) {
    D3D12_RESOURCE_DESC fbDesc={};
    fbDesc.Dimension=D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    fbDesc.Width=canvas.Width();
    fbDesc.Height=canvas.Height();
    fbDesc.DepthOrArraySize=1;
    fbDesc.MipLevels=1;//
    fbDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
    fbDesc.SampleDesc.Count=1;
    fbDesc.SampleDesc.Quality=0;
    fbDesc.Flags=D3D12_RESOURCE_FLAG_NONE;



    ThrowIfFailed(mDevice->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &fbDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,IID_PPV_ARGS(&texture)
            ));

    const UINT64 upBufferSize=GetRequiredIntermediateSize(texture.Get(),0,1);

    ThrowIfFailed(mDevice->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(upBufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,IID_PPV_ARGS(&textureUpHeap)
            ));

    D3D12_SUBRESOURCE_DATA texData={};
    texData.pData=canvas.Data();
    texData.RowPitch=canvas.Width()*canvas.PixelSize();
    texData.SlicePitch=texData.RowPitch*canvas.Height();

    UpdateSubresources(mCommandList.Get(),texture.Get(),textureUpHeap.Get(),0,0,1,&texData);
    mCommandList->ResourceBarrier(1,&CD3DX12_RESOURCE_BARRIER::Transition(texture.Get(),D3D12_RESOURCE_STATE_COPY_DEST,D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc={};
    srvDesc.Shader4ComponentMapping=D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format=fbDesc.Format;
    srvDesc.ViewDimension=D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels=1;
    mDevice->CreateShaderResourceView(texture.Get(),&srvDesc,mSrvHeap->GetCPUDescriptorHandleForHeapStart());

}

//找到所有适配器
void GRCore::D3DApp::LogAdapters() {
    UINT i=0;
    IDXGIAdapter* adapter= nullptr;
    std::vector<IDXGIAdapter*> adapterList;
    while(mDxgiFactory->EnumAdapters(i,&adapter)!=DXGI_ERROR_NOT_FOUND){
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);

        std::wstring text=L"**Adapter: ";
        text+=desc.Description;
        text+=L"\n";

        OutputDebugStringW(text.c_str());

        adapterList.push_back(adapter);
        ++i;
    }

    for(size_t i=0;i<adapterList.size();++i){
        LogAdapterOutputs(adapterList[i]);
        ReleaseCom(adapterList[i]);
    }
}

void GRCore::D3DApp::LogAdapterOutputs(IDXGIAdapter *adapter) {
    UINT i=0;
    IDXGIOutput*output= nullptr;
    while(adapter->EnumOutputs(i,&output)!=DXGI_ERROR_NOT_FOUND){
        DXGI_OUTPUT_DESC desc;
        output->GetDesc(&desc);

        std::wstring text=L"***Output: ";
        text+=desc.DeviceName;
        text+=L"\n";
        OutputDebugStringW(text.c_str());

        LogOutputDisplayModes(output,DXGI_FORMAT_B8G8R8A8_UNORM);

        ReleaseCom(output);
        ++i;
    }
}

void GRCore::D3DApp::LogOutputDisplayModes(IDXGIOutput *output, DXGI_FORMAT format) {
    UINT count=0;
    UINT flag=0;

    output->GetDisplayModeList(format,flag,&count, nullptr);

    std::vector<DXGI_MODE_DESC> modeList(count);
    output->GetDisplayModeList(format,flag,&count,&modeList[0]);

    for(auto& x : modeList){
        UINT n=x.RefreshRate.Numerator;
        UINT d=x.RefreshRate.Denominator;
        std::wstring text=L"Width="+std::to_wstring(x.Width)+L" Height="+std::to_wstring(x.Height)+L" Refresh="+
                std::to_wstring(n)+L"/"+std::to_wstring(d)+L"\n";
        ::OutputDebugStringW(text.c_str());
    }
}


void GRCore::D3DApp::FlushCommandQueue() {
    mCurrentFence++;

    ThrowIfFailed(mCommandQueue->Signal(mFence.Get(),mCurrentFence));

    if(mFence->GetCompletedValue()<mCurrentFence){
        HANDLE eventHandle=CreateEventEx(nullptr,false,false,EVENT_ALL_ACCESS);

        ThrowIfFailed(mFence->SetEventOnCompletion(mCurrentFence,eventHandle));
        WaitForSingleObject(eventHandle,INFINITE);
        CloseHandle(eventHandle);
    }
}


bool GRCore::D3DApp::InitDirectx() {
#if defined(DEBUG)||defined(_DEBUG)
{
    McComPtr<ID3D12Debug> debugController;
    ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
    debugController->EnableDebugLayer();
}
#endif
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mDxgiFactory)));

    HRESULT hardwareResult=D3D12CreateDevice(nullptr,D3D_FEATURE_LEVEL_11_0,IID_PPV_ARGS(&mDevice));

    if(FAILED(hardwareResult)){
        McComPtr<IDXGIAdapter> pWarpAdapter;
        ThrowIfFailed(mDxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

        ThrowIfFailed(D3D12CreateDevice(pWarpAdapter.Get(),D3D_FEATURE_LEVEL_11_0,IID_PPV_ARGS(&mDevice)));
    }

    ThrowIfFailed(mDevice->CreateFence(0,D3D12_FENCE_FLAG_NONE,IID_PPV_ARGS(&mFence)));
    mRtvDescriptorSize=mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    mDsvDecriptorSize=mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    mCbvSrvUavDescriptorSize=mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

#ifdef _DEBUG
    LogAdapters();
#endif
    CreateCommandObjs();
    CreateSwapChain();
    CreateRtvAndDsvDescriptorHeaps();

    return true;

}

void GRCore::D3DApp::CreateCommandObjs() {
    D3D12_COMMAND_QUEUE_DESC queueDesc={};
    queueDesc.Type=D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags=D3D12_COMMAND_QUEUE_FLAG_NONE;
    ThrowIfFailed(mDevice->CreateCommandQueue(&queueDesc,IID_PPV_ARGS(&mCommandQueue)));
    ThrowIfFailed(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(mCommandAllocator.GetAddressOf())));
    ThrowIfFailed(mDevice->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT,mCommandAllocator.Get(), nullptr,IID_PPV_ARGS(mCommandList.GetAddressOf())));

    mCommandList->Close();
}

void GRCore::D3DApp::CreateSwapChain() {
    mSwapChain.Reset();

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width=mWidth;
    sd.BufferDesc.Height=mHeight;
    sd.BufferDesc.RefreshRate.Numerator=60;
    sd.BufferDesc.RefreshRate.Denominator=1;
    sd.BufferDesc.Format=mBackBufferFormat;
    sd.BufferDesc.ScanlineOrdering=DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling=DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count=1;
    sd.SampleDesc.Quality=0;
    sd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount=mSwapChainBufferCount;
    sd.OutputWindow=mMainWindow;
    sd.Windowed=true;
    sd.SwapEffect=DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags=DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    ThrowIfFailed(mDxgiFactory->CreateSwapChain(mCommandQueue.Get(),&sd,mSwapChain.GetAddressOf()));
}

void GRCore::D3DApp::CreateRtvAndDsvDescriptorHeaps() {
    D3D12_DESCRIPTOR_HEAP_DESC desc;
    desc.NumDescriptors=mSwapChainBufferCount;
    desc.Type=D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags=D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    desc.NodeMask=0;
    ThrowIfFailed(mDevice->CreateDescriptorHeap(&desc,IID_PPV_ARGS(mRtvHeap.GetAddressOf())));

    D3D12_DESCRIPTOR_HEAP_DESC dsvDesc;
    dsvDesc.NumDescriptors=1;
    dsvDesc.Type=D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvDesc.Flags=D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvDesc.NodeMask=0;
    ThrowIfFailed(mDevice->CreateDescriptorHeap(&dsvDesc,IID_PPV_ARGS(mDsvHeap.GetAddressOf())));

    D3D12_DESCRIPTOR_HEAP_DESC srcHeapDesc={};
    srcHeapDesc.NumDescriptors=1;
    srcHeapDesc.Type=D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srcHeapDesc.Flags=D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    ThrowIfFailed(mDevice->CreateDescriptorHeap(&srcHeapDesc,IID_PPV_ARGS(mSrvHeap.GetAddressOf())));
}

//计算帧数
void GRCore::D3DApp::CalculateFrameStats() {
    static int frameCount=0;
    static float timeElapsed=0.0f;

    frameCount++;

    if((mTimer.TotalTime()-timeElapsed)>=1.0f){
        float fps=(float)frameCount;
        float mspf=1000.0f/fps;

        std::string fpsStr=std::to_string(fps);
        std::string mspfStr=std::to_string(mspf);

        std::string title=mMainWindowCaption+"    fps:"+fpsStr+"     mspf:"+mspfStr;

        SetWindowText(mMainWindow,title.c_str());

        frameCount=0;
        timeElapsed+=1.0f;
    }
}


int GRCore::D3DApp::Run() {
    MSG msg={0};

    mTimer.Reset();

    while(msg.message!=WM_QUIT){
        if(PeekMessage(&msg,0,0,0,PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else{
            mTimer.Tick();

            if(!mAppPaused){
                CalculateFrameStats();
                Update(mTimer);
                Draw(mTimer);
            }
            else{
                Sleep(100);
            }
        }
    }
    return (int)msg.wParam;
}


void GRCore::D3DApp::OnResize() {
    assert(mDevice);
    assert(mSwapChain);
    assert(mCommandAllocator);

    FlushCommandQueue();
    ThrowIfFailed(mCommandList->Reset(mCommandAllocator.Get(), nullptr));

    //释放之前的资源
    for(int i=0;i<mSwapChainBufferCount;++i){
        mSwapChainBuffer[i].Reset();
    }
    mDepthStencilBuffer.Reset();
    //调整缓冲区大小并重置
    ThrowIfFailed(mSwapChain->ResizeBuffers(mSwapChainBufferCount,mWidth,mHeight,mBackBufferFormat,DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
    mCurrentBackBuffer=0;

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
    for(UINT i=0;i<mSwapChainBufferCount;++i){
        ThrowIfFailed(mSwapChain->GetBuffer(i,IID_PPV_ARGS(&mSwapChainBuffer[i])));
        mDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr,rtvHeapHandle);
        rtvHeapHandle.Offset(1,mRtvDescriptorSize);
    }
    //重置depth/stencil缓冲区1
    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = mWidth;
    depthStencilDesc.Height = mHeight;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.Format = mDepthStencilFormat;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear;
    optClear.Format=mDepthStencilFormat;
    optClear.DepthStencil.Depth=1.0f;
    optClear.DepthStencil.Stencil=0;
    ThrowIfFailed(mDevice->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &depthStencilDesc,
            D3D12_RESOURCE_STATE_COMMON,
            &optClear,
            IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())));

    mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr,DepthStencilView());

    mCommandList->ResourceBarrier(1,&CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_DEPTH_WRITE));

    ThrowIfFailed(mCommandList->Close());
    ID3D12CommandList* cmdLists[]={mCommandList.Get()};
    mCommandQueue->ExecuteCommandLists(_countof(cmdLists),cmdLists);

    FlushCommandQueue();

    mScreenViewPort.TopLeftX=0;
    mScreenViewPort.TopLeftY=0;
    mScreenViewPort.Width= static_cast<float>(mWidth);
    mScreenViewPort.Height= static_cast<float>(mHeight);
    mScreenViewPort.MinDepth=0.0f;
    mScreenViewPort.MaxDepth=1.0f;

    mScissorRect={0,0,mWidth,mHeight};
}