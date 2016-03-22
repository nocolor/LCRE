//
//  type_vec4_inl.hpp
//  qyt_fundation
//
//  Created by qyt on 16/2/17.
//  Copyright © 2016年 qyt. All rights reserved.
//

#ifndef type_vec4_inl_h
#define type_vec4_inl_h

namespace qyt
{
    namespace __detail
    {
        template <typename T>
        size_t __tvec4<T>::size() const
        {
            return 4;
        }
        
        template <typename T>
        T& __tvec4<T>::operator[](size_t i)
        {
            assert(i >= 0 && i < this->size());
            return (&x)[i];
        }
        
        template <typename T>
        const T& __tvec4<T>::operator[](size_t i) const
        {
            assert(i >= 0 && i < this->size());
            return (&x)[i];
        }
        
        template <typename T>
        __tvec4<T>::__tvec4():
        x(0), y(0), z(0), w(0)
        {}
        
        template <typename T>
        __tvec4<T>::__tvec4(const __tvec4<T>& v):
        x(v.x), y(v.y), z(v.z), w(v.w)
        {}
        
        template <typename T>
        __tvec4<T>::__tvec4(T s):
        x(static_cast<T>(s)),
        y(static_cast<T>(s)),
        z(static_cast<T>(s)),
        w(static_cast<T>(s))
        {}
        
        template <typename T>
        __tvec4<T>::__tvec4(typename __simd<T>::__type v):
        x(static_cast<T>(v[0])),
        y(static_cast<T>(v[1])),
        z(static_cast<T>(v[2])),
        w(static_cast<T>(v[3]))
        {}
        
        
        template <typename T>
        __tvec4<T>::__tvec4(T a, T b, T c, T d):
        x(static_cast<T>(a)),
        y(static_cast<T>(b)),
        z(static_cast<T>(c)),
        w(static_cast<T>(d))
        {}
        
        template <typename T>
        template <typename A, typename B, typename C, typename D>
        __tvec4<T>::__tvec4(A a, B b, C c, D d):
        x(static_cast<T>(a)),
        y(static_cast<T>(b)),
        z(static_cast<T>(c)),
        w(static_cast<T>(d))
        {}
        
        template <typename T>
        template <typename A, typename B, typename C>
        __tvec4<T>::__tvec4(const __tvec2<A>& a, B b, C c):
        x(static_cast<T>(a.x)),
        y(static_cast<T>(a.y)),
        z(static_cast<T>(b)),
        w(static_cast<T>(c))
        {}
        
        template <typename T>
        template <typename A, typename B, typename C>
        __tvec4<T>::__tvec4(A a, const __tvec2<B>& b, C c):
        x(static_cast<T>(a)),
        y(static_cast<T>(b.x)),
        z(static_cast<T>(b.y)),
        w(static_cast<T>(c))
        {}
        
        template <typename T>
        template <typename A, typename B, typename C>
        __tvec4<T>::__tvec4(A a, B b, const __tvec2<C>& c):
        x(static_cast<T>(a)),
        y(static_cast<T>(b)),
        z(static_cast<T>(c.x)),
        w(static_cast<T>(c,y))
        {}
        
        template <typename T>
        template <typename A, typename B>
        __tvec4<T>::__tvec4(const __tvec3<A>& a, B b):
        x(static_cast<T>(a.x)),
        y(static_cast<T>(a.y)),
        z(static_cast<T>(a.z)),
        w(static_cast<T>(b))
        {}
        
        template <typename T>
        template <typename A, typename B>
        __tvec4<T>::__tvec4(A a, const __tvec3<B>& b):
        x(static_cast<T>(a)),
        y(static_cast<T>(b.x)),
        z(static_cast<T>(b.y)),
        w(static_cast<T>(b.z))
        {}
        
        
        template <typename T>
        template <typename A, typename B>
        __tvec4<T>::__tvec4(const __tvec2<A>& a, const __tvec2<B>& b):
        x(static_cast<T>(a.x)),
        y(static_cast<T>(a.y)),
        z(static_cast<T>(b.x)),
        w(static_cast<T>(b.y))
        {}
        
        template <typename T>
        template <typename U>
        __tvec4<T>::__tvec4(const __tvec4<U> &v):
        x(static_cast<T>(v.x)),
        y(static_cast<T>(v.y)),
        z(static_cast<T>(v.z)),
        w(static_cast<T>(v.w))
        {}
        
        template <typename T>
        __tvec4<T>& __tvec4<T>::operator=(const __tvec4<T> &v)
        {
            if(this != &v)
            {
                this->x = v.x;
                this->y = v.y;
                this->z = v.z;
                this->w = v.w;
            }
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tvec4<T>& __tvec4<T>::operator=(const __tvec4<U> &v)
        {
            if(this != &v)
            {
                this->x = static_cast<T>(v.x);
                this->y = static_cast<T>(v.y);
                this->z = static_cast<T>(v.z);
                this->w = static_cast<T>(v.w);
            }
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tvec4<T>& __tvec4<T>::operator+=(const __tvec4<U> &v)
        {
            if(this != &v)
            {
                this->x += static_cast<T>(v.x);
                this->y += static_cast<T>(v.y);
                this->z += static_cast<T>(v.z);
                this->w += static_cast<T>(v.w);
            }
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tvec4<T>& __tvec4<T>::operator+=(U s)
        {
            this->x += static_cast<T>(s);
            this->y += static_cast<T>(s);
            this->z += static_cast<T>(s);
            this->w += static_cast<T>(s);
            
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tvec4<T>& __tvec4<T>::operator-=(const __tvec4<U> &v)
        {
            if(this != &v)
            {
                this->x -= static_cast<T>(v.x);
                this->y -= static_cast<T>(v.y);
                this->z -= static_cast<T>(v.z);
                this->w -= static_cast<T>(v.w);
            }
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tvec4<T>& __tvec4<T>::operator*=(const __tvec4<U> &v)
        {
            if(this != &v)
            {
                this->x *= static_cast<T>(v.x);
                this->y *= static_cast<T>(v.y);
                this->z *= static_cast<T>(v.z);
                this->w *= static_cast<T>(v.w);
            }
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tvec4<T>& __tvec4<T>::operator*=(U s)
        {
            this->x *= static_cast<T>(s);
            this->y *= static_cast<T>(s);
            this->z *= static_cast<T>(s);
            this->w *= static_cast<T>(s);
            
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tvec4<T>& __tvec4<T>::operator/=(const __tvec4<U> &v)
        {
            if(this != &v)
            {
                this->x /= static_cast<T>(v.x);
                this->y /= static_cast<T>(v.y);
                this->z /= static_cast<T>(v.z);
                this->w /= static_cast<T>(v.w);
            }
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tvec4<T>& __tvec4<T>::operator/=(U s)
        {
            this->x /= static_cast<T>(s);
            this->y /= static_cast<T>(s);
            this->z /= static_cast<T>(s);
            this->w /= static_cast<T>(s);
            
            return *this;
        }

        
        
        template <typename T>
        __tvec4<T> operator+(const __tvec4<T>& v, const T& s)
        {
            return __tvec4<T>(v.x + s,
                              v.y + s,
                              v.z + s,
                              v.w + s);
        }
        
        template <typename T>
        __tvec4<T> operator+(const T& s, const __tvec4<T>& v)
        {
            return __tvec4<T>(s + v.x,
                              s + v.y,
                              v.z + s,
                              v.w + s);
        }
        
        template <typename T>
        __tvec4<T> operator+(const __tvec4<T>& v1, const __tvec4<T>& v2)
        {
            return __tvec4<T>(v1.x + v2.x,
                              v1.y + v2.y,
                              v1.z + v2.z,
                              v1.w + v2.w);
        }
        
        
        template <typename T>
        __tvec4<T> operator-(const __tvec4<T>& v, const T& s)
        {
            return __tvec4<T>(v.x - s,
                              v.y - s,
                              v.z - s,
                              v.w - s);
        }
        
        template <typename T>
        __tvec4<T> operator-(const T& s, const __tvec4<T>& v)
        {
            return __tvec4<T>(s - v.x,
                              s - v.y,
                              v.z - s,
                              v.w - s);
        }
        
        template <typename T>
        __tvec4<T> operator-(const __tvec4<T>& v1, const __tvec4<T>& v2)
        {
            return __tvec4<T>(v1.x - v2.x,
                              v1.y - v2.y,
                              v1.z - v2.z,
                              v1.w - v2.w);
        }
        
        template <typename T>
        __tvec4<T> operator*(const __tvec4<T>& v, const T& s)
        {
            return __tvec4<T>(v.x * s,
                              v.y * s,
                              v.z * s,
                              v.w * s);
        }
        
        template <typename T>
        __tvec4<T> operator*(const T& s, const __tvec4<T>& v)
        {
            return __tvec4<T>(s * v.x,
                              s * v.y,
                              v.z * s,
                              v.w * s);
        }
        
        template <typename T>
        __tvec4<T> operator*(const __tvec4<T>& v1, const __tvec4<T>& v2)
        {
            return __tvec4<T>(v1.x * v2.x,
                              v1.y * v2.y,
                              v1.z * v2.z,
                              v1.w * v2.w);
        }
        
        template <typename T>
        __tvec4<T> operator/(const __tvec4<T>& v, const T& s)
        {
            return __tvec4<T>(v.x / s,
                              v.y / s,
                              v.z / s,
                              v.w / s);
        }
        
        template <typename T>
        __tvec4<T> operator/(const T& s, const __tvec4<T>& v)
        {
            return __tvec4<T>(s / v.x,
                              s / v.y,
                              v.z / s,
                              v.w / s);
        }
        
        template <typename T>
        __tvec4<T> operator/(const __tvec4<T>& v1, const __tvec4<T>& v2)
        {
            return __tvec4<T>(v1.x / v2.x,
                              v1.y / v2.y,
                              v1.z / v2.z,
                              v1.w / v2.w);
        }
        
        template <typename T>
        __tvec4<T> operator-(const __tvec4<T>& v)
        {
            return __tvec4<T>(-v.x, -v.y, -v.z, -v.w);
        }
        
        template <typename T>
        bool operator==(const __tvec4<T>& v1, const __tvec4<T>& v2)
        {
            return (v1.x == v2.x)&&(v1.y == v2.y)&&(v1.z == v2.z)&&(v1.w == v2.w);
        }
        
        template <typename T>
        bool operator!=(const __tvec4<T>& v1, const __tvec4<T>& v2)
        {
            return (v1.x != v2.x)||(v1.y != v2.y)||(v1.z != v2.z)||(v1.w == v2.w);
        }
    }
}

#include "__type_vec4_sse_inl.hpp"

#endif /* type_vec4_inl_h */
