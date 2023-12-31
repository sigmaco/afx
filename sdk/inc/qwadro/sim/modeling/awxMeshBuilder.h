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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MESH_BUILDER_H
#define AFX_MESH_BUILDER_H

#include "qwadro/sim/awxSimDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/math/afxAabb.h"
#include "qwadro/core/afxObject.h"
#include "qwadro/sim/awxMaterial.h"
#include "qwadro/sim/awxSkeleton.h"
#include "qwadro/io/afxUrd.h"
#include "qwadro/math/afxVertex.h"

AFX_DEFINE_STRUCT(afxVertex)
{
    afxNat              baseBiasIdx;
    afxNat              biasCnt;
};

AFX_DEFINE_STRUCT(afxCoveredTriangle)
{
    afxNat              vtxIdx[3];
    afxNat              surfIdx;
};

AFX_DEFINE_STRUCT(awxMeshBuilder)
{
    _AFX_DBG_FCC;
    afxArray            faces2;
    afxNat              triCnt;
    struct
    {
        afxNat          vtxIdx[3];
        afxNat          surfIdx;
        afxNat          artCnt;
    }*                  faces;
    afxNat              surfCnt;
    afxArray            biases;

    afxNat              artCnt;
    afxString*          pivots;
    struct
    {
        afxString       name;
        afxNat          triCnt;
    }*                  pivots2;

    afxNat              vtxCnt;
    afxVertex*          vtx;
    afxV4d*             posn;
    afxV3d*             nrm;
    afxV2d*             uv;
    afxString           id;
};

AFXINL afxError         AfxBeginMeshBuilding(awxMeshBuilder* mshb, afxString const* id, afxNat vtxCnt, afxNat triCnt, afxNat surfCnt, afxNat artCnt);
AFXINL void             AfxEndMeshBuilding(awxMeshBuilder* mshb);

AFXINL void             AfxUpdateVertices(awxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxNat const baseBiasIdx[], afxNat const biasCnt[]);
AFXINL void             AfxUpdateVertexPositions(awxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const posn[][3]);
AFXINL void             AfxUpdateVertexPositions4(awxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const posn[][4]);
AFXINL afxError         AfxUpdateVertexNormals(awxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const nrm[][3]);
AFXINL afxError         AfxUpdateVertexWraps(awxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const uv[][2]);

AFXINL void             AfxEmitTriangles(awxMeshBuilder* mshb, afxNat surfIdx, afxNat baseTriIdx, afxNat triCnt, afxNat const triVtxIdx[][3]);

AFXINL afxNat           AfxAddVertexBiases(awxMeshBuilder* mshb, afxNat cnt, afxNat const jntIdx[], afxReal const weight[]);

AFXINL void             AfxRenameVertexPivots(awxMeshBuilder* mshb, afxNat basePivotIdx, afxNat cnt, afxString const name[]);

AFX awxMesh             AfxBuildCubeMesh(afxSimulation sim, afxReal scale);
AFX awxMesh             AfxBuildParallelepipedMesh(afxSimulation sim, afxReal width, afxReal height);
AFX awxMesh             AfxBuildDomeMesh(afxSimulation sim, afxReal radius, afxNat slices);

#endif//AFX_MESH_BUILDER_H
