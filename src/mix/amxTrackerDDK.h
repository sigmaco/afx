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

#ifndef AMX_TRACKER_DDK_H
#define AMX_TRACKER_DDK_H

#include "amxBufferDDK.h"

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

AMX afxClassConfig const _AMX_VOX_CLASS_CONFIG;

AMX afxClass const* _AmxTraxGetVoxClass(amxTracker trax);

#endif//AMX_TRACKER_DDK_H
