//
// Created by qyt on 16/2/17.
//

#ifndef QYT_FUNDATION_TYPE_VEC4_HPP
#define QYT_FUNDATION_TYPE_VEC4_HPP

#include <iostream>
#include "__swizzle.hpp"
#include <xmmintrin.h>

namespace qyt
{
    namespace __detail
    {
        //sse指令优化使用
        template <typename T>
        struct __simd
        {
            typedef T __type[4];
        };
        
        template <>
        struct __simd<float>
        {
            typedef __m128 __type;
        };
        

        template <typename T>
        struct __tvec4
        {
            union
            {
                struct { T x, y, z, w; };
                struct { T r, g, b, a; };
                struct { T s, t, p, q; };
                
                typename __simd<T>::__type data;
                
                __GENERATE_SWIZZLE_4_2(T, __tvec2, x, y, z, w)
                __GENERATE_SWIZZLE_4_2(T, __tvec2, r, g, b, a)
                __GENERATE_SWIZZLE_4_2(T, __tvec2, s, t, p, q)
                
                __GENERATE_SWIZZLE_4_3(T, __tvec3, x, y, z, w)
                __GENERATE_SWIZZLE_4_3(T, __tvec3, r, g, b, a)
                __GENERATE_SWIZZLE_4_3(T, __tvec3, s, t, p, q)
                
                __GENERATE_SWIZZLE_4_4(T, __tvec4, x, y, z, w)
                __GENERATE_SWIZZLE_4_4(T, __tvec4, r, g, b, a)
                __GENERATE_SWIZZLE_4_4(T, __tvec4, s, t, p, q)
            };
            
            size_t size() const;
            T& operator[](size_t i);
            const T& operator[](size_t i) const;
            
            __tvec4();
            __tvec4(const __tvec4<T>& v);
            explicit __tvec4(T s);
            explicit __tvec4(typename __simd<T>::__type v);
            __tvec4(T a, T b, T c, T d);
            
            template <typename A, typename B, typename C, typename D>
            __tvec4(A a, B b, C c, D d);
            

            template <typename A, typename B, typename C>
            explicit __tvec4(const __tvec2<A> & a, B b, C c);

            template <typename A, typename B, typename C>
            explicit __tvec4(A a, const __tvec2<B> & b, C c);

            template <typename A, typename B, typename C>
            explicit __tvec4(A a, B b, const __tvec2<C> & c);

            template <typename A, typename B>
            explicit __tvec4(const __tvec3<A> & a, B b);

            template <typename A, typename B>
            explicit __tvec4(A a, const __tvec3<B> & b);
            
            template <typename A, typename B>
            explicit __tvec4(const __tvec2<A> & a, const __tvec2<B> & b);
            
            template <typename U>
            __tvec4(const __tvec4<U>& v);
            
            
            //__swizzle ctors...
            
            //__tvec4<float> v(u.xyzw);
            template <int E0, int E1, int E2, int E3>
            __tvec4(const __swizzle<4, T, __tvec4<T>, E0, E1, E2, E3>& that)
            {
                *this = that();
            }
            
            //__tvec4<float> v(u.xy, w.yx);
            template <int E0, int E1, int F0, int F1>
            __tvec4(const __swizzle<2, T, __tvec2<T>, E0, E1, -1, -2>& v,
                    const __swizzle<2, T, __tvec2<T>, F0, F1, -1, -2>& u)
            {
                *this = __tvec4<T>(v(), u());
            }
            
            //__tvec4<float> v(1.f, 2.f, w.yx);
            template <int E0, int E1>
            __tvec4(const T& s1, const T& s2, const __swizzle<2, T, __tvec2<T>, E0, E1, -1, -2>& v)
            {
                *this = __tvec4<T>(s1, s2, v());
            }
            
            //__tvec4<float> v(1.f, w.yx, 2.f);
            template <int E0, int E1>
            __tvec4(const T& s1, const __swizzle<2, T, __tvec2<T>, E0, E1, -1, -2>& v, const T& s2)
            {
                *this = __tvec4<T>(s1, v(), s2);
            }
            
            //__tvec4<float> v(w.yx, 1.f, 2.f);
            template <int E0, int E1>
            __tvec4(const __swizzle<2, T, __tvec2<T>, E0, E1, -1, -2>& v, const T& s1, const T& s2)
            {
                *this = __tvec4<T>(v(), s1, s2);
            }
            
            //__tvec4<float> v(w.yxz, 1.f);
            template <int E0, int E1, int E2>
            __tvec4(const __swizzle<3, T, __tvec3<T>, E0, E1, E2, -1>& v, const T& s1)
            {
                *this = __tvec4<T>(v(), s1);
            }
            
            //__tvec4<float> v(1.f, w.yxz);
            template <int E0, int E1, int E2>
            __tvec4(const T& s1, __swizzle<3, T, __tvec3<T>, E0, E1, E2, -1>& v)
            {
                *this = __tvec4<T>(s1, v());
            }
            
            __tvec4<T>& operator=(const __tvec4<T>& v);
            
            template <typename U>
            __tvec4<T>& operator=(const __tvec4<U> & v);
            template <typename U>
            __tvec4<T>& operator+=(U s);
            template <typename U>
            __tvec4<T>& operator+=(const __tvec4<U> & v);
            template <typename U>
            __tvec4<T>& operator-=(U s);
            template <typename U>
            __tvec4<T>& operator-=(const __tvec4<U> & v);
            template <typename U>
            __tvec4<T>& operator*=(U s);
            template <typename U>
            __tvec4<T>& operator*=(const __tvec4<U> & v);
            template <typename U>
            __tvec4<T>& operator/=(U s);
            template <typename U>
            __tvec4<T>& operator/=(const __tvec4<U> & v);
        };
        
        template <typename T>
        __tvec4<T> operator+(const __tvec4<T> & v, const T & s);
        
        template <typename T>
        __tvec4<T> operator+(const T & s, const __tvec4<T> & v);
        
        template <typename T>
        __tvec4<T> operator+(const __tvec4<T> & v1, const __tvec4<T> & v2);
        
        template <typename T>
        __tvec4<T> operator-(const __tvec4<T> & v, const T & s);
        
        template <typename T>
        __tvec4<T> operator-(const T & s, const __tvec4<T> & v);
        
        template <typename T>
        __tvec4<T> operator-(const __tvec4<T> & v1, const __tvec4<T> & v2);
        
        template <typename T>
        __tvec4<T> operator/(const __tvec4<T> & v, const T & s);
        
        template <typename T>
        __tvec4<T> operator/(const T & s, const __tvec4<T> & v);
        
        template <typename T>
        __tvec4<T> operator/(const __tvec4<T> & v1, const __tvec4<T> & v2);
        
        template <typename T>
        __tvec4<T> operator-(const __tvec4<T> & v);
        
        template <typename T>
        bool operator==(const __tvec4<T>& v1, const __tvec4<T> & v2);
        
        template <typename T>
        bool operator!=(const __tvec4<T>& v1, const __tvec4<T> & v2);
        
        template<typename T>
        std::ostream &operator<<(std::ostream &os, const __tvec4<T> &v)
        {
            printf("[%-*.*g, %-*.*g, %-*.*g, %-*.*g]", 12, 6, v.x, 12, 6, v.y, 12, 6, v.z, 12, 6, v.w);
            return os;
        }
        
    }
}


#include "type_vec4_inl.hpp"



#endif //QYT_FUNDATION_TYPE_VEC4_HPP
