/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef AMX_SINK_IMPL_H
#define AMX_SINK_IMPL_H

#include "../impl/afxExecImplKit.h"
#include "qwadro/mix/afxMixSystem.h"
#include "amxAudioImpl.h"
#include "amxContextImpl.h"
#include "mpu/amxMpuExecutor.h"
#include "amxSoundscapeImpl.h"
#include "amxSystemImpl.h"

typedef struct
// Overwrite-on-Full Audio Ring
{
    afxByte* buffer;        // Interleaved audio buffer
    afxUnit bufStride;
    size_t capacity;      // Total frames
    size_t channels;      // Channels per frame
    size_t write_pos;     // Absolute frame index (can overflow safely)
    size_t read_pos;      // Absolute frame index (can overflow safely)
} AudioRingBuffer;

#ifdef _AMX_SINK_C
#ifdef _AMX_SINK_IMPL
AFX_OBJECT(_amxSink)
#else
AFX_OBJECT(afxSink)
#endif
{
    afxUnit             muteReqCnt;

    amxFormat           fmt;
    afxUnit             freq; // Hz
    afxUnit             chanCnt;
    afxUnit             samplesPerFrame; // in samples
    afxUnit             latency; // frames ready
    amxAudio*           buffers;
    afxInterlockedQueue freeBuffers;
    afxInterlockedQueue readyBuffers;
    afxAtom32           sinkingBufIdx;

    afxRing             ioRing;

    void*       udd; // user-defined data
    afxBool     (*getIddCb)(afxSink, afxUnit, void*);

    afxError    (*lockCb)(afxSink asi, afxUnit64 timeout, afxUnit minFrameCnt, amxBufferedTrack* room);
    afxError    (*unlockCb)(afxSink asi, afxFlags flags);
    void        (*flushCb)(afxSink asi);
    afxError    (*pushCb)(afxSink asio, amxAudio buf, amxAudioPeriod const* seg);
    afxError    (*pullCb)(afxSink asio, afxUnit, void*, afxUnit*);
    afxUnit     (*getAvailFramesCb)(afxSink asi);

    AudioRingBuffer rb;

    afxClock    startClock;
    afxClock    lastClock;
    afxClock    outCntResetClock;

    afxError(*iddDtorCb)(afxSink);
    struct _afxSoutIdd* idd;
};
#endif//_AMX_SINK_C

AMX int audio_ringbuffer_init(AudioRingBuffer* rb, void* bufPtr, afxUnit bufStride, size_t capacity_frames, size_t channels);

AMX void audio_ringbuffer_free(AudioRingBuffer* rb);

AMX void audio_ringbuffer_write(AudioRingBuffer* rb, const afxByte* input, afxUnit srcStride, size_t frames);

AMX size_t audio_ringbuffer_read(AudioRingBuffer* rb, afxByte* output, afxUnit dstStride, size_t max_frames);

AMX size_t audio_ringbuffer_available(const AudioRingBuffer* rb);

AMX size_t audio_ringbuffer_writable(const AudioRingBuffer* rb);

#endif//AMX_SINK_IMPL_H
