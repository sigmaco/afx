/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Video Graphics Extensions & Experiments.

#include "avxIcd.h"

avxOrigin const AVX_ORIGIN_ZERO = { 0, 0, 0 };
avxOrigin const AVX_ORIGIN_MIN = { AFX_I32_MIN, AFX_I32_MIN, AFX_I32_MIN };
avxOrigin const AVX_ORIGIN_MAX = { AFX_I32_MAX, AFX_I32_MAX, AFX_I32_MAX };

avxRange const AVX_RANGE_ZERO = { 0, 0, 0 };
avxRange const AVX_RANGE_MIN = { 1, 1, 1 };
avxRange const AVX_RANGE_MAX = { AFX_U32_MAX, AFX_U32_MAX, AFX_U32_MAX };

// EXTENT

_AVXINL avxRange AvxMinRange(avxRange const a, avxRange const b)
{
    afxError err = { 0 };
    return (avxRange const)
    { 
        .w = AFX_MIN(a.w, b.w), 
        .h = AFX_MIN(a.h, b.h), 
        .d = AFX_MIN(a.d, b.d) 
    };
}

_AVXINL avxRange AvxMaxRange(avxRange const a, avxRange const b)
{
    afxError err = { 0 };
    return (avxRange const)
    {
        .w = AFX_MAX(a.w, b.w), 
        .h = AFX_MAX(a.h, b.h), 
        .d = AFX_MAX(a.d, b.d) 
    };
}

_AVXINL avxRange AvxClampRange(avxRange const in, avxRange const min, avxRange const max)
{
    afxError err = { 0 };
    return (avxRange const)
    {
        .w = AFX_CLAMP(in.w, min.w, max.w), 
        .h = AFX_CLAMP(in.h, min.h, max.h), 
        .d = AFX_CLAMP(in.d, min.d, max.d) 
    };
}

_AVXINL avxRange AvxLimitRange(avxOrigin const origin, avxRange const extent)
{
    afxError err = { 0 };
    return (avxRange const)
    {
        .w = AFX_MIN(origin.x, (afxInt)(extent.w - 1)), 
        .h = AFX_MIN(origin.y, (afxInt)(extent.h - 1)), 
        .d = AFX_MIN(origin.z, (afxInt)(extent.d - 1)) 
    };
}

_AVXINL afxBool AvxIsRangeEqual(avxRange const whd, avxRange const other)
{
    afxError err = { 0 };
    return  (whd.w == other.w) || 
            (whd.h == other.h) || 
            (whd.d == other.d);
}

_AVXINL afxBool AvxIsRangeLequal(avxRange const whd, avxRange const other)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return ((   (whd.w <= other.w) && 
                (whd.h <= other.h) && 
                (whd.d <= other.d)) != 0);
}

_AVXINL afxBool AvxIsRangeLess(avxRange const whd, avxRange const other)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return ((   (whd.w < other.w) && 
                (whd.h < other.h) && 
                (whd.d < other.d)) != 0);
}

_AVXINL afxBool AvxIsRangeZero(avxRange const whd)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    return (AFX_REAL(0) == (whd.w + whd.h + whd.d));
}

_AVXINL avxRange AvxAddRange(avxRange const a, avxRange const b)
{
    afxError err = { 0 };
    return (avxRange const) {
        .w = a.w + b.w,
        .h = a.h + b.h,
        .d = a.d + b.d
    };
}

_AVXINL avxRange AvxSubtractRange(avxRange const a, avxRange const b)
{
    afxError err = { 0 };
    return (avxRange const) {
        .w = a.w - b.w,
        .h = a.h - b.h,
        .d = a.d - b.d
    };
}

_AVXINL avxRange AvxDivideRange(avxRange const in, afxUnit div)
{
    afxError err = { 0 };
    return (avxRange const) {
        .w = in.w / div,
        .h = in.h / div,
        .d = in.d / div
    };
}

_AVXINL avxRange AvxModRange(avxRange const in, afxUnit div)
{
    afxError err = { 0 };
    return (avxRange const) {
        .w = in.w % div,
        .h = in.h % div,
        .d = in.d % div
    };
}

_AVXINL avxRange AvxScaleRange(avxRange const in, afxUnit s)
{
    afxError err = { 0 };
    return (avxRange const) {
        .w = in.w * s,
        .h = in.h * s,
        .d = in.d * s
    };
}

_AVXINL avxRange AvxHalfRange(avxRange const in)
{
    afxError err = { 0 };
    return (avxRange const) {
        .w = AFX_MAX(in.w ? 1 : 0, in.w >> 1),
        .h = AFX_MAX(in.h ? 1 : 0, in.h >> 1),
        .d = AFX_MAX(in.d ? 1 : 0, in.d >> 1)
    };
}

_AVXINL afxUnit AvxSumRange(avxRange const whd)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    return whd.w + whd.h + whd.d;
}

_AVXINL afxUnit AvxMagRange(avxRange const whd)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    return whd.w * whd.h * whd.d;
}

_AVXINL afxUnit AvxDotRange(avxRange const whd, avxRange const other)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return (whd.w * other.w) + (whd.h * other.h) + (whd.d * other.d);
}

_AVXINL avxOrigin AvxClampOrigin(avxOrigin const origin, avxRange const max)
{
    afxError err = { 0 };
    return (avxOrigin) {
        .x = AFX_MIN(origin.x, (afxInt)(max.w - 1)),
        .y = AFX_MIN(origin.y, (afxInt)(max.h - 1)),
        .z = AFX_MIN(origin.z, (afxInt)(max.d - 1))
    };
}

_AVXINL avxOrigin AvxHalfOrigin(avxOrigin const in)
{
    afxError err = { 0 };
    return (avxOrigin) {
        .x = in.x >> 1,
        .y = in.y >> 1,
        .z = in.z >> 1
    };
}

_AVXINL void AfxMakeViewport(avxViewport* vp, afxReal x, afxReal y, afxReal w, afxReal h, afxReal minDepth, afxReal maxDepth)
{
    afxError err = { 0 };
    AFX_ASSERT(vp);
    AFX_ASSERT(w);
    AFX_ASSERT(h);
    *vp = (avxViewport const)
    {
        .origin = { x, y },
        .extent = { w, h },
        .minDepth = minDepth,
        .maxDepth = maxDepth
    };
}

_AVXINL void AvxFlipViewport(avxViewport* vp, avxViewport const* in, afxBool swapDepthRange)
{
    afxError err = { 0 };
    AFX_ASSERT(vp);
    AFX_ASSERT(in);
    avxViewport flipped = *in;

    // Shift origin.y by height to move origin to top-left of original viewport
    flipped.origin[1] = in->origin[1] + in->extent[1];

    // Negate the height to flip vertically
    flipped.extent[1] = -in->extent[1];

    // Optionally swap minDepth and maxDepth if your depth range needs flipping:
    if (swapDepthRange)
    {
        afxReal md = flipped.minDepth;
        flipped.minDepth = flipped.maxDepth;
        flipped.maxDepth = md;
    }
    *vp = flipped;
}

_AVXINL void AvxViewportTransform(avxViewport const* vp, avxViewportFlags flags, afxUnit cnt, afxV4d const ndcPos[], afxV4d canvPos[], afxUnit dstStride, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT(vp);
    AFX_ASSERT_ALIGNMENT(srcStride, sizeof(afxReal));
    AFX_ASSERT(!srcStride || (srcStride >= sizeof(afxV3d)));
    AFX_ASSERT_ALIGNMENT(dstStride, sizeof(afxReal));
    AFX_ASSERT(!dstStride || (dstStride >= sizeof(afxV3d)));
    srcStride = AFX_OR(srcStride, sizeof(afxV3d));
    dstStride = AFX_OR(dstStride, sizeof(afxV3d));
    afxUnit srcStep = srcStride / sizeof(afxReal);
    afxUnit dstStep = dstStride / sizeof(afxReal);
    afxReal const* ndcPos2 = &ndcPos[0][0];
    afxReal* canvPos2 = &canvPos[0][0];

    afxReal vpX = vp->origin[0];
    afxReal vpY = vp->origin[1];
    afxReal vpW = vp->extent[0];
    afxReal vpH = vp->extent[1];
    afxReal minDepth = vp->minDepth;
    afxReal depthDiff = (vp->maxDepth - vp->minDepth);

    // If we're following Vulkan-style NDC where Z is already in [0, 1], skip the ndcZ * 0.5 + 0.5 step.
    if (flags & avxViewportFlag_NDCZ)
    {
        if (flags & avxViewportFlag_FLIP)
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit dstIdx = i * dstStep;
                afxUnit srcIdx = i * srcStep;
                // X and Y: [-1, 1] -> canvas coordinates
                canvPos2[dstIdx + 0] =      vpX + (        (ndcPos2[srcIdx + 0] * 0.5f + 0.5f)) * vpW;
                canvPos2[dstIdx + 1] =      vpY + ( 1.0f - (ndcPos2[srcIdx + 1] * 0.5f + 0.5f)) * vpH; // Y-flip
                canvPos2[dstIdx + 2] = minDepth + (        (ndcPos2[srcIdx + 2]))               * depthDiff;
            }
        }
        else
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit dstIdx = i * dstStep;
                afxUnit srcIdx = i * srcStep;
                // X and Y: [-1, 1] -> canvas coordinates
                canvPos2[dstIdx + 0] =      vpX + (ndcPos2[srcIdx + 0] * 0.5f + 0.5f) * vpW;
                canvPos2[dstIdx + 1] =      vpY + (ndcPos2[srcIdx + 1] * 0.5f + 0.5f) * vpH;
                canvPos2[dstIdx + 2] = minDepth + (ndcPos2[srcIdx + 2])               * depthDiff; // raw NDC Z
            }
        }
    }
    else
    {
        if (flags & avxViewportFlag_FLIP)
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit dstIdx = i * dstStep;
                afxUnit srcIdx = i * srcStep;
                // X and Y: [-1, 1] -> canvas coordinates
                canvPos2[dstIdx + 0] =      vpX + (        (ndcPos2[srcIdx + 0] * 0.5f + 0.5f)) * vpW;
                canvPos2[dstIdx + 1] =      vpY + ( 1.0f - (ndcPos2[srcIdx + 1] * 0.5f + 0.5f)) * vpH; // Y-flip
                canvPos2[dstIdx + 2] = minDepth + (        (ndcPos2[srcIdx + 2] * 0.5f + 0.5f)) * depthDiff;
            }
        }
        else
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit dstIdx = i * dstStep;
                afxUnit srcIdx = i * srcStep;
                // X and Y: [-1, 1] -> canvas coordinates
                canvPos2[dstIdx + 0] =      vpX + (ndcPos2[srcIdx + 0] * 0.5f + 0.5f) * vpW;
                canvPos2[dstIdx + 1] =      vpY + (ndcPos2[srcIdx + 1] * 0.5f + 0.5f) * vpH;
                canvPos2[dstIdx + 2] = minDepth + (ndcPos2[srcIdx + 2] * 0.5f + 0.5f) * depthDiff;
            }
        }
    }
}
