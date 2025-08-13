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

#ifndef ASX_MATERIAL_H
#define ASX_MATERIAL_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/base/afxFixedString.h"

ASX afxString const*AfxGetMaterialUrn(afxMaterial mtl);
ASX afxUnit         AfxCountMaterialMaps(afxMaterial mtl);

ASX afxMaterial     AfxFindSubmaterial(afxMaterial mtl, afxString const *usage);
ASX afxMaterial     AfxGetSubmaterial(afxMaterial mtl, afxUnit mapIdx);
ASX void            AfxRebindSubmaterial(afxMaterial mtl, afxUnit mapIdx, afxMaterial subMtl);

ASX void            AfxResetMaterialMap(afxMaterial mtl, afxUnit mapIdx, afxString const* usage, afxMaterial subMtl);

ASX avxRaster       AfxGetMaterialTexture(afxMaterial mtl);
ASX void            AfxRebindMaterialTexture(afxMaterial mtl, avxRaster tex);
ASX void            AfxReloadMaterialTexture(afxMaterial mtl, afxUri const *tex);

// non-Granny
ASX void            AfxColorizeMaterial(afxMaterial mtl, afxV4d const color);
ASX void            AfxShineMaterial(afxMaterial mtl, afxReal shininess);

////////////////////////////////////////////////////////////////////////////////

#endif//ASX_MATERIAL_H
