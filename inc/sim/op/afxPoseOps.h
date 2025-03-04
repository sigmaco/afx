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

#ifndef ASX_POSE_OPS_H
#define ASX_POSE_OPS_H

#include "qwadro/inc/sim/body/afxPose.h"

ASX afxCmdId AsxCmdCopyPose(afxContext ctx, afxPose src, afxUnit from, afxPose dst, afxUnit base, afxUnit cnt);

ASX afxCmdId AsxCmdApplyRootMotionVectors(afxContext ctx, afxPose pose, afxV3d const translation, afxV3d const rotation);

ASX afxCmdId AsxCmdSampleBodyAnimationsAcceleratedLOD(afxContext ctx, afxBody bod, afxUnit pivotCnt, afxM4d const offset, afxPose scratch, afxPlacement plce, afxReal allowedErr);
ASX afxCmdId AsxCmdSampleSingleBodyAnimationLODSparse(afxContext ctx, afxBody bod, afxCapstan moto, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray);
ASX afxCmdId AsxCmdSampleBodyAnimationsLODSparse(afxContext ctx, afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray);

// MODEL OPS

ASX afxCmdId    AsxCmdDisplaceModel(afxContext sctx, afxModel mdl, afxTransform const* t);

ASX afxCmdId    AsxCmdBuildRiggedMeshCompositeMatrices(afxContext sctx, afxModel mdl, afxUnit rigIdx, afxPlacement const plce, afxUnit cnt, afxUnit bufIdx, afxSize offset);

// SKELETON OPS

ASX afxCmdId    AsxCmdReparentJoints(afxContext sctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit idxSiz, void const* idxData);
ASX afxCmdId    AsxCmdResetJointTransforms(afxContext sctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxTransform const transforms[]);
ASX afxCmdId    AsxCmdResetJointInversors(afxContext sctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit mtxSiz, void const* matrices);
ASX afxCmdId    AsxCmdResetJointLodErrors(afxContext sctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxReal const lodErrors[]);

ASX afxCmdId    AsxCmdBuildRestPose(afxContext sctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxPose pose);
ASX afxCmdId    AsxCmdBuildPlacement(afxContext sctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit baseReqJnt, afxUnit reqJntCnt, afxPose pose, afxM4d const displace, afxBool noComposite, afxPlacement plce);
ASX afxCmdId    AsxCmdRebuildPose(afxContext sctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxM4d const displace, afxPlacement plce, afxBool rigid, afxPose pose);

ASX afxCmdId    AsxCmdBuildCompositeMatrices(afxContext sctx, afxModel mdl, afxPlacement plce, afxUnit baseJnt, afxUnit jntCnt, afxBool /*3x4*/transposed, afxUnit bufIdx, afxSize offset);
ASX afxCmdId    AsxCmdBuildIndexedCompositeMatrices(afxContext sctx, afxModel mdl, afxPlacement plce, afxUnit jntCnt, afxUnit const jntMap[], afxBool /*3x4*/transposed, afxUnit bufIdx, afxSize offset);

#endif//ASX_POSE_OPS_H
