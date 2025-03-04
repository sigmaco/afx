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

#define _AMX_MIX_C
#define _AMX_BUFFER_C
#define _AMX_WAVEFORM_C
#include "../impl/amxImplementation.h"

 // WAV header structure
#pragma pack(push, 1)
typedef struct
{
    afxChar chunkFcc[4]; // "RIFF"
    afxUnit32 chunkSiz; // 36 + SubChunk2Size
    afxChar waveFcc[4]; // "WAVE"
    afxChar fmtFcc[4]; // "fmt "
    afxUnit32 subchunk1Siz; // 16 for PCM
    afxUnit16 fmt; // PCM = 1
    afxUnit16 chanCnt; // Mono or Stereo
    afxUnit32 freq; // 44100, 48000, etc.
    afxUnit32 byteRate; // (freq * chanCnt * bitsPerSample) / 8
    afxUnit16 blockAlign; // (chanCnt * bitsPerSample) / 8
    afxUnit16 bitsPerSample; // 8, 16, 24, or 32
    afxChar data[4];         // "data"
    afxUnit32 dataSiz; // (num_samples * chanCnt * bitsPerSample) / 8
} WAVHeader;
#pragma pack(pop)

_AMXINL afxMixSystem AfxGetAudioContext(afxAudio wav)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);
    afxMixSystem msys = AfxGetProvider(wav);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys;
}

_AMXINL void AfxDescribeAudio(afxAudio wav, afxAudioInfo* desc)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);
    AFX_ASSERT(desc);
    desc->fmt = wav->fmt;
    desc->chanCnt = wav->chanCnt;
    desc->freq = wav->freq;
    desc->sampCnt = wav->sampCnt;
    //desc->frameCnt = wav->frameCnt;
    desc->udd = wav->udd;
}

_AMXINL afxReal AfxGetWaveDuration(afxAudio wav)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);
    return wav->sampCnt / wav->freq;
}

_AMX void _AmxSanitizeWaveInterval(afxAudio wav, afxWaveInterval const* raw, afxWaveInterval* san)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);

    /*
        This code sanitizes a afxWaveInterval struct to be within the bounds of the audio objects.
    */

    san->baseChan = AfxMin(raw->baseChan, wav->chanCnt - 1);
    san->baseSamp = AfxMin(raw->baseSamp, wav->sampCnt - 1);

    san->chanCnt = AfxClamp(raw->chanCnt, 1, wav->chanCnt - san->baseChan);
    san->sampCnt = AfxClamp(raw->sampCnt, 1, wav->sampCnt - san->baseSamp);
}

_AMX void _AmxSanitizeWaveCopy(afxAudio src, afxAudio dst, afxWaveCopy const* raw, afxWaveCopy* san)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &dst);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);

    /*
        This code sanitizes a afxWaveCopy struct to be within the bounds of the audio objects.
    */

    san->dstBaseChan = AfxMin(raw->dstBaseChan, src->chanCnt - 1);
    san->dstBaseSamp = AfxMin(raw->dstBaseSamp, src->sampCnt - 1);
    san->src.baseChan = AfxMin(raw->src.baseChan, dst->chanCnt - 1);
    san->src.baseSamp = AfxMin(raw->src.baseSamp, dst->sampCnt - 1);

    san->src.chanCnt = AfxClamp(raw->src.chanCnt, 1, AfxMin(src->chanCnt - san->dstBaseChan, dst->chanCnt - san->src.baseChan));
    san->src.sampCnt = AfxClamp(raw->src.sampCnt, 1, AfxMin(src->sampCnt - san->dstBaseSamp, dst->sampCnt - san->src.baseSamp));
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxUpdateAudio(afxAudio wav, afxUnit opCnt, afxWaveIo const ops[], void const* src, afxUnit portIdx)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    afxMixBridge mexu;
    afxMixSystem msys = AfxGetAudioContext(wav);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    if (!AfxGetMixBridges(msys, portIdx, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxWaveIo const* op = &ops[i];
        //AFX_ASSERT_RANGE(ras->lodCnt, op->rgn.lodIdx, 1);
        //AFX_ASSERT_RANGE_WHD(ras->whd, op->rgn.origin, op->rgn.whd);
    }
#endif

    amxTransference transfer = { 0 };
    transfer.dstFcc = afxFcc_WAV;
    transfer.dst.wav = wav;
    transfer.src.src = src;

    if (_AmxSubmitTransferences(mexu, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    if (AfxWaitForIdleMixQueue(msys, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    return err;
}

_AMX afxError AfxDumpAudio(afxAudio wav, afxUnit opCnt, afxWaveIo const ops[], void* dst, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);
    AFX_ASSERT(((wav->bufFlags & avxBufferFlag_ACCESS) & avxBufferFlag_R));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(dst);

    afxMixBridge mexu;
    afxMixSystem msys = AfxGetAudioContext(wav);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    if (!AfxGetMixBridges(msys, portId, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxWaveIo const* op = &ops[i];
    }
#endif

    amxTransference transfer = { 0 };
    transfer.srcFcc = afxFcc_WAV;
    transfer.src.wav = wav;
    transfer.dst.dst = dst;

    if (_AmxSubmitTransferences(mexu, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    if (AfxWaitForIdleMixQueue(dsys, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxUploadAudio(afxAudio wav, afxUnit opCnt, afxWaveIo const ops[], afxStream in, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    afxMixBridge mexu;
    afxMixSystem msys = AfxGetAudioContext(wav);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    if (!AfxGetMixBridges(msys, portId, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxWaveIo const* op = &ops[i];
    }
#endif

    amxTransference transfer = { 0 };
    transfer.dstFcc = afxFcc_WAV;
    transfer.srcFcc = afxFcc_IOB;
    transfer.dst.wav = wav;
    transfer.src.iob = in;

    if (_AmxSubmitTransferences(mexu, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    if (AfxWaitForMixQueue(dsys, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    return err;
}

_AMX afxError AfxDownloadAudio(afxAudio wav, afxUnit opCnt, afxWaveIo const ops[], afxStream out, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    afxMixBridge mexu;
    afxMixSystem msys = AfxGetAudioContext(wav);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    if (!AfxGetMixBridges(msys, portId, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxWaveIo const* op = &ops[i];
    }
#endif

    amxTransference transfer = { 0 };
    transfer.srcFcc = afxFcc_WAV;
    transfer.dstFcc = afxFcc_IOB;
    transfer.src.wav = wav;
    transfer.dst.iob = out;

    if (_AmxSubmitTransferences(mexu, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    if (AfxWaitForMixQueue(dsys, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    return err;
}

_AMX afxError AfxPrintAudio(afxAudio wav, afxWaveInterval const* op, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    if (!uri || AfxIsUriBlank(uri))
    {
        AfxThrowError();
        return err;
    }

    afxStream file;
    if (AfxOpenFile(uri, afxFileFlag_W, &file))
    {
        AfxThrowError();
        return err;
    }

    afxWaveIo op2 = { 0 };

    if (op) _AmxSanitizeWaveInterval(wav, op, &op2.seg);
    else
    {
        op2.seg.sampCnt = wav->sampCnt;
        op2.seg.chanCnt = wav->chanCnt;
    }

    // Prepare the WAV header
    WAVHeader hdr;
    hdr.chunkFcc[0] = 'R';
    hdr.chunkFcc[1] = 'I';
    hdr.chunkFcc[2] = 'F';
    hdr.chunkFcc[3] = 'F';
    hdr.waveFcc[0] = 'W';
    hdr.waveFcc[1] = 'A';
    hdr.waveFcc[2] = 'V';
    hdr.waveFcc[3] = 'E';
    hdr.fmtFcc[0] = 'f';
    hdr.fmtFcc[1] = 'm';
    hdr.fmtFcc[2] = 't';
    hdr.fmtFcc[3] = ' ';
    hdr.fmt = 1;  // PCM
    hdr.chanCnt = op2.seg.chanCnt;  // Mono
    hdr.freq = wav->freq;
    hdr.data[0] = 'd';
    hdr.data[1] = 'a';
    hdr.data[2] = 't';
    hdr.data[3] = 'a';

    // Number of samples in the audio
    afxUnit numSamples = wav->sampCnt;

    switch (wav->fmt)
    {
    case amxFormat_S32f:
    {
        hdr.fmt = 3; // 3 for IEEE float
        hdr.bitsPerSample = 32;  // 32-bit floating point
        hdr.byteRate = hdr.freq * hdr.chanCnt * 4; // 4 bytes per sample (32-bit float)
        hdr.blockAlign = hdr.chanCnt * 4; // 4 bytes per sample
        hdr.dataSiz = numSamples * hdr.chanCnt * 4;  // data size
        hdr.chunkSiz = 36 + hdr.dataSiz;  // total file size minus 8 bytes

        op2.samplesPerChan = hdr.chanCnt;
        op2.chansPerFrame = numSamples;
        break;
    }
    case amxFormat_A32f:
    {
        hdr.fmt = 3; // 3 for IEEE float
        hdr.bitsPerSample = 32;  // 32-bit floating point
        hdr.byteRate = hdr.freq * hdr.chanCnt * 4; // 4 bytes per sample (32-bit float)
        hdr.blockAlign = 4; // 4 bytes per sample
        hdr.dataSiz = numSamples * hdr.chanCnt * 4;  // data size
        hdr.chunkSiz = 36 + hdr.dataSiz;  // total file size minus 8 bytes

        op2.samplesPerChan = numSamples;
        op2.chansPerFrame = hdr.chanCnt;
        break;
    }
    case amxFormat_S16i:
    {
        hdr.fmt = 1;
        hdr.bitsPerSample = 16;  // 16-bit audio
        hdr.byteRate = hdr.freq * hdr.chanCnt * 2; // 2 bytes per sample (16-bit audio)
        hdr.blockAlign = hdr.chanCnt * 2; // 2 bytes per sample
        hdr.dataSiz = numSamples * hdr.chanCnt * 2;  // data size
        hdr.chunkSiz = 36 + hdr.dataSiz;  // total file size minus 8 bytes

        op2.samplesPerChan = hdr.chanCnt;
        op2.chansPerFrame = numSamples;
        break;
    }
    case amxFormat_A16i:
    {
        hdr.fmt = 1;
        hdr.bitsPerSample = 16;  // 16-bit audio
        hdr.byteRate = hdr.freq * hdr.chanCnt * 2; // 2 bytes per sample (16-bit audio)
        hdr.blockAlign = 2; // 2 bytes per sample
        hdr.dataSiz = numSamples * hdr.chanCnt * 2;  // data size
        hdr.chunkSiz = 36 + hdr.dataSiz;  // total file size minus 8 bytes

        op2.samplesPerChan = numSamples;
        op2.chansPerFrame = numSamples;
        break;
    }
    default: AfxThrowError(); break;
    }

    // Write the header to the file
    if (AfxWriteStream(file, sizeof(WAVHeader), 1, &hdr))
    {
        AfxThrowError();
        AfxDisposeObjects(1, &file);
        return err;
    }

    op2.offset = AfxAskStreamPosn(file);

    if (AfxDownloadAudio(wav, 1, &op2, file, 0))
    {
        AfxThrowError();
        AfxDisposeObjects(1, &file);
        return err;
    }

    afxMixSystem msys = AfxGetAudioContext(wav);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    if (AfxWaitForMixBridge(msys, 0, 0))
        AfxThrowError();

    AfxDisposeObjects(1, &file);

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError _AmxWavDtorCb(afxAudio wav)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    afxMixSystem msys = AfxGetAudioContext(wav);

    if (wav->mappedRange)
    {
        //AfxUnmapAudio(wav, TRUE);
        AFX_ASSERT(wav->sysmemBuffered || !wav->mappedRange);
    }

    if (wav->bytemap)
    {
        if (!wav->sysmemBuffered)
        {
            AfxThrowError();
        }
        else
        {
            AfxDeallocate((void**)&wav->bytemap, AfxHere());
            wav->sysmemBuffered = FALSE;
        }
    }

    return err;
}

_AMX afxError _AmxWavCtorCb(afxAudio wav, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    afxMixSystem msys = AfxGetAudioContext(wav);
    afxAudioInfo const *spec = ((afxAudioInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(spec);
    afxBool allocOnSysMem = *(afxBool*)(args[2]);

    if (!spec->fmt)
    {
        AfxThrowError();
        return err;
    }

    if (!spec->freq)
    {
        AfxThrowError();
        return err;
    }

    if (!spec->sampCnt)
    {
        AfxThrowError();
        return err;
    }

    afxUnit bps = 0;
    switch (spec->fmt)
    {
    case amxFormat_A32f: bps = 32; break;
    case amxFormat_S32f: bps = 32; break;
    case amxFormat_A32i: bps = 32; break;
    case amxFormat_S32i: bps = 32; break;
    case amxFormat_A24i: bps = 24; break;
    case amxFormat_S24i: bps = 24; break;
    case amxFormat_A16i: bps = 16; break;
    case amxFormat_S16i: bps = 16; break;
    case amxFormat_A8i: bps = 8; break;
    case amxFormat_S8i: bps = 8; break;
    default: AfxThrowError();  break;
    }
    wav->fmt = spec->fmt;
    wav->fmtBps = bps;
    wav->fmtStride = bps / AFX_BYTE_SIZE;

    afxUnit sampCnt = AfxMax(1, spec->sampCnt);
    afxUnit chanCnt = AfxMax(1, spec->chanCnt);
    afxUnit freq = AfxMax(1, spec->freq);
    
    wav->sampCnt = sampCnt;
    wav->chanCnt = chanCnt;
    //wav->frameCnt = frameCnt;
    wav->freq = freq;

    afxUnit siz = AFX_ALIGNED_SIZE((wav->fmtStride * sampCnt * chanCnt), AFX_SIMD_ALIGNMENT) * chanCnt;
    wav->bufCap = AFX_ALIGNED_SIZE(siz, AFX_SIMD_ALIGNMENT);
    
    wav->bytemap = NIL;
    wav->mappedOffset = 0;
    wav->mappedRange = 0;
    wav->mappedFlags = NIL;

    wav->udd = spec->udd;
    wav->sysmemBuffered = !!allocOnSysMem;

    if (allocOnSysMem)
    {
        if (AfxAllocate(wav->bufCap, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&wav->bytemap))
            AfxThrowError();
    }
    return err;
}

_AMX afxClassConfig const _AMX_WAV_CLASS_CONFIG =
{
    .fcc = afxFcc_WAV,
    .name = "Audio",
    .desc = "Formatted Audio Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxAudio)),
    .ctor = (void*)_AmxWavCtorCb,
    .dtor = (void*)_AmxWavDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquireAudios(afxMixSystem msys, afxUnit cnt, afxAudioInfo const info[], afxAudio buffers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(buffers);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxGetAudioClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_WAV);

    afxBool allocOnSysMem = TRUE;

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { msys, (void*)info, &allocOnSysMem }))
        AfxThrowError();

    return err;
}

_AMX afxError AmxLoadAudios(afxMixSystem msys, afxUnit cnt, afxUri const uris[], afxAudio buffers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(buffers);
    AFX_ASSERT(uris);
    AFX_ASSERT(cnt);
    
    /*
        In a .wav file, the interleaving of audio samples (whether the file is stereo with interleaved or 
        deinterleaved channels) is not explicitly stored as a flag in the header. However, you can infer whether 
        the audio is interleaved or deinterleaved based on how the data is structured, which can be derived from 
        the num_channels and block_align fields in the WAV header.
    */

    //  if (hdr.blockAlign == hdr.chanCnt * hdr.bitsPerSample / AFX_BYTE_SIZE)

    for (afxUnit fIdx = 0; fIdx < cnt; fIdx++)
    {
        afxStream iob;
        AfxOpenFile(&uris[fIdx], afxFileFlag_R, &iob);

        WAVHeader hdr;
        AfxReadStream(iob, sizeof(hdr), 0, &hdr);
        amxFormat fmt = NIL;

        switch (hdr.fmt)
        {
        case 1:
        {
            fmt = amxFormat_S16i;
            break;
        }
        case 3:
        {
            fmt = amxFormat_S32f;
            break;
        }
        default: AfxThrowError(); break;
        }

        afxUnit totalSamplesPerChan = hdr.dataSiz / (hdr.chanCnt * hdr.bitsPerSample / 8);

        afxAudioInfo wavi = { 0 };
        wavi.fmt = fmt;
        wavi.sampCnt = totalSamplesPerChan; // our channel will contain a fraction of the frequency.
        wavi.chanCnt = hdr.chanCnt; // our channels will be deinterlaved for that fraction.
        wavi.freq = hdr.freq;

        if (AfxAcquireAudios(msys, 1, &wavi, &buffers[fIdx]))
            AfxThrowError();

        void* data;
        AfxAllocate(hdr.dataSiz, 0, AfxHere(), (void**)&data);

        AfxReadStream(iob, hdr.dataSiz, 0, data);

        afxWaveIo iop = { 0 };
        iop.offset = 0;// AfxAskStreamPosn(iob);
        iop.samplesPerChan = hdr.chanCnt; // interleaved
        iop.chansPerFrame = wavi.sampCnt;
        iop.seg.sampCnt = wavi.sampCnt;
        iop.seg.chanCnt = wavi.chanCnt;
        //iop.fmt = fmt;
        //AfxUploadAudio(buffers[fIdx], 1, &iop, iob, 0);
        //_AmxUploadAudio(buffers[fIdx], &iop, iob);
        AfxWaitForMixBridge(msys, 0, 0);
        
        _AmxUpdateAudio(buffers[fIdx], &iop, data);
        AfxDeallocate((void**)&data, AfxHere());

        AfxPrintAudio(buffers[fIdx], &iop.seg, AfxUri("../test.wav"));

        AfxDisposeObjects(1, &iob);
    }
    return err;
}
