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
ARX afxUnit         ArxCountMaterialMaps(arxMaterial mtl);

ARX arxMaterial     ArxFindSubmaterial(arxMaterial mtl, afxString const *usage);
ARX arxMaterial     ArxGetSubmaterial(arxMaterial mtl, afxUnit mapIdx);
ARX void            ArxRebindSubmaterial(arxMaterial mtl, afxUnit mapIdx, arxMaterial subMtl);

ARX void            ArxResetMaterialMap(arxMaterial mtl, afxUnit mapIdx, afxString const* usage, arxMaterial subMtl);

ARX avxRaster       ArxGetMaterialTexture(arxMaterial mtl);
ARX void            ArxRebindMaterialTexture(arxMaterial mtl, avxRaster tex);
ARX void            ArxReloadMaterialTexture(arxMaterial mtl, afxUri const *tex);

// non-Granny
ARX void            ArxColorizeMaterial(arxMaterial mtl, afxV4d const color);
ARX void            ArxShineMaterial(arxMaterial mtl, afxReal shininess);

////////////////////////////////////////////////////////////////////////////////


ARX afxUnit         ArxFindMaterialIndices(arxRenderware rwe, afxUnit cnt, afxString const materials[], afxUnit indices[]);

ARX afxUnit         ArxEnumerateMaterials(arxRenderware rwe, afxUnit first, afxUnit cnt, arxMaterial materials[]);

ARX afxError        ArxDeclareMaterials(arxRenderware rwe, afxUnit cnt, afxString const ids[], arxMaterial materials[]);


#endif//ARX_MATERIAL_H
