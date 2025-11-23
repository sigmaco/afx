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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#include "avxIcd.h"

/*
    Algorithm: Sutherland–Hodgman Clipping (Adapted for homogeneous 4D)
    This is a polygon clipping algorithm extended to work in 4D homogeneous clip space.

    Clip the triangle against each of the 6 clip planes.
    For each plane:
        If all points are inside, then keep it.
        If all points are outside, then discard it.
        If the triangle straddles the plane:
            Interpolate to compute new intersection points (on the plane).
            Create one or two new triangles from the split.
*/

afxBool ClipTestInside(afxV4d v, int plane)
{
    // Core Plane Clipping Function (Example: Clip against x <= w)

    switch (plane)
    {
    case 0: return v[0] >= -v[3]; // left
    case 1: return v[0] <= v[3]; // right
    case 2: return v[1] >= -v[3]; // bottom
    case 3: return v[1] <= v[3]; // top
    case 4: return v[2] >= -v[3]; // near
    case 5: return v[2] <= v[3]; // far
    }
    return FALSE;
}

void ClipIntersectVertices(afxV4d v, afxV4d a, afxV4d b, int plane)
{

    // Interpolating Between Vertices
    // If we have two vertices, v0 and v1, and one is inside while the other is outside, 
    // we find the intersection point on the clip plane using homogeneous interpolation.

    afxReal t;
    afxReal aVal, bVal;

    switch (plane)
    {
    case 0: aVal = a[0] + a[3]; bVal = b[0] + b[3]; break; // x >= -w
    case 1: aVal = a[3] - a[0]; bVal = b[3] - b[0]; break; // x <= +w
    case 2: aVal = a[1] + a[3]; bVal = b[1] + b[3]; break;
    case 3: aVal = a[3] - a[1]; bVal = b[3] - b[1]; break;
    case 4: aVal = a[2] + a[3]; bVal = b[2] + b[3]; break;
    case 5: aVal = a[3] - a[2]; bVal = b[3] - b[2]; break;
    }

    t = aVal / (aVal - bVal);
    AfxV4dMix(v, a, b, t); // mix(a, b, t); // Linear interpolation in homogeneous space
}

afxUnit ClipTriangle(afxUnit vtxCnt, afxV4d input[], afxV4d vertices[])
{
    afxUnit rsltCnt = 0;

    for (afxUnit plane = 0; plane < 6; ++plane)
    {
        for (afxUnit i = 0; i < vtxCnt; ++i)
        {
            //afxReal* current = input[i];
            //afxReal* prev = input[(i + vtxCnt - 1) % vtxCnt];

            afxBool currIn = ClipTestInside(/*current*/input[i], plane);
            afxBool prevIn = ClipTestInside(/*prev*/input[(i + vtxCnt - 1) % vtxCnt], plane);

            if (currIn && prevIn)
            {
                AfxV4dCopy(vertices[rsltCnt], /*current*/input[i]);
                ++rsltCnt;
            }
            else if (currIn && !prevIn)
            {
                ClipIntersectVertices(vertices[rsltCnt], /*prev*/input[(i + vtxCnt - 1) % vtxCnt], /*current*/input[i], plane);
                ++rsltCnt;
                AfxV4dCopy(vertices[rsltCnt], /*current*/input[i]);
                ++rsltCnt;
            }
            else if (!currIn && prevIn)
            {
                ClipIntersectVertices(vertices[rsltCnt], /*prev*/input[(i + vtxCnt - 1) % vtxCnt], /*current*/input[i], plane);
                ++rsltCnt;
            }
            // else both out -> discard edge.
        }

        if (rsltCnt < 3) break; // Clipped away completely
    }

    /*
        Clipping is done in 4D clip space to preserve linear interpolation across planes.
        GPU hardware clips automatically, but in custom engines or geometry shaders, this logic matters.
        After clipping, continue as normal: divide by W, transform to NDC, map via viewport

        Output.
        After clipping, the original triangle might turn into:
            1 triangle (if partially clipped)
            2 triangles (if cut across a plane)
            0 triangles (if fully outside the frustum)
        Each output vertex is still a vec4 in clip space, and we'll still perform the perspective divide (v.xyz / v.w) afterward.
    */

    return rsltCnt;
}
