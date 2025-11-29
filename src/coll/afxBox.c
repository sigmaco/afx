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

#include "qwadro/coll/afxBox.h"
#include "qwadro/coll/afxSphere.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/mem/afxMemory.h"

_AFX afxBox const AFX_AABB_IDENTITY = { .min = { 0, 0, 0, 1 }, .max = { 0, 0, 0, 1 } };
// Void AABB is a box initialized with extreme values meaning no volume. A AABB is valid only if min is less or equal to max.
_AFX afxBox const AFX_AABB_VOID = { .min = { AFX_R32_MAX, AFX_R32_MAX, AFX_R32_MAX, 1 }, .max = { -AFX_R32_MAX, -AFX_R32_MAX, -AFX_R32_MAX, 1 } };

_AFXINL void AfxMakeAabb(afxBox* bb, afxUnit cnt, afxV3d const points[])
{
    afxError err = { 0 };
    AFX_ASSERT(bb);
    AFX_ASSERT(!cnt || points);

    // Initialize the AABB with extreme values
    AfxFillV4d(bb->min, AFX_R32_MAX);
    bb->min[3] = 1;
    AfxFillV4d(bb->max, -AFX_R32_MAX);
    bb->max[3] = 1;

    if (cnt)
    {
        // Add the points regularly.
        AfxEmboxVectors(bb, cnt, points);
    }
}

_AFXINL void AfxEmboxTriangles(afxBox* bb, afxUnit triCnt, afxV3d const vertices[], afxUnit const indices[], afxUnit idxStride)
{
    afxError err = { 0 };
    AFX_ASSERT(vertices);
    AFX_ASSERT(bb);

    // To generate an Axis-Aligned Bounding Box (AABB) from an array of vertices, you'll need to calculate the minimum 
    // and maximum values for the x, y, and z coordinates. 
    // These minimum and maximum values will define the corners of the AABB.

    if (!indices)
    {
        // Loop through the points and adjust the min and max values
        for (afxUnit i = 0; i < triCnt; i++)
        {
            afxUnit ia = i * 3 + 0;
            afxUnit ib = i * 3 + 1;
            afxUnit ic = i * 3 + 2;
            AFX_ASSERT_TRIANGLE_BOUNDS(ia, ib, ic, triCnt);

            AfxV3dMin(bb->min, bb->min, vertices[ia]);
            AfxV3dMax(bb->max, bb->max, vertices[ia]);
            AfxV3dMin(bb->min, bb->min, vertices[ib]);
            AfxV3dMax(bb->max, bb->max, vertices[ib]);
            AfxV3dMin(bb->min, bb->min, vertices[ic]);
            AfxV3dMax(bb->max, bb->max, vertices[ic]);
        }
    }
    else
    {
        if (idxStride == 4)
        {
            afxUnit32 const* indices2 = (void*)indices;

            // Loop through the points and adjust the min and max values
            for (afxUnit i = 0; i < triCnt; i++)
            {
                afxUnit ia = indices2[i * 3 + 0];
                afxUnit ib = indices2[i * 3 + 1];
                afxUnit ic = indices2[i * 3 + 2];
                AFX_ASSERT_TRIANGLE_BOUNDS(ia, ib, ic, triCnt);

                AfxV3dMin(bb->min, bb->min, vertices[ia]);
                AfxV3dMax(bb->max, bb->max, vertices[ia]);
                AfxV3dMin(bb->min, bb->min, vertices[ib]);
                AfxV3dMax(bb->max, bb->max, vertices[ib]);
                AfxV3dMin(bb->min, bb->min, vertices[ic]);
                AfxV3dMax(bb->max, bb->max, vertices[ic]);
            }
        }
        else if (idxStride == 2)
        {
            afxUnit16 const* indices2 = (void*)indices;

            // Loop through the points and adjust the min and max values
            for (afxUnit i = 0; i < triCnt; i++)
            {
                afxUnit ia = indices2[i * 3 + 0];
                afxUnit ib = indices2[i * 3 + 1];
                afxUnit ic = indices2[i * 3 + 2];
                AFX_ASSERT_TRIANGLE_BOUNDS(ia, ib, ic, triCnt);

                AfxV3dMin(bb->min, bb->min, vertices[ia]);
                AfxV3dMax(bb->max, bb->max, vertices[ia]);
                AfxV3dMin(bb->min, bb->min, vertices[ib]);
                AfxV3dMax(bb->max, bb->max, vertices[ib]);
                AfxV3dMin(bb->min, bb->min, vertices[ic]);
                AfxV3dMax(bb->max, bb->max, vertices[ic]);
            }
        }
    }
}

_AFXINL void AfxEmboxVectors(afxBox* bb, afxUnit cnt, afxV3d const vectors[])
{
    afxError err = { 0 };
    AFX_ASSERT(bb);
    AFX_ASSERT(vectors);

    // To generate an Axis-Aligned Bounding Box (AABB) from an array of points, you'll need to calculate the minimum 
    // and maximum values for the x, y, and z coordinates (assuming you're working with 3D points). 
    // These minimum and maximum values will define the corners of the AABB.

    // Loop through the points and adjust the min and max values
    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dMin(bb->min, bb->min, vectors[i]);
        AfxV3dMax(bb->max, bb->max, vectors[i]);
    }
}

_AFXINL void AfxEmboxPoints(afxBox* bb, afxUnit cnt, afxV4d const points[])
{
    afxError err = { 0 };
    AFX_ASSERT(bb);
    AFX_ASSERT(points);

    // To generate an Axis-Aligned Bounding Box (AABB) from an array of points, you'll need to calculate the minimum 
    // and maximum values for the x, y, and z coordinates (assuming you're working with 3D points). 
    // These minimum and maximum values will define the corners of the AABB.

    // Loop through the points and adjust the min and max values
    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dMin(bb->min, bb->min, points[i]);
        AfxV3dMax(bb->max, bb->max, points[i]);
    }
}

_AFXINL void AfxEmboxAabbs(afxBox* bb, afxUnit cnt, afxBox const boxes[])
{
    afxError err = { 0 };
    AFX_ASSERT(bb);
    AFX_ASSERT(boxes);

    // To compute the Axis-Aligned Bounding Box(AABB) for an array of AABBs, the process is straightforward. 
    // An AABB is already defined by its minimum and maximum corners (i.e., two 3D points, one for min and one for max), 
    // so you simply need to find the overall minimum and maximum x, y, and z values across all the AABBs in the array.

    AFX_STATIC_ASSERT(offsetof(afxBox, min) == 0, "");

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dMin(bb->min, bb->min, boxes[i].min);
        AfxV3dMax(bb->max, bb->max, boxes[i].max);
    }
}

_AFXINL void AfxEmboxSpheres(afxBox* bb, afxUnit cnt, afxSphere const spheres[])
{
    afxError err = { 0 };
    AFX_ASSERT(bb);
    AFX_ASSERT(spheres);

    // To compute the Axis-Aligned Bounding Box (AABB) for an array of spheres, the concept is similar to computing the AABB for an array of points. 
    // However, a sphere has both a center (point in 3D space) and a radius, so you'll need to account for the radius when computing the AABB.

    // Loop through each sphere and adjust the min and max values of the AABB
    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxEmboxAabbs(bb, 1, (afxBox[]) { AfxGetSphereAabb(spheres[i]) });
    }
    bb->max[3] = (bb->min[3] = AFX_REAL(1));
}

_AFXINL afxBool AfxGetAabbExtents(afxBox const* bb, afxV3d extent)
{
    afxError err = { 0 };
    AFX_ASSERT(bb);
    AFX_ASSERT(extent);
    // Calculate the bounding box extent: (max - min).
    AfxV3dSub(extent, bb->max, bb->min);
    // Multiply by 0.5 to get half the extent (center-to-corner) if so desired.
    return !!(AfxV3dSum(extent));
}

_AFXINL afxReal AfxGetAabbCentre(afxBox const* bb, afxV4d centre)
{
    afxError err = { 0 };
    AFX_ASSERT(bb);
    AFX_ASSERT(centre);
    // (min + max) / 2
    // 0.5 * (min + max)
    centre[0] = (bb->min[0] + bb->max[0]) * 0.5;
    centre[1] = (bb->min[1] + bb->max[1]) * 0.5;
    centre[2] = (bb->min[2] + bb->max[2]) * 0.5;
    centre[3] = 1.f;
    return fmaxf(fmaxf((bb->max[0] - bb->min[0]) * 0.5, (bb->max[1] - bb->min[1]) * 0.5), (bb->max[2] - bb->min[2]) * 0.5);
}

_AFXINL void AfxGetAabbCorners(afxBox const* bb, afxV3d vertices[AFX_NUM_BOX_CORNERS])
// Generate vertices from box corners.
{
    // The vertices are defined as 8 unique corner points.

    afxError err = { 0 };
    AFX_ASSERT(vertices);
    AFX_ASSERT(bb);

    // Initialize the 8 unique vertices (the 8 corners of the cuboid)
    AfxV3dSet(vertices[0], bb->min[0], bb->min[1], bb->min[2]); // Front-left bottom
    AfxV3dSet(vertices[1], bb->max[0], bb->min[1], bb->min[2]); // Front-right bottom
    AfxV3dSet(vertices[2], bb->max[0], bb->min[1], bb->max[2]); // Back-right bottom
    AfxV3dSet(vertices[3], bb->min[0], bb->min[1], bb->max[2]); // Back-left bottom
    AfxV3dSet(vertices[4], bb->min[0], bb->max[1], bb->min[2]); // Front-left top
    AfxV3dSet(vertices[5], bb->max[0], bb->max[1], bb->min[2]); // Front-right top
    AfxV3dSet(vertices[6], bb->max[0], bb->max[1], bb->max[2]); // Back-right top
    AfxV3dSet(vertices[7], bb->min[0], bb->max[1], bb->max[2]); // Back-left top
}

_AFXINL afxUnit AsxGenerateIndexedLinesForAabbs(afxUnit cnt, afxBox const aabbs[], afxV3d vertices[][AFX_NUM_BOX_CORNERS], afxUnit vtxStride, afxUnit indices[][AFX_NUM_BOX_EDGE_VERTICES], afxUnit idxSiz)
// Generate an Indexed Line List for AABBs.
// A line list means we're rendering edges, so each line connects 2 vertices, and we use pairs of indices to define each line segment.
// For a cube (AABB), there are 12 edges, so we need 24 indices per AABB.
{
    afxError err = { 0 };
    AFX_ASSERT(vertices);
    // vertices[] array must be capable of storing @cnt * 8 vertices.
    AFX_ASSERT(indices);
    // indices[] array must be capable of stroring @cnt * 24 indices.
    
    //if (outIdxCnt) *outIdxCnt = cnt * 24;
    //if (outVtxCnt) *outVtxCnt = cnt * 8;

    afxUnit rslt = cnt * AFX_NUM_BOX_EDGES;

    static afxUnit const cubeLineIndices[AFX_NUM_BOX_EDGE_VERTICES] =
    {
        // Bottom edges
        0, 1,  1, 3,  3, 2,  2, 0,
        // Top edges
        4, 5,  5, 7,  7, 6,  6, 4,
        // Vertical edges
        0, 4,  1, 5,  2, 6,  3, 7
    };

    for (afxUnit bbIdx = 0; bbIdx < cnt; ++bbIdx)
    {
        afxBox const* box = &aabbs[bbIdx];
        afxReal xmin = box->min[0], ymin = box->min[1], zmin = box->min[2];
        afxReal xmax = box->max[0], ymax = box->max[1], zmax = box->max[2];
        afxV3d const corners[AFX_NUM_BOX_CORNERS] =
        {
            {xmin, ymin, zmin}, // 0
            {xmax, ymin, zmin}, // 1
            {xmin, ymax, zmin}, // 2
            {xmax, ymax, zmin}, // 3
            {xmin, ymin, zmax}, // 4
            {xmax, ymin, zmax}, // 5
            {xmin, ymax, zmax}, // 6
            {xmax, ymax, zmax}  // 7
        };

        // vertices[bbIdx * 8 + j], corners[j]
        for (afxUnit j = 0; j < AFX_NUM_BOX_CORNERS; ++j)
        {
            AfxV3dCopy(vertices[bbIdx][j], corners[j]);
        }

        // Copy line indices (offset by current AABB vertex index base)
        // indices[bbIdx * 24 + j] = cubeLineIndices[j] + bbIdx * 8;

        if (sizeof(afxUnit16) == idxSiz)
        {
            afxUnit16* indices16 = (afxUnit16*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_EDGE_VERTICES; ++j)
                indices16[(bbIdx * AFX_NUM_BOX_EDGE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeLineIndices[j];
        }
        else if (sizeof(afxUnit32) == idxSiz)
        {
            afxUnit32* indices32 = (afxUnit32*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_EDGE_VERTICES; ++j)
                indices32[(bbIdx * AFX_NUM_BOX_EDGE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeLineIndices[j];
        }
        else if (sizeof(afxUnit8) == idxSiz)
        {
            afxUnit8* indices8 = (afxUnit8*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_EDGE_VERTICES; ++j)
                indices8[(bbIdx * AFX_NUM_BOX_EDGE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeLineIndices[j];
        }
        else
        {
            AFX_ASSERT((idxSiz == sizeof(afxUnit16)) || (idxSiz == sizeof(afxUnit32)) || (idxSiz == sizeof(afxUnit8)));
            AfxThrowError();
        }
    }
    return rslt;
}

_AFXINL afxUnit AsxGenerateIndexedFacesForAabbs(afxUnit cnt, afxBox const aabbs[], afxV3d vertices[][AFX_NUM_BOX_CORNERS], afxUnit vtxStride, afxUnit indices[][AFX_NUM_BOX_FACE_VERTICES], afxUnit idxSiz)
// Generate an indexed triangle list for an array of AABBs.
// Each AABB will be converted into a cube (box) made of 12 triangles (2 per face × 6 faces).
// Each triangle consists of 3 vertex indices, so you'll generate 36 indices per AABB.
// Each AABB has 8 unique vertices(corners).
{
    afxError err = { 0 };
    AFX_ASSERT(vertices);
    // vertices[] array must be capable of storing @cnt * 8 vertices.
    AFX_ASSERT(indices);
    // indices[] array must be capable of stroring @cnt * 36 indices.
    
    //if (outIdxCnt) *outIdxCnt = cnt * 36;
    //if (outVtxCnt) *outVtxCnt = cnt * 8;

    afxUnit rslt = cnt * AFX_NUM_BOX_FACE_VERTICES;

    static afxUnit const cubeFaceIndices[AFX_NUM_BOX_FACE_VERTICES] =
    {
#if 0 // CW
        // Front face
        0, 1, 2,  2, 1, 3,
        // Back face
        5, 4, 7,  7, 4, 6,
        // Left face
        4, 0, 6,  6, 0, 2,
        // Right face
        1, 5, 3,  3, 5, 7,
        // Top face
        2, 3, 6,  6, 3, 7,
        // Bottom face
        4, 5, 0,  0, 5, 1
#else // CCW
        // Front face (zmin)
        0, 2, 1,  1, 2, 3,
        // Back face (zmax)
        5, 7, 4,  4, 7, 6,
        // Left face (xmin)
        4, 6, 0,  0, 6, 2,
        // Right face (xmax)
        1, 3, 5,  5, 3, 7,
        // Top face (ymax)
        2, 6, 3,  3, 6, 7,
        // Bottom face (ymin)
        4, 0, 5,  5, 0, 1
#endif
    };

    for (afxUnit bbIdx = 0; bbIdx < cnt; ++bbIdx)
    {
        afxBox const* box = &aabbs[bbIdx];
        afxReal xmin = box->min[0], ymin = box->min[1], zmin = box->min[2];
        afxReal xmax = box->max[0], ymax = box->max[1], zmax = box->max[2];
        afxV3d const cubeCorners[AFX_NUM_BOX_CORNERS] =
        {
            {xmin, ymin, zmin}, // 0
            {xmax, ymin, zmin}, // 1
            {xmin, ymax, zmin}, // 2
            {xmax, ymax, zmin}, // 3
            {xmin, ymin, zmax}, // 4
            {xmax, ymin, zmax}, // 5
            {xmin, ymax, zmax}, // 6
            {xmax, ymax, zmax}  // 7
        };
        
        // vertices[bbIdx * 8 + j], corners[j]
        for (afxUnit j = 0; j < AFX_NUM_BOX_CORNERS; ++j)
        {
            AfxV3dCopy(vertices[bbIdx][j], cubeCorners[j]);
        }

        // Copy indices (offset by vertex start index)
        // indices[bbIdx * 36 + j] = cubeIndices[j] + bbIdx * 8;

        if (sizeof(afxUnit16) == idxSiz)
        {
            afxUnit16* indices16 = (afxUnit16*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_FACE_VERTICES; ++j)
                indices16[(bbIdx * AFX_NUM_BOX_FACE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeFaceIndices[j];
        }
        else if (sizeof(afxUnit32) == idxSiz)
        {
            afxUnit32* indices32 = (afxUnit32*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_FACE_VERTICES; ++j)
                indices32[(bbIdx * AFX_NUM_BOX_FACE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeFaceIndices[j];
        }
        else if (sizeof(afxUnit8) == idxSiz)
        {
            afxUnit8* indices8 = (afxUnit8*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_FACE_VERTICES; ++j)
                indices8[(bbIdx * AFX_NUM_BOX_FACE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeFaceIndices[j];
        }
        else
        {
            AFX_ASSERT((idxSiz == sizeof(afxUnit16)) || (idxSiz == sizeof(afxUnit32)) || (idxSiz == sizeof(afxUnit8)));
            AfxThrowError();
        }
    }
    return rslt;
}

_AFXINL afxUnit AfxDoesAabbIncludeAtv3d(afxBox const* bb, afxUnit cnt, afxV3d const point[])
{
    afxError err = { 0 };
    AFX_ASSERT(bb);
    AFX_ASSERT(point);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        if
        (
            (!(bb->max[0] < point[i][0])) && 
            (!(bb->max[1] < point[i][1])) && 
            (!(bb->max[2] < point[i][2])) &&
            (!(bb->min[0] > point[i][0])) && 
            (!(bb->min[1] > point[i][1])) && 
            (!(bb->min[2] > point[i][2]))
        )
        {
            ++rslt;
        }
    }
    return rslt;
}

_AFXINL afxBool AfxIntersectAabbs(afxBox* bb, afxBox const a, afxBox const b)
{
    // Compute the intersection of two AABBs
    // Returns true if they intersect, and fills the result AABB
    
    bb->min[0] = fmaxf(a.min[0], b.min[0]);
    bb->min[1] = fmaxf(a.min[1], b.min[1]);
    bb->min[2] = fmaxf(a.min[2], b.min[2]);

    bb->max[0] = fminf(a.max[0], b.max[0]);
    bb->max[1] = fminf(a.max[1], b.max[1]);
    bb->max[2] = fminf(a.max[2], b.max[2]);

    // Check if the intersection is valid (non-empty)
    return (bb->min[0] <= bb->max[0]) && (bb->min[1] <= bb->max[1]) && (bb->min[2] <= bb->max[2]);
}

_AFXINL afxUnit AfxDoesAabbInclude(afxBox const* bb, afxUnit cnt, afxBox const other[])
{
    afxError err = { 0 };
    AFX_ASSERT(bb);
    AFX_ASSERT(other);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        // Check if an AABB intersects with another AABB (overlapping region)

        if ((bb->min[0] < other[i].max[0] && bb->max[0] > other[i].min[0]) &&
            (bb->min[1] < other[i].max[1] && bb->max[1] > other[i].min[1]) &&
            (bb->min[2] < other[i].max[2] && bb->max[2] > other[i].min[2]))
        {
            ++rslt;
        }
    }
    return rslt;
}

afxBool AfxTestSphereBox(afxSphere const* bs, afxBox const* bb)
{
    afxError err = { 0 };
    AFX_ASSERT(bb);

    // test if a bounding box is fully inside a bounding sphere.

    for (afxUnit i = 0; i < 3; i++)
        if ((bs->xyzr[i] + bs->xyzr[AFX_SPHERE_RADIUS] < bb->min[i]) || 
            (bs->xyzr[i] - bs->xyzr[AFX_SPHERE_RADIUS] > bb->max[i]))
            return FALSE;

    return TRUE;
}

_AFXINL void AfxTransformAabb(afxBox const* bb, afxM4d const m, afxBox* to)
{
    afxError err = { 0 };
    AFX_ASSERT(bb);
    AFX_ASSERT(m);
    AFX_ASSERT(to);
    AfxResetBoxes(1, to, 0);
    AfxM4dPostMultiplyV4d(m, 2, &bb->max, &to->max);
}

_AFXINL void AfxTransformObbs(afxM3d const ltm, afxV4d const atv, afxUnit cnt, afxBox const in[], afxBox out[])
{
    afxError err = { 0 };
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    // box = obb [min, max]

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxV3d max, min, pos;
        AfxFillV3d(max, -3.4028235e38);
        AfxFillV3d(min, 3.4028235e38);

        for (afxUnit z = 0; z < 2; z++)
        {
            for (afxUnit y = 0; y < 2; y++)
            {
                for (afxUnit x = 0; x < 2; x++)
                {
                    afxV3d tmp;
                    AfxV3dSet(tmp,  x ? in[i].max[0] : in[i].min[0],
                                    y ? in[i].max[1] : in[i].min[1],
                                    z ? in[i].max[2] : in[i].min[2]);

                    AfxV3dPostMultiplyM3d(pos, ltm, tmp);
                    AfxV3dAdd(pos, pos, atv);

                    AfxV3dMin(min, min, pos);
                    AfxV3dMax(max, max, pos);
                }
            }
        }

        AfxV3dCopy(out[i].max, max);
        AfxV3dCopy(out[i].min, min);
    }
}

_AFXINL void AfxCopyBoxes(afxUnit cnt, afxBox const in[], afxUnit inStride, afxBox out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(out);
    AFX_ASSERT(in);

    // is a array copy operation (not sparsed elements)?
    if ((inStride == outStride) && ((inStride == sizeof(afxBox)) || (inStride == 0)))
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            out[i] = in[i];
        }
    }
    // is a spam operation (single to multiple elements)?
    else if (inStride == 0)
    {
        AFX_ASSERT((cnt == 1) || (outStride > sizeof(afxBox)));

        afxBox box = in[0];
        for (afxUnit i = 0; i < cnt; i++)
        {
            out[0] = box;
            out = (afxBox*)(AFX_CAST(afxByte const*, out) + outStride);
        }
    }
    else
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            out[0] = in[0];
            out = (afxBox*)(AFX_CAST(afxByte const*, out) + outStride);
            in = (afxBox const*)(AFX_CAST(afxByte const*, in) + inStride);
        }
    }
}

_AFXINL void AfxResetBoxes(afxUnit cnt, afxBox boxes[], afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT(boxes);
    AFX_ASSERT(cnt);

    // if elements are not sparsed.
    if (!stride || (stride == sizeof(afxBox)))
    {
        for (afxUnit i = 0; i < cnt; i++)
            AfxMakeAabb(&boxes[i], 0, NIL);
    }
    else
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxMakeAabb(&boxes[0], 0, NIL);
            boxes = (afxBox*)(AFX_CAST(afxByte const*, boxes) + stride);
        }
    }
}
