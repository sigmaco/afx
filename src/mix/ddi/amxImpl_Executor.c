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

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>
// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AMX_MIX_C
#define _AMX_MIX_BRIDGE_C
#define _AMX_MIX_QUEUE_C
#define _AMX_MIX_SYSTEM_C
#define _AMX_BUFFER_C
#define _AMX_AUDIO_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_VOICE_C
#define _AMX_SINK_C
#define _AMX_TRACKER_C
#include "amxImplementation.h"

_AMX afxError _AmxSpuWork_CallbackCb(amxMpu* mpu, _amxIoReqPacket* work)
{
    afxError err = AFX_ERR_NONE;
    return work->Callback.f(mpu, work->Callback.udd);
}

_AMX afxError _AmxSpuWork_ExecuteCb(amxMpu* mpu, _amxIoReqPacket* work)
{
    afxError err = AFX_ERR_NONE;

    for (afxUnit i = 0; i < work->Execute.cmdbCnt; i++)
    {
        afxMixContext mix = work->Execute.cmdbs[i].mctx;
        AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
        //_SpuRollMixer(mpu, mix);
    }
    return err;
}

_AMX afxError _AmxSpuWork_SinkCb(amxMpu* mpu, _amxIoReqPacket* work)
{
    afxError err = AFX_ERR_NONE;

    //work->Sink.sink->pushCb(work->Sink.sink, work->Sink.buf, &work->Sink.seg);
    return err;
}

_AMX afxError _AmxSpuWork_RemapCb(amxMpu* mpu, _amxIoReqPacket* work)
{
    afxError err = AFX_ERR_NONE;

    //work->Remap.buf, work->Remap.off, work->Remap.ran, work->Remap.flags, work->Remap.placeholder)

    //AfxDisposeObjects(1, &work->Remap.);

    return err;
}

_AMX afxError _AmxSpuWork_TransferCb(amxMpu* mpu, _amxIoReqPacket* work)
{
    afxError err = AFX_ERR_NONE;

    switch (work->Transfer.srcFcc)
    {
    case NIL:
    {
        switch (work->Transfer.dstFcc)
        {
        case afxFcc_MBUF: // raw to mbuf
        {
            amxBuffer buf = work->Transfer.dst.buf;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxBufferIo const* op = &work->Transfer.bufOps[i];
                _AmxUpdateBuffer(buf, op, work->Transfer.src.src);
            }

            AfxDisposeObjects(1, &buf);

            break;
        }
        case afxFcc_AUD: // raw to wav
        {
            amxAudio aud = work->Transfer.dst.aud;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxAudioIo const* op = &work->Transfer.wavOps[i];
                _AmxUpdateAudio(aud, op, work->Transfer.src.src);
            }

            AfxDisposeObjects(1, &aud);

            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxFcc_AUD:
    {
        amxAudio aud = work->Transfer.src.aud;

        switch (work->Transfer.dstFcc)
        {
        case NIL: // wav to raw
        {
            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxAudioIo const* op = &work->Transfer.wavOps[i];
                _AmxDumpAudio(aud, op, work->Transfer.dst.dst);
            }

            AfxDisposeObjects(1, &aud);

            break;
        }
        case afxFcc_MBUF: // raw to mbuf
        {
            amxBuffer buf = work->Transfer.dst.buf;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxAudioIo const* op = &work->Transfer.wavOps[i];
                _AmxDumpAudio(aud, op, work->Transfer.dst.buf->storage[0].hostedAlloc.bytemap);
            }

            AfxDisposeObjects(1, &buf);

            break;
        }
        case afxFcc_IOB: // wav to iob
        {
            afxStream iob = work->Transfer.dst.iob;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxAudioIo const* op = &work->Transfer.wavOps[i];
                _AmxDownloadAudio(aud, op, iob);
            }

            AfxDisposeObjects(1, &iob);

            break;
        }
        default: AfxThrowError(); break;
        }
        
        AfxDisposeObjects(1, &aud);

        break;
    }
    case afxFcc_MBUF:
    {
        amxBuffer buf = work->Transfer.src.buf;

        switch (work->Transfer.dstFcc)
        {
        case NIL: // buf to raw
        {
            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxBufferIo const* op = &work->Transfer.bufOps[i];
                _AmxDumpBuffer(buf, op, work->Transfer.dst.dst);
            }

            break;
        }
        case afxFcc_MBUF: // raw to mbuf
        {
            amxBuffer buf2 = work->Transfer.dst.buf;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxBufferCopy const* op = &work->Transfer.bufCpyOps[i];
                _AmxCopyBuffer(buf2, op, buf);
            }

            AfxDisposeObjects(1, &buf2);

            break;
        }
        case afxFcc_AUD: // iob to wav
        {
            amxAudio aud = work->Transfer.dst.aud;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                //amxAudioIo const* op = &work->Transfer.wavOps[i];
                //_AmxUnpackAudio(aud, op, buf);
                AfxThrowError();
            }

            AfxDisposeObjects(1, &aud);

            break;
        }
        case afxFcc_IOB: // iob to buf
        {
            afxStream iob = work->Transfer.dst.iob;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxBufferIo const* op = &work->Transfer.bufOps[i];
                _AmxUploadBuffer(buf, op, iob);
            }

            AfxDisposeObjects(1, &iob);

            break;
        }
        default: AfxThrowError(); break;
        }

        AfxDisposeObjects(1, &buf);

        break;
    }
    case afxFcc_IOB:
    {
        afxStream iob = work->Transfer.src.iob;

        switch (work->Transfer.dstFcc)
        {
        case afxFcc_MBUF: // raw to mbuf
        {
            amxBuffer buf = work->Transfer.dst.buf;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxBufferIo const* op = &work->Transfer.bufOps[i];
                _AmxUploadBuffer(buf, op, iob);
            }

            AfxDisposeObjects(1, &buf);

            break;
        }
        case afxFcc_AUD: // iob to wav
        {
            amxAudio aud = work->Transfer.dst.aud;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxAudioIo const* op = &work->Transfer.wavOps[i];
                _AmxUploadAudio(aud, op, iob);
            }

            AfxDisposeObjects(1, &aud);

            break;
        }
        default: AfxThrowError(); break;
        }

        AfxDisposeObjects(1, &iob);

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMX _amxIoReqLut const _AMX_MPU_IORP_VMT =
{
    .Callback = _AmxSpuWork_CallbackCb,
    .Execute = _AmxSpuWork_ExecuteCb,
    .Sink = _AmxSpuWork_SinkCb,
    .Transfer = _AmxSpuWork_TransferCb,
    //.Remap = _AmxSpuWork_RemapCb,
};

_AMX afxBool _AmxMpu_ProcCb(amxMpu* mpu)
{
    afxError err = AFX_ERR_NONE;
    
    afxMixBridge mexu = mpu->mexu;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxMixSystem msys = AfxGetHost(mexu);

    amxTracker trax;
    AFX_ITERATE_CHAIN_B2F(AFX_OBJ(amxTracker), trax, executor, &msys->activeTrackers)
    {
        AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

        if (!trax->playing || trax->paused)
            continue;

        // step mixer
        afxReal64 curTime, lastTime;
        AfxGetThreadTime(&curTime, &lastTime);
        afxReal dt = curTime - trax->motor.localClock;
        trax->motor.localClock = curTime;
        trax->motor.dtLocalClockPending += dt;
#if 0
        arxSampleContext sampCtx;
        _SetUpSampleContext(&sampCtx, trax);
        mpu->sampCtx = sampCtx;
#endif

        afxSink sink = trax->sink;
        if (!sink) continue;

        //afxUnit frameCnt = audio_ringbuffer_available(&sink->rb);
        //afxUnit frameCnt = audio_ringbuffer_writable(&sink->rb);
        //frameCnt = AFX_MIN(frameCnt, sink->freq / 50); // 50Hz~ update; considering the 10ms of latency.
        //if (!frameCnt) continue;

        amxBufferedTrack room;
        if (!AmxLockSinkBuffer(sink, 0, 512, &room))
        {
            afxUnit i = 0;
            amxVoice vox;
            while (AfxEnumerateObjects(_AmxTraxGetVoxClass(trax), i++, 1, (afxObject*)&vox))
            {
                AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

                if (vox->playing2 && !vox->paused)
                {
                    //amxProcessVoice(vox, &trax->tracks[0].aud.aud->buf->storage[0].hostedAlloc.bytemap[sink->rb.capacity % audio_ringbuffer_writable(&sink->rb)], frameCnt);
                    amxProcessVoice(vox, (afxReal*)room.offset, room.frameCnt, 2);
                }
            }
            AmxUnlockSinkBuffer(sink, NIL);
        }
    };

    afxUnit j = 0;
    afxSink sink;
    afxClass* sinkCls = (afxClass*)_AmxMsysGetSinkClass(msys);
    while (AfxEnumerateObjects(sinkCls, j++, 1, (afxObject*)&sink))
    {
        AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
#if 0
        afxReal f[512*2] = { 1 };
        _AmxGenerateSquareWave(f, 512, 48000, 10, 10.0, 1.0);
        audio_ringbuffer_write(&sink->rb, f, 4, 512);
#endif   
#if 0
        afxUnit cnt = 512;
        while (cnt--)
        {
            sink->rb.write_pos++;
            // Overwrite oldest data if buffer is full
            if (sink->rb.write_pos - sink->rb.read_pos > sink->rb.capacity)
                sink->rb.read_pos = sink->rb.write_pos - sink->rb.capacity;
        }
#endif
        //sink->flushCb(sink);
        _AmxMexu_PingCb(mexu, 0);
    }

#if 0
    afxUnit j = 0;
    afxMixContext mix2;
    afxClass* mixCls = (afxClass*)_AmxMsysGetMixClass(AfxGetHost(mexu));
    while (AfxEnumerateObjects(mixCls, j++, 1, (afxObject*)&mix2))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix2);
        (mix2->state == amxMixState_PENDING);

        afxUnit k = 0;
        amxVoice vox;
        afxClass* voxCls = (afxClass*)_AmxMixGetVoxClass(mix2);
        while (AfxEnumerateObjects(voxCls, k++, 1, &vox))
        {
            AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

        }
    }
#endif
#if 0
    afxSink sink;
    afxUnit i = 0;
    while (AfxEnumerateObjects(_AmxMsysGetSinkClass(AfxGetHost(mexu)), i++, 1, (afxObject*)&sink))
    {
        if (sink->flushCb)
            sink->flushCb(sink);
    }
#endif
    _amxIoReqLut const* iorpVmt = mexu->iorpVmt;

    afxClass const* mqueCls = _AmxMexuGetMqueClass(mexu);
    afxUnit queCnt = mqueCls->instCnt;

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxMixQueue mque;
        if (!AmxGetMixQueues(mexu, queIdx, 1, &mque) || !mque)
        {
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

        if (AfxTryLockMutex(&mque->iorpChnMtx))
        {
            _amxIoReqPacket* iorp;
            AFX_ITERATE_CHAIN_B2F(_amxIoReqPacket, iorp, hdr.chain, &mque->iorpChn)
            {
                AFX_ASSERT(mque->iorpChn.cnt);
                AfxGetTime(&iorp->hdr.pullTime);

                iorpVmt->f[iorp->hdr.id](mpu, iorp);

                AfxGetTime(&iorp->hdr.complTime);
                _AmxMquePopIoReqPacket(mque, iorp);
            }
            AfxSignalCondition(&mque->idleCnd);
            AfxUnlockMutex(&mque->iorpChnMtx);
        }
    }
    //_AmxMexu_PingCb(mexu, 0);
    return TRUE;
}

_AMX afxInt _AMX_MPU_THREAD_PROC(afxMixBridge mexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == mexu->worker);

    afxUnit exuId;
    afxMixSystem msys = AmxGetBridgedMixSystem(mexu, &exuId);
    //afxUnit portId = mexu->portId;

    amxMpu* mpu;
    if (AfxAllocate(sizeof(*mpu), 0, AfxHere(), (void**)&mpu))
        AfxThrowError();

    AfxZero(mpu, sizeof(*mpu));
    mpu->mexu = mexu;
    mpu->msys = msys;
    mpu->exuIdx = exuId;
    mpu->running = TRUE;
    mpu->thr = thr;

    amxAudioInfo wavi = { 0 };
    wavi.freq = 48000;
    wavi.segCnt = 1;
    wavi.chanCnt = 1;
    wavi.sampCnt = wavi.freq;
    wavi.fmt = amxFormat_M32f;
    AmxAcquireAudios(msys, 1, &wavi, &mpu->a);
    AmxAcquireAudios(msys, 1, &wavi, &mpu->b);

    mpu->samplesPerFrame = wavi.freq / 60;

    AfxReportf(6, NIL, "Mix Execution Unit %.03.", exuId);
    
    mexu->schedCnt = 1;

    do
    {
#if !0
        AfxLockMutex(&mexu->schedCndMtx);

        while (!mexu->schedCnt)
            AfxWaitTimedCondition(&mexu->schedCnd, &mexu->schedCndMtx, (afxTimeSpec[]) { { .nsecs = AFX_NSECS_PER_MSEC(16) } });

        mexu->schedCnt = 0;
        AfxUnlockMutex(&mexu->schedCndMtx);
#endif

        mexu->procCb(mpu);
        
        //_ZalProcessDeletionQueue(al, &mexu->deletionQueue);

        if (AfxShouldThreadBeInterrupted())
            break;

    } while (1);

    AfxDisposeObjects(1, &mpu->a);
    AfxDisposeObjects(1, &mpu->b);

    AFX_ASSERT(mpu == mexu->mpu);
    AfxDeallocate((void**)&mpu, AfxHere());

    return 0;
}

_AMX afxError _AmxMexu_PingCb(afxMixBridge mexu, afxUnit queIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    AfxLockMutex(&mexu->schedCndMtx);
    mexu->schedCnt += 1;
    AfxSignalCondition(&mexu->schedCnd);
    AfxUnlockMutex(&mexu->schedCndMtx);
    return err;
}

_AMX afxError _AmxMqueSinkBuffers(afxMixQueue mque, afxUnit cnt, amxFlush presentations[])
{
    afxError err = AFX_ERR_NONE;

    for (afxUnit i = 0; i < cnt; i++)
    {
        amxFlush* pres = &presentations[i];

        afxSink sink = NIL;// pres->sink;
        AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);

    }
    return err;
}
