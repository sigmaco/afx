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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef ARX_IMPL___BODY_H
#define ARX_IMPL___BODY_H

#include "qwadro/inc/render/arxRenderware.h"

/*
    Body
    Animation
        Linked Motion
            Motion
            Skeleton
*/

AFX_DEFINE_STRUCT(arxBodyPart)
{
    // based on RpAtomic
    // one per mesh rigged to model
    
    // In RW, RpAtomic must be individual because it is bound directly to the DAG as a generic discrete object.
    afxNodular          nodr;
    
    // In RW, there is one RpAtomic per mesh, which is a dynamic object storing the state of a RpGeometry.
    // RpGeometry would be a arxMesh. All RpAtomic's collectively would be a arxBody.
    afxUnit             rigIdx;

    // based on RpInterpolator
    arxMeshMorphing     morph;

    // In RW, there are two bounding spheres for interpolated geometry, one in object space and one in world space.
    afxBox              bb; // can change when morphing.
    afxSphere           bs; // can change when morphing.

    afxFlags            flags;
    afxMask             mask;

    // In RW, RpAtomic has a RwResEntry caching the dynamic state of a RpAtomic; RpGeometry has its own for static data.
    arxRenderableCache  cache;


    /*
        RpAtomic have a render callback, a render frame counter, and a render pipeline.

        RpAtomic have a link to only one RpClump nesting it in a group of related RpAtomic's, RpLight's and RwCamera's.
        As RpClump chains RpAtomic, RpLight and RwCamera, which are DAG entities, RpClump is considered a dynamic entity.
        RpClump is also used for providing callback function for frustum culling.
    */
};

#ifdef _ARX_BODY_C
#ifdef _ARX_BODY_IMPL
AFX_OBJECT(_arxBody)
#else
AFX_OBJECT(arxBody)
#endif
{
    arxModel            mdl;
    afxUnit             partCnt;
    arxBodyPart*        parts;
    afxChain            motives;
    void*               userData[4];
    afxUnit32           reserved0,
                        reserved1;


    struct RigidBody
    {
        // Constant quantities
        afxReal mass; // mass M
        afxM3d Ibody; // Ibody --- inertia tensor of block
        afxM3d Ibodyinv; // I-1 body (inverse of Ibody)
        // State variables
        afxV3d x; // x(t) --- centre of mass (world space) position
        afxQuat q; // q(t) --- orientation
        
        afxV3d linearMom; // P(t) ---  total linear momentum
        afxV3d angularMom; // L(t) ---  angular momentum
        // Derived quantities (auxiliary variables)
        afxM3d Iinv; // I-1(t)
        afxM3d R; // R(t)
        
        afxV3d v; // v(t) --- linear velocity
        afxV3d omega; // w(t) --- angular velocity
        // Computed quantities
        afxV3d force; // F(t)
        afxV3d torque; // t(t)


    } rigid;
#if 0
    afxM4d matrix;
    afxM4d collWorldMtx;
    afxM4d invWorldInertiaMtx;
    afxQuat rot;

    afxV3d veloc;
    afxV3d omega;
    afxV3d accel;
    afxV3d alpha;
    afxV3d netForce;
    afxV3d netTorque;
    afxV3d prevExtlForce;
    afxV3d prevExtlTorque;

    afxV3d mass;
    afxV3d invMass;
    afxV3d aparentMass;
    afxV3d localCentreOfMass;
    afxV3d globalCentreOfMass;
    afxBox aabb;
    afxV3d dampCoef;

    int freeze : 1;
    int sleeping : 1;
    int autoSleep : 1;
    int isInWorld : 1;
    int equilibrium : 1;
    int continueCollisionMode : 1;
    int spawnnedFromCallback : 1;
    int collideWithLinkedBodies : 1;
    int solverInContinueCollision : 1;
    int inCallback : 1;
#endif
    arxPose             pose;
    arxPlacement        placement;
};
#endif//_ARX_BODY_C


ARX afxClassConfig _ARX_BOD_CLASS_CONFIG;
ARX afxClassConfig const _ARX_MOTV_CLASS_CONFIG;
ARX afxClassConfig const _ARX_ANIK_CLASS_CONFIG;

#endif//ARX_IMPL___BODY_H
