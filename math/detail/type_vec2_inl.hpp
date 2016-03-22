//
//  type_vec2.cpp
//  lrcm
//
//  Created by qyt on 16/2/15.
//  Copyright © 2016年 qyt. All rights reserved.
//
#include <assert.h>

namespace qyt
{
    namespace __detail
    {
        template<typename T>
        size_t __tvec2<T>::size() const
        {
            return 2;
        }
        
        template<typename T>
        T& __tvec2<T>::operator[](size_t i)
        {
            assert(i >= 0 && i < this->size());
            return (&x)[i];
        }
        
        template<typename T>
        const T& __tvec2<T>::operator[](size_t i) const
        {
            assert(i >= 0 && i < this->size());
            return (&x)[i];
        }
        
        template<typename T>
        __tvec2<T>::__tvec2()
        :x(0),y(0)
        {}
        
        template<typename T>
        __tvec2<T>::__tvec2(const __tvec2<T> &v)
        :x(v.x),y(v.y)
        {}
        
        template<typename T>
        __tvec2<T>::__tvec2(const T& s)
        :x(s),y(s)
        {}
        
        template<typename T>
        __tvec2<T>::__tvec2(const T&s1, const T& s2)
        :x(s1),y(s2)
        {}
        
        template<typename T>
        template<typename A, typename B>
        __tvec2<T>::__tvec2(const A &s1, const B &s2)
        :x(static_cast<T>(s1)),y(static_cast<T>(s2))
        {}
        
        template<typename T>
        template<typename U>
        __tvec2<T>::__tvec2(const __tvec2<U> &v)
        :x(static_cast<T>(v.x)),y(static_cast<T>(v.y))
        {}
        
        template<typename T>
        __tvec2<T>& __tvec2<T>::operator=(const __tvec2<T> &v)
        {
            this->x = v.x;
            this->y = v.y;
            return *this;
        }
        
        template<typename T>
        template<typename U>
        __tvec2<T>& __tvec2<T>::operator=(const __tvec2<U>& v)
        {
            this->x = static_cast<T>(v.x);
            this->y = static_cast<T>(v.y);
            return *this;
        }
        
        template<typename T>
        template<typename U>
        __tvec2<T>& __tvec2<T>::operator+=(U s)
        {
            this->x += static_cast<T>(s);
            this->y += static_cast<T>(s);
            return *this;
        }
        
        template<typename T>
        template<typename U>
        __tvec2<T>& __tvec2<T>::operator+=(const __tvec2<U> &v)
        {
            this->x += static_cast<T>(v.x);
            this->y += static_cast<T>(v.y);
            return *this;
        }
        
        template<typename T>
        template<typename U>
        __tvec2<T>& __tvec2<T>::operator-=(U s)
        {
            this->x -= static_cast<T>(s);
            this->y -= static_cast<T>(s);
            return *this;
        }
        
        template<typename T>
        template<typename U>
        __tvec2<T>& __tvec2<T>::operator-=(const __tvec2<U> &v)
        {
            this->x -= static_cast<T>(v.x);
            this->y -= static_cast<T>(v.y);
            return *this;
        }
        
        template<typename T>
        template<typename U>
        __tvec2<T>& __tvec2<T>::operator*=(U s)
        {
            this->x *= static_cast<T>(s);
            this->y *= static_cast<T>(s);
            return *this;
        }
        
        template<typename T>
        template<typename U>
        __tvec2<T>& __tvec2<T>::operator*=(const __tvec2<U> &v)
        {
            this->x *= static_cast<T>(v.x);
            this->y *= static_cast<T>(v.y);
            return *this;
        }
        
        template<typename T>
        template<typename U>
        __tvec2<T>& __tvec2<T>::operator/=(U s)
        {
            this->x /= static_cast<T>(s);
            this->y /= static_cast<T>(s);
            return *this;
        }
        
        template<typename T>
        template<typename U>
        __tvec2<T>& __tvec2<T>::operator/=(const __tvec2<U> &v)
        {
            this->x /= static_cast<T>(v.x);
            this->y /= static_cast<T>(v.y);
            return *this;
        }
        
        template<typename T>
        __tvec2<T> operator+(const __tvec2<T> & v, const T & s)
        {
            return __tvec2<T>(v.x + s,
                              v.y + s);
        }
        
        template <typename T>
        __tvec2<T> operator+(const T & s, const __tvec2<T> & v)
        {
            return __tvec2<T>(s + v.x,
                              s + v.y);
        }
        
        template <typename T>
        __tvec2<T> operator+(const __tvec2<T> & v1, const __tvec2<T> & v2)
        {
            return __tvec2<T>(v1.x + v2.x,
                              v1.y + v2.y);
        }
        
        template<typename T>
        __tvec2<T> operator-(const __tvec2<T> & v, const T & s)
        {
            return __tvec2<T>(v.x - s,
                              v.y - s);
        }
        
        template <typename T>
        __tvec2<T> operator-(const T & s, const __tvec2<T> & v)
        {
            return __tvec2<T>(s - v.x,
                              s - v.y);
        }
        
        template <typename T>
        __tvec2<T> operator-(const __tvec2<T> & v1, const __tvec2<T> & v2)
        {
            return __tvec2<T>(v1.x - v2.x,
                              v1.y - v2.y);
        }

        template<typename T>
        __tvec2<T> operator*(const __tvec2<T> & v, const T & s)
        {
            return __tvec2<T>(v.x * s,
                              v.y * s);
        }
        
        template <typename T>
        __tvec2<T> operator*(const T & s, const __tvec2<T> & v)
        {
            return __tvec2<T>(s * v.x,
                              s * v.y);
        }
        
        template <typename T>
        __tvec2<T> operator*(const __tvec2<T> & v1, const __tvec2<T> & v2)
        {
            return __tvec2<T>(v1.x * v2.x,
                              v1.y * v2.y);
        }

        template<typename T>
        __tvec2<T> operator/(const __tvec2<T> & v, const T & s)
        {
            return __tvec2<T>(v.x / s,
                              v.y / s);
        }
        
        template <typename T>
        __tvec2<T> operator/(const T & s, const __tvec2<T> & v)
        {
            return __tvec2<T>(s / v.x,
                              s / v.y);
        }
        
        template <typename T>
        __tvec2<T> operator/(const __tvec2<T> & v1, const __tvec2<T> & v2)
        {
            return __tvec2<T>(v1.x / v2.x,
                              v1.y / v2.y);
        }

        template <typename T>
        __tvec2<T> operator-(const __tvec2<T>& v)
        {
            return __tvec2<T>(-v.x, -v.y);
        }
        
        template <typename T>
        bool operator==(const __tvec2<T>& v1, const __tvec2<T> & v2)
        {
            return (v1.x == v2.x) && (v1.y == v2.y);
        }
        
        template <typename T>
        bool operator!=(const __tvec2<T>& v1, const __tvec2<T> & v2)
        {
            return (v1.x != v2.x) || (v1.y != v2.y);
        }
        
        
        
    }//end __detail
}// end qyt