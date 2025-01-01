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

#ifndef AMX_IMPL___BODY_H
#define AMX_IMPL___BODY_H

#include "qwadro/inc/sim/afxSimulation.h"

/*
    Body
    Animation
        Linked Motion
            Motion
            Skeleton
*/


#ifdef _AMX_BODY_C
#ifdef _AMX_BODY_IMPL
AFX_OBJECT(_amxBody)
#else
AFX_OBJECT(afxBody)
#endif
{
    afxModel            mdl;
    afxChain            motives;
    void*               userData[4];
    afxUnit32            reserved0,
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
    afxPose             pose;
    afxM4d              placement;
};
#endif//_AMX_BODY_C


AMX afxClassConfig _AMX_BOD_CLASS_CONFIG;

#endif//AMX_IMPL___BODY_H
