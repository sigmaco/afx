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
    afxSoundContext sctx = AfxGetParent(wav);
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    return sctx;
}

_ASXINL void* AfxGetWaveUdd(afxWaveform wav)
{
    afxError err;
    AfxAssertObjects(1, &wav, afxFcc_WAV);
    return wav->udd;
}

_ASXINL afxNat AfxCountWaveChannels(afxWaveform wav)
{
    afxError err;
    AfxAssertObjects(1, &wav, afxFcc_WAV);
    return wav->chanCnt;
}

_ASXINL afxNat AfxGetWaveLength(afxWaveform wav)
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
        AfxAssert(!wav->mappedRange);
    }

    if (wav->bytemap)
        AfxDeallocate(wav->bytemap);

    return err;
}

_ASX afxError _AsxWavStdCtorCb(afxWaveform wav, void** args, afxNat invokeNo)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &wav, afxFcc_WAV);

    afxSoundContext sctx = AfxGetWaveContext(wav);
    asxWaveInfo const *spec = ((asxWaveInfo const *)args[1]) + invokeNo;
    AfxAssert(spec);
    
    afxNat usage = 1;
    afxNat siz = spec->bps * spec->sampleCnt * spec->freq * spec->chanCnt;

    if (!siz) AfxThrowError();
    else if (!usage) AfxThrowError();
    else
    {
        wav->bps = spec->bps;
        wav->bufCap = AFX_ALIGNED_SIZEOF(siz, AFX_SIMD_ALIGN);

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

_ASX afxError AsxAcquireWaves(afxSoundContext sctx, afxNat cnt, asxWaveInfo const info[], afxWaveform buffers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AfxAssert(buffers);
    AfxAssert(info);
    AfxAssert(cnt);

    afxClass* cls = (afxClass*)AfxGetWaveformClass(sctx);
    AfxAssertClass(cls, afxFcc_WAV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { sctx, (void*)info }))
        AfxThrowError();

    return err;
}

_ASX afxError AfxUpdateWave(afxWaveform wav, afxNat portIdx, afxNat opCnt, afxWaveIo const ops[], void const* src)
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
        for (afxNat i = 0; i < opCnt; i++)
        {
            afxWaveIo const* op = &ops[i];
            //AfxAssertRange(ras->lodCnt, op->rgn.lodIdx, 1);
            //AfxAssertRangeWhd(ras->whd, op->rgn.origin, op->rgn.whd);
        }
#endif

        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AsxSubmitTransferences(sdge, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForIdleSoundQueue(sctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_ASX afxError AsxLoadWaves(afxSoundContext sctx, afxNat cnt, afxUri const uris[], afxWaveform buffers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AfxAssert(buffers);
    AfxAssert(uris);
    AfxAssert(cnt);

    for (afxNat fIdx = 0; fIdx < cnt; fIdx++)
    {
        afxStream iob;
        AfxLoadFile(&uris[fIdx], &iob);

        AfxSeekStream(iob, 22, afxSeekMode_ABSOLUTE);
        afxNat16 chCnt;
        AfxReadStream(iob, sizeof(chCnt), 0, &chCnt);
        afxNat32 sampRate;
        AfxReadStream(iob, sizeof(sampRate), 0, &sampRate);
        AfxSeekStream(iob, 34, afxSeekMode_ABSOLUTE);
        afxNat16 bps;
        AfxReadStream(iob, sizeof(bps), 0, &bps);
        AfxSeekStream(iob, 40, afxSeekMode_ABSOLUTE);
        afxNat32 dataSiz;
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