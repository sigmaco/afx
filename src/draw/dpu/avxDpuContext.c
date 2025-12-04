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

_AVX afxError _AvxDpuRollContext(avxDpu* dpu, afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    switch (dctx->state)
    {
    case avxContextStatus_PENDING:
    {
        AfxIncAtom32(&dctx->submCnt);
        dctx->state = avxContextStatus_INTERNAL_EXECUTING;
        break;
    }
    case avxContextStatus_INTERNAL_EXECUTING:
    {
        AFX_ASSERT((dctx->cmdFlags & avxCmdFlag_SHARED));
        AfxIncAtom32(&dctx->submCnt);
        break;
    }
    default:
    {
        AFX_ASSERT( (dctx->state == avxContextStatus_PENDING) ||
                    (dctx->state == avxContextStatus_INTERNAL_EXECUTING));
        AfxThrowError();
        return err;
    }
    }

    afxDrawBridge dexu = dpu->dexu;
    _avxCmdLut const*cmdVmt = dexu->cmdVmt;
    afxCmdId lastId = 0; // DBG

    _avxCmd *cmdHdr;
    AFX_ITERATE_CHAIN_B2F(cmdHdr, hdr.script, &dctx->commands)
    {
#ifdef _AFX_DEBUG
        lastId = cmdHdr->hdr.id;
#endif

        if (cmdHdr->hdr.id == NIL/*ZGL_CMD_END*/)
        {
            break;
        }

        //if (cmdb->state != avxContextStatus_PENDING)
        {
            //AfxThrowError();
            //break;
        }

        AFX_ASSERT(cmdVmt->f[cmdHdr->hdr.id]);
        cmdVmt->f[cmdHdr->hdr.id](dpu, cmdHdr);
    }

    switch (dctx->state)
    {
    case avxContextStatus_INTERNAL_EXECUTING:
    {
        if (0 == AfxDecAtom32(&dctx->submCnt))
        {
            if (dctx->cmdFlags & avxCmdFlag_ONCE)
            {
                dctx->state = avxContextStatus_INVALID;
                AvxPrepareDrawCommands(dctx, FALSE, NIL);
            }
            else
            {
                dctx->state = avxContextStatus_EXECUTABLE;
            }
        }
        else
        {
            AFX_ASSERT((dctx->cmdFlags & avxCmdFlag_SHARED));
        }
        break;
    }
    default:
    {
        AFX_ASSERT((dctx->state == avxContextStatus_INTERNAL_EXECUTING));
        AfxThrowError();
        return err;
    }
    }

    return err;
}
