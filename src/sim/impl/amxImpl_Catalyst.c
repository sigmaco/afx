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

#define _AMX_POSE_C
#define _AMX_PLACEMENT_C
#define _AMX_BODY_C
#define _AMX_MODEL_C
#define _AMX_ANIMATION_C
#define _AMX_MOTION_C
#define _AMX_MOTOR_C
#define _AMX_SIM_BRIDGE_C
#define _AMX_SIM_QUEUE_C
#define _AMX_CATALYST_C
#include "afx/src/sim/impl/amxImplementation.h"
#include "afx/src/sim/impl/amxImpl_Catalyst.h"

_AMX afxError _AmxMpuCmd_CopyPose(amxMpu* mpu, amxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;

    afxUnit cnt = cmd->CopyPose.cnt;
    afxUnit base = cmd->CopyPose.base;
    afxUnit from = cmd->CopyPose.from;
    afxPose dst = cmd->CopyPose.dst;
    afxPose src = cmd->CopyPose.src;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit toIdx = base + i;
        afxUnit fromIdx = from + i;

        dst->arts[toIdx].weight = src->arts[fromIdx].weight;
        dst->arts[toIdx].cnt = src->arts[fromIdx].cnt;
        dst->arts[toIdx].traversalId = src->arts[fromIdx].traversalId;
        AfxCopyTransform(&dst->arts[toIdx].xform, &src->arts[fromIdx].xform);
    }

    return err;
}

_AMX afxCmdId _AmxMpuCmd_ApplyRootMotionVectors(amxMpu* mpu, amxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;

    afxTransform* t = AfxGetPoseTransform(cmd->ApplyRootMotionVectors.pose, 0);
    AfxV3dAdd(t->position, t->position, cmd->ApplyRootMotionVectors.translation);

    afxQuat rot;
    AfxQuatFromAngularVelocity(rot, cmd->ApplyRootMotionVectors.rotation);
    AfxQuatMultiply(t->orientation, rot, t->orientation);

    return 0;
}

_AMX amxCmdList const _AMX_MPU_CMD_VMT =
{
    .CopyPose = _AmxMpuCmd_CopyPose,
    .ApplyRootMotionVectors = _AmxMpuCmd_ApplyRootMotionVectors,

};

_AMX afxError _AmxMpuRollContext(amxMpu* mpu, afxCatalyst cyst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    AFX_ASSERT(cyst->state == amxCatalystState_PENDING);

    if (cyst->state != amxCatalystState_PENDING)
    {
        AfxThrowError();
        return err;
    }

    afxSimBridge mexu = mpu->mexu;
    amxCmdList const*cmdVmt = mexu->cmdVmt;
    afxCmdId lastId = 0; // DBG

    amxCmd *cmdHdr;
    AfxChainForEveryLinkageB2F(&cyst->commands, amxCmd, hdr.script, cmdHdr)
    {
        lastId = cmdHdr->hdr.id;

        if (cmdHdr->hdr.id == NIL/*ZGL_CMD_END*/)
        {
            break;
        }

        if (cyst->state != amxCatalystState_PENDING)
        {
            AfxThrowError();
            break;
        }

        AFX_ASSERT(cmdVmt->f[cmdHdr->hdr.id]);
        cmdVmt->f[cmdHdr->hdr.id](mpu, cmdHdr);
    }

    mexu->endOfCtxCb(mpu);

    if (!err)
    {
        cyst->state = amxCatalystState_EXECUTABLE;
    }

    AfxDecAtom32(&cyst->submCnt);

    if (err || cyst->disposable)
    {
#if 0
        //AFX_ASSERT(cyst->portId == mpu->portId);
        cyst->state = amxCatalystState_INVALID;

        afxSimQueue mque = AfxGetProvider(cyst);
        AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

        //afxUnit poolIdx = cyst->poolIdx;

        AfxEnterSlockExclusive(&mque->cmdbReqLock);

        if (AfxPushQueue(&mque->cmdbRecycQue, &cyst))
        {
            AfxDisposeObjects(1, &cyst);
        }

        AfxExitSlockExclusive(&mque->cmdbReqLock);
#endif
        AfxRecycleCatalyst(cyst, TRUE);
    }
    else
    {
        AfxDisposeObjects(1, &cyst);
    }
    return err;
}

_AMX afxError _AmxCystResetCb(afxCatalyst cyst, afxBool freeMem, afxBool permanent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cyst->state != amxCatalystState_PENDING);

    cyst->disposable = !permanent;
    AfxDeployChain(&cyst->commands, cyst);
    AfxExhaustArena(&cyst->cmdArena);
    cyst->state = amxCatalystState_RECORDING;

    return err;
}

_AMX afxError _AmxCystEndCb(afxCatalyst cyst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &cyst);
    return err;
}
