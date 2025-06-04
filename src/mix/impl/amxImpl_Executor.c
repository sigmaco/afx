/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                     Q W A D R O   S O U N D   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>

#define _AMX_MIX_C
#define _AMX_MIX_BRIDGE_C
#define _AMX_MIX_QUEUE_C
#define _AMX_BUFFER_C
#define _AMX_AUDIO_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_VOICE_C
#define _AMX_SINK_C
#include "amxImplementation.h"

_AMX afxError _AmxSpuWork_CallbackCb(amxMpu* mpu, _amxIoReqPacket* work)
{
    afxError err = AFX_ERR_NONE;
    work->Callback.f(mpu, work->Callback.udd);
    return err;
}

_AMX afxError _AmxSpuWork_ExecuteCb(amxMpu* mpu, _amxIoReqPacket* work)
{
    afxError err = AFX_ERR_NONE;

    for (afxUnit i = 0; i < work->Execute.mixCnt; i++)
    {
        afxMixContext mix = work->Execute.mixers[i];
        AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
        //_SpuRollMixer(mpu, mix);
    }
    return err;
}

_AMX afxError _AmxSpuWork_SinkCb(amxMpu* mpu, _amxIoReqPacket* work)
{
    afxError err = AFX_ERR_NONE;

    work->Sink.sink->pushCb(work->Sink.sink, work->Sink.buf, &work->Sink.seg);
    return err;
}

_AMX afxError _AmxSpuWork_RemapCb(amxMpu* mpu, _amxIoReqPacket* work)
{
    afxError err = AFX_ERR_NONE;

    //work->Remap.buf, work->Remap.off, work->Remap.ran, work->Remap.flags, work->Remap.placeholder)

    AfxDisposeObjects(1, &work->Remap.wav);

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
        switch (work->Transfer.dstFcc)
        {
        case NIL: // wav to raw
        {
            amxAudio aud = work->Transfer.src.aud;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxAudioIo const* op = &work->Transfer.wavOps[i];
                _AmxDumpAudio(aud, op, work->Transfer.dst.dst);
            }
            AfxDisposeObjects(1, &aud);
            break;
        }
        case afxFcc_IOB: // wav to iob
        {
            afxStream iob = work->Transfer.dst.iob;
            amxAudio aud = work->Transfer.src.aud;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxAudioIo const* op = &work->Transfer.wavOps[i];
                _AmxDownloadAudio(aud, op, iob);
            }
            AfxDisposeObjects(1, &aud);
            AfxDisposeObjects(1, &iob);
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxFcc_IOB:
    {
        switch (work->Transfer.dstFcc)
        {
        case afxFcc_AUD: // iob to wav
        {
            amxAudio aud = work->Transfer.dst.aud;
            afxStream iob = work->Transfer.src.iob;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                amxAudioIo const* op = &work->Transfer.wavOps[i];
                _AmxUploadAudio(aud, op, iob);
            }

            AfxDisposeObjects(1, &aud);
            AfxDisposeObjects(1, &iob);
            break;
        }
        default: AfxThrowError(); break;
        }
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
    .Remap = _AmxSpuWork_RemapCb,
};

_AMX afxBool _AmxMpu_ProcCb(amxMpu* mpu)
{
    afxError err = AFX_ERR_NONE;
    
    afxMixBridge mexu = mpu->mexu;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    
    afxMixContext mix;
    AFX_ITERATE_CHAIN_B2F(AFX_OBJ(afxMixContext), mix, executor, &mexu->activeMixers)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
        _AmxMpuRollMixContexts(mpu, mix);
    };

    afxSink sink;
    afxUnit i = 0;
    while (AfxEnumerateObjects(_AmxGetAudioSinkClass(AfxGetProvider(mexu)), i++, 1, (afxObject*)&sink))
    {
        if (sink->flushCb)
            sink->flushCb(sink);
    }

    _amxIoReqLut const* iorpVmt = mexu->iorpVmt;

    afxClass const* mqueCls = _AmxGetMixQueueClass(mexu);
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
    return TRUE;
}

_AMX afxInt _AMX_MPU_THREAD_PROC(afxMixBridge mexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == mexu->worker);

    afxMixSystem msys = AmxGetBridgedMixSystem(mexu);
    afxUnit portId = mexu->portId;

    amxMpu* mpu;
    if (AfxAllocate(sizeof(*mpu), 0, AfxHere(), (void**)&mpu))
        AfxThrowError();

    AfxZero(mpu, sizeof(*mpu));
    mpu->mexu = mexu;
    mpu->msys = msys;
    mpu->exuIdx = portId;
    mpu->running = TRUE;
    mpu->thr = thr;

    amxAudioInfo wavi = { 0 };
    wavi.freq = 48000;
    wavi.chanCnt = 1;
    wavi.sampCnt = wavi.freq / 60;
    wavi.fmt = amxFormat_M32f;
    AmxAcquireAudios(msys, 1, &wavi, &mpu->a);
    AmxAcquireAudios(msys, 1, &wavi, &mpu->b);

    mpu->samplesPerFrame = wavi.freq / 60;

    AfxReportf(6, NIL, "Sound Execution Unit %.03.", portId);

    do
    {
#if 0
        AfxLockMutex(&mexu->schedCndMtx);

        while (!mexu->schedCnt)
            AfxWaitCondition(&mexu->schedCnd, &mexu->schedCndMtx);

        mexu->schedCnt = 0;
        AfxUnlockMutex(&mexu->schedCndMtx);
#endif

        mexu->procCb(mpu);
        
        AfxSleep(1);

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
    mexu->schedCnt += 1;
    AfxSignalCondition(&mexu->schedCnd);
    return err;
}
