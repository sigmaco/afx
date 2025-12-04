/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This file is part of Acceleration for RenderWare on Qwadro.

#ifndef ARX_SKELETON_H
#define ARX_SKELETON_H

/*
    O objeto arxSkeleton é uma coleção hierárquica de articulações que descrevem a estrutura articular interna de um arxModel, e auxilia no manejo e na animação do mesmo. 

    Cada articulação no arranjo do arxSkeleton aloja o transforme para aquela articulação em dois meios diferentes. 
    Primeiro, afxTransform "local" é o transforme decomposto da articulação relativa a seu parente imediato (dado pelo índice do parente).
    Se esta não houver parente (isto é, o índex do parente é igual a AFX_INVALID_INDEX), então esta é relativa a origem do arxSkeleton.

    Segundo, "iw" é uma afxM4d que é o transforme invertido no world-space para a articulação na postura padrão do arxSkeleton (isto é, a postura na qual o arxSkeleton foi originalmente modelado).

    O afxTransform "local" é usado primariamente na composição de animações, e a afxM4d "iw" é usada primariamente para deformação de malha, razão pela qual esta informação é alojada em dois diferentes formatos.

    Cada articulação também opcionalmente contém dados definidos pelo usuário "UDD".
    Este campo aponta para algum dado, estranho à lógica do Qwadro, associado com a articulação, se houver qualquer.

    Skinning is a technique for deforming geometry by linearly weighting vertices to a set of transformations, represented by nodes.
    Nodes that affect a particular geometry are usually organized into a single hierarchy called a 'skeleton', although the influencing nodes may come from unrelated parts of the hierarchy.
    The nodes of such a hierarchy represents the 'joints' of the skeleton, which should not be confused with the 'bones', which are the imaginary line segments connecting two joints.
*/

#include "qwadro/cad/arxPose.h"
#include "qwadro/scene/arxPlacement.h"

typedef enum arxJointType
{
    // No relative motion between bodies. (welded parts)
    arxJointType_FIXED,
    // Rotation about a single axis, like a door hinge. (elbow, door)
    arxJointType_HINGE,
    // Free rotation in all directions, no translation. (shoulder, hip)
    arxJointType_BALL,
    // Movement along a single axis, no rotation. (telescopic arm, piston)
    arxJointType_SLIDER,
    // Like a fixed joint, but allows soft compliance (springy or damped motion). (soft ropes, cloth physics)
    arxJointType_SPRING,
    // Fully customizable: each axis can be locked, limited, or free. (complex mechanical systems)
    arxJointType_6DOF,
} arxJointType;

AFX_DEFINE_STRUCT(arxJointConstraint)
{
    arxJointType type;
    afxV3d anchorA; // local
    afxV3d anchorB; // local
    union
    {
        //struct { } fixed;
        struct
        {
            afxV3d axis; // Hinge axis in local space
            afxReal minAngle;
            afxReal maxAngle;
            afxReal damping;
        } hinge;
        struct
        {
            afxReal swingLimit;
            afxReal twistLimit;
        } ball;
        struct
        {
            afxV3d axis; // Translation axis
            afxReal minLimit;
            afxReal maxLimit;
            afxReal damping;
        } slider;
        struct
        {
            afxReal stiffness; // Hooke's Law constant
            afxReal damping; // Energy loss
            afxReal restLen;
        } spring;
        struct
        {
            // locked, limited or free

            afxDof flags;

            afxV3d linearLimitMin;
            afxV3d linearLimitMax;
            afxV3d angularLimitMin; // in radians
            afxV3d angularLimitMax;
            afxReal stiffness;
            afxReal damping;
        } dof;
    };
};

AFX_DEFINE_STRUCT(arxSkeletonInfo)
{
    afxUnit             boneCnt;
    afxString const*    bones;
    afxUnit const*      pi;
    afxTransform const* lt;
    afxM4d const*       iw;
    afxReal const*      le;
};

ARX afxError    ArxAcquireSkeletons
(
    arxScenario scio, 
    afxUnit cnt, 
    afxString const urns[], 
    arxSkeletonInfo const infos[], 
    arxSkeleton skeletons[]
);

ARX afxUnit     ArxEnumerateSkeletons
(
    arxScenario scio, 
    afxUnit first, 
    afxUnit cnt, 
    arxSkeleton skeletons[]
);

ARX afxBool ArxIdentifySkeletons
(
    arxScenario scio,
    afxUnit cnt,
    afxString const ids[],
    arxSkeleton skeletons[]
);

ARX void        ArxTransformSkeletons
(
    afxM3d const ltm, 
    afxM3d const iltm, 
    afxReal ltmTol, 
    afxV3d const atv, 
    afxReal atvTol, 
    afxFlags flags, 
    afxUnit cnt, 
    arxSkeleton skeletons[]
);

////////////////////////////////////////////////////////////////////////////////

ARX afxBool     ArxGetSkeletonUrn
(
    arxSkeleton skl, 
    afxString* id
);

ARX void        ArxDescribeSkeleton
(
    arxSkeleton skl, 
    arxSkeletonInfo* info
);

// SKELETON

ARX afxUnit     ArxGetSkeletonDepth
(
    arxSkeleton skl
);

ARX afxUnit     ArxGetBoneTransforms
(
    arxSkeleton skl, 
    afxUnit boneIdx, 
    afxUnit boneCnt,
    afxTransform transforms[]
);

ARX afxUnit     ArxGetBoneLodErrors
(
    arxSkeleton skl, 
    afxUnit boneIdx, 
    afxUnit boneCnt,
    afxReal lodErrors[]
);

ARX afxUnit     ArxGetBoneInversors
(
    arxSkeleton skl, 
    afxUnit boneIdx, 
    afxUnit boneCnt, 
    void* matrices, 
    afxUnit stride
);

ARX afxUnit     ArxGetBoneParents
(
    arxSkeleton skl, 
    afxUnit boneIdx, 
    afxUnit boneCnt,
    void* indices, 
    afxUnit stride
);

ARX afxError    ArxReparentBones
(
    arxSkeleton skl, 
    afxUnit boneIdx, 
    afxUnit boneCnt,
    void const* indices, 
    afxUnit stride
);

ARX afxError    ArxResetBoneTransforms
(
    arxSkeleton skl, 
    afxUnit boneIdx, 
    afxUnit boneCnt,
    afxTransform const transforms[]
);

ARX afxError    ArxResetBoneInversors
(
    arxSkeleton skl, 
    afxUnit boneIdx, 
    afxUnit boneCnt,
    void const* matrices, 
    afxUnit stride
);

ARX afxError    ArxResetBoneLodErrors
(
    arxSkeleton skl, 
    afxUnit boneIdx, 
    afxUnit boneCnt,
    afxReal const lodErrors[]
);

ARX afxUnit     ArxFindBones
(
    arxSkeleton skl, 
    afxUnit boneCnt,
    afxString const ids[], 
    afxUnit indices[]
);

#endif//ARX_SKELETON_H
