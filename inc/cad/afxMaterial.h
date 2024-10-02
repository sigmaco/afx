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

#ifndef AMX_MATERIAL_H
#define AMX_MATERIAL_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/draw/io/afxRaster.h"
#include "qwadro/inc/base/afxFixedString.h"

AMX afxString const*AfxGetMaterialUrn(afxMaterial mtl);
AMX afxNat          AfxCountMaterialMaps(afxMaterial mtl);

AMX afxMaterial     AfxFindSubmaterial(afxMaterial mtl, afxString const *usage);
AMX afxMaterial     AfxGetSubmaterial(afxMaterial mtl, afxNat mapIdx);
AMX void            AfxRebindSubmaterial(afxMaterial mtl, afxNat mapIdx, afxMaterial subMtl);

AMX void            AfxResetMaterialMap(afxMaterial mtl, afxNat mapIdx, afxString const* usage, afxMaterial subMtl);

AMX afxRaster       AfxGetMaterialTexture(afxMaterial mtl);
AMX void            AfxRebindMaterialTexture(afxMaterial mtl, afxRaster tex);
AMX void            AfxReloadMaterialTexture(afxMaterial mtl, afxUri const *tex);

// non-Granny
AMX void            AfxColorizeMaterial(afxMaterial mtl, afxV4d const color);
AMX void            AfxShineMaterial(afxMaterial mtl, afxReal shininess);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AMX afxNat          AfxEnumerateMaterials(afxSimulation sim, afxNat base, afxNat cnt, afxMaterial materials[]);

AMX afxError        AfxAcquireMaterial(afxSimulation sim, afxString const* id, afxRaster tex, afxNat subCnt, afxMaterial *mtl);

#endif//AMX_MATERIAL_H
