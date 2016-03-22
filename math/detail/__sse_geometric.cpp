#include "__sse_geometric.hpp"

namespace qyt
{
    namespace __detail
    {
        __m128 __sse_dot_ps(__m128 v1, __m128 v2)
        {
            __m128 mul0 = _mm_mul_ps(v1, v2);
            __m128 swp0 = _mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(2, 3, 0, 1));
            __m128 add0 = _mm_add_ps(mul0, swp0);
            __m128 swp1 = _mm_shuffle_ps(add0, add0, _MM_SHUFFLE(0, 1, 2, 3));
            __m128 add1 = _mm_add_ps(add0, swp1);
            return add1;
        }
        
        __m128 __sse_length_ps(__m128 v)
        {
            __m128 dot0 = __sse_dot_ps(v, v);
            __m128 sqt0 = _mm_sqrt_ps(dot0);
            return sqt0;
        }
        
        __m128 __sse_cross_ps(__m128 v1, __m128 v2)
        {
            __m128 swp0 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 swp1 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 1, 0, 2));
            __m128 swp2 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 swp3 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2));
            __m128 mul0 = _mm_mul_ps(swp0, swp3);
            __m128 mul1 = _mm_mul_ps(swp1, swp2);
            __m128 sub0 = _mm_sub_ps(mul0, mul1);
            return sub0;
        }
        
        __m128 __sse_normalize_ps(__m128 v)
        {
            __m128 dot0 = __sse_dot_ps(v, v);
            __m128 isr0 = _mm_rsqrt_ps(dot0);
            __m128 mul0 = _mm_mul_ps(v, isr0);
            return mul0;
        }
    }
}