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

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

#include "qwadro/base/afxRect.h"

afxRect const AFX_RECT_ZERO = { 0, 0, 0, 0 };
afxRect const AFX_RECT_MIN = { AFX_I32_MIN, AFX_I32_MIN, 1, 1 };
afxRect const AFX_RECT_MAX = { AFX_I32_MAX, AFX_I32_MAX, AFX_U32_MAX, AFX_U32_MAX };

afxLayeredRect const AFX_LAYERED_RECT_ZERO = { { 0, 0, 0, 0 }, 0, 0 };
afxLayeredRect const AFX_LAYERED_RECT_MIN = { { AFX_I32_MIN, AFX_I32_MIN, 1, 1 }, 0, 1 };
afxLayeredRect const AFX_LAYERED_RECT_MAX = { { AFX_I32_MAX, AFX_I32_MAX, AFX_U32_MAX, AFX_U32_MAX }, AFX_U32_MAX, AFX_U32_MAX };

_AFXINL afxUnit AfxGetRectArea(afxRect const* rc)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    return (rc->w * rc->h);
}

_AFXINL afxUnit AfxMergeRects(afxRect* rc, afxRect const* a, afxRect const* b)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    
    // Essentially the inverse of AfxIntersectRects().
    afxInt x = AFX_MIN(a->x, b->x);
    afxInt y = AFX_MIN(a->y, b->y);
    rc->x = x;
    rc->y = y;
    rc->w = AFX_MAX(a->x + a->w, b->x + b->w) - x;
    rc->h = AFX_MAX(a->y + a->h, b->y + b->h) - y;
    return (rc->w * rc->h);
}

_AFXINL afxUnit AfxIntersectRects(afxRect* rc, afxRect const* a, afxRect const* b)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    // Essentially the inverse of AfxMergeRects().
    afxInt x = AFX_MAX(a->x, b->x);
    afxInt y = AFX_MAX(a->y, b->y);
    rc->x = x;
    rc->y = y;
    rc->w = AFX_MIN(a->x + a->w, b->x + b->w) - x;
    rc->h = AFX_MIN(a->y + a->h, b->y + b->h) - y;
    return (rc->w * rc->h);
}

_AFXINL afxBool AfxAreRectsEqual(afxRect const* a, afxRect const* b)
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    return ((a->x == b->x) && (a->y == b->y) && (a->w == b->w) && (a->h == b->h));
}

_AFXINL afxBool AfxAreRectsOverlapping(afxRect const* a, afxRect const* b)
{
    afxError err = { 0 };
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

_AFXINL afxBool AfxDoesRectContainBiased(afxRect const* a, afxRect const* b, afxInt tolX, afxInt tolY)
// Compare if rectangle \a a contains rectangle \a b in coordinate, with specified tolerance allowed.
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    return  (b->x >= a->x - tolX) &&
            (b->y >= a->y - tolY) &&
            ((b->x + b->w) <= (a->x + a->w + tolX)) &&
            ((b->y + b->h) <= (a->y + a->h + tolY));
}

_AFXINL afxBool AfxDoesRectContain(afxRect const* a, afxRect const* b)
// This checks that all corners of b are inside a, i.e., b is entirely within a.
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);    
    return  (b->x >= a->x) &&
            (b->y >= a->y) &&
            ((b->x + b->w) <= (a->x + a->w)) &&
            ((b->y + b->h) <= (a->y + a->h));
}

_AFXINL afxBool AfxIsRectOutside(afxRect const* a, afxRect const* b)
// This test whether @a and @b don't intersect at all.
{
    return ((b->x + (afxInt)b->w) <= a->x ||
        b->x >= (a->x + (afxInt)a->w) ||
        (b->y + (afxInt)b->h) <= a->y ||
        b->y >= (a->y + (afxInt)a->h));
}

_AFXINL afxBool AfxDoRectsIntersect(afxRect const* a, afxRect const* b)
// This test whether @a and @b overlap at all (fully or partially).
{
    return !((b->x + (afxInt)b->w) <= a->x ||
        b->x >= (a->x + (afxInt)a->w) ||
        (b->y + (afxInt)b->h) <= a->y ||
        b->y >= (a->y + (afxInt)a->h));
}

_AFXINL void AfxExtractRectMargins(afxRect const* outer, afxRect const* inner, afxInt* left, afxInt* top, afxInt* right, afxInt* bottom)
{
    afxError err = { 0 };
    AFX_ASSERT(outer);
    AFX_ASSERT(inner);
    AFX_ASSERT(left);
    AFX_ASSERT(top);
    AFX_ASSERT(right);
    AFX_ASSERT(bottom);
    // calculate the margins between two such rectangles, for example, the difference between an outer rectangle and an inner rectangle;
    // where the inner rectangle is fully contained inside the outer.

    // Assumes inner is inside outer.
    *left = inner->x - outer->x;
    *top = inner->y - outer->y;
    *right = (outer->x + (afxInt)outer->w) - (inner->x + (afxInt)inner->w);
    *bottom = (outer->y + (afxInt)outer->h) - (inner->y + (afxInt)inner->h);
}

_AFXINL void AfxFlipRect(afxRect const* rc, afxUnit height, afxRect* flipped)
{
    afxError err = { 0 };
    AFX_ASSERT(flipped);
    AFX_ASSERT(height);
    AFX_ASSERT(rc);
    *flipped = AFX_RECT(rc->x, (height - (rc->y + rc->h)), rc->w, rc->h);
}

_AFXINL afxUnit AfxScissorRect(afxRect const* rc, afxUnit maxW, afxUnit maxH, afxRect outsides[4])
{
    afxError err = { 0 };
    AFX_ASSERT(outsides);
    AFX_ASSERT(rc);
    afxUnit invRcCnt = 0;

    // If the given rectangle is completely inside the screen, we calculate the outside areas.

    // These rects may overlap, especially at corners - e.g. if a rect spills out top-left, both the left and top exclusions will cover that corner area.
    // This is different from a region subtraction, where we would remove the intersection and return only non-overlapping leftovers.
    // Useful in GUI layout engines, clipping rejection, or debugging overlays.

    // Left exclusion
    if (rc->x < 0)
        outsides[invRcCnt++] = AFX_RECT( rc->x, rc->y, -rc->x, rc->h );

    // Right exclusion
    if (rc->x + rc->w > maxW)
        outsides[invRcCnt++] = AFX_RECT(maxW, rc->y, (rc->x + rc->w) - maxW, rc->h );

    // Top exclusion
    if (rc->y < 0)
        outsides[invRcCnt++] = AFX_RECT( rc->x, rc->y, rc->w, -rc->y );

    // Bottom exclusion
    if (rc->y + rc->h > maxH)
        outsides[invRcCnt++] = AFX_RECT(rc->x, maxH, rc->w, (rc->y + rc->h) - maxH );

    return invRcCnt;
}

_AFXINL afxUnit AfxClampRect(afxRect* rc, afxRect const* src, afxRect const* min, afxRect const* max)
{
    afxError err = { 0 };
    AFX_ASSERT(src);
    AFX_ASSERT(min);
    AFX_ASSERT(max);
    AFX_ASSERT(rc);
    *rc = AFX_RECT( AFX_CLAMP(src->x, min->x, max->x),
                    AFX_CLAMP(src->y, min->y, max->y),
                    AFX_CLAMP(src->w, min->w, max->w),
                    AFX_CLAMP(src->h, min->h, max->h));
    // Returns the area = w * h; (like 4 x 2 = 8 square units)
    return (rc->w * rc->h);
}

_AFXINL void AfxAccumulateRects(afxRect* rc, afxUnit cnt, afxRect* in)
{
    afxError err = { 0 };
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

    *rc = AFX_RECT(minX, minY, maxX - minX, maxY - minY);
}
