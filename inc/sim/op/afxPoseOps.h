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

ASX afxCmdId AmxCmdCopyPose(afxContext ctx, afxPose src, afxUnit from, afxPose dst, afxUnit base, afxUnit cnt);

ASX afxCmdId AmxCmdApplyRootMotionVectors(afxContext ctx, afxPose pose, afxV3d const translation, afxV3d const rotation);

ASX afxCmdId AsxCmdSampleBodyAnimationsAcceleratedLOD(afxContext ctx, afxBody bod, afxUnit pivotCnt, afxM4d const offset, afxPose scratch, afxPlacement plce, afxReal allowedErr);
ASX afxCmdId AmxCmdSampleSingleBodyAnimationLODSparse(afxContext ctx, afxBody bod, afxCapstan moto, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray);
ASX afxCmdId AmxCmdSampleBodyAnimationsLODSparse(afxContext ctx, afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray);

// MODEL OPS

ASX afxCmdId    AmxCmdDisplaceModel(afxContext mctx, afxModel mdl, afxTransform const* t);

ASX afxCmdId    AmxCmdBuildRiggedMeshCompositeMatrices(afxContext mctx, afxModel mdl, afxUnit rigIdx, afxPlacement const plce, afxUnit cnt, afxUnit bufIdx, afxSize offset);

// SKELETON OPS

ASX afxCmdId    AmxCmdReparentJoints(afxContext mctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit idxSiz, void const* idxData);
ASX afxCmdId    AmxCmdResetJointTransforms(afxContext mctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxTransform const transforms[]);
ASX afxCmdId    AmxCmdResetJointInversors(afxContext mctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit mtxSiz, void const* matrices);
ASX afxCmdId    AmxCmdResetJointLodErrors(afxContext mctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxReal const lodErrors[]);

ASX afxCmdId    AmxCmdBuildRestPose(afxContext mctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxPose pose);
ASX afxCmdId    AmxCmdBuildPlacement(afxContext mctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit baseReqJnt, afxUnit reqJntCnt, afxPose pose, afxM4d const displace, afxBool noComposite, afxPlacement plce);
ASX afxCmdId    AmxCmdRebuildPose(afxContext mctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxM4d const displace, afxPlacement plce, afxBool rigid, afxPose pose);

ASX afxCmdId    AsxCmdBuildCompositeMatrices(afxContext mctx, afxModel mdl, afxPlacement plce, afxUnit baseJnt, afxUnit jntCnt, afxBool /*3x4*/transposed, afxUnit bufIdx, afxSize offset);
ASX afxCmdId    AmxCmdBuildIndexedCompositeMatrices(afxContext mctx, afxModel mdl, afxPlacement plce, afxUnit jntCnt, afxUnit const jntMap[], afxBool /*3x4*/transposed, afxUnit bufIdx, afxSize offset);

#endif//ASX_POSE_OPS_H
