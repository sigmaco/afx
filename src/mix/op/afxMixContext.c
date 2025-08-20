/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

 // This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

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
#include "../ddi/amxImplementation.h"

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

_AMX afxClass const* _AmxMixGetVoxClass(afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    afxClass const* cls = &mix->voxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VOX);
    return cls;
}

_AMX _amxCmdBatch* _AmxGetCmdBatch(afxMixContext mix, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    _amxCmdBatch* batch = NIL;
    afxBool valid = AfxGetPoolUnit(&mix->batches, idx, (void**)&batch);
    if ((!valid) || (!batch)) return NIL;
    AFX_ASSERT(batch->fcc == afxFcc_MCMD);
    return batch;
}

_AMX afxError _AmxMixPushCmd(afxMixContext mix, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxCmd** ppCmd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    AFX_ASSERT(siz >= sizeof(_amxCmdHdr));

    _amxCmd* cmd = AfxRequestFromArena(&mix->cmdArena, siz, 1, NIL, 0);
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

_AMX afxCmdId AmxBindIoStream(afxMixContext mix, afxUnit pin, afxStream iob, afxSize offset, afxSize range, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    mix->streams[pin].type = afxFcc_IOB;
    mix->streams[pin].iob.iob = iob;
    mix->streams[pin].iob.offset = offset;
    mix->streams[pin].iob.range = range;
    mix->streams[pin].iob.stride = stride;
}

_AMX afxCmdId AmxBindBufferedStream(afxMixContext mix, afxUnit pin, amxBuffer buf, afxSize offset, afxSize range, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    mix->streams[pin].type = afxFcc_MBUF;
    mix->streams[pin].posn = 0;
    mix->streams[pin].buf.buf = buf;
    mix->streams[pin].buf.offset = offset;
    mix->streams[pin].buf.range = range;
    mix->streams[pin].buf.stride = stride;
}

_AMX afxCmdId AmxBindAudioTrack(afxMixContext mix, afxUnit pin, amxAudio aud, afxUnit srcIdx, amxFormat fmt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    mix->tracks[pin].type = afxFcc_AUD;
    mix->tracks[pin].posn = 0;
    mix->tracks[pin].fmt = fmt;
    mix->tracks[pin].srcIdx = srcIdx;
    mix->tracks[pin].aud.aud = aud;
}

_AMX afxCmdId AmxBindBuffer(afxMixContext mix, afxUnit bank, amxBuffer buf, afxUnit offset, afxUnit range, afxUnit freq)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    mix->banks[bank].type = afxFcc_MBUF;
    mix->banks[bank].buf.buf = buf;
    mix->banks[bank].buf.offset = offset;
    mix->banks[bank].buf.range = range;
}

_AMX afxCmdId AmxBindAudio(afxMixContext mix, afxUnit bank, amxAudio aud)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    mix->banks[bank].type = afxFcc_AUD;
    mix->banks[bank].aud.aud = aud;
}

_AMX amxMixState AfxGetMixerState(afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->state;
}

_AMX afxUnit AfxGetMixerPort(afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->portId;
}

_AMX afxUnit AfxGetMixerPool(afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->poolIdx;
}

_AMX afxError AmxCmdPlayAudio(afxMixContext mix, afxUnit cnt, amxBufferedAudio const maps[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    return err;
}

_AMX afxError AmxExhaustMixContext(afxMixContext mix, afxBool freeMem)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    if (mix->pimpl->exhaust)
    {
        if (mix->pimpl->exhaust(mix, freeMem))
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

    AfxLockFutex(&mix->cmdbReqLock, FALSE);
#if 0
    if (mix->objsToBeDisposed.pop)
    {
        AfxDisposeObjects(mix->objsToBeDisposed.pop, mix->objsToBeDisposed.items);

        AfxEmptyArray(&mix->objsToBeDisposed, !freeMem, FALSE);
    }
#endif

    while (1)
    {
        afxUnit leftCnt = 0;
        _amxCmdBatch* batch;
        for (afxUnit i = 0; AfxEnumeratePoolItems(&mix->batches, i, 1, (void**)&batch); i++)
        {
#if 0
            while (AfxLoadAtom32(&batch->submCnt))
            {
                AfxYield();
            }
#else
            if (AfxLoadAtom32(&batch->submCnt))
            {
                ++leftCnt;
                continue;
            }
#endif
            if (AmxRecycleMixCommands(mix, i, freeMem))
                AfxThrowError();
        }

        if (leftCnt)
            AfxYield();
        else
            break;
    }

    if (freeMem)
    {
        AfxExhaustArena(&mix->cmdArena);
        AfxExhaustPool(&mix->batches, !freeMem);
    }

    AfxDeployChain(&mix->commands, mix);
    AFX_ASSERT(mix->batches.totalUsedCnt == 0);

    AfxUnlockFutex(&mix->cmdbReqLock, FALSE);

    return err;
}

_AMX afxError AmxRecordMixCommands(afxMixContext mix, afxBool once, afxBool deferred, afxUnit* batchId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    AFX_ASSERT(batchId);

    if (mix->pimpl->compose)
    {
        afxUnit bId;
        if (mix->pimpl->compose(mix, once, deferred, &bId))
        {
            AfxThrowError();
            bId = AFX_INVALID_INDEX;
        }
        else
        {
            //AFX_ASSERT(mix->state == amxMixContextState_EXECUTABLE);
            AFX_ASSERT(AfxIsChainEmpty(&mix->commands));
        }

        AFX_ASSERT(batchId);
        *batchId = bId;

        return err;
    }

    // AMX assumes the compilation when ICD does not take the front.

#if 0
    if (!AfxTryLockFutex(&mix->cmdbReqLock, FALSE))
    {
        // Bail out immediately.
        return err;
    }
#else
    AfxLockFutex(&mix->cmdbReqLock, FALSE);
#endif

    // Firstly, try recycling batches.
    afxBool recycled = FALSE;

    afxUnit cmdbId;
    _amxCmdBatch* cmdb;
    AFX_ITERATE_CHAIN_B2F(_amxCmdBatch, cmdb, recyc, &mix->cmdbRecycChain)
    {
        AFX_ASSERT(cmdb->fcc == afxFcc_MCMD);

        AfxPopLink(&cmdb->recyc);

        AfxDeployChain(&cmdb->commands, mix);

        // Move the allocated resource to the context.
        if (AfxMergeArena(&mix->cmdArena, &cmdb->cmdArenaCompiled))
        {
            AfxThrowError();
        }

        if (mix->cmdArena.cleanupCnt > 3)
            AfxExhaustArena(&mix->cmdArena);

        AFX_ASSERT(AfxLoadAtom32(&cmdb->submCnt) == 0);
        cmdb->submCnt = 0;
        cmdb->submQueMask = NIL;

        AFX_ASSERT(!once || (once == TRUE));
        cmdb->once = !!once;
        AFX_ASSERT(!deferred || (deferred == TRUE));
        cmdb->deferred = !!deferred;

        cmdbId = cmdb->uniqId;

        recycled = TRUE;
        break;
    }

    while (!recycled)
    {
        // If could not recycle a batch, request a new one from the pool.

        if (mix->batches.totalUsedCnt >= 100)
        {
            AfxThrowError();
            break;
        }

        if (AfxRequestPoolUnits(&mix->batches, 1, &cmdbId, (void**)&cmdb))
        {
            AfxThrowError();
            break;
        }

        cmdb->fcc = afxFcc_MCMD;
        cmdb->uniqId = cmdbId;
        cmdb->submCnt = 0;
        cmdb->submQueMask = NIL;

        AfxPushLink(&cmdb->recyc, NIL);

        AFX_ASSERT(!once || (once == TRUE));
        cmdb->once = !!once;
        AFX_ASSERT(!deferred || (deferred == TRUE));
        cmdb->deferred = !!deferred;

        AfxDeployChain(&cmdb->commands, mix);

        AfxMakeArena(&cmdb->cmdArenaCompiled, NIL, AfxHere());
        break;
    }

    if (!err)
    {
#if 0
        cmdb->inMixScope = FALSE;
        cmdb->inVideoCoding = FALSE;
#endif
        mix->state = amxMixState_RECORDING;
        AFX_ASSERT(mix->state == amxMixState_RECORDING);

        AFX_ASSERT(batchId);
        *batchId = cmdbId;
    }

    AfxUnlockFutex(&mix->cmdbReqLock, FALSE);

    return err;
}

_AMX afxError AmxDiscardMixCommands(afxMixContext mix, afxBool freeRes)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    if (mix->pimpl->discard)
    {
        if (mix->pimpl->discard(mix, freeRes))
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

    // AMX assumes the compilation when ICD does not take the front.

    if (AfxIsChainEmpty(&mix->commands))
        return err;

#if 0
    if (!AfxTryLockFutex(&mix->cmdbReqLock, FALSE))
    {
        // Bail out immediately.
        // Don't care for robustness here because these resources will be reused anyway if this context is kept operational.
        AfxThrowError();
        return err;
    }
#else
    AfxLockFutex(&mix->cmdbReqLock, FALSE);
#endif

    // How to use @freeRes here? It is mandatory to reclaim every allocation to arena as we could purge it all at once.

#if 0
    if (freeRes)
    {
        AfxExhaustArena(&mix->cmdArena);
    }
    else
    {
        _amxCmd* cmd;
        AFX_ITERATE_CHAIN(_amxCmd, cmd, hdr.script, &mix->commands)
        {
            AfxPopLink(&cmd->hdr.script);
            AfxReclaimToArena(&mix->cmdArena, cmd, cmd->hdr.siz);
        }
    }
#else
    AfxExhaustArena(&mix->cmdArena);
#endif

    AfxDeployChain(&mix->commands, mix);

    AfxUnlockFutex(&mix->cmdbReqLock, FALSE);

    return err;
}

_AMX afxError AmxCompileMixCommands(afxMixContext mix, afxUnit batchId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    // If there was an error during recording, the application will be notified by an unsuccessful return 
    // code returned by AfxEndCommandBuffer, and the draw context will be moved to the invalid state.
    // The draw context must have been in the recording state, and, if successful, is moved to the executable state.

    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return err;
    }

    if (mix->pimpl->compile)
    {
        if (mix->pimpl->compile(mix, batchId))
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

    // AMX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&mix->cmdbReqLock, FALSE);

    AFX_ASSERT(AfxIsAnValidPoolUnit(&mix->batches, batchId));
    _amxCmdBatch* batch = _AmxGetCmdBatch(mix, batchId);

    if (!batch)
    {
        AFX_ASSERT(batch);
    }
    else
    {
        if (AfxLoadAtom32(&batch->submCnt))
        {
            AFX_ASSERT(batch->submCnt == 0);
            AfxThrowError();
        }
        else
        {
            if (batch->recyc.next != &batch->recyc)
            {
                AFX_ASSERT(!(batch->recyc.next != &batch->recyc));
                AfxThrowError();
            }
            else
            {
                //AfxAssumeArena(&batch->cmdArenaCompiled, &mix->cmdArena);
                AfxMergeArena(&batch->cmdArenaCompiled, &mix->cmdArena);
                AfxAppendChain(&batch->commands, &mix->commands);
                AfxExhaustArena(&mix->cmdArena);
                AFX_ASSERT(AfxIsChainEmpty(&mix->commands));
            }
        }
    }

    AfxUnlockFutex(&mix->cmdbReqLock, FALSE);

    return err;
}

_AMX afxError AmxRecycleMixCommands(afxMixContext mix, afxUnit batchId, afxBool freeRes)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return err;
    }

    if (mix->pimpl->recycle)
    {
        if (mix->pimpl->recycle(mix, batchId, freeRes))
        {
            AfxThrowError();
        }
        return err;
    }

    // AMX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&mix->cmdbReqLock, FALSE);

    AFX_ASSERT(AfxIsAnValidPoolUnit(&mix->batches, batchId));
    _amxCmdBatch* cmdb = _AmxGetCmdBatch(mix, batchId);

    if (!cmdb)
    {
        AFX_ASSERT(cmdb);
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(cmdb->fcc == afxFcc_MCMD);
    AFX_ASSERT(cmdb->uniqId == batchId);

    // Should wait or return?
    // On the next roll, it should be recycled anyway.
#if 0
    while (AfxLoadAtom32(&cmdb->submCnt))
    {
        AfxYield();
    }
#else
    if (AfxLoadAtom32(&cmdb->submCnt))
    {
        AfxUnlockFutex(&mix->cmdbReqLock, FALSE);
        return afxError_BUSY;
    }
#endif

    // There is some issues if it is called from MPU as there not a lock mechanism for arena and batches' pool.

#if 0
    if (freeRes)
    {
        AFX_ASSERT(freeRes == TRUE);
        AfxExhaustArena(&cmdb->cmdArenaCompiled);
    }
    else
    {
        _amxCmd* cmd;
        AFX_ITERATE_CHAIN(_amxCmd, cmd, hdr.script, &cmdb->commands)
        {
            AfxPopLink(&cmd->hdr.script);
            AfxReclaimToArena(&cmdb->cmdArenaCompiled, cmd, cmd->hdr.siz);
        }
    }
#else
    AfxExhaustArena(&cmdb->cmdArenaCompiled);
#endif

    AfxDeployChain(&cmdb->commands, cmdb);

    afxBool recycled = FALSE;
#if 0
    if (3 > mix->cmdbRecycChain.cnt)
    {
        AfxPushLink(&cmdb->recyc, &mix->cmdbRecycChain);
        recycled = TRUE;
    }
    // If could not enqueue for recyclage, destroy it.
#endif

    if (!recycled)
    {
        //AfxExhaustArena(&cmdb->cmdArenaCompiled);
        AfxDismantleArena(&cmdb->cmdArenaCompiled);

        if (AfxReclaimPoolUnits(&mix->batches, 1, (void**)&cmdb))
        {
            AfxThrowError();
        }
        //AfxExhaustPool(&mix->batches, FALSE);
    }

    AfxUnlockFutex(&mix->cmdbReqLock, FALSE);
    return err;
}

_AMX afxBool AmxDoesMixCommandsExist(afxMixContext dctx, afxUnit batchId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &dctx);

    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return FALSE;
    }

    // AMX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&dctx->cmdbReqLock, FALSE);

    _amxCmdBatch* batch = _AmxGetCmdBatch(dctx, batchId);

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return !!batch;
}

_AMX afxError _AmxMixDtorCb(afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    while (AfxLoadAtom32(&mix->submCnt))
    {
        AfxYield();
    }

    AmxExhaustMixContext(mix, TRUE);

    AfxDismantleFutex(&mix->cmdbReqLock);
    AfxDeployChain(&mix->cmdbRecycChain, mix);

    AfxDismantleArena(&mix->cmdArena);
    AfxExhaustPool(&mix->batches, TRUE);

    AfxDismountClass(&mix->voxCls);

    return err;
}

_AMX afxError _AmxMixCtorCb(afxMixContext mix, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxMixConfig const* cfg = AFX_CAST(afxMixConfig const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    //afxUnit chanCnt = AFX_MAX(1, cfg->chanCnt);
    //afxUnit freq = AFX_MAX(1, cfg->freq);
    //mix->bps = cfg->bps ? cfg->bps : 32;
    //mix->latency = AFX_MAX(cfg->latency, 4096);
    //mix->freq = freq;
    //mix->chanCnt = chanCnt;

    mix->disabled = FALSE;

    mix->muted = FALSE;
    mix->routing = NIL;
    //mix->chanCnt = mix->chanCnt;
    //mix->latency = mix->latency;
    //mix->freq = mix->freq;

    mix->routing = NIL;

    mix->cmdbLockedForReq = FALSE;
    AfxDeployFutex(&mix->cmdbReqLock);
    AfxDeployChain(&mix->cmdbRecycChain, mix);

    AfxMakeArena(&mix->cmdArena, NIL, AfxHere());
    AfxDeployChain(&mix->commands, mix);

    mix->resetCb = _AmxMixResetCb;
    mix->endCb = _AmxMixEndCb;
    mix->state = amxMixState_RECORDING;

    mix->pimpl = NIL;// &_AMX_MCTX_DDI;

    AfxDeployPool(&mix->batches, sizeof(_amxCmdBatch), 3, 0);

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

    AfxMountClass(&mix->voxCls, NIL, NIL, &_AMX_VOX_CLASS_CONFIG);

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

_AMX afxError AmxAcquireMixContext(afxMixSystem msys, afxMixConfig const* cfg, afxMixContext* mixer)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(mixer);
    AFX_ASSERT(cfg);

    afxClass* cls = (afxClass*)_AmxMsysGetMixClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MIX);
    
    afxMixContext mix;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&mix, (void const*[]) { msys, cfg }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    AFX_ASSERT(mixer);
    *mixer = mix;
    return err;
}

_AMX afxError AmxRollMixContext(afxMixContext mix, afxUnit frameCnt)
{
    afxError err = AFX_ERR_NONE;
    // mctx must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    afxMixSystem msys = AfxGetProvider(mix);

    afxMixBridge mexu;
    AFX_ASSERT_RANGE(msys->bridgeCnt, /*ctrl->exuIdx*/0, 1);
    if (!AmxGetMixBridges(msys, /*ctrl->exuIdx*/0, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    if (_AmxMexuRollMixers(mexu, 0, 1, 1, 1, &mix))
        AfxThrowError();

    return err;
}
