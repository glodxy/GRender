//
// Created by 田淙宇 on 2019/9/8.
//

#ifndef GRENDERER_D3DAPP_H
#define GRENDERER_D3DAPP_H

#include "../../GApp.h"
#include "D3DUtil.h"
#include "../../../Util/Timer/GTimer.h"




namespace GRCore {
    typedef GRCore::GTimer GameTimer;

    class D3DApp:public GApp {

    protected:
        D3DApp(HINSTANCE hInstance);

        D3DApp(const D3DApp &) = delete;

        D3DApp &operator=(const D3DApp &) = delete;

        virtual ~D3DApp();

    public:
        static D3DApp *GetApp();

        HINSTANCE AppInstance() const;

        HWND MainWnd() const;

        int Run()override;

        bool Initialize(int width=800,int height=600)override ;

        virtual LRESULT MsgProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    protected:
        void DrawCanvas(const GCanvas& canvas)override;

        virtual void CreateRtvAndDsvDescriptorHeaps();

        void OnResize()override;

        //鼠标处理
        virtual void OnMouseDown(WPARAM wParam, int x, int y) {}

        virtual void OnMouseUp(WPARAM wParam, int x, int y) {}

        virtual void OnMouseMove(WPARAM wParam, int x, int y) {}

    protected:
        bool InitWindow();

        bool InitDirectx();

        void CreateCommandObjs();

        void CreateSwapChain();

        void FlushCommandQueue();

        ID3D12Resource *CurrentBackBuffer() const {
            return mSwapChainBuffer[mCurrentBackBuffer].Get();
        }

        D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const {
            return CD3DX12_CPU_DESCRIPTOR_HANDLE(mRtvHeap->GetCPUDescriptorHandleForHeapStart(), mCurrentBackBuffer,
                                                 mRtvDescriptorSize);
        }

        D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const {
            return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
        }

        void CalculateFrameStats()override;

        void LogAdapters();

        void LogAdapterOutputs(IDXGIAdapter *adapter);

        void LogOutputDisplayModes(IDXGIOutput *output, DXGI_FORMAT format);

    protected:
        static D3DApp *mApp;

        HINSTANCE mAppInstance = nullptr;
        HWND mMainWindow = nullptr;
        template<class T>
        using McComPtr= Microsoft::WRL::ComPtr<T>;

        McComPtr<IDXGIFactory4> mDxgiFactory;
        McComPtr<IDXGISwapChain> mSwapChain;
        McComPtr<ID3D12Device> mDevice;

        McComPtr<ID3D12Fence> mFence;
        UINT64 mCurrentFence = 0;

        McComPtr<ID3D12CommandQueue> mCommandQueue;
        McComPtr<ID3D12CommandAllocator> mCommandAllocator;
        McComPtr<ID3D12GraphicsCommandList> mCommandList;

        static const int mSwapChainBufferCount = 2;
        int mCurrentBackBuffer = 0;
        McComPtr<ID3D12Resource> mSwapChainBuffer[mSwapChainBufferCount];
        McComPtr<ID3D12Resource> mDepthStencilBuffer;

        McComPtr<ID3D12DescriptorHeap> mRtvHeap;
        McComPtr<ID3D12DescriptorHeap> mDsvHeap;
        McComPtr<ID3D12DescriptorHeap> mSrvHeap;

        McComPtr<ID3D12Resource> texture;
        McComPtr<ID3D12Resource> textureUpHeap;

        D3D12_VIEWPORT mScreenViewPort;
        D3D12_RECT mScissorRect;

        UINT mRtvDescriptorSize = 0;
        UINT mDsvDecriptorSize = 0;
        UINT mCbvSrvUavDescriptorSize = 0;

    protected:
        //继承类修改值
        D3D_DRIVER_TYPE mDriverType = D3D_DRIVER_TYPE_HARDWARE;
        DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


    };
}


#endif //GRENDERER_D3DAPP_H
