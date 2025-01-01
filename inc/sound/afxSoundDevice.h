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

  //////////////////////////////////////////////////////////////////////////////
 // Advanced Sound Experience on Qwadro                                      //
//////////////////////////////////////////////////////////////////////////////
// This section is part of SIGMA A4D; Advanced Audio Extensions for Qwadro.

// Qwadro Sound Synthesis Infrastructure

#ifndef ASX_SOUND_DEVICE_H
#define ASX_SOUND_DEVICE_H

#include "qwadro/inc/exec/afxDevice.h"
#include "qwadro/inc/sound/afxSoundBridge.h"
#include "qwadro/inc/sound/op/afxSink.h"

#define AFX_MAX_AUDITION_HEADS 8
#define AFX_MAX_AUDIO_CHANNELS 32

AFX_DEFINE_STRUCT(afxSoundFeatures)
{
    afxBool8    eax;
    afxBool8    hrtf;
    afxBool8    ambisonics;
};

AFX_DEFINE_STRUCT(afxSoundLimits)
{
    afxUnit maxHeadCnt; // audition heads are listeners in Qwadro parlance. At least AFX_MAX_AUDIO_HEADS.
    afxUnit maxSrcCnt;
    afxUnit maxVoxCnt;
    afxUnit maxSampCnt; // AFX_N32_MAX
    afxUnit maxChanCnt; // AFX_N32_MAX
    afxUnit minFreq; // 4
    afxUnit maxFreq; // 192000 --- 192kHz
    afxUnit maxAmbisOrder; // 2 --- 9 channels
};

// SOUND DEVICE HANDLING ///////////////////////////////////////////////////////

ASX afxBool     AfxIsSoundDevicePrompt(afxSoundDevice sdev);

ASX afxUnit     AfxCountSoundPorts(afxSoundDevice sdev);

ASX void        AfxQuerySoundDeviceLimits(afxSoundDevice sdev, afxSoundLimits* limits);

ASX void        AfxQuerySoundDeviceFeatures(afxSoundDevice sdev, afxSoundFeatures* features);

ASX void        AfxQuerySoundPortCapabilities(afxSoundDevice sdev, afxUnit portId, afxSoundPortCaps* caps);

ASX afxUnit     AfxChooseSoundPorts(afxSoundDevice sdev, afxSoundPortFlags caps, afxAcceleration accel, afxUnit maxCnt, afxUnit portId[]);

// IMPLEMENTATION DISCOVERY ////////////////////////////////////////////////////

ASX afxUnit     AfxEnumerateSoundDevices(afxUnit icd, afxUnit first, afxUnit cnt, afxSoundDevice devices[]);
ASX afxUnit     AfxInvokeSoundDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxSoundDevice), afxUnit cnt);
ASX afxUnit     AfxEvokeSoundDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxSoundDevice), afxUnit cnt, afxSoundDevice devices[]);

ASX afxUnit     AfxEnumerateSoundSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxSoundSystem systems[]);
ASX afxUnit     AfxInvokeSoundSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxSoundSystem), afxUnit cnt);
ASX afxUnit     AfxEvokeSoundSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxSoundSystem), afxUnit cnt, afxSoundSystem systems[]);

#endif//ASX_SOUND_DEVICE_H
