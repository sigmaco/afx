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

AFX_DEFINE_STRUCT(afxTerrainConfig)
{
    afxUnit width;
    afxUnit depth;
    afxUnit secWidth;
    afxUnit secHeight;
    afxUri  heightmap;
    afxReal heightScale;
};

ASX afxError    AsxDrawTerrain(afxTerrain ter, akxRenderer scn, afxDrawContext dctx);
ASX afxError    AsxDrawTerrainSector(afxTerrain ter, afxUnit secIdx, akxRenderer scn, afxDrawContext dctx);

ASX afxError    AfxResetTerrainSector(afxTerrain ter, afxUnit secIdx, afxMesh msh);

ASX afxUnit     AfxGetTerrainMeshes(afxTerrain ter, afxUnit secIdx, afxUnit cnt, afxMesh meshes[]);

ASX afxBool     AsxGetTerrainHeightAt(afxTerrain ter, afxReal x, afxReal z, afxReal* y);

////////////////////////////////////////////////////////////////////////////////

ASX afxError    AfxAcquireTerrain(afxSimulation sim, afxTerrainConfig const* cfg, afxTerrain* terrain);

ASX afxError    AfxGenerateTerrain(afxSimulation sim, afxWhd const whd, afxTerrain* terrain);

ASX afxError    AfxGenerateHeightmappedTerrain(afxSimulation sim, afxUri const* uri, afxTerrain* terrain);


ASX afxBool     RenderCells(afxTerrain ter, afxFrustum* frustum, afxBool showFaces, afxBool showDbgLines, afxDrawContext dctx);

#endif//ASX_TERRAIN_H
