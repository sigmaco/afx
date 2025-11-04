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

#ifndef AFX_RECT_H
#define AFX_RECT_H

#include "qwadro/math/afxVector.h"
#include "qwadro/draw/afxDrawDefs.h"

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxRect)
// Rectangles are used to describe a specified rectangular region within an bidimensional area.
{
    // the rectangle origin.
    //avxOrigin2 origin;
    // the X and Y origins, respectively.
    afxInt x, y;
    
    // the rectangle extent.
    //avxRange2 extent;
    // The width and height of the represented area, respectively.
    afxUnit w, h;
};

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxLayeredRect)
// Structure specifying a layered rectangle of consideration.
{
    // the two-dimensional region to be considered.
    afxRect             area;
    // The first layer to be considered.
    afxUnit32           baseLayer;
    // The number of layers to considered.
    afxUnit32           layerCnt;
};

AFX afxRect const AFX_RECT_ZERO;
AFX afxRect const AFX_RECT_MIN;
AFX afxRect const AFX_RECT_MAX;

#define AFX_RECT(x_, y_, w_, h_) (afxRect){ \
    .x = (afxInt)(x_), .y = (afxInt)(y_), \
    .w = (afxUnit)(w_), .h = (afxUnit)(h_) }

AFX afxLayeredRect const AFX_LAYERED_RECT_ZERO;
AFX afxLayeredRect const AFX_LAYERED_RECT_MIN;
AFX afxLayeredRect const AFX_LAYERED_RECT_MAX;

#define AFX_LAYERED_RECT(x_, y_, w_, h_, z_, d_) (afxLayeredRect){ \
    .area.x = (afxInt)(x_), .area.y = (afxInt)(y_), \
    .area.w = (afxUnit)(w_), .area.h = (afxUnit)(h_), \
    .baseLayer = (z_), .layerCnt = (d_) }

/*
    The AfxGetRectArea() function calculates the square units for a rectangle.
    The resulting value can be used to determine whether a rectangle is or not empty.
*/

AFXINL afxUnit      AfxGetRectArea
// Returns the square units of the rectangle.
(
    afxRect const*  rc
);

/*
    The AfxMergeRects() function calculates the union of two rectangles, meaning it finds the smallest
    rectangle that can contain both input rectangles.
    This function does the inverse of AfxIntersectRects(), it expands the rect.
    Returns the square units of the united rectangle.
*/

AFXINL afxUnit      AfxMergeRects
// Returns the square units of the united rectangle.
(
    afxRect*        rc, 
    afxRect const*  a, 
    afxRect const*  b
);

/*
    The AfxIntersectRects() function computes the overlapping region (intersection) of rectangles a and b, 
    and stores the result in rc. This function does the inverse of AfxMergeRects(), it shrinks the rect.
    Returns the square units of the intersection if the rectangles intersect, 
    or zero if there's no overlap (and probably leaves rc undefined or zeroed).
*/

AFXINL afxUnit      AfxIntersectRects
// Returns the square units of the united rectangle.
(
    afxRect*        rc, 
    afxRect const*  a, 
    afxRect const*  b
);

AFXINL afxBool      AfxAreRectsEqual
(
    afxRect const*  a, 
    afxRect const*  b
);

AFXINL afxBool      AfxAreRectsOverlapping
(
    afxRect const*  a,
    afxRect const*  b
);

/*
    The function checks whether rectangle @a contains rectangle @b, with a bias or tolerance applied
    meaning @b is allowed to "poke out" by up to @tolX and @tolY on the respective axes.

    It's like a "soft containment" check, useful for layout fuzziness, hit testing, or floating-point rounding tolerances.
*/

AFXINL afxBool      AfxDoesRectContainBiased
(
    afxRect const*  a, 
    afxRect const*  b, 
    afxInt          tolX, 
    afxInt          tolY
);

/*
    The AfxDoesRectContain() function checks whether rect @a fully contains rect @b.
    It's inclusive containment, which mean boundaries are allowed to touch.
    Returns true if @b fits completely within @a.
*/

AFXINL afxBool      AfxDoesRectContain
(
    // The major rect.
    afxRect const*  a,
    // The minor rect.
    afxRect const*  b
);

AFXINL afxBool      AfxIsRectOutside
(
    afxRect const*  a, 
    afxRect const*  b
);

AFXINL afxBool      AfxDoRectsIntersect
(
    afxRect const*  a, 
    afxRect const*  b
);

AFXINL void     AfxExtractRectMargins
(
    afxRect const* outer, 
    afxRect const* inner, 
    afxInt* left, 
    afxInt* top, 
    afxInt* right, 
    afxInt* bottom
);

/*
    The AfxFlipRect() function transforms the rectangle's y coordinate, 
    flipping it relative to the vertical extent (origin shift) - using the total height of the space to invert it for bottom-up/top-down rendering.
*/

AFXINL void         AfxFlipRect
(
    // The source rectangle.
    afxRect const*  rc,
    // The maximum vertical size.
    afxUnit         height,
    // The rectangle that will be modified or created.
    afxRect*        flipped
);

/*
    The AfxScissorRect() function takes a rectangle and a maximum area, and calculates the areas outside/excluded from the given rectangle.
    These areas are returned as exclusive rectangles. The function stores the results in the inverseRects array and returns the count.
    This function divides the space outside a given rectangle into four exclusive regions (left, top, right, bottom),
    which can be useful when you want to identify areas outside a defined rectangle within a screen.
    Returns the number of excluded regions.

    Use cases:
      UI clipping.
      Scissor test helpers.
      Avoiding rendering off-screen content.
      Calculating drop shadows or effects that leak out of bounds.
*/

AFXINL afxUnit      AfxScissorRect
(
    afxRect const*  rc, 
    afxUnit         maxW, 
    afxUnit         maxH, 
    afxRect         outsides[4]
);

/*
    The AfxAccumulateRects() function calculates the inclusive bounding rectangle for a number of "outside" rectangles
    (rectangles outside a given rectangle), the goal is to take a list of rectangles and return the smallest rectangle that
    encompasses all those given rectangles.
*/

AFXINL void         AfxAccumulateRects
(
    afxRect*        rc, 
    afxUnit         cnt, 
    afxRect*        in
);

/*
    The AfxClampRect() function makes sure each individual field (x, y, w, h) stays within bounds,
    not concerned with ensuring the rectangle stays spatially inside some container.
    Returns the area of the rectangle in square units. If zero, the rectangle has not area.
*/

AFXINL afxUnit      AfxClampRect
// Returns the area of the rectangle in square units. If zero, the rectangle has not area.
(
    // The rectangle where clamped area will be stored.
    afxRect*        rc, 
    // The source rectangle to be clamped.
    afxRect const*  src, 
    // The rectangle defining the minimum allowed coordinates. (Lower bounds)
    afxRect const*  min, 
    // The rectangle defining the maximum allowed coordinates. (Upper bounds)
    afxRect const*  max
);

#endif//AFX_RECT_H
