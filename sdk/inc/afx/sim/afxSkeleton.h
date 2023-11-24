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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SKELETON_H
#define AFX_SKELETON_H

#include "afxPose.h"
#include "afxWorldPose.h"
#include "afx/core/afxUri.h"
#include "afxSimDefs.h"
#include "afx/math/afxMatrix.h"

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

AFX_DEFINE_STRUCT(afxSkeletonBone)
{
    afxString           id; // 32
    afxNat              parentIdx;
    afxTransform        local;
    afxM4d              iw;
    afxReal             lodError;
    void*               extendedData;
};

#ifdef _AFX_SKELETON_C
AFX_OBJECT(afxSkeleton)
{
    afxNat              boneCnt;
    afxSkeletonBone*    bones; // afxNode
    afxNat              lodType;
    afxString           id; // 128
};
#endif//_AFX_SKELETON_C

AFX_DEFINE_STRUCT(afxSkeletonBuilder)
{
    _AFX_DBG_FCC;
    afxNat              boneCnt;
    afxSkeletonBone*    bones;
    afxNat              lodType;
    afxString32         id;
    void                (*IndexRemapping)(void* data, afxNat boneIdx, afxNat outIdx);
};

AFX afxString const*    AfxGetSkeletonId(afxSkeleton skl);

AFX afxNat              AfxCountBones(afxSkeleton skl);
AFX afxNat              AfxCountBonesForLod(afxSkeleton skl, afxReal allowedErr);
AFX afxSkeletonBone*    AfxGetBone(afxSkeleton skl, afxNat boneIdx);
AFX afxBool             AfxFindBone(afxSkeleton skl, afxString const* id, afxNat *boneIdx);

AFX void                AfxComputeSkeletonRestLocalPose(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxPose *lp);
AFX void                AfxComputeSkeletonRestWorldPose(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxReal const offset[4][4], afxWorldPose *wp);

AFX void                AfxComputeSkeletonPose(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxPose const *lp, afxReal const offset[4][4], afxWorldPose *wp);

AFX void                AfxLocalPoseFromWorldPoseNoScale(afxSkeleton skl, afxPose* lp, afxWorldPose const* wp, afxReal const offset[4][4], afxNat firstBone, afxNat boneCnt);
AFX void                AfxGetWorldMatrixFromLocalPose(afxSkeleton skl, afxNat artIdx, afxPose const* lp, afxReal const offset[4][4], afxReal m[4][4], afxNat const* sparseBoneArray, afxNat const* sparseBoneArrayReverse);
AFX void                AfxGetAttachmentOffset(afxSkeleton skl, afxNat artIdx, afxPose const* lp, afxReal const offset[4][4], afxReal m[4][4], afxNat const* sparseArtArray, afxNat const* sparseArtArrayReverse);

AFX void AfxBuildIndexedCompositeBuffer(afxSkeleton skl, afxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxReal buffer[][4][4]);
AFX void AfxBuildIndexedCompositeBufferTransposed(afxSkeleton skl, afxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxReal buffer[][3][4]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AfxBuildSkeletons(afxSimulation sim, afxNat cnt, afxSkeletonBuilder const sklb[], afxSkeleton skl[]);

AFX void                AfxTransformSkeletons(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxNat cnt, afxSkeleton skl[]);

////////////////////////////////////////////////////////////////////////////////
// BUILDING OPERATIONS                                                        //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxError         AfxBeginSkeletonBuilding(afxSkeletonBuilder* sklb, afxNat boneCnt, afxString const* id, afxNat lodType);
AFXINL void             AfxEndSkeletonBuilding(afxSkeletonBuilder* sklb);

AFXINL void             AfxResetBoneWithIw(afxSkeletonBuilder const* sklb, afxNat boneIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxReal const iw[4][4]);
AFXINL void             AfxResetBone(afxSkeletonBuilder const* sklb, afxNat boneIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxTransform const* wt);

#endif//AFX_SKELETON_H
