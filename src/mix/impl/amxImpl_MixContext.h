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

#ifndef AMX_IMPL___MIX_CONTEXT_H
#define AMX_IMPL___MIX_CONTEXT_H

#include "qwadro/inc/mix/afxMixSystem.h"

AFX_DECLARE_UNION(_amxCmd);

#ifdef _AMX_MIX_CONTEXT_C
#ifdef _AMX_MIX_CONTEXT_IMPL
AFX_OBJECT(_amxMixContext)
#else
AFX_OBJECT(afxMixContext)
#endif
{
    afxUnit     portId;
    afxUnit     poolIdx;
    afxBool     disposable; // if true, at execution end, it is moved to invalid state and considered in recycle chain.

    amxMixState state;
    afxChain    commands;
    afxArena    cmdArena; // owned by msys data for specific port

    afxError    (*endCb)(afxMixContext);
    afxError    (*resetCb)(afxMixContext, afxBool freeMem, afxBool permanent);
    afxAtom32   submCnt; // number of submissions
    afxMask64   submQueMask; // one for each queue where this dctx was submitted into.

    //afxUnit     latency; // samples per audio frame (256, 512, 1024, or 2048).
    //afxUnit     chanCnt; // 2 --- stereo
    //afxUnit     freq; // 48kHz --- sample rate
    //afxUnit     bps; // 32-bit --- bits per sample

    afxUnit     frameIdx;

    afxBool     disabled; // suspend processing

    afxReal     spanning; // 0 --- center
    afxReal     volume; // 0.000 dB
    afxReal     width; // 0.000 %
    afxReal     delay; // 0.000 ms --- unable in a master or send track

    afxBool     muted; // suspend routing the output to the endpoint.
    afxSink     routing;


    afxLink     executor;
    struct
    {
        /*afxCapstanFlags*/afxFlags   flags;
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

        amxMixScope scope;
        amxMixSink AFX_SIMD chans[];
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

        amxAudio buf;
        void const* src;
        amxAudioIo op;
    } UpdateAudio;
};

AMX afxError _AmxMixPushCmd(afxMixContext mix, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxCmd** ppCmd);

AMX afxError _AmxMixResetCb(afxMixContext mix, afxBool freeMem, afxBool permanent);
AMX afxError _AmxMixEndCb(afxMixContext mix);

AMX afxClassConfig const _AMX_MIX_CLASS_CONFIG;

#endif//AMX_IMPL___MIX_CONTEXT_H
