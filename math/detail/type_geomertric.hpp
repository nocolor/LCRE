//
//  type_geomertric.h
//  qyt_fundation
//
//  Created by qyt on 16/2/23.
//  Copyright © 2016年 qyt. All rights reserved.
//

#ifndef type_geomertric_h
#define type_geomertric_h

#include "__sse_geometric.hpp"
#include <cmath>
#include "type_vec2.hpp"
#include "type_vec3.hpp"
#include "type_vec4.hpp"

namespace qyt
{
    namespace __detail
    {
        //dot
        //----------------------------------------------------------------------
        template <template <class> class V, typename T>
        struct __dot{};
        
        template <typename T>
        struct __dot<__tvec2, T>
        {
            static T __call(const __tvec2<T>& v0, const __tvec2<T>& v1)
            {
                __tvec2<T> __t(v0*v1);
                return __t.x+__t.y;
            }
        };
        
        template <typename T>
        struct __dot<__tvec3, T>
        {
            static T __call(const __tvec3<T>& v0, const __tvec3<T>& v1)
            {
                __tvec3<T> __t(v0*v1);
                return __t.x+__t.y+__t.z;
            }
        };
        
        template <typename T>
        struct __dot<__tvec4, T>
        {
            static T __call(const __tvec4<T>& v0, const __tvec4<T>& v1)
            {
                __tvec4<T> __t(v0*v1);
                return __t.x+__t.y+__t.z+__t.w;
            }
        };
        
        template <>
        struct __dot<__tvec4, float>
        {
            static float __call(const __tvec4<float>& v0, const __tvec4<float>& v1)
            {
                return __sse_dot_ps(v0.data, v1.data)[0];
            }
        };
        
        //cross
        //----------------------------------------------------------------------
        template <template <class> class V, typename T>
        struct __cross{};
        
        template <typename T>
        struct __cross<__tvec3, T>
        {
            static __tvec3<T> __call(const __tvec3<T>& v0, const __tvec3<T>& v1)
            {
                return __tvec3<T>(v0.y * v1.z - v1.y * v0.z,
                                  v0.z * v1.x - v1.z * v0.x,
                                  v0.x * v1.y - v1.x * v0.y);
            }
        };
        
        template <>
        struct __cross<__tvec4, float>
        {
            static __tvec4<float> __call(const __tvec4<float>& v0, const __tvec4<float>& v1)
            {
                return __tvec4<float>(__sse_cross_ps(v0.data, v1.data));
            }
        };
        
        //length
        //----------------------------------------------------------------------
        template <template <class> class V, typename T>
        struct __length{};
        
        template <typename T>
        struct __length<__tvec2, T>
        {
            static T __call(const __tvec2<T>& v)
            {
                return static_cast<T>(sqrt(__detail::__dot<__tvec2, T>::__call(v,v)));
            }
        };
        
        template <typename T>
        struct __length<__tvec3, T>
        {
            static T __call(const __tvec3<T>& v)
            {
                return static_cast<T>(sqrt(__detail::__dot<__tvec3, T>::__call(v, v)));
            }
        };
        
        template <typename T>
        struct __length<__tvec4, T>
        {
            static T __call(const __tvec4<T>& v)
            {
                return static_cast<T>(sqrt(__detail::__dot<__tvec4, T>::__call(v, v)));
            }
        };
        
        template <>
        struct __length<__tvec4, float>
        {
            static float __call(const __tvec4<float>& v)
            {
                return __detail::__sse_length_ps(v.data)[0];
            }
        };

        //normalize
        //----------------------------------------------------------------------
        template <template <class> class V, typename T>
        struct __normalize{};
        
        template <typename T>
        struct __normalize<__tvec2, T>
        {
            static __tvec2<T> __call(const __tvec2<T>& v)
            {
                return v/__detail::__length<__tvec2, T>::__call(v);
            }
        };
        
        template <typename T>
        struct __normalize<__tvec3, T>
        {
            static __tvec3<T> __call(const __tvec3<T>& v)
            {
                return v/__detail::__length<__tvec3, T>::__call(v);
            }
        };
        
        template <typename T>
        struct __normalize<__tvec4, T>
        {
            static __tvec4<T> __call(const __tvec4<T>& v)
            {
                return v/__detail::__length<__tvec4, T>::__call(v);
            }
        };
        
        template <>
        struct __normalize<__tvec4, float>
        {
            static __tvec4<float> __call(const __tvec4<float>& v)
            {
                return __tvec4<float>(__detail::__sse_normalize_ps(v.data));
            }
        };
        
    }
    
//func(u, v)
#define __vec_func_wraper_template_v_v  template <typename T, template <class> class V>
//func(u.xyz, v.xyz)
#define __vec_func_wraper_template_s_s template <int N, typename T, typename V, int E0,\
                                                 int E1, int E2, int E3, int F0, int F1,\
                                                 int F2, int F3>
//func(u.xyz, v)
#define __vec_func_wraper_template_s_v template <int N, typename T, typename V, int E0,\
                                                 int E1, int E2, int E3,\
                                                 template <class> class VEC>

//func(u, v.xyz)
#define __vec_func_wraper_template_v_s template <template <class> class VEC,\
                                                 int N, typename T, typename V, int E0,\
                                                 int E1, int E2, int E3>
//func(v)
#define __vec_func_wraper_template_v    template <typename T, template <class> class V>
//func(v.xyz)
#define __vec_func_wraper_template_s    template <int N, typename T, typename V, int E0, int E1, int E2, int E3>
    

#define __vec_func_wraper_arg2_v_v(__return_type, __func) \
__vec_func_wraper_template_v_v \
__return_type __func ( const V<T>& v0, const V<T>& v1 ) \
{ \
   return __detail::__##__func<V, T>::__call(v0, v1); \
}
    
#define __vec_func_wraper_arg2_s_s(__return_type, __func) \
__vec_func_wraper_template_s_s \
__return_type __func(const __detail::__swizzle<N, T, V, E0, E1, E2, E3>& v0, \
                     const __detail::__swizzle<N, T, V, F0, F1, F2, F3>& v1) \
{ \
    return qyt::__func(v0(), v1()); \
}
    
#define __vec_func_wraper_arg2_s_v(__return_type, __func) \
__vec_func_wraper_template_s_v \
__return_type __func(const __detail::__swizzle<N, T, V, E0, E1, E2, E3>& v0, \
                     const VEC<T>& v1) \
{ \
    return qyt::__func(v0(), v1); \
}

#define __vec_func_wraper_arg2_v_s(__return_type, __func) \
__vec_func_wraper_template_v_s \
__return_type __func(const VEC<T>& v0, \
                     const __detail::__swizzle<N, T, V, E0, E1, E2, E3>& v1) \
{ \
    return qyt::__func(v0, v1()); \
} \

#define __vec_func_wraper_arg1_v(__return_type, __func) \
__vec_func_wraper_template_v \
__return_type __func(const V<T>& v) \
{\
   return __detail::__##__func<V, T>::__call(v); \
}
    
#define __vec_func_wraper_arg1_s(__return_type, __func) \
__vec_func_wraper_template_s \
__return_type __func(const __detail::__swizzle<N, T, V, E0, E1, E2, E3>& v) \
{ \
    return qyt::__func(v()); \
}

    __vec_func_wraper_arg2_v_v(T, dot)
    __vec_func_wraper_arg2_s_s(T, dot)
    __vec_func_wraper_arg2_s_v(T, dot)
    __vec_func_wraper_arg2_v_s(T, dot)
    
    __vec_func_wraper_arg2_v_v(V<T>, cross)
    __vec_func_wraper_arg2_s_s(V, cross)
    __vec_func_wraper_arg2_s_v(V, cross)
    __vec_func_wraper_arg2_v_s(V, cross)
    
    __vec_func_wraper_arg1_v(T, length)
    __vec_func_wraper_arg1_s(T, length)
    
    __vec_func_wraper_arg1_v(V<T>, normalize)
    __vec_func_wraper_arg1_s(V, normalize)

}


#endif /* type_geomertric_h */
