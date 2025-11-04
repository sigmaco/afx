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

// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

#ifndef ASX_COLLISION_H
#define ASX_COLLISION_H

#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/cad/arxMesh.h"

AFX_DEFINE_HANDLE(asxShape);

typedef enum arxColliderType
{
    arxColliderType_CONVEX,
    arxColliderType_COMPOUND,
    arxColliderType_DECORATED,
    arxColliderType_MESH,
    arxColliderType_HEIGHT_FIELD,
    arxColliderType_SOFT_BODY,
    arxColliderType_USER1,
    arxColliderType_USER2,
    arxColliderType_USER3,
    arxColliderType_USER4
} arxColliderType;

typedef enum arxShapeType
{
    arxShapeType_SPHERE,
    arxShapeType_BOX,
    arxShapeType_TRIANGLE,
    arxShapeType_CAPSULE,
    arxShapeType_TAPERED_CAPSULE,
    arxShapeType_CYLINDER,
    arxShapeType_CONVEX_HULL,
    arxShapeType_STATIC_COMPOUND,
    arxShapeType_MUTABLE_COMPOUND,
    arxShapeType_ROTATED_TRANSLATED,
    arxShapeType_SCALED,
    arxShapeType_OFFSET_CENTER_OF_MASS,
    arxShapeType_MESH,
    arxShapeType_HEIGHT_FIELD,
    arxShapeType_SOFT_BODY,
    arxShapeType_USER1,
    arxShapeType_USER2,
    arxShapeType_USER3,
    arxShapeType_USER4,
    arxShapeType_USER5,
    arxShapeType_USER6,
    arxShapeType_USER7,
    arxShapeType_USER8,
    arxShapeType_USER_CONVEX1,
    arxShapeType_USER_CONVEX2,
    arxShapeType_USER_CONVEX3,
    arxShapeType_USER_CONVEX4,
    arxShapeType_USER_CONVEX5,
    arxShapeType_USER_CONVEX6,
    arxShapeType_USER_CONVEX7,
    arxShapeType_USER_CONVEX8,
} arxShapeType;

AFX_DEFINE_STRUCT(asxShapeInfo)
{
    afxTransform    t;
    arxShapeType    type;
    union
    {
        afxBox      box;
        afxSphere   sph;
        afxPlane    plane;
        arxMesh     msh;
    };
};

AFX_DEFINE_STRUCT(asxRigidBody)
{

};

AFX_DEFINE_STRUCT(asxSoftBody)
{

};

AFX_DEFINE_STRUCT(asxCollider)
// collider is the geometric shape of a collidable object.
{
    afxUnit     bsCnt;
    afxSphere*  bs;
    afxUnit     bbCnt;
    afxBox*     bb;
    afxUnit     plCnt;
    afxPlane*   pl;
    afxUnit     vtxCnt;
    afxV3d*     vtx;
    afxUnit     triCnt;
    afxUnit*    tri;
};

AFX_DEFINE_STRUCT(asxCollision)
// collision is the result of a collidable interaction.
{

};

AFX_DEFINE_STRUCT(arxShapeSupportingFace)
{
    afxFlags    flags;
    afxUnit     pntCnt;
    afxV4d      pnts[4]; // 4th element is ignored; world space
};

ASX afxError ArxGetShapeSurfaceNormal(asxCollider col, afxUnit subId, afxV3d const point, afxV3d normal);

ASX afxError ArxGetShapeSupportingFace(asxCollider col, afxUnit subId, afxV3d const dir, afxV3d const scale, afxM4d const t, arxShapeSupportingFace* face);

ASX afxError AsxAcquireShapes
(
    arxSimulation sim, 
    afxUnit cnt, 
    asxShapeInfo const info[], 
    asxShape shapes[]
);

ASX afxError ArxBuildMeshShape
(
    arxSimulation sim, 
    arxMesh msh, 
    asxShape* shape
);

#endif//ASX_COLLISION_H
