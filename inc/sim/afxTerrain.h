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

#ifndef AMX_TERRAIN_H
#define AMX_TERRAIN_H

#include "qwadro/inc/sim/body/afxModel.h"
#include "qwadro/inc/sim/akxLighting.h"

#ifdef _AMX_TERRAIN_C
AFX_OBJECT(afxTerrain)
{
    afxUnit      secCnt;
    struct
    {
        afxUnit  width, depth;
        afxMesh msh;
    }           *sectors;
};
#endif//_AMX_TERRAIN_C

AMX afxError    AmxDrawTerrain(afxTerrain ter, akxRenderer scn, afxDrawContext dctx);
AMX afxError    AmxDrawTerrainSector(afxTerrain ter, afxUnit secIdx, akxRenderer scn, afxDrawContext dctx);

AMX afxError    AfxResetTerrainSector(afxTerrain ter, afxUnit secIdx, afxMesh msh);

AMX afxUnit     AfxGetTerrainMeshes(afxTerrain ter, afxUnit secIdx, afxUnit cnt, afxMesh meshes[]);

////////////////////////////////////////////////////////////////////////////////

AMX afxError    AfxAcquireTerrain(afxSimulation sim, afxUnit secCnt, afxTerrain* terrain);

AMX afxError    AfxGenerateTerrain(afxSimulation sim, afxWhd const whd, afxTerrain* terrain);

AMX afxError    AfxGenerateHeightmappedTerrain(afxSimulation sim, afxUri const* uri, afxTerrain* terrain);

#endif//AMX_TERRAIN_H
