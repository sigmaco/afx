/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_SIM_C
#define _ASX_LIGHT_C
#include "impl/asxImplementation.h"
#include "qwadro/inc/sim/akxLighting.h"

_ASX afxError _AfxLitDtor(akxLight lit)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_LIT, 1, &lit);

    return err;
}

_ASX afxError _AfxLitCtor(akxLight lit, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_LIT, 1, &lit);

    afxSimulation sim = args[0];
    akxLightType type = *(akxLightType*)args[1];
    lit->type = (type) ? type : akxLightType_SPOT;
    
    lit->distFadeBegin = 40.f;
    lit->distFadeLen = 10.f;
    lit->distFadeShadow = 50.f;
    lit->angularDist = 0.f;
    lit->bakeMode = 2;
    AvxMakeColor(lit->color, 1, 1, 1, 1);
    lit->cullMask = 0xFFFFFFFF;
    lit->energy = 1.f;
    lit->indirectEnergy = 1.f;
    lit->projector = NIL;
    lit->size = 0.f;
    lit->specular = 0.5f;    
    lit->volFogEnergy = 1.f;
    lit->shadowBias = 0.1f;
    lit->shadowBlur = 1.f;
    lit->shadowNormalBias = 2.f;
    lit->shadowOpacity = 1.f;
    lit->shadowTransBias = 0.05f;

    switch(type)
    {
    case akxLightType_OMNI:
        lit->omni.attenuation = 1.f;
        lit->omni.range = 5.f;
        lit->omni.shadowNormalBias = 1.f;
        break;
    case akxLightType_SPOT:
        lit->spot.shadowBias = 0.03f;
        lit->spot.shadowNormalBias = 1.f;
        lit->spot.angle = 45.f;
        lit->spot.angleAttenuation = 1.f;
        lit->spot.range = 5.f;
        break;
    case akxLightType_DIRECTIONAL:
        lit->dir.shadowBlendSplits = FALSE;
        lit->dir.shadowFadeStart = 0.8;
        lit->dir.shadowMaxDist = 100.f;
        lit->dir.shadowPancakeSiz = 20.f;
        lit->dir.shadowSplit1 = 0.1f;
        lit->dir.shadowSplit2 = 0.2f;
        lit->dir.shadowSplit3 = 0.5f;
        break;
    default: AfxThrowError(); break;
    };
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxAcquireDirectionalLights(afxSimulation sim, afxUnit cnt, afxUnit uid[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    akxLight lit;

    for (afxUnit i = 0; i < cnt; i++)
        if (AfxAcquireObjects((afxClass *)_AsxGetLightClass(sim), 1, (afxObject*)&lit, (void const*[]) { sim, (akxLightType[]) { akxLightType_DIRECTIONAL } })) AfxThrowError();
        else
            uid[i] = AfxGetObjectId(lit);

    return err;
}

_ASX afxError AfxAcquireOmniLights(afxSimulation sim, afxUnit cnt, afxUnit uid[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    akxLight lit;

    for (afxUnit i = 0; i < cnt; i++)
        if (AfxAcquireObjects((afxClass *)_AsxGetLightClass(sim), 1, (afxObject*)&lit, (void const*[]) { sim, (akxLightType[]) { akxLightType_OMNI } })) AfxThrowError();
        else
            uid[i] = AfxGetObjectId(lit);

    return err;
}

_ASX afxError AfxAcquireSpotLights(afxSimulation sim, afxUnit cnt, afxUnit uid[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    akxLight lit;

    for (afxUnit i = 0; i < cnt; i++)
        if (AfxAcquireObjects((afxClass *)_AsxGetLightClass(sim), 1, (afxObject*)&lit, (void const*[]) { sim, (akxLightType[]) { akxLightType_SPOT } })) AfxThrowError();
        else
            uid[i] = AfxGetObjectId(lit);

    return err;
}

_ASX afxClassConfig const _ASX_LIT_CLASS_CONFIG =
{
    .fcc = afxFcc_LIT,
    .name = "Light",
    .fixedSiz = sizeof(AFX_OBJECT(akxLight)),
    .ctor = (void*)_AfxLitCtor,
    .dtor = (void*)_AfxLitDtor
};
