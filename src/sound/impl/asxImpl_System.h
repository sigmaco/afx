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

#ifndef ASX_IMPL___SYSTEM_H
#define ASX_IMPL___SYSTEM_H

#include "qwadro/inc/sound/afxSoundSystem.h"

#define AFX_MAX_SOUND_BRIDGE_PER_CONTEXT 16
#define AFX_MAX_SOUND_QUEUE_PER_BRIDGE 32

AFX_DEFINE_STRUCT(_asxSoundSystemAcquisition)
{
    afxUnit             bridgeCnt;
    afxSoundFeatures    requirements;
    afxUnit             extensionCnt;
    afxString const*    extensions;
    void*               udd;

    afxClassConfig const* wavClsCfg;
    afxClassConfig const* sndClsCfg;
    afxClassConfig const* audiClsCfg;

    afxClassConfig const* sinkClsCfg;

    afxClassConfig const* sexuClsCfg;
    afxClassConfig const* mixClsCfg;
};

#ifdef _ASX_SOUND_SYSTEM_C
#ifdef _ASX_SOUND_SYSTEM_IMPL
AFX_OBJECT(_asxSoundSystem)
#else
AFX_OBJECT(afxSoundSystem)
#endif
{
    AFX_OBJECT(afxContext) ctx;
    afxBool             running;
    afxUnit             bridgeCnt;
    afxSoundBridge*     bridges;
    afxSoundFeatures    requirements;

    //afxChain            classes;
    afxClass            sexuCls;
    afxClass            asioCls;
    afxClass            wavCls;
    afxClass            sndCls;
    afxClass            audiCls;
    afxClass            brokCls;
    afxClass            mixCls;

    afxAudient         heads[AFX_MAX_AUDITION_HEADS];

    afxV3d              listener;
    afxV3d              orientation;

    afxError(*waitCb)(afxSoundSystem);

    struct _afxSctxIdd* idd;
    void*               udd; // user-defined data
};
#ifdef _ASX_SOUND_SYSTEM_IMPL
//AFX_STATIC_ASSERT(offsetof(struct _afxBaseSoundSystem, ctx) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxSoundSystem), ctx) == 0x00, "");
#endif
#endif//_ASX_SOUND_SYSTEM_C

AFX_DEFINE_STRUCT(afxTrackedNote)
{
    afxAudio    aud;
    afxUnit     srcId;
    afxUnit     iterCnt;
    afxUnit     sampleOffset;
    afxUnit     sampleCnt;
    afxUnit     frameIdx;

    afxUnit     playBegin; // sample index
    afxUnit     playLen; // sample count
    afxUnit     loopBegin; // sample index
    afxUnit     loopLen; // sample count
    afxUnit     loopCnt;
};

#ifdef _ASX_AUDIENT_C
#ifdef _ASX_AUDIENT_IMPL
AFX_OBJECT(_asxAudient)
#else
AFX_OBJECT(afxAudient)
#endif
{
    afxTransform    placement;
    afxV3d          velocity;

    afxMixer        mix;
    afxUnit         baseSmixIdx;
    afxUnit         exuIdx; // renderer
};
#endif

#ifdef _ASX_SOUND_C
#ifdef _ASX_SOUND_IMPL
AFX_OBJECT(_asxSound)
#else
AFX_OBJECT(afxSound)
#endif
{
    afxReal gain; // A value of 0.0 is meaningless with respect to a logarithmic scale; it is silent.
    afxBool looping;
    afxV3d position;
    afxV3d velocity;
    afxV3d direction;
    afxReal pitch; // A multiplier for the frequency (sample rate) of the source's buffer.
    afxReal innerConeAngle; // The angle covered by the inner cone, where the source will not attenuate.
    afxReal outerConeAngle; // The angle covered by the outer cone, where the source will be fully attenuated.

    afxBool playing;
    afxSphere bounding;

    afxTransform t;
    afxLink playQueue;

    struct
    {
        afxFlags   flags;
        afxReal         dtLocalClockPending;
        afxReal         localClock;
        afxReal         speed;
        afxReal         localDur;
        afxInt          currIterIdx;
        afxInt          iterCnt;
        afxReal         currWeight;
        afxCapstanTiming  timing;
        afxUnit32       easeInValues;
        afxUnit32       easeOutValues;
        void*           userData[4];
    } ctrl;
    afxAudio    audio;
    afxUnit     chanCnt;
    afxUnit     offset;
    afxFlags    flags;
    afxReal     streamPos;
};
#endif

#ifdef _ASX_WAVEFORM_C
#ifdef _ASX_WAVEFORM_IMPL
AFX_OBJECT(_asxAudio)
#else
AFX_OBJECT(afxAudio)
#endif
{
    asxFormat       fmt; // type and bits per sample
    afxUnit         sampCnt; // Number of samples (e.g., time slots)
    // its length, meaning the number of sample frames inside the buffer.
    afxUnit         chanCnt; // Number of channels (e.g., stereo = 2)
    //afxUnit         frameCnt; // Number of frames (playable intervals)
    afxUnit         freq; // the sample rate, the number of sample frames played per second.

    afxUnit         fmtBps;
    afxUnit         fmtStride;
    void*           udd;
    
    afxBufferFlags  bufFlags;
    afxUnit         bufCap; // with any alignment
    union
    {
        void*       data;
        afxByte*    bytemap;
        afxReal32*  samples32f;
        afxInt16*   samples16i;
    };
    afxUnit         mappedOffset;
    afxUnit         mappedRange;
    afxFlags        mappedFlags;
    afxBool         sysmemBuffered;
    afxAtom32       pendingRemap;
    afxError        (*bufRemap)(afxAudio, afxUnit, afxUnit, afxFlags, void**);
};
#endif

ASX afxClass const* _AsxGetSoundBridgeClass(afxSoundSystem ssys);
ASX afxClass const* AsxGetAudioSinkClass(afxSoundSystem ssys);

ASX afxClassConfig const _ASX_ASIO_CLASS_CONFIG;

ASX afxClassConfig const _ASX_SND_CLASS_CONFIG;
ASX afxClassConfig const _ASX_WAV_CLASS_CONFIG;
ASX afxClassConfig const _ASX_AUDI_CLASS_CONFIG;

ASX afxClassConfig const _ASX_SSYS_CLASS_CONFIG;

ASX afxClass const* AsxGetMixerClass(afxSoundSystem ssys);
ASX afxClass const* AsxGetAudioClass(afxSoundSystem ssys);
ASX afxClass const* AsxGetSoundClass(afxSoundSystem ssys);
ASX afxClass const* AsxGetAudientClass(afxSoundSystem ssys);

#endif//ASX_IMPL___SYSTEM_H
