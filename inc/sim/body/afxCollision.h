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

#ifndef ASX_COLLISION_H
#define ASX_COLLISION_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/base/afxFixedString.h"


AFX_DEFINE_HANDLE(asxShape);

typedef enum asxShapeType
{
    asxShapeType_BOX,
    asxShapeType_SPHERE,
    asxShapeType_PLANE,
    asxShapeType_COMPOUND,
    asxShapeType_CONCAVE,
    asxShapeType_CONVEX,
    asxShapeType_EMPTY,
    asxShapeType_CYLINDER,
    asxShapeType_CONE,
    asxShapeType_CAPSULE,
    asxShapeType_BVH,
    asxShapeType_MESH
} asxShapeType;

AFX_DEFINE_STRUCT(asxShapeInfo)
{
    afxTransform    t;
    asxShapeType    type;
    union
    {
        afxBox      box;
        afxSphere   sph;
        afxPlane    plane;
        afxMesh     msh;
    };
};


ASX afxError AsxAcquireShapes(afxSimulation sim, afxUnit cnt, asxShapeInfo const info[], asxShape shapes[]);

ASX afxError AfxBuildMeshShape(afxMesh msh, asxShape* shape);

#endif//ASX_COLLISION_H
