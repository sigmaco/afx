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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_C
//#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_INPUT_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
#define _AVX_CMD_BUFFER_C
#include "../../dev/AvxImplKit.h"

////////////////////////////////////////////////////////////////////////////////

_AVX afxCmdId AvxCmdPushDebugScope(avxCmdb cmdb, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);
    
    return cmdb->stdCmds->PushDebugScope(cmdb, name, color);
}

_AVX afxCmdId AvxCmdPopDebugScope(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);
    
    return cmdb->stdCmds->PopDebugScope(cmdb);
}

_AVX afxCmdId AvxCmdInsertDebugLabel(avxCmdb cmdb, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    return cmdb->stdCmds->InsertDebugLabel(cmdb, name, color);
}

////////////////////////////////////////////////////////////////////////////////

_AVX avxCmdbState AfxGetCmdBufferState(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    return cmdb->state;
}

_AVX afxNat AfxGetCmdBufferPort(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    return cmdb->portIdx;
}

_AVX afxNat AfxGetCmdBufferPool(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    return cmdb->poolIdx;
}

_AVX afxError _AvxCmdbStdDtor(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

    while (AfxLoadAtom32(&cmdb->submCnt))
    {
        AfxYield();
    }

    AfxAssert(cmdb->state != avxCmdbState_PENDING);

    //AfxRecycleCmdBuffers(TRUE, 1, &cmdb);

    //if (cmdb->base.vmt->dtor && cmdb->base.vmt->dtor(cmdb))
        //AfxThrowError();

    //AfxAssert(!cmdb->base.idd);

    AfxDismantleArena(&cmdb->cmdArena);

    return err;
}

_AVX afxError _AvxCmdbStdCtor(avxCmdb cmdb, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

    afxDrawQueue dque = args[0];
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
    
    //afxDrawContext dctx = AfxGetDrawBridgeContext(dexu);

    cmdb->submCnt = 0;
    cmdb->submQueMask = NIL;
    cmdb->poolIdx = AfxGetDrawBridgePort(AfxGetParent(dque));
    AfxDeployArena(&cmdb->cmdArena, NIL, AfxHere());

    cmdb->disposable = TRUE;

    cmdb->state = avxCmdbState_INITIAL;

    cmdb->stdCmds = NIL;

    cmdb->beginCb = NIL;
    cmdb->endCb = NIL;
    cmdb->resetCb = NIL;
    
    cmdb->inRenderPass = FALSE;
    cmdb->inVideoCoding = FALSE;

    return err;
}

_AVX afxClassConfig const _AvxCmdbStdImplementation =
{
    .fcc = afxFcc_CMDB,
    .name = "CmdBatch",
    .desc = "Draw Command Batch",
    .fixedSiz = sizeof(AFX_OBJECT(avxCmdb)),
    .ctor = (void*)_AvxCmdbStdCtor,
    .dtor = (void*)_AvxCmdbStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxCompileCmdBuffers(afxNat cnt, avxCmdb streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(streams);
    AfxAssert(cnt);

    /// If there was an error during recording, the application will be notified by an unsuccessful return code returned by AfxEndCommandBuffer, and the command buffer will be moved to the invalid state.

    for (afxNat i = 0; i < cnt; i++)
    {
        avxCmdb cmdb = streams[i];
        AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

        /// The command buffer must have been in the recording state, and, if successful, is moved to the executable state.

        if (cmdb->state != avxCmdbState_RECORDING) AfxThrowError();
        else
        {
            if (cmdb->endCb(cmdb)) AfxThrowError();
            else
            {
                cmdb->state = avxCmdbState_EXECUTABLE;
            }
        }
    }
    return err;
}

_AVX afxError AfxRecycleCmdBuffers(afxBool freeRes, afxNat cnt, avxCmdb streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(streams);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        avxCmdb cmdb = streams[i];
        AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
        afxDrawQueue dque = AfxGetParent(cmdb);
        AfxAssertObjects(1, &dque, afxFcc_DIN);
        afxNat poolIdx = cmdb->poolIdx;
        
        if (AfxTryEnterSlockExclusive(&dque->cmdbReqLock))
        {
            if (AfxPushQueue(&dque->cmdbRecycQue, &cmdb))
            {
                AfxReleaseObjects(1, &cmdb);
            }
            else
            {
                cmdb->state = avxCmdbState_INVALID;
            }
            AfxExitSlockExclusive(&dque->cmdbReqLock);
        }
    }
    return err;
}

#if 0
_AVX afxError AfxRecordCmdBuffers(avxCmdbUsage usage, afxNat cnt, avxCmdb streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(streams);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        avxCmdb cmdb = streams[i];
        AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

        if (!(cmdb->state == avxCmdbState_INITIAL || cmdb->state == avxCmdbState_EXECUTABLE)) AfxThrowError();
        else
        {
            if (cmdb->beginCb(cmdb, usage)) AfxThrowError();
            else
            {
                cmdb->state = avxCmdbState_RECORDING;
            }
        }
    }
    return err;
}
#endif

_AVX afxError AfxAcquireBatches(afxDrawContext dctx, afxNat portId, afxNat cnt, avxCmdb batches[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(batches);
    AfxAssert(cnt);

    afxNat cnt2 = 0;

    afxNat dexuCnt = AfxCountDrawBridges(dctx, portId, 0);
    afxDrawBridge dexu;
    
    for (afxNat i = 0; i < dexuCnt; i++)
    {
        if (AfxQueryDrawBridges(dctx, portId, i, 1, &dexu))
        {
            AfxAssertObjects(1, &dexu, afxFcc_DEXU);
            afxNat queCnt = AfxCountDrawQueues(dexu, 0);
            afxDrawQueue dque;

            for (afxNat j = 0; j < queCnt; j++)
            {
                if (AfxGetDrawQueues(dexu, j, 1, &dque))
                {
                    AfxAssertObjects(1, &dque, afxFcc_DQUE);

                    if (AfxTryEnterSlockExclusive(&dque->cmdbReqLock))
                    {
                        avxCmdb* unit;

                        while ((unit = AfxPeekQueue(&dque->cmdbRecycQue)))
                        {
                            avxCmdb cmdb = *unit;
                            AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

                            if (cmdb->resetCb(cmdb)) AfxThrowError();
                            else
                            {
                                cmdb->state = avxCmdbState_INITIAL;

                                if (cmdb->beginCb(cmdb, avxCmdbUsage_ONCE)) AfxThrowError();
                                else
                                {
                                    cmdb->state = avxCmdbState_RECORDING;

                                    batches[cnt2] = cmdb;
                                    ++cnt2;
                                }
                            }

                            if (err)
                                AfxReleaseObjects(1, &cmdb);

                            AfxPopQueue(&dque->cmdbRecycQue);

                            if (cnt2 == cnt)
                                break;
                        }

                        AfxAssertObjects(cnt2, batches, afxFcc_CMDB);

                        AfxExitSlockExclusive(&dque->cmdbReqLock);

                        if (cnt2 == cnt)
                            break;
                    }

                    if (cnt2 < cnt)
                    {
                        afxNat cnt3 = cnt - cnt2;

                        if (AfxAcquireObjects(&dque->cmdbCls, cnt3, (afxObject*)&batches[cnt2], (void const*[]) { dque }))
                        {
                            AfxThrowError();
                            AfxRecycleCmdBuffers(TRUE, cnt2, batches);
                            cnt2 = 0;
                        }
                        else
                        {
                            for (afxNat k = 0; k < cnt3; k++)
                            {
                                avxCmdb cmdb = batches[cnt2 + k];
                                AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

                                if (cmdb->beginCb(cmdb, avxCmdbUsage_ONCE)) AfxThrowError();
                                else
                                {
                                    cmdb->state = avxCmdbState_RECORDING;

                                    batches[cnt2] = cmdb;
                                    ++cnt2;
                                }
                            }
                            AfxAssertObjects(cnt2, batches, afxFcc_CMDB);
                        }
                    }
                }

                if (cnt2 == cnt)
                    break;
            }
        }

        if (cnt2 == cnt)
            break;
    }
    return err;
}
