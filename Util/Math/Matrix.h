//
// Created by 田淙宇 on 2019/9/11.
//

#ifndef GRENDERER_MATRIX_H
#define GRENDERER_MATRIX_H


#include "Vec.h"

namespace GRCore{

    template <size_t row,size_t col,class Type>
    class Matrix{
    public:
        Matrix(){}

        Vec<col,Type>&operator[](int idx){
            assert(idx<row);
            return mValue[idx];
        }

        const Vec<col,Type>&operator[](int idx)const{
            assert(idx<row);
            return mValue[idx];
        }

        Vec<row,Type>& Col(int idx){
            assert(idx<col);
            Vec<row,Type> temp;
            for(int i=0;i<row;++i){
                temp[i]=mValue[i][idx];
            }
            return temp;
        }

        const Vec<row,Type>& Col(int idx)const{
            assert(idx<col);
            Vec<row,Type> temp;
            for(int i=0;i<row;++i){
                temp[i]=mValue[i][idx];
            }
            return temp;
        }

        void SetCol(Vec<row,Type> v,int idx){
            assert(idx<col);
            for(int i=0;i<row;++i){
                mValue[i][idx]=v[i];
            }
        }

        void SetRow(Vec<col,Type> v,int idx){
            assert(idx<row);
            mValue[idx]=v;
        }

        template<size_t nCol>
        Matrix<row,nCol,Type> operator*(const Matrix<col,nCol,Type>& m){
            Matrix<row,nCol,Type> result;
            for(int i=0;i<row;++i){
                for(int j=0;j<nCol;++j){
                    result[i][j]=(mValue[i]).Dot(m.Col(j));
                }
            }
            return result;
        }

        Vec<row,Type> operator*(const Vec<col,Type>& v){
            Vec<row,Type>result;
            for(int i=0;i<row;++i){
                result[i]=mValue[i].Dot(v);
            }
            return result;
        }

        bool operator==(const Matrix<row,col,Type>&r)const{
            for(int i=0;i<row;++i){
                if(mValue[i]!=r.mValue[i])
                    return false;
            }
            return true;
        }

        bool operator!=(const Matrix<row,col,Type>&r)const{
            return !((*this)==r);
        }

        friend std::ostream&operator<<(std::ostream& os,const Matrix<row,col,Type>& mat){
            for(int i=0;i<row;++i){
                os<<mat[i]<<std::endl;
            }
            return os;
        }

        static Matrix<row,col,Type> Identity(){
            assert(row==col);
            Matrix<row,col,Type> result;
            for(int i=0;i<row;++i){
                for(int j=0;j<col;++j){
                    if(i==j)
                        result[i][j]=1;
                    else
                        result[i][j]=0;
                }
            }
            return result;
        }

    private:
        Vec<col,Type>mValue[row];


    };

    typedef Matrix<4,4,float> Matrix4f;
    typedef Matrix<3,3,float> Matrix3f;
    typedef Matrix<4,4,int> Matrix4i;
    typedef Matrix<3,3,int> Matrix3i;

    static Matrix4f Translate(const Vec3f& t){
        Matrix4f tran=Matrix4f::Identity();
        Vec4f temp=t.Embed<4>(1);
        tran.SetCol(temp,3);
        return tran;
    }

    static Matrix4f Rotate(const Vec3f& axis,float angle){
        Matrix4f result=Matrix4f::Identity();
        float c=cos(angle);
        float s=sin(angle);

        result.SetRow(Vec4f({c+(1-c)*axis.X()*axis.X(),(1-c)*axis.X()*axis.Y()-s*axis.Z(),(1-c)*axis.X()*axis.Z()+s*axis.Y(),0}),0);
        result.SetRow(Vec4f({(1-c)*axis.X()*axis.Y()+s*axis.Z(),c+(1-c)*axis.Y()*axis.Y(),(1-c)*axis.Y()*axis.Z()-s*axis.X(),0}),1);
        result.SetRow(Vec4f({(1-c)*axis.X()*axis.Z()-s*axis.Y(),(1-c)*axis.Y()*axis.Z()+s*axis.X(),c+(1-c)*axis.Z()*axis.Z(),0}),2);
        return result;
    }


    //视口矩阵
    static Matrix4f Lookat(){}
    //投影矩阵
    static Matrix4f Perspective(int angle,float wh,float n,float f){
//        float r,l,t,b;
//        r=(std::abs(n)*(std::tan((float)angle/2)));
//        l=-r;
//        t=r/wh;
//        b=-t;
//
//        Matrix4f result;
//        result.SetRow(Vec4f({2*n/(r-l),0.0f,(r+l)/(r-l),0.0f}),0);
//        result.SetRow(Vec4f({0.0f,2*n/(t-b),(t+b)/(t-b),0.0f}),1);
//        result.SetRow(Vec4f({0.0f,0.0f,-(f+n)/(f-n),-2*n*f/(f-n)}),2);
//        result.SetRow(Vec4f({0.0f,0.0f,-1.0f,0.0f}),3);
//        return result;
        const float tanHalfFovy = tan(angle / 2.f);

        Matrix4f rst;
        rst[0][0] = 1.f / (wh * tanHalfFovy);
        rst[1][1] = 1.f / (tanHalfFovy);
        rst[2][2] = -(f + n) / (f - n);
        rst[3][2] = -1.f;
        rst[2][3] = -(2.f * f * n) / (f - n);

        return rst;
    }

    //投影矩阵
    static Matrix4f Perspective(float w,float h,float n,float f){
        float r=w/2;
        float l=-r;
        float t=h/2;
        float b=-t;
        n=-n;
        f=-f;

        Matrix4f result;
        result.SetRow(Vec4f({2*n/(r-l),0.0f,(r+l)/(r-l),0.0f}),0);
        result.SetRow(Vec4f({0.0f,2*n/(t-b),(t+b)/(t-b),0.0f}),1);
        result.SetRow(Vec4f({0.0f,0.0f,(f+n)/(n-f),2*n*f/(n-f)}),2);
        result.SetRow(Vec4f({0.0f,0.0f,-1.0f,0.0f}),3);
        //std::cout<<"perspective mat:"<<result<<std::endl;
        return result;
    }

    static Matrix4f Projection(float coeff) {
        Matrix4f Projection = Matrix4f::Identity();
        Projection[3][2] = coeff;
        return Projection;
    }

    //llokat矩阵
    static Matrix4f Lookat(const Vec3f& eye,const Vec3f& center,const Vec3f& up) {
        Vec3f z = (eye-center).Normalized();
        Vec3f x = up.Cross(z).Normalized();
        Vec3f y = z.Cross(x).Normalized();
        Matrix4f Minv = Matrix4f::Identity();
        Matrix4f Tr   = Matrix4f::Identity();
        for (int i=0; i<3; i++) {
            Minv[0][i] = x[i];
            Minv[1][i] = y[i];
            Minv[2][i] = z[i];
            Tr[i][3] = -center[i];
        }
        return Minv*Tr;
    }


}
#endif //GRENDERER_MATRIX_H
