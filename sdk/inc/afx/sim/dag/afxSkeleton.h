/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_SKELETON_H
#define AFX_SKELETON_H

#include "afx/core/io/afxUri.h"
#include "afxNode.h"
#include "afx/math/afxTransform.h"
#include "afx/core/mem/afxArray.h"

/*
// Skeleton for characters and animated objects.
// Skeleton provides a hierarchical interface for managing bones, including pose, rest and animation (see Animation).


The granny_skeleton structure is a directly accessible collection of bones that describe the internal structure of a granny_model.

Each granny_bone structure in the granny_skeleton's bone array stores the transform for that bone in two different ways.
First, LocalTransform is the decomposed transform of the bone relative to its parent bone (given by ParentIndex).
If it has no parent (its ParentIndex field is equal to GrannyNoParentBone), then it is relative to the origin.

Second, InverseWorld4x4 is a 4x4 matrix that is the inverted world-space transform for the bone in the default pose of the skeleton (ie., the pose in which the skeleton was originally modeled).

LocalTransform is used primarily in compositing animations, and InverseWorld4x4 is used primarily for mesh deformation, which is why the two are stored in different formats.

Each bone also optionally contains light or camera data.
These fields point to light and camera data associated with the bone, but are not present if the bone was neither a light nor a camera.
*/

AFX_DEFINE_HANDLE(afxSkeleton);

// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de um afxSkeleton.

AFX_DEFINE_STRUCT(afxSkeletonBlueprintBone)
{
    afxNat                      parentIdx;
    afxTransform                local;
    afxM4d                      iw;
    afxReal                     lodError;
};

AFX_DEFINE_STRUCT(afxSkeletonBlueprint)
{
    afxFcc                      fcc;
    void*                       sim;
    afxArray                    bones; // afxSkeletonBlueprintBone    
    afxNat                      lodType; // granny::skeleton_lod_type
    afxNat                      *remappingTable;
};

AFX_DEFINE_STRUCT(afxSkeletonLocalPoseTransform)
{
    afxReal                         weight;
    afxInt                          count;
    afxTransform                    transform;
    afxInt                          traversalID;
};

AFX_DEFINE_STRUCT(afxSkeletonLocalPose)
{
    afxNat                          boneCount;
    afxSkeletonLocalPoseTransform*  transforms;
    afxReal                         fillThreshold;
    afxInt                          traversalID;
};

AFX_DEFINE_STRUCT(afxSkeletonWorldPose)
{
    afxNat boneCount;
    afxReal(*worldTransformBuffer)[4][4];
    afxReal(*compositeTransformBuffer)[4][4];
};

AFX_DEFINE_STRUCT(afxSkeletonBone)
{
    afxString*          name; // 32
    afxNat              parentIdx;
    afxTransform        local;
    afxM4d              iw;
    afxReal             lodError;
    void*               extendedData;
};

AFX_OBJECT(afxSkeleton)
{
    afxObject           obj;
    afxUri*             uri; // 128
    afxNat              boneCnt;
    afxSkeletonBone     *bones; // afxNode
    afxNat              lodType;
};

AFX void*               AfxSkeletonGetSimulation(afxSkeleton skl);

AFX afxUri const*       AfxSkeletonGetUri(afxSkeleton skl);
AFX afxSkeleton         AfxSkeletonClone(afxSkeleton skl);

AFX afxNat              AfxSkeletonGetBoneCount(afxSkeleton skl);
AFX afxSkeletonBone*    AfxSkeletonGetBone(afxSkeleton skl, afxNat boneIdx);

AFX afxBool             AfxSkeletonFindBone(afxSkeleton skl, afxString const *name, afxNat *boneIdx);

AFX void                AfxSkeletonTransform(afxSkeleton skl, afxV3d const affine, afxM3d const linear, afxM3d const invLinear);

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

// Inicializa uma blueprint, e reserva recursos.
AFX void                AfxSkeletonBlueprintDeploy(afxSkeletonBlueprint *sklb, void *sim);

// Retira de uso uma blueprint, e libera recursos.
AFX void                AfxSkeletonBlueprintDiscard(afxSkeletonBlueprint *sklb);

// Adiciona um osso a um esqueleto. Ao menos um deve existir.
AFX void                AfxSkeletonBlueprintAddBoneWithIw(afxSkeletonBlueprint *sklb, afxV3d const pos, afxQuat const orientation, afxM3d const scaleShear, afxM4d const iw);
AFX void                AfxSkeletonBlueprintAddBone(afxSkeletonBlueprint *sklb, afxV3d const pos, afxQuat const orientation, afxM3d const scaleShear, afxV3d const worldPos, afxQuat const worldOrientation, afxM3d const worldScaleShear);

// Define uma tolerância a erro de LOD para um especificado osso.
AFX void                AfxSkeletonBlueprintSetBoneLodError(afxSkeletonBlueprint *sklb, afxNat idx, afxReal lodError);

// Associa um osso como filho de outro osso numa blueprint.
AFX void                AfxSkeletonBlueprintSetBoneParent(afxSkeletonBlueprint *sklb, afxNat idx, afxNat parentIdx);

AFX void                AfxSkeletonBlueprintSetRemappingTable(afxSkeletonBlueprint *sklb, afxNat *remappingTable);

#endif//AFX_SKELETON_H