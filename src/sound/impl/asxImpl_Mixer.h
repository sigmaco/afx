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

#ifndef ASX_IMPL___MIXER_H
#define ASX_IMPL___MIXER_H

#include "qwadro/inc/sound/afxSoundSystem.h"

AFX_DECLARE_UNION(asxCmd);

#ifdef _ASX_MIXER_C
#ifdef _ASX_MIXER_IMPL
AFX_OBJECT(_asxMixer)
#else
AFX_OBJECT(afxMixer)
#endif
{
    afxUnit     portId;
    afxUnit     poolIdx;
    afxBool     disposable; /// if true, at execution end, it is moved to invalid state and considered in recycle chain.

    asxMixState state;
    afxChain    commands;
    afxArena    cmdArena; /// owned by ssys data for specific port

    afxError    (*endCb)(afxMixer);
    afxError    (*resetCb)(afxMixer, afxBool freeMem, afxBool permanent);
    afxAtom32   submCnt; /// number of submissions
    afxMask64   submQueMask; /// one for each queue where this dctx was submitted into.

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

AFX_DEFINE_STRUCT(asxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(asxCmdList)
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
    void(*f[])(void*, asxCmd const*);
};

#define ASX_GET_STD_CMD_ID(cmdName_) (offsetof(asxCmdList, cmdName_) / sizeof(void*))

AFX_DEFINE_UNION(asxCmd)
{
    asxCmdHdr hdr;
    struct
    {
        asxCmdHdr hdr;
        asxMixScope scope;
        asxMixSink AFX_SIMD chans[];
    } CommenceMixScope;
    struct
    {
        asxCmdHdr hdr;
    } ConcludeMixScope;
    struct
    {
        asxCmdHdr hdr;

        afxReal wetMix;
        afxReal roomSiz;
        afxReal width;
        afxReal damp;
        afxReal dryMix;
    } Reverb;
    struct
    {
        asxCmdHdr hdr;

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
        asxCmdHdr hdr;

        afxReal gain;
        afxReal pan;
        afxMask invChan;
    } Gainer;
    struct
    {
        asxCmdHdr hdr;

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
        asxCmdHdr hdr;

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
        asxCmdHdr hdr;

        afxReal threshold;
        afxReal ratio;
        afxReal attack;
        afxReal release;
        afxReal makeup;
    } Compressor;
    struct
    {
        asxCmdHdr hdr;

        afxUnit line;
        afxMask chanMask;
        afxUnit latency;
        afxReal pan;
        afxReal vol;
    } LineIn;
    struct
    {
        asxCmdHdr hdr;

        afxUnit src;
        afxMask chanMask;
        afxReal pan;
        afxReal vol;
    } RemoteIn;
    struct
    {
        asxCmdHdr hdr;

        afxBool muteSrc;
        afxReal amount;
        afxReal pan;
        afxUnit receiverSubmix;
    } Send;
    struct
    {
        asxCmdHdr hdr;

        afxUnit headIdx;
    } FetchAudition;
    struct
    {
        afxAudio        src;
        afxAudio        dst;
        afxWaveCopy     op;
    } TransposeAudio;
    struct
    {
        afxAudio        src;
        afxAudio        dst;
        afxWaveInterval srci;
        afxWaveInterval dsti;
    } ResampleAudio;
    struct
    {
        afxAudio buf;
        void const* src;
        afxWaveIo op;
    } UpdateAudio;
};

ASX asxCmd* _AsxMixPushCmd(afxMixer mix, afxUnit id, afxUnit siz, afxCmdId* cmdId);


ASX afxError _AsxMixResetCb(afxMixer mix, afxBool freeMem, afxBool permanent);
ASX afxError _AsxMixEndCb(afxMixer mix);

ASX afxClassConfig const _ASX_MIX_CLASS_CONFIG;

#endif//ASX_IMPL___MIXER_H
