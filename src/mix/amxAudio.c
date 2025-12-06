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
#define _AMX_BUFFER_C
#define _AMX_AUDIO_C
#include "amxIcd.h"

AFX_DEFINE_STRUCT(amxSoundBank)
{
    afxUri      uri;
    amxBuffer   buf;
    // All samples must use an only format to avoid pipeline/state changing.
    amxFormat   fmt;
    afxUnit     freq;
    afxUnit     chCnt;

    // In Amiga AHI, an audio nests a group of audio tracks sharing same amount of channels and frequency.

    struct
    {
        afxSize     offset;
        afxUnit     size;
        afxReal     vol;
        afxReal     pan;
    } tracks[];
};

 // WAV header structure
#pragma pack(push, 1)
typedef struct _WAVHeader
{
    afxChar chunkFcc[4]; // "RIFF"
    afxUnit32 chunkSiz; // 36 + SubChunk2Size
    afxChar audeFcc[4]; // "WAVE"
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
} _WAVHeader;
#pragma pack(pop)

_AMXINL afxMixSystem AmxGetAudioHost(amxAudio aud)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);
    afxMixSystem msys = AfxGetHost(aud);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys;
}

_AMXINL void AmxDescribeAudio(amxAudio aud, amxAudioInfo* desc)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);
    AFX_ASSERT(desc);
    desc->fmt = aud->fmt;
    desc->chanCnt = aud->chanCnt;
    desc->freq = aud->freq;
    desc->sampCnt = aud->sampCnt;
    //desc->frameCnt = aud->frameCnt;
    desc->udd = aud->udd;
}

_AMXINL amxBuffer AmxGetAudioBuffer(amxAudio aud)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);
    return aud->buf;
}

_AMXINL afxReal AmxGetAudioDuration(amxAudio aud)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);
    return aud->sampCnt / aud->freq;
}

_AMX void _AmxSanitizeAudioPeriod(amxAudio aud, amxAudioPeriod const* raw, amxAudioPeriod* san)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);

    /*
        This code sanitizes a amxAudioPeriod struct to be within the bounds of the audio objects.
    */

    san->baseChan = AFX_MIN(raw->baseChan, aud->chanCnt - 1);
    san->baseSamp = AFX_MIN(raw->baseSamp, aud->sampCnt - 1);

    san->chanCnt = AFX_CLAMP(raw->chanCnt, 1, aud->chanCnt - san->baseChan);
    san->sampCnt = AFX_CLAMP(raw->sampCnt, 1, aud->sampCnt - san->baseSamp);
}

_AMX void _AmxSanitizeAudioCopy(amxAudio src, amxAudio dst, amxAudioCopy const* raw, amxAudioCopy* san)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &dst);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);

    /*
        This code sanitizes a amxAudioCopy struct to be within the bounds of the audio objects.
    */

    san->dstBaseChan = AFX_MIN(raw->dstBaseChan, src->chanCnt - 1);
    san->dstBaseSamp = AFX_MIN(raw->dstBaseSamp, src->sampCnt - 1);
    san->src.baseChan = AFX_MIN(raw->src.baseChan, dst->chanCnt - 1);
    san->src.baseSamp = AFX_MIN(raw->src.baseSamp, dst->sampCnt - 1);

    san->src.chanCnt = AFX_CLAMP(raw->src.chanCnt, 1, AFX_MIN(src->chanCnt - san->dstBaseChan, dst->chanCnt - san->src.baseChan));
    san->src.sampCnt = AFX_CLAMP(raw->src.sampCnt, 1, AFX_MIN(src->sampCnt - san->dstBaseSamp, dst->sampCnt - san->src.baseSamp));
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxUpdateAudio(amxAudio aud, afxUnit opCnt, amxAudioIo const ops[], void const* src, avxFence signal)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxAudioIo const* op = &ops[i];
        //AFX_ASSERT_RANGE(ras->mipCnt, op->rgn.lodIdx, 1);
        //AFX_ASSERT_RANGE_WHD(ras->whd, op->rgn.origin, op->rgn.whd);
    }
#endif

    amxTransference transfer = { 0 };
    transfer.dstFcc = afxFcc_AUD;
    transfer.dst.aud = aud;
    transfer.src.src = src;

    afxMixSystem msys = AmxGetAudioHost(aud);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (_AmxMsysGetDdi(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);

    return err;
}

_AMX afxError AmxDumpAudio(amxAudio aud, afxUnit opCnt, amxAudioIo const ops[], void* dst, avxFence signal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);
    AFX_ASSERT(((aud->bufFlags & avxBufferFlag_ACCESS) & avxBufferFlag_R));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(dst);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxAudioIo const* op = &ops[i];
    }
#endif

    amxTransference transfer = { 0 };
    transfer.srcFcc = afxFcc_AUD;
    transfer.src.aud = aud;
    transfer.dst.dst = dst;

    afxMixSystem msys = AmxGetAudioHost(aud);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (_AmxMsysGetDdi(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxUploadAudio(amxAudio aud, afxUnit opCnt, amxAudioIo const ops[], afxStream in, avxFence signal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxAudioIo const* op = &ops[i];
    }
#endif

    amxTransference transfer = { 0 };
    transfer.dstFcc = afxFcc_AUD;
    transfer.srcFcc = afxFcc_IOB;
    transfer.dst.aud = aud;
    transfer.src.iob = in;

    afxMixSystem msys = AmxGetAudioHost(aud);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (_AmxMsysGetDdi(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);

    return err;
}

_AMX afxError AmxDownloadAudio(amxAudio aud, afxUnit opCnt, amxAudioIo const ops[], afxStream out, avxFence signal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxAudioIo const* op = &ops[i];
    }
#endif

    amxTransference transfer = { 0 };
    transfer.srcFcc = afxFcc_AUD;
    transfer.dstFcc = afxFcc_IOB;
    transfer.src.aud = aud;
    transfer.dst.iob = out;

    afxMixSystem msys = AmxGetAudioHost(aud);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (_AmxMsysGetDdi(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);

    return err;
}

_AMX afxError AmxPrintAudio(amxAudio aud, amxAudioPeriod const* op, afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

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

    amxAudioIo op2 = { 0 };

    if (op) _AmxSanitizeAudioPeriod(aud, op, &op2.period);
    else
    {
        op2.period.sampCnt = aud->sampCnt;
        op2.period.chanCnt = aud->chanCnt;
    }

    // Prepare the WAV header
    _WAVHeader hdr;
    hdr.chunkFcc[0] = 'R';
    hdr.chunkFcc[1] = 'I';
    hdr.chunkFcc[2] = 'F';
    hdr.chunkFcc[3] = 'F';
    hdr.audeFcc[0] = 'W';
    hdr.audeFcc[1] = 'A';
    hdr.audeFcc[2] = 'V';
    hdr.audeFcc[3] = 'E';
    hdr.fmtFcc[0] = 'f';
    hdr.fmtFcc[1] = 'm';
    hdr.fmtFcc[2] = 't';
    hdr.fmtFcc[3] = ' ';
    hdr.fmt = 1;  // PCM
    hdr.chanCnt = op2.period.chanCnt;  // Mono
    hdr.freq = aud->freq;
    hdr.data[0] = 'd';
    hdr.data[1] = 'a';
    hdr.data[2] = 't';
    hdr.data[3] = 'a';

    // Number of samples in the audio
    afxUnit numSamples = aud->sampCnt;

    switch (aud->fmt)
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
    case amxFormat_M32f:
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
    case amxFormat_M16i:
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
    if (AfxWriteStream(file, sizeof(_WAVHeader), 1, &hdr))
    {
        AfxThrowError();
        AfxDisposeObjects(1, &file);
        return err;
    }

    op2.offset = AfxAskStreamPosn(file);

    if (AmxDownloadAudio(aud, 1, &op2, file, 0))
    {
        AfxThrowError();
        AfxDisposeObjects(1, &file);
        return err;
    }

    afxMixSystem msys = AmxGetAudioHost(aud);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    if (AmxWaitForMixBridge(msys, 0, 0))
        AfxThrowError();

    AfxDisposeObjects(1, &file);

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError _AmxAudDtorCb(amxAudio aud)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    afxMixSystem msys = AmxGetAudioHost(aud);

    if (aud->buf)
        AfxDisposeObjects(1, &aud->buf);

    return err;
}
AMX afxError _AmxMpu_ResampleI16F32(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op);
_AMX afxError _AmxAudCtorCb(amxAudio aud, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    afxMixSystem msys = AmxGetAudioHost(aud);
    amxAudioInfo const *spec = ((amxAudioInfo const *)args[1]) + invokeNo;
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
    case amxFormat_M32f: bps = 32; break;
    case amxFormat_S32f: bps = 32; break;
    case amxFormat_M32i: bps = 32; break;
    case amxFormat_S32i: bps = 32; break;
    case amxFormat_M24i: bps = 24; break;
    case amxFormat_S24i: bps = 24; break;
    case amxFormat_M16i: bps = 16; break;
    case amxFormat_S16i: bps = 16; break;
    case amxFormat_M8i: bps = 8; break;
    case amxFormat_S8i: bps = 8; break;
    default: AfxThrowError();  break;
    }
    aud->fmt = spec->fmt;
    aud->fmtBps = bps;
    aud->fmtStride = bps / AFX_BYTE_SIZE;

    afxUnit segCnt = AFX_MAX(1, spec->segCnt);
    afxUnit sampCnt = AFX_MAX(1, spec->sampCnt);
    afxUnit chanCnt = AFX_MAX(1, spec->chanCnt);
    afxUnit freq = AFX_MAX(1, spec->freq);
    
    aud->sampCnt = sampCnt;
    aud->chanCnt = chanCnt;
    aud->segCnt = segCnt;
    aud->freq = freq;

    afxUnit siz = AFX_ALIGN_SIZE((aud->fmtStride * sampCnt * chanCnt), AFX_SIMD_ALIGNMENT) * segCnt;
    aud->bufCap = AFX_ALIGN_SIZE(siz, AFX_SIMD_ALIGNMENT);
    
    aud->tag = spec->tag;
    aud->udd = spec->udd;

    amxBuffer buf;
    amxBufferInfo bufi = { 0 };
    bufi.fmt = aud->fmt;
    bufi.size = aud->bufCap;
    bufi.usage = amxBufferUsage_MIX;
    bufi.flags = amxBufferFlag_RW;
    AmxAcquireBuffers(msys, 1, &bufi, &buf);

    aud->buf = buf;

    return err;
}

_AMX afxClassConfig const _AMX_AUD_CLASS_CONFIG =
{
    .fcc = afxFcc_AUD,
    .name = "Audio",
    .desc = "Formatted Audio Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(amxAudio)),
    .ctor = (void*)_AmxAudCtorCb,
    .dtor = (void*)_AmxAudDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireAudios(afxMixSystem msys, afxUnit cnt, amxAudioInfo const info[], amxAudio audios[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(audios);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxMsysGetAudClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_AUD);

    afxBool allocOnSysMem = TRUE;

    if (AfxAcquireObjects(cls, cnt, (afxObject*)audios, (void const*[]) { msys, (void*)info, &allocOnSysMem }))
        AfxThrowError();

    return err;
}

_AMX afxError AmxLoadAudios(afxMixSystem msys, afxUnit cnt, afxUri const uris[], amxAudio audios[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(audios);
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

        _WAVHeader hdr;
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

        amxAudioInfo audi = { 0 };
        audi.fmt = fmt;
        audi.segCnt = 1;
        audi.sampCnt = totalSamplesPerChan; // our channel will contain a fraction of the frequency.
        audi.chanCnt = hdr.chanCnt; // our channels will be deinterlaved for that fraction.
        audi.freq = hdr.freq;

        if (AmxAcquireAudios(msys, 1, &audi, &audios[fIdx]))
            AfxThrowError();

        void* data;
        AfxAllocate(hdr.dataSiz, 0, AfxHere(), (void**)&data);

        AfxReadStream(iob, hdr.dataSiz, 0, data);

        amxAudioIo iop = { 0 };
        iop.offset = 0;// AfxAskStreamPosn(iob);
        iop.samplesPerChan = hdr.chanCnt; // interleaved
        iop.chansPerFrame = audi.sampCnt;
        iop.period.segCnt = 1;
        iop.period.chanCnt = audi.chanCnt;
        iop.period.sampCnt = audi.sampCnt;
        //iop.fmt = fmt;
        //AmxUploadAudio(buffers[fIdx], 1, &iop, iob, 0);
        //_AmxUploadAudio(buffers[fIdx], &iop, iob);
        AmxWaitForMixBridge(msys, 0, 0);
        
        _AmxUpdateAudio(audios[fIdx], &iop, data);
        AfxDeallocate((void**)&data, AfxHere());

        AmxPrintAudio(audios[fIdx], &iop.period, AfxUri("../test.wav"));

        AfxDisposeObjects(1, &iob);
    }
    return err;
}
