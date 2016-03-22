//
//  matrix_transform.hpp
//  qyt_fundation
//
//  Created by qyt on 16/2/23.
//  Copyright © 2016年 qyt. All rights reserved.
//

#ifndef matrix_transform_h
#define matrix_transform_h

#include "type_matrix4x4.hpp"
namespace qyt
{
    template <typename T> using tvec3   = __detail::__tvec3<T>;
    template <typename T> using tmat4x4 = __detail::__tmat4x4<T>;
    
    
    //translate
    //--------------------------------------------------------------------------
    template <typename T>
    tmat4x4<T> translate(const tmat4x4<T>& m, const tvec3<T>& v)
    {
        tmat4x4<T> res(m);
        res[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
        return res;
    }
    
    template <int N, typename T, typename V, int E0, int E1, int E2, int E3>
    tmat4x4<T> translate(const tmat4x4<T>& m, const __detail::__swizzle<N, T, V, E0, E1, E2, E3>& v)
    {
        return translate(m, v());
    }
    
    //rotate
    //--------------------------------------------------------------------------
    template <typename T>
    tmat4x4<T> rotate(const tmat4x4<T>& m, T angle, const tvec3<T>& v)
    {
        T const a = angle;
        T const c = cos(a);
        T const s = sin(a);
        
        tvec3<T> axis(normalize(v));
        tvec3<T> temp((T(1) - c) * axis);
        
        tmat4x4<T> rotate;
        rotate[0][0] = c + temp[0] * axis[0];
        rotate[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
        rotate[0][2] = 0 + temp[0] * axis[2] - s * axis[1];
        
        rotate[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
        rotate[1][1] = c + temp[1] * axis[1];
        rotate[1][2] = 0 + temp[1] * axis[2] + s * axis[0];
        
        rotate[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
        rotate[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
        rotate[2][2] = c + temp[2] * axis[2];
        
        tmat4x4<T> result;
        result[0] = m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2];
        result[1] = m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2];
        result[2] = m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2];
        result[3] = m[3];
        return result;
    }
    
    template <int N, typename T, typename V, int E0, int E1, int E2, int E3>
    tmat4x4<T> rotate(const tmat4x4<T>& m, T angle, const __detail::__swizzle<N, T, V, E0, E1, E2, E3>& v)
    {
        return rotate(m, angle, v());
    }
    
    //scale
    //--------------------------------------------------------------------------
    template <typename T>
    tmat4x4<T> scale(const tmat4x4<T>& m, const tvec3<T>& v)
    {
        tmat4x4<T> res;
        res[0] = m[0] * v[0];
        res[1] = m[1] * v[1];
        res[2] = m[2] * v[2];
        res[3] = m[3];
        return res;
    }
    
    template <int N, typename T, typename V, int E0, int E1, int E2, int E3>
    tmat4x4<T> scale(const tmat4x4<T>& m, const __detail::__swizzle<N, T, V, E0, E1, E2, E3>& v)
    {
        return scale(m, v());
    }
    
    //ortho
    //--------------------------------------------------------------------------
    template <typename T>
    tmat4x4<T> ortho(T left,
                     T right,
                     T bottom,
                     T top,
                     T zNear,
                     T zFar)
    {
        tmat4x4<T> result(1);
        result[0][0] = static_cast<T>(2) / (right - left);
        result[1][1] = static_cast<T>(2) / (top - bottom);
        result[2][2] = - static_cast<T>(2) / (zFar - zNear);
        result[3][0] = - (right + left) / (right - left);
        result[3][1] = - (top + bottom) / (top - bottom);
        result[3][2] = - (zFar + zNear) / (zFar - zNear);
        return result;
    }
    
    template <typename T>
    tmat4x4<T> ortho(T left,
                     T right,
                     T bottom,
                     T top)
    {
        tmat4x4<T> result(1);
        result[0][0] = static_cast<T>(2) / (right - left);
        result[1][1] = static_cast<T>(2) / (top - bottom);
        result[2][2] = - static_cast<T>(1);
        result[3][0] = - (right + left) / (right - left);
        result[3][1] = - (top + bottom) / (top - bottom);
        return result;
    }
    
    //ortho
    //--------------------------------------------------------------------------
    template <typename T>
    tmat4x4<T> frustum(T left,
                       T right,
                       T bottom,
                       T top,
                       T nearVal,
                       T farVal)
    {
        tmat4x4<T> result(0);
        result[0][0] = (static_cast<T>(2) * nearVal) / (right - left);
        result[1][1] = (static_cast<T>(2) * nearVal) / (top - bottom);
        result[2][0] = (right + left) / (right - left);
        result[2][1] = (top + bottom) / (top - bottom);
        result[2][2] = -(farVal + nearVal) / (farVal - nearVal);
        result[2][3] = static_cast<T>(-1);
        result[3][2] = -(static_cast<T>(2) * farVal * nearVal) / (farVal - nearVal);
        return result;
    }
    
    
    //perspective
    //--------------------------------------------------------------------------
    template <typename T>
    tmat4x4<T> perspective(T fovy,
                           T aspect,
                           T zNear,
                           T zFar)
    {
        assert(std::abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));
        
        T const tanHalfFovy = tan(fovy / static_cast<T>(2));
        
        tmat4x4<T> result(static_cast<T>(0));
        result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
        result[1][1] = static_cast<T>(1) / (tanHalfFovy);
        result[2][2] = - (zFar + zNear) / (zFar - zNear);
        result[2][3] = - static_cast<T>(1);
        result[3][2] = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
        return result;
    }
    
    //lookAt
    //--------------------------------------------------------------------------
    template <typename T>
    tmat4x4<T> lookAt(tvec3<T> const & eye,
                      tvec3<T> const & center,
                      tvec3<T> const & up)
    {
        tvec3<T> const f(normalize(center - eye));
        tvec3<T> const s(normalize(cross(f, up)));
        tvec3<T> const u(cross(s, f));
        
        tmat4x4<T> result(1);
        result[0][0] = s.x;
        result[1][0] = s.y;
        result[2][0] = s.z;
        result[0][1] = u.x;
        result[1][1] = u.y;
        result[2][1] = u.z;
        result[0][2] =-f.x;
        result[1][2] =-f.y;
        result[2][2] =-f.z;
        result[3][0] =-dot(s, eye);
        result[3][1] =-dot(u, eye);
        result[3][2] = dot(f, eye);
        return result;
    }
    
    template <typename T>
    tmat4x4<T> lookAt(T eyex, T eyey, T eyez,
                      T centerx, T centery, T centerz,
                      T upx, T upy, T upz)
    {
        return lookAt(tvec3<T>(eyex, eyey, eyez),
                      tvec3<T>(centerx, centery, centerz),
                      tvec3<T>(upx, upy, upz));
    }
    

}

#endif /* matrix_transform_h */
