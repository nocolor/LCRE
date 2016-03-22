//
// Created by qyt on 16/2/16.
//

#ifndef QYT_FUNDATION_QYTF_HPP
#define QYT_FUNDATION_QYTF_HPP

#include "detail/type_geomertric.hpp"

#include "detail/type_vec2.hpp"
#include "detail/type_vec3.hpp"
#include "detail/type_vec4.hpp"

#include "detail/type_matrix4x4.hpp"
#include "detail/matrix_transform.hpp"

namespace qyt
{
    typedef __detail::__tvec2<float> vec2f;
    typedef __detail::__tvec3<float> vec3f;
    typedef __detail::__tvec4<float> vec4f;
    typedef __detail::__tmat4x4<float> mat4x4f;
    
    typedef vec2f   vec2;
    typedef vec3f   vec3;
    typedef vec4f   vec4;
    typedef mat4x4f mat4;
}

#endif //QYT_FUNDATION_QYTF_HPP
