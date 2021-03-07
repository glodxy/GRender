//
// Created by 田淙宇 on 2019/9/18.
//

#ifndef GRENDERER_GROBJECT_H
#define GRENDERER_GROBJECT_H

#include <vector>
#include "../../Util/Math/Matrix.h"
#include "../GCanvas.h"

using std::vector;
namespace GRCore {

    typedef Vec3i FaceType;

    class GRObject {
    public:

        int VertCount()const{
            return mVertices.size();
        }

        int FaceCount()const{
            return mFaceCount;
        }

        int NormCount()const{
            return mNormals.size();
        }

        int UVCount()const{
            return mUVs.size();
        }

        Vec3f& Vert(int idx){
            return mVertices[idx];
        }

        const Vec3f& Vert(int idx)const{
            return mVertices[idx];
        }

        FaceType& Face(int idx){
            return mFaces[idx];
        }

        const FaceType& Face(int idx)const{
            return mFaces[idx];
        }

        Vec2f& UV(int idx){
            return mUVs[idx];
        }

        const Vec2f& UV(int idx)const{
            return mUVs[idx];
        }

        void AddVert(const Vec3f& v){
            mVertices.push_back(v);
        }

        void AddFace(const FaceType& f){
            mFaces.push_back(f);
        }

        void AddNorm(const Vec3f& n){
            mNormals.push_back(n);
        }

        void AddUv(const Vec2f& u){
            mUVs.push_back(u);
        }



        virtual void Draw(GCanvas&canvas);



        int mFaceCount=0;//面数
    protected:
        vector<Vec3f> mVertices;//顶点序列
        vector<FaceType> mFaces;//面序列
        vector<Vec3f> mNormals;
        vector<Vec2f> mUVs;

        Matrix4f mCurMatrix;
    private:

    };

}

#endif //GRENDERER_GROBJECT_H
