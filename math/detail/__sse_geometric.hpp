//
//  __sse_geometric.h
//  qyt_fundation
//
//  Created by qyt on 16/2/23.
//  Copyright © 2016年 qyt. All rights reserved.
//

#ifndef __sse_geometric_h
#define __sse_geometric_h
#include <xmmintrin.h>


namespace qyt
{
    namespace __detail
    {
        __m128 __sse_dot_ps(__m128 v1, __m128 v2);
        __m128 __sse_length_ps(__m128 v);
        __m128 __sse_cross_ps(__m128 v1, __m128 v2);
        __m128 __sse_normalize_ps(__m128 v);
    }
}

#endif /* __sse_geometric_h */
