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

// Graphics hardware is still undergoing major changes every year, and as such, there is still no standard way to represent material data for real-time rendering.
// Qwadro tries to avoid restricting your material rendering by keeping the standardized structure of materials to a minimum, and packing basically every material parameter provided by each art tool into the ExtendedData section of the afxMaterial structure.

// All professional art tools construct materials as "shader graphs", where materials are connected to each other to form a tree of shaders.
// Qwadro reproduces this data in its material data.
// Each material can have an arbitrary number of sub-materials that it uses as maps, which you can iterate over.
// Additionally, each material may be textured, and will have a reference to a afxRaster that provides the image data for the material.

#ifndef AKX_MATERIAL_H
#define AKX_MATERIAL_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/draw/afxRaster.h"
#include "qwadro/base/afxFixedString.h"

AKX afxString const*AfxGetMaterialUrn(afxMaterial mtl);
AKX afxNat          AfxCountMaterialMaps(afxMaterial mtl);

AKX afxMaterial     AfxFindSubmaterial(afxMaterial mtl, afxString const *usage);
AKX afxMaterial     AfxGetSubmaterial(afxMaterial mtl, afxNat mapIdx);
AKX void            AfxRebindSubmaterial(afxMaterial mtl, afxNat mapIdx, afxMaterial subMtl);

AKX void            AfxResetMaterialMap(afxMaterial mtl, afxNat mapIdx, afxString const* usage, afxMaterial subMtl);

AKX afxRaster       AfxGetMaterialTexture(afxMaterial mtl);
AKX void            AfxRebindMaterialTexture(afxMaterial mtl, afxRaster tex);
AKX void            AfxReloadMaterialTexture(afxMaterial mtl, afxUri const *tex);

// non-Granny
AKX void            AfxColorizeMaterial(afxMaterial mtl, afxV4d const color);
AKX void            AfxShineMaterial(afxMaterial mtl, afxReal shininess);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxError        AfxAcquireMaterial(afxSimulation sim, afxString const* id, afxRaster tex, afxNat subCnt, afxMaterial *mtl);

#endif//AKX_MATERIAL_H
