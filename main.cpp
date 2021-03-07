#include <iostream>
#include "CanvaApp.h"
#include "3DApp.h"


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prevInstance,PSTR cmdLine,int showCmd){
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
    freopen("debug.log","w",stdout);
    std::cout<<"size of vec3f:"<< sizeof(Vec3f)<<" size of float:"<< sizeof(float)<<std::endl;
#endif
    freopen("debug.err","w",stderr);

#ifdef DX
    try{
#endif
        Vec2i result;
        //GRGraph::Intersect2D(Vec2i(100,500),Vec2i(500,500),Vec2i(150,250),Vec2i(140,600),result);
        //std::cout<<"intersect:"<<result<<std::endl;
        LoadObj("african_head/african_head.obj",obj);
        Win3DApp app(hInstance);
        //TestBuffer();
        if(!app.Initialize())
            return 0;
        return app.Run();
#ifdef DX
    }
    catch(DxException &  e){
        MessageBox(nullptr,WStringToAnsi(e.ToString().c_str()).c_str(),"Failed",MB_OK);
        return 0;
    }
#endif
}