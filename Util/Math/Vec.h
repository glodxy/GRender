//
// Created by 田淙宇 on 2019/9/4.
//

#ifndef GRENDERER_VEC_H
#define GRENDERER_VEC_H

#include <cmath>
#include <initializer_list>
#include <cassert>
#include <iostream>

using std::initializer_list;

namespace GRCore {
    template<size_t size, class Type>
    class Vec {
    public:
        Vec() {
            memset(this->mValue,0, sizeof(Type)* size);
        }

        Vec(void* src):Vec(){
            memcpy(this->mValue,(char*)src, sizeof(Type)*size);
        }

        template <class U>
        Vec(const Vec<size,U>& r):Vec(){
            for(int i=0;i<size;++i){
                mValue[i]=(Type)(r[i]);
            }
        }


        Vec(const Vec<size, Type> &r):Vec() {
            memcpy(this->mValue, (char*)r.mValue, sizeof(Type) * size);
        }

//        ~Vec(){
//            delete[] mValue;
//        }

        Vec<size, Type> &operator=(const Vec<size, Type> &r) {
            memcpy(this->mValue, (char*)r.mValue, sizeof(Type) * size);
            return *this;
        }

        Vec(initializer_list<Type> vList) : Vec() {
            int idx = 0;
            for (auto i = vList.begin(); i != vList.end(); ++i) {
                mValue[idx] = *i;
                ++idx;
            }
        }

        Vec(Type n0, Type n1) : Vec() {
            Init<size>(n0, n1);
        }

        Vec(Type n0, Type n1, Type n2,Type n3=0) : Vec() {
            Init<size>(n0, n1, n2,n3);
        }

        Type X() const {
            return mValue[0];
        }

        Type Y() const {
            assert(size > 1);
            return mValue[1];
        }

        Type Z() const {
            assert(size > 2);
            return mValue[2];
        }

        Type& operator[](int idx){
            assert(idx<size);
            return mValue[idx];
        }

        const Type& operator[](int idx)const{
            assert(idx<size);
            return mValue[idx];
        }

        Vec<size,Type> operator+(const Vec<size, Type> &r)const {
            Vec<size, Type> temp;
            for (int i = 0; i < size; ++i) {
                temp.mValue[i] = (this->mValue)[i] + r.mValue[i];
            }
            return temp;
        }

        Vec<size,Type> operator-(const Vec<size, Type> &r)const{
            Vec<size, Type> temp;
            for (int i = 0; i < size; ++i) {
                temp.mValue[i] = (this->mValue)[i] - r.mValue[i];
            }
            return temp;
        }

        template<class T>
        Vec<size,Type> operator*(T value){
            Vec<size,Type> result;
            for(int i=0;i<size;++i){
                result.mValue[i]=mValue[i]*value;
            }
            return result;
        }

        template<class T>
        Vec<size,Type> operator/(T value){
            Vec<size,Type> result;
            for(int i=0;i<size;++i){
                result.mValue[i]=mValue[i]/value;
            }
            return result;
        }

        bool operator==(const Vec<size,Type>& r)const{
            for(int i=0;i<size;++i){
                if(mValue[i]!=r.mValue[i])
                    return false;
            }
            return true;
        }

        bool operator!=(const Vec<size,Type>& r)const{
            return !((*this)==r);
        }

        template<class T=Type>
        T Dot(const Vec<size, Type> &r) {
            Type result = 0;
            for (int i = 0; i < size; ++i) {
                result += (this->mValue)[i] * r.mValue[i];
            }
            return result;
        }


        Vec<3, Type> Cross(const Vec<3, Type> &r) const{
            assert(size == 3);
            Type x = Y() * r.Z() - Z() * r.Y();
            Type y = Z() * r.X() - X() * r.Z();
            Type z = X() * r.Y() - Y() * r.X();
            return Vec<3, Type>(x, y, z);
        }

        template<size_t nSize>
        Vec<nSize,Type> Embed(Type fill)const{
            Vec<nSize,Type> result;
            for(int i=0;i<nSize;++i){
                result[i]=i>size-1?fill:mValue[i];
            }
            return result;
        }

        Vec<size+1,Type> Embed()const{
            return Embed<size+1>(static_cast<Type>(0));
        }

        Vec<size,Type> Normalized()const{
            Vec<size,Type> result;
            Type sum=0;
            bool zero=true;
            for(int i=0;i<size;++i){
                if(mValue[i]!=0){
                    zero=false;
                    sum+=(mValue[i]*mValue[i]);
                }
            }
            if(zero){
                return nullptr;
            }
            sum=std::sqrt(sum);
            for(int i=0;i<size;++i){
                result.mValue[i]=(mValue[i]/sum);
            }
            return result;
        }

        friend std::ostream &operator<<(std::ostream &os, const Vec<size, Type> &v) {
            for (int i = 0; i < size; ++i) {
                os << v.mValue[i];
                if(i!=size-1)
                    os<<",";
            }
            return os;
        }

    protected:

        template<size_t nSize>
        void Init(Type n0, Type n1) {
            std::cerr << "error when init vec2:size is not 2!";
            exit(0);
        }

        template<size_t nSize>
        void Init(Type n0, Type n1, Type n2,Type n3=0) {
            std::cerr << "error when init vec3:size is not 3!";
            exit(0);
        }

        template<>
        void Init<2>(Type n0, Type n1) {
            mValue[0] = n0;
            mValue[1] = n1;
        }

        template<>
        void Init<3>(Type n0, Type n1, Type n2,Type n3) {
            mValue[0] = n0;
            mValue[1] = n1;
            mValue[2] = n2;
        };

        template<>
        void Init<4>(Type n0, Type n1, Type n2,Type n3) {
            mValue[0] = n0;
            mValue[1] = n1;
            mValue[2] = n2;
            mValue[3]=n3;
        };


    private:
        Type mValue[size];
    };

    typedef Vec<2, float> Vec2f;
    typedef Vec<2, int> Vec2i;
    typedef Vec<3, float> Vec3f;
    typedef Vec<3, int> Vec3i;
    typedef Vec<4,float> Vec4f;
    typedef Vec<4,int> Vec4i;
}
#endif //GRENDERER_VEC_H
