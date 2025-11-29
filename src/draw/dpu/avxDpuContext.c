/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_QUEUE_C
//#define _AVX_SURFACE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_BUFFER_C
#include "../avxIcd.h"

_AVX afxError _AvxDpuCmd_CopyBufferCb(avxDpu* dpu, _avxCmd* cmd)
{
    afxError err = { 0 };
    //cmd->CopyBuffer.
    return err;
}

_AVX _avxCmdLut const _AVX_DPU_CMD_VMT =
{
    .CopyBuffer = _AvxDpuCmd_CopyBufferCb
};

_AVX afxError _AvxDpuRollContext(avxDpu* dpu, afxDrawContext dctx, afxUnit batchId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    _avxCmdBatch* cmdb = _AvxDctxGetCmdBatch(dctx, batchId);

    if (!cmdb)
    {
        AfxThrowError();
        return err;
    }

    //AFX_ASSERT(cmdb->state == avxDrawContextState_PENDING);

    //if (cmdb->state != avxDrawContextState_PENDING)
    {
        //AfxThrowError();
        //return err;
    }

    afxDrawBridge dexu = dpu->dexu;
    _avxCmdLut const*cmdVmt = dexu->cmdVmt;
    afxCmdId lastId = 0; // DBG

    _avxCmd *cmdHdr;
    AFX_ITERATE_CHAIN_B2F(cmdHdr, hdr.script, &cmdb->commands)
    {
        lastId = cmdHdr->hdr.id;

        if (cmdHdr->hdr.id == NIL/*ZGL_CMD_END*/)
        {
            break;
        }

        //if (cmdb->state != avxDrawContextState_PENDING)
        {
            //AfxThrowError();
            //break;
        }

        AFX_ASSERT(cmdVmt->f[cmdHdr->hdr.id]);
        cmdVmt->f[cmdHdr->hdr.id](dpu, cmdHdr);
    }

    if (!err)
    {
        //cmdb->state = avxDrawContextState_EXECUTABLE;
    }

    if (err || cmdb->once)
    {
        //AFX_ASSERT(dctx->portId == dpu->portId);
        //cmdb->state = avxDrawContextState_INVALID;

        afxDrawBridge dexu = AfxGetHost(dctx);
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
#if 0
        //afxUnit poolIdx = dctx->poolIdx;

        AfxLockFutex(&dque->cmdbReqLock);

        if (AfxPushQueue(&dque->cmdbRecycQue, &dctx))
        {
            AfxDisposeObjects(1, (void**)&dctx);
        }

        AfxUnlockFutex(&dque->cmdbReqLock);
#endif
    }
    return err;
}
