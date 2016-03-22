//
// Created by qyt on 16/2/16.
//

#ifndef QYT_FUNDATION_TYPE_VEC3_HPP
#define QYT_FUNDATION_TYPE_VEC3_HPP

#include <iostream>
#include "__swizzle.hpp"

namespace qyt
{
    namespace __detail
    {
        template <typename T>
        struct __tvec3
        {
            union
            {
                struct{T x, y, z;};
                struct{T r, g, b;};
                struct{T s, t, p;};

                __GENERATE_SWIZZLE_3_2(T, __tvec2, x, y, z)
                __GENERATE_SWIZZLE_3_2(T, __tvec2, r, g, b)
                __GENERATE_SWIZZLE_3_2(T, __tvec2, s, t, p)

                __GENERATE_SWIZZLE_3_3(T, __tvec3, x, y, z)
                __GENERATE_SWIZZLE_3_3(T, __tvec3, r, g, b)
                __GENERATE_SWIZZLE_3_3(T, __tvec3, s, t, p)

                __GENERATE_SWIZZLE_3_4(T, __tvec4, x, y, z)
                __GENERATE_SWIZZLE_3_4(T, __tvec4, r, g, b)
                __GENERATE_SWIZZLE_3_4(T, __tvec4, s, t, p)
            };

            size_t size() const;
            T& operator[](size_t i);
            const T& operator[](size_t i) const;
            __tvec3();
            __tvec3(const __tvec3<T>& v);
            explicit __tvec3(const T& s);
            __tvec3(const T& s1, const T& s2);
            __tvec3(const T& s1, const T& s2, const T& s3);


            template <typename A, typename B, typename C>
            __tvec3(const A& s1, const B& s2, const C& s3);

            template <typename U>
            __tvec3(const __tvec3<U>& v);


            template <typename A, typename B>
            __tvec3(const __tvec2<A>& v, const B& s);

            template <typename A, typename B>
            __tvec3(const A& s, const __tvec2<B>& v);


            //__swuzzle ctors...
            template <int E0, int E1, int E2>
            __tvec3(const __swizzle<3, T, __tvec3<T>, E0, E1, E2, -1>& _sv)
            {
                *this = _sv();
            }

            template <int E0, int E1>
            __tvec3(const __swizzle<2, T, __tvec2<T>, E0, E1, -1, -2>& _sv, const T& s)
            {
                *this = __tvec3<T>(_sv(), s);
            }

            template <int E0, int E1>
            __tvec3(const T& s, const __swizzle<2, T, __tvec2<T>, E0, E1, -1, -2>& _sv)
            {
                *this = __tvec3<T>(s, _sv());
            };


            __tvec3<T>& operator=(const __tvec3<T>& v);

            template <typename U>
            __tvec3<T>& operator=(const __tvec3<U> & v);
            template <typename U>
            __tvec3<T>& operator+=(U s);
            template <typename U>
            __tvec3<T>& operator+=(const __tvec3<U> & v);
            template <typename U>
            __tvec3<T>& operator-=(U s);
            template <typename U>
            __tvec3<T>& operator-=(const __tvec3<U> & v);
            template <typename U>
            __tvec3<T>& operator*=(U s);
            template <typename U>
            __tvec3<T>& operator*=(const __tvec3<U> & v);
            template <typename U>
            __tvec3<T>& operator/=(U s);
            template <typename U>
            __tvec3<T>& operator/=(const __tvec3<U> & v);
        };

        template <typename T>
        __tvec3<T> operator+(const __tvec3<T> & v, const T & s);

        template <typename T>
        __tvec3<T> operator+(const T & s, const __tvec3<T> & v);

        template <typename T>
        __tvec3<T> operator+(const __tvec3<T> & v1, const __tvec3<T> & v2);

        template <typename T>
        __tvec3<T> operator-(const __tvec3<T> & v, const T & s);

        template <typename T>
        __tvec3<T> operator-(const T & s, const __tvec3<T> & v);

        template <typename T>
        __tvec3<T> operator-(const __tvec3<T> & v1, const __tvec3<T> & v2);

        template <typename T>
        __tvec3<T> operator/(const __tvec3<T> & v, const T & s);

        template <typename T>
        __tvec3<T> operator/(const T & s, const __tvec3<T> & v);

        template <typename T>
        __tvec3<T> operator/(const __tvec3<T> & v1, const __tvec3<T> & v2);

        template <typename T>
        __tvec3<T> operator-(const __tvec3<T> & v);

        template <typename T>
        bool operator==(const __tvec3<T>& v1, const __tvec3<T> & v2);

        template <typename T>
        bool operator!=(const __tvec3<T>& v1, const __tvec3<T> & v2);

        template<typename T>
        std::ostream &operator<<(std::ostream &os, const __tvec3<T> &v)
        {
//            os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
            printf("[%-*.*f, %-*.*f, %-*.*f]", 12, 6, v.x, 12, 6, v.y, 12, 6, v.z);
            return os;
        }
    }
}

#include "type_vec3_inl.hpp"
#endif //QYT_FUNDATION_TYPE_VEC3_HPP
