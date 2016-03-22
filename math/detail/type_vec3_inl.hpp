//
// Created by qyt on 16/2/16.
//

#ifndef QYT_FUNDATION_TYPE_VEC3_INL_HPP
#define QYT_FUNDATION_TYPE_VEC3_INL_HPP
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
        template <typename T>
        size_t __tvec3<T>::size() const
        {
            return 3;
        }

        template <typename T>
        T& __tvec3<T>::operator[](size_t i)
        {
            assert(i >= 0 && i < this->size());
            return (&x)[i];
        }

        template <typename T>
        const T& __tvec3<T>::operator[](size_t i) const
        {
            assert(i >= 0 && i < this->size());
            return (&x)[i];
        }

        template <typename T>
        __tvec3<T>::__tvec3()
                :x(0),y(0),z(0)
        {}

        template <typename T>
        __tvec3<T>::__tvec3(const __tvec3<T> &v)
                :x(v.x),y(v.y),z(0)
        {}

        template <typename T>
        template <typename A, typename B>
        __tvec3<T>::__tvec3(const __tvec2<A> &v, const B &s)
                :x(v.x),y(v.y),z(s)
        {}

        template <typename T>
        template <typename A, typename B>
        __tvec3<T>::__tvec3(const A &s, const __tvec2<B> &v)
                :x(s),y(v.x),z(v.y)
        {}

        template <typename T>
        __tvec3<T>::__tvec3(const T& s1, const T& s2, const T& s3)
        :x(s1), y(s2), z(s3)
        {}

        template<typename T>
        __tvec3<T>::__tvec3(const T& s)
                :x(s),y(s),z(s)
        {}

        template<typename T>
        __tvec3<T>::__tvec3(const T&s1, const T& s2)
                :x(s1),y(s2),z(0)
        {}

        template<typename T>
        template<typename A, typename B, typename C>
        __tvec3<T>::__tvec3(const A &s1, const B &s2, const C& s3)
                :x(static_cast<T>(s1)),y(static_cast<T>(s2)), z(static_cast<T>(s3))
        {}

        template<typename T>
        template<typename U>
        __tvec3<T>::__tvec3(const __tvec3<U> &v)
                :x(static_cast<T>(v.x)),y(static_cast<T>(v.y))
        {}

        template<typename T>
        __tvec3<T>& __tvec3<T>::operator=(const __tvec3<T> &v)
        {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            return *this;
        }

        template<typename T>
        template<typename U>
        __tvec3<T>& __tvec3<T>::operator=(const __tvec3<U>& v)
        {
            this->x = static_cast<T>(v.x);
            this->y = static_cast<T>(v.y);
            this->z = static_cast<T>(v.z);
            return *this;
        }

        template<typename T>
        template<typename U>
        __tvec3<T>& __tvec3<T>::operator+=(U s)
        {
            this->x += static_cast<T>(s);
            this->y += static_cast<T>(s);
            this->z += static_cast<T>(s);
            return *this;
        }

        template<typename T>
        template<typename U>
        __tvec3<T>& __tvec3<T>::operator+=(const __tvec3<U> &v)
        {
            this->x += static_cast<T>(v.x);
            this->y += static_cast<T>(v.y);
            this->z += static_cast<T>(v.z);
            return *this;
        }

        template<typename T>
        template<typename U>
        __tvec3<T>& __tvec3<T>::operator-=(U s)
        {
            this->x -= static_cast<T>(s);
            this->y -= static_cast<T>(s);
            this->z -= static_cast<T>(s);
            return *this;
        }

        template<typename T>
        template<typename U>
        __tvec3<T>& __tvec3<T>::operator-=(const __tvec3<U> &v)
        {
            this->x -= static_cast<T>(v.x);
            this->y -= static_cast<T>(v.y);
            this->z -= static_cast<T>(v.z);
            return *this;
        }

        template<typename T>
        template<typename U>
        __tvec3<T>& __tvec3<T>::operator*=(U s)
        {
            this->x *= static_cast<T>(s);
            this->y *= static_cast<T>(s);
            this->z *= static_cast<T>(s);
            return *this;
        }

        template<typename T>
        template<typename U>
        __tvec3<T>& __tvec3<T>::operator*=(const __tvec3<U> &v)
        {
            this->x *= static_cast<T>(v.x);
            this->y *= static_cast<T>(v.y);
            this->z *= static_cast<T>(v.z);
            return *this;
        }

        template<typename T>
        template<typename U>
        __tvec3<T>& __tvec3<T>::operator/=(U s)
        {
            this->x /= static_cast<T>(s);
            this->y /= static_cast<T>(s);
            this->z /= static_cast<T>(s);
            return *this;
        }

        template<typename T>
        template<typename U>
        __tvec3<T>& __tvec3<T>::operator/=(const __tvec3<U> &v)
        {
            this->x /= static_cast<T>(v.x);
            this->y /= static_cast<T>(v.y);
            this->z /= static_cast<T>(v.z);
            return *this;
        }

        template<typename T>
        __tvec3<T> operator+(const __tvec3<T> & v, const T & s)
        {
            return __tvec3<T>(v.x + s,
                              v.y + s,
                              v.z + s);
        }

        template <typename T>
        __tvec3<T> operator+(const T & s, const __tvec3<T> & v)
        {
            return __tvec3<T>(s + v.x,
                              s + v.y,
                              s + v.z);
        }

        template <typename T>
        __tvec3<T> operator+(const __tvec3<T> & v1, const __tvec3<T> & v2)
        {
            return __tvec3<T>(v1.x + v2.x,
                              v1.y + v2.y,
                              v1.z + v2.z);
        }

        template<typename T>
        __tvec3<T> operator-(const __tvec3<T> & v, const T & s)
        {
            return __tvec3<T>(v.x - s,
                              v.y - s,
                              v.z - s);
        }

        template <typename T>
        __tvec3<T> operator-(const T & s, const __tvec3<T> & v)
        {
            return __tvec3<T>(s - v.x,
                              s - v.y,
                              s - v.z);
        }

        template <typename T>
        __tvec3<T> operator-(const __tvec3<T> & v1, const __tvec3<T> & v2)
        {
            return __tvec3<T>(v1.x - v2.x,
                              v1.y - v2.y,
                              v1.z - v2.z);
        }

        template<typename T>
        __tvec3<T> operator*(const __tvec3<T> & v, const T & s)
        {
            return __tvec3<T>(v.x * s,
                              v.y * s,
                              v.z * s);
        }

        template <typename T>
        __tvec3<T> operator*(const T & s, const __tvec3<T> & v)
        {
            return __tvec3<T>(s * v.x,
                              s * v.y,
                              s * v.z);
        }

        template <typename T>
        __tvec3<T> operator*(const __tvec3<T> & v1, const __tvec3<T> & v2)
        {
            return __tvec3<T>(v1.x * v2.x,
                              v1.y * v2.y,
                              v1.z * v2.z);
        }

        template<typename T>
        __tvec3<T> operator/(const __tvec3<T> & v, const T & s)
        {
            return __tvec3<T>(v.x / s,
                              v.y / s,
                              v.z / s);
        }

        template <typename T>
        __tvec3<T> operator/(const T & s, const __tvec3<T> & v)
        {
            return __tvec3<T>(s / v.x,
                              s / v.y,
                              s / v.z);
        }

        template <typename T>
        __tvec3<T> operator/(const __tvec3<T> & v1, const __tvec3<T> & v2)
        {
            return __tvec3<T>(v1.x / v2.x,
                              v1.y / v2.y,
                              v1.z / v2.z);
        }
        
        template <typename T>
        __tvec3<T> operator-(const __tvec3<T>& v)
        {
            return __tvec3<T>(-v.x, -v.y, -v.z);
        }

        template <typename T>
        bool operator==(const __tvec3<T>& v1, const __tvec3<T> & v2)
        {
            return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
        }

        template <typename T>
        bool operator!=(const __tvec3<T>& v1, const __tvec3<T> & v2)
        {
            return (v1.x != v2.x) || (v1.y != v2.y)|| (v1.z != v2.z);
        }
    }//end __detail
}// end qyt
#endif //QYT_FUNDATION_TYPE_VEC3_INL_HPP
