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

#ifndef AKX_TERRAIN_H
#define AKX_TERRAIN_H

#include "qwadro/cad/afxModel.h"
#include "qwadro/sim/rendering/akxLighting.h"

AFX_DEFINE_HANDLE(akxTerrain);

#ifdef _AKX_TERRAIN_C
AFX_OBJECT(akxTerrain)
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
};
#endif//_AKX_TERRAIN_C

AKX afxError AkxDrawTerrain(akxTerrain ter, akxRenderer scn, avxCmdb cmdb);

////////////////////////////////////////////////////////////////////////////////

AKX afxError    AfxAcquireTerrain(afxSimulation sim, akxTerrain* terrain);

#endif//AKX_TERRAIN_H
