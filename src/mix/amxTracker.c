/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AMX_MIX_C
#define _AMX_MIX_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_BRIDGE_C
#define _AMX_SINK_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_TRACKER_C
#define _AMX_AUDIO_C
#include "amxIcd.h"
#include "qwadro/mix/amxTracker.h"

_AMX afxClass const* _AmxTraxGetVoxClass(amxTracker trax)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);
    afxClass const* cls = &trax->voxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VOX);
    return cls;
}

_AMX afxCmdId AmxBindIoStream(amxTracker trax, afxUnit pin, afxStream iob, afxSize offset, afxSize range, afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    trax->srcs[pin].type = afxFcc_IOB;
    trax->srcs[pin].posn = 0;
    trax->srcs[pin].rs = (ResampleState) { 0 };
    trax->srcs[pin].rs.needs_more_input = 1;
    trax->srcs[pin].rs.ratio = 1.0;
    trax->srcs[pin].iob.iob = iob;
    trax->srcs[pin].iob.offset = offset;
    trax->srcs[pin].iob.range = range;
    trax->srcs[pin].iob.stride = stride;
}

_AMX afxCmdId AmxBindBufferedStream(amxTracker trax, afxUnit pin, amxBuffer buf, afxSize offset, afxSize range, afxUnit freq, amxFormat fmt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    trax->srcs[pin].type = afxFcc_MBUF;
    trax->srcs[pin].posn = 0;
    trax->srcs[pin].rs = (ResampleState) { 0 };
    trax->srcs[pin].rs.needs_more_input = 1;
    trax->srcs[pin].rs.ratio = 1.0;
    trax->srcs[pin].buf.buf = buf;
    trax->srcs[pin].buf.offset = offset;
    trax->srcs[pin].buf.range = range;
    trax->srcs[pin].buf.stride = freq;
    trax->srcs[pin].fmt = fmt;
}

_AMX afxCmdId AmxBindAudioTrack(amxTracker trax, afxUnit track, amxAudio aud, afxUnit srcIdx, amxFormat fmt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    trax->tracks[track].type = afxFcc_AUD;
    trax->tracks[track].posn = 0;
    trax->tracks[track].fmt = fmt;
    trax->tracks[track].srcIdx = srcIdx;
    trax->tracks[track].aud.aud = aud;
}

_AMX afxCmdId AmxBindBuffer(amxTracker trax, afxUnit bank, amxBuffer buf, afxUnit offset, afxUnit range, afxUnit freq)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    trax->banks[bank].type = afxFcc_MBUF;
    trax->banks[bank].buf.buf = buf;
    trax->banks[bank].buf.offset = offset;
    trax->banks[bank].buf.range = range;
}

_AMX afxCmdId AmxBindAudio(amxTracker trax, afxUnit bank, amxAudio aud)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    trax->banks[bank].type = afxFcc_AUD;
    trax->banks[bank].aud.aud = aud;
}

AMX afxCmdId AmxBindSink(amxTracker trax, afxSink sink)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);
    trax->sink = sink;
}

_AMX afxError AmxStartTracker(amxTracker trax, afxBool resume)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    trax->playing = TRUE;

    if (!trax->executor.chain)
    {
        afxMixSystem msys = AfxGetHost(trax);
        AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

        AfxPushLink(&trax->executor, &msys->activeTrackers);
        //trax->state = amxContextState_PENDING;
        trax->motor.localClock = 0;
        trax->motor.localDur = 1;
        trax->motor.iterCnt = 1;

        afxMixBridge mexu;
        AFX_ASSERT_RANGE(msys->bridgeCnt, /*ctrl->exuIdx*/0, 1);
        if (!AmxGetMixBridges(msys, /*ctrl->exuIdx*/0, 1, &mexu))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

        mexu->pingCb(mexu, 0);
    }
    return err;
}

_AMX afxError AmxStopTracker(amxTracker trax, afxBool suspend)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    trax->playing = suspend ? trax->playing : FALSE;
    trax->paused = suspend ? TRUE : trax->paused;

    if (trax->executor.chain)
        AfxPopLink(&trax->executor);

    return err;
}

_AMX afxError AmxExhaustTracker(amxTracker trax, afxFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    if (AmxStopTracker(trax, FALSE))
        AfxThrowError();

    return err;
}

_AMX afxError _AmxTraxDtorCb(amxTracker trax)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    if (AmxExhaustTracker(trax, NIL))
        AfxThrowError();

    AfxExhaustChainedClasses(&trax->classes);
    AfxDeregisterChainedClasses(&trax->classes);

    return err;
}

_AMX afxError _AmxTraxCtorCb(amxTracker trax, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(args[1]);
    amxTrackerConfig const* cfg = ((amxTrackerConfig const *)args[1]) + invokeNo;

    trax->tag = cfg->tag;
    trax->udd = cfg->udd;

    trax->playing = FALSE;
    trax->paused = FALSE;

    AfxMakeChain(&trax->classes, trax);
    AfxMountClass(&trax->voxCls, NIL, &trax->classes, &_AMX_VOX_CLASS_CONFIG);

    afxUnit voices[32];
    if (AmxAllocateVoices(trax, NIL, AFX_MAX(1, AFX_MIN(32, cfg->voiceCnt)), voices))
        AfxThrowError();

    if (err)
    {
        AfxDeregisterChainedClasses(&trax->classes);
    }
    return err;
}

_AMX afxClassConfig const _AMX_TRAX_CLASS_CONFIG =
{
    .fcc = afxFcc_TRAX,
    .name = "Tracker",
    .desc = "Tracker",
    .fixedSiz = sizeof(AFX_OBJECT(amxTracker)),
    .ctor = (void*)_AmxTraxCtorCb,
    .dtor = (void*)_AmxTraxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireTracker(afxMixSystem msys, amxTrackerConfig const* cfg, amxTracker* tracker)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(tracker);

    afxClass* cls = (afxClass*)_AmxMsysGetTraxClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_TRAX);
    
    amxTracker trax;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&trax, (void const*[]) { msys, cfg }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);
    AFX_ASSERT(tracker);
    *tracker = trax;

    return err;
}
