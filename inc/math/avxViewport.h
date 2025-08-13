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

#ifndef AVX_VIEWPORT_H
#define AVX_VIEWPORT_H

#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/math/afxRect.h"

AFX_DEFINE_STRUCT(avxOrigin2)
// Structure specifying the origin of an area.
{
    // the X and Y origins, respectively.
    afxInt x, y;
};

AFX_DEFINE_STRUCT(avxOrigin)
// Structure specifying the origin of a volume.
{
    // the X, Y and Z origins, respectively.
    afxInt x, y, z;
};

AFX_DEFINE_STRUCT(avxRange2)
// Structure specifying the extent of a area.
{
    // The width and height of the represented area, respectively.
    afxUnit w, h;
};

AFX_DEFINE_STRUCT(avxRange)
// Structure specifying the extent of a volume.
{
    // The width, height and depth of the represented volume, respectively.
    afxUnit w, h, d;
};

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, avxViewport)
/// Structure specifying a viewport
{
    afxV2d origin; // [x, y] are the viewport's bottom left corner (x,y).
    afxV2d extent; // [w, h] are the viewport's width and height, respectively.
    afxReal minDepth; // is the normalized min depth range for the viewport.
    afxReal maxDepth; // is the normalized max depth range for the viewport.
};

/// min depth can be less than, equal to, or greater than max depth.

/// The viewport parameters shown in the above equations are found from these values as

/// ox = x + w / 2
/// oy = y + h / 2
/// oz = n (or (f + n) / 2 if clip control delimiting range is [-1.f, 1.f]).
/// px = w
/// py = h
/// pz = f - n (or (f - n) / 2 if clip control delimiting range is [-1.f, 1.f])).

/// The application can specify a negative term for @height, which has the effect of negating the y coordinate in clip space before performing the transform.
/// When using a negative @height, the application should also adjust the y value to point to the lower left corner of the viewport instead of the upper left corner.
/// Using the negative @height allows the application to avoid having to negate the y component of the Position output from the last pre-rasterization shader stage.

/// The width and height of the implementation-dependent maximum viewport dimensions must be greater than or equal to the width and height of the largest image which can be created and attached to a framebuffer.

/// The floating-point viewport bounds are represented with an implementation-dependent precision.

////////////////////////////////////////////////////////////////////////////////

AVX avxViewport const AVX_VIEWPORT_MIN;
AVX avxViewport const AVX_VIEWPORT_MAX;

AVX avxOrigin const AVX_ORIGIN_ZERO;
AVX avxOrigin const AVX_ORIGIN_MIN;
AVX avxOrigin const AVX_ORIGIN_MAX;

AVX avxRange const AVX_RANGE_ZERO;
AVX avxRange const AVX_RANGE_MIN;
AVX avxRange const AVX_RANGE_MAX;

#define AVX_RANGE(w_, h_, d_) (avxRange){ \
    .w = (afxUnit)(w_), \
    .h = (afxUnit)(h_), \
    .d = (afxUnit)(d_) }

#define AVX_ORIGIN(x_, y_, z_) (avxOrigin){ \
    .x = (afxInt)(x_), \
    .y = (afxInt)(y_), \
    .z = (afxInt)(z_) }

#define AVX_VIEWPORT(x_, y_, w_, h_, minDepth_, maxDepth_) (avxViewport){ \
    .origin = { (afxReal)(x_), (afxReal)(y_) }, \
    .extent = { (afxReal)(w_), (afxReal)(h_) }, \
    .minDepth = (afxReal)(minDepth_), \
    .maxDepth = (afxReal)(maxDepth_) }



AVXINL avxRange AvxMinRange(avxRange const a, avxRange const b);
AVXINL avxRange AvxMaxRange(avxRange const a, avxRange const b);
AVXINL avxRange AvxClampRange(avxRange const in, avxRange const min, avxRange const max);
AVXINL avxRange AvxLimitRange(avxOrigin const origin, avxRange const extent);


AVXINL afxBool AvxIsRangeEqual(avxRange const whd, avxRange const other);
AVXINL afxBool AvxIsRangeLequal(avxRange const whd, avxRange const other);
AVXINL afxBool AvxIsRangeLess(avxRange const whd, avxRange const other);
AVXINL afxBool AvxIsRangeZero(avxRange const whd);
AVXINL avxRange AvxAddRange(avxRange const a, avxRange const b);
AVXINL avxRange AvxSubtractRange(avxRange const a, avxRange const b);
AVXINL avxRange AvxDivideRange(avxRange const in, afxUnit div);
AVXINL avxRange AvxModRange(avxRange const in, afxUnit div);
AVXINL avxRange AvxScaleRange(avxRange const in, afxUnit s);
AVXINL avxRange AvxHalfRange(avxRange const in);
AVXINL afxUnit AvxSumRange(avxRange const whd);
AVXINL afxUnit AvxMagRange(avxRange const whd);
AVXINL afxUnit AvxDotRange(avxRange const whd, avxRange const other);

AVXINL avxOrigin AvxClampOrigin(avxOrigin const origin, avxRange const max);

AVXINL avxOrigin AvxHalfOrigin(avxOrigin const in);

AVXINL void         AfxMakeViewport(avxViewport* vp, afxReal x, afxReal y, afxReal w, afxReal h, afxReal minDepth, afxReal maxDepth);

#endif//AVX_VIEWPORT_H
