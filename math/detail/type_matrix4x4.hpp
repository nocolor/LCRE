//
//  type_matrix4x4.h
//  qyt_fundation
//
//  Created by qyt on 16/2/18.
//  Copyright © 2016年 qyt. All rights reserved.
//

#ifndef type_matrix4x4_h
#define type_matrix4x4_h

#include "type_vec4.hpp"
namespace  qyt
{
    namespace  __detail
    {
        template <typename T>
        struct __tmat4x4
        {
            typedef __tvec4<T> __col_type;
            typedef __tvec4<T> __row_type;
            
        private:
            __col_type value[4];
            
        public:
            __tmat4x4();
            __tmat4x4(const __tmat4x4<T>& m);
            __tmat4x4(const T& s);
            __tmat4x4(const T& x0, const T& y0, const T& z0, const T& w0,
                      const T& x1, const T& y1, const T& z1, const T& w1,
                      const T& x2, const T& y2, const T& z2, const T& w2,
                      const T& x3, const T& y3, const T& z3, const T& w3);
            __tmat4x4(const __col_type& v0,
                      const __col_type& v1,
                      const __col_type& v2,
                      const __col_type& v3);
            template<
            typename X1, typename Y1, typename Z1, typename W1,
            typename X2, typename Y2, typename Z2, typename W2,
            typename X3, typename Y3, typename Z3, typename W3,
            typename X4, typename Y4, typename Z4, typename W4>
            __tmat4x4(const X1& x1, const Y1& y1, const Z1& z1, const W1& w1,
                      const X2& x2, const Y2& y2, const Z2& z2, const W2& w2,
                      const X3& x3, const Y3& y3, const Z3& z3, const W3& w3,
                      const X4& x4, const Y4& y4, const Z4& z4, const W4& w4);
            
            template <typename V0, typename V1, typename V2, typename V3>
            __tmat4x4(const __tvec4<V0>& v0,
                      const __tvec4<V1>& v1,
                      const __tvec4<V2>& v2,
                      const __tvec4<V3>& v3);
            
            template <typename U>
            __tmat4x4(const __tmat4x4<U>& m);
            
            size_t size() const;
            __col_type& operator[](size_t i);
            const __col_type& operator[](size_t i) const;
            
            __tmat4x4<T>& operator=(const __tmat4x4<T>& m);
            template <typename U>
            __tmat4x4<T>& operator=(const __tmat4x4<U>& m);
            
            template <typename U>
            __tmat4x4<T>& operator+=(const __tmat4x4<U>& m);
            template <typename U>
            __tmat4x4<T>& operator+=(const U& s);
            
            template <typename U>
            __tmat4x4<T>& operator-=(const __tmat4x4<U>& m);
            template <typename U>
            __tmat4x4<T>& operator-=(const U& s);
            
            
            template <typename U>
            __tmat4x4<T>& operator*=(const __tmat4x4<U>& m);
            template <typename U>
            __tmat4x4<T>& operator*=(const U& s);
            
            template <typename U>
            __tmat4x4<T>& operator/=(const __tmat4x4<U>& m);
            template <typename U>
            __tmat4x4<T>& operator/=(const U& s);
            
            __tmat4x4<T> operator~() const;
            __tmat4x4<T> operator!() const;
            
            static __tmat4x4<T> get_inverse(const __tmat4x4<T>& m);
        };
        
        template <typename T>
        __tmat4x4<T> operator+(const __tmat4x4<T>& m, const T& s);
        
        template <typename T>
        __tmat4x4<T> operator+(const T& s, const __tmat4x4<T>& m);
        
        template <typename T>
        __tmat4x4<T> operator+(const __tmat4x4<T>& m0, const __tmat4x4<T>& m1);
        
        
        template <typename T>
        __tmat4x4<T> operator-(const __tmat4x4<T>& m, const T& s);
        
        template <typename T>
        __tmat4x4<T> operator-(const T& s, const __tmat4x4<T>& m);
        
        template <typename T>
        __tmat4x4<T> operator-(const __tmat4x4<T>& m0, const __tmat4x4<T>& m1);
        
        
        template <typename T>
        __tmat4x4<T> operator*(const __tmat4x4<T>& m, const T& s);
        
        template <typename T>
        __tmat4x4<T> operator*(const T& s, const __tmat4x4<T>& m);
        
        template <typename T>
        __tmat4x4<T> operator*(const __tmat4x4<T>& m0, const __tmat4x4<T>& m1);
        
        template <typename T>
        typename __tmat4x4<T>::__col_type operator*(const __tmat4x4<T>& m,
                                                    const typename __tmat4x4<T>::__col_type& v);
        template <typename T>
        typename __tmat4x4<T>::__row_type operator*(const typename __tmat4x4<T>::__col_type& v,
                                                    const __tmat4x4<T>& m);
        
        
        template <typename T>
        __tmat4x4<T> operator/(const __tmat4x4<T>& m, const T& s);
        
        template <typename T>
        __tmat4x4<T> operator/(const T& s, const __tmat4x4<T>& m);
        
        template <typename T>
        __tmat4x4<T> operator/(const __tmat4x4<T>& m0, const __tmat4x4<T>& m1);
        
        template <typename T>
        typename __tmat4x4<T>::__col_type operator/(const __tmat4x4<T>& m,
                                                    const typename __tmat4x4<T>::__col_type& v);
        template <typename T>
        typename __tmat4x4<T>::__col_type operator/(const typename __tmat4x4<T>::__col_type& v,
                                                    const __tmat4x4<T>& m);
        
        template <typename T>
        __tmat4x4<T> operator-(const __tmat4x4<T>& m);
        
        template<typename T>
        std::ostream &operator<<(std::ostream &os, const __tmat4x4<T> &m)
        {
            printf("|%-*.*g| %-*.*g| %-*.*g| %-*.*g|\n", 12, 6, m[0][0], 12, 6, m[1][0], 12, 6, m[2][0], 12, 6, m[3][0]);
            printf("|%-*.*g| %-*.*g| %-*.*g| %-*.*g|\n", 12, 6, m[0][1], 12, 6, m[1][1], 12, 6, m[2][1], 12, 6, m[3][1]);
            printf("|%-*.*g| %-*.*g| %-*.*g| %-*.*g|\n", 12, 6, m[0][2], 12, 6, m[1][2], 12, 6, m[2][2], 12, 6, m[3][2]);
            printf("|%-*.*g| %-*.*g| %-*.*g| %-*.*g|\n", 12, 6, m[0][3], 12, 6, m[1][3], 12, 6, m[2][3], 12, 6, m[3][3]);
            return os;
        }
        

        
    }
}

#include "type_matrix4x4_inl.hpp"

#endif /* type_matrix4x4_h */
