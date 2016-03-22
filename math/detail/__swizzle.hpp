//
// Created by qyt on 16/2/16.
//

#ifndef QYT_FUNDATION_SWIZZLE_HPP
#define QYT_FUNDATION_SWIZZLE_HPP

/**
 * 为了实现glsl类似的向量成员访问,比如
 * vec4 x;
 * x.xzy = ...
 * 在这里实现swizzle方式
 */
#include <stddef.h>

template <typename T> struct __tvec2;
template <typename T> struct __tvec3;
template <typename T> struct __tvec4;

namespace qyt
{
    namespace __detail
    {
        /**
         * @class __swizzle_base
         * 这是一个仅仅用于内部实现的类,将这个类(的子类)放在__tvec*类的union成员中,
         * 已达到swizzle访问操作
         */
        template <typename T, int N>
        class __swizzle_base
        {
        protected:
            T __buffer[1];

            T& at(size_t i)
            {return (reinterpret_cast<T*>(__buffer))[i];}

            const T& at(size_t i) const
            { return (reinterpret_cast<const T*>(__buffer))[i];}
        };

        /**
         * @class __swizzle_type_wrapper
         * 提供一个可供访问特定数目向量成员的封装类,只提供定义,并不做任何实质的工作
         * 但是这个类有三种偏特化,对于不同的模板参数,这个类的特化可以提供针对不同维
         * 度成员访问的封装
         *
         * @param T 数据类型 e.g. float, double...
         * @param V 向量类型 e.g. __tvec*, vec2...
         * @param _e0~_e3 对应每次swizzle访问的成员操作符 e.g xy, xyz, rgb, xxy...
         * @param N 操作符维度 e.g. xy->2, xyz->3, xxy->3, wwww->4
         */
        template <typename T, typename V, int _e0, int _e1, int _e2, int _e3, int N>
        class __swizzle_type_wrapper:__swizzle_base<T, N>
        {};

        template <typename T, typename V, int _e0, int _e1>
        class __swizzle_type_wrapper<T, V, _e0, _e1, -1, -2, 2>:public __swizzle_base<T, 2>
        {
        public:
            V operator()()const
            { return V(this->at(_e0),this->at(_e1));}
        };

        template <typename T, typename V, int _e0, int _e1, int _e2>
        class __swizzle_type_wrapper<T, V, _e0, _e1, _e2, -1, 3>:public __swizzle_base<T, 3>
        {
        public:
            V operator()()const
            { return V(this->at(_e0),this->at(_e1), this->at(_e2));}
        };


        template <typename T, typename V, int _e0, int _e1, int _e2, int _e3>
        class __swizzle_type_wrapper<T, V, _e0, _e1, _e2, _e3, 4>:public __swizzle_base<T, 4>
        {
        public:
            V operator()()const
            { return V(this->at(_e0),this->at(_e1), this->at(_e2), this->at(_e3));}
        };

        ////////////////////////////////////////////////////////////////////////////////
        /**
         * @class __swizzle_opwrapper
         * 封装向量成员间的操作
         * e.g.
         * v1.xx += v2.xy;
         * ...
         *
         * 模板参数意义和__swizzle_type_wrapper一样,最后的参数DUPLICATE_ELEMENTS表示是否
         * 有重复下标
         *
         * @param DUPLICATE_ELEMENTS 如果操作符中有重复下标为1,否则为0
         *                           e.g. xyz->(0), xxw->(1)
         */
        template <typename T, typename V, int E0, int E1, int E2, int E3, int N, int DUPLICATE_ELEMENTS>
        class __swizzle_opwrapper:public __swizzle_type_wrapper<T, V, E0, E1, E2, E3, N>
        {
        public:
            T& operator[](size_t i)
            {
                const int __offset[4] = {E0, E1, E2, E3};
                return this->at(__offset[i]);
            }

            const T& operator[](size_t i) const
            {
                const int __offset[4] = {E0, E1, E2, E3};
                return this->at(__offset[i]);
            }

            __swizzle_opwrapper& operator=(const T& t)
            {
                for (int i = 0; i < N; ++i)
                    (*this)[i] = t;
                return *this;
            }

            __swizzle_opwrapper& operator=(const V& v)
            {
                struct __op
                { void operator()(T& s, T& d){s = d;} };
                __apply(v, __op());
                return *this;
            }

            __swizzle_opwrapper& operator-=(const V& v)
            {
                struct __op
                { void operator()(T& s, T& d){s -= d;} };
                __apply(v, __op());
                return *this;
            }

            __swizzle_opwrapper& operator+=(const V& v)
            {
                struct __op
                { void operator()(T& s, T& d){s += d;} };
                __apply(v, __op());
                return *this;
            }

            __swizzle_opwrapper& operator*=(const V& v)
            {
                struct __op
                { void operator()(T& s, T& d){s *= d;} };
                __apply(v, __op());
                return *this;
            }

            __swizzle_opwrapper& operator/=(const V& v)
            {
                struct __op
                { void operator()(T& s, T& d){s /= d;} };
                __apply(v, __op());
                return *this;
            }
        protected:

            /**
             * @function __apply
             * 如果赋值语句中出现下面的情况:
             * v.xyz = v.zyx;
             * 那么,一个必须的任务是对v的数据成员进行拷贝,函数__apply负责进行拷贝数据,然后
             * 进行相应的操作,该函数的调用直接导致调用对象的数据改变
             *
             * @param that operator的右操作数(向量)
             * @param op   仿函数对象
             */
            template <typename __function>
            void __apply(const V& that, __function op)
            {
                T __v[N];
                for (int i = 0; i < N; ++i)
                    __v[i] = that[i];
                for (int i = 0; i < N; ++i)
                    op((*this)[i], __v[i]);
            }
        };

        /**
         * @class __swizzle_opwrapper<T, V, E0, E1, E2, E3, N, 1>
         * 对于有重复下标元素的,不要修改它们...
         * e.g.
         * v1(1, 2);
         * v2(3, 4);
         * v1.xx = v2.xy;   //v1的值不会改变
         */
        template <typename T, typename V, int E0, int E1, int E2, int E3, int N>
        class __swizzle_opwrapper<T, V, E0, E1, E2, E3, N, 1>:public __swizzle_type_wrapper<T, V, E0, E1, E2, E3, N>
        {
        public:
            T operator[](size_t i) const
            {
                const int __offset[4] = {E0, E1, E2, E3};
                return this->at(__offset[i]);
            }

            __swizzle_opwrapper& operator=(const V&){ return *this;}
        };

#define __IS_DUPLICATE_ ((E0)==(E1)||(E0)==(E2)||(E0)==(E3)||(E1)==(E2)||(E1)==(E3)||(E2)==(E3))
        template <int N, typename T, typename V, int E0, int E1, int E2, int E3>
        class __swizzle:public __swizzle_opwrapper<T, V, E0, E1, E2, E3, N, __IS_DUPLICATE_>
        {
        public:
            using __swizzle_opwrapper<T, V, E0, E1, E2, E3, N, __IS_DUPLICATE_>::operator=;
            operator V() const
            {
                return (*this)();
            }
        };

		template<int N, typename T, typename V, int E0, int E1, int E2, int E3>
		std::ostream &operator<<(std::ostream &os, const __swizzle<N, T, V, E0, E1, E2, E3> &v)
		{
			os << v();
			return os;
		}
#undef __IS_DUPLICATE_



        /**
         * 定义工具支持__swizzle之间的操作
         * e.g.
         * __SWIZZLE_TEMPLATE_VEC_AND_VEC   -> u.xy + v.xy
         * __SWIZZLE_TEMPLATE_VEC_AND_VALUE -> 3.f + u.xy
         */
#define __SWIZZLE_TEMPLATE_VEC_AND_VALUE template<int N, typename T, typename V, int E0, int E1, int E2, int E3>
#define __SWIZZLE_TEMPLATE_VEC_AND_VEC   template<typename T, typename V, int E0, int E1, int E2, int E3, int F0, int F1, int F2, int F3, int N>
#define __SWIZZLE_TYPE_E                 __swizzle<N, T, V, E0, E1, E2, E3>
#define __SWIZZLE_TYPE_F                 __swizzle<N, T, V, F0, F1, F2, F3>


#define __SWIZZLE_OP_V_V(__op)                                                      \
        __SWIZZLE_TEMPLATE_VEC_AND_VEC                                              \
        V operator __op(const __SWIZZLE_TYPE_E& v1, const __SWIZZLE_TYPE_F& v2)     \
        {return v1() __op v2();}                                                    \
        __SWIZZLE_TEMPLATE_VEC_AND_VEC                                              \
        V operator __op(const __SWIZZLE_TYPE_E& v1, const V& v2)                    \
        {return v1() __op v2;}                                                      \
        __SWIZZLE_TEMPLATE_VEC_AND_VEC                                              \
        V operator __op(const V& v1, const __SWIZZLE_TYPE_E& v2)                    \
        {return v1 __op v2();}                                                      \

#define __SWIZZLE_OP_V_T(__op)                                                      \
        __SWIZZLE_TEMPLATE_VEC_AND_VALUE                                            \
        V operator __op(const __SWIZZLE_TYPE_E& v, const T& s)                      \
        {return v() __op s;}                                                        \
        __SWIZZLE_TEMPLATE_VEC_AND_VALUE                                            \
        V operator __op(const T& s, const __SWIZZLE_TYPE_E& v)                      \
        {return s __op v();}
        
        
        //添加操作符支持...
        __SWIZZLE_OP_V_V(+)
        __SWIZZLE_OP_V_T(+)
        __SWIZZLE_OP_V_V(-)
        __SWIZZLE_OP_V_T(-)
        __SWIZZLE_OP_V_V(*)
        __SWIZZLE_OP_V_T(*)
        __SWIZZLE_OP_V_V(/)
        __SWIZZLE_OP_V_T(/)
        

        /**
         * 根据排列组合,定义恰当的__swizzle对象,并将它们封装到__tvec*中
         * 例如,对于__tvec2,其__swizzle有四个(不考虑r,g分量和s,t分量等),分别是
         * xx,xy,yy,yx
         * 那么对应的__swizzle便是
         * 0,0
         * 0,1
         * 1,0
         * 1,1
         * 其他维度的__swizzle也以此类推
         */
#define __GENERATE_SWIZZLE_2_2(T, V, E0, E1)                                     \
        struct{__swizzle<2, T, V<T>, 0, 0, -1, -2> E0 ## E0;};                   \
        struct{__swizzle<2, T, V<T>, 0, 1, -1, -2> E0 ## E1;};                   \
        struct{__swizzle<2, T, V<T>, 1, 1, -1, -2> E1 ## E1;};                   \
        struct{__swizzle<2, T, V<T>, 1, 0, -1, -2> E1 ## E0;};

#define __GENERATE_SWIZZLE_2_3(T,  V, E0,E1) \
	struct { __swizzle<3,T,V<T>, 0,0,0,-1> E0 ## E0 ## E0; }; \
	struct { __swizzle<3,T,V<T>, 0,0,1,-1> E0 ## E0 ## E1; }; \
	struct { __swizzle<3,T,V<T>, 0,1,0,-1> E0 ## E1 ## E0; }; \
	struct { __swizzle<3,T,V<T>, 0,1,1,-1> E0 ## E1 ## E1; }; \
	struct { __swizzle<3,T,V<T>, 1,0,0,-1> E1 ## E0 ## E0; }; \
	struct { __swizzle<3,T,V<T>, 1,0,1,-1> E1 ## E0 ## E1; }; \
	struct { __swizzle<3,T,V<T>, 1,1,0,-1> E1 ## E1 ## E0; }; \
	struct { __swizzle<3,T,V<T>, 1,1,1,-1> E1 ## E1 ## E1; };

#define __GENERATE_SWIZZLE_2_4(T,  V, E0,E1) \
	struct { __swizzle<4,T,V<T>, 0,0,0,0> E0 ## E0 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,0,0,1> E0 ## E0 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,0,1,0> E0 ## E0 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,0,1,1> E0 ## E0 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,1,0,0> E0 ## E1 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,1,0,1> E0 ## E1 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,1,1,0> E0 ## E1 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,1,1,1> E0 ## E1 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,0,0,0> E1 ## E0 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,0,0,1> E1 ## E0 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,0,1,0> E1 ## E0 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,0,1,1> E1 ## E0 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,1,0,0> E1 ## E1 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,1,0,1> E1 ## E1 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,1,1,0> E1 ## E1 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,1,1,1> E1 ## E1 ## E1 ## E1; };

#define __GENERATE_SWIZZLE_3_2(T,  V, E0,E1,E2) \
	struct { __swizzle<2,T,V<T>, 0,0,-1,-2> E0 ## E0; }; \
	struct { __swizzle<2,T,V<T>, 0,1,-1,-2> E0 ## E1; }; \
	struct { __swizzle<2,T,V<T>, 0,2,-1,-2> E0 ## E2; }; \
	struct { __swizzle<2,T,V<T>, 1,0,-1,-2> E1 ## E0; }; \
	struct { __swizzle<2,T,V<T>, 1,1,-1,-2> E1 ## E1; }; \
	struct { __swizzle<2,T,V<T>, 1,2,-1,-2> E1 ## E2; }; \
	struct { __swizzle<2,T,V<T>, 2,0,-1,-2> E2 ## E0; }; \
	struct { __swizzle<2,T,V<T>, 2,1,-1,-2> E2 ## E1; }; \
	struct { __swizzle<2,T,V<T>, 2,2,-1,-2> E2 ## E2; };

#define __GENERATE_SWIZZLE_3_3(T,  V ,E0,E1,E2) \
	struct { __swizzle<3,T, V<T>, 0,0,0,-1> E0 ## E0 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 0,0,1,-1> E0 ## E0 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 0,0,2,-1> E0 ## E0 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 0,1,0,-1> E0 ## E1 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 0,1,1,-1> E0 ## E1 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 0,1,2,-1> E0 ## E1 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 0,2,0,-1> E0 ## E2 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 0,2,1,-1> E0 ## E2 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 0,2,2,-1> E0 ## E2 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 1,0,0,-1> E1 ## E0 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 1,0,1,-1> E1 ## E0 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 1,0,2,-1> E1 ## E0 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 1,1,0,-1> E1 ## E1 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 1,1,1,-1> E1 ## E1 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 1,1,2,-1> E1 ## E1 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 1,2,0,-1> E1 ## E2 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 1,2,1,-1> E1 ## E2 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 1,2,2,-1> E1 ## E2 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 2,0,0,-1> E2 ## E0 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 2,0,1,-1> E2 ## E0 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 2,0,2,-1> E2 ## E0 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 2,1,0,-1> E2 ## E1 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 2,1,1,-1> E2 ## E1 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 2,1,2,-1> E2 ## E1 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 2,2,0,-1> E2 ## E2 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 2,2,1,-1> E2 ## E2 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 2,2,2,-1> E2 ## E2 ## E2; };

#define __GENERATE_SWIZZLE_3_4(T,  V, E0,E1,E2) \
	struct { __swizzle<4,T,V<T>, 0,0,0,0> E0 ## E0 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,0,0,1> E0 ## E0 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,0,0,2> E0 ## E0 ## E0 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 0,0,1,0> E0 ## E0 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,0,1,1> E0 ## E0 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,0,1,2> E0 ## E0 ## E1 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 0,0,2,0> E0 ## E0 ## E2 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,0,2,1> E0 ## E0 ## E2 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,0,2,2> E0 ## E0 ## E2 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 0,1,0,0> E0 ## E1 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,1,0,1> E0 ## E1 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,1,0,2> E0 ## E1 ## E0 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 0,1,1,0> E0 ## E1 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,1,1,1> E0 ## E1 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,1,1,2> E0 ## E1 ## E1 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 0,1,2,0> E0 ## E1 ## E2 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,1,2,1> E0 ## E1 ## E2 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,1,2,2> E0 ## E1 ## E2 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 0,2,0,0> E0 ## E2 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,2,0,1> E0 ## E2 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,2,0,2> E0 ## E2 ## E0 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 0,2,1,0> E0 ## E2 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,2,1,1> E0 ## E2 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,2,1,2> E0 ## E2 ## E1 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 0,2,2,0> E0 ## E2 ## E2 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 0,2,2,1> E0 ## E2 ## E2 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 0,2,2,2> E0 ## E2 ## E2 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 1,0,0,0> E1 ## E0 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,0,0,1> E1 ## E0 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,0,0,2> E1 ## E0 ## E0 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 1,0,1,0> E1 ## E0 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,0,1,1> E1 ## E0 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,0,1,2> E1 ## E0 ## E1 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 1,0,2,0> E1 ## E0 ## E2 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,0,2,1> E1 ## E0 ## E2 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,0,2,2> E1 ## E0 ## E2 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 1,1,0,0> E1 ## E1 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,1,0,1> E1 ## E1 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,1,0,2> E1 ## E1 ## E0 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 1,1,1,0> E1 ## E1 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,1,1,1> E1 ## E1 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,1,1,2> E1 ## E1 ## E1 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 1,1,2,0> E1 ## E1 ## E2 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,1,2,1> E1 ## E1 ## E2 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,1,2,2> E1 ## E1 ## E2 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 1,2,0,0> E1 ## E2 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,2,0,1> E1 ## E2 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,2,0,2> E1 ## E2 ## E0 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 1,2,1,0> E1 ## E2 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,2,1,1> E1 ## E2 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,2,1,2> E1 ## E2 ## E1 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 1,2,2,0> E1 ## E2 ## E2 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 1,2,2,1> E1 ## E2 ## E2 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 1,2,2,2> E1 ## E2 ## E2 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 2,0,0,0> E2 ## E0 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 2,0,0,1> E2 ## E0 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 2,0,0,2> E2 ## E0 ## E0 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 2,0,1,0> E2 ## E0 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 2,0,1,1> E2 ## E0 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 2,0,1,2> E2 ## E0 ## E1 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 2,0,2,0> E2 ## E0 ## E2 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 2,0,2,1> E2 ## E0 ## E2 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 2,0,2,2> E2 ## E0 ## E2 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 2,1,0,0> E2 ## E1 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 2,1,0,1> E2 ## E1 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 2,1,0,2> E2 ## E1 ## E0 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 2,1,1,0> E2 ## E1 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 2,1,1,1> E2 ## E1 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 2,1,1,2> E2 ## E1 ## E1 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 2,1,2,0> E2 ## E1 ## E2 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 2,1,2,1> E2 ## E1 ## E2 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 2,1,2,2> E2 ## E1 ## E2 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 2,2,0,0> E2 ## E2 ## E0 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 2,2,0,1> E2 ## E2 ## E0 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 2,2,0,2> E2 ## E2 ## E0 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 2,2,1,0> E2 ## E2 ## E1 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 2,2,1,1> E2 ## E2 ## E1 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 2,2,1,2> E2 ## E2 ## E1 ## E2; }; \
	struct { __swizzle<4,T,V<T>, 2,2,2,0> E2 ## E2 ## E2 ## E0; }; \
	struct { __swizzle<4,T,V<T>, 2,2,2,1> E2 ## E2 ## E2 ## E1; }; \
	struct { __swizzle<4,T,V<T>, 2,2,2,2> E2 ## E2 ## E2 ## E2; };

#define __GENERATE_SWIZZLE_4_2(T,  V, E0,E1,E2,E3) \
	struct { __swizzle<2,T,V<T>, 0,0,-1,-2> E0 ## E0; }; \
	struct { __swizzle<2,T,V<T>, 0,1,-1,-2> E0 ## E1; }; \
	struct { __swizzle<2,T,V<T>, 0,2,-1,-2> E0 ## E2; }; \
	struct { __swizzle<2,T,V<T>, 0,3,-1,-2> E0 ## E3; }; \
	struct { __swizzle<2,T,V<T>, 1,0,-1,-2> E1 ## E0; }; \
	struct { __swizzle<2,T,V<T>, 1,1,-1,-2> E1 ## E1; }; \
	struct { __swizzle<2,T,V<T>, 1,2,-1,-2> E1 ## E2; }; \
	struct { __swizzle<2,T,V<T>, 1,3,-1,-2> E1 ## E3; }; \
	struct { __swizzle<2,T,V<T>, 2,0,-1,-2> E2 ## E0; }; \
	struct { __swizzle<2,T,V<T>, 2,1,-1,-2> E2 ## E1; }; \
	struct { __swizzle<2,T,V<T>, 2,2,-1,-2> E2 ## E2; }; \
	struct { __swizzle<2,T,V<T>, 2,3,-1,-2> E2 ## E3; }; \
	struct { __swizzle<2,T,V<T>, 3,0,-1,-2> E3 ## E0; }; \
	struct { __swizzle<2,T,V<T>, 3,1,-1,-2> E3 ## E1; }; \
	struct { __swizzle<2,T,V<T>, 3,2,-1,-2> E3 ## E2; }; \
	struct { __swizzle<2,T,V<T>, 3,3,-1,-2> E3 ## E3; };

#define __GENERATE_SWIZZLE_4_3(T, V, E0,E1,E2,E3) \
	struct { __swizzle<3,T, V<T>, 0,0,0,-1> E0 ## E0 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 0,0,1,-1> E0 ## E0 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 0,0,2,-1> E0 ## E0 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 0,0,3,-1> E0 ## E0 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 0,1,0,-1> E0 ## E1 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 0,1,1,-1> E0 ## E1 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 0,1,2,-1> E0 ## E1 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 0,1,3,-1> E0 ## E1 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 0,2,0,-1> E0 ## E2 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 0,2,1,-1> E0 ## E2 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 0,2,2,-1> E0 ## E2 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 0,2,3,-1> E0 ## E2 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 0,3,0,-1> E0 ## E3 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 0,3,1,-1> E0 ## E3 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 0,3,2,-1> E0 ## E3 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 0,3,3,-1> E0 ## E3 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 1,0,0,-1> E1 ## E0 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 1,0,1,-1> E1 ## E0 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 1,0,2,-1> E1 ## E0 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 1,0,3,-1> E1 ## E0 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 1,1,0,-1> E1 ## E1 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 1,1,1,-1> E1 ## E1 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 1,1,2,-1> E1 ## E1 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 1,1,3,-1> E1 ## E1 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 1,2,0,-1> E1 ## E2 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 1,2,1,-1> E1 ## E2 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 1,2,2,-1> E1 ## E2 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 1,2,3,-1> E1 ## E2 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 1,3,0,-1> E1 ## E3 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 1,3,1,-1> E1 ## E3 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 1,3,2,-1> E1 ## E3 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 1,3,3,-1> E1 ## E3 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 2,0,0,-1> E2 ## E0 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 2,0,1,-1> E2 ## E0 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 2,0,2,-1> E2 ## E0 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 2,0,3,-1> E2 ## E0 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 2,1,0,-1> E2 ## E1 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 2,1,1,-1> E2 ## E1 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 2,1,2,-1> E2 ## E1 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 2,1,3,-1> E2 ## E1 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 2,2,0,-1> E2 ## E2 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 2,2,1,-1> E2 ## E2 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 2,2,2,-1> E2 ## E2 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 2,2,3,-1> E2 ## E2 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 2,3,0,-1> E2 ## E3 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 2,3,1,-1> E2 ## E3 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 2,3,2,-1> E2 ## E3 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 2,3,3,-1> E2 ## E3 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 3,0,0,-1> E3 ## E0 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 3,0,1,-1> E3 ## E0 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 3,0,2,-1> E3 ## E0 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 3,0,3,-1> E3 ## E0 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 3,1,0,-1> E3 ## E1 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 3,1,1,-1> E3 ## E1 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 3,1,2,-1> E3 ## E1 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 3,1,3,-1> E3 ## E1 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 3,2,0,-1> E3 ## E2 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 3,2,1,-1> E3 ## E2 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 3,2,2,-1> E3 ## E2 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 3,2,3,-1> E3 ## E2 ## E3; }; \
	struct { __swizzle<3,T, V<T>, 3,3,0,-1> E3 ## E3 ## E0; }; \
	struct { __swizzle<3,T, V<T>, 3,3,1,-1> E3 ## E3 ## E1; }; \
	struct { __swizzle<3,T, V<T>, 3,3,2,-1> E3 ## E3 ## E2; }; \
	struct { __swizzle<3,T, V<T>, 3,3,3,-1> E3 ## E3 ## E3; };

#define __GENERATE_SWIZZLE_4_4(T,  V, E0,E1,E2,E3) \
	struct { __swizzle<4, T,V<T>, 0,0,0,0> E0 ## E0 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,0,0,1> E0 ## E0 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,0,0,2> E0 ## E0 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,0,0,3> E0 ## E0 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,0,1,0> E0 ## E0 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,0,1,1> E0 ## E0 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,0,1,2> E0 ## E0 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,0,1,3> E0 ## E0 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,0,2,0> E0 ## E0 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,0,2,1> E0 ## E0 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,0,2,2> E0 ## E0 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,0,2,3> E0 ## E0 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,0,3,0> E0 ## E0 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,0,3,1> E0 ## E0 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,0,3,2> E0 ## E0 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,0,3,3> E0 ## E0 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,1,0,0> E0 ## E1 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,1,0,1> E0 ## E1 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,1,0,2> E0 ## E1 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,1,0,3> E0 ## E1 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,1,1,0> E0 ## E1 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,1,1,1> E0 ## E1 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,1,1,2> E0 ## E1 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,1,1,3> E0 ## E1 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,1,2,0> E0 ## E1 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,1,2,1> E0 ## E1 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,1,2,2> E0 ## E1 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,1,2,3> E0 ## E1 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,1,3,0> E0 ## E1 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,1,3,1> E0 ## E1 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,1,3,2> E0 ## E1 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,1,3,3> E0 ## E1 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,2,0,0> E0 ## E2 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,2,0,1> E0 ## E2 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,2,0,2> E0 ## E2 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,2,0,3> E0 ## E2 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,2,1,0> E0 ## E2 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,2,1,1> E0 ## E2 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,2,1,2> E0 ## E2 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,2,1,3> E0 ## E2 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,2,2,0> E0 ## E2 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,2,2,1> E0 ## E2 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,2,2,2> E0 ## E2 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,2,2,3> E0 ## E2 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,2,3,0> E0 ## E2 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,2,3,1> E0 ## E2 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,2,3,2> E0 ## E2 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,2,3,3> E0 ## E2 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,3,0,0> E0 ## E3 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,3,0,1> E0 ## E3 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,3,0,2> E0 ## E3 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,3,0,3> E0 ## E3 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,3,1,0> E0 ## E3 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,3,1,1> E0 ## E3 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,3,1,2> E0 ## E3 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,3,1,3> E0 ## E3 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,3,2,0> E0 ## E3 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,3,2,1> E0 ## E3 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,3,2,2> E0 ## E3 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,3,2,3> E0 ## E3 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 0,3,3,0> E0 ## E3 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 0,3,3,1> E0 ## E3 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 0,3,3,2> E0 ## E3 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 0,3,3,3> E0 ## E3 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,0,0,0> E1 ## E0 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,0,0,1> E1 ## E0 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,0,0,2> E1 ## E0 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,0,0,3> E1 ## E0 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,0,1,0> E1 ## E0 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,0,1,1> E1 ## E0 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,0,1,2> E1 ## E0 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,0,1,3> E1 ## E0 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,0,2,0> E1 ## E0 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,0,2,1> E1 ## E0 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,0,2,2> E1 ## E0 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,0,2,3> E1 ## E0 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,0,3,0> E1 ## E0 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,0,3,1> E1 ## E0 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,0,3,2> E1 ## E0 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,0,3,3> E1 ## E0 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,1,0,0> E1 ## E1 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,1,0,1> E1 ## E1 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,1,0,2> E1 ## E1 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,1,0,3> E1 ## E1 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,1,1,0> E1 ## E1 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,1,1,1> E1 ## E1 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,1,1,2> E1 ## E1 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,1,1,3> E1 ## E1 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,1,2,0> E1 ## E1 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,1,2,1> E1 ## E1 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,1,2,2> E1 ## E1 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,1,2,3> E1 ## E1 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,1,3,0> E1 ## E1 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,1,3,1> E1 ## E1 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,1,3,2> E1 ## E1 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,1,3,3> E1 ## E1 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,2,0,0> E1 ## E2 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,2,0,1> E1 ## E2 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,2,0,2> E1 ## E2 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,2,0,3> E1 ## E2 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,2,1,0> E1 ## E2 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,2,1,1> E1 ## E2 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,2,1,2> E1 ## E2 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,2,1,3> E1 ## E2 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,2,2,0> E1 ## E2 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,2,2,1> E1 ## E2 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,2,2,2> E1 ## E2 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,2,2,3> E1 ## E2 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,2,3,0> E1 ## E2 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,2,3,1> E1 ## E2 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,2,3,2> E1 ## E2 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,2,3,3> E1 ## E2 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,3,0,0> E1 ## E3 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,3,0,1> E1 ## E3 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,3,0,2> E1 ## E3 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,3,0,3> E1 ## E3 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,3,1,0> E1 ## E3 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,3,1,1> E1 ## E3 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,3,1,2> E1 ## E3 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,3,1,3> E1 ## E3 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,3,2,0> E1 ## E3 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,3,2,1> E1 ## E3 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,3,2,2> E1 ## E3 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,3,2,3> E1 ## E3 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 1,3,3,0> E1 ## E3 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 1,3,3,1> E1 ## E3 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 1,3,3,2> E1 ## E3 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 1,3,3,3> E1 ## E3 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,0,0,0> E2 ## E0 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,0,0,1> E2 ## E0 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,0,0,2> E2 ## E0 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,0,0,3> E2 ## E0 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,0,1,0> E2 ## E0 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,0,1,1> E2 ## E0 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,0,1,2> E2 ## E0 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,0,1,3> E2 ## E0 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,0,2,0> E2 ## E0 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,0,2,1> E2 ## E0 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,0,2,2> E2 ## E0 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,0,2,3> E2 ## E0 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,0,3,0> E2 ## E0 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,0,3,1> E2 ## E0 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,0,3,2> E2 ## E0 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,0,3,3> E2 ## E0 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,1,0,0> E2 ## E1 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,1,0,1> E2 ## E1 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,1,0,2> E2 ## E1 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,1,0,3> E2 ## E1 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,1,1,0> E2 ## E1 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,1,1,1> E2 ## E1 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,1,1,2> E2 ## E1 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,1,1,3> E2 ## E1 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,1,2,0> E2 ## E1 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,1,2,1> E2 ## E1 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,1,2,2> E2 ## E1 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,1,2,3> E2 ## E1 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,1,3,0> E2 ## E1 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,1,3,1> E2 ## E1 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,1,3,2> E2 ## E1 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,1,3,3> E2 ## E1 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,2,0,0> E2 ## E2 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,2,0,1> E2 ## E2 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,2,0,2> E2 ## E2 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,2,0,3> E2 ## E2 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,2,1,0> E2 ## E2 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,2,1,1> E2 ## E2 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,2,1,2> E2 ## E2 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,2,1,3> E2 ## E2 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,2,2,0> E2 ## E2 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,2,2,1> E2 ## E2 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,2,2,2> E2 ## E2 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,2,2,3> E2 ## E2 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,2,3,0> E2 ## E2 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,2,3,1> E2 ## E2 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,2,3,2> E2 ## E2 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,2,3,3> E2 ## E2 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,3,0,0> E2 ## E3 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,3,0,1> E2 ## E3 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,3,0,2> E2 ## E3 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,3,0,3> E2 ## E3 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,3,1,0> E2 ## E3 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,3,1,1> E2 ## E3 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,3,1,2> E2 ## E3 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,3,1,3> E2 ## E3 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,3,2,0> E2 ## E3 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,3,2,1> E2 ## E3 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,3,2,2> E2 ## E3 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,3,2,3> E2 ## E3 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 2,3,3,0> E2 ## E3 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 2,3,3,1> E2 ## E3 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 2,3,3,2> E2 ## E3 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 2,3,3,3> E2 ## E3 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,0,0,0> E3 ## E0 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,0,0,1> E3 ## E0 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,0,0,2> E3 ## E0 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,0,0,3> E3 ## E0 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,0,1,0> E3 ## E0 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,0,1,1> E3 ## E0 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,0,1,2> E3 ## E0 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,0,1,3> E3 ## E0 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,0,2,0> E3 ## E0 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,0,2,1> E3 ## E0 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,0,2,2> E3 ## E0 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,0,2,3> E3 ## E0 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,0,3,0> E3 ## E0 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,0,3,1> E3 ## E0 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,0,3,2> E3 ## E0 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,0,3,3> E3 ## E0 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,1,0,0> E3 ## E1 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,1,0,1> E3 ## E1 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,1,0,2> E3 ## E1 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,1,0,3> E3 ## E1 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,1,1,0> E3 ## E1 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,1,1,1> E3 ## E1 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,1,1,2> E3 ## E1 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,1,1,3> E3 ## E1 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,1,2,0> E3 ## E1 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,1,2,1> E3 ## E1 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,1,2,2> E3 ## E1 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,1,2,3> E3 ## E1 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,1,3,0> E3 ## E1 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,1,3,1> E3 ## E1 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,1,3,2> E3 ## E1 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,1,3,3> E3 ## E1 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,2,0,0> E3 ## E2 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,2,0,1> E3 ## E2 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,2,0,2> E3 ## E2 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,2,0,3> E3 ## E2 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,2,1,0> E3 ## E2 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,2,1,1> E3 ## E2 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,2,1,2> E3 ## E2 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,2,1,3> E3 ## E2 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,2,2,0> E3 ## E2 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,2,2,1> E3 ## E2 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,2,2,2> E3 ## E2 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,2,2,3> E3 ## E2 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,2,3,0> E3 ## E2 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,2,3,1> E3 ## E2 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,2,3,2> E3 ## E2 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,2,3,3> E3 ## E2 ## E3 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,3,0,0> E3 ## E3 ## E0 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,3,0,1> E3 ## E3 ## E0 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,3,0,2> E3 ## E3 ## E0 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,3,0,3> E3 ## E3 ## E0 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,3,1,0> E3 ## E3 ## E1 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,3,1,1> E3 ## E3 ## E1 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,3,1,2> E3 ## E3 ## E1 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,3,1,3> E3 ## E3 ## E1 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,3,2,0> E3 ## E3 ## E2 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,3,2,1> E3 ## E3 ## E2 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,3,2,2> E3 ## E3 ## E2 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,3,2,3> E3 ## E3 ## E2 ## E3; }; \
	struct { __swizzle<4, T,V<T>, 3,3,3,0> E3 ## E3 ## E3 ## E0; }; \
	struct { __swizzle<4, T,V<T>, 3,3,3,1> E3 ## E3 ## E3 ## E1; }; \
	struct { __swizzle<4, T,V<T>, 3,3,3,2> E3 ## E3 ## E3 ## E2; }; \
	struct { __swizzle<4, T,V<T>, 3,3,3,3> E3 ## E3 ## E3 ## E3; };

    }
}

#endif //QYT_FUNDATION_SWIZZLE_HPP
