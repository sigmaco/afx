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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _ASX_ENGINE_C
#define _ASX_SIM_BRIDGE_C
#define _ASX_SIM_QUEUE_C
#define _ASX_CONTEXT_C
#define _ASX_POSE_C
#define _ASX_PLACEMENT_C
#include "../impl/asxImplementation.h"


_ASX afxCmdId AsxCmdCopyPose(afxContext ctx, afxPose src, afxUnit from, afxPose dst, afxUnit base, afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    AFX_ASSERT(dst);
    AFX_ASSERT(src);

    AFX_ASSERT_RANGE(src->artCnt, from, cnt);
    AFX_ASSERT_RANGE(dst->artCnt, base, cnt);

    from = AFX_MIN(from, src->artCnt - 1);
    base = AFX_MIN(from, dst->artCnt - 1);
    cnt = AFX_MIN(cnt, src->artCnt - from);
    cnt = AFX_MIN(cnt, dst->artCnt - base);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(CopyPose), sizeof(cmd->CopyPose), &cmdId);
    AFX_ASSERT(cmd);

    cmd->CopyPose.src = src;
    cmd->CopyPose.dst = dst;
    cmd->CopyPose.cnt = cnt;
    cmd->CopyPose.base = base;
    cmd->CopyPose.from = from;

    return 0;
}

_ASX afxCmdId AsxCmdApplyRootMotionVectors(afxContext ctx, afxPose pose, afxV3d const translation, afxV3d const rotation)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(ApplyRootMotionVectors), sizeof(cmd->ApplyRootMotionVectors), &cmdId);
    AFX_ASSERT(cmd);

    cmd->ApplyRootMotionVectors.pose = pose;
    AfxV3dCopy(cmd->ApplyRootMotionVectors.translation, translation);
    AfxV3dCopy(cmd->ApplyRootMotionVectors.rotation, rotation);

    return 0;
}

_ASX afxCmdId AsxCmdSampleBodyAnimationsLODSparse(afxContext ctx, afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(SampleBodyAnimations), sizeof(cmd->SampleBodyAnimations), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    cmd->SampleBodyAnimations.allowedErr = allowedErr;
    cmd->SampleBodyAnimations.basePivotIdx = basePivotIdx;
    cmd->SampleBodyAnimations.bod = bod;
    cmd->SampleBodyAnimations.pivotCnt = pivotCnt;
    cmd->SampleBodyAnimations.pose = pose;
    cmd->SampleBodyAnimations.sparseBoneArray = sparseBoneArray;
}

_ASX afxCmdId AsxCmdSampleSingleBodyAnimationLODSparse(afxContext ctx, afxBody bod, afxCapstan moto, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(SampleSingleBodyAnimation), sizeof(cmd->SampleSingleBodyAnimation), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

    cmd->SampleSingleBodyAnimation.allowedErr = allowedErr;
    cmd->SampleSingleBodyAnimation.basePivotIdx = basePivotIdx;
    cmd->SampleSingleBodyAnimation.bod = bod;
    cmd->SampleSingleBodyAnimation.moto = moto;
    cmd->SampleSingleBodyAnimation.pivotCnt = pivotCnt;
    cmd->SampleSingleBodyAnimation.pose = pose;
    cmd->SampleSingleBodyAnimation.sparseBoneArray = sparseBoneArray;
    
    return cmdId;
}

_ASX afxCmdId AsxCmdSampleBodyAnimationsAcceleratedLOD(afxContext ctx, afxBody bod, afxUnit pivotCnt, afxM4d const displace, afxPose scratch, afxPlacement plce, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(SampleBodyAnimationsAccelerated), sizeof(cmd->SampleBodyAnimationsAccelerated), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    AFX_ASSERT(scratch);

    cmd->SampleBodyAnimationsAccelerated.bod = bod;
    cmd->SampleBodyAnimationsAccelerated.allowedErr = allowedErr;
    AfxM4dCopy(cmd->SampleBodyAnimationsAccelerated.displace, displace ? displace : AFX_M4D_IDENTITY);
    cmd->SampleBodyAnimationsAccelerated.pivotCnt = pivotCnt;
    cmd->SampleBodyAnimationsAccelerated.plce = plce;
    cmd->SampleBodyAnimationsAccelerated.scratch = scratch;
    
    return cmdId;
}

// MODEL OPS

_ASX afxCmdId AsxCmdDisplaceModel(afxContext ctx, afxModel mdl, afxTransform const* t)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(DisplaceModel), sizeof(cmd->DisplaceModel), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(t);
    //AFX_ASSERT(bufIdx);

    cmd->DisplaceModel.mdl = mdl;
    cmd->DisplaceModel.displacement = t ? *t : AFX_TRANSFORM_IDENTITY;

    return cmdId;
}

_ASX afxCmdId AsxCmdBuildRiggedMeshCompositeMatrices(afxContext ctx, afxModel mdl, afxUnit rigIdx, afxPlacement const plce, afxUnit cnt, afxUnit bufIdx, afxSize offset)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BuildRiggedMeshCompositeMatrices), sizeof(cmd->BuildRiggedMeshCompositeMatrices), &cmdId);
    AFX_ASSERT(cmd);

    afxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AfxDescribeModel(mdl, &mdli);
    AFX_ASSERT_RANGE(mdli.rigCnt, rigIdx, 1);

    AFX_ASSERT(mdli.jntCnt >= cnt);
    AFX_ASSERT(plce);
    AFX_ASSERT(plce->matCnt >= cnt);
    //AFX_ASSERT(bufIdx);

    cmd->BuildRiggedMeshCompositeMatrices.mdl = mdl;
    cmd->BuildRiggedMeshCompositeMatrices.plce = plce;
    cmd->BuildRiggedMeshCompositeMatrices.rigIdx = rigIdx;
    cmd->BuildRiggedMeshCompositeMatrices.cnt = cnt;
    cmd->BuildRiggedMeshCompositeMatrices.bufIdx = bufIdx;
    cmd->BuildRiggedMeshCompositeMatrices.bufOffset = offset;

    return cmdId;
}

// SKELETON OPS

_ASX afxCmdId AsxCmdReparentJoints(afxContext ctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit idxSiz, void const* idxData)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(ReparentJoints), sizeof(cmd->ReparentJoints) + (jntCnt * idxSiz), &cmdId);
    AFX_ASSERT(cmd);

    afxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AfxDescribeModel(mdl, &mdli);

    AFX_ASSERT_RANGE(mdli.jntCnt, baseJntIdx, jntCnt);
    AFX_ASSERT(idxData);
    AFX_ASSERT(idxSiz);

    cmd->ReparentJoints.mdl = mdl;
    cmd->ReparentJoints.baseJntIdx = baseJntIdx;
    cmd->ReparentJoints.jntCnt = jntCnt;
    cmd->ReparentJoints.idxSiz = idxSiz;
    AfxCopy(cmd->ReparentJoints.idxData, idxData, jntCnt * idxSiz);

    return cmdId;
}

_ASX afxCmdId AsxCmdResetJointTransforms(afxContext ctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxTransform const transforms[])
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(ResetJointTransforms), sizeof(cmd->ResetJointTransforms) + (jntCnt * sizeof(cmd->ResetJointTransforms.transforms[0])), &cmdId);
    AFX_ASSERT(cmd);

    afxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AfxDescribeModel(mdl, &mdli);

    AFX_ASSERT_RANGE(mdli.jntCnt, baseJntIdx, jntCnt);
    AFX_ASSERT(transforms);
    
    cmd->ResetJointTransforms.mdl = mdl;
    cmd->ResetJointTransforms.baseJntIdx = baseJntIdx;
    cmd->ResetJointTransforms.jntCnt = jntCnt;

    for (afxUnit i = 0; i < jntCnt; i++)
        AfxCopyTransform(&cmd->ResetJointTransforms.transforms[i], &transforms[i]);

    return cmdId;
}

_ASX afxCmdId AsxCmdResetJointInversors(afxContext ctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit mtxSiz, void const* matrices)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(ResetJointInversors), sizeof(cmd->ResetJointInversors) + (jntCnt * mtxSiz), &cmdId);
    AFX_ASSERT(cmd);

    afxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AfxDescribeModel(mdl, &mdli);

    AFX_ASSERT_RANGE(mdli.jntCnt, baseJntIdx, jntCnt);
    AFX_ASSERT(matrices);
    AFX_ASSERT(mtxSiz);

    cmd->ResetJointInversors.mdl = mdl;
    cmd->ResetJointInversors.baseJntIdx = baseJntIdx;
    cmd->ResetJointInversors.jntCnt = jntCnt;
    cmd->ResetJointInversors.mtxSiz = mtxSiz;
    AfxCopy(cmd->ResetJointInversors.mtxData, matrices, jntCnt * mtxSiz);

    return cmdId;
}

_ASX afxCmdId AsxCmdResetJointLodErrors(afxContext ctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxReal const lodErrors[])
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(ResetJointLodErrors), sizeof(cmd->ResetJointLodErrors) + (jntCnt * sizeof(cmd->ResetJointLodErrors.lodErrors[0])), &cmdId);
    AFX_ASSERT(cmd);

    afxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AfxDescribeModel(mdl, &mdli);

    AFX_ASSERT_RANGE(mdli.jntCnt, baseJntIdx, jntCnt);
    AFX_ASSERT(lodErrors);

    cmd->ResetJointLodErrors.mdl = mdl;
    cmd->ResetJointLodErrors.baseJntIdx = baseJntIdx;
    cmd->ResetJointLodErrors.jntCnt = jntCnt;
    AfxCopy(cmd->ResetJointLodErrors.lodErrors, lodErrors, jntCnt * sizeof(lodErrors[0]));

    return cmdId;
}

_ASX afxCmdId AsxCmdRebuildPose(afxContext ctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxM4d const displace, afxPlacement plce, afxBool rigid, afxPose pose)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(RebuildPose), sizeof(cmd->RebuildPose), &cmdId);
    AFX_ASSERT(cmd);

    afxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AfxDescribeModel(mdl, &mdli);

    AFX_ASSERT_RANGE(mdli.jntCnt, baseJntIdx, jntCnt);
    AFX_ASSERT(plce);
    AFX_ASSERT(plce->matCnt >= jntCnt);
    AFX_ASSERT(pose);
    AFX_ASSERT(pose->artCnt >= jntCnt);
    //AFX_ASSERT(bufIdx);

    cmd->RebuildPose.mdl = mdl;
    cmd->RebuildPose.plce = plce;
    cmd->RebuildPose.baseJntIdx = baseJntIdx;
    cmd->RebuildPose.jntCnt = jntCnt;
    cmd->RebuildPose.rigid = rigid;
    cmd->RebuildPose.pose = pose;

    if (displace)
        AfxM4dCopy(cmd->RebuildPose.displace, displace);
    else
        AfxM4dReset(cmd->RebuildPose.displace);

    return cmdId;
}

_ASX afxCmdId AsxCmdBuildRestPose(afxContext ctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxPose pose)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BuildRestPose), sizeof(cmd->BuildRestPose), &cmdId);
    AFX_ASSERT(cmd);

    afxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AfxDescribeModel(mdl, &mdli);

    AFX_ASSERT_RANGE(mdli.jntCnt, baseJntIdx, jntCnt);
    AFX_ASSERT(pose);

    cmd->BuildRestPose.mdl = mdl;
    cmd->BuildRestPose.baseJntIdx = baseJntIdx;
    cmd->BuildRestPose.jntCnt = jntCnt;
    cmd->BuildRestPose.pose = pose;
    
    return cmdId;
}

_ASX afxCmdId AsxCmdBuildPlacement(afxContext ctx, afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit baseReqJnt, afxUnit reqJntCnt, afxPose pose, afxM4d const displace, afxBool noComposite, afxPlacement plce)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BuildPlacement), sizeof(cmd->BuildPlacement), &cmdId);
    AFX_ASSERT(cmd);

    afxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AfxDescribeModel(mdl, &mdli);

    AFX_ASSERT_RANGE(mdli.jntCnt, baseJntIdx, jntCnt);
    AFX_ASSERT(pose);
    AFX_ASSERT(plce);

    cmd->BuildPlacement.mdl = mdl;
    cmd->BuildPlacement.baseJntIdx = baseJntIdx;
    cmd->BuildPlacement.jntCnt = jntCnt;
    cmd->BuildPlacement.baseReqJnt = baseReqJnt;
    cmd->BuildPlacement.reqJntCnt = reqJntCnt;
    cmd->BuildPlacement.pose = pose;
    cmd->BuildPlacement.noComposite = noComposite;
    cmd->BuildPlacement.plce = plce;

    if (displace)
        AfxM4dCopy(cmd->BuildPlacement.displace, displace);
    else
        AfxM4dReset(cmd->BuildPlacement.displace);

    return cmdId;
}

_ASX afxCmdId AsxCmdBuildCompositeMatrices(afxContext ctx, afxModel mdl, afxPlacement plce, afxUnit baseJnt, afxUnit jntCnt, afxBool /*3x4*/transposed, afxUnit bufIdx, afxSize offset)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BuildCompositeMatrices), sizeof(cmd->BuildCompositeMatrices), &cmdId);
    AFX_ASSERT(cmd);

    afxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AfxDescribeModel(mdl, &mdli);

    AFX_ASSERT_RANGE(mdli.jntCnt, baseJnt, jntCnt);
    AFX_ASSERT(plce);

    cmd->BuildCompositeMatrices.mdl = mdl;
    cmd->BuildCompositeMatrices.plce = plce;
    cmd->BuildCompositeMatrices.baseJnt = baseJnt;
    cmd->BuildCompositeMatrices.jntCnt = jntCnt;
    cmd->BuildCompositeMatrices.transposed = transposed;
    cmd->BuildCompositeMatrices.bufIdx = bufIdx;
    cmd->BuildCompositeMatrices.offset = offset;

    return cmdId;
}

_ASX afxCmdId AsxCmdBuildIndexedCompositeMatrices(afxContext ctx, afxModel mdl, afxPlacement plce, afxUnit jntCnt, afxUnit const jntMap[], afxBool /*3x4*/transposed, afxUnit bufIdx, afxSize offset)
{
    afxError err = AFX_ERR_NONE;
    // ctx must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BuildIndexedCompositeMatrices), sizeof(cmd->BuildIndexedCompositeMatrices) + (jntCnt * sizeof(cmd->BuildIndexedCompositeMatrices.jntMap[0])), &cmdId);
    AFX_ASSERT(cmd);

    afxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AfxDescribeModel(mdl, &mdli);

    AFX_ASSERT_RANGE(mdli.jntCnt, 0, jntCnt);
    AFX_ASSERT(plce);

    cmd->BuildIndexedCompositeMatrices.mdl = mdl;
    cmd->BuildIndexedCompositeMatrices.plce = plce;
    cmd->BuildIndexedCompositeMatrices.jntCnt = jntCnt;
    cmd->BuildIndexedCompositeMatrices.transposed = transposed;
    cmd->BuildIndexedCompositeMatrices.bufIdx = bufIdx;
    cmd->BuildIndexedCompositeMatrices.offset = offset;

    for (afxUnit i = 0; i < jntCnt; i++)
        cmd->BuildIndexedCompositeMatrices.jntMap[i] = jntMap[i];

    return cmdId;
}
