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

_AVXINL afxBool AfxClipRectUnion(afxRect* rc, afxRect const* a, afxRect const* b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(rc);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    
    afxInt x = AFX_MIN(a->x, b->x);
    afxInt y = AFX_MIN(a->y, b->y);
    afxUnit w = AFX_MAX(a->x + a->w, b->x + b->w);
    afxUnit h = AFX_MAX(a->y + a->h, b->y + b->h);
    rc->x = x;
    rc->y = y;
    rc->w = w - x;
    rc->h = h - y;
    return (rc->w > 0 && rc->h > 0);
}

_AVXINL afxBool AfxClipRectIntersection(afxRect* rc, afxRect const* a, afxRect const* b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(rc);
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    afxInt x = AFX_MAX(a->x, b->x);
    afxInt y = AFX_MAX(a->y, b->y);
    afxUnit w = AFX_MIN(a->x + a->w, b->x + b->w);
    afxUnit h = AFX_MIN(a->y + a->h, b->y + b->h);

    if (w > x && h > y)
    {
        rc->x = x;
        rc->y = y;
        rc->w = w - x;
        rc->h = h - y;
        return TRUE;
    }
    else
    {
        rc->x = rc->y = rc->w = rc->h = 0; // Optional: clear out
        return FALSE;
    }
}

_AVXINL afxBool AvxDoesRectOverlaps(afxRect const* a, afxRect const* b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    // is B completely right of A?
    if (AFX_MAX(a->x, (afxInt)a->w) < AFX_MIN(b->x, (afxInt)b->w))
        return FALSE;

    // is B completely left of A?
    if (AFX_MAX(b->x, (afxInt)b->w) < AFX_MIN(a->x, (afxInt)a->w))
        return FALSE;

    // is B completely above A?
    if (AFX_MAX(a->y, (afxInt)a->h) < AFX_MIN(b->y, (afxInt)b->h))
        return FALSE;

    // is B completely below A?
    if (AFX_MAX(b->y, (afxInt)b->h) < AFX_MIN(a->y, (afxInt)a->h))
        return FALSE;

    return TRUE; // overlapping
}

_AVXINL afxBool AvxDoesRectContainBiased(afxRect const* a, afxRect const* b, afxInt tolX, afxInt tolY)
// Compare if rectangle \a a contains rectangle \a b in coordinate, with specified tolerance allowed.
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    return  (b->x >= a->x - tolX) &&
            (b->y >= a->y - tolY) &&
            (b->x + b->w <= a->x + a->w + tolX) &&
            (b->y + b->h <= a->y + a->h + tolY);
}

_AVXINL afxBool AvxDoesRectContain(afxRect const* a, afxRect const* b)
// This checks that all corners of b are inside a, i.e., b is entirely within a.
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(a);
    AFX_ASSERT(b);    
    return  (b->x >= a->x) &&
            (b->y >= a->y) &&
            (b->x + b->w <= a->x + a->w) &&
            (b->y + b->h <= a->y + a->h);
}

_AVXINL afxBool AvxIsRectOutside(afxRect const* a, afxRect const* b)
// This test whether @a and @b don't intersect at all.
{
    return (b->x + b->w <= a->x ||
        b->x >= a->x + a->w ||
        b->y + b->h <= a->y ||
        b->y >= a->y + a->h);
}

_AVXINL afxBool AvxDoRectsIntersect(afxRect const* a, afxRect const* b)
// This test whether @a and @b overlap at all (fully or partially).
{
    return !(b->x + b->w <= a->x ||
        b->x >= a->x + a->w ||
        b->y + b->h <= a->y ||
        b->y >= a->y + a->h);
}

_AFXINL void AfxFlipRectVertically(afxRect const* rc, afxUnit height, afxRect* flipped)
{
    afxError err = NIL;
    AFX_ASSERT(flipped);
    AFX_ASSERT(height);
    AFX_ASSERT(rc);
    *flipped = AVX_RECT(rc->x, (height - (rc->y + rc->h)), rc->w, rc->h);
}

_AVXINL afxUnit AfxClipRectExclusion(afxRect const* rc, afxUnit maxW, afxUnit maxH, afxRect outsides[4])
{
    afxError err = NIL;
    AFX_ASSERT(outsides);
    AFX_ASSERT(rc);
    afxUnit invRcCnt = 0;

    // If the given rectangle is completely inside the screen, we calculate the outside areas.

    // Left exclusion
    if (rc->x < 0)
        outsides[invRcCnt++] = AVX_RECT( rc->x, rc->y, -rc->x, rc->h );

    // Right exclusion
    if (rc->x + rc->w > maxW)
        outsides[invRcCnt++] = AVX_RECT(maxW, rc->y, (rc->x + rc->w) - maxW, rc->h );

    // Top exclusion
    if (rc->y < 0)
        outsides[invRcCnt++] = AVX_RECT( rc->x, rc->y, rc->w, -rc->y );

    // Bottom exclusion
    if (rc->y + rc->h > maxH)
        outsides[invRcCnt++] = AVX_RECT(rc->x, maxH, rc->w, (rc->y + rc->h) - maxH );

    return invRcCnt;
}

_AVXINL void AfxMergeRects(afxRect* rc, afxUnit cnt, afxRect* in)
{
    afxError err = NIL;
    AFX_ASSERT(in);
    AFX_ASSERT(rc);
    // Initialize to extreme values
    afxInt minX = INT_MAX;
    afxInt minY = INT_MAX;
    afxUnit maxX = INT_MIN;
    afxUnit maxY = INT_MIN;

    // Iterate over the list of rectangles
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxRect r = in[i];

        // Update min and max values for bounding rectangle
        if (r.x < minX) minX = r.x;
        if (r.y < minY) minY = r.y;
        if (r.x + r.w > maxX) maxX = r.x + r.w;
        if (r.y + r.h > maxY) maxY = r.y + r.h;
    }

    *rc = AVX_RECT(minX, minY, maxX - minX, maxY - minY);
}
