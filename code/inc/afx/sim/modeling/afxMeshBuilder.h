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

#include "afx/sim/afxSimDefs.h"
#include "afx/core/afxArray.h"
#include "afx/math/afxAabb.h"
#include "afx/core/afxInstance.h"
#include "afx/sim/afxMaterial.h"
#include "afx/sim/afxSkeleton.h"
#include "afx/core/afxUrd.h"
#include "afx/math/afxVertex.h"

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

AFX_DEFINE_STRUCT(afxMeshBuilder)
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

AFXINL afxError         AfxBeginMeshBuilding(afxMeshBuilder* mshb, afxString const* id, afxNat vtxCnt, afxNat triCnt, afxNat surfCnt, afxNat artCnt);
AFXINL void             AfxEndMeshBuilding(afxMeshBuilder* mshb);

AFXINL void             AfxUpdateVertices(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxNat const baseBiasIdx[], afxNat const biasCnt[]);
AFXINL void             AfxUpdateVertexPositions(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const posn[][3]);
AFXINL void             AfxUpdateVertexPositions4(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const posn[][4]);
AFXINL afxError         AfxUpdateVertexNormals(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const nrm[][3]);
AFXINL afxError         AfxUpdateVertexWraps(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const uv[][2]);

AFXINL void             AfxEmitTriangles(afxMeshBuilder* mshb, afxNat surfIdx, afxNat baseTriIdx, afxNat triCnt, afxNat const triVtxIdx[][3]);

AFXINL afxNat           AfxAddVertexBiases(afxMeshBuilder* mshb, afxNat cnt, afxNat const jntIdx[], afxReal const weight[]);

AFXINL void             AfxRenameVertexPivots(afxMeshBuilder* mshb, afxNat basePivotIdx, afxNat cnt, afxString const name[]);

AFX afxMesh             AfxBuildCubeMesh(afxSimulation sim, afxReal scale);
AFX afxMesh             AfxBuildParallelepipedMesh(afxSimulation sim, afxReal width, afxReal height);
AFX afxMesh             AfxBuildDomeMesh(afxSimulation sim, afxReal radius, afxNat slices);

#endif//AFX_MESH_BUILDER_H
