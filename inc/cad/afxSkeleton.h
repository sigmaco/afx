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

/// O objeto afxSkeleton é uma coleção hierárquica de articulações que descrevem a estrutura articular interna de um afxModel, e auxilia no manejo e na animação do mesmo. 

/// Cada articulação no arranjo do afxSkeleton aloja o transforme para aquela articulação em dois meios diferentes. 
/// Primeiro, afxTransform "local" é o transforme decomposto da articulação relativa a seu parente imediato (dado pelo índice do parente).
/// Se esta não houver parente (isto é, o índex do parente é igual a AFX_INVALID_INDEX), então esta é relativa a origem do afxSkeleton.

/// Segundo, "iw" é uma afxM4d que é o transforme invertido no world-space para a articulação na postura padrão do afxSkeleton (isto é, a postura na qual o afxSkeleton foi originalmente modelado).

/// O afxTransform "local" é usado primariamente na composição de animações, e a afxM4d "iw" é usada primariamente para deformação de malha, razão pela qual esta informação é alojada em dois diferentes formatos.

/// Cada articulação também opcionalmente contém dados definidos pelo usuário "UDD".
/// Este campo aponta para algum dado, estranho à lógica do Qwadro, associado com a articulação, se houver qualquer.

/// Skinning is a technique for deforming geometry by linearly weighting vertices to a set of transformations, represented by nodes.
/// Nodes that affect a particular geometry are usually organized into a single hierarchy called a 'skeleton', although the influencing nodes may come from unrelated parts of the hierarchy.
/// The nodes of such a hierarchy represents the 'joints' of the skeleton, which should not be confused with the 'bones', which are the imaginary line segments connecting two joints.

#ifndef AMX_SKELETON_H
#define AMX_SKELETON_H

#include "qwadro/inc/cad/afxPose.h"
#include "qwadro/inc/cad/afxPoseBuffer.h"
#include "qwadro/inc/io/afxUri.h"
#include "qwadro/inc/math/afxMatrix.h"
#include "qwadro/inc/base/afxFixedString.h"

typedef enum afxSkeletonFlag
{
    afxSkeletonFlag_NONE
} afxSkeletonFlags;

AFX_DEFINE_STRUCT(afxSkeletonBlueprint)
{
    afxString32         urn;
    afxUnit             lodType;
    afxBool             scalable;
    afxBool             deformable;
    afxUnit             jointCnt;
    afxString const*    joints;
    //afxUnit const*      parents;
    //afxTransform const* transforms; // local
    //afxM4d const*       matrices; // inverse world
    //afxReal const*      lodErrors;
};

AMX afxBool             AfxGetSkeletonUrn(afxSkeleton skl, afxString* id);

AMX afxUnit             AfxCountSkeletonJoints(afxSkeleton skl, afxReal allowedErr);

AMX afxUnit             AfxFindSkeletonJoints(afxSkeleton skl, afxUnit cnt, afxString const ids[], afxUnit indices[]);

AMX afxString*          AfxGetSkeletonJoints(afxSkeleton skl, afxUnit jntIdx);

AMXINL afxTransform*    AfxGetSkeletonTransform(afxSkeleton skl, afxUnit jntIdx);
AMX afxM4d*             AfxGetSkeletonMatrices(afxSkeleton skl, afxUnit baseJntIdx);
AMXINL afxUnit*         AfxGetSkeletonParentIndexes(afxSkeleton skl, afxUnit baseJntIdx);

AMX afxError            AfxResetSkeletonMatrices(afxSkeleton skl, afxUnit baseJntIdx, afxUnit cnt, void const* matrices, afxUnit stride);
AMX afxError            AfxResetSkeletonLodErrors(afxSkeleton skl, afxUnit baseJntIdx, afxUnit cnt, afxReal const lodErrors[]);
AMX afxError            AfxReparentSkeletonJoints(afxSkeleton skl, afxUnit baseJntIdx, afxUnit cnt, void const* indices, afxUnit stride);
AMX afxError            AfxResetSkeletonTransforms(afxSkeleton skl, afxUnit baseJntIdx, afxUnit cnt, afxTransform const transforms[]);

AMX void                AfxQuerySkeletonErrorTolerance(afxSkeleton skl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler);

AMX void                AfxLocalPoseFromPoseBuffer(afxSkeleton skl, afxPose lp, afxPoseBuffer const wp, afxM4d const offset, afxUnit firstBone, afxUnit boneCnt);
AMX void                AfxLocalPoseFromPoseBufferNoScale(afxSkeleton skl, afxPose lp, afxPoseBuffer const wp, afxM4d const offset, afxUnit firstBone, afxUnit boneCnt);
AMX void                AfxGetWorldMatrixFromLocalPose(afxSkeleton skl, afxUnit jntIdx, afxPose const lp, afxM4d const offset, afxM4d m, afxUnit const* sparseBoneArray, afxUnit const* sparseBoneArrayReverse);
AMX void                AfxGetSkeletonAttachmentOffset(afxSkeleton skl, afxUnit jntIdx, afxPose const lp, afxM4d const offset, afxM4d m, afxUnit const* sparseArtArray, afxUnit const* sparseArtArrayReverse);

AMX void                AfxBuildIndexedCompositeBuffer(afxSkeleton skl, afxPoseBuffer const wp, afxUnit const* indices, afxUnit idxCnt, afxM4d buffer[]);
AMX void                AfxBuildIndexedCompositeBufferTransposed(afxSkeleton skl, afxPoseBuffer const wp, afxUnit const* indices, afxUnit idxCnt, afxReal buffer[][3][4]);

AMX void                AfxComputeRestLocalPose(afxSkeleton skl, afxUnit baseJnt, afxUnit jntCnt, afxPose lp);
AMX void                AfxComputeRestPoseBuffer(afxSkeleton skl, afxUnit baseJnt, afxUnit jntCnt, afxM4d const offset, afxPoseBuffer wp);

AMX void                AfxComputePoseBuffer(afxSkeleton skl, afxUnit baseJnt, afxUnit jntCnt, afxPose const lp, afxM4d const offset, afxPoseBuffer wp);
AMX void                AfxComputePoseBufferLod(afxSkeleton skl, afxUnit baseJnt, afxUnit jntCnt, afxUnit firstValidLocalArt, afxUnit validLocalArtCnt, afxPose const lp, afxM4d const offset, afxPoseBuffer wp);
AMX void                AfxComputePoseBufferNoCompositeLod(afxSkeleton skl, afxUnit baseJnt, afxUnit jntCnt, afxUnit firstValidLocalArt, afxUnit validLocalArtCnt, afxPose const lp, afxM4d const offset, afxPoseBuffer wp);

////////////////////////////////////////////////////////////////////////////////

AMX afxUnit             AfxEnumerateSkeletons(afxSimulation sim, afxUnit first, afxUnit cnt, afxSkeleton skeletons[]);

AMX void                AfxTransformSkeletons(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxUnit cnt, afxSkeleton skeletons[]);

AMX afxError            AfxAssembleSkeletons(afxSimulation sim, afxUnit cnt, afxSkeletonBlueprint const sklb[], afxSkeleton skeletons[]);

#endif//AMX_SKELETON_H
