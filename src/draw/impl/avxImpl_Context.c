/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_QUEUE_C
//#define _AVX_DRAW_OUTPUT_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_BUFFER_C
#include "avxImplementation.h"

_AVX afxError _AvxDpuCmd_CopyBufferCb(avxDpu* dpu, avxCmd* cmd)
{
    afxError err = AFX_ERR_NONE;
    //cmd->CopyBuffer.
    return err;
}

_AVX avxCmdList const _AVX_DPU_CMD_VMT =
{
    .CopyBuffer = _AvxDpuCmd_CopyBufferCb
};

_AVX afxError _AvxDpuRollContext(avxDpu* dpu, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT(dctx->state == avxCmdbState_PENDING);

    if (dctx->state != avxCmdbState_PENDING)
    {
        AfxThrowError();
        return err;
    }

    afxDrawBridge dexu = dpu->dexu;
    avxCmdList const*cmdVmt = dexu->cmdVmt;
    afxCmdId lastId = 0; // DBG

    avxCmd *cmdHdr;
    AFX_ITERATE_CHAIN_B2F(&dctx->commands, avxCmd, hdr.script, cmdHdr)
    {
        lastId = cmdHdr->hdr.id;

        if (cmdHdr->hdr.id == NIL/*ZGL_CMD_END*/)
        {
            break;
        }

        if (dctx->state != avxCmdbState_PENDING)
        {
            AfxThrowError();
            break;
        }

        AFX_ASSERT(cmdVmt->f[cmdHdr->hdr.id]);
        cmdVmt->f[cmdHdr->hdr.id](dpu, cmdHdr);
    }

    if (!err)
    {
        dctx->state = avxCmdbState_EXECUTABLE;
    }

    if (err || dctx->disposable)
    {
        //AFX_ASSERT(dctx->portId == dpu->portId);
        dctx->state = avxCmdbState_INVALID;

        afxDrawQueue dque = AfxGetProvider(dctx);
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        //afxUnit poolIdx = dctx->poolIdx;

        AfxEnterSlockExclusive(&dque->cmdbReqLock);

        if (AfxPushQueue(&dque->cmdbRecycQue, &dctx))
        {
            AfxDisposeObjects(1, (void**)&dctx);
        }

        AfxExitSlockExclusive(&dque->cmdbReqLock);
    }
    AfxDecAtom32(&dctx->submCnt);
    AfxDisposeObjects(1, &dctx);
    return err;
}

_AVX afxError _AvxDctxResetCb(afxDrawContext dctx, afxBool freeMem, afxBool permanent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(dctx->state != avxCmdbState_PENDING);

    dctx->disposable = !permanent;
    AfxDeployChain(&dctx->commands, dctx);
    AfxExhaustArena(&dctx->cmdArena);
    dctx->state = avxCmdbState_RECORDING;

    return err;
}

_AVX afxError _AvxDctxEndCb(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return err;
}
