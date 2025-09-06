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

#ifndef AMX_IMPLEMENTATION_H
#define AMX_IMPLEMENTATION_H

#include "../../impl/afxExecImplKit.h"
#include "qwadro/mix/afxMixSystem.h"
#include "amxImpl_Audio.h"
#include "amxImpl_MixContext.h"
#include "amxImpl_Executor.h"
#include "amxImpl_Soundscape.h"
#include "amxImpl_System.h"

AFX_DEFINE_STRUCT(_amxMixSystemImplementation)
{
    afxClassConfig mcdcCls;
    afxClassConfig mdevCls;
    afxClassConfig msysCls;
};

AFX_DEFINE_STRUCT(_amxCodecRegistration)
{

};

AFX_DEFINE_STRUCT(_amxMixDeviceRegistration)
{
    afxDeviceInfo       dev;
    afxAcceleration     acceleration;
    afxMixCaps          capabilities;
    afxMixFeatures      features;
    afxMixLimits        limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
};

#ifdef _AMX_MIX_DEVICE_C
#ifdef _AMX_MIX_DEVICE_IMPL
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(_amxMixDevice)
#else
AFX_OBJECT(afxMixDevice)
#endif
{
    AFX_OBJ(afxDevice)  dev;
    afxAcceleration     acceleration;
    afxMixCaps          capabilities;
    afxMixFeatures      features;
    afxMixLimits        limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxString128        desc; // before port extintion
    afxString8          urn; // before port extintion

    afxClass            asioCls;
    struct _afxSdevIdd* idd;
};
#endif//_AMX_MIX_DEVICE_C

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

AMX afxClassConfig const _AMX_MDEV_CLASS_CONFIG;

AMX afxBool _AmxGetIcd(afxUnit icdIdx, afxModule* driver);

AMX afxClass const* _AmxIcdGetAudioServiceClass(afxModule icd);
AMX afxClass const* _AmxIcdGetMdevClass(afxModule icd);
AMX afxClass const* _AmxIcdGetMcdcClass(afxModule icd);
AMX afxClass const* _AmxIcdGetMsysClass(afxModule icd);



AMX afxError _AmxImplementMixSystem(afxModule icd, _amxMixSystemImplementation const* cfg);
AMX afxError _AmxRegisterCodecs(afxModule icd, afxUnit cnt, _amxCodecRegistration const infos[], amxCodec codecs[]);
AMX afxError _AmxRegisterMixDevices(afxModule icd, afxUnit cnt, _amxMixDeviceRegistration const infos[], afxMixDevice devices[]);

AMX afxError _AmxRegisterAudioServices(afxModule icd, afxUnit cnt, afxMixDeviceInfo const infos[], afxDevice devices[]);



AMX int audio_ringbuffer_init(AudioRingBuffer* rb, void* bufPtr, afxUnit bufStride, size_t capacity_frames, size_t channels);

AMX void audio_ringbuffer_free(AudioRingBuffer* rb);

AMX void audio_ringbuffer_write(AudioRingBuffer* rb, const afxByte* input, afxUnit srcStride, size_t frames);

AMX size_t audio_ringbuffer_read(AudioRingBuffer* rb, afxByte* output, afxUnit dstStride, size_t max_frames);

AMX size_t audio_ringbuffer_available(const AudioRingBuffer* rb);

AMX size_t audio_ringbuffer_writable(const AudioRingBuffer* rb);

#endif//AMX_IMPLEMENTATION_H
