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

#define _ASX_AUDIENT_C
#include "../impl/asxImplementation.h"

// É muito difícil inventar uma coisa quando ninguém inventou aquilo ainda.

// Audient means a person who listens or hears.

_ASX afxError AfxSetAuditionRenderer(afxAudient audi, afxUnit exuIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &audi);
    
    audi->exuIdx = exuIdx;

    return err;
}

_ASX afxError AfxAdjustAudition(afxAudient audi, afxTransform const* placement, afxV3d const velocity)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &audi);

    if (placement || velocity)
    {
        if (placement)
            AfxCopyTransform(&audi->placement, placement);

        if (velocity)
            AfxV3dCopy(audi->velocity, velocity);
    }
    else
    {
        AfxResetTransform(&audi->placement);
        AfxV3dZero(audi->velocity);
    }
    return err;
}

_ASX afxError AfxRouteAudition(afxAudient audi, afxMixer mix, afxUnit baseSmixIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &audi);
    afxMixer curr = audi->mix;

    if (curr != mix)
    {
        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &audi->mix);
            AfxDisposeObjects(1, &audi->mix);
        }

        audi->mix = mix;
        audi->baseSmixIdx = baseSmixIdx;

        if (mix)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
            AfxReacquireObjects(1, &mix);
        }
    }
    return err;
}

_ASX afxError _AsxAudiStdDtorCb(afxAudient audi)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &audi);

    if (audi->mix)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &audi->mix);
        AfxRouteAudition(audi, NIL, 0);
    }

    return err;
}

_ASX afxError _AsxAudiStdCtorCb(afxAudient audi, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &audi);

    afxSoundSystem ssys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    audi->exuIdx = 0;
    audi->mix = NIL;
    audi->baseSmixIdx = 1;
    AfxResetTransform(&audi->placement);
    AfxV3dZero(audi->velocity);

    return err;
}

_ASX afxClassConfig const _ASX_AUDI_CLASS_CONFIG =
{
    .fcc = afxFcc_AUD,
    .name = "Audition",
    .desc = "Audition Head",
    .fixedSiz = sizeof(AFX_OBJECT(afxAudient)),
    .ctor = (void*)_AsxAudiStdCtorCb,
    .dtor = (void*)_AsxAudiStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxAcquireAudient(afxSoundSystem ssys, afxAudient* audition)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    afxClass* cls = (afxClass*)AsxGetAudientClass(ssys);
    AFX_ASSERT_CLASS(cls, afxFcc_AUD);
    afxAudient audi;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&audi, (void const*[]) { ssys }))
    {
        AfxThrowError();
        return err;
    }
    
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &audi);
    *audition = audi;

    return err;
}
