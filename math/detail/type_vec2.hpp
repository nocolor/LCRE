//
//  type_vec2.hpp
//  lrcm
//
//  Created by qyt on 16/2/15.
//  Copyright © 2016年 qyt. All rights reserved.
//

#ifndef type_vec2_hpp
#define type_vec2_hpp

#include <iostream>
#include "__swizzle.hpp"

namespace qyt
{
    namespace __detail
    {
        template <typename T>
        struct __tvec2
        {
            union
            {
                struct{T x, y;};
                struct{T r, g;};
                struct{T s, t;};

                __GENERATE_SWIZZLE_2_2(T, __tvec2, x, y)
                __GENERATE_SWIZZLE_2_2(T, __tvec2, r, g)
                __GENERATE_SWIZZLE_2_2(T, __tvec2, s, t)

                __GENERATE_SWIZZLE_2_3(T, __tvec3, x, y)
                __GENERATE_SWIZZLE_2_3(T, __tvec3, r, g)
                __GENERATE_SWIZZLE_2_3(T, __tvec3, s, t)

                __GENERATE_SWIZZLE_2_4(T, __tvec4, x, y)
                __GENERATE_SWIZZLE_2_4(T, __tvec4, r, g)
                __GENERATE_SWIZZLE_2_4(T, __tvec4, s, t)

            };
            
            size_t size() const;
            T& operator[](size_t i);
            const T& operator[](size_t i) const;
            __tvec2();
            __tvec2(const __tvec2<T>& v);
            explicit __tvec2(const T& s);
            __tvec2(const T& s1, const T& s2);
            
            template<typename A, typename B>
            __tvec2(const A& s1, const B& s2);
            
            template<typename U>
            __tvec2(const __tvec2<U>& v);
            
            
            __tvec2<T>& operator=(const __tvec2<T>& v);
            
            template <typename U>
            __tvec2<T>& operator=(const __tvec2<U> & v);
            template <typename U>
            __tvec2<T>& operator+=(U s);
            template <typename U>
            __tvec2<T>& operator+=(const __tvec2<U> & v);
            template <typename U>
            __tvec2<T>& operator-=(U s);
            template <typename U>
            __tvec2<T>& operator-=(const __tvec2<U> & v);
            template <typename U>
            __tvec2<T>& operator*=(U s);
            template <typename U>
            __tvec2<T>& operator*=(const __tvec2<U> & v);
            template <typename U>
            __tvec2<T>& operator/=(U s);
            template <typename U>
            __tvec2<T>& operator/=(const __tvec2<U> & v);
            
        };
        
        template <typename T>
        __tvec2<T> operator+(const __tvec2<T> & v, const T & s);
        
        template <typename T>
        __tvec2<T> operator+(const T & s, const __tvec2<T> & v);
        
        template <typename T>
        __tvec2<T> operator+(const __tvec2<T> & v1, const __tvec2<T> & v2);
        
        template <typename T>
        __tvec2<T> operator-(const __tvec2<T> & v, const T & s);
        
        template <typename T>
        __tvec2<T> operator-(const T & s, const __tvec2<T> & v);
        
        template <typename T>
        __tvec2<T> operator-(const __tvec2<T> & v1, const __tvec2<T> & v2);
        
        template <typename T>
        __tvec2<T> operator/(const __tvec2<T> & v, const T & s);
        
        template <typename T>
        __tvec2<T> operator/(const T & s, const __tvec2<T> & v);
        
        template <typename T>
        __tvec2<T> operator/(const __tvec2<T> & v1, const __tvec2<T> & v2);

        template <typename T>
        __tvec2<T> operator-(const __tvec2<T> & v);
        
        template <typename T>
        bool operator==(const __tvec2<T>& v1, const __tvec2<T> & v2);
        
        template <typename T>
        bool operator!=(const __tvec2<T>& v1, const __tvec2<T> & v2);
        
        template<typename T>
        std::ostream &operator<<(std::ostream &os, const __tvec2<T> &v)
        {
            printf("[%-*.*f, %-*.*f]", 12, 6, v.x, 12, 6, v.y);
            return os;
        }
    }
}

#include "type_vec2_inl.hpp"

#endif /* type_vec2_hpp */
