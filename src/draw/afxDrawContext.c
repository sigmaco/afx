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
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_CONTEXT_C
#include "impl/avxImplementation.h"

_AVX _avxCmd* _AvxDctxPushCmd(afxDrawContext dctx, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT(siz >= sizeof(avxCmdHdr));

    _avxCmd* cmd = AfxRequestArenaUnit(&dctx->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &dctx->commands);
    AFX_ASSERT(id != 268895528);
    return cmd;
}

////////////////////////////////////////////////////////////////////////////////

_AVX avxDrawContextState _AvxGetCommandStatus(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->state;
}

_AVX afxUnit AvxGetCommandPort(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->portId;
}

_AVX afxUnit AvxGetCommandPool(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->poolIdx;
}

_AVX afxError AvxExhaustDrawContext(afxDrawContext dctx, afxBool freeMem)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    if (dctx->state == avxDrawContextState_PENDING)
    {
        AfxThrowError();
        return err;
    }

    if (dctx->objsToBeDisposed.pop)
    {
        AfxDisposeObjects(dctx->objsToBeDisposed.pop, dctx->objsToBeDisposed.data);
        
        if (dctx->disposable)
            AfxEmptyArray(&dctx->objsToBeDisposed, FALSE, FALSE);
        else
            AfxEmptyArray(&dctx->objsToBeDisposed, TRUE, FALSE);
    }

    if (dctx->pimpl->reset)
    {
        if (dctx->pimpl->reset(dctx, TRUE, !dctx->disposable))
            AfxThrowError();
    }
    else
    {
        AfxDeployChain(&dctx->commands, dctx);
        AfxExhaustArena(&dctx->cmdArena);
        dctx->state = avxDrawContextState_RECORDING;
    }
    return err;
}

_AVX afxError AvxCompileDrawCommands(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // If there was an error during recording, the application will be notified by an unsuccessful return 
    // code returned by AfxEndCommandBuffer, and the draw context will be moved to the invalid state.
    // The draw context must have been in the recording state, and, if successful, is moved to the executable state.

    if (dctx->state != avxDrawContextState_RECORDING)
    {
        AfxThrowError();
        return err;
    }

    if (dctx->pimpl->end)
    {
        if (dctx->pimpl->end(dctx))
            AfxThrowError();
    }
    else
    {
        dctx->state = avxDrawContextState_EXECUTABLE;
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

    AFX_ASSERT(dctx->state != avxDrawContextState_PENDING);

    AvxExhaustDrawContext(dctx, TRUE);
    AFX_ASSERT(dctx->objsToBeDisposed.pop == 0);
    AfxEmptyArray(&dctx->objsToBeDisposed, FALSE, FALSE);

    AfxDismantleArena(&dctx->cmdArena);

    return err;
}

_AVX afxError _AvxDctxCtorCb(afxDrawContext dctx, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxDrawQueue dque = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    
    //afxDrawSystem dsys = AvxGetBridgedDrawSystem(dexu);
    
    dctx->submCnt = 0;
    dctx->submQueMask = NIL;
    dctx->portId = AvxGetDrawQueuePort(dque);
    dctx->poolIdx = AvxGetDrawQueuePort(dque);
    
    afxDrawBridge dexu = AfxGetProvider(dque);
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawSystem dsys = AfxGetProvider(dexu);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxDrawDevice ddev = AvxGetBridgedDrawDevice(dexu, NIL);
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    dctx->devLimits = _AvxDdevGetLimits(ddev);
    dctx->enabledFeatures = _AvxDsysAccessReqFeatures(dsys);

    dctx->disposable = TRUE;

    dctx->state = avxDrawContextState_INITIAL;

    AfxDeployChain(&dctx->commands, dctx);
    AfxMakeArena(&dctx->cmdArena, NIL, AfxHere());

    dctx->pimpl = &_AVX_DCTX_DDI;
    
    dctx->inDrawScope = FALSE;
    dctx->inVideoCoding = FALSE;

    AfxMakeArray(&dctx->objsToBeDisposed, sizeof(afxObject), 4, NIL, 0);

    return err;
}

_AVX afxClassConfig const _AVX_DCTX_CLASS_CONFIG =
{
    .fcc = afxFcc_DCTX,
    .name = "DrawContext",
    .desc = "Draw Operation Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawContext)),
    .unitsPerPage = 4,
    .ctor = (void*)_AvxDctxCtorCb,
    .dtor = (void*)_AvxDctxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireDrawContexts(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx, afxUnit cnt, afxDrawContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(contexts);
    AFX_ASSERT(cnt);

    afxDrawBridge dexu;
    if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawQueue dque;
    if (!AvxGetDrawQueues(dexu, queIdx, 1, &dque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (!AfxTryLockFutex(&dque->cmdbReqLock))
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

        if (AvxExhaustDrawContext(dctx, FALSE))
        {
            AfxDisposeObjects(1, &dctx);
        }
        else
        {
            AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
            contexts[cnt2] = dctx;
            ++cnt2;
        }

        AfxPopQueue(&dque->cmdbRecycQue);

        if (cnt2 == cnt)
            break;
    }

    AfxUnlockFutex(&dque->cmdbReqLock);

    if (err) return err;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, cnt2, contexts);

    if (cnt2 < cnt)
    {
        afxUnit cnt3 = cnt - cnt2;

        afxClass* cls = (afxClass*)_AvxGetDrawContextClass(dque);
        AFX_ASSERT_CLASS(cls, afxFcc_DCTX);

        if (AfxAcquireObjects(cls, cnt3, (afxObject*)&contexts[cnt2], (void const*[]) { dque }))
        {
            AfxThrowError();
            AvxRecycleDrawContexts(dsys, TRUE, cnt2, contexts);
            cnt2 = 0;
        }
        else
        {
            for (afxUnit k = 0; k < cnt3; k++)
            {
                afxDrawContext dctx = contexts[cnt2 + k];
                AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

                if (AvxExhaustDrawContext(dctx, TRUE))
                {
                    AfxThrowError();
                    AfxDisposeObjects(cnt, contexts);
                    cnt2 = 0;
                    break;
                }
                else
                {
                    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
                    contexts[cnt2] = dctx;
                    ++cnt2;
                }
            }
        }
    }

    if (!err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, cnt, contexts);
        AFX_ASSERT(cnt == cnt2);
    }
    return err;
}

_AVX afxError AvxRecycleDrawContexts(afxDrawSystem dsys, afxBool freeRes, afxUnit cnt, afxDrawContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxDrawContext dctx = contexts[i];

        if (!dctx) continue;
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

        afxDrawQueue dque = AfxGetProvider(dctx);
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        afxUnit poolIdx = dctx->poolIdx;

        if (AfxTryLockFutex(&dque->cmdbReqLock))
        {
            if (AfxPushQueue(&dque->cmdbRecycQue, &dctx))
            {
                AfxDisposeObjects(1, &dctx);
            }
            else
            {
                dctx->state = avxDrawContextState_INVALID;
            }
            AfxUnlockFutex(&dque->cmdbReqLock);
        }
        else
        {
            AfxDisposeObjects(1, &dctx);
        }
    }
    return err;
}

_AVX afxError AvxExecuteDrawCommands(afxDrawSystem dsys, avxSubmission* ctrl, afxUnit cnt, afxDrawContext contexts[], avxFence fence)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(contexts);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(cnt);

    /*
        If any draw context submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a draw context complete, it moves from the pending state, back to the executable state.
        If a draw context was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */
    
    afxMask exuMask = ctrl->exuMask;
    afxUnit exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, AFX_INVALID_INDEX, NIL, 0, 0, NIL);
    for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
    {
        if (exuMask && !(exuMask & AFX_BIT(exuIdx)))
            continue;

        afxDrawBridge dexu;
        if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
        {
            AfxThrowError();
            return err;
        }

        afxClass const* dqueCls = _AvxGetDrawQueueClass(dexu);

        // sanitize arguments
        afxUnit totalQueCnt = dqueCls->instCnt;
        afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, totalQueCnt - 1);
        afxUnit queCnt = AFX_CLAMP(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
        AFX_ASSERT(queCnt);
        afxBool queued = FALSE;

        do
        {
            for (afxUnit i = 0; i < queCnt; i++)
            {
                afxUnit queIdx = baseQueIdx + i;

                afxDrawQueue dque;
                if (!AvxGetDrawQueues(dexu, queIdx, 1, &dque))
                {
                    AfxThrowError();
                    break;
                }

                afxError err2;
                if ((err2 = _AvxDqueExecuteDrawCommands(dque, ctrl, 1, &contexts[i], fence)))
                {
                    if (err2 == afxError_TIMEOUT)
                        continue;

                    AfxThrowError();
                    break;
                }
                queued = TRUE;
                break;
            }

            if (err || queued)
                break; // reiterate if not queue for timeout?

        } while (0);
        break;
    }
    return err;
}
