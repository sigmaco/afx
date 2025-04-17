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

#define _ASX_POSE_C
#define _ASX_PLACEMENT_C
#define _ASX_BODY_C
#define _ASX_MODEL_C
#define _ASX_ANIMATION_C
#define _ASX_MOTION_C
#define _ASX_MOTOR_C
#define _ASX_SIM_BRIDGE_C
#define _ASX_SIM_QUEUE_C
#define _ASX_CONTEXT_C
#include "afx/src/sim/impl/asxImplementation.h"
#include "afx/src/sim/impl/asxImpl_Context.h"

_ASX afxError _AsxSpuCmd_CopyPose(asxSpu* spu, asxCmd const* cmd)
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

_ASX afxCmdId _AsxSpuCmd_ApplyRootMotionVectors(asxSpu* spu, asxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;

    afxTransform* t = AsxGetPoseTransform(cmd->ApplyRootMotionVectors.pose, 0);
    AfxV3dAdd(t->position, t->position, cmd->ApplyRootMotionVectors.translation);

    afxQuat rot;
    AfxQuatFromAngularVelocity(rot, cmd->ApplyRootMotionVectors.rotation);
    AfxQuatMultiply(t->orientation, rot, t->orientation);

    return 0;
}

_ASX asxCmdList const _ASX_MPU_CMD_VMT =
{
    .CopyPose = _AsxSpuCmd_CopyPose,
    .ApplyRootMotionVectors = _AsxSpuCmd_ApplyRootMotionVectors,

};

_ASX afxError _AsxSpuRollContext(asxSpu* spu, afxContext ctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    AFX_ASSERT(ctx->state == asxContextState_PENDING);

    if (ctx->state != asxContextState_PENDING)
    {
        AfxThrowError();
        return err;
    }

    afxSimBridge sexu = spu->sexu;
    asxCmdList const*cmdVmt = sexu->cmdVmt;
    afxCmdId lastId = 0; // DBG

    asxCmd *cmdHdr;
    AFX_ITERATE_CHAIN_B2F(asxCmd, cmdHdr, hdr.script, &ctx->commands)
    {
        lastId = cmdHdr->hdr.id;

        if (cmdHdr->hdr.id == NIL/*ZGL_CMD_END*/)
        {
            break;
        }

        if (ctx->state != asxContextState_PENDING)
        {
            AfxThrowError();
            break;
        }

        AFX_ASSERT(cmdVmt->f[cmdHdr->hdr.id]);
        cmdVmt->f[cmdHdr->hdr.id](spu, cmdHdr);
    }

    sexu->endOfCtxCb(spu);

    if (!err)
    {
        ctx->state = asxContextState_EXECUTABLE;
    }

    AfxDecAtom32(&ctx->submCnt);

    if (err || ctx->disposable)
    {
#if 0
        //AFX_ASSERT(ctx->portId == spu->portId);
        ctx->state = asxContextState_INVALID;

        afxSimQueue sque = AfxGetProvider(ctx);
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

        //afxUnit poolIdx = ctx->poolIdx;

        AfxLockFutex(&sque->cmdbReqLock);

        if (AfxPushQueue(&sque->cmdbRecycQue, &ctx))
        {
            AfxDisposeObjects(1, &ctx);
        }

        AfxUnlockFutex(&sque->cmdbReqLock);
#endif
        AfxRecycleCatalyst(ctx, TRUE);
    }
    else
    {
        AfxDisposeObjects(1, &ctx);
    }
    return err;
}

_ASX afxError _AsxCtxResetCb(afxContext ctx, afxBool freeMem, afxBool permanent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ctx->state != asxContextState_PENDING);

    ctx->disposable = !permanent;
    AfxDeployChain(&ctx->commands, ctx);
    AfxExhaustArena(&ctx->cmdArena);
    ctx->state = asxContextState_RECORDING;

    return err;
}

_ASX afxError _AsxCtxEndCb(afxContext ctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    return err;
}
