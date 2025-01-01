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

#define _ASX_SOUND_C
#define _ASX_SOUND_BRIDGE_C
#define _ASX_SOUND_QUEUE_C
#define _ASX_BUFFER_C
#define _ASX_WAVEFORM_C
#define _ASX_MIXER_C
#define _ASX_VOICE_C
#define _ASX_SINK_C
#include "asxImplementation.h"

_ASX afxError _AsxSpuWork_CallbackCb(asxSpu* spu, asxWork* work)
{
    afxError err = AFX_ERR_NONE;
    work->Callback.f(spu, work->Callback.udd);
    return err;
}

_ASX afxError _AsxSpuWork_ExecuteCb(asxSpu* spu, asxWork* work)
{
    afxError err = AFX_ERR_NONE;

    for (afxUnit i = 0; i < work->Execute.mixCnt; i++)
    {
        afxMixer mix = work->Execute.mixers[i];
        AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
        //_SpuRollMixer(spu, mix);
    }
    return err;
}

_ASX afxError _AsxSpuWork_SinkCb(asxSpu* spu, asxWork* work)
{
    afxError err = AFX_ERR_NONE;

    work->Sink.sink->pushCb(work->Sink.sink, work->Sink.buf, &work->Sink.seg);
    return err;
}

_ASX afxError _AsxSpuWork_RemapCb(asxSpu* spu, asxWork* work)
{
    afxError err = AFX_ERR_NONE;

    //work->Remap.buf, work->Remap.off, work->Remap.ran, work->Remap.flags, work->Remap.placeholder)

    AfxDisposeObjects(1, &work->Remap.wav);

    return err;
}

_ASX afxError _AsxSpuWork_TransferCb(asxSpu* spu, asxWork* work)
{
    afxError err = AFX_ERR_NONE;

    switch (work->Transfer.srcFcc)
    {
    case NIL:
    {
        switch (work->Transfer.dstFcc)
        {
        case afxFcc_WAV: // raw to wav
        {
            afxAudio wav = work->Transfer.dst.wav;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                afxWaveIo const* op = &work->Transfer.wavOps[i];
                _AsxUpdateAudio(wav, op, work->Transfer.src.src);
            }
            AfxDisposeObjects(1, &wav);
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxFcc_WAV:
    {
        switch (work->Transfer.dstFcc)
        {
        case NIL: // wav to raw
        {
            afxAudio wav = work->Transfer.src.wav;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                afxWaveIo const* op = &work->Transfer.wavOps[i];
                _AsxDumpAudio(wav, op, work->Transfer.dst.dst);
            }
            AfxDisposeObjects(1, &wav);
            break;
        }
        case afxFcc_IOB: // wav to iob
        {
            afxStream iob = work->Transfer.dst.iob;
            afxAudio wav = work->Transfer.src.wav;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                afxWaveIo const* op = &work->Transfer.wavOps[i];
                _AsxDownloadAudio(wav, op, iob);
            }
            AfxDisposeObjects(1, &wav);
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
        case afxFcc_WAV: // iob to wav
        {
            afxAudio wav = work->Transfer.dst.wav;
            afxStream iob = work->Transfer.src.iob;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                afxWaveIo const* op = &work->Transfer.wavOps[i];
                _AsxUploadAudio(wav, op, iob);
            }

            AfxDisposeObjects(1, &wav);
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

_ASX asxWorkList const _ASX_SPU_WORK_VMT =
{
    .Callback = _AsxSpuWork_CallbackCb,
    .Execute = _AsxSpuWork_ExecuteCb,
    .Sink = _AsxSpuWork_SinkCb,
    .Transfer = _AsxSpuWork_TransferCb,
    .Remap = _AsxSpuWork_RemapCb,
};

_ASX afxBool _AsxSpu_ProcCb(asxSpu* spu)
{
    afxError err = AFX_ERR_NONE;
    
    afxSoundBridge sexu = spu->sexu;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    
    afxMixer mix;
    AfxChainForEveryLinkageB2F(&sexu->activeMixers, AFX_OBJ(afxMixer), executor, mix)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
        _AsxSpuRollMixers(spu, mix);
    };

    afxSink sink;
    afxUnit i = 0;
    while (AfxEnumerateObjects(AsxGetAudioSinkClass(AfxGetProvider(sexu)), i++, 1, (afxObject*)&sink))
    {
        sink->flushCb(sink);
    }

    asxWorkList const* workVmt = sexu->workVmt;

    afxClass const* squeCls = _AsxGetSoundQueueClass(sexu);
    afxUnit queCnt = squeCls->instCnt;

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxSoundQueue sque;
        if (!AfxGetSoundQueues(sexu, queIdx, 1, &sque) || !sque)
        {
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

        if (AfxTryLockMutex(&sque->workChnMtx))
        {
            asxWork* work;
            AfxChainForEveryLinkageB2F(&sque->workChn, asxWork, hdr.chain, work)
            {
                AFX_ASSERT(sque->workChn.cnt);
                AfxGetTime(&work->hdr.pullTime);

                workVmt->f[work->hdr.id](spu, work);

                AfxGetTime(&work->hdr.complTime);
                _AsxSquePopWork(sque, work);
            }
            AfxUnlockMutex(&sque->workChnMtx);
            AfxSignalCondition(&sque->idleCnd);
        }
    }
    return TRUE;
}

_ASX afxInt _ASX_SPU_THREAD_PROC(afxSoundBridge sexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == sexu->worker);

    afxSoundSystem ssys = AfxGetSoundBridgeSystem(sexu);
    afxUnit portId = sexu->portId;

    asxSpu* spu;
    if (AfxAllocate(sizeof(*spu), 0, AfxHere(), (void**)&spu))
        AfxThrowError();

    AfxZero(spu, sizeof(*spu));
    spu->sexu = sexu;
    spu->ssys = ssys;
    spu->exuIdx = portId;
    spu->running = TRUE;
    spu->thr = thr;

    afxAudioInfo wavi = { 0 };
    wavi.freq = 48000;
    wavi.chanCnt = 1;
    wavi.sampCnt = wavi.freq / 60;
    wavi.fmt = asxFormat_A32f;
    AfxAcquireAudios(ssys, 1, &wavi, &spu->a);
    AfxAcquireAudios(ssys, 1, &wavi, &spu->b);

    spu->samplesPerFrame = wavi.freq / 60;

    AfxDbgLogf(6, NIL, "Sound Execution Unit %.03.", portId);

    do
    {
#if 0
        AfxLockMutex(&sexu->schedCndMtx);

        while (!sexu->schedCnt)
            AfxWaitCondition(&sexu->schedCnd, &sexu->schedCndMtx);

        sexu->schedCnt = 0;
        AfxUnlockMutex(&sexu->schedCndMtx);
#endif

        sexu->procCb(spu);
        
        AfxSleep(1);

        //_ZalProcessDeletionQueue(al, &sexu->deletionQueue);

        if (AfxShouldThreadBeInterrupted())
            break;

    } while (1);

    AfxDisposeObjects(1, &spu->a);
    AfxDisposeObjects(1, &spu->b);

    AFX_ASSERT(spu == sexu->spu);
    AfxDeallocate((void**)&spu, AfxHere());

    return 0;
}

_ASX afxError _AsxSexu_PingCb(afxSoundBridge sexu, afxUnit queIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    sexu->schedCnt += 1;
    AfxSignalCondition(&sexu->schedCnd);
    return err;
}
