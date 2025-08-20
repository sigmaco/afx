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


AFX_DEFINE_STRUCT(_amxMsysAcquisition)
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

    afxClassConfig const* msesClsCfg;
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
    afxClass            msesCls;
    afxClass            mcdcCls;
    afxClass            msrcCls;
    afxClass            msnkCls;

    amxSoundscape         heads[AFX_MAX_AUDITION_HEADS];

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
    amxAudio    aud;
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
AFX_OBJECT(amxSoundscape)
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
AFX_OBJECT(amxSound)
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
        //arxCapstanTiming  timing;
        afxUnit32       easeInValues;
        afxUnit32       easeOutValues;
        void*           userData[4];
    } ctrl;
    amxAudio    audio;
    afxUnit     chanCnt;
    afxUnit     offset;
    afxFlags    flags;
    afxReal     streamPos;
};
#endif

#ifdef _AMX_AUDIO_C
#ifdef _AMX_WAVEFORM_IMPL
AFX_OBJECT(_amxAudio)
#else
AFX_OBJECT(amxAudio)
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
    
    afxString       tag;
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
    afxError        (*bufRemap)(amxAudio, afxUnit, afxUnit, afxFlags, void**);
};
#endif

AFX_DEFINE_STRUCT(_amxBufStorage)
{
    afxLink iommu;
    // binding
    afxUnit mmu; // memory holder
    afxSize offset; // offset into memory holder bound to this storage block.
    // Persistent mapping required at acquisition. Do not allow unmapping.
    afxBool     permanentlyMapped;
    afxAtom32   pendingRemap;
    afxSize     mapOffset;
    afxUnit     mapRange;
    afxFlags    mapFlags;
    afxByte*    mapPtr; // used by mapping
    union
    {
        afxSize     addr;
        afxByte*    bytemap;
        afxUnit8*   u8;
        afxUnit16*  u16;
        afxUnit32*  u32;
        afxInt8*    i8;
        afxInt16*   i16;
        afxInt32*   i32;
        afxReal32*  f32;
        afxReal64*  f64;
        afxV2d*     f32v2;
        afxV3d*     f32v3;
        afxV4d*     f32v4;
    } hostedAlloc;
};

#ifdef _AMX_BUFFER_C
#ifdef _AMX_BUFFER_IMPL
AFX_OBJECT(_amxBuffer)
#else
AFX_OBJECT(amxBuffer)
#endif
{
    afxString       tag;
    void*           udd;
    amxBuffer       base; // handle used by subacquision of another buffer
    afxSize         from; // offset used by subacquision of another buffer
    //afxUnit         size; // capacity
    amxBufferUsage  usage;
    amxBufferFlags  flags;
    amxFormat       fmt; // for tensor buffer

    afxMask         sharingMask;

    // STORAGE
    // required size of this storage block.
    afxSize         reqSiz;
    afxUnit         reqAlign;
    // required memory conditions for this storage block.
    afxFlags        reqMemType;
    _amxBufStorage  storage[1]; // non-sparse
};
#endif

AMX afxClass const* _AmxMsysGetMexuClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetSinkClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetSesClass(afxMixSystem msys);

AMX afxClassConfig const _AMX_MCDC_CLASS_CONFIG;
AMX afxClassConfig const _AMX_ASIO_CLASS_CONFIG;
AMX afxClassConfig const _AMX_MSES_CLASS_CONFIG;

AMX afxClassConfig const _AMX_MBUF_CLASS_CONFIG;
AMX afxClassConfig const _AMX_SND_CLASS_CONFIG;
AMX afxClassConfig const _AMX_AUD_CLASS_CONFIG;
AMX afxClassConfig const _AMX_SNDS_CLASS_CONFIG;

AMX afxClassConfig const _AMX_MSYS_CLASS_CONFIG;

AMX afxClass const* _AmxMsysGetMixClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetAudClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetBufClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetSndClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetAudiClass(afxMixSystem msys);

#endif//AMX_IMPL___SYSTEM_H
