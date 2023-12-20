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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_SIM_C
#define _AFX_LIGHT_C
#include "qwadro/sim/rendering/awxLight.h"
#include "qwadro/sim/awxSimulation.h"

_AFX afxError _AfxLitDtor(awxLight lit)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lit, afxFcc_LIT);

    return err;
}

_AFX afxError _AfxLitCtor(awxLight lit, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("lit=%p", lit);
    AfxAssertObjects(1, &lit, afxFcc_LIT);

    awxSimulation sim = cookie->udd[0];
    awxLightType type = *(awxLightType*)cookie->udd[1];
    lit->type = (type) ? type : awxLightType_SPOT;
    
    lit->distFadeBegin = 40.f;
    lit->distFadeLen = 10.f;
    lit->distFadeShadow = 50.f;
    lit->angularDist = 0.f;
    lit->bakeMode = 2;
    AfxSetColor(lit->color, 1, 1, 1, 1);
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
    case awxLightType_OMNI:
        lit->omni.attenuation = 1.f;
        lit->omni.range = 5.f;
        lit->omni.shadowNormalBias = 1.f;
        break;
    case awxLightType_SPOT:
        lit->spot.shadowBias = 0.03f;
        lit->spot.shadowNormalBias = 1.f;
        lit->spot.angle = 45.f;
        lit->spot.angleAttenuation = 1.f;
        lit->spot.range = 5.f;
        break;
    case awxLightType_DIRECTIONAL:
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

_AFX afxError AfxAcquireDirectionalLights(awxSimulation sim, afxNat cnt, afxNat uid[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    awxLight lit;

    for (afxNat i = 0; i < cnt; i++)
        if (AfxAcquireObjects(AfxGetLightClass(sim), 1, (afxObject*)&lit, (void const*[]) { sim, (awxLightType[]) { awxLightType_DIRECTIONAL } })) AfxThrowError();
        else
            uid[i] = AfxIdentifyObject(lit);

    return err;
}

_AFX afxError AfxAcquireOmniLights(awxSimulation sim, afxNat cnt, afxNat uid[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    awxLight lit;

    for (afxNat i = 0; i < cnt; i++)
        if (AfxAcquireObjects(AfxGetLightClass(sim), 1, (afxObject*)&lit, (void const*[]) { sim, (awxLightType[]) { awxLightType_OMNI } })) AfxThrowError();
        else
            uid[i] = AfxIdentifyObject(lit);

    return err;
}

_AFX afxError AfxAcquireSpotLights(awxSimulation sim, afxNat cnt, afxNat uid[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    awxLight lit;

    for (afxNat i = 0; i < cnt; i++)
        if (AfxAcquireObjects(AfxGetLightClass(sim), 1, (afxObject*)&lit, (void const*[]) { sim, (awxLightType[]) { awxLightType_SPOT } })) AfxThrowError();
        else
            uid[i] = AfxIdentifyObject(lit);

    return err;
}

_AFX afxClassConfig _AfxLitClsConfig =
{
    .fcc = afxFcc_LIT,
    .name = "Light",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(awxLight)),
    .mmu = NIL,
    .ctor = (void*)_AfxLitCtor,
    .dtor = (void*)_AfxLitDtor
};
