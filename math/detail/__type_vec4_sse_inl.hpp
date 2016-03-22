//
//  __type_vec4_sse_inl.hpp
//  qyt_fundation
//
//  Created by qyt on 16/2/18.
//  Copyright © 2016年 qyt. All rights reserved.
//

#ifndef __type_vec4_sse_inl_h
#define __type_vec4_sse_inl_h

namespace qyt
{
    namespace __detail
    {
        template <>
        inline __tvec4<float>::__tvec4():
        data(_mm_setzero_ps())
        {}
        
        template <>
        inline __tvec4<float>::__tvec4(float s):
        data(_mm_set1_ps(s))
        {}
    
        template <>
        inline __tvec4<float>::__tvec4(float s0, float s1, float s2, float s3):
        data(_mm_set_ps(s3, s2, s1, s0))
        {}
    
        template <>
        inline __tvec4<float>::__tvec4(typename __simd<float>::__type v):
        data(v)
        {}
    
        template <>
        template <>
        inline __tvec4<float>::__tvec4(const __tvec2<float>& v, float s0, float s1):
        data(_mm_set_ps(s1, s0, v.y, v.x))
        {}
    
        template <>
        template <>
        inline __tvec4<float>::__tvec4(float s0, const __tvec2<float>& v, float s1):
        data(_mm_set_ps(s1, v.y, v.x, s0))
        {}
    
        template <>
        template <>
        inline __tvec4<float>::__tvec4(float s0, float s1, const __tvec2<float>& v):
        data(_mm_set_ps(v.y, v.x, s1, s0))
        {}
    
        template <>
        template <>
        inline __tvec4<float>::__tvec4(const __tvec3<float> &v, float s):
        data(_mm_set_ps(s, v.z, v.y, v.x))
        {}
    
        template <>
        template <>
        inline __tvec4<float>::__tvec4(float s, const __tvec3<float>& v):
        data(_mm_set_ps(v.z, v.y, v.x, s))
        {}
    
        template <>
        inline __tvec4<float>::__tvec4(const __tvec4<float> &v):
        data(_mm_set_ps(v.w, v.z, v.y, v.x))
        {}
    
        template <>
        template <typename U>
        inline __tvec4<float>::__tvec4(const __tvec4<U>& v):
        data(_mm_set_ps(static_cast<float>(v.w),
                        static_cast<float>(v.z),
                        static_cast<float>(v.y),
                        static_cast<float>(v.x)))
        {}
    
        //__swizzle ctors...
    
        //__tvec4<float> v(u.xyzw);
        template <>
        template <int E0, int E1, int E2, int E3>
        inline __tvec4<float>::__tvec4(const __swizzle<4, float, __tvec4<float>, E0, E1, E2, E3> &that):
        data(_mm_set_ps(that[3], that[2], that[1], that[0]))
        {}
    
        //__tvec4<float> v(u.xy, w.yx);
        template <>
        template <int E0, int E1, int F0, int F1>
        inline __tvec4<float>::__tvec4(const __swizzle<2, float, __tvec2<float>, E0, E1, -1, -2> &v,
                                const __swizzle<2, float, __tvec2<float>, F0, F1, -1, -2> &u):
        data(_mm_set_ps(u[1], u[0], v[1], v[0]))
        {}
    
        //__tvec4<float> v(1.f, 2.f, w.yx);
        template <>
        template <int E0, int E1>
        inline __tvec4<float>::__tvec4(const float& s0, const float& s1,
                                const __swizzle<2, float, __tvec2<float>, E0, E1, -1, -2>& v):
        data(_mm_set_ps(v[1], v[0], s1, s0))
        {}
    
        //__tvec4<float> v(1.f, w.yx, 2.f);
        template <>
        template <int E0, int E1>
        inline __tvec4<float>::__tvec4(const float& s0,
                                const __swizzle<2, float, __tvec2<float>, E0, E1, -1, -2>& v,
                                const float& s1):
        data(_mm_set_ps(s1, v.y, v.x, s0))
        {}
    
        //__tvec4<float> v(w.yx, 1.f, 2.f);
        template <>
        template <int E0, int E1>
        inline __tvec4<float>::__tvec4(const __swizzle<2, float, __tvec2<float>, E0, E1, -1, -2>& v,
                                const float& s0,
                                const float& s1):
        data(_mm_set_ps(s1, s0, v[1], v[0]))
        {}
    
        //__tvec4<float> v(w.yxz, 1.f);
        template <>
        template <int E0, int E1, int E2>
        inline __tvec4<float>::__tvec4(const __swizzle<3, float, __tvec3<float>, E0, E1, E2, -1> &v,
                                const float &s1):
        data(_mm_set_ps(s1, v[2], v[1], v[0]))
        {}
    
        //__tvec4<float> v(1.f, w.yxz);
        template <>
        template <int E0, int E1, int E2>
        inline __tvec4<float>::__tvec4(const float &s1,
                                __swizzle<3, float, __tvec3<float>, E0, E1, E2, -1> &v):
        data(_mm_set_ps(v[2], v[1], v[1], s1))
        {}
    
        template <>
        inline __tvec4<float>& __tvec4<float>::operator=(const __tvec4<float>& v)
        {
            if (this != &v)
                this->data = _mm_set_ps(v.w, v.z, v.y, v.x);
            return *this;
        }
    
        template <>
        template <typename U>
        inline __tvec4<float>& __tvec4<float>::operator=(const __tvec4<U>& v)
        {
            if (this != &v)
                this->data = _mm_set_ps(static_cast<float>(v.w),
                                        static_cast<float>(v.z),
                                        static_cast<float>(v.y),
                                        static_cast<float>(v.x));
    
            return *this;
        }
    
        template<>
        template<typename U>
        inline __tvec4<float>& __tvec4<float>::operator+=(U s)
        {
            this->data = _mm_add_ps(this->data, _mm_set_ps1(s));
            return *this;
        }
    
        template <>
        template <typename U>
        inline __tvec4<float>& __tvec4<float>::operator+=(const __tvec4<U>& v)
        {
            if (this != &v)
                this->data = _mm_add_ps(this->data,
                                        _mm_set_ps(static_cast<float>(v.w),
                                                   static_cast<float>(v.z),
                                                   static_cast<float>(v.y),
                                                   static_cast<float>(v.x)));
            return *this;
        }
    
        template<>
        template<typename U>
        inline __tvec4<float>& __tvec4<float>::operator-=(U s)
        {
            this->data = _mm_sub_ps(this->data, _mm_set_ps1(static_cast<float>(s)));
            return *this;
        }
    
        template <>
        template <typename U>
        inline __tvec4<float>& __tvec4<float>::operator-=(const __tvec4<U>& v)
        {
            if (this != &v)
                this->data = _mm_sub_ps(this->data,
                                        _mm_set_ps(static_cast<float>(v.w),
                                                   static_cast<float>(v.z),
                                                   static_cast<float>(v.y),
                                                   static_cast<float>(v.x)));
            return *this;
        }
    
        template<>
        template<typename U>
        inline __tvec4<float>& __tvec4<float>::operator*=(U s)
        {
            this->data = _mm_mul_ps(this->data, _mm_set_ps1(static_cast<float>(s)));
            return *this;
        }
    
        template <>
        template <typename U>
        inline __tvec4<float>& __tvec4<float>::operator*=(const __tvec4<U>& v)
        {
            if (this != &v)
                this->data = _mm_mul_ps(this->data,
                                        _mm_set_ps(static_cast<float>(v.w),
                                                   static_cast<float>(v.z),
                                                   static_cast<float>(v.y),
                                                   static_cast<float>(v.x)));
            return *this;
        }
        
        template<>
        template<typename U>
        inline __tvec4<float>& __tvec4<float>::operator/=(U s)
        {
            this->data = _mm_div_ps(this->data, _mm_set_ps1(static_cast<float>(s)));
            return *this;
        }
        
        template <>
        template <typename U>
        inline __tvec4<float>& __tvec4<float>::operator/=(const __tvec4<U>& v)
        {
            if (this != &v)
                this->data = _mm_div_ps(this->data,
                                        _mm_set_ps(static_cast<float>(v.w),
                                                   static_cast<float>(v.z),
                                                   static_cast<float>(v.y),
                                                   static_cast<float>(v.x)));
            return *this;
        }
        
        /*
         无需特化
         template <>
         __tvec4<float> operator+(const __tvec4<float>& v, const float& s)
         这一类模板，因为形如
         w = v + u; //都是vec4f
         这类调用会直接到
         template <typename T>
         __tvec4<T> operator+...
         族函数中，后者的实现会根据模板参数类型匹配，如果T是float，那么在构造的时候自然就调用到
         本文件中定义的函数中
         */

    }
    

    
}

#endif /* __type_vec4_sse_inl_h */
