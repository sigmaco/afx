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

// This file is part of Acceleration for RenderWare on Qwadro.

#ifndef ARX_MATERIAL_H
#define ARX_MATERIAL_H

/*
    A arxMaterial refers to a set of properties that define how an object's surface 
    interacts with light, texture, and other physical properties in the simulated environment. 
    Materials are used to give objects their appearance and simulate how they would look and 
    behave in the real world under various conditions.
*/

#include "qwadro/render/arxRenderable.h"

ARX afxString const*ArxGetMaterialUrn(arxMaterial mtl);

ARX afxUnit         ArxGetMaterialTextures(arxMaterial mtl, afxUnit baseIdx, afxUnit cnt, avxRaster rasters[], avxSampler samplers[]);
ARX afxError        ArxBindMaterialTextures(arxMaterial mtl, afxUnit baseIdx, afxUnit cnt, avxRaster rasters[]);
ARX void            ArxReloadMaterialTexture(arxMaterial mtl, afxUri const *tex);

AFX_DEFINE_STRUCT(arxMaterialInfo)
{
    afxUnit baseMtlId;

    // Used by rendering and shading:
    // PBR-compatible parameters
    afxV3d      albedo; // base color --- (0.7, 0.7, 0.75)
    afxReal     metallic; // [ 0, 1 ] --- (ex.: metal = 1.0)
    afxReal     roughness; // [ 0, 1 ] --- (ex.: metal = 0.25)
    // Reflectivity for real-time ray-traced reflections or environment mapping
    afxReal     reflectivity; // [ 0, 1 ] --- (ex.: metal = 0.9)
    // Texture maps (albedo, normal, specular, etc.)
    afxUnit     albedoMapId;
    afxUnit     normalMapId;
    afxUnit     specularMapId;

    // The coefficient of friction is a measure of the frictional force between two surfaces in contact, represented by the ratio of 
    // the frictional force to the normal force pressing the surfaces together. It varies depending on the materials involved and 
    // can be classified into static (when surfaces are at rest) and kinetic (when surfaces are moving) coefficients.
    // The coefficient of restitution, is the ratio of the relative velocity of separation after collision to the relative velocity 
    // of approach before collision. It can aIso be defined as the square root of the ratio of the final kinetic energy to the initial 
    // kinetic energy. It normally ranges from 0 to 1 where 1 would be a perfectly elastic collision. A perfectly inelastic collision 
    // has a coefficient of 0, but a 0 value does not have to be perfectly inelastic. It is measured in the Leeb rebound hardness test.
    // Used by physics and collision systems:
    // mass per cubic meter (affects inertia, buoyancy).
    afxReal     density; // kg/m³ or unitless (ex.: metal = 7850)
    // affects motion resistance.
    afxReal     staticFriction; // [ 0, 1 ] (ex.: metal = 0.6)
    afxReal     dynFriction; // [ 0, 1 ] (ex.: metal = 0.4)
    // bounce response.
    afxReal     restitution; // [ 0, 1 ] (ex.: metal = 0.2)
    // used for deformation, penetration, and wear simulation
    afxReal     hardness; // (ex.: metal = 7.5)

    // Used by audio generation systems:
    // supports sound propagation through solids.
    afxReal     sndSpeed; // (ex.: metal = 5960)
    // Impact sound variations for different intensity ranges
    afxUnit     impactSndId[3]; // (ex.: metal = "sounds/impact/[metal_light,metal_medium,metal_heavy].ogg")
    // Scrape / drag sounds
    afxUnit     scrapeSndId; // (ex.: metal = "sounds/scrape/metal.ogg")
    // Resonance frequency (for ringing sounds on impacts)
    afxUnit     acousticResonanceFreq; // (ex.: metal = 440)
    // Damping factor --- how fast a vibration dies out.
    afxReal     acousticDampingFactor; // (ex.: metal = 0.3)

    // Thermal
    // heat simulation
    afxReal thermalCondutivity; // (ex.: metal = 50.2)
    afxUnit heatCapacity; // (ex.: metal = 450)
    afxReal emissivity; // (ex.: metal = 0.8)
    afxReal ThermalExpansion; // (ex.: metal = 0.00001)
    // Thermal reactivity
    afxReal meltTemperature; // (ex.: metal = 1000)
    afxReal colorChangeTempThreshold; // (ex.: metal = 500)

    // Electrical
    afxReal electricalConductivity; // (ex.: metal = 5.8e7)
    afxReal electricalResistance; // (ex.: metal = 0.005)
    // Magnetic
    afxReal magneticPermeability; // (ex.: metal = 1.26e-6)
    afxReal magneticRemanence; // (ex.: metal = 1.2)
    afxUnit curieTemperature; // (ex.: metal = 800)

    // Damage
    afxReal crackGrowthRate; // (ex.: metal = 0.01)
    afxReal fractureToughness; // (ex.: metal = 300)
    afxReal wearFactor; // (ex.: metal = 0.2)
};

ARX void            ArxUpdateMaterial(arxMaterial mtl, arxMaterialInfo const* info);

////////////////////////////////////////////////////////////////////////////////

ARX afxUnit             ArxEnumerateMaterials
(
    arxMtd mtd,
    afxUnit             first, 
    afxUnit             cnt, 
    arxMaterial         materials[]
);

ARX afxUnit             ArxFindMaterials
(
    arxMtd mtd,
    afxUnit             cnt,
    afxString const     materials[],
    afxUnit             ids[],
    arxMaterial         handles[]
);

ARX afxError            ArxDeclareMaterials
(
    arxMtd mtd,
    afxUnit             cnt, 
    afxString const     ids[], 
    arxMaterial         materials[]
);


AFX_DEFINE_STRUCT(arxMaterialLayout)
{
    afxReal     density; // kg/m³ or unitless
    afxReal     friction; // [ 0, 1 ]
    afxReal     restitution; // [ 0, 1 ]

    union
    {
        struct
        {
            // Visual
            afxV3d      albedo; // base color --- (1.0, 0.5, 0.3)
            afxReal     metallic; // [ 0, 1 ] --- 0.1
            afxReal     roughness; // [ 0, 1 ] --- 0.4
            avxRaster   texture; // optional
            avxRaster   normal; // optional
        } pbr;
    };
};

ARX afxError ArxAcquireMtds
(
    arxScenario scio, 
    afxUnit cnt, 
    afxString const names[], 
    arxMaterialLayout const layouts[], 
    arxMtd mtds[]
);

ARX afxError ArxLoadMtd(arxScenario scio, afxUri const* uri, arxMtd* pMtd);

#endif//ARX_MATERIAL_H
