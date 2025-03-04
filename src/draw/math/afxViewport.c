/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

#include "../impl/avxImplementation.h"

afxRect const AVX_RECT_ZERO = { 0, 0, 0, 0 };
afxRect const AVX_RECT_MIN = { AFX_I32_MIN, AFX_I32_MIN, 1, 1 };
afxRect const AVX_RECT_MAX = { AFX_I32_MAX, AFX_I32_MAX, AFX_U32_MAX, AFX_U32_MAX };

afxViewport const AVX_VIEWPORT_MIN = { .origin = { AFX_R32_MIN, AFX_R32_MIN }, .extent = { 1, 1 } };
afxViewport const AVX_VIEWPORT_MAX = { .origin = { AFX_R32_MAX, AFX_R32_MAX }, .extent = { AFX_R32_MAX, AFX_R32_MAX } };

avxOrigin const AVX_ORIGIN_ZERO = { 0, 0, 0 };
avxOrigin const AVX_ORIGIN_MIN = { AFX_I32_MIN, AFX_I32_MIN, AFX_I32_MIN };
avxOrigin const AVX_ORIGIN_MAX = { AFX_I32_MAX, AFX_I32_MAX, AFX_I32_MAX };

avxRange const AVX_RANGE_ZERO = { 0, 0, 0 };
avxRange const AVX_RANGE_MIN = { 1, 1, 1 };
avxRange const AVX_RANGE_MAX = { AFX_U32_MAX, AFX_U32_MAX, AFX_U32_MAX };

_AVXINL afxBool AvxDoesRectOverlaps(afxRect const* a, afxRect const* b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    // is B completely right of A?
    if (AfxMax(a->x, (afxInt)a->w) < AfxMin(b->x, (afxInt)b->w))
        return FALSE;

    // is B completely left of A?
    if (AfxMax(b->x, (afxInt)b->w) < AfxMin(a->x, (afxInt)a->w))
        return FALSE;

    // is B completely above A?
    if (AfxMax(a->y, (afxInt)a->h) < AfxMin(b->y, (afxInt)b->h))
        return FALSE;

    // is B completely below A?
    if (AfxMax(b->y, (afxInt)b->h) < AfxMin(a->y, (afxInt)a->h))
        return FALSE;

    return TRUE; // overlapping
}

_AVXINL afxBool AvxContainsRectBiased(afxRect const* a, afxRect const* b, afxInt tolX, afxInt tolY)
// Compare if rectangle \a a contains rectangle \a b in coordinate, with specified tolerance allowed.
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    return ((a->x <= (b->x + tolX)) && (a->y <= (b->y + tolY)) &&
            ((a->w + tolX) >= b->w) && ((a->h + tolY) >= b->h));
}

_AVXINL afxBool AvxContainsRect(afxRect const* a, afxRect const* b)
// Compare if rectangle \a a contains rectangle \a b in coordinate.
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(a);
    AFX_ASSERT(b);    
    return ((a->x <= b->x) && (a->y <= b->y) &&
            (a->w >= b->w) && (a->h >= b->h));
}

// EXTENT

_AVXINL avxRange AvxMinRange(avxRange const a, avxRange const b)
{
    afxError err = AFX_ERR_NONE;
    return (avxRange const)
    { 
        .w = AfxMin(a.w, b.w), 
        .h = AfxMin(a.h, b.h), 
        .d = AfxMin(a.d, b.d) 
    };
}

_AVXINL avxRange AvxMaxRange(avxRange const a, avxRange const b)
{
    afxError err = AFX_ERR_NONE;
    return (avxRange const)
    {
        .w = AfxMax(a.w, b.w), 
        .h = AfxMax(a.h, b.h), 
        .d = AfxMax(a.d, b.d) 
    };
}

_AVXINL avxRange AvxClampRange(avxRange const in, avxRange const min, avxRange const max)
{
    afxError err = AFX_ERR_NONE;
    return (avxRange const)
    {
        .w = AfxClamp(in.w, min.w, max.w), 
        .h = AfxClamp(in.h, min.h, max.h), 
        .d = AfxClamp(in.d, min.d, max.d) 
    };
}

_AVXINL avxRange AvxLimitRange(avxOrigin const origin, avxRange const extent)
{
    afxError err = AFX_ERR_NONE;
    return (avxRange const)
    {
        .w = AfxMin(origin.x, (afxInt)(extent.w - 1)), 
        .h = AfxMin(origin.y, (afxInt)(extent.h - 1)), 
        .d = AfxMin(origin.z, (afxInt)(extent.d - 1)) 
    };
}

_AVXINL afxBool AvxIsRangeEqual(avxRange const whd, avxRange const other)
{
    afxError err = AFX_ERR_NONE;
    return  (whd.w == other.w) || 
            (whd.h == other.h) || 
            (whd.d == other.d);
}

_AVXINL afxBool AvxIsRangeLequal(avxRange const whd, avxRange const other)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return ((   (whd.w <= other.w) && 
                (whd.h <= other.h) && 
                (whd.d <= other.d)) != 0);
}

_AVXINL afxBool AvxIsRangeLess(avxRange const whd, avxRange const other)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return ((   (whd.w < other.w) && 
                (whd.h < other.h) && 
                (whd.d < other.d)) != 0);
}

_AVXINL afxBool AvxIsRangeZero(avxRange const whd)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    return (AFX_R(0) == (whd.w + whd.h + whd.d));
}

_AVXINL avxRange AvxAddRange(avxRange const a, avxRange const b)
{
    afxError err = AFX_ERR_NONE;
    return (avxRange const) {
        .w = a.w + b.w,
        .h = a.h + b.h,
        .d = a.d + b.d
    };
}

_AVXINL avxRange AvxSubtractRange(avxRange const a, avxRange const b)
{
    afxError err = AFX_ERR_NONE;
    return (avxRange const) {
        .w = a.w - b.w,
        .h = a.h - b.h,
        .d = a.d - b.d
    };
}

_AVXINL avxRange AvxDivideRange(avxRange const in, afxUnit div)
{
    afxError err = AFX_ERR_NONE;
    return (avxRange const) {
        .w = in.w / div,
        .h = in.h / div,
        .d = in.d / div
    };
}

_AVXINL avxRange AvxModRange(avxRange const in, afxUnit div)
{
    afxError err = AFX_ERR_NONE;
    return (avxRange const) {
        .w = in.w % div,
        .h = in.h % div,
        .d = in.d % div
    };
}

_AVXINL avxRange AvxScaleRange(avxRange const in, afxUnit s)
{
    afxError err = AFX_ERR_NONE;
    return (avxRange const) {
        .w = in.w * s,
        .h = in.h * s,
        .d = in.d * s
    };
}

_AVXINL avxRange AvxHalfRange(avxRange const in)
{
    afxError err = AFX_ERR_NONE;
    return (avxRange const) {
        .w = AfxMax(in.w ? 1 : 0, in.w >> 1),
        .h = AfxMax(in.h ? 1 : 0, in.h >> 1),
        .d = AfxMax(in.d ? 1 : 0, in.d >> 1)
    };
}

_AVXINL afxUnit AvxSumRange(avxRange const whd)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    return whd.w + whd.h + whd.d;
}

_AVXINL afxUnit AvxMagRange(avxRange const whd)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    return whd.w * whd.h * whd.d;
}

_AVXINL afxUnit AvxDotRange(avxRange const whd, avxRange const other)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return (whd.w * other.w) + (whd.h * other.h) + (whd.d * other.d);
}

_AVXINL avxOrigin AvxClampOrigin(avxOrigin const origin, avxRange const max)
{
    afxError err = AFX_ERR_NONE;
    return (avxOrigin) {
        .x = AfxMin(origin.x, max.w - 1),
        .y = AfxMin(origin.y, max.h - 1),
        .z = AfxMin(origin.z, max.d - 1)
    };
}

_AVXINL avxOrigin AvxHalfOrigin(avxOrigin const in)
{
    afxError err = AFX_ERR_NONE;
    return (avxOrigin) {
        .x = in.x >> 1,
        .y = in.y >> 1,
        .z = in.z >> 1
    };
}

_AVXINL void AfxMakeViewport(afxViewport* vp, afxReal x, afxReal y, afxReal w, afxReal h, afxReal minDepth, afxReal maxDepth)
{
    afxError err = NIL;
    AFX_ASSERT(vp);
    AFX_ASSERT(w);
    AFX_ASSERT(h);
    *vp = (afxViewport const)
    {
        .origin = { x, y },
        .extent = { w, h },
        .minDepth = minDepth,
        .maxDepth = maxDepth
    };
}
