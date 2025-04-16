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

#ifndef AMX_IMPLEMENTATION_H
#define AMX_IMPLEMENTATION_H

#include "../../impl/afxExecImplKit.h"
#include "qwadro/inc/mix/afxMixSystem.h"
#include "amxImpl_Audio.h"
#include "amxImpl_MixContext.h"
#include "amxImpl_Executor.h"
#include "amxImpl_System.h"

AFX_DEFINE_STRUCT(_amxMixDeviceRegistration)
{
    afxDeviceInfo       dev;
    afxMixLimits        limits;
    afxMixFeatures      features;
    afxUnit             portCnt;
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
    afxMixLimits      limits;
    afxMixFeatures    features;
    struct
    {
        afxString8          urn;
        afxString128        desc;
        afxMixCapabilities  caps;
    }*ports;
    afxUnit             portCnt;

    afxClass            asioCls;
    struct _afxSdevIdd* idd;
};
#endif//_AMX_MIX_DEVICE_C

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

    void        (*flushCb)(afxSink asi);
    afxError    (*pushCb)(afxSink asio, amxAudio buf, amxAudioPeriod const* seg);
    afxError    (*pullCb)(afxSink asio, afxUnit, void*, afxUnit*);
    afxUnit     (*getAvailFramesCb)(afxSink asi);

    afxClock    startClock;
    afxClock    lastClock;
    afxClock    outCntResetClock;

    afxError(*iddDtorCb)(afxSink);
    struct _afxSoutIdd* idd;
};
#endif//_AMX_SINK_C

AMX afxClassConfig const _AMX_MDEV_CLASS_CONFIG;

AMX afxBool _AmxGetIcd(afxUnit icdIdx, afxModule* driver);

AMX afxClass const* _AmxGetAudioServiceClass(afxModule icd);
AMX afxClass const* _AmxGetMixDeviceClass(afxModule icd);
AMX afxClass const* _AmxGetMixSystemClass(afxModule icd);



AMX afxError _AmxImplementMixSystem(afxModule icd, afxClassConfig const* mdevCls, afxClassConfig const* msysCls);
AMX afxError _AmxRegisterMixDevices(afxModule icd, afxUnit cnt, _amxMixDeviceRegistration const infos[], afxMixDevice devices[]);

AMX afxError _AmxRegisterAudioServices(afxModule icd, afxUnit cnt, afxMixDeviceInfo const infos[], afxDevice devices[]);

#endif//AMX_IMPLEMENTATION_H
