/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
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

AFX_DEFINE_STRUCT_SIMD(afxRect)
/// Rectangles are used to describe a specified rectangular region of pixels within an image or framebuffer.
/// Rectangles include both an offset and an extent of the same dimensionality, as described above.
{
    union
    {
        afxInt      origin[2];
        struct
        {
            afxInt  x, y;
        };
    };
    union
    {
        afxNat      extent[2];
        struct
        {
            afxNat  w, h;
        };
    };
};

AFX_DEFINE_STRUCT(afxViewport)
/// Structure specifying a viewport
{
    afxV2d          offset; /// [x, y] are the viewport's upper left corner (x,y).
    afxV2d          extent; /// [w, h] are the viewport's width and height, respectively.
    afxV2d          depth;  /// [n, f] are the normalized min/max depth range for the viewport.
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

AVXINL void         AfxZeroRect(afxRect* rc);

AVXINL void         AfxCopyRect(afxRect* rc, afxRect const* src);

AVXINL void         AfxSetRect(afxRect* rc, afxInt x, afxInt y, afxNat w, afxNat h);

#endif//AVX_VIEWPORT_H
