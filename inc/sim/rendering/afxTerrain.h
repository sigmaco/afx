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

#include "qwadro/inc/cad/afxModel.h"
#include "qwadro/inc/sim/rendering/akxLighting.h"

#ifdef _AMX_TERRAIN_C
AFX_OBJECT(afxTerrain)
{
    afxBuffer   vbo[2];
    afxNat      vboBase[2];
    afxNat      vboRange[2];
    afxNat      vboStride[2];
    afxBuffer   ibo;
    afxNat      iboRange;
    afxNat      iboStride;

    afxNat      width, depth;
    afxReal     step;
    afxReal*    height;

    afxMesh*    mesh;
    afxNat*     indices;
    afxNat      num_indices;
    afxV3d*     vertices;
    afxV3d*     normals;
    afxNat      vertex_count;

    afxNat      rez;
    afxNat      numStrips;
    afxNat      numTrisPerStrip;

    afxNat      secCnt;
    struct
    {
        afxMesh msh;
    }           *sectors;
};
#endif//_AMX_TERRAIN_C

AMX afxError    AkxDrawTerrain(afxTerrain ter, akxRenderer scn, avxCmdb cmdb);
AMX afxError    AkxDrawTerrainSector(afxTerrain ter, afxNat secIdx, akxRenderer scn, avxCmdb cmdb);

AMX afxError    AfxResetTerrainSector(afxTerrain ter, afxNat secIdx, afxMesh msh);

////////////////////////////////////////////////////////////////////////////////

AMX afxError    AfxAcquireTerrain(afxSimulation sim, afxNat secCnt, afxTerrain* terrain);

AMX afxError    AfxGenerateTerrain(afxSimulation sim, afxWhd const whd, afxTerrain* terrain);

#endif//AMX_TERRAIN_H
