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

/*
    Audio Element	Role in Pipeline	                Graphics Analogy
    Track	        Audio source (sample, synth, loop)	Mesh / Object
    Voice	        Instance of a track being played    Mesh instance / draw call
    Sample	        Raw PCM data (per frame)	        Vertex data
    Bus	            Group of mixed signals	            Render layer or render pass
    DSP Unit (FX)	Filter, EQ, compressor	            Shader module (e.g., BRDF)
    Master Bus	    Final stereo or surround output	    Framebuffer
    Sample Rate	    44100 Hz = 44100 frames/sec	        Resolution or frame rate
*/

AFX_DEFINE_STRUCT(_amxPlaying)
{
    afxLink             link;
    amxBufferedTrack    buf;
    afxUnit64           posn;
    afxUnit             iterCnt;
};

_AMX afxError _AmxMctxPushCmd(afxMixContext mctx, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxCmd** ppCmd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);
    AFX_ASSERT(siz >= sizeof(_amxCmdHdr));

    _amxCmd* cmd = AfxRequestArena(&mctx->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &mctx->commands);
    AFX_ASSERT(ppCmd);
    *ppCmd = cmd;

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxClass const* _AmxMctxGetMctxClass(afxMixContext mctx)
{
    afxError err = { 0 };
    // mctx must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);
    afxClass const* cls = &mctx->mctxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MCTX);
    return cls;
}

_AMX amxContextState _AmxMctxGetStatus(afxMixContext mctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);
    return mctx->state;
}

_AMX afxMask AmxGetCommandPort(afxMixContext mctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);
    return mctx->exuMask;
}

_AMX afxMixContext AmxGetCommandPool(afxMixContext mctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);
    return mctx->pool;
}

_AMX afxError AmxPrepareMixCommands(afxMixContext mctx, afxBool purge, amxCmdFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);
    // mctx must be a valid afxMixContext handle.
    // mctx must not be in the PENDING state.
    AFX_ASSERT(mctx->state != amxContextState_PENDING);

    AFX_ASSERT(mctx->ddi->prepare);
    if (mctx->ddi->prepare(mctx, purge, flags))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(mctx->state == amxContextState_RECORDING);
        AFX_ASSERT(AfxIsChainEmpty(&mctx->commands));
    }
    return err;
}

_AMX afxError AmxCompileMixCommands(afxMixContext mctx)
{
    afxError err = { 0 };
    // mctx must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);
    // mctx must be in the RECORDING state.
    AFX_ASSERT(mctx->state == amxContextState_RECORDING);

    AFX_ASSERT(mctx->ddi->compile);
    if (mctx->ddi->compile(mctx))
    {
        AfxThrowError();

        // If there was an error during recording, the application will be notified by an unsuccessful return 
        // code returned by AfxCompileMixCommands, and the draw context will be moved to the invalid state.

        AFX_ASSERT(mctx->state == amxContextState_INVALID);
    }
    else
    {
        // The mix context must have been in the recording state, and, if successful, is moved to the executable state.
        AFX_ASSERT(mctx->state == amxContextState_EXECUTABLE);
        //AFX_ASSERT(AfxIsChainEmpty(&mctx->commands));
    }
    return err;
}

_AMX afxError AmxExhaustMixContext(afxMixContext mctx, afxBool freeMem)
{
    afxError err = { 0 };
    // mctx must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);
    // dctx must be a pool context.
    AFX_ASSERT(!mctx->pool || (mctx->pool == mctx));

    AFX_ASSERT(mctx->ddi->exhaust);
    if (mctx->ddi->exhaust(mctx, freeMem))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(AfxIsChainEmpty(&mctx->commands));
    }
    return err;
}

_AMX afxError _AmxMctxDtorCb(afxMixContext mctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);

    AmxExhaustMixContext(mctx, TRUE);

    AfxExhaustInterlockedQueue(&mctx->recycQue);

    AfxDismantleArena(&mctx->cmdArena);

    AfxDeregisterChainedClasses(&mctx->classes);

    return err;
}

_AMX afxError _AmxMctxCtorCb(afxMixContext mctx, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxMixContext pool = AFX_CAST(afxMixContext, args[1]);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_MCTX, 1, &pool);
    AFX_ASSERT(args[2]);
    amxContextConfig const* info = AFX_CAST(amxContextConfig const*, args[2]);
    afxClassConfig const* pMctxClsCfg = NIL;/* AFX_CAST(afxClassConfig const*, args[3]);*/

    mctx->ddi = &_AMX_MCTX_DDI;
    mctx->udd = info->udd;
    mctx->tag = info->tag;
    //mctx->devLimits = _AmxMsysGetLimits(msys);
    //mctx->enabledFeatures = _AmxMsysGetReqFeatures(msys);
    
    AfxMakeChain(&mctx->classes, mctx);

    if (!pool)
    {
        mctx->pool = mctx;

        mctx->caps = info->caps;
        mctx->exuMask = info->exuMask;

        afxClassConfig mctxClsCfg = pMctxClsCfg ? *pMctxClsCfg : _AMX_MCTX_CLASS_CONFIG;
        AFX_ASSERT(mctxClsCfg.fcc == afxFcc_MCTX);
        mctxClsCfg.unitsPerPage = AFX_OR(AFX_MIN(info->auxCnt, 32), mctxClsCfg.unitsPerPage);
        AfxMountClass(&mctx->mctxCls, NIL, &mctx->classes, &mctxClsCfg);

        // FIXME: Interlocked queue can not be zero.
        AfxMakeInterlockedQueue(&mctx->recycQue, sizeof(afxMixContext), AFX_MAX(AFX_MIN(info->recycCap, 32), 1));
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &pool);

        // If there is a base, the base must be its own base.
        // Effectively, a aux context can only be acquired from a base context.
        // By allowing it to be itself, we allow the code to proceed normally avoid testing against NIL.
        if (pool->pool && (pool->pool != pool))
        {
            AfxThrowError();
            return err;
        }
        mctx->pool = pool;

        mctx->caps = (pool->caps & info->caps);
        mctx->exuMask = (pool->exuMask & info->exuMask);

        mctx->mctxCls = (afxClass) { 0 };

        // FIXME: Interlocked queue can not be zero.
        AfxMakeInterlockedQueue(&mctx->recycQue, sizeof(afxMixContext), 1);
    }

    mctx->flags = info->flags;
    mctx->cmdFlags = info->cmdFlags;
    mctx->state = amxContextState_INITIAL;

    AfxMakeChain(&mctx->commands, mctx);
    AfxMakeArena(&mctx->cmdArena, NIL, AfxHere());

    //afxUnit chanCnt = AFX_MAX(1, cfg->chanCnt);
    //afxUnit freq = AFX_MAX(1, cfg->freq);
    //mctx->bps = cfg->bps ? cfg->bps : 32;
    //mctx->latency = AFX_MAX(cfg->latency, 4096);
    //mctx->freq = freq;
    //mctx->chanCnt = chanCnt;
    mctx->disabled = FALSE;
    mctx->muted = FALSE;
    mctx->routing = NIL;
    //mctx->chanCnt = mctx->chanCnt;
    //mctx->latency = mctx->latency;
    //mctx->freq = mctx->freq;

    {
        mctx->motor.flags = 1;// arxCapstanFlag_ACTIVE;
        mctx->motor.timing.currClock = 0;// cfg->currClock; --- set by roll
        mctx->motor.dtLocalClockPending = 0.0;
        mctx->motor.localClock = 0.0;
        mctx->motor.speed = 1.0;
        mctx->motor.localDur = 0;// cfg->localDur; --- set by roll
        mctx->motor.currIterIdx = 0;
        mctx->motor.iterCnt = 0;// cfg->iterCnt; --- set by roll
        mctx->motor.timing.killClock = 0;
        mctx->motor.currWeight = 1;
        mctx->motor.timing.easeInStartClock = 0;
        mctx->motor.timing.easeInEndClock = 0;
        mctx->motor.easeInValues = (afxUnit)-65536;
        mctx->motor.timing.easeOutStartClock = 0;
        mctx->motor.timing.easeOutEndClock = 0;
        mctx->motor.easeOutValues = (afxUnit)0xFFFF;
    }

    return err;
}

_AMX afxClassConfig const _AMX_MCTX_CLASS_CONFIG =
{
    .fcc = afxFcc_MCTX,
    .name = "MixContext",
    .desc = "AMX Operation Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixContext)),
    .ctor = (void*)_AmxMctxCtorCb,
    .dtor = (void*)_AmxMctxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireMixContexts(afxMixSystem msys, afxMixContext pool, amxContextConfig const* info, afxUnit cnt, afxMixContext contexts[])
{
    afxError err = { 0 };
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    // contexts must be a valid pointer to an array of @cnt * afxMixContext handles.
    AFX_ASSERT(contexts);
    // @info must be a valid pointer to a valid amxContextConfig structure.
    AFX_ASSERT(info);
    // @cnt must be greater than 0.
    AFX_ASSERT(cnt);

    AFX_ASSERT(32 >= info->auxCnt);
    AFX_ASSERT(32 >= info->recycCap);

    afxClass* cls;

    if (!pool)
    {
        cls = (afxClass*)_AmxMsysGetMixClass(msys);
        AFX_ASSERT_CLASS(cls, afxFcc_MCTX);
        if (AfxAcquireObjects(cls, cnt, (afxObject*)contexts, (void const*[]) { msys, pool, info }))
        {
            AfxThrowError();
            return err;
        }

        AFX_ASSERT_OBJECTS(afxFcc_MCTX, cnt, contexts);
        return err;
    }

    // If @pool is not NIL, @pool must be a valid afxMixContext handle.
    AFX_TRY_ASSERT_OBJECTS(afxFcc_MCTX, 1, &pool);

    if (pool->pool != pool)
    {
        AFX_ASSERT(pool->pool != pool);
        AfxThrowError();
        return err;
    }

    afxBool once = (info->cmdFlags & amxCmdFlag_ONCE);
    afxBool deferred = (info->cmdFlags & amxCmdFlag_DEFERRED);

    afxUnit recycCnt = 0;
    while (1)
    {
        // Firstly, try recycling contexts.

        afxMixContext aux;
        if (AfxPopInterlockedQueue(&pool->recycQue, &aux))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &aux);
            AFX_ASSERT(aux->submCnt == 0);
            AFX_ASSERT(aux->submQueMask == 0);

            AmxPrepareMixCommands(aux, TRUE, info->cmdFlags);

            AfxMakeChain(&aux->commands, aux);

            AFX_ASSERT(AfxLoadAtom32(&aux->submCnt) == 0);
            aux->submCnt = 0;
            aux->submQueMask = NIL;

            aux->flags = info->flags ? (pool->flags & info->flags) : pool->flags;
            aux->cmdFlags = info->cmdFlags ? (pool->cmdFlags & info->cmdFlags) : pool->cmdFlags;
            aux->exuMask = info->exuMask ? (pool->exuMask & info->exuMask) : pool->exuMask;
            aux->caps = info->caps ? (pool->caps & info->caps) : pool->caps;

            contexts[recycCnt] = aux;

            ++recycCnt;
            if (recycCnt >= cnt)
                break;
        }
    }

    afxUnit remainCnt = (cnt - recycCnt);
    if (remainCnt)
    {
        cls = (afxClass*)_AmxMctxGetMctxClass(pool);
        AFX_ASSERT_CLASS(cls, afxFcc_MCTX);
        if (AfxAcquireObjects(cls, remainCnt, (afxObject*)&contexts[recycCnt], (void const*[]) { msys, pool, info }))
        {
            AfxThrowError();

            // If acquisition failed, dispose all recycled contexts.
            // TODO: Recycle it.
            AfxDisposeObjects(recycCnt, contexts);
            return err;
        }
    }

    AFX_ASSERT_OBJECTS(afxFcc_MCTX, cnt, contexts);
    return err;
}

_AMX afxError AmxRecycleMixContexts(afxBool freeRes, afxUnit cnt, afxMixContext contexts[])
{
    afxError err = { 0 };

    for (afxUnit iter = 0; iter < cnt; iter++)
    {
        afxMixContext mctx = contexts[iter];
        if (!mctx) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);
        AFX_ASSERT(mctx->state != amxContextState_PENDING);

        AFX_ASSERT(mctx->ddi->recycle);
        if (mctx->ddi->recycle(mctx, freeRes))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(mctx->state == amxContextState_INITIAL);
            AFX_ASSERT(AfxIsChainEmpty(&mctx->commands));
        }
    }
    return err;
}

_AMX afxError AmxExecuteMixCommands(afxMixSystem msys, afxUnit cnt, amxSubmission const submissions[])
{
    afxError err = { 0 };
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(submissions);
    AFX_ASSERT(cnt);

    for (afxUnit ctxIt = 0; ctxIt < cnt; ctxIt++)
    {
        amxSubmission const* subm = &submissions[ctxIt];

        afxMixContext mctx = subm->mctx;
        if (!mctx)
        {
            AFX_ASSERT(subm->mctx);
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mctx);

        AFX_ASSERT(!(mctx->cmdFlags & amxCmdFlag_DEFERRED));

        if (mctx->state != amxContextState_EXECUTABLE)
        {
            if ((mctx->state == amxContextState_PENDING) ||
                (mctx->state == amxContextState_INTERNAL_EXECUTING))
            {
                if (!(mctx->cmdFlags & amxCmdFlag_SHARED))
                {
                    AFX_ASSERT(!(mctx->cmdFlags & amxCmdFlag_SHARED));
                    AfxThrowError();
                }
            }
        }

        afxMask exuMask = subm->exuMask;
        afxUnit exuCnt = AmxChooseMixBridges(msys, AFX_INVALID_INDEX, NIL, exuMask, 0, 0, NIL);
        afxUnit nextExuIdx = AfxRandom2(0, exuCnt - 1);

        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit exuIter = nextExuIdx; exuIter < exuCnt; exuIter++)
            {
                afxMixBridge dexu;
                if (!AmxChooseMixBridges(msys, AFX_INVALID_INDEX, NIL, exuMask, exuIter, 1, &dexu))
                {
                    AfxThrowError();
                    continue;
                }

                nextExuIdx = 0;

                while (1)
                {
                    afxMixQueue mque;
                    afxUnit nextQueIdx = 0;
                    while (AmxGetMixQueues(dexu, nextQueIdx, 1, &mque))
                    {
                        AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
                        ++nextQueIdx;

                        afxError err2 = _AmxMqueSubmitMixCommands(mque, 1, subm);

                        if (!err2)
                        {
                            queued = TRUE;
                            break; // while --- get queue
                        }

                        if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                            break; // while --- get queue

                        AfxThrowError();
                    }
                    if (err || queued) break; // while --- find queues
                }
                if (err || queued) break; // for --- iterate bridge
            }
            if (err || queued) break; // while --- find bridges
        }
    }
    return err;
}
