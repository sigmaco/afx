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

#ifndef AMX_CONTEXT_DDK_H
#define AMX_CONTEXT_DDK_H

#include "qwadro/mix/afxMixSystem.h"

typedef enum amxContextState
{
    amxContextState_INITIAL,
    amxContextState_RECORDING,
    amxContextState_EXECUTABLE,
    amxContextState_PENDING,
    amxContextState_INVALID,
    amxContextState_INTERNAL_EXECUTING
} amxContextState;

AFX_DECLARE_UNION(_amxCmd);

AFX_DECLARE_STRUCT(_amxIddMctx);

#ifndef _AMX_MIX_C
AFX_DECLARE_STRUCT(_amxDdiMctx);
#else
AFX_DEFINE_STRUCT(_amxDdiMctx)
{
    afxError(*prepare)(afxMixContext, afxBool purge, amxCmdFlags);
    afxError(*compile)(afxMixContext);
    afxError(*recycle)(afxMixContext, afxBool freeMem);
    afxError(*exhaust)(afxMixContext, afxBool freeMem);
};
#endif

AFX_DEFINE_STRUCT(_amxCmdBatch)
{
    afxFcc          fcc;
    afxUnit         uniqId;
    afxLink         recyc;
    afxBool         once; // if true, at execution end, it is moved to invalid state and considered in recycle chain.
    afxBool         deferred;
    afxAtom32       submCnt; // number of submissions
    afxMask64       submQueMask; // one for each queue where this dctx was submitted into.
    afxChain        commands;
    afxArena        cmdArenaCompiled;
};

#ifdef _AMX_MIX_CONTEXT_C
#ifdef _AMX_MIX_CONTEXT_IMPL
AFX_OBJECT(_amxMixContext)
#else
AFX_OBJECT(afxMixContext)
#endif
{
    _amxDdiMctx const*  ddi;
    _amxIddMctx*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;
    amxLimits const* devLimits; // dbg copies
    amxFeatures const*enabledFeatures; // dbg copies

    amxAptitude         caps;
    afxMask             exuMask;
    afxAtom32           submCnt; // number of submissions
    afxMask64           submQueMask; // one for each queue where this dctx was submitted into.

    amxContextFlags     flags;
    afxMixContext       pool;
    afxChain            classes;
    afxClass            mctxCls;
    afxInterlockedQueue recycQue;
    amxCmdFlags         cmdFlags;
    amxContextState     state;
    afxArena            cmdArena; // owned by dsys data for specific port
    afxChain            commands;

    // leftovers
    afxUnit     frameIdx;
    afxBool     disabled; // suspend processing
    afxReal     spanning; // 0 --- center
    afxReal     volume; // 0.000 dB
    afxReal     width; // 0.000 %
    afxReal     delay; // 0.000 ms --- unable in a master or send track
    afxBool     muted; // suspend routing the output to the endpoint.
    afxSink     routing;
    afxUnit     queuedFrameCnt;
    afxUnit64   current_sample_time;
    struct
    {
        /*arxCapstanFlags*/afxFlags   flags;
        afxReal         dtLocalClockPending;
        afxReal         localClock;
        afxReal         speed;
        afxReal         localDur;
        afxInt          currIterIdx;
        afxInt          iterCnt;
        afxReal         currWeight;
        struct {
            afxReal         currClock;
            afxReal         killClock;
            afxReal         easeInStartClock;
            afxReal         easeInEndClock;
            afxReal         easeOutStartClock;
            afxReal         easeOutEndClock;
        }  timing;
        afxUnit32       easeInValues;
        afxUnit32       easeOutValues;
        void*           userData[4];
    } motor;

};
#endif

AFX_DEFINE_STRUCT(_amxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(_amxCmdLut)
{
    struct
    {
        void* _reservedToBeZero;
        void* Reverb;
        void* Phaser;
        void* Gainer;
        void* Flanger;
        void* Analog;
        void* Compressor;
        void* LineIn;
        void* RemoteIn;
        void* Send;
        void* FetchAudition;
        void* ResampleAudio;
        void* ResampleBufferedAudio;
        void* TransposeAudio;
        void* UpdateAudio;
        void* CommenceMixScope;
        void* ConcludeMixScope;
    };
    void(*f[])(void*, _amxCmd const*);
};

#define _AMX_CMD_ID(cmdName_) (offsetof(_amxCmdLut, cmdName_) / sizeof(void*))

AFX_DEFINE_UNION(_amxCmd)
{
    _amxCmdHdr hdr;

    struct
    {
        _amxCmdHdr hdr;

        afxFlags            flags;
        amxAudio            sink;
        afxUnit             baseSamp;
        afxUnit             baseChan;
        afxUnit             baseSeg;
        afxUnit             sampCnt;
        afxUnit             chanCnt;
        afxUnit             segCnt;
        amxMixTarget AFX_SIMD chans[];
    } CommenceMixScope;
    struct
    {
        _amxCmdHdr hdr;

    } ConcludeMixScope;
    struct
    {
        _amxCmdHdr hdr;

        afxUnit voice;
        afxReal wetMix;
        afxReal roomSiz;
        afxReal width;
        afxReal damp;
        afxReal dryMix;
    } Reverb;
    struct
    {
        _amxCmdHdr hdr;

        afxUnit voice;
        afxReal floor;
        afxReal ceil;
        afxReal rate;
        afxReal feedback;
        afxReal depth;
        afxReal phase;
        afxUnit stageCnt;
    } Phaser;
    struct
    {
        _amxCmdHdr hdr;

        afxUnit voice;
        afxReal gain;
        afxReal pan;
        afxMask invChan;
    } Gainer;
    struct
    {
        _amxCmdHdr hdr;

        afxUnit voice;
        afxReal amount;
        afxReal rate;
        afxReal amplitude;
        afxReal feedback;
        afxReal delay;
        afxReal phase;
        afxUnit flt;
    } Flanger;
    struct
    {
        _amxCmdHdr hdr;

        afxUnit voice;
        afxUnit type;
        afxUnit flt;
        afxReal cutoff;
        afxReal resonance;
        afxReal inertia;
        afxReal drive;
        afxUnit oversamp;
        afxReal start;
        afxReal end;
    } Analog;
    struct
    {
        _amxCmdHdr hdr;

        afxUnit voice;
        afxReal threshold;
        afxReal ratio;
        afxReal attack;
        afxReal release;
        afxReal makeup;
    } Compressor;
    struct
    {
        _amxCmdHdr hdr;

        afxUnit voice;
        afxUnit line;
        afxMask chanMask;
        afxUnit latency;
        afxReal pan;
        afxReal vol;
    } LineIn;
    struct
    {
        _amxCmdHdr hdr;

        afxUnit voice;
        afxUnit src;
        afxMask chanMask;
        afxReal pan;
        afxReal vol;
    } RemoteIn;
    struct
    {
        _amxCmdHdr hdr;

        afxUnit voice;
        afxBool muteSrc;
        afxReal amount;
        afxReal pan;
        afxUnit receiverSubmix;
    } Send;
    struct
    {
        _amxCmdHdr hdr;

        afxUnit voice;
        afxUnit headIdx;
    } FetchAudition;
    struct
    {
        _amxCmdHdr hdr;

        amxAudio        src;
        amxAudio        dst;
        amxAudioCopy     op;
    } TransposeAudio;
    struct
    {
        _amxCmdHdr hdr;

        amxAudio        src;
        amxAudio        dst;
        amxAudioPeriod srcp;
        amxAudioPeriod dstp;
    } ResampleAudio;
    struct
    {
        _amxCmdHdr hdr;

        amxBufferedAudio src;
        amxBufferedAudio dst;
        amxAudioPeriod srcp;
        amxAudioPeriod dstp;
    } ResampleBufferedAudio;
    struct
    {
        _amxCmdHdr hdr;

        amxAudio buf;
        void const* src;
        amxAudioIo op;
    } UpdateAudio;
};

AMX afxError _AmxMctxPushCmd(afxMixContext mix, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxCmd** ppCmd);
AMX _amxCmdBatch* _AmxMctxGetCmdBatch(afxMixContext mix, afxUnit idx);
AMX afxError _AmxMixResetCb(afxMixContext mix, afxBool freeMem, afxBool permanent);
AMX afxError _AmxMixEndCb(afxMixContext mix);

AMX afxClassConfig const _AMX_MCTX_CLASS_CONFIG;

AMX _amxDdiMctx const _AMX_MCTX_DDI;

AMX amxContextState _AmxMctxGetStatus(afxMixContext mix);

#endif//AMX_CONTEXT_DDK_H
