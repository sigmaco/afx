/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_INPUT_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_CONTEXT_C
#include "../impl/avxImplementation.h"

_AVX avxCmd* _AvxDctxPushCmd(afxDrawContext dctx, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    avxCmd* cmd = AfxRequestArenaUnit(&dctx->cmdArena, siz);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &dctx->commands);
    AFX_ASSERT(id != 268895528);
    return cmd;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxCmdId AvxCmdPushDebugScope(afxDrawContext dctx, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(PushDebugScope), sizeof(cmd->PushDebugScope), &cmdId);
    AFX_ASSERT(cmd);

    AfxMakeString2048(&cmd->PushDebugScope.label, name);

    if (color)
        AfxColorCopy(cmd->PushDebugScope.color, color);
    else
        AfxColorReset(cmd->PushDebugScope.color);

    return cmdId;
}

_AVX afxCmdId AvxCmdPopDebugScope(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(PopDebugScope), sizeof(cmd->PopDebugScope), &cmdId);
    AFX_ASSERT(cmd);
    cmd->PopDebugScope.nothing = NIL;
    return cmdId;
}

_AVX afxCmdId AvxCmdInsertDebugLabel(afxDrawContext dctx, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(InsertDebugLabel), sizeof(cmd->InsertDebugLabel), &cmdId);
    AFX_ASSERT(cmd);

    AfxMakeString2048(&cmd->InsertDebugLabel.label, name);

    if (color)
        AfxColorCopy(cmd->InsertDebugLabel.color, color);
    else
        AfxColorReset(cmd->InsertDebugLabel.color);

    return cmdId;
}

////////////////////////////////////////////////////////////////////////////////

_AVX avxCmdbState _AvxGetCommandStatus(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->state;
}

_AVX afxUnit AfxGetCommandPort(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->portId;
}

_AVX afxUnit AfxGetCommandPool(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->poolIdx;
}

_AVX afxError AfxCompileDrawCommands(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    /// If there was an error during recording, the application will be notified by an unsuccessful return 
    /// code returned by AfxEndCommandBuffer, and the command buffer will be moved to the invalid state.
    /// The command buffer must have been in the recording state, and, if successful, is moved to the executable state.

    if (dctx->state != avxCmdbState_RECORDING) AfxThrowError();
    else
    {
        if (dctx->endCb(dctx)) AfxThrowError();
        else
        {
            dctx->state = avxCmdbState_EXECUTABLE;
        }
    }
    return err;
}

_AVX afxError AfxRecycleDrawContext(afxDrawContext dctx, afxBool freeRes)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxDrawQueue dque = AfxGetProvider(dctx);
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    afxUnit poolIdx = dctx->poolIdx;

    if (AfxTryEnterSlockExclusive(&dque->cmdbReqLock))
    {
        if (AfxPushQueue(&dque->cmdbRecycQue, &dctx))
        {
            AfxDisposeObjects(1, &dctx);
        }
        else
        {
            dctx->state = avxCmdbState_INVALID;
        }
        AfxExitSlockExclusive(&dque->cmdbReqLock);
    }
    else
    {
        AfxDisposeObjects(1, &dctx);
    }
    return err;
}

_AVX afxError _AvxDctxDtorCb(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    while (AfxLoadAtom32(&dctx->submCnt))
    {
        AfxYield();
    }

    AFX_ASSERT(dctx->state != avxCmdbState_PENDING);

    //AfxRecycleCmdBuffers(TRUE, 1, &dctx);

    //if (dctx->base.vmt->dtor && dctx->base.vmt->dtor(dctx))
        //AfxThrowError();

    //AFX_ASSERT(!dctx->base.idd);

    AfxDismantleArena(&dctx->cmdArena);

    return err;
}

_AVX afxError _AvxDctxCtorCb(afxDrawContext dctx, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxDrawQueue dque = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    
    //afxDrawSystem dsys = AfxGetDrawBridgeContext(dexu);
    
    dctx->submCnt = 0;
    dctx->submQueMask = NIL;
    dctx->portId = AfxGetDrawQueuePort(dque);
    dctx->poolIdx = AfxGetDrawQueuePort(dque);

    dctx->disposable = TRUE;

    dctx->state = avxCmdbState_INITIAL;

    AfxDeployChain(&dctx->commands, dctx);
    AfxDeployArena(&dctx->cmdArena, NIL, AfxHere());

    dctx->endCb = _AvxDctxEndCb;
    dctx->resetCb = _AvxDctxResetCb;
    
    dctx->inRenderPass = FALSE;
    dctx->inVideoCoding = FALSE;

    return err;
}

_AVX afxClassConfig const _AVX_DCTX_CLASS_CONFIG =
{
    .fcc = afxFcc_DCTX,
    .name = "DrawContext",
    .desc = "Draw Operation Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawContext)),
    .ctor = (void*)_AvxDctxCtorCb,
    .dtor = (void*)_AvxDctxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireDrawContexts(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx, afxUnit cnt, afxDrawContext batches[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(batches);
    AFX_ASSERT(cnt);

    afxDrawBridge dexu;
    if (!AfxGetDrawBridges(dsys, exuIdx, 1, &dexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawQueue dque;
    if (!AfxGetDrawQueues(dexu, queIdx, 1, &dque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (!AfxTryEnterSlockExclusive(&dque->cmdbReqLock))
    {
        AfxThrowError();
        return err;
    }

    afxUnit cnt2 = 0;
    afxDrawContext* unit;
    while ((unit = AfxPeekQueue(&dque->cmdbRecycQue)))
    {
        afxDrawContext dctx = *unit;
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

        if (dctx->resetCb(dctx, TRUE, avxCmdbUsage_ONCE)) AfxThrowError();
        else
        {
            AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);

            batches[cnt2] = dctx;
            ++cnt2;
        }

        if (err)
            AfxDisposeObjects(1, &dctx);

        AfxPopQueue(&dque->cmdbRecycQue);

        if (cnt2 == cnt)
            break;
    }

    AFX_ASSERT_OBJECTS(afxFcc_DCTX, cnt2, batches);

    AfxExitSlockExclusive(&dque->cmdbReqLock);

    if (cnt2 < cnt)
    {
        afxUnit cnt3 = cnt - cnt2;

        afxClass* cls = (afxClass*)_AvxGetDrawBatchClass(dque);
        AFX_ASSERT_CLASS(cls, afxFcc_DCTX);

        if (AfxAcquireObjects(cls, cnt3, (afxObject*)&batches[cnt2], (void const*[]) { dque }))
        {
            AfxThrowError();

            for (afxUnit j = cnt2; j-->0;)
                AfxRecycleDrawContext(batches[j], TRUE);

            cnt2 = 0;
        }
        else
        {
            for (afxUnit k = 0; k < cnt3; k++)
            {
                afxDrawContext dctx = batches[cnt2 + k];
                AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

                if (dctx->resetCb(dctx, TRUE, avxCmdbUsage_ONCE)) AfxThrowError();
                else
                {
                    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);

                    batches[cnt2] = dctx;
                    ++cnt2;
                }
            }
            AFX_ASSERT_OBJECTS(afxFcc_DCTX, cnt2, batches);
        }
    }
    return err;
}
