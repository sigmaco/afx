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

// 5417

#define _AMX_MIX_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_QUEUE_C
#define _AMX_BUFFER_C
#define _AMX_AUDIO_C
#define _AMX_VOICE_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_SINK_C
#include "amxImplementation.h"

_AMX afxError _AmxFillBuffer(amxBuffer buf, afxSize offset, afxSize range, afxUnit stride, afxUnit sampleCnt, afxUnit sampleRate, amxFormat fmt, afxReal amplitude, afxReal freq, afxReal dur)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    //afxUnit sampleRate = aud->freq;
    // period is the duration
    afxInt num_samples = (afxInt)(sampleRate * dur);  // Total number of samples --- wavelength
    // num_samples is the wavelength (in samples)
    num_samples = AFX_MIN(num_samples, sampleCnt);

    switch (fmt)
    {
    case amxFormat_M32f:
    case amxFormat_S32f:
    {
        for (afxUnit j = 0; j < num_samples; j++)
        {
            afxReal32* out = (afxReal32*)&buf->storage[0].hostedAlloc.bytemap[offset + stride * j];

            afxReal t = (afxReal)j / sampleRate; // Time in seconds
            out[0] = amplitude * (freq * t);
        }
        break;
    }
    case amxFormat_S16i:
    case amxFormat_M16i:
    {
        for (afxUnit j = 0; j < num_samples; j++)
        {
            afxInt16* out = (afxInt16*)&buf->storage[0].hostedAlloc.bytemap[offset + stride * j];

            afxReal t = (afxReal)j / sampleRate; // Time in seconds
            out[0] = (afxInt16)((amplitude * (freq * t)) * 32767.0f);
        }
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMX afxError _AmxCopyBuffer(amxBuffer buf, amxBufferCopy const* op, amxBuffer dst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT(dst);
    AFX_ASSERT(op);

    AfxCopy(&dst->storage[0].hostedAlloc.bytemap[op->to], &buf->storage[0].hostedAlloc.bytemap[op->from], op->range);

    return err;
}

_AMX afxError _AmxDumpBuffer(amxBuffer buf, amxBufferIo const* op, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT(dst);
    AFX_ASSERT(op);

    afxByte* dst2 = dst;

    AfxStream2(op->rowCnt, &buf->storage[0].hostedAlloc.bytemap[op->srcOffset], op->srcStride, &dst2[op->dstOffset], op->dstStride);

    return err;
}

_AMX afxError _AmxUpdateBuffer(amxBuffer buf, amxBufferIo const* op, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT(src);
    AFX_ASSERT(op);

    afxByte const* src2 = src;

    AfxStream2(op->rowCnt, &src2[op->srcOffset], op->srcStride, &buf->storage[0].hostedAlloc.bytemap[op->dstOffset], op->dstStride);

    return err;
}

_AMX afxError _AmxDownloadBuffer(amxBuffer buf, amxBufferIo const* op, afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT(iob);
    AFX_ASSERT(op);

    if (AfxSeekStream(iob, op->dstOffset, afxSeekOrigin_BEGIN))
    {
        AfxThrowError();
    }

    if (AfxWriteStream2(iob, op->rowCnt * op->dstStride, op->dstStride, &buf->storage[0].hostedAlloc.bytemap[op->srcOffset], op->srcStride))
    {
        AfxThrowError();
    }
    return err;
}

_AMX afxError _AmxUploadBuffer(amxBuffer buf, amxBufferIo const* op, afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT(iob);
    AFX_ASSERT(op);

    if (AfxSeekStream(iob, op->srcOffset, afxSeekOrigin_BEGIN))
    {
        AfxThrowError();
    }

    if (AfxReadStream2(iob, op->rowCnt * op->srcStride, op->srcStride, &buf->storage[0].hostedAlloc.bytemap[op->dstOffset], op->dstStride))
    {
        AfxThrowError();
    }
    return err;
}
