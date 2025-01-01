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
#define _AMX_ENGINE_C
#define _AMX_SIM_BRIDGE_C
#define _AMX_SIM_QUEUE_C
#define _AMX_CATALYST_C
#define _AMX_POSE_C
#define _AMX_PLACEMENT_C
#include "../impl/amxImplementation.h"


_AMX afxCmdId AmxCmdCopyPose(afxCatalyst cyst, afxPose src, afxUnit from, afxPose dst, afxUnit base, afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    /// cyst must be a valid afxCatalyst handle.
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    /// cyst must be in the recording state.
    AFX_ASSERT(cyst->state == amxCatalystState_RECORDING);

    AFX_ASSERT(dst);
    AFX_ASSERT(src);

    AFX_ASSERT_RANGE(src->artCnt, from, cnt);
    AFX_ASSERT_RANGE(dst->artCnt, base, cnt);

    from = AfxMin(from, src->artCnt - 1);
    base = AfxMin(from, dst->artCnt - 1);
    cnt = AfxMin(cnt, src->artCnt - from);
    cnt = AfxMin(cnt, dst->artCnt - base);

    afxCmdId cmdId;
    amxCmd* cmd = _AmxCystPushCmd(cyst, AMX_GET_STD_CMD_ID(CopyPose), sizeof(cmd->CopyPose), &cmdId);
    AFX_ASSERT(cmd);

    cmd->CopyPose.src = src;
    cmd->CopyPose.dst = dst;
    cmd->CopyPose.cnt = cnt;
    cmd->CopyPose.base = base;
    cmd->CopyPose.from = from;

    return 0;
}

_AMX afxCmdId AmxCmdApplyRootMotionVectors(afxCatalyst cyst, afxPose pose, afxV3d const translation, afxV3d const rotation)
{
    afxError err = AFX_ERR_NONE;
    /// cyst must be a valid afxCatalyst handle.
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    /// cyst must be in the recording state.
    AFX_ASSERT(cyst->state == amxCatalystState_RECORDING);

    afxCmdId cmdId;
    amxCmd* cmd = _AmxCystPushCmd(cyst, AMX_GET_STD_CMD_ID(ApplyRootMotionVectors), sizeof(cmd->ApplyRootMotionVectors), &cmdId);
    AFX_ASSERT(cmd);

    cmd->ApplyRootMotionVectors.pose = pose;
    AfxV3dCopy(cmd->ApplyRootMotionVectors.translation, translation);
    AfxV3dCopy(cmd->ApplyRootMotionVectors.rotation, rotation);

    return 0;
}

_AMX afxCmdId AmxCmdSampleBodyAnimationsLODSparse(afxCatalyst cyst, afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray)
{
    afxError err = AFX_ERR_NONE;
    /// cyst must be a valid afxCatalyst handle.
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    /// cyst must be in the recording state.
    AFX_ASSERT(cyst->state == amxCatalystState_RECORDING);

    afxCmdId cmdId;
    amxCmd* cmd = _AmxCystPushCmd(cyst, AMX_GET_STD_CMD_ID(SampleBodyAnimations), sizeof(cmd->SampleBodyAnimations), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    cmd->SampleBodyAnimations.allowedErr = allowedErr;
    cmd->SampleBodyAnimations.basePivotIdx = basePivotIdx;
    cmd->SampleBodyAnimations.bod = bod;
    cmd->SampleBodyAnimations.pivotCnt = pivotCnt;
    cmd->SampleBodyAnimations.pose = pose;
    cmd->SampleBodyAnimations.sparseBoneArray = sparseBoneArray;
}

_AMX afxCmdId AmxCmdSampleSingleBodyAnimationLODSparse(afxCatalyst cyst, afxBody bod, afxCapstan moto, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray)
{
    afxError err = AFX_ERR_NONE;
    /// cyst must be a valid afxCatalyst handle.
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    /// cyst must be in the recording state.
    AFX_ASSERT(cyst->state == amxCatalystState_RECORDING);

    afxCmdId cmdId;
    amxCmd* cmd = _AmxCystPushCmd(cyst, AMX_GET_STD_CMD_ID(SampleSingleBodyAnimation), sizeof(cmd->SampleSingleBodyAnimation), &cmdId);
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

_AMX afxCmdId AmxCmdSampleBodyAnimationsAcceleratedLOD(afxCatalyst cyst, afxBody bod, afxUnit pivotCnt, afxM4d const offset, afxPose scratch, afxPlacement plce, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    /// cyst must be a valid afxCatalyst handle.
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    /// cyst must be in the recording state.
    AFX_ASSERT(cyst->state == amxCatalystState_RECORDING);

    afxCmdId cmdId;
    amxCmd* cmd = _AmxCystPushCmd(cyst, AMX_GET_STD_CMD_ID(SampleBodyAnimationsAccelerated), sizeof(cmd->SampleBodyAnimationsAccelerated), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    AFX_ASSERT(scratch);

    cmd->SampleBodyAnimationsAccelerated.bod = bod;
    cmd->SampleBodyAnimationsAccelerated.allowedErr = allowedErr;
    AfxM4dCopy(cmd->SampleBodyAnimationsAccelerated.offset, offset ? offset : AFX_M4D_IDENTITY);
    cmd->SampleBodyAnimationsAccelerated.pivotCnt = pivotCnt;
    cmd->SampleBodyAnimationsAccelerated.plce = plce;
    cmd->SampleBodyAnimationsAccelerated.scratch = scratch;
    
    return cmdId;
}

_AMX afxCmdId AmxCmdBuildIndexedCompositeBuffer(afxCatalyst cyst, afxModel mdl, afxUnit rigIdx, afxPlacement const plce, afxUnit cnt, afxUnit bufIdx, afxUnit offset)
{
    afxError err = AFX_ERR_NONE;
    /// cyst must be a valid afxCatalyst handle.
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    /// cyst must be in the recording state.
    AFX_ASSERT(cyst->state == amxCatalystState_RECORDING);

    afxCmdId cmdId;
    amxCmd* cmd = _AmxCystPushCmd(cyst, AMX_GET_STD_CMD_ID(BuildIndexedCompositeBuffer), sizeof(cmd->BuildIndexedCompositeBuffer), &cmdId);
    AFX_ASSERT(cmd);

    afxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AfxDescribeModel(mdl, &mdli);
    AFX_ASSERT_RANGE(mdli.rigCnt, rigIdx, 1);

    AFX_ASSERT(mdli.jntCnt >= cnt);
    AFX_ASSERT(plce);
    AFX_ASSERT(plce->matCnt >= cnt);
    //AFX_ASSERT(bufIdx);

    cmd->BuildIndexedCompositeBuffer.mdl = mdl;
    cmd->BuildIndexedCompositeBuffer.plce = plce;
    cmd->BuildIndexedCompositeBuffer.rigIdx = rigIdx;
    cmd->BuildIndexedCompositeBuffer.cnt = cnt;
    cmd->BuildIndexedCompositeBuffer.bufIdx = bufIdx;
    cmd->BuildIndexedCompositeBuffer.bufOffset = offset;

    return cmdId;
}
