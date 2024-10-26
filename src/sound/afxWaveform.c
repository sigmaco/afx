/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _ASX_SOUND_C
#define _ASX_BUFFER_C
#define _ASX_WAVEFORM_C
#include "../dev/AsxImplKit.h"

_ASXINL afxSoundContext AfxGetWaveContext(afxWaveform wav)
{
    afxError err;
    AfxAssertObjects(1, &wav, afxFcc_WAV);
    afxSoundContext sctx = AfxGetProvider(wav);
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    return sctx;
}

_ASXINL void* AfxGetWaveUdd(afxWaveform wav)
{
    afxError err;
    AfxAssertObjects(1, &wav, afxFcc_WAV);
    return wav->udd;
}

_ASXINL afxUnit AfxCountWaveChannels(afxWaveform wav)
{
    afxError err;
    AfxAssertObjects(1, &wav, afxFcc_WAV);
    return wav->chanCnt;
}

_ASXINL afxUnit AfxGetWaveLength(afxWaveform wav)
{
    afxError err;
    AfxAssertObjects(1, &wav, afxFcc_WAV);
    return wav->sampleCnt;
}

_ASXINL afxReal AfxGetWaveFrequency(afxWaveform wav)
{
    afxError err;
    AfxAssertObjects(1, &wav, afxFcc_WAV);
    return wav->freq;
}

_ASXINL afxReal AfxGetWaveDuration(afxWaveform wav)
{
    afxError err;
    AfxAssertObjects(1, &wav, afxFcc_WAV);
    return wav->sampleCnt / wav->freq;
}

_ASX afxError _AsxWavStdDtorCb(afxWaveform wav)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wav, afxFcc_WAV);

    afxSoundContext sctx = AfxGetWaveContext(wav);

    if (wav->mappedRange)
    {
        //AfxUnmapBuffer(wav, TRUE);
        AFX_ASSERT(!wav->mappedRange);
    }

    if (wav->bytemap)
        AfxDeallocate(wav->bytemap);

    return err;
}

_ASX afxError _AsxWavStdCtorCb(afxWaveform wav, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &wav, afxFcc_WAV);

    afxSoundContext sctx = AfxGetWaveContext(wav);
    asxWaveInfo const *spec = ((asxWaveInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(spec);
    
    afxUnit usage = 1;
    afxUnit siz = spec->bps * spec->sampleCnt * spec->freq * spec->chanCnt;

    if (!siz) AfxThrowError();
    else if (!usage) AfxThrowError();
    else
    {
        wav->bps = spec->bps;
        wav->bufCap = AFX_ALIGNED_SIZE(siz, AFX_SIMD_ALIGNMENT);

        wav->udd = spec->udd;

        wav->bytemap = AfxAllocate(1, siz, 0, AfxHere());
        wav->mappedOffset = 0;
        wav->mappedRange = 0;
        wav->mappedFlags = NIL;

        if (spec->src)
        {
            AfxCopy(wav->bytemap, spec->src, siz);
        }
    }
    return err;
}

_ASX afxClassConfig const _AsxWavStdImplementation =
{
    .fcc = afxFcc_WAV,
    .name = "Waveform",
    .desc = "Waveform",
    .fixedSiz = sizeof(AFX_OBJECT(afxWaveform)),
    .ctor = (void*)_AsxWavStdCtorCb,
    .dtor = (void*)_AsxWavStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AsxAcquireWaves(afxSoundContext sctx, afxUnit cnt, asxWaveInfo const info[], afxWaveform buffers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AFX_ASSERT(buffers);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)AfxGetWaveformClass(sctx);
    AfxAssertClass(cls, afxFcc_WAV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { sctx, (void*)info }))
        AfxThrowError();

    return err;
}

_ASX afxError AfxUpdateWave(afxWaveform wav, afxUnit portIdx, afxUnit opCnt, afxWaveIo const ops[], void const* src)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &wav, afxFcc_WAV);

    afxSoundBridge sdge;
    afxSoundContext sctx = AfxGetWaveContext(wav);
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    if (!AfxGetSoundBridge(sctx, portIdx, &sdge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        asxTransference transfer = { 0 };

        transfer.dstFcc = afxFcc_WAV;
        transfer.snd.wav = wav;
        transfer.snd.src = src;

#if _AFX_DEBUG
        for (afxUnit i = 0; i < opCnt; i++)
        {
            afxWaveIo const* op = &ops[i];
            //AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
            //AfxAssertRangeWhd(ras->whd, op->rgn.origin, op->rgn.whd);
        }
#endif

        afxUnit queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AsxSubmitTransferences(sdge, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AFX_ASSERT(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForIdleSoundQueue(sctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_ASX afxError AsxLoadWaves(afxSoundContext sctx, afxUnit cnt, afxUri const uris[], afxWaveform buffers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AFX_ASSERT(buffers);
    AFX_ASSERT(uris);
    AFX_ASSERT(cnt);

    for (afxUnit fIdx = 0; fIdx < cnt; fIdx++)
    {
        afxStream iob;
        afxStreamInfo iobi = { 0 };
        iobi.usage = afxStreamUsage_FILE;
        iobi.flags = afxStreamFlag_READABLE;
        AfxAcquireStream(1, &iobi, &iob);
        AfxReloadFile(iob, &uris[fIdx]);

        AfxSeekStream(iob, 22, afxSeekOrigin_BEGIN);
        afxUnit16 chCnt;
        AfxReadStream(iob, sizeof(chCnt), 0, &chCnt);
        afxUnit32 sampRate;
        AfxReadStream(iob, sizeof(sampRate), 0, &sampRate);
        AfxSeekStream(iob, 34, afxSeekOrigin_BEGIN);
        afxUnit16 bps;
        AfxReadStream(iob, sizeof(bps), 0, &bps);
        AfxSeekStream(iob, 40, afxSeekOrigin_BEGIN);
        afxUnit32 dataSiz;
        AfxReadStream(iob, sizeof(dataSiz), 0, &dataSiz);

        asxWaveInfo wavi = { 0 };
        wavi.bps = bps;
        wavi.chanCnt = chCnt;
        wavi.freq = sampRate;
        wavi.sampleCnt = sampRate * bps / sampRate;
        
        if (AsxAcquireWaves(sctx, 1, &wavi, buffers))
            AfxThrowError();

        //AfxReadStream(iob, dataSiz, 0, dst);

        afxWaveIo op = { 0 };

        //AfxUpdateWave(sctx, 0, 1, &op, iob);
    }
    return err;
}
