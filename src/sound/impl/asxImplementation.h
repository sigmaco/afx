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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef ASX_IMPLEMENTATION_H
#define ASX_IMPLEMENTATION_H

#include "../../dev/afxExecImplKit.h"
#include "qwadro/inc/sound/afxSoundSystem.h"
#include "asxImpl_Audio.h"
#include "asxImpl_Mixer.h"
#include "asxImpl_Executor.h"
#include "asxImpl_System.h"

AFX_DEFINE_STRUCT(afxSoundDeviceInfo)
{
    afxDeviceInfo           dev;
    afxSoundLimits          limits;
    afxSoundFeatures        features;
    afxUnit                 portCnt;
};

#ifdef _ASX_SOUND_DEVICE_C
#ifdef _ASX_SOUND_DEVICE_IMPL
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(_asxSoundDevice)
#else
AFX_OBJECT(afxSoundDevice)
#endif
{
    AFX_OBJ(afxDevice)  dev;
    afxSoundLimits      limits;
    afxSoundFeatures    features;
    struct
    {
        afxString8      urn;
        afxString128    desc;
        afxSoundPortCaps caps;
    }*ports;
    afxUnit             portCnt;

    struct _afxSdevIdd* idd;
};
#endif//_ASX_SOUND_DEVICE_C

#ifdef _ASX_SINK_C
#ifdef _ASX_SINK_IMPL
AFX_OBJECT(_asxSink)
#else
AFX_OBJECT(afxSink)
#endif
{
    afxUnit             muteReqCnt;

    asxFormat           fmt;
    afxUnit             freq; // Hz
    afxUnit             chanCnt;
    afxUnit             samplesPerFrame; // in samples
    afxUnit             latency; // frames ready
    afxAudio*           buffers;
    afxInterlockedQueue freeBuffers;
    afxInterlockedQueue readyBuffers;
    afxAtom32           sinkingBufIdx;

    afxRing             ioRing;

    void*       udd; // user-defined data
    afxBool     (*getIddCb)(afxSink, afxUnit, void*);

    void        (*flushCb)(afxSink asi);
    afxError    (*pushCb)(afxSink asio, afxAudio buf, afxWaveInterval const* seg);
    afxError    (*pullCb)(afxSink asio, afxUnit, void*, afxUnit*);
    afxUnit     (*getAvailFramesCb)(afxSink asi);

    afxClock    startClock;
    afxClock    lastClock;
    afxClock    outCntResetClock;

    afxError(*iddDtorCb)(afxSink);
    struct _afxSoutIdd* idd;
};
#endif//_ASX_SINK_C

ASX afxClassConfig const _ASX_SDEV_CLASS_CONFIG;

ASX afxBool AsxGetIcd(afxUnit icdIdx, afxModule* driver);

ASX afxClass const* _AsxGetAudioServiceClass(afxModule icd);
ASX afxClass const* _AsxGetSoundDeviceClass(afxModule icd);
ASX afxClass const* _AsxGetSoundSystemClass(afxModule icd);



ASX afxError _AsxImplementSoundSystem(afxModule icd, afxClassConfig const* asiCls, afxClassConfig const* sdevCls, afxClassConfig const* ssysCls);
ASX afxError _AsxRegisterSoundDevices(afxModule icd, afxUnit cnt, afxSoundDeviceInfo const infos[], afxSoundDevice devices[]);

#endif//ASX_IMPLEMENTATION_H
