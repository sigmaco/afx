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

#ifndef AMX_IMPL___MIX_CONTEXT_H
#define AMX_IMPL___MIX_CONTEXT_H

#include "qwadro/mix/afxMixSystem.h"

typedef struct
{
    float input_buf[1024];   // float-converted Doom samples
    int in_len;              // number of input frames available
    int in_pos;              // read head
    double frac_pos;         // fractional resample phase
    double ratio;            // 48000.0 / 11025.0
    afxBool needs_more_input;   // flag to fetch next Doom chunk
} ResampleState;

#ifdef _AMX_VOICE_C
#ifdef _AMX_VOICE_IMPL
AFX_OBJECT(_amxVoice)
#else
AFX_OBJECT(amxVoice)
#endif
{
#if 0
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

    afxSize     frameCursPosn;
    afxSize     framesProced;
#endif
    // xa2
    afxBool     playing2;
    afxBool     paused;
    afxBool     looping2;
    afxReal     freqRatio;
    afxUnit     sampRate;
    afxUnit64   samplesPlayed;
    afxUnit     bufQueued;
    afxInterlockedQueue bufQue;
    amxBuffer   srcBuf;
    afxSize     srcOffset;
    afxSize     srcRange;
    afxUnit     srcStride;
    afxSize     srcCursor;
    afxUnit     iterBegin;
    afxUnit     iterEnd;
    afxUnit     iterCnt;
    afxUnit     iterIdx;

    ResampleState rs;
};
#endif

#ifdef _AMX_TRACKER_C
#ifdef _AMX_TRACKER_IMPL
AFX_OBJECT(_amxTracker)
#else
AFX_OBJECT(amxTracker)
#endif
// This interface is the primary interface that applications use to control the pipeline.
{
    afxString   tag;
    void*       udd;

    afxBool     playing;
    afxBool     paused;

    struct
    {
        afxFcc type;
        amxFormat fmt;
        union
        {
            struct
            {
                amxBuffer buf;
                afxSize offset;
                afxSize range;
            } buf;
            struct
            {
                amxAudio aud;
            } aud;
            struct
            {
                //amxVideo vid;
                int a;
            } vid;
        };
    } banks[4];
    afxUnit bankCnt;
    struct
    {
        afxFcc type;
        afxSize posn;
        amxFormat fmt;
        ResampleState rs;
        union
        {
            struct
            {
                afxStream iob;
                afxSize offset;
                afxSize range;
                afxUnit stride;
            } iob;
            struct
            {
                amxBuffer buf;
                afxSize offset;
                afxSize range;
                afxUnit stride;
            } buf;
        };
    } srcs[4];
    afxUnit srcCnt;
    struct
    {
        afxFcc type;
        afxSize posn;
        afxUnit srcIdx;
        afxUnit location;
        amxFormat fmt;
        afxFlags flags;
        union
        {
            struct
            {
                amxBuffer buf;
                afxSize offset;
                afxSize range;
            } buf;
            struct
            {
                amxAudio aud;
            } aud;
            struct
            {
                //amxVideo vid;
                int a;
            } vid;
        };
    } tracks[4];
    afxUnit trackCnt;

    afxSink sink;
    afxLink     executor;

    afxChain    classes;
    afxClass     voxCls;

    struct
    {
        /*arxCapstanFlags*/afxFlags flags;
        afxReal dtLocalClockPending;
        afxReal localClock;
        afxReal speed;
        afxReal localDur;
        afxInt currIterIdx;
        afxInt iterCnt;
        afxReal currWeight;
        struct {
            afxReal currClock;
            afxReal killClock;
            afxReal easeInStartClock;
            afxReal easeInEndClock;
            afxReal easeOutStartClock;
            afxReal easeOutEndClock;
        }  timing;
        afxUnit32 easeInValues;
        afxUnit32 easeOutValues;
        void* userData[4];
    } motor;
};
#endif

AFX_DECLARE_UNION(_amxCmd);

#ifndef _AMX_MIX_C
AFX_DECLARE_STRUCT(_amxMctxDdi);
#else
AFX_DEFINE_STRUCT(_amxMctxDdi)
{
    afxError(*compose)(afxMixContext, afxBool once, afxBool deferred, afxUnit* bid);
    afxError(*compile)(afxMixContext, afxUnit bid);
    afxError(*recycle)(afxMixContext, afxUnit bid, afxBool freeMem);
    afxError(*discard)(afxMixContext, afxBool freeMem);
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
    _amxMctxDdi const*ddi;
    afxString   tag;
    void*       udd;
    afxUnit     portId;
    afxUnit     poolIdx;
    afxBool     disposable; // if true, at execution end, it is moved to invalid state and considered in recycle chain.
    
    afxPool     batches;
    afxChain    cmdbRecycChain;
    afxFutex    cmdbReqLock;
    afxBool     cmdbLockedForReq;

    amxMixState state;
    afxChain    commands;
    afxArena    cmdArena; // owned by msys data for specific port

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

        amxAudio buf;
        void const* src;
        amxAudioIo op;
    } UpdateAudio;
};

AMX afxError _AmxMixPushCmd(afxMixContext mix, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxCmd** ppCmd);
AMX _amxCmdBatch* _AmxGetCmdBatch(afxMixContext mix, afxUnit idx);
AMX afxError _AmxMixResetCb(afxMixContext mix, afxBool freeMem, afxBool permanent);
AMX afxError _AmxMixEndCb(afxMixContext mix);

AMX afxClassConfig const _AMX_MIX_CLASS_CONFIG;
AMX afxClassConfig const _AMX_VOX_CLASS_CONFIG;

AMX _amxMctxDdi const _AMX_MCTX_DDI;

AMX afxClass const* _AmxTraxGetVoxClass(amxTracker trax);


AMX void resample_stream(ResampleState* state, float* out, int out_count);

AMX afxSize amxProcessVoice(amxVoice vs, float* outBuf, afxSize outFrames, afxUnit stride);

#endif//AMX_IMPL___MIX_CONTEXT_H
