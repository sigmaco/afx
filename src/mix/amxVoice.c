/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AMX_MIX_C
#define _AMX_AUDIO_C
#define _AMX_VOICE_C
//#define _AMX_MIX_SYSTEM_C
#include "../ddi/amxImplementation.h"

// This is the sound of Z.

typedef enum amxVoiceState
{
    amxVoiceState_STOP,
    amxVoiceState_PLAY,
    amxVoiceState_PAUSE,
} amxVoiceState;

_AMX afxError _AmxToggleVoice(amxVoice vox, amxVoiceState state)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    vox->offset = 0;
    vox->streamPos = 0.0f;

    vox->frameCursPosn = 0;
    vox->framesProced = 0;

    return 0;
}

_AMX void AmxTransformVoice(amxVoice vox, afxTransform const* t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);
    AfxCopyTransform(&vox->t, t);

    // look for audients and relate them?
};

_AMX afxUnit AmxAccumulateVoiceSamples(amxVoice vox, afxUnit sampleCnt, afxUnit chCnt, afxUnit freq, afxUnit bufCap, afxReal output[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);
    
    amxAudio aud = vox->audio;

    if (!aud)
        return 0;

    afxUnit dataLeft = aud->sampCnt - vox->offset;
    afxUnit copyLen = dataLeft;

    if (copyLen > sampleCnt)
        copyLen = sampleCnt;

    //AfxResampleAudio(aud, sampleCnt, chCnt, freq, bufCap, output);

    vox->offset += copyLen;

    return copyLen;
}

_AMX afxError AmxRewindVoice(amxVoice vox)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    vox->offset = 0;
    vox->streamPos = 0.0f;

    return 0;
}

_AMX afxBool AmxHasVoiceEnded(amxVoice vox)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    if (!(vox->flags & 0x1) && vox->offset >= vox->audio->sampCnt)
    {
        return 1;
    }

    return 0;
}

_AMX afxError _AmxVoxDtorCb(amxVoice vox)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    afxMixContext mix = AfxGetProvider(vox);
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);



    return err;
}

_AMX afxError _AmxVoxCtorCb(amxVoice vox, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    afxMixContext mix = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    //amxVoiceInfo const* spec = AFX_CAST(amxVoiceInfo const*, args[1]) + invokeNo;
    //AFX_ASSERT(spec);

    {
        vox->ctrl.flags = /*arxCapstanFlag_ACTIVE*/0x01;
        //vox->ctrl.timing.currClock = /*startTime*/0;
        vox->ctrl.dtLocalClockPending = 0.0;
        vox->ctrl.localClock = 0.0;
        vox->ctrl.speed = 1.0;
        vox->ctrl.localDur = /*cfg->localDur*/NIL;//spec->playLen;
        vox->ctrl.currIterIdx = 0;
        vox->ctrl.iterCnt = /*iterCnt*/NIL;//spec->iterCnt;
        //vox->ctrl.timing.killClock = 0;
        vox->ctrl.currWeight = 1;
        //vox->ctrl.timing.easeInStartClock = 0;
        //vox->ctrl.timing.easeInEndClock = 0;
        vox->ctrl.easeInValues = (afxUnit)-65536;
        //vox->ctrl.timing.easeOutStartClock = 0;
        //vox->ctrl.timing.easeOutEndClock = 0;
        vox->ctrl.easeOutValues = (afxUnit)0xFFFF;
    }

    vox->audio = NIL;// spec->aud;
    vox->streamPos = 0;
    vox->offset = 0;
    vox->flags = 0;

    return err;
}

_AMX afxClassConfig const _AMX_VOX_CLASS_CONFIG =
{
    .fcc = afxFcc_VOX,
    .name = "Voice",
    .desc = "Mix In Voice",
    .fixedSiz = sizeof(AFX_OBJECT(amxVoice)),
    .ctor = (void*)_AmxVoxCtorCb,
    .dtor = (void*)_AmxVoxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxDeallocateVoices(afxMixContext mix, afxUnit firstId, afxUnit cnt, afxUnit voiceIdMap[])
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    afxClass* cls = (afxClass*)_AmxMixGetVoxClass(mix);
    AFX_ASSERT_CLASS(cls, afxFcc_VOX);

    for (afxUnit i = cnt; i--> 0;)
    {
        afxUnit id = voiceIdMap ? voiceIdMap[i] + firstId : firstId + i;

        amxVoice vox;
        if (AfxEnumerateObjects(cls, id, 1, &vox))
        {
            AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);
            AfxDisposeObjects(1, &vox);
        }
    }

    return err;
}

_AMX afxError AmxAllocateVoices(afxMixContext mix, amxVoiceCaps caps, afxUnit cnt, afxUnit voiceIds[])
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    AFX_ASSERT(voiceIds);

    afxClass* cls = (afxClass*)_AmxMixGetVoxClass(mix);
    AFX_ASSERT_CLASS(cls, afxFcc_VOX);

    amxVoice voices[32];
    static const afxUnit batchSiz = 32;
    afxUnit remainCnt = cnt % batchSiz;
    afxUnit batchCnt = cnt / batchSiz;
    afxUnit baseArrIdx = 0;
    for (afxUnit i = 0; i < batchCnt; i++)
    {
        if (AfxAcquireObjects(cls, batchSiz, (afxObject)voices, (void const*[]) { mix, NIL }))
        {
            AfxThrowError();
            for (afxUnit j = i; j--> 0;)
            {
                for (afxUnit k = batchSiz; k--> 0;)
                {
                    amxVoice vox;
                    if (AfxEnumerateObjects(cls, voiceIds[(batchSiz * j) + k], 1, &vox))
                    {
                        AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);
                        AfxDisposeObjects(1, &vox);
                    }
                    else
                    {
                        AfxThrowError();
                    }
                }
            }
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_VOX, batchSiz, voices);
            for (afxUnit j = 0; j < batchCnt; j++)
            {
                afxUnit uId = AfxGetObjectId(voices[j]);
                voiceIds[baseArrIdx + j] = uId;
            }
            baseArrIdx += batchSiz;
        }
    }

    if (AfxAcquireObjects(cls, remainCnt, (afxObject)voices, (void const*[]) { mix, NIL }))
    {
        AfxThrowError();
        for (afxUnit j = batchCnt; j-- > 0;)
        {
            for (afxUnit k = batchSiz; k-- > 0;)
            {
                amxVoice vox;
                if (AfxEnumerateObjects(cls, voiceIds[(batchSiz * j) + k], 1, &vox))
                {
                    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);
                    AfxDisposeObjects(1, &vox);
                }
                else
                {
                    AfxThrowError();
                }
            }
        }
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_VOX, remainCnt, voices);
        for (afxUnit j = 0; j < remainCnt; j++)
        {
            afxUnit uId = AfxGetObjectId(voices[j]);
            voiceIds[baseArrIdx + j] = uId;
        }
        baseArrIdx += batchSiz;
    }

    return err;
}
