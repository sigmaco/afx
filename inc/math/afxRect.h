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

#ifndef AVX_RECT_H
#define AVX_RECT_H

#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/draw/afxDrawDefs.h"

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

AVX afxRect const AVX_RECT_ZERO;
AVX afxRect const AVX_RECT_MIN;
AVX afxRect const AVX_RECT_MAX;

#define AVX_RECT(x_, y_, w_, h_) (afxRect){ \
    .x = (afxInt)(x_), \
    .y = (afxInt)(y_), \
    .w = (afxUnit)(w_), \
    .h = (afxUnit)(h_) }

/*
    The AfxGetRectArea() function calculates the square units for a rectangle.
    The resulting value can be used to determine whether a rectangle is or not empty.
*/

AVXINL afxUnit      AfxGetRectArea
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

AVXINL afxUnit      AfxMergeRects
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

AVXINL afxUnit      AfxIntersectRects
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

AVXINL afxBool      AvxAreRectsOverlapping
(
    afxRect const*  a,
    afxRect const*  b
);

/*
    The function checks whether rectangle @a contains rectangle @b, with a bias or tolerance applied
    meaning @b is allowed to "poke out" by up to @tolX and @tolY on the respective axes.

    It's like a "soft containment" check, useful for layout fuzziness, hit testing, or floating-point rounding tolerances.
*/

AVXINL afxBool      AvxDoesRectContainBiased
(
    afxRect const*  a, 
    afxRect const*  b, 
    afxInt          tolX, 
    afxInt          tolY
);

/*
    The AvxDoesRectContain() function checks whether rect @a fully contains rect @b.
    It's inclusive containment, which mean boundaries are allowed to touch.
    Returns true if @b fits completely within @a.
*/

AVXINL afxBool      AvxDoesRectContain
(
    // The major rect.
    afxRect const*  a,
    // The minor rect.
    afxRect const*  b
);

AVXINL afxBool      AvxIsRectOutside
(
    afxRect const*  a, 
    afxRect const*  b
);

AVXINL afxBool      AvxDoRectsIntersect
(
    afxRect const*  a, 
    afxRect const*  b
);

/*
    The AfxFlipRectVertically() function transforms the rectangle's y coordinate, 
    flipping it relative to the vertical extent (origin shift) - using the total height of the space to invert it for bottom-up/top-down rendering.
*/

AFXINL void         AfxFlipRectVertically
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

AVXINL afxUnit      AfxScissorRect
(
    afxRect const*  rc, 
    afxUnit         maxW, 
    afxUnit         maxH, 
    afxRect         outsides[4]
);

/*
    The AfxMakeRect() function calculates the inclusive bounding rectangle for a number of "outside" rectangles
    (rectangles outside a given rectangle), the goal is to take a list of rectangles and return the smallest rectangle that
    encompasses all those given rectangles.
*/

AVXINL void         AfxMakeRect
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

AVXINL afxUnit      AfxClampRect
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

#endif//AVX_RECT_H
