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

#define _AMX_SOUNDSCAPE_C
#include "amxIcd.h"

// É muito difícil inventar uma coisa quando ninguém inventou aquilo ainda.

// Audient means a person who listens or hears.

_AMX afxClass const* _AmxSndsGetSndClass(amxSoundscape snds)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);
    afxClass const* cls = &snds->sndCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SND);
    return cls;
}

_AMX afxError AmxSetAuditionRenderer(amxSoundscape snds, afxUnit exuIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);
    
    snds->exuIdx = exuIdx;

    return err;
}

_AMX afxError AmxUpdateAudient(amxSoundscape snds, afxTransform const* placement, afxV3d const velocity)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);

    if (placement || velocity)
    {
        if (placement)
            AfxCopyTransform(&snds->placement, placement);

        if (velocity)
            AfxV3dCopy(snds->velocity, velocity);
    }
    else
    {
        AfxResetTransform(&snds->placement);
        AfxV3dZero(snds->velocity);
    }
    return err;
}

_AMX afxError AmxRouteAudition(amxSoundscape snds, afxMixContext mix, afxUnit baseSmixIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);
    afxMixContext curr = snds->mix;

    if (curr != mix)
    {
        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &snds->mix);
            AfxDisposeObjects(1, &snds->mix);
        }

        snds->mix = mix;
        snds->baseSmixIdx = baseSmixIdx;

        if (mix)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
            AfxReacquireObjects(1, &mix);
        }
    }
    return err;
}

_AMX afxError _AmxSndsDtorCb(amxSoundscape snds)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);

    if (snds->mix)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &snds->mix);
        AmxRouteAudition(snds, NIL, 0);
    }
    
    AfxDeregisterChainedClasses(&snds->classes);
    AFX_ASSERT(AfxIsChainEmpty(&snds->classes));

    return err;
}

_AMX afxError _AmxSndsCtorCb(amxSoundscape snds, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxSoundscapeConfig const* cfg = ((amxSoundscapeConfig const*)args[1]) + invokeNo;

    snds->ambiOrder = AFX_MAX(1, cfg->ambiOrder);
    snds->flags = cfg->flags;
    snds->exuIdx = 0;
    snds->mix = NIL;
    snds->baseSmixIdx = 1;
    AfxResetTransform(&snds->placement);
    AfxV3dZero(snds->velocity);

    afxChain* classes = &snds->classes;
    AfxMakeChain(classes, (void*)snds);

    afxClassConfig clsCfg = /*cfg->sndClsCfg ? *cfg->sndClsCfg :*/ _AMX_SND_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_SND);
    AfxMountClass(&snds->sndCls, NIL, &snds->classes, &clsCfg);

    if (err)
    {
        AfxDeregisterChainedClasses(&snds->classes);
        AFX_ASSERT(AfxIsChainEmpty(&snds->classes));
    }
    return err;
}

_AMX afxClassConfig const _AMX_SNDS_CLASS_CONFIG =
{
    .fcc = afxFcc_SNDS,
    .name = "Soundscape",
    .desc = "Ambisonic Soundscape",
    .fixedSiz = sizeof(AFX_OBJECT(amxSoundscape)),
    .ctor = (void*)_AmxSndsCtorCb,
    .dtor = (void*)_AmxSndsDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireSoundscape(afxMixSystem msys, amxSoundscapeConfig const* cfg, amxSoundscape* soundscape)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(soundscape);
    AFX_ASSERT(cfg);

    afxClass* cls = (afxClass*)_AmxMsysGetSndsClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_SNDS);
    
    if (AfxAcquireObjects(cls, 1, (afxObject*)soundscape, (void const*[]) { msys, (void*)cfg }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, soundscape);
    }
    return err;
}
