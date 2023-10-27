/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MESH_DATA_H
#define AFX_MESH_DATA_H

#include "afx/sim/afxSimDefs.h"
#include "afx/core/afxArray.h"
#include "afx/math/afxAabb.h"
#include "afx/core/afxInstance.h"
#include "afx/draw/afxIndexBuffer.h"
#include "afx/draw/afxVertexBuffer.h"
#include "afx/sim/afxMaterial.h"
#include "afx/sim/afxSkeleton.h"
#include "afx/core/afxUrd.h"
#include "afx/sim/modeling/afxVertex.h"

AFX_DEFINE_STRUCT(afxVertexBias)
{
    afxNat8 baseWeightIdx;
    afxNat8 weightCnt;
};

AFX_DEFINE_STRUCT(afxVertexWeight)
{
    afxNat8 pivotIdx;
    afxReal weight;
};

AFX_OBJECT(afxMeshData)
#ifdef _AFX_MESH_DATA_C
{
    afxNat          weightCnt;
    afxVertexWeight*weights;

    afxNat          vtxCnt;
    union
    {
        afxV3d*     xyz;
        afxV4d*     xyzw;
    };

    afxNat          biasCnt;
    afxVertexBias*  biases;

    union
    {
        afxV2d*     uv;
        afxV3d*     uvw;
    };
    union
    {
        struct
        {
            afxV3d* tan;
            afxV3d* bit;
            afxV3d* nrm;
        };
        afxM3d*     tbn;
        afxQuat     tbnq;
    };
    afxV3d*         colors;
}
#endif
;

AFX_DEFINE_STRUCT(afxMeshBuilder)
{
    void(*GetVertexInfo)(void* data, afxNat* vtxCnt, afxNat* weightCnt, afxNat* biasCnt);
    void(*GetWeights)(void* data, afxNat first, afxNat cnt, afxVertexWeight weight[]);
    void(*GetVertices)(void* data, afxNat first, afxNat cnt, afxV3d xyz[], afxV4d xyzw[]);
    void(*GetBiases)(void* data, afxNat first, afxNat cnt, afxVertexBias bias[]);
    void(*GetTangents)(void* data, afxNat first, afxNat cnt, afxV3d tan[], afxV3d bit[], afxV3d nrm[], afxM3d tbn[], afxQuat tbnq[]);
    void(*GetWrappings)(void* data, afxNat first, afxNat cnt, afxV2d uv[], afxV3d uvw[]);

    afxNat(*GetTopologyInfo)(void* data, afxNat* triCnt, afxNat* surfaceCnt);
    afxNat(*GetSurfaceData)(void* data, afxNat surfaceIdx, afxNat* mtlIdx, afxNat triangles[]); // output tri indices for a given surface
    void(*GetCoincidentMap)(void* data, afxNat firstVtx, afxNat vtxCnt, afxNat map[]); // output original vtx
    void(*GetTriangleMap)(void* data, afxNat firstVtx, afxNat vtxCnt, afxNat map[]); // output owner tri for each vertex
    void(*GetEdgeMap)(void* data, afxNat first, afxNat triCnt, afxNat const indices[], afxNat map[]); // output edge data for each tri
    
    void(*GetBindingInfo)(void* data, afxNat* mtlCnt, afxNat* artCnt);
    afxMaterial(*GetMaterial)(void* data, afxNat mtlIdx);    
    afxNat(*GetVertebra)(void* data, afxNat artIdx, afxString* name);

    void(*Cleanup)(void* data);
    void*data[4];
};

AFX afxError        AfxBuildMeshDatas(afxSimulation sim, afxMeshBuilder const* mshb, afxNat cnt, void *data[], afxMeshData mshd[]);

#endif//AFX_MESH_DATA_H