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

#define _AMX_AUDIENT_C
#include "../ddi/amxImplementation.h"

// É muito difícil inventar uma coisa quando ninguém inventou aquilo ainda.

// Audient means a person who listens or hears.

_AMX afxError AmxSetAuditionRenderer(amxSoundscape snds, afxUnit exuIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);
    
    snds->exuIdx = exuIdx;

    return err;
}

_AMX afxError AmxUpdateAudient(amxSoundscape snds, afxTransform const* placement, afxV3d const velocity)
{
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);

    if (snds->mix)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &snds->mix);
        AmxRouteAudition(snds, NIL, 0);
    }

    return err;
}

_AMX afxError _AmxSndsCtorCb(amxSoundscape snds, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    snds->exuIdx = 0;
    snds->mix = NIL;
    snds->baseSmixIdx = 1;
    AfxResetTransform(&snds->placement);
    AfxV3dZero(snds->velocity);

    return err;
}

_AMX afxClassConfig const _AMX_SNDS_CLASS_CONFIG =
{
    .fcc = afxFcc_SNDS,
    .name = "Soundscape",
    .desc = "Soundscape",
    .fixedSiz = sizeof(AFX_OBJECT(amxSoundscape)),
    .ctor = (void*)_AmxSndsCtorCb,
    .dtor = (void*)_AmxSndsDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireSoundscape(afxMixSystem msys, amxSoundscape* soundscape)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxClass* cls = (afxClass*)_AmxMsysGetAudiClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_SNDS);
    amxSoundscape snds;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&snds, (void const*[]) { msys }))
    {
        AfxThrowError();
        return err;
    }
    
    AFX_ASSERT_OBJECTS(afxFcc_SNDS, 1, &snds);
    *soundscape = snds;

    return err;
}
