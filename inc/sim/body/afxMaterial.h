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

/*
    A afxMaterial refers to a set of properties that define how an object's surface 
    interacts with light, texture, and other physical properties in the simulated environment. 
    Materials are used to give objects their appearance and simulate how they would look and 
    behave in the real world under various conditions.
*/

#ifndef AMX_MATERIAL_H
#define AMX_MATERIAL_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/draw/io/afxRaster.h"
#include "qwadro/inc/base/afxFixedString.h"

AMX afxString const*AfxGetMaterialUrn(afxMaterial mtl);
AMX afxUnit         AfxCountMaterialMaps(afxMaterial mtl);

AMX afxMaterial     AfxFindSubmaterial(afxMaterial mtl, afxString const *usage);
AMX afxMaterial     AfxGetSubmaterial(afxMaterial mtl, afxUnit mapIdx);
AMX void            AfxRebindSubmaterial(afxMaterial mtl, afxUnit mapIdx, afxMaterial subMtl);

AMX void            AfxResetMaterialMap(afxMaterial mtl, afxUnit mapIdx, afxString const* usage, afxMaterial subMtl);

AMX afxRaster       AfxGetMaterialTexture(afxMaterial mtl);
AMX void            AfxRebindMaterialTexture(afxMaterial mtl, afxRaster tex);
AMX void            AfxReloadMaterialTexture(afxMaterial mtl, afxUri const *tex);

// non-Granny
AMX void            AfxColorizeMaterial(afxMaterial mtl, afxV4d const color);
AMX void            AfxShineMaterial(afxMaterial mtl, afxReal shininess);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AMX afxUnit         AfxEnumerateMaterials(afxSimulation sim, afxUnit first, afxUnit cnt, afxMaterial materials[]);

AMX afxError        AfxAcquireMaterial(afxSimulation sim, afxString const* id, afxRaster tex, afxUnit subCnt, afxMaterial *mtl);

#endif//AMX_MATERIAL_H
