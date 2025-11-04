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

#ifndef ARX_TERRAIN_H
#define ARX_TERRAIN_H

#include "qwadro/cad/arxModel.h"

AFX_DEFINE_STRUCT(arxTerrainConfig)
{
    afxUnit width;
    afxUnit depth;
    afxUnit secWidth;
    afxUnit secHeight;
    afxUri  heightmap;
    afxReal heightScale;
};

ARX afxError    ArxResetTerrainSector(arxTerrain ter, afxUnit secIdx, arxMesh msh);

ARX afxUnit     ArxGetTerrainMeshes(arxTerrain ter, afxUnit secIdx, afxUnit cnt, arxMesh meshes[]);

ARX afxBool     ArxGetTerrainHeightAt(arxTerrain ter, afxReal x, afxReal z, afxReal* y);

////////////////////////////////////////////////////////////////////////////////

ARX afxError    ArxAcquireTerrain(arxRenderContext rctx, arxTerrainConfig const* cfg, arxTerrain* terrain);

ARX afxError    ArxGenerateTerrain(arxRenderContext rctx, afxWarp const whd, arxTerrain* terrain);

ARX afxError    ArxGenerateHeightmappedTerrain(arxRenderContext rctx, afxUri const* uri, arxTerrain* terrain);


#endif//ARX_TERRAIN_H
