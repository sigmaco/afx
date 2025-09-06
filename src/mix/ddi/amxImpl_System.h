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

#ifndef AMX_IMPL___SYSTEM_H
#define AMX_IMPL___SYSTEM_H

#include "qwadro/mix/afxMixSystem.h"
#include "amxImpl_Executor.h"
#include "amxImpl_Soundscape.h"

#ifndef _AMX_MIX_C
AFX_DECLARE_STRUCT(_amxMsysImpl);
#else
AFX_DEFINE_STRUCT(_amxMsysImpl)
{
    afxError(*waitCb)(afxMixSystem, afxUnit64);
    //afxError(*waitFencCb)(afxMixSystem, afxBool, afxUnit64, afxUnit, amxFence const[]);
    //afxError(*resetFencCb)(afxMixSystem, afxUnit, amxFence const[]);
    afxError(*cohereCb)(afxMixSystem, afxBool, afxUnit, amxBufferedMap const[]);
    afxError(*remapCb)(afxMixSystem, afxBool, afxUnit, _amxBufferRemapping const[]);
    afxUnit(*getProcCb)(afxMixSystem, afxUnit, afxString const[], void*[]);
    afxError(*transferCb)(afxMixSystem dsys, amxTransference* ctrl, afxUnit opCnt, void const* ops);
    afxError(*allocBufCb)(afxMixSystem, afxUnit, amxBufferInfo const[], amxBuffer[]);
    afxError(*deallocBufCb)(afxMixSystem, afxUnit, amxBuffer[]);

    afxClass const*(*mexuCls)(afxMixSystem);
    afxClass const*(*fencCls)(afxMixSystem);
    afxClass const*(*sinkCls)(afxMixSystem);
    afxClass const*(*bufCls)(afxMixSystem);
};
#endif


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
    afxClassConfig const* sndsClsCfg;

    afxClassConfig const* vidClsCfg;
    afxClassConfig const* viddClsCfg;

    afxClassConfig const* traxClsCfg;
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
    _amxMsysImpl const* ddi;
    afxBool             running;
    afxUnit             bridgeCnt;
    afxMixBridge*       bridges;
    afxMask             ioExuMask;
    afxMask             dedIoExuMask;
    afxMask             cfxExuMask;
    afxMask             dedCfxExuMask;
    afxMask             gfxExuMask;
    afxMask             videoExuMask;
    afxMixFeatures      requirements;
    afxMixLimits const* limits;

    //afxChain            classes;
    afxClass            mexuCls;
    afxClass            asioCls;
    afxClass            mbufCls;
    afxClass            wavCls;
    afxClass            sndsCls;
    afxClass            brokCls;
    afxClass            vidCls;
    afxClass            viddCls;
    afxClass            mixCls;
    afxClass            traxCls;
    afxClass            mcdcCls;
    afxClass            msrcCls;
    afxClass            msnkCls;

    amxSoundscape         heads[AFX_MAX_AUDITION_HEADS];

    afxDrawSystem       dsys;

    afxV3d              listener;
    afxV3d              orientation;

    afxChain        activeTrackers;
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
    afxUnit         segCnt; // Number of frames (playable intervals)
    afxUnit         freq; // the sample rate, the number of sample frames played per second.

    afxUnit         fmtBps;
    afxUnit         fmtStride;
    
    afxUnit64 current_sample_time;

    afxString       tag;
    void*           udd;
    
    amxBufferFlags  bufFlags;
    afxUnit         bufCap; // with any alignment
    amxBuffer       buf;
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
    afxUnit     mapRefCnt;
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
    afxUnit         fmtBps;
    afxUnit         fmtStride;

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
AMX afxClass const* _AmxMsysGetTraxClass(afxMixSystem msys);

AMX afxClassConfig const _AMX_MCDC_CLASS_CONFIG;
AMX afxClassConfig const _AMX_ASIO_CLASS_CONFIG;
AMX afxClassConfig const _AMX_TRAX_CLASS_CONFIG;

AMX afxClassConfig const _AMX_MBUF_CLASS_CONFIG;
AMX afxClassConfig const _AMX_SND_CLASS_CONFIG;
AMX afxClassConfig const _AMX_AUD_CLASS_CONFIG;
AMX afxClassConfig const _AMX_SNDS_CLASS_CONFIG;

AMX afxClassConfig const _AMX_VID_CLASS_CONFIG;
AMX afxClassConfig const _AMX_VIDD_CLASS_CONFIG;

AMX afxClassConfig const _AMX_MSYS_CLASS_CONFIG;

AMX afxClass const* _AmxMsysGetMixClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetAudClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetBufClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetSndsClass(afxMixSystem msys);

AMX afxClass const* _AmxMsysGetVidClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetViddClass(afxMixSystem msys);

AMX _amxMsysImpl const* _AmxMsysGetImpl(afxMixSystem msys);
AMX afxMask _AmxMsysGetIoExuMask(afxMixSystem msys, afxMask* dedIoExuMask);

#endif//AMX_IMPL___SYSTEM_H
