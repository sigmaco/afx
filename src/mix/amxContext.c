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

_AMX _amxCmdBatch* _AmxMctxGetCmdBatch(afxMixContext mix, afxUnit idx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    _amxCmdBatch* batch = NIL;
    afxBool valid = AfxGetPoolUnit(&mix->batches, idx, (void**)&batch);
    if ((!valid) || (!batch)) return NIL;
    AFX_ASSERT(batch->fcc == afxFcc_MCMD);
    return batch;
}

_AMX afxError _AmxMctxPushCmd(afxMixContext mix, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxCmd** ppCmd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    AFX_ASSERT(siz >= sizeof(_amxCmdHdr));

    _amxCmd* cmd = AfxRequestArena(&mix->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &mix->commands);
    AFX_ASSERT(ppCmd);
    *ppCmd = cmd;

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX amxMixState _AmxMctxGetStatus(afxMixContext mix)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->state;
}

_AMX afxUnit AmxGetCommandPort(afxMixContext mix)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->portId;
}

_AMX afxUnit AmxGetCommandPool(afxMixContext mix)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->poolIdx;
}

_AMX afxError AmxCmdPlayAudio(afxMixContext mix, afxUnit cnt, amxBufferedAudio const maps[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    return err;
}

_AMX afxError AmxExhaustMixContext(afxMixContext mix, afxBool freeMem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    AFX_ASSERT(mix->ddi->exhaust);
    if (mix->ddi->exhaust(mix, freeMem))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(AfxIsChainEmpty(&mix->commands));
        AFX_ASSERT(mix->batches.totalUsedCnt == 0);
    }
    return err;
}

_AMX afxError AmxRecordMixCommands(afxMixContext mix, afxBool once, afxBool deferred)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    AFX_ASSERT(mix->ddi->compose);
    if (mix->ddi->compose(mix, once, deferred))
    {
        AfxThrowError();
    }
    else
    {
        //AFX_ASSERT(mix->state == amxMixContextState_EXECUTABLE);
        AFX_ASSERT(AfxIsChainEmpty(&mix->commands));
    }
    return err;
}

_AMX afxError AmxDiscardMixCommands(afxMixContext mix, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    AFX_ASSERT(mix->ddi->discard);
    if (mix->ddi->discard(mix, freeRes))
    {
        AfxThrowError();
    }
    else
    {
        //AFX_ASSERT(mix->state == amxMixContextState_EXECUTABLE);
        AFX_ASSERT(AfxIsChainEmpty(&mix->commands));
    }
    return err;
}

_AMX afxError AmxCompileMixCommands(afxMixContext mix)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    // If there was an error during recording, the application will be notified by an unsuccessful return 
    // code returned by AfxEndCommandBuffer, and the draw context will be moved to the invalid state.
    // The draw context must have been in the recording state, and, if successful, is moved to the executable state.

    AFX_ASSERT(mix->ddi->compile);
    if (mix->ddi->compile(mix))
    {
        AfxThrowError();
    }
    else
    {
        //AFX_ASSERT(mix->state == amxMixContextState_EXECUTABLE);
        AFX_ASSERT(AfxIsChainEmpty(&mix->commands));
    }
    return err;
}

_AMX afxError AmxRecycleMixCommands(afxMixContext mix, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    AFX_ASSERT(mix->ddi->recycle);
    if (mix->ddi->recycle(mix, freeRes))
    {
        AfxThrowError();
    }
    return err;
}

_AMX afxBool AmxDoesMixCommandsExist_(afxMixContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &dctx);

    afxUnit batchId = dctx->batchId;
    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return FALSE;
    }

    // AMX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&dctx->cmdbReqLock, FALSE);

    _amxCmdBatch* batch = _AmxMctxGetCmdBatch(dctx, batchId);

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return !!batch;
}

_AMX afxError _AmxMixDtorCb(afxMixContext mix)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    while (AfxLoadAtom32(&mix->submCnt))
    {
        AfxYield();
    }

    AmxExhaustMixContext(mix, TRUE);

    AfxCleanUpFutex(&mix->cmdbReqLock);
    AfxMakeChain(&mix->cmdbRecycChain, mix);

    AfxDismantleArena(&mix->cmdArena);
    AfxExhaustPool(&mix->batches, TRUE);
    mix->batchId = AFX_INVALID_INDEX;

    return err;
}

_AMX afxError _AmxMixCtorCb(afxMixContext mix, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(args[1]);
    afxMixConfig const* cfg = AFX_CAST(afxMixConfig const*, args[1]) + invokeNo;

    //afxUnit chanCnt = AFX_MAX(1, cfg->chanCnt);
    //afxUnit freq = AFX_MAX(1, cfg->freq);
    //mix->bps = cfg->bps ? cfg->bps : 32;
    //mix->latency = AFX_MAX(cfg->latency, 4096);
    //mix->freq = freq;
    //mix->chanCnt = chanCnt;

    mix->tag = cfg->tag;
    mix->udd = cfg->udd;

    mix->disabled = FALSE;

    mix->muted = FALSE;
    mix->routing = NIL;
    //mix->chanCnt = mix->chanCnt;
    //mix->latency = mix->latency;
    //mix->freq = mix->freq;

    mix->routing = NIL;

    mix->cmdbLockedForReq = FALSE;
    AfxSetUpFutex(&mix->cmdbReqLock);
    AfxMakeChain(&mix->cmdbRecycChain, mix);

    AfxMakeArena(&mix->cmdArena, NIL, AfxHere());
    AfxMakeChain(&mix->commands, mix);

    mix->state = amxMixState_RECORDING;

    mix->ddi = &_AMX_MCTX_DDI;

    AfxSetUpPool(&mix->batches, sizeof(_amxCmdBatch), 3, 0);

    {
        mix->motor.flags = 1;// arxCapstanFlag_ACTIVE;
        mix->motor.timing.currClock = 0;// cfg->currClock; --- set by roll
        mix->motor.dtLocalClockPending = 0.0;
        mix->motor.localClock = 0.0;
        mix->motor.speed = 1.0;
        mix->motor.localDur = 0;// cfg->localDur; --- set by roll
        mix->motor.currIterIdx = 0;
        mix->motor.iterCnt = 0;// cfg->iterCnt; --- set by roll
        mix->motor.timing.killClock = 0;
        mix->motor.currWeight = 1;
        mix->motor.timing.easeInStartClock = 0;
        mix->motor.timing.easeInEndClock = 0;
        mix->motor.easeInValues = (afxUnit)-65536;
        mix->motor.timing.easeOutStartClock = 0;
        mix->motor.timing.easeOutEndClock = 0;
        mix->motor.easeOutValues = (afxUnit)0xFFFF;
    }

    return err;
}

_AMX afxClassConfig const _AMX_MIX_CLASS_CONFIG =
{
    .fcc = afxFcc_MIX,
    .name = "MixContext",
    .desc = "Tracked Mixing Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixContext)),
    .ctor = (void*)_AmxMixCtorCb,
    .dtor = (void*)_AmxMixDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireMixContext(afxMixSystem msys, afxMixConfig const* mcfg, afxMixContext* context)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(context);
    AFX_ASSERT(mcfg);

    afxClass* cls = (afxClass*)_AmxMsysGetMixClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MIX);
    
    afxMixContext mctx;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&mctx, (void const*[]) { msys, mcfg }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mctx);
    AFX_ASSERT(context);
    *context = mctx;
    return err;
}

_AMX afxError AmxExecuteMixCommands(afxMixSystem msys, afxUnit cnt, amxSubmission const subms[])
{
    afxError err = { 0 };
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    for (afxUnit ctxIt = 0; ctxIt < cnt; ctxIt++)
    {
        amxSubmission const* subm = &subms[ctxIt];

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

                        afxError err2 = _AmxMqueExecuteMixCommands(mque, 1, subm);

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
