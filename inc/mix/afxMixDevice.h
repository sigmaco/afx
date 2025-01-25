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

#ifndef AMX_MIX_DEVICE_H
#define AMX_MIX_DEVICE_H

#include "qwadro/inc/exec/afxDevice.h"
#include "qwadro/inc/mix/afxMixBridge.h"
#include "qwadro/inc/mix/op/afxSink.h"

#define AFX_MAX_AUDITION_HEADS 8
#define AFX_MAX_AUDIO_CHANNELS 32

AFX_DEFINE_STRUCT(afxMixFeatures)
{
    afxBool8    eax;
    afxBool8    hrtf;
    afxBool8    ambisonics;
};

AFX_DEFINE_STRUCT(afxMixLimits)
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

// MIX DEVICE HANDLING /////////////////////////////////////////////////////////

AMX afxBool     AfxIsMixDevicePrompt(afxMixDevice mdev);

AMX afxUnit     AfxCountMixPorts(afxMixDevice mdev);

AMX void        AfxQueryMixDeviceLimits(afxMixDevice mdev, afxMixLimits* limits);

AMX void        AfxQueryMixDeviceFeatures(afxMixDevice mdev, afxMixFeatures* features);

AMX void        AfxQueryMixPortCapabilities(afxMixDevice mdev, afxUnit portId, afxMixPortCaps* caps);

AMX afxUnit     AfxChooseMixPorts(afxMixDevice mdev, afxMixPortFlags caps, afxAcceleration accel, afxUnit maxCnt, afxUnit portId[]);

// IMPLEMENTATION DISCOVERY ////////////////////////////////////////////////////

AMX afxUnit     AfxEnumerateSoundDevices(afxUnit icd, afxUnit first, afxUnit cnt, afxMixDevice devices[]);
AMX afxUnit     AfxInvokeSoundDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxMixDevice), afxUnit cnt);
AMX afxUnit     AfxEvokeSoundDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxMixDevice), afxUnit cnt, afxMixDevice devices[]);

AMX afxUnit     AfxEnumerateSoundSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxMixSystem systems[]);
AMX afxUnit     AfxInvokeSoundSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxMixSystem), afxUnit cnt);
AMX afxUnit     AfxEvokeSoundSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxMixSystem), afxUnit cnt, afxMixSystem systems[]);

#endif//AMX_MIX_DEVICE_H
