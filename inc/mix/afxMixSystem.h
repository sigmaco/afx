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

// Qwadro Sound Synthesis Infrastructure

/**
    The afxMixSystem object manages the construction and lifetime of the audio
    processing graph. It owns the output node that connects the audio processing
    graph to the audio hardware.
*/

#ifndef AMX_SOUND_SYSTEM_H
#define AMX_SOUND_SYSTEM_H

#include "qwadro/inc/mix/afxMixDevice.h"
#include "qwadro/inc/mix/io/afxAudio.h"
#include "qwadro/inc/mix/io/afxSound.h"
#include "qwadro/inc/mix/op/afxSink.h"
#include "qwadro/inc/mix/op/afxMixContext.h"
#include "qwadro/inc/mix/io/afxAudient.h"

AFX_DEFINE_STRUCT(afxMixSystemConfig)
// The system-wide settings and parameters.
{
    afxMixBridgeConfig    prime;
    afxUnit                 auxCnt;
    afxMixBridgeConfig*   auxs;

    afxMixFeatures        requirements;
    afxUnit                 extensionCnt;
    afxString const*        extensions;

    afxUnit                 maxAmbisOrder; // 1
    afxUri const*           orderHrirCfg;

    void*                   udd;
};

AMX afxMixDevice  AfxGetSoundSystemDevice(afxMixSystem msys);

AMX afxUnit         AfxGetSoundBridges(afxMixSystem msys, afxUnit baseIdx, afxUnit cnt, afxMixBridge bridges[]);

AMX afxUnit         AfxQuerySoundBridges(afxMixSystem msys, afxUnit sdevId, afxUnit portId, afxUnit first, afxUnit cnt, afxMixBridge bridges[]);

/// Wait for a device context to become idle.
/// Wait for a bridge to become idle. To wait on the host for the completion of outstanding queue operations for a given bridge.
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.

AMX afxError        AfxWaitForSoundSystem(afxMixSystem msys, afxTime timeout);
AMX afxError        AfxWaitForSoundBridge(afxMixSystem msys, afxUnit exuIdx, afxTime timeout);
AMX afxError        AfxWaitForSoundQueue(afxMixSystem msys, afxUnit exuIdx, afxUnit queIdx, afxTime timeout);

AMX afxError        AfxRollMixers(afxMixSystem msys, afxReal clock, afxReal dur, afxUnit iterCnt, afxUnit cnt, afxMixContext mixers[]);

AMX afxError        AfxSinkAudioSignal(afxMixSystem msys, void* ctrl, afxSink sink, afxAudio aud, afxWaveInterval const* seg);

////////////////////////////////////////////////////////////////////////////////

AMX afxError        AfxConfigureMixSystem(afxUnit icd, afxMixSystemConfig* cfg);

AMX afxError        AfxEstablishMixSystem(afxUnit icd, afxMixSystemConfig const* cfg, afxMixSystem* system);

#endif//AMX_SOUND_SYSTEM_H
