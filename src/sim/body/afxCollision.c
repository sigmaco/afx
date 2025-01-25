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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#include "../impl/asxImplementation.h"

AFX_DEFINE_STRUCT(afxTriangle)
{
    afxV3d  v0; // Reference vertex of the triangle
    afxV3d  e1; // First edge of the triangle (v1 - v0)
    afxV3d  e2; // Second edge of the triangle (v2 - v0)
    afxV3d  nrm; // Normal of the triangle (cross(edge1, edge2))
};

AFX_DEFINE_STRUCT(afxShape)
{
    afxTriangle*    tris;
    afxUnit         triCnt;
};

// Precompute the edges, normal, and cross product for the triangle
_ASX void precompute_triangle_edges(afxUnit triCnt, afxUnit const indices[], afxV3d const vertices[], afxTriangle tris[])
{
    for (afxUnit i = 0; i < triCnt; i++)
    {
        AfxV3dCopy(tris[i].v0, vertices[indices[3 * i]]); // Store reference vertex v0 of the triangle
        AfxV3dSub(tris[i].e1, vertices[indices[3 * i + 1]], tris[i].v0); // First edge of the triangle (v1 - v0)
        AfxV3dSub(tris[i].e2, vertices[indices[3 * i + 2]], tris[i].v0); // Second edge of the triangle (v2 - v0)
        AfxV3dCross(tris[i].nrm, tris[i].e1, tris[i].e2);  // The normal of the triangle
    }
}

// Apply the transformation matrix to the edges and normal of a triangle
// Once this struct store differential values,
// it will just work it if all triangle vertices are rigidly transformed.
_ASX void transform_triangle_edges_and_normal(afxM4d const m, afxTriangle const* in, afxTriangle* out)
{
    // Transform the reference vertex v0
    
    AfxM4dPostMultiplyAtv3d(m, 1, &in->v0, &out->v0);
    
    // Transform the edges (edge1, edge2)
    AfxM4dPostMultiplyAtv3d(m, 1, &in->e1, &out->e1);
    AfxM4dPostMultiplyAtv3d(m, 1, &in->e2, &out->e2);

    // Recompute the normal (cross product) after transforming edges
    AfxV3dCross(out->nrm, in->e1, in->e2);
}

// The Moller–Trumbore ray-triangle intersection algorithm (optimized with precomputed edges)
_ASX afxBool intersect_triangle(afxTriangle const* triA, afxTriangle const* triB)
{
    // Compute the vector s (v0 - u0) from the reference vertices of the two triangles
    afxV3d s;
    AfxV3dSub(s, triA->v0, triB->v0);

    // Calculate the dot product for the intersection test (using precomputed 'h' values)
    afxReal a = AfxV3dDot(triB->nrm, triA->e1);
    
    if (a > -AFX_EPSILON && a < AFX_EPSILON)
        return FALSE;  // The triangles are parallel or coplanar

    afxReal f = 1.0f / a;
    afxReal u = f * AfxV3dDot(s, triB->nrm);
    
    if (u < 0.0f || u > 1.0f)
        return FALSE;  // No intersection

    afxV3d q;
    AfxV3dCross(q, s, triA->e1);
    afxReal v = f * AfxV3dDot(triB->nrm, q);
    
    if (v < 0.0f || u + v > 1.0f)
        return FALSE;  // No intersection

    afxReal t = f * AfxV3dDot(triA->e2, q);
    return t > AFX_EPSILON;  // If t is positive, intersection is valid
}

_ASX afxBool AfxTestShapeCollision2(afxShape const* s1, afxM4d const m1, afxShape const* s2, afxM4d const m2)
{
    // Edge-to-edge intersection test using Moller–Trumbore algorithm
    afxTriangle s1t_cache;
    afxTriangle* s1t = &s1t_cache;
    afxTriangle s2t_cache;
    afxTriangle* s2t = &s2t_cache;

    for (afxUnit i = 0; i < s1->triCnt; i++)
    {
        if (m1)
            transform_triangle_edges_and_normal(m1, &s1->tris[i], s1t);

        for (afxUnit j = 0; j < s2->triCnt; j++)
        {
            if (m2)
                transform_triangle_edges_and_normal(m2, &s2->tris[j], s2t);

            afxReal rslt = intersect_triangle(s1t, s2t);

            if (rslt > AFX_EPSILON)
                return TRUE;
        }
    }
    return 0;
}

_ASX afxBool AfxTestShapeCollision(afxShape const* s1, afxShape const* s2)
{
    // Edge-to-edge intersection test using Moller–Trumbore algorithm

    for (afxUnit i = 0; i < s1->triCnt; i++)
    {
        for (afxUnit j = 0; j < s2->triCnt; j++)
        {
            afxReal rslt = intersect_triangle(&s1->tris[i], &s2->tris[j]);

            if (rslt > AFX_EPSILON)
                return TRUE;
        }
    }
}

_ASX afxError AfxBuildMeshShape(afxMesh msh, afxShape* shape)
{
    afxError err = NIL;

    afxMeshInfo mshi;
    AfxDescribeMesh(msh, &mshi);

    AfxAllocate(mshi.triCnt, 0, AfxHere(), (void**)&shape);
    AfxAllocate(mshi.triCnt * sizeof(shape->tris[0]), 0, AfxHere(), (void**)&shape->tris);

    // Function to compute the edges and cross product of a triangle
    {
        afxUnit* indices = AfxGetMeshIndices(msh, 0);
        afxV3d* pos = AfxAccessVertexData(msh, 0, 0, 0);
        shape->triCnt = mshi.triCnt;
        precompute_triangle_edges(mshi.triCnt, indices, pos, shape->tris);
    }
    return err;
}
