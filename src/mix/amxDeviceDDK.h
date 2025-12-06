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

#ifndef AMX_DEVICE_DDK_H
#define AMX_DEVICE_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/mix/afxMixSystem.h"
#include "amxAudioDDK.h"
#include "amxContextDDK.h"
#include "mpu/amxMpuExecutor.h"
#include "amxSoundscapeDDK.h"
#include "amxSystemDDK.h"

AFX_DEFINE_STRUCT(_amxMdevReg)
{
    afxDeviceInfo       dev;
    afxAcceleration     acceleration;
    amxAptitude          capabilities;
    amxFeatures      features;
    amxLimits        limits;
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
    amxAptitude          capabilities;
    amxFeatures      features;
    amxLimits        limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxString128        desc; // before port extintion
    afxString8          urn; // before port extintion

    afxClass            asioCls;
    struct _afxSdevIdd* idd;
};
#endif//_AMX_MIX_DEVICE_C

AMX afxClassConfig const _AMX_MDEV_CLASS_CONFIG;

#endif//AMX_DEVICE_DDK_H
