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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_LIGHT_H
#define AFX_LIGHT_H

#include "qwadro/sim/awxNode.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/draw/afxColor.h"

#define AFX_LIT_ALIGN AFX_SIMD_ALIGN

typedef enum
{
    /// A light placed infinitely far away. It affects everything in the scene.
    awxLightType_DIRECTIONAL,

    /// A light that shines equally in all directions from its location, affecting all objects within its Range.
    awxLightType_OMNI,

    /// A light that shines everywhere within a cone defined by Spot Angle and Range. Only objects within this region are affected by the light.
    awxLightType_SPOT,

    /// A light that shines in all directions to one side of a rectangular area of a plane. The rectangle is defined by the Width and Height properties.
    /// Area lights are only available during lightmap baking and have no effect on objects at runtime.
    awxLightType_AREA
} awxLightType;

typedef enum awxLightFlag
{
    awxLightFlag_FADE_ENABLED   = AFX_BIT_OFFSET(0), // false
    awxLightFlag_NEGATIVE       = AFX_BIT_OFFSET(1), // false
    awxLightFlag_SHADOW_ENABLED = AFX_BIT_OFFSET(2), // false
    awxLightFlag_REV_CULL_FACE  = AFX_BIT_OFFSET(3), // false
} awxLightFlags;

#ifdef _AFX_LIGHT_C
AFX_OBJECT(awxLight)
{
    afxReal     distFadeBegin; // 40.f
    /// A luz será suavemente esvanecida afora quando longe da câmera ativa iniciando de distFadeBegin.
    /// Isto atua como uma forma de LOD. A luz será esmaecida afora sobre distFadeBegin + distFadeLen, após a qual será coligida e não enviada a shader.

    afxReal     distFadeLen; // 10.f
    /// Distância sobre a qual a luz e sua sombra esmaecem. A energia da luz e a opacidade da sombra são progressivamente reduzidas sobre esta distância e é completamente invisível ao fim.
    
    afxReal     distFadeShadow; // 50.f
    /// A distância da câmera na qual a sombra da luz corta (em unidades 3D).

    afxReal     angularDist; // 0.f
    
    afxNat32    bakeMode; // 2
    afxColor    color; // [1, 1, 1, 1]
    afxMask     cullMask; // 0xFFFFFFFF
    afxReal     energy; // 1.f
    afxReal     indirectEnergy; // 1.f
    afxReal     intensLumens;
    afxReal     intensLux;
    afxRaster  projector;
    afxReal     size; // 0.f
    afxReal     specular; // 0.5f
    afxReal     temperature;
    afxReal     volFogEnergy; // 1.f
    afxReal     shadowBias; // 0.1f
    afxReal     shadowBlur; // 1.f
    afxReal     shadowNormalBias; // 2.f
    afxReal     shadowOpacity; // 1.f
    afxReal     shadowTransBias; // 0.05f

    awxLightType            type;
    union
    {
        struct
        {
            afxReal attenuation; // 1.f
            afxReal range; // 5.f
            // shadow mode
            afxReal shadowNormalBias; // 1.0
        } omni;
        struct
        {
            afxReal shadowBias; // 0.03f
            afxReal shadowNormalBias; // 1.f
            afxReal angle; // 45.f
            afxReal angleAttenuation; // 1.f
            afxReal range; // 5.f
        } spot;
        struct
        {
            afxBool shadowBlendSplits; // false
            afxReal shadowFadeStart; // 0.8
            afxReal shadowMaxDist; // 100.f
            // shadow mode
            afxReal shadowPancakeSiz; // 20.f
            afxReal shadowSplit1; // 0.1f
            afxReal shadowSplit2; // 0.2f
            afxReal shadowSplit3; // 0.5f
            // sky mode
        } dir;
    };
};
#endif//_AFX_LIGHT_C

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxError    AfxAcquireDirectionalLights(afxSimulation sim, afxNat cnt, afxNat uid[]);
AKX afxError    AfxAcquireOmniLights(afxSimulation sim, afxNat cnt, afxNat uid[]);
AKX afxError    AfxAcquireSpotLights(afxSimulation sim, afxNat cnt, afxNat uid[]);

#endif//AFX_LIGHT_H
