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
    The afxSoundSystem object manages the construction and lifetime of the audio
    processing graph. It owns the output node that connects the audio processing
    graph to the audio hardware.
*/

#ifndef ASX_SOUND_SYSTEM_H
#define ASX_SOUND_SYSTEM_H

#include "qwadro/inc/sound/afxSoundDevice.h"
#include "qwadro/inc/sound/io/afxAudio.h"
#include "qwadro/inc/sound/io/afxSound.h"
#include "qwadro/inc/sound/op/afxSink.h"
#include "qwadro/inc/sound/op/afxMixer.h"
#include "qwadro/inc/sound/io/afxAudient.h"

AFX_DEFINE_STRUCT(afxSoundSystemConfig)
// The system-wide settings and parameters.
{
    afxSoundBridgeConfig    prime;
    afxUnit                 auxCnt;
    afxSoundBridgeConfig*   auxs;

    afxSoundFeatures        requirements;
    afxUnit                 extensionCnt;
    afxString const*        extensions;

    afxUnit                 maxAmbisOrder; // 1
    afxUri const*           orderHrirCfg;

    void*                   udd;
};

ASX afxSoundDevice  AfxGetSoundSystemDevice(afxSoundSystem ssys);

ASX afxUnit         AfxGetSoundBridges(afxSoundSystem ssys, afxUnit baseIdx, afxUnit cnt, afxSoundBridge bridges[]);

ASX afxUnit         AfxQuerySoundBridges(afxSoundSystem ssys, afxUnit sdevId, afxUnit portId, afxUnit first, afxUnit cnt, afxSoundBridge bridges[]);

/// Wait for a device context to become idle.
/// Wait for a bridge to become idle. To wait on the host for the completion of outstanding queue operations for a given bridge.
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.

ASX afxError        AfxWaitForSoundSystem(afxSoundSystem ssys, afxTime timeout);
ASX afxError        AfxWaitForSoundBridge(afxSoundSystem ssys, afxUnit exuIdx, afxTime timeout);
ASX afxError        AfxWaitForSoundQueue(afxSoundSystem ssys, afxUnit exuIdx, afxUnit queIdx, afxTime timeout);

ASX afxError        AfxRollMixers(afxSoundSystem ssys, afxReal clock, afxReal dur, afxUnit iterCnt, afxUnit cnt, afxMixer mixers[]);

ASX afxError        AfxSinkAudioSignal(afxSoundSystem ssys, void* ctrl, afxSink sink, afxAudio aud, afxWaveInterval const* seg);

////////////////////////////////////////////////////////////////////////////////

ASX afxError        AfxConfigureSoundSystem(afxUnit icd, afxSoundSystemConfig* cfg);

ASX afxError        AfxEstablishSoundSystem(afxUnit icd, afxSoundSystemConfig const* cfg, afxSoundSystem* system);

#endif//ASX_SOUND_SYSTEM_H
