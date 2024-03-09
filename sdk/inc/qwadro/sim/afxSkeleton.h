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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_SKELETON_H
#define AFX_SKELETON_H

#include "awxPose.h"
#include "awxWorldPose.h"
#include "qwadro/io/afxUri.h"
#include "awxSimDefs.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/core/afxFixedString.h"

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

#ifdef _AFX_SKELETON_C
AFX_OBJECT(afxSkeleton)
{
    afxNat              jointCnt;
    afxNat*             parentIdx;
    afxTransform*       local;
    afxM4d*             iw;
    afxReal*            lodError;
    afxString*          label;
    void**              udd;

    afxReal             allowedLodErrFadingFactor;
    afxNat              lodType;
    afxString           id; // 32
    afxStringCatalog    strc;
};
#endif//_AFX_SKELETON_C

AFX_DEFINE_STRUCT(afxSkeletonBlueprint)
{
    afxNat              jointCnt;
    afxNat              lodType;
    afxString32         id;
    afxBool             scalable;
    afxBool             deformable;
    afxString const*    joints;
};

AFX_DEFINE_STRUCT(afxSkeletonBone)
{
    afxNat              idStrIdx;
    afxString32    id; // 32
    afxNat              parentIdx;
    afxTransform        local;
    afxM4d              iw;
    afxReal             lodError;
    void*               extendedData;
};

AFX_DEFINE_STRUCT(afxSkeletonBuilder)
{
    _AFX_DBG_FCC;
    afxNat              jointCnt;
    afxSkeletonBone*    bones;
    afxNat              lodType;
    afxString32    id;
    void                (*IndexRemapping)(void* data, afxNat jointIdx, afxNat outIdx);
};

AKX afxBool             AfxGetSkeletonId(afxSkeleton skl, afxString* id);

AKX afxNat              AfxCountSkeletonJoints(afxSkeleton skl);
AKX afxNat              AfxCountSkeletonJointsForLod(afxSkeleton skl, afxReal allowedErr);

AKX afxBool             AfxFindSkeletonJoint(afxSkeleton skl, afxString const* id, afxNat *jointIdx);
AKX afxNat              AfxFindSkeletonJoints(afxSkeleton skl, afxStringCatalog strc, afxNat cnt, afxString const ids[], afxNat indices[]);

AKX afxBool             AfxGetSkeletonJointTag(afxSkeleton skl, afxNat jointIdx, afxString* id);

AKX afxM4d*             AfxGetSkeletonIwm(afxSkeleton skl, afxNat jointIdx);

AKX void                AfxQuerySkeletonErrorTolerance(afxSkeleton skl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler);

AKX void                AfxLocalPoseFromWorldPose(afxSkeleton skl, awxPose *lp, awxWorldPose const* wp, afxM4d const offset, afxNat firstBone, afxNat boneCnt);
AKX void                AfxLocalPoseFromWorldPoseNoScale(afxSkeleton skl, awxPose* lp, awxWorldPose const* wp, afxM4d const offset, afxNat firstBone, afxNat boneCnt);
AKX void                AfxGetWorldMatrixFromLocalPose(afxSkeleton skl, afxNat jointIdx, awxPose const* lp, afxM4d const offset, afxM4d m, afxNat const* sparseBoneArray, afxNat const* sparseBoneArrayReverse);
AKX void                AfxGetSkeletonAttachmentOffset(afxSkeleton skl, afxNat jointIdx, awxPose const* lp, afxM4d const offset, afxM4d m, afxNat const* sparseArtArray, afxNat const* sparseArtArrayReverse);

AKX void                AfxBuildIndexedCompositeBuffer(afxSkeleton skl, awxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxM4d buffer[]);
AKX void                AfxBuildIndexedCompositeBufferTransposed(afxSkeleton skl, awxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxReal buffer[][3][4]);

AKX void                AfxComputeRestLocalPose(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, awxPose *lp);
AKX void                AfxComputeRestWorldPose(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, afxM4d const offset, awxWorldPose *wp);

AKX void                AfxComputeWorldPose(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, awxPose const *lp, afxM4d const offset, awxWorldPose *wp);
AKX void                AfxComputeWorldPoseLod(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, afxNat firstValidLocalArt, afxNat validLocalArtCnt, awxPose const *lp, afxM4d const offset, awxWorldPose *wp);
AKX void                AfxComputeWorldPoseNoCompositeLod(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, afxNat firstValidLocalArt, afxNat validLocalArtCnt, awxPose const *lp, afxM4d const offset, awxWorldPose *wp);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxError            AfxAcquireSkeletons(afxSimulation sim, afxStringCatalog strc, afxNat cnt, afxSkeletonBlueprint const sklb[], afxSkeleton skeletons[]);

AKX afxError            AfxBuildSkeletons(afxSimulation sim, afxStringCatalog strc, afxNat cnt, afxSkeletonBuilder const sklb[], afxSkeleton skeletons[]);

AKX void                AfxTransformSkeletons(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxNat cnt, afxSkeleton skeletons[]);

AKX afxError            AfxDeserializeSkeletons(afxStream in, afxSimulation sim, afxNat cnt, afxSkeleton skl[]);
AKX afxError            AfxSerializeSkeletons(afxStream out, afxNat cnt, afxSkeleton const skl[]);

////////////////////////////////////////////////////////////////////////////////
// BUILDING OPERATIONS                                                        //
////////////////////////////////////////////////////////////////////////////////

AKXINL afxError         AfxBeginSkeletonBuilding(afxSkeletonBuilder* sklb, afxNat boneCnt, afxString const* id, afxNat lodType);
AKXINL void             AfxEndSkeletonBuilding(afxSkeletonBuilder* sklb);

AKXINL void             AfxResetBoneWithIw(afxSkeletonBuilder const* sklb, afxNat jointIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxM4d const iw, afxReal lodError);
AKXINL void             AfxResetBone(afxSkeletonBuilder const* sklb, afxNat jointIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxTransform const* wt, afxReal lodError);

#endif//AFX_SKELETON_H
