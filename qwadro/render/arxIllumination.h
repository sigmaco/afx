/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

#ifndef ARX_LIGHT_H
#define ARX_LIGHT_H

#include "qwadro/render/arxRenderable.h"

#define AFX_LIT_ALIGN AFX_SIMD_ALIGNMENT

// light rigs as in Doom

typedef enum
{
    // A light placed infinitely far away. It affects everything in the scene.
    akxLightType_DIRECTIONAL,

    // A light that shines equally in all directions from its location, affecting all objects within its Range.
    akxLightType_OMNI,

    // A light that shines everywhere within a cone defined by Spot Angle and Range. Only objects within this region are affected by the light.
    akxLightType_SPOT,

    // A light that shines in all directions to one side of a rectangular area of a plane. The rectangle is defined by the Width and Height properties.
    // Area lights are only available during lightmap baking and have no effect on objects at runtime.
    akxLightType_AREA
} akxLightType;

typedef enum akxLightFlag
{
    akxLightFlag_FADE_ENABLED   = AFX_BITMASK(0), // false
    akxLightFlag_NEGATIVE       = AFX_BITMASK(1), // false
    akxLightFlag_SHADOW_ENABLED = AFX_BITMASK(2), // false
    akxLightFlag_REV_CULL_FACE  = AFX_BITMASK(3), // false
} akxLightFlags;

#ifdef _ARX_LIGHT_C
AFX_OBJECT(akxLight)
{
    afxReal     distFadeBegin; // 40.f
    // A luz será suavemente esvanecida afora quando longe da câmera ativa iniciando de distFadeBegin.
    // Isto atua como uma forma de LOD. A luz será esmaecida afora sobre distFadeBegin + distFadeLen, após a qual será coligida e não enviada a shader.

    afxReal     distFadeLen; // 10.f
    // Distância sobre a qual a luz e sua sombra esmaecem. A energia da luz e a opacidade da sombra são progressivamente reduzidas sobre esta distância e é completamente invisível ao fim.
    
    afxReal     distFadeShadow; // 50.f
    // A distância da câmera na qual a sombra da luz corta (em unidades 3D).

    afxReal     angularDist; // 0.f
    
    afxUnit32    bakeMode; // 2
    avxColor    color; // [1, 1, 1, 1]
    afxMask     cullMask; // 0xFFFFFFFF
    afxReal     energy; // 1.f
    afxReal     indirectEnergy; // 1.f
    afxReal     intensLumens;
    afxReal     intensLux;
    avxRaster  projector;
    afxReal     size; // 0.f
    afxReal     specular; // 0.5f
    afxReal     temperature;
    afxReal     volFogEnergy; // 1.f
    afxReal     shadowBias; // 0.1f
    afxReal     shadowBlur; // 1.f
    afxReal     shadowNormalBias; // 2.f
    afxReal     shadowOpacity; // 1.f
    afxReal     shadowTransBias; // 0.05f

    akxLightType            type;
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
#endif//_ARX_LIGHT_C

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

ARX afxError    ArxAcquireDirectionalLights(arxRenderContext rctx, afxUnit cnt, afxUnit uid[]);
ARX afxError    ArxAcquireOmniLights(arxRenderContext rctx, afxUnit cnt, afxUnit uid[]);
ARX afxError    ArxAcquireSpotLights(arxRenderContext rctx, afxUnit cnt, afxUnit uid[]);

#endif//ARX_LIGHT_H
