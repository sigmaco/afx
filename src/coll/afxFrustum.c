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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/coll/afxFrustum.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/coll/afxPlane.h"
#include "qwadro/mem/afxMemory.h"

_AFX afxV4d const AFX_V4D_LBN = { -1, -1, -1, 1 };
_AFX afxV4d const AFX_V4D_RBN = { 1, -1, -1, 1 };
_AFX afxV4d const AFX_V4D_RTN = { 1, 1, -1, 1 };
_AFX afxV4d const AFX_V4D_LTN = { -1, 1, -1, 1 };

_AFX afxV4d const AFX_V4D_LBF = { -1, -1, 1, 1 };
_AFX afxV4d const AFX_V4D_RBF = { 1, -1, 1, 1 };
_AFX afxV4d const AFX_V4D_RTF = { 1, 1, 1, 1 };
_AFX afxV4d const AFX_V4D_LTF = { -1, 1, 1, 1 };

_AFXINL void AfxGetFrustumCorners(afxFrustum* f, afxV3d vertices[AFX_NUM_BOX_CORNERS])
{
    afxError err = { 0 };
    AFX_ASSERT(vertices);
    AFX_ASSERT(f);
    AfxCopy(vertices, f->corners, sizeof(f->corners));
}

_AFXINL void AfxGetFrustumEdges(afxFrustum* f, afxV3d vertices[AFX_NUM_BOX_CORNERS], afxUnit indices[AFX_NUM_BOX_EDGE_VERTICES])
{
    afxError err = { 0 };
    AFX_ASSERT(vertices);
    AFX_ASSERT(f);

    if (vertices)
        AfxGetFrustumCorners(f, vertices);

    if (indices)
    {
        // Indices for the frustum, connecting the corners to form the frustum's 12 edges.
        afxUnit const frustumIndices[] =
        {
            0, 1, 1, 2, 2, 3, 3, 0, // Near plane edges
            4, 5, 5, 6, 6, 7, 7, 4, // Far plane edges
            0, 4, 1, 5, 2, 6, 3, 7  // Connecting edges between near and far planes
        };
        AfxCopy(indices, frustumIndices, sizeof(frustumIndices));
    }
    // 24 indices (12 edges * 2 vertices per edge).
}

_AFXINL void AfxCopyFrustum(afxFrustum* f, afxFrustum const* in)
{
    afxError err = { 0 };
    AFX_ASSERT(f);
    AFX_ASSERT(in);
    afxUnit i;

    AfxV4dCopy(f->origin, in->origin);

    AfxCopyPlanes(afxCubeFace_TOTAL, in->planes, f->planes);

    for (i = 0; i < afxCubeCorner_TOTAL; i++)
        AfxV4dCopy(f->corners[i], in->corners[i]);
}

_AFXINL afxBool AfxDoesFrustumCullSpheres(afxFrustum const* f, afxUnit cnt, afxSphere const spheres[])
{
    afxError err = { 0 };
    AFX_ASSERT(!cnt || spheres);
    AFX_ASSERT(f);

    for (afxUnit i = 0; i < cnt; ++i)
    {
        afxSphere const* sph = &spheres[i];

        for (afxUnit j = 0; j < afxCubeFace_TOTAL; ++j)
            if (!AfxPlaneTestSpheres(&f->planes[i], 1, sph))
                return FALSE;
    }
    return TRUE;
}

_AFXINL afxResult AfxAabbTestPlanes(afxBox const* aabb, afxUnit cnt, afxPlane const planes[])
{
    afxError err = { 0 };
    AFX_ASSERT2(aabb, planes);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal minD;
        afxReal maxD;

        afxPlane const* p = &planes[i];

        if (p->uvwd[0] > 0.0f)
        {
            minD = p->uvwd[0] * aabb->min[0];
            maxD = p->uvwd[0] * aabb->max[0];
        }
        else
        {
            minD = p->uvwd[0] * aabb->max[0];
            maxD = p->uvwd[0] * aabb->min[0];
        }

        if (p->uvwd[1] > 0.0f)
        {
            minD += p->uvwd[1] * aabb->min[1];
            maxD += p->uvwd[1] * aabb->max[1];
        }
        else
        {
            minD += p->uvwd[1] * aabb->max[1];;
            maxD += p->uvwd[1] * aabb->min[1];
        }

        if (p->uvwd[2] > 0.0f)
        {
            minD += p->uvwd[2] * aabb->min[2];
            maxD += p->uvwd[2] * aabb->max[2];
        }
        else
        {
            minD += p->uvwd[2] * aabb->max[2];
            maxD += p->uvwd[2] * aabb->min[2];
        }

        // What side of the plane

        if (minD >= -p->uvwd[AFX_PLANE_DIST])
            return +1;

        if (maxD <= -p->uvwd[AFX_PLANE_DIST])
            return -1;
    }
    //Intersection
    return 0;
}

_AFXINL afxBool AfxDoesFrustumCullAabbs(afxFrustum const* f, afxUnit cnt, afxBox const aabb[])
{
    afxError err = { 0 };
    AFX_ASSERT(f);
    AFX_ASSERT(aabb);

    for (afxUnit i = 0; i < cnt; ++i)
    {
        if (AfxAabbTestPlanes(&aabb[i], afxCubeFace_TOTAL, f->planes) == -1)
            return FALSE;
    }
    return TRUE;
#if 0
    afxV4d v;
    afxBool volInside = TRUE;

    // check box outside/inside of frustum

    for (afxUnit i = 0; i < afxCubeFace_TOTAL; i++)
    {
        v.x = f->planes[i].x;
        v.y = f->planes[i].y;
        v.z = f->planes[i].z;
        v.w = f->planes[i].w;

        afxUnit out = 0;
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_INF].x, aabb[AFX_AABB_INF].y, aabb[AFX_AABB_INF].z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_SUP].x, aabb[AFX_AABB_INF].y, aabb[AFX_AABB_INF].z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_INF].x, aabb[AFX_AABB_SUP].y, aabb[AFX_AABB_INF].z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_SUP].x, aabb[AFX_AABB_SUP].y, aabb[AFX_AABB_INF].z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_INF].x, aabb[AFX_AABB_INF].y, aabb[AFX_AABB_SUP].z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_SUP].x, aabb[AFX_AABB_INF].y, aabb[AFX_AABB_SUP].z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_INF].x, aabb[AFX_AABB_SUP].y, aabb[AFX_AABB_SUP].z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_SUP].x, aabb[AFX_AABB_SUP].y, aabb[AFX_AABB_SUP].z, 1.0f)) < 0.0) ? 1 : 0);

        if (out == 8)
        {
            volInside = FALSE;
            break;
        }
    }

    if (volInside)
    {
        // check frustum outside/inside box

        for (afxUnit j = 0; j < 3; j++)
        {
            afxUnit out;

            for (afxUnit i = 0; i < afxCubeCorner_TOTAL; i++)
                out += ((f->corners[i][j] > aabb[AFX_AABB_SUP][j]) ? 1 : 0);

            if (out == 8)
            {
                volInside = FALSE;
                break;
            }

            for (afxUnit i = 0; i < afxCubeCorner_TOTAL; i++)
                out += ((f->corners[i][j] < aabb[AFX_AABB_INF][j]) ? 1 : 0);

            if (out == 8)
            {
                volInside = FALSE;
                break;
            }
        }
    }
    return !volInside;
#endif
}

_AFXINL afxBool AfxDoesFrustumCullFrustums(afxFrustum const* f, afxUnit cnt, afxFrustum const others[])
{
    afxError err = { 0 };
    AFX_ASSERT(!cnt || others);
    AFX_ASSERT(f);

    afxBool allOut = TRUE;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxFrustum const* o = &others[i];

        for (afxUnit j = 0; j < afxCubeFace_TOTAL; ++j)
        {
            for (afxUnit k = 0; k < afxCubeCorner_TOTAL; ++k)
            {
                if (AfxFindPlaneDistance(&f->planes[j], o->corners[k]) < 0)
                {
                    allOut = FALSE;
                    break;
                }
            }

            if (!allOut)
                break;
        }
    }
    return allOut;
}

_AFXINL void AfxEmboxFrustum(afxBox* aabb, afxFrustum const* f)
{
    afxError err = { 0 };
    AFX_ASSERT(f);
    AFX_ASSERT(aabb);
    AfxEmboxPoints(aabb, afxCubeCorner_TOTAL, f->corners);
}

_AFXINL void AfxMakeFrustum(afxFrustum* f, afxM4d const pv, afxM4d const ipv)
{
    afxError err = { 0 };
    AFX_ASSERT(f);
    AFX_ASSERT(pv);

    /*
        Planes in camera space (using PV matrix)
        The frustum planes are defined in camera space, which is why we calculate them directly from the projection-view matrix (PV). 
        The projection-view matrix is a transformation matrix that combines both the view matrix (camera's position and orientation) and 
        the projection matrix (the camera's frustum properties).

        The frustum planes are typically extracted from the combined PV matrix because the matrix transforms geometry from world space 
        into clip space (and ultimately into camera space after the perspective divide). The camera-space frustum planes are easy to 
        compute by directly accessing the components of the projection-view matrix.
    */

    // Calculate frustum planes from the view-projection matrix.

    // Left plane
    AfxMakePlane(&f->planes[afxCubeFace_L], AFX_V3D(pv[0][3] + pv[0][0], pv[1][3] + pv[1][0], pv[2][3] + pv[2][0]), pv[3][3] + pv[3][0]);
    // Right plane
    AfxMakePlane(&f->planes[afxCubeFace_R], AFX_V3D(pv[0][3] - pv[0][0], pv[1][3] - pv[1][0], pv[2][3] - pv[2][0]), pv[3][3] - pv[3][0]);
    // Bottom plane
    AfxMakePlane(&f->planes[afxCubeFace_B], AFX_V3D(pv[0][3] + pv[0][1], pv[1][3] + pv[1][1], pv[2][3] + pv[2][1]), pv[3][3] + pv[3][1]);
    // Top plane
    AfxMakePlane(&f->planes[afxCubeFace_T], AFX_V3D(pv[0][3] - pv[0][1], pv[1][3] - pv[1][1], pv[2][3] - pv[2][1]), pv[3][3] - pv[3][1]);
    // Near plane
    AfxMakePlane(&f->planes[afxCubeFace_N], AFX_V3D(pv[0][3] + pv[0][2], pv[1][3] + pv[1][2], pv[2][3] + pv[2][2]), pv[3][3] + pv[3][2]);
    // Far plane
    AfxMakePlane(&f->planes[afxCubeFace_F], AFX_V3D(pv[0][3] - pv[0][2], pv[1][3] - pv[1][2], pv[2][3] - pv[2][2]), pv[3][3] - pv[3][2]);

    /*
        Corners in world space (using inverse of projection-view matrix)
        When extracting the frustum corners, we are interested in the coordinates of the corners in world space (not camera space), 
        so we need to transform the corners from normalized device coordinates (NDC) to world space. The inverse of the projection-view 
        matrix is used to map the corners from NDC (which lies between [-1, 1] in all three axes) to world space, where the camera's 
        position and orientation are respected.
    */

    // Define the 8 corners in normalized device coordinates (NDC)
    afxV4d const ndcCorners[8] =
    {
        { -1, -1, -1, 1 },  // Near-Left-Bottom
        {  1, -1, -1, 1 },  // Near-Right-Bottom
        {  1,  1, -1, 1 },  // Near-Right-Top
        { -1,  1, -1, 1 },  // Near-Left-Top
        { -1, -1,  1, 1 },  // Far-Left-Bottom
        {  1, -1,  1, 1 },  // Far-Right-Bottom
        {  1,  1,  1, 1 },  // Far-Right-Top
        { -1,  1,  1, 1 }   // Far-Left-Top
    };

    // Transform the corners from NDC to world space by multiplying with the inverse projection-view matrix.
    AfxM4dPostMultiplyV4d(ipv, ARRAY_SIZE(ndcCorners), ndcCorners, f->corners);
    for (afxUnit i = 0; i < ARRAY_SIZE(ndcCorners); i++)
    {
        // Perform the perspective divide to normalize (divide by w)
        AfxV3dDiv(f->corners[i], f->corners[i], AFX_V3D(f->corners[i][3], f->corners[i][3], f->corners[i][3]));
        // Homogeneous coordinate after perspective divide
        f->corners[i][3] = 1.f;
    }
}
