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

// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AMX_MIX_C
#define _AMX_MIX_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_SYSTEM_C
#define _AMX_SINK_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_TRACK_C
#define _AMX_AUDIO_C
#define _AMX_AUDIO_C
#include "amxIcd.h"

_AMX afxError _AmxMctxExhaustCb(afxMixContext mctx, afxBool freeMem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);

    while (1)
    {
        afxUnit leftCnt = 0;
        afxMixContext aux;
        for (afxUnit i = 0; AfxEnumerateObjects(&mctx->mctxCls, i, 1, (void**)&aux); i++)
        {
            AFX_ASSERT(aux->state != amxContextState_RECORDING);
            AFX_ASSERT(aux->state != amxContextState_PENDING);
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
            if (AmxPrepareMixCommands(aux, freeMem, NIL))
                AfxThrowError();
}

        if (leftCnt)
            AfxYield();
        else
            break;
    }

    if (AmxPrepareMixCommands(mctx, freeMem, NIL))
        AfxThrowError();
#if 0
    if (mctx->objsToBeDisposed.pop)
    {
        AfxDisposeObjects(mctx->objsToBeDisposed.pop, mctx->objsToBeDisposed.items);

        AfxEmptyArray(&mctx->objsToBeDisposed, !freeMem, FALSE);
    }
#endif

    if (freeMem)
    {
        AfxExhaustArena(&mctx->cmdArena);
        //AfxExhaustPool(&mctx->batches, !freeMem);
    }
    return err;
}

_AMX afxError _AmxMctxPrepareCb(afxMixContext mctx, afxBool purge, amxCmdFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);

    // AMX assumes the compilation when ICD does not take the front.

#if 0
    if (purge)
    {
        AfxExhaustArena(&mctx->cmdArena);
    }
#else
    AfxExhaustArena(&mctx->cmdArena);
#endif
    AfxMakeChain(&mctx->commands, mctx);

    mctx->state = amxContextState_RECORDING;
    AFX_ASSERT(mctx->state == amxContextState_RECORDING);

    return err;
}

_AMX afxError _AmxMctxCompileCb(afxMixContext mctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);

    // AMX assumes the compilation when ICD does not take the front.

    AFX_ASSERT(mctx->state == amxContextState_RECORDING);
    mctx->state = amxContextState_EXECUTABLE;

    return err;
}

_AMX afxError _AmxMctxRecycleCb(afxMixContext mctx, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);

    // Should wait or return?
    // On the next roll, it should be recycled anyway.
#if 0
    while (AfxLoadAtom32(&cmdb->submCnt))
    {
        AfxYield();
    }
#else
    if (AfxLoadAtom32(&mctx->submCnt))
    {
        AfxThrowError();
        return afxError_BUSY;
    }
#endif

    // There is some issues if it is called from MPU as there not a lock mechanism for arena and batches' pool.

    AmxPrepareMixCommands(mctx, freeRes, NIL);

    afxMixContext pool = AmxGetCommandPool(mctx);
    // Command pool can not be recycled by itself.

    if (pool != mctx)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &pool);
        AfxPushInterlockedQueue(&pool->recycQue, &mctx);
    }

    return err;
}

_AMX _amxDdiMctx const _AMX_MCTX_DDI =
{
    .prepare = _AmxMctxPrepareCb,
    .compile = _AmxMctxCompileCb,
    .recycle = _AmxMctxRecycleCb,
    .exhaust = _AmxMctxExhaustCb
};
