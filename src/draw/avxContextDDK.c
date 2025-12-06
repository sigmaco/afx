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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_INPUT_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_CONTEXT_C
#include "avxIcd.h"

_AVX afxError _AvxDctxExhaustCb(afxDrawContext dctx, afxBool freeMem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    /*
        vkResetCommandPool does not free command buffers. It invalidates all command buffers allocated from that pool, 
        meaning any recorded commands are discarded and the buffers return to the initial state (just allocated, but empty).
        They are not freed; they still exist, and we can re-record commands into them.
    */

    while (1)
    {
        afxUnit leftCnt = 0;
        afxDrawContext aux;
        for (afxUnit i = 0; AfxEnumerateObjects(&dctx->dctxCls, i, 1, (void**)&aux); i++)
        {
            AFX_ASSERT(aux->state != avxContextState_RECORDING);
            AFX_ASSERT(aux->state != avxContextState_PENDING);
#if 0
            while (AfxLoadAtom32(&aux->submCnt))
            {
                AfxYield();
            }
#else
            if (AfxLoadAtom32(&aux->submCnt))
            {
                ++leftCnt;
                continue;
            }
#endif
            if (AvxPrepareDrawCommands(aux, freeMem, NIL))
                AfxThrowError();
        }

        if (leftCnt)
            AfxYield();
        else
            break;
    }

    if (AvxPrepareDrawCommands(dctx, freeMem, NIL))
        AfxThrowError();

    if (dctx->objsToBeDisposed.pop)
    {
        AfxDisposeObjects(dctx->objsToBeDisposed.pop, dctx->objsToBeDisposed.items);

        AfxEmptyArray(&dctx->objsToBeDisposed, !freeMem, FALSE);
    }

    if (freeMem)
    {
        AfxExhaustArena(&dctx->cmdArena);
        //AfxExhaustPool(&dctx->batches, !freeMem);
    }

    return err;
}

_AVX afxError _AvxDctxPrepareCb(afxDrawContext dctx, afxBool purge, avxCmdFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // AVX assumes the compilation when ICD does not take the front.

#if 0
    if (purge)
    {
        AfxExhaustArena(&dctx->cmdArena);
    }
#else
    AfxExhaustArena(&dctx->cmdArena);
#endif
    AfxMakeChain(&dctx->commands, dctx);

    dctx->inDrawScope = FALSE;
    dctx->inVideoCoding = FALSE;

    dctx->state = avxContextState_RECORDING;
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);

    return err;
}

_AVX afxError _AvxDctxCompileCb(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // AVX assumes the compilation when ICD does not take the front.
    
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    dctx->state = avxContextState_EXECUTABLE;

    return err;
}

_AVX afxError _AvxDctxRecycleCb(afxDrawContext dctx, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // AVX assumes the compilation when ICD does not take the front.

    // Should wait or return?
    // On the next roll, it should be recycled anyway.
#if 0
    while (AfxLoadAtom32(&cmdb->submCnt))
    {
        AfxYield();
    }
#else
    if (AfxLoadAtom32(&dctx->submCnt))
    {
        AfxThrowError();
        return afxError_BUSY;
    }
#endif

    // There is some issues if it is called from DPU as there not a lock mechanism for arena and batches' pool.

#if 0
    if (freeRes)
    {
        AFX_ASSERT(freeRes == TRUE);
        AfxExhaustArena(&cmdb->cmdArenaCompiled);
    }
    else
    {
        _avxCmd* cmd;
        AFX_ITERATE_CHAIN(_avxCmd, cmd, hdr.script, &cmdb->commands)
        {
            AfxPopLink(&cmd->hdr.script);
            AfxReclaimArena(&cmdb->cmdArenaCompiled, cmd, cmd->hdr.siz);
        }
    }
#else
    //AfxExhaustArena(&cmdb->cmdArenaCompiled);
#endif

    //AfxMakeChain(&cmdb->commands, cmdb);

    //afxBool recycled = FALSE;
#if 0
    if (3 > dctx->cmdbRecycChain.cnt)
    {
        AfxPushLink(&cmdb->recyc, &dctx->cmdbRecycChain);
        recycled = TRUE;
    }
    // If could not enqueue for recyclage, destroy it.
#endif

#if 0
    if (!recycled)
    {
        //AfxExhaustArena(&cmdb->cmdArenaCompiled);
        AfxDismantleArena(&cmdb->cmdArenaCompiled);

        if (AfxReclaimPoolUnits(&dctx->batches, AfxHere(), 1, (void**)&cmdb))
        {
            AfxThrowError();
        }
        //AfxExhaustPool(&dctx->batches, FALSE);
    }
#endif

    AvxPrepareDrawCommands(dctx, freeRes, NIL);

    afxDrawContext pool = AvxGetCommandPool(dctx);
    // Command pool can not be recycled by itself.

    if (pool != dctx)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &pool);
        AfxPushInterlockedQueue(&pool->recycQue, &dctx);
    }

    return err;
}

_AVX _avxDdiDctx const _AVX_DDI_DCTX =
{
    .prepare = _AvxDctxPrepareCb,
    .compile = _AvxDctxCompileCb,
    .recycle = _AvxDctxRecycleCb,
    .exhaust = _AvxDctxExhaustCb
};
