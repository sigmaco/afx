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
#include "qwadro/inc/sim/body/afxCollision.h"

AFX_DEFINE_STRUCT(asxTriangle)
{
    afxV3d  v0; // Reference vertex of the triangle
    afxV3d  e1; // First edge of the triangle (v1 - v0)
    afxV3d  e2; // Second edge of the triangle (v2 - v0)
    afxV3d  nrm; // Normal of the triangle (cross(edge1, edge2))
};

typedef enum asxCollisionType
{
    asxCollisionType_LOGICAL, // capsules for fast discard
    asxCollisionType_FIRST_COLLISION
} asxCollisionType;

typedef enum asxShapeFlag
{
    asxShapeFlag_LOGICAL,
    asxShapeFlag_PHYSICAL
} asxShapeFlags;

AFX_DEFINE_STRUCT(asxShapeLod)
{
    asxShapeFlags   flags;
    afxUnit         baseSph;
    afxUnit         sphCnt;
    afxUnit         baseBox;
    afxUnit         boxCnt;
    afxUnit         baseTri;
    afxUnit         triCnt;
};

AFX_OBJECT(asxShape)
{
    asxShapeInfo    info;
    afxUnit         sphCnt;
    afxSphere*      spheres;
    /*
        The first sphere must be a sphere encapsulating all other spheres, 
        for quick discard, once it is known to be the greater one, so having not chance to having any other collision.
    */
    afxUnit         boxCnt;
    afxBox*         boxes;
    /*
        The first box must be a box encapsulating all other boxes,
        for quick discard, once it is known to be the greater one, so having not chance to having any other collision.
    */
    afxUnit         triCnt;
    asxTriangle*    tris;
    afxUnit         triGrpCnt;
    struct
    {
        afxUnit     mtlId;
    }*              triGrps;
};

// Precompute the edges, normal, and cross product for the triangle
_ASX void precompute_triangle_edges(afxUnit triCnt, afxUnit const indices[], afxV3d const vertices[], asxTriangle tris[])
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
_ASX void transform_triangle_edges_and_normal(afxM4d const m, asxTriangle const* in, asxTriangle* out)
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
_ASX afxBool intersect_triangle(asxTriangle const* triA, asxTriangle const* triB)
{
    // Calculate the dot product for the intersection test (using precomputed 'h' values)
    afxReal a = AfxV3dDot(triB->nrm, triA->e1);
    
    if (a > -AFX_EPSILON && a < AFX_EPSILON)
        return FALSE;  // The triangles are parallel or coplanar

    // Compute the vector s (v0 - u0) from the reference vertices of the two triangles
    afxV3d s;
    AfxV3dSub(s, triA->v0, triB->v0);

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

_ASX afxBool AfxTestShapeCollision2(asxShape const s1, afxM4d const m1, asxShape const s2, afxM4d const m2)
{
    // Edge-to-edge intersection test using Moller–Trumbore algorithm
    asxTriangle s1t_cache;
    asxTriangle* s1t = &s1t_cache;
    asxTriangle s2t_cache;
    asxTriangle* s2t = &s2t_cache;

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

_ASX afxBool AfxTestShapeCollision(asxShape const s1, asxShape const s2)
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
    return 0;
}

_ASX afxError _AsxShapDtorCb(asxShape shap)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHAP, 1, &shap);

    switch (shap->info.type)
    {
    case asxShapeType_MESH:
    {

        break;
    }
    default: break;
    }

    return err;
}

_ASX afxError _AsxShapCtorCb(asxShape shap, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHAP, 1, &shap);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    asxShapeInfo const* info = AFX_CAST(asxShapeInfo const*, args[1]) + invokeNo;
    
    shap->info = *info;

    // Validate zeroed transforms.
    if (!shap->info.t.flags)
        AfxResetTransform(&shap->info.t);

    if (info->type == asxShapeType_MESH)
    {
        arxMesh msh = info->msh;
        arxMeshInfo mshi;
        ArxDescribeMesh(msh, &mshi);

        afxUnit* indices = ArxGetMeshIndices(msh, 0);
        afxV3d* pos = ArxAccessVertexData(msh, 0, 0, 0);
        
        shap->triCnt = mshi.triCnt;

        AfxAllocate(mshi.triCnt * sizeof(shap->tris[0]), 0, AfxHere(), (void**)&shap->tris);
        precompute_triangle_edges(mshi.triCnt, indices, pos, shap->tris);
    }
    return err;
}

_ASX afxClassConfig const _ASX_SHAP_CLASS_CONFIG =
{
    .fcc = afxFcc_SHAP,
    .name = "Shape",
    .desc = "Collision Shape",
    .fixedSiz = sizeof(AFX_OBJECT(asxShape)),
    .ctor = (void*)_AsxShapCtorCb,
    .dtor = (void*)_AsxShapDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AsxAcquireShapes(afxSimulation sim, afxUnit cnt, asxShapeInfo const info[], asxShape shapes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_AsxGetShapClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_SHAP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)shapes, (void const*[]) { sim }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_ASX afxError AfxBuildMeshShape(arxMesh msh, asxShape* shape)
{
    afxError err = NIL;

    arxMeshInfo mshi;
    ArxDescribeMesh(msh, &mshi);

    asxShape shap;
    asxShapeInfo shapi = { 0 };
    shapi.msh = msh;
    AsxAcquireShapes(ArxGetMeshMateriality(msh), 1, &shapi, &shap);

    return err;
}
