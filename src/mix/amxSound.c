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

#define _AMX_AUDIO_C
#define _AMX_SOUND_C
#define _AMX_MIX_C
//#define _AMX_MIX_SYSTEM_C
#include "amxIcd.h"

_AMX void AmxTransformSound(amxSound snd, afxTransform const* t)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SND, 1, &snd);
    AfxCopyTransform(&snd->t, t);

    // look for audients and relate them?
};

_AMX afxUnit AmxAccumulateSoundSamples(amxSound snd, afxUnit sampleCnt, afxUnit chCnt, afxUnit freq, afxUnit bufCap, afxReal output[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SND, 1, &snd);
    
    amxAudio aud = snd->audio;

    if (!aud)
        return 0;

    afxUnit dataLeft = aud->sampCnt - snd->offset;
    afxUnit copyLen = dataLeft;

    if (copyLen > sampleCnt)
        copyLen = sampleCnt;

    //AfxResampleAudio(aud, sampleCnt, chCnt, freq, bufCap, output);

    snd->offset += copyLen;
    return copyLen;
}

_AMX afxError AmxRewindSound(amxSound snd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SND, 1, &snd);

    snd->offset = 0;
    snd->streamPos = 0.0f;
    return 0;
}

_AMX afxBool AmxHasSoundEnded(amxSound snd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SND, 1, &snd);

    if (!(snd->flags & 0x1) && snd->offset >= snd->audio->sampCnt)
    {
        return 1;
    }
    return 0;
}

_AMX afxError _AmxSndDtorCb(amxSound snd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SND, 1, &snd);

    amxSoundscape snds = AfxGetHost(snd);



    return err;
}

_AMX afxError _AmxSndCtorCb(amxSound snd, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_SND, 1, &snd);

    amxSoundscape snds = AfxGetHost(snd);
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);
    amxSoundInfo const* spec = AFX_CAST(amxSoundInfo const*, args[1]) + invokeNo;
    AFX_ASSERT(spec);

    {
        snd->ctrl.flags = /*arxCapstanFlag_ACTIVE*/0x01;
        //snd->ctrl.timing.currClock = /*startTime*/0;
        snd->ctrl.dtLocalClockPending = 0.0;
        snd->ctrl.localClock = 0.0;
        snd->ctrl.speed = 1.0;
        snd->ctrl.localDur = /*cfg->localDur*/spec->playLen;
        snd->ctrl.currIterIdx = 0;
        snd->ctrl.iterCnt = /*iterCnt*/spec->iterCnt;
        //snd->ctrl.timing.killClock = 0;
        snd->ctrl.currWeight = 1;
        //snd->ctrl.timing.easeInStartClock = 0;
        //snd->ctrl.timing.easeInEndClock = 0;
        snd->ctrl.easeInValues = (afxUnit)-65536;
        //snd->ctrl.timing.easeOutStartClock = 0;
        //snd->ctrl.timing.easeOutEndClock = 0;
        snd->ctrl.easeOutValues = (afxUnit)0xFFFF;
    }
    snd->audio = spec->aud;
    snd->streamPos = 0;
    snd->offset = 0;
    snd->flags = 0;
    return err;
}

_AMX afxClassConfig const _AMX_SND_CLASS_CONFIG =
{
    .fcc = afxFcc_SND,
    .name = "Sound",
    .desc = "Sound Source",
    .fixedSiz = sizeof(AFX_OBJECT(amxSound)),
    .ctor = (void*)_AmxSndCtorCb,
    .dtor = (void*)_AmxSndDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxDoSound(amxSoundscape snds, amxSoundInfo const* info, afxUnit* soundId)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);
    AFX_ASSERT(soundId);
    AFX_ASSERT(info);

    amxSound snd;
    afxClass* cls = (afxClass*)_AmxSndsGetSndClass(snds);
    AFX_ASSERT_CLASS(cls, afxFcc_SND);    
    if (AfxAcquireObjects(cls, 1, (afxObject)&snd, (void const*[]) { snds, info }))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SND, 1, &snd);
    afxUnit uId = AfxGetObjectId(snd);
    *soundId = uId;

    return err;
}
