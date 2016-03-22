//
//  type_matrix4x4_inl.hpp
//  qyt_fundation
//
//  Created by qyt on 16/2/18.
//  Copyright © 2016年 qyt. All rights reserved.
//

#ifndef type_matrix4x4_inl_h
#define type_matrix4x4_inl_h

namespace qyt
{
    namespace __detail
    {
        template <typename T>
        __tmat4x4<T> __tmat4x4<T>::get_inverse(const __tmat4x4<T>& m)
        {
            T Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
            T Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
            T Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
            
            T Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
            T Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
            T Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
            
            T Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
            T Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
            T Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
            
            T Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
            T Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
            T Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
            
            T Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
            T Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
            T Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
            
            T Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
            T Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
            T Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
            
            __tvec4<T> Fac0(Coef00, Coef00, Coef02, Coef03);
            __tvec4<T> Fac1(Coef04, Coef04, Coef06, Coef07);
            __tvec4<T> Fac2(Coef08, Coef08, Coef10, Coef11);
            __tvec4<T> Fac3(Coef12, Coef12, Coef14, Coef15);
            __tvec4<T> Fac4(Coef16, Coef16, Coef18, Coef19);
            __tvec4<T> Fac5(Coef20, Coef20, Coef22, Coef23);
            
            __tvec4<T> Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
            __tvec4<T> Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
            __tvec4<T> Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
            __tvec4<T> Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);
            
            __tvec4<T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
            __tvec4<T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
            __tvec4<T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
            __tvec4<T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);
            
            __tvec4<T> SignA(+1, -1, +1, -1);
            __tvec4<T> SignB(-1, +1, -1, +1);
            __tmat4x4<T> Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);
            
            __tvec4<T> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);
            
            __tvec4<T> Dot0(m[0] * Row0);
            T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);
            
            T OneOverDeterminant = static_cast<T>(1) / Dot1;
            
            return Inverse * OneOverDeterminant;
        }

        
        template <typename T>
        __tmat4x4<T>::__tmat4x4()
        {
            this->value[0] = __col_type(1, 0, 0, 0);
            this->value[1] = __col_type(0, 1, 0, 0);
            this->value[2] = __col_type(0, 0, 1, 0);
            this->value[3] = __col_type(0, 0, 0, 1);
        }
        
        template <typename T>
        __tmat4x4<T>::__tmat4x4(const __tmat4x4<T>& m)
        {
            memcpy(&this->value, &m.value, 16*sizeof(T));
        }
        
        template <typename T>
        __tmat4x4<T>::__tmat4x4(const T& s)
        {
            this->value[0] = __col_type(s, 0, 0, 0);
            this->value[1] = __col_type(0, s, 0, 0);
            this->value[2] = __col_type(0, 0, s, 0);
            this->value[3] = __col_type(0, 0, 0, s);
        }
        
        template <typename T>
        __tmat4x4<T>::__tmat4x4(const T& x0, const T& y0, const T& z0, const T& w0,
                                const T& x1, const T& y1, const T& z1, const T& w1,
                                const T& x2, const T& y2, const T& z2, const T& w2,
                                const T& x3, const T& y3, const T& z3, const T& w3)
        {
            this->value[0] = __col_type(x0, y0, z0, w0);
            this->value[1] = __col_type(x1, y1, z1, w1);
            this->value[2] = __col_type(x2, y2, z2, w2);
            this->value[3] = __col_type(x3, y3, z3, w3);
        }
        
        template <typename T>
        __tmat4x4<T>::__tmat4x4(const __col_type& v0,
                                const __col_type& v1,
                                const __col_type& v2,
                                const __col_type& v3)
        {
            this->value[0] = v0;
            this->value[1] = v1;
            this->value[2] = v2;
            this->value[3] = v3;
        }
        
        template <typename T>
        template <
        typename X1, typename Y1, typename Z1, typename W1,
        typename X2, typename Y2, typename Z2, typename W2,
        typename X3, typename Y3, typename Z3, typename W3,
        typename X4, typename Y4, typename Z4, typename W4>
        __tmat4x4<T>::__tmat4x4(const X1& x1, const Y1& y1, const Z1& z1, const W1& w1,
                                const X2& x2, const Y2& y2, const Z2& z2, const W2& w2,
                                const X3& x3, const Y3& y3, const Z3& z3, const W3& w3,
                                const X4& x4, const Y4& y4, const Z4& z4, const W4& w4)
        {
            this->value[0] = __col_type(static_cast<T>(x1), static_cast<T>(y1), static_cast<T>(z1), static_cast<T>(w1));
            this->value[1] = __col_type(static_cast<T>(x2), static_cast<T>(y2), static_cast<T>(z2), static_cast<T>(w2));
            this->value[2] = __col_type(static_cast<T>(x3), static_cast<T>(y3), static_cast<T>(z3), static_cast<T>(w3));
            this->value[3] = __col_type(static_cast<T>(x4), static_cast<T>(y4), static_cast<T>(z4), static_cast<T>(w4));
        }
        
        template <typename T>
        template <typename V0, typename V1, typename V2, typename V3>
        __tmat4x4<T>::__tmat4x4(const __tvec4<V0> &v0,
                                const __tvec4<V1> &v1,
                                const __tvec4<V2> &v2,
                                const __tvec4<V3> &v3)
        {
            this->value[0] = __col_type(v0);
            this->value[1] = __col_type(v1);
            this->value[2] = __col_type(v2);
            this->value[3] = __col_type(v3);
        }
        
        template <typename T>
        template <typename U>
        __tmat4x4<T>::__tmat4x4(const __tmat4x4<U> &m)
        {
            this->value[0] = __col_type(m[0]);
            this->value[1] = __col_type(m[1]);
            this->value[2] = __col_type(m[2]);
            this->value[3] = __col_type(m[3]);
        }
        
        template <typename T>
        size_t __tmat4x4<T>::size() const
        {
            return 4;
        }
        
        template <typename T>
        typename __tmat4x4<T>::__col_type& __tmat4x4<T>::operator[](size_t i)
        {
            assert(i < this->size());
            return this->value[i];
        }
        
        template <typename T>
        const typename __tmat4x4<T>::__col_type& __tmat4x4<T>::operator[](size_t i) const
        {
            assert(i < this->size());
            return this->value[i];
        }
        
        template <typename T>
        __tmat4x4<T>& __tmat4x4<T>::operator=(const __tmat4x4<T>& m)
        {
            if (this != &m)
                memcpy(&this->value, &m.value, 16*sizeof(T));
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tmat4x4<T>& __tmat4x4<T>::operator=(const __tmat4x4<U>& m)
        {
            if (this != &m)
            {
                this->value[0] = m[0];
                this->value[1] = m[1];
                this->value[2] = m[2];
                this->value[3] = m[3];
            }
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tmat4x4<T>& __tmat4x4<T>::operator+=(const __tmat4x4<U> &m)
        {
            this->value[0] += m[0];
            this->value[1] += m[1];
            this->value[2] += m[2];
            this->value[3] += m[3];
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tmat4x4<T>& __tmat4x4<T>::operator+=(const U &s)
        {
            this->value[0] += s;
            this->value[1] += s;
            this->value[2] += s;
            this->value[3] += s;
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tmat4x4<T>& __tmat4x4<T>::operator-=(const __tmat4x4<U> &m)
        {
            this->value[0] -= m[0];
            this->value[1] -= m[1];
            this->value[2] -= m[2];
            this->value[3] -= m[3];
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tmat4x4<T>& __tmat4x4<T>::operator-=(const U &s)
        {
            this->value[0] -= s;
            this->value[1] -= s;
            this->value[2] -= s;
            this->value[3] -= s;
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tmat4x4<T>& __tmat4x4<T>::operator*=(const __tmat4x4<U> &m)
        {
            this->value[0] *= m[0];
            this->value[1] *= m[1];
            this->value[2] *= m[2];
            this->value[3] *= m[3];
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tmat4x4<T>& __tmat4x4<T>::operator*=(const U &s)
        {
            this->value[0] *= s;
            this->value[1] *= s;
            this->value[2] *= s;
            this->value[3] *= s;
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tmat4x4<T>& __tmat4x4<T>::operator/=(const __tmat4x4<U> &m)
        {
            this->value[0] /= m[0];
            this->value[1] /= m[1];
            this->value[2] /= m[2];
            this->value[3] /= m[3];
            return *this;
        }
        
        template <typename T>
        template <typename U>
        __tmat4x4<T>& __tmat4x4<T>::operator/=(const U &s)
        {
            this->value[0] /= s;
            this->value[1] /= s;
            this->value[2] /= s;
            this->value[3] /= s;
            return *this;
        }
        
        template <typename T>
        __tmat4x4<T> __tmat4x4<T>::operator~() const
        {
            __tmat4x4<T> m(*this);
            _MM_TRANSPOSE4_PS(m[0].data,
                              m[1].data,
                              m[2].data,
                              m[3].data);
            return m;
        }
        
        template <typename T>
        __tmat4x4<T> __tmat4x4<T>::operator!() const
        {
            return get_inverse(*this);
        }
        
        template <typename T>
        __tmat4x4<T> operator+(const __tmat4x4<T>& m, const T& s)
        {
            return __tmat4x4<T>(m[0] + s,
                                m[1] + s,
                                m[2] + s,
                                m[3] + s);
        }
        
        
        template <typename T>
        __tmat4x4<T> operator+(const T& s, const __tmat4x4<T>& m)
        {
            return __tmat4x4<T>(s + m[0],
                                s + m[1],
                                s + m[2],
                                s + m[3]);
        }
        
        template <typename T>
        __tmat4x4<T> operator+(const __tmat4x4<T>& m0, const __tmat4x4<T>& m1)
        {
            return __tmat4x4<T>(m0[0] + m1[0],
                                m0[1] + m1[1],
                                m0[2] + m1[2],
                                m0[3] + m1[3]);
        }
        
        template <typename T>
        __tmat4x4<T> operator-(const __tmat4x4<T>& m, const T& s)
        {
            return __tmat4x4<T>(m[0] - s,
                                m[1] - s,
                                m[2] - s,
                                m[3] - s);
        }
        
        
        template <typename T>
        __tmat4x4<T> operator-(const T& s, const __tmat4x4<T>& m)
        {
            return __tmat4x4<T>(s - m[0],
                                s - m[1],
                                s - m[2],
                                s - m[3]);
        }
        
        template <typename T>
        __tmat4x4<T> operator-(const __tmat4x4<T>& m0, const __tmat4x4<T>& m1)
        {
            return __tmat4x4<T>(m0[0] - m1[0],
                                m0[1] - m1[1],
                                m0[2] - m1[2],
                                m0[3] - m1[3]);
        }
        
        template <typename T>
        __tmat4x4<T> operator*(const __tmat4x4<T>& m, const T& s)
        {
            return __tmat4x4<T>(m[0] * s,
                                m[1] * s,
                                m[2] * s,
                                m[3] * s);
        }
        
        
        template <typename T>
        __tmat4x4<T> operator*(const T& s, const __tmat4x4<T>& m)
        {
            return __tmat4x4<T>(s * m[0],
                                s * m[1],
                                s * m[2],
                                s * m[3]);
        }
        
        template <typename T>
        __tmat4x4<T> operator*(const __tmat4x4<T>& m0, const __tmat4x4<T>& m1)
        {
            const typename __tmat4x4<T>::__col_type SrcA0 = m0[0];
            const typename __tmat4x4<T>::__col_type SrcA1 = m0[1];
            const typename __tmat4x4<T>::__col_type SrcA2 = m0[2];
            const typename __tmat4x4<T>::__col_type SrcA3 = m0[3];
            
            const typename __tmat4x4<T>::__col_type SrcB0 = m1[0];
            const typename __tmat4x4<T>::__col_type SrcB1 = m1[1];
            const typename __tmat4x4<T>::__col_type SrcB2 = m1[2];
            const typename __tmat4x4<T>::__col_type SrcB3 = m1[3];
            
            __tmat4x4<T> Result;
            Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
            Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
            Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
            Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
            return Result;
        }
        
        template <typename T>
        typename __tmat4x4<T>::__col_type operator*(const __tmat4x4<T>& m,
                                                    const typename __tmat4x4<T>::__col_type& v)
        {
            __m128 v0 = _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(0, 0, 0, 0));
            __m128 v1 = _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(1, 1, 1, 1));
            __m128 v2 = _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(2, 2, 2, 2));
            __m128 v3 = _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(3, 3, 3, 3));
            
            __m128 m0 = _mm_mul_ps(m[0].data, v0);
            __m128 m1 = _mm_mul_ps(m[1].data, v1);
            __m128 a0 = _mm_add_ps(m0, m1);
            
            __m128 m2 = _mm_mul_ps(m[2].data, v2);
            __m128 m3 = _mm_mul_ps(m[3].data, v3);
            __m128 a1 = _mm_add_ps(m2, m3);
            
            __m128 a2 = _mm_add_ps(a0, a1);
            
            return typename __tmat4x4<T>::__col_type(a2);
        }
        
        template <typename T>
        typename __tmat4x4<T>::__row_type operator*(const typename __tmat4x4<T>::__col_type& v,
                                                    const __tmat4x4<T>& m)
        {
            return typename __tmat4x4<T>::__row_type(m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3] * v[3],
                                                     m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3] * v[3],
                                                     m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3] * v[3],
                                                     m[3][0] * v[0] + m[3][1] * v[1] + m[3][2] * v[2] + m[3][3] * v[3]);
        }
        
        template <typename T>
        __tmat4x4<T> operator/(const __tmat4x4<T>& m, const T& s)
        {
            return __tmat4x4<T>(m[0] / s,
                                m[1] / s,
                                m[2] / s,
                                m[3] / s);
        }
        
        template <typename T>
        __tmat4x4<T> operator/(const T& s, const __tmat4x4<T>& m)
        {
            return __tmat4x4<T>(s/m[0],
                                s/m[1],
                                s/m[2],
                                s/m[3]);
        }
        
        template <typename T>
        __tmat4x4<T> operator/(const __tmat4x4<T>& m0, const __tmat4x4<T>& m1)
        {
            __tmat4x4<T> m(m0);
            return m /= m1;
        }
        
        template <typename T>
        typename __tmat4x4<T>::__col_type operator/(const __tmat4x4<T>& m,
                                                    const typename __tmat4x4<T>::__col_type& v)
        {
            return __tmat4x4<T>::get_inverse(m)*v;
        }
        
        template <typename T>
        typename __tmat4x4<T>::__col_type operator/(const typename __tmat4x4<T>::__col_type& v,
                                                    const __tmat4x4<T>& m)
        {
            return v*__tmat4x4<T>::get_inverse(m);
        }
    }
}

#endif /* type_matrix4x4_inl_h */
