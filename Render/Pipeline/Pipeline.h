//
// Created by 田淙宇 on 2019/9/19.
//

#ifndef GRENDERER_PIPELINE_H
#define GRENDERER_PIPELINE_H

#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include "../Graph/Graph.h"
#include "../../Util/Math/Vec.h"
#include "../Object/GRObject.h"
#include "../Object/Texture/Texture.h"

using std::vector;
using std::string;
using std::unordered_map;
namespace GRCore {
    class BaseShader;
    struct Buffer{
        void *buffer;
        int count;//数据个数
    };
    class _declspec(dllexport) Pipeline {
    public:
        virtual ~Pipeline();

        virtual void ExecuteVertex() = 0;
        virtual void ExecuteIndex()=0;
        virtual void ResetAll()=0;

        void InitializeForExecute(bool useIndex);

        //检查画布大小变化
        virtual void CheckCanvasSize();

        void SetShader(BaseShader* shader);

        void SetVertexBuffer(string name){
            mVertexBufferID=name;
        }

        void SetIndexBuffer(string name){
            mVertexIdxBufferID=name;
        }

        void SetCanvas(GCanvas*canvas);

        BaseShader* GetShader()const{
            return mShader;
        }

        //按照T类型导入数据
        /*
         * count:每组数据要取出的数目
         * persize:每组数据的数目
         * size:数据个数
         * offset:开始的偏移量
         * id:分配的id
         * */
        template<class T,int count=1>
        void LoadBuffer(void* src,size_t perSize,int size,size_t offset,string name){
            T *dst=new T[size*count];
            for(int i=0;i<size;++i){
                for(int j=0;j<count;++j){
                    dst[i*count+j]=*((T*)((char*)src+offset+(size_t)(perSize*i)+(sizeof(T)*j)));
                    //std::cout<<"read data:"<<dst[i*count+j]<<std::endl;
                }

            }
            Buffer buf{};
            buf.buffer=dst;
            buf.count=size*count;
            mBuffers[name]=buf;
        }

        void LoadTexture(const char*fileName,const string& name,bool flip=false){
            Texture* tex=new Texture(fileName,flip);
            mTextures[name]=tex;
        }
        /*
         * count:每次加入的int数
         * persize:每次取出的int数
         * size:要取出的组数
         * */
        template<size_t count=1>
        void LoadIndex(int* idxData,size_t perSize,int size,size_t offset,string name){
            vector<int> temp;
            for(int i=0;i<size;++i){
                for(int j=0;j<count;++j){
                    temp.push_back(*(idxData+offset+(size_t)(perSize*i)+(size_t)j));
                    //std::cout<<"read data:"<<dst[i*count+j]<<std::endl;
                }
            }
            mIndices[name]=temp;
        }

        Buffer GetBuffer(string name){
            return mBuffers[name];
        }

        //返回第idx个顶点的序列位置
        int GetIndex(const string& name,int idx)const{
            return mIndices.at(name).at(idx);
        }

        Texture* GetTexture(const string& name){
            return mTextures[name];
        }

    protected:
        unordered_map<string,Buffer> mBuffers;
        unordered_map<string,Texture*> mTextures;

        string mVertexBufferID;
        string mVertexIdxBufferID;

        BaseShader *mShader=nullptr;
        GCanvas*mCanvas=nullptr;

        unordered_map<string,vector<int>> mIndices;//存储所有索引的序列
        unordered_map<string,vector<Vec3f>> mTempVertices;//原始顶点序列(用于渲染过程中)

        virtual void OnCanvasChanged(int ow,int oh,int nw,int nh){}
        int mWidth=0,mHeight=0;
    };
}



#endif //GRENDERER_PIPELINE_H
