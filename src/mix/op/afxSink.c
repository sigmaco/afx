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
#include "../impl/amxImplementation.h"

_AMX afxMixSystem AfxGetAudioSinkContext(afxSink sink)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
    afxMixSystem msys = AfxGetProvider(sink);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys;
}

_AMX afxMixDevice AfxGetAudioSinkDevice(afxSink sink)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
    afxMixSystem msys = AfxGetAudioSinkContext(sink);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxMixDevice mdev = AfxGetProvider(sink);
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    return mdev;
}

_AMX afxError AfxGetAudioSinkIdd(afxSink sink, afxUnit code, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
    sink->getIddCb(sink, code, dst);
    return err;
}

_AMX afxError AfxMuteAudioSink(afxSink sink, afxBool mute)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);

    sink->muteReqCnt += !!mute;

    return err;
}

_AMX afxError AfxRequestSinkBuffer(afxSink sink, afxTime timeout, afxUnit *bufIdx)
// Pull an available sink buffer
{
    afxError err = AFX_ERR_NONE;
    // sink must be a valid afxSink handle.
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
    afxUnit bufIdx2 = AFX_INVALID_INDEX;

#if 0
    if (sink->lockCb)
    {
        if (!(err = sink->lockCb(sink, timeout, &bufIdx2)))
        {
            AFX_ASSERT(AFX_INVALID_INDEX != bufIdx2);
            AFX_ASSERT_RANGE(sink->latency, bufIdx2, 1);
        }
    }
    else
#endif
    {
        afxBool success = FALSE;
        afxTime time, t2;
        AfxGetTime(&time);

        afxClock start, last;

        if (timeout)
        {
            AfxGetClock(&start);
            last = start;
        }

        while (1)
        {
            afxUnit lockedBufIdx = AFX_INVALID_INDEX;

            if (AfxPopInterlockedQueue(&sink->freeBuffers, &lockedBufIdx))
            {
                amxAudio wav = sink->buffers[lockedBufIdx];

                if (wav)
                {
                    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &wav);
                }
                bufIdx2 = lockedBufIdx;
                success = TRUE;
            }

            if (success)
                break;

            if (!success && (!timeout || timeout < AfxGetTime(&t2) - time))
            {
                err = __LINE__;
                bufIdx2 = AFX_INVALID_INDEX;
                break;
            }
        }
    }
    AFX_ASSERT(bufIdx);
    *bufIdx = bufIdx2;
    return err;
}

_AMX afxError AfxDiscardSinkBuffer(afxSink sink, afxUnit bufIdx)
{
    afxError err = AFX_ERR_NONE;
    // sink must be a valid afxSink handle.
    AFX_ASSERT_OBJECTS(afxFcc_ASI, 1, &sink);
    AFX_ASSERT_RANGE(sink->latency, bufIdx, 1);

    if (bufIdx < sink->latency)
        AfxPushInterlockedQueue(&sink->freeBuffers, (afxUnit[]) { bufIdx });
    else
        AfxThrowError();

    return err;
}

_AMX afxError _AmxAsioDtorCb(afxSink sink)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
    afxMixDevice mdev = AfxGetAudioSinkDevice(sink);
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    AfxDismantleInterlockedQueue(&sink->freeBuffers);
    AfxDismantleInterlockedQueue(&sink->readyBuffers);

    // Dispose all acquired buffer objects.
    AfxDisposeObjects(sink->latency, sink->buffers);

    afxObjectStash const stashs[] =
    {
        {
            .cnt = sink->latency,
            .siz = sizeof(sink->buffers[0]),
            .var = (void**)&sink->buffers
        }
    };
    AfxDeallocateInstanceData(sink, ARRAY_SIZE(stashs), stashs);

    if (sink->iddDtorCb)
        sink->iddDtorCb(sink);

    AFX_ASSERT(!sink->idd);

    return err;
}

_AMX afxError _AmxAsioCtorCb(afxSink sink, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;

    AfxZero(sink, sizeof(sink[0]));

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxSinkConfig const* cfg = ((afxSinkConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    
    sink->udd = cfg->udd;

    afxSinkConfig def;
    AfxConfigureAudioSink(msys, &def);

    sink->fmt = cfg->fmt ? cfg->fmt : def.fmt;
    sink->chanCnt = cfg->chanCnt ? cfg->chanCnt : def.chanCnt;
    sink->freq = cfg->freq ? cfg->freq : def.freq;
    sink->samplesPerFrame = cfg->samplesPerFrame ? cfg->samplesPerFrame : def.samplesPerFrame;
    sink->latency = cfg->latency ? cfg->latency : def.latency;

    afxObjectStash const stashs[] =
    {
        {
            .cnt = sink->latency,
            .siz = sizeof(sink->buffers[0]),
            .var = (void**)&sink->buffers
        }
    };

    if (AfxAllocateInstanceData(sink, ARRAY_SIZE(stashs), stashs)) AfxThrowError();
    else
    {
        AfxZero(sink->buffers, sizeof(sink->buffers[0]) * sink->latency);

        AfxDeployInterlockedQueue(&sink->freeBuffers, sizeof(afxUnit), sink->latency);
        AfxDeployInterlockedQueue(&sink->readyBuffers, sizeof(afxUnit), sink->latency);

        amxAudioInfo audi = { 0 };
        audi.chanCnt = sink->chanCnt;
        audi.fmt = sink->fmt;
        audi.freq = sink->freq;
        audi.sampCnt = sink->samplesPerFrame;
        audi.udd = sink;

        for (afxUnit i = 0; i < sink->latency; i++)
        {
            if (AmxAcquireAudios(msys, 1, &audi, &sink->buffers[i]))
            {
                AfxThrowError();
                // Dispose all objects acquire up to this iteration.
                AfxDisposeObjects(i, sink->buffers);
            }

            // Enqueue our buffer into the queue of disponible buffers.
            AfxPushInterlockedQueue(&sink->freeBuffers, (afxUnit[]) { i });
        }

        if (err)
            AfxDeallocateInstanceData(sink, ARRAY_SIZE(stashs), stashs);
    }
    return err;
}

_AMX afxClassConfig const _AMX_ASIO_CLASS_CONFIG =
{
    .fcc = afxFcc_ASIO,
    .name = "Sink",
    .desc = "Audio Stream Input/Output",
    .fixedSiz = sizeof(AFX_OBJECT(afxSink)),
    .ctor = (void*)_AmxAsioCtorCb,
    .dtor = (void*)_AmxAsioDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxConfigureAudioSink(afxMixSystem msys, afxSinkConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(cfg);
    *cfg = (afxSinkConfig) { 0 };
    cfg->fmt = amxFormat_S16i;
    cfg->freq = 48000; // DVD
    cfg->chanCnt = 2; // stereo
    cfg->samplesPerFrame = cfg->freq / 60;
    cfg->latency = 6;
    return err;
}

_AMX afxError AfxOpenAudioSink(afxMixSystem msys, afxSinkConfig const* cfg, afxSink* sink)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(cfg);

    afxClass* cls = (afxClass*)_AmxGetAudioSinkClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_ASIO);
    afxSink snk;
    afxBool record = FALSE;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&snk, (void const*[]) { msys, cfg, &record }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &snk);
    AFX_ASSERT(sink);
    *sink = snk;
    return err;
}
