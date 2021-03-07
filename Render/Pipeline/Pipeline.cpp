//
// Created by 田淙宇 on 2019/9/21.
//

#include "Pipeline.h"
#include "../Shader/BaseShader.h"

using std::map;
void Pipeline::SetShader(BaseShader* shader){
    mShader=shader;
    mShader->pipeline=this;
}


void Pipeline::CheckCanvasSize() {
    if(mCanvas->Width()!=mWidth||mCanvas->Height()!=mHeight){
        OnCanvasChanged(mWidth,mHeight,mCanvas->Width(),mCanvas->Height());
    }
}

void Pipeline::SetCanvas(GCanvas *canvas) {
    GCanvas* o=mCanvas;
    mCanvas=canvas;
    int ow=0,oh=0,nw=0,nh=0;
    if(o!=nullptr){
        ow=o->Width();
        oh=o->Height();
    }
    if(canvas!=nullptr){
        nw=canvas->Width();
        nh=canvas->Height();
    }
    OnCanvasChanged(ow,oh,nw,nh);
}

void Pipeline::InitializeForExecute(bool useIndex) {
    assert(mShader!=nullptr&&mCanvas!=nullptr);

    map<string,vector<int>> tempIndices;
    for(auto i=mBuffers.begin();i!=mBuffers.end();++i){
        string name=(i->first);
        //不存在该索引
        //则创建索引
        if(mIndices.find(name)==mIndices.end()){
            vector<int> temp;
            tempIndices[name]=temp;
            std::cout<<"create index:"<<name<<std::endl;
        }
    }

    Buffer vertex=mBuffers[mVertexBufferID];
    vector<Vec3f> tempVertices;
    std::cout<<"get buffer suc"<<std::endl;

    int size=vertex.count/3;//顶点个数
    int vertexCount=useIndex?mIndices[mVertexIdxBufferID].size():size;
    //加载三角形顶点
    for(size_t i=0;i<vertexCount;++i){
        int temp=useIndex?mIndices[mVertexIdxBufferID][i]:i;
        //为新建的索引赋值
        for(auto iter=tempIndices.begin();iter!=tempIndices.end();++iter){
            iter->second.push_back(temp);
        }
        //std::cout<<"index "<<i<<":"<<temp<<std::endl;
        if(temp>=size){
            std::cerr<<"error load vertex index:"<<temp<<", it's too large"<<std::endl;
            exit(0);
        }
        Vec3f tempVertex((float*)vertex.buffer+3*temp);
        tempVertices.push_back(tempVertex);
    }
    mTempVertices[mVertexBufferID]=tempVertices;
    std::cout<<"load indices from buffer suc"<<std::endl;
    //将新建的索引写入
    for(auto iter=tempIndices.begin();iter!=tempIndices.end();++iter){
        mIndices[iter->first]=iter->second;
        std::cout<<"write index:"<<iter->first<<std::endl;
    }
    std::cout<<"create new index suc"<<std::endl;
    std::cout<<"initialize for execution suc"<<std::endl;
}

Pipeline::~Pipeline() {
    for(auto iter=mBuffers.begin();iter!=mBuffers.end();++iter){
        delete[] iter->second.buffer;
    }
    mBuffers.clear();
}
