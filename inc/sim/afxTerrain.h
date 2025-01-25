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

#ifndef ASX_TERRAIN_H
#define ASX_TERRAIN_H

#include "qwadro/inc/sim/io/afxModel.h"
#include "qwadro/inc/sim/akxLighting.h"

#ifdef _ASX_TERRAIN_C
AFX_OBJECT(afxTerrain)
{
    afxUnit      secCnt;
    struct
    {
        afxUnit  width, depth;
        afxMesh msh;
    }           *sectors;
};
#endif//_ASX_TERRAIN_C

ASX afxError    AsxDrawTerrain(afxTerrain ter, akxRenderer scn, afxDrawContext dctx);
ASX afxError    AsxDrawTerrainSector(afxTerrain ter, afxUnit secIdx, akxRenderer scn, afxDrawContext dctx);

ASX afxError    AfxResetTerrainSector(afxTerrain ter, afxUnit secIdx, afxMesh msh);

ASX afxUnit     AfxGetTerrainMeshes(afxTerrain ter, afxUnit secIdx, afxUnit cnt, afxMesh meshes[]);

////////////////////////////////////////////////////////////////////////////////

ASX afxError    AfxAcquireTerrain(afxSimulation sim, afxUnit secCnt, afxTerrain* terrain);

ASX afxError    AfxGenerateTerrain(afxSimulation sim, afxWhd const whd, afxTerrain* terrain);

ASX afxError    AfxGenerateHeightmappedTerrain(afxSimulation sim, afxUri const* uri, afxTerrain* terrain);

#endif//ASX_TERRAIN_H
