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

#ifndef AMX_IMPL___SYSTEM_H
#define AMX_IMPL___SYSTEM_H

#include "qwadro/inc/mix/afxMixSystem.h"


AFX_DEFINE_STRUCT(_amxMixSystemAcquisition)
{
    afxUnit             bridgeCnt;
    afxMixFeatures      reqFeatures;
    afxUnit             reqExtCnt;
    afxString const*    reqExts;
    void*               udd;
    afxDrawSystem       dsys;

    afxClassConfig const* mbufClsCfg;
    afxClassConfig const* wavClsCfg;
    afxClassConfig const* sndClsCfg;
    afxClassConfig const* audiClsCfg;

    afxClassConfig const* sinkClsCfg;

    afxClassConfig const* mexuClsCfg;
    afxClassConfig const* mixClsCfg;
};

#ifdef _AMX_MIX_SYSTEM_C
#ifdef _AMX_MIX_SYSTEM_IMPL
AFX_OBJECT(_amxMixSystem)
#else
AFX_OBJECT(afxMixSystem)
#endif
{
    AFX_OBJECT(afxDevLink) ctx;
    afxBool             running;
    afxUnit             bridgeCnt;
    afxMixBridge*     bridges;
    afxMixFeatures    requirements;

    //afxChain            classes;
    afxClass            mexuCls;
    afxClass            asioCls;
    afxClass            mbufCls;
    afxClass            wavCls;
    afxClass            sndCls;
    afxClass            audiCls;
    afxClass            brokCls;
    afxClass            mixCls;

    afxAudient         heads[AFX_MAX_AUDITION_HEADS];

    afxDrawSystem       dsys;

    afxV3d              listener;
    afxV3d              orientation;

    afxError(*waitCb)(afxMixSystem);

    struct _afxSctxIdd* idd;
    void*               udd; // user-defined data
};
#ifdef _AMX_MIX_SYSTEM_IMPL
//AFX_STATIC_ASSERT(offsetof(struct _afxBaseMixSystem, ctx) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxMixSystem), ctx) == 0x00, "");
#endif
#endif//_AMX_MIX_SYSTEM_C

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

#ifdef _AMX_AUDIENT_C
#ifdef _AMX_AUDIENT_IMPL
AFX_OBJECT(_amxAudient)
#else
AFX_OBJECT(afxAudient)
#endif
{
    afxTransform    placement;
    afxV3d          velocity;

    afxMixContext        mix;
    afxUnit         baseSmixIdx;
    afxUnit         exuIdx; // renderer
};
#endif

#ifdef _AMX_MIX_C
#ifdef _AMX_SOUND_IMPL
AFX_OBJECT(_amxSound)
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

#ifdef _AMX_WAVEFORM_C
#ifdef _AMX_WAVEFORM_IMPL
AFX_OBJECT(_amxAudio)
#else
AFX_OBJECT(afxAudio)
#endif
{
    amxFormat       fmt; // type and bits per sample
    afxUnit         sampCnt; // Number of samples (e.g., time slots)
    // its length, meaning the number of sample frames inside the buffer.
    afxUnit         chanCnt; // Number of channels (e.g., stereo = 2)
    //afxUnit         frameCnt; // Number of frames (playable intervals)
    afxUnit         freq; // the sample rate, the number of sample frames played per second.

    afxUnit         fmtBps;
    afxUnit         fmtStride;
    void*           udd;
    
    avxBufferFlags  bufFlags;
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

#ifdef _AMX_BUFFER_C
#ifdef _AMX_BUFFER_IMPL
AFX_OBJECT(_amxBuffer)
#else
AFX_OBJECT(amxBuffer)
#endif
{
    amxFormat       fmt; // type and bits per sample
    afxUnit         sampCnt; // Number of samples (e.g., time slots)
    // its length, meaning the number of sample frames inside the buffer.
    afxUnit         chanCnt; // Number of channels (e.g., stereo = 2)
    //afxUnit         frameCnt; // Number of frames (playable intervals)
    afxUnit         freq; // the sample rate, the number of sample frames played per second.

    afxUnit         fmtBps;
    afxUnit         fmtStride;
    void*           udd;

    avxBufferFlags  bufFlags;
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
    afxError(*bufRemap)(afxAudio, afxUnit, afxUnit, afxFlags, void**);
};
#endif

AMX afxClass const* _AmxGetMixBridgeClass(afxMixSystem msys);
AMX afxClass const* _AmxGetAudioSinkClass(afxMixSystem msys);

AMX afxClassConfig const _AMX_ASIO_CLASS_CONFIG;

AMX afxClassConfig const _AMX_MBUF_CLASS_CONFIG;
AMX afxClassConfig const _AMX_SND_CLASS_CONFIG;
AMX afxClassConfig const _AMX_WAV_CLASS_CONFIG;
AMX afxClassConfig const _AMX_AUDI_CLASS_CONFIG;

AMX afxClassConfig const _AMX_MSYS_CLASS_CONFIG;

AMX afxClass const* _AmxGetMixContextClass(afxMixSystem msys);
AMX afxClass const* _AmxGetAudioClass(afxMixSystem msys);
AMX afxClass const* _AmxGetBufferClass(afxMixSystem msys);
AMX afxClass const* _AmxGetSoundClass(afxMixSystem msys);
AMX afxClass const* _AmxGetAudientClass(afxMixSystem msys);

#endif//AMX_IMPL___SYSTEM_H
