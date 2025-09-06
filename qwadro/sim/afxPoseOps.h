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

#include "qwadro/cad/arxPose.h"

ASX afxCmdId AsxCmdCopyPose(afxContext ctx, arxPose src, afxUnit from, arxPose dst, afxUnit base, afxUnit cnt);

ASX afxCmdId AsxCmdApplyRootMotionVectors(afxContext ctx, arxPose pose, afxV3d const translation, afxV3d const rotation);

ASX afxCmdId AsxCmdSampleBodyAnimationsAcceleratedLOD(afxContext ctx, arxBody bod, afxUnit pivotCnt, afxM4d const offset, arxPose scratch, arxPlacement plce, afxReal allowedErr);
ASX afxCmdId AsxCmdSampleSingleBodyAnimationLODSparse(afxContext ctx, arxBody bod, arxCapstan moto, afxUnit basePivotIdx, afxUnit pivotCnt, arxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray);
ASX afxCmdId AsxCmdSampleBodyAnimationsLODSparse(afxContext ctx, arxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, arxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray);

// MODEL OPS

ASX afxCmdId    AsxCmdDisplaceModel(afxContext sctx, arxModel mdl, afxTransform const* t);

ASX afxCmdId    AsxCmdBuildRiggedMeshCompositeMatrices(afxContext sctx, arxModel mdl, afxUnit rigIdx, arxPlacement const plce, afxUnit cnt, afxUnit bufIdx, afxSize offset);

// SKELETON OPS

ASX afxCmdId    AsxCmdReparentJoints(afxContext sctx, arxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit idxSiz, void const* idxData);
ASX afxCmdId    AsxCmdResetJointTransforms(afxContext sctx, arxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxTransform const transforms[]);
ASX afxCmdId    AsxCmdResetJointInversors(afxContext sctx, arxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit mtxSiz, void const* matrices);
ASX afxCmdId    AsxCmdResetJointLodErrors(afxContext sctx, arxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxReal const lodErrors[]);

ASX afxCmdId    AsxCmdBuildRestPose(afxContext sctx, arxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, arxPose pose);
ASX afxCmdId    AsxCmdBuildPlacement(afxContext sctx, arxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit baseReqJnt, afxUnit reqJntCnt, arxPose pose, afxM4d const displace, afxBool noComposite, arxPlacement plce);
ASX afxCmdId    AsxCmdRebuildPose(afxContext sctx, arxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxM4d const displace, arxPlacement plce, afxBool rigid, arxPose pose);

ASX afxCmdId    AsxCmdBuildCompositeMatrices(afxContext sctx, arxModel mdl, arxPlacement plce, afxUnit baseJnt, afxUnit jntCnt, afxBool /*3x4*/transposed, afxUnit bufIdx, afxSize offset);
ASX afxCmdId    AsxCmdBuildIndexedCompositeMatrices(afxContext sctx, arxModel mdl, arxPlacement plce, afxUnit jntCnt, afxUnit const jntMap[], afxBool /*3x4*/transposed, afxUnit bufIdx, afxSize offset);

#endif//ASX_POSE_OPS_H
