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

/// Vertex data consists of vertex elements combined to form vertex components.
/// Vertex elements, the smallest unit of a vertex, represent entities such as position, normal, or color.
/// Vertex components are one or more vertex elements stored contiguously (interleaved per vertex) in a single memory buffer.

#ifndef AFX_VERTEX_DATA_H
#define AFX_VERTEX_DATA_H

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
    afxReal weight;
    afxNat8 pivotIdx;
};

AFX_DEFINE_STRUCT(afxVertexDataStream)
{
    afxChain                junctions;
};

AFX_DEFINE_STRUCT(afxVertexDataComponent)
{
    afxNat                  attrIdx;
    afxNat32                offset; // buffered offset
    afxVertexFormat         fmt; // buffered format
};

AFX_DEFINE_STRUCT(afxVertexDataCache)
{
    afxLinkage              stream;

    afxBuffer               buf;
    afxNat32                base;
    afxNat32                range;
    afxNat32                stride;

    afxVertexData           vtd;
    afxVertexUsage          usage; // combined usage of each component
    afxNat                  compCnt;
    afxVertexDataComponent* comps;
};

AFX_OBJECT(afxVertexData)
#ifdef _AFX_VERTEX_DATA_C
{
    afxNat              vtxCnt;
    afxVertexBias*      biases; // one for each vertex
    afxNat              weightCnt;
    afxVertexWeight*    weights;
    struct
    {
        afxNat          cacheIdx;
        afxVertexUsage  usage;
        afxVertexFlags  flags;
        afxVertexFormat fmt;
        void*           data;
        afxString8      tag;
    }                  *attrs;
    afxNat              attrCnt;
    afxNat              cacheCnt;
    afxVertexDataCache* caches;
}
#endif
;

AFX_DEFINE_STRUCT(afxVertexAttrSpec)
{
    afxChar const*  tag;
    afxVertexFormat fmt;
    afxVertexUsage  usage;
    afxVertexFlags  flags;
    afxNat          cacheIdx;
};

AFX_DEFINE_STRUCT(afxMeshBuilder)
{
    void(*GetVertexInfo)(void* data, afxNat* vtxCnt, afxBool* biased, afxNat* weightCnt, afxNat* attrCnt);
    void(*GetVertexBiases)(void* data, afxNat baseVtxIdx, afxNat vtxCnt, afxVertexBias bias[]);
    void(*GetVertexWeights)(void* data, afxNat first, afxNat cnt, afxVertexWeight weight[]);
    void(*GetVertexSpecs)(void* data, afxNat baseAttrIdx, afxNat attrCnt, afxVertexAttrSpec spec[], afxBool hasData[]);
    void(*GetVertexData)(void* data, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt, void *dst);
    
    afxNat(*GetTopologyInfo)(void* data, afxNat* triCnt, afxNat* surfaceCnt);
    afxNat(*GetSurfaceData)(void* data, afxNat surfaceIdx, afxNat* mtlIdx, afxNat triangles[]); // output tri indices for a given surface
    void(*GetCoincidentMap)(void* data, afxNat firstVtx, afxNat vtxCnt, afxNat map[]); // output original vtx
    void(*GetTriangleMap)(void* data, afxNat firstVtx, afxNat vtxCnt, afxNat map[]); // output owner tri for each vertex
    void(*GetEdgeMap)(void* data, afxNat first, afxNat triCnt, afxNat const indices[], afxNat map[]); // output edge data for each tri
    
    void(*GetBindingInfo)(void* data, afxNat* mtlCnt, afxNat* artCnt);
    afxMaterial(*GetMaterial)(void* data, afxNat mtlIdx);    
    afxNat(*GetVertebraInfo)(void* data, afxNat artIdx, afxString* name);
    void(*GetVertebraData)(void* data, afxString const* name, afxNat baseTriIdx, afxNat triCnt, void *dst);

    void(*Cleanup)(void* data);
    void*data[4];
};

AFX afxNat              AfxFindVertexDataAttributes(afxVertexData vtd, afxNat cnt, afxString const names[], afxNat attrIdx[]);

AFX afxError            AfxGetVertexAttributeInfo(afxVertexData vtd, afxNat attrIdx, afxVertexFormat* fmt, afxVertexUsage* usage, afxVertexFlags* flags, afxNat* cacheIdx);

AFX afxVertexFormat     AfxGetVertexAttributeFormat(afxVertexData vtd, afxNat attrIdx);
AFX afxVertexUsage      AfxGetVertexAttributeUsage(afxVertexData vtd, afxNat attrIdx);
AFX afxVertexFlags      AfxGetVertexAttributeFlags(afxVertexData vtd, afxNat attrIdx);
AFX afxNat              AfxGetVertexAttributeCache(afxVertexData vtd, afxNat attrIdx);

AFX afxError            AfxUpdateVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt, void const* src, afxNat32 srcStride);

AFX afxVertexDataCache* AfxGetVertexDataCache(afxVertexData vtd, afxNat cacheIdx);

AFX afxError            AfxBufferizeVertexData(afxVertexData vtd);
AFX afxError            AfxBindVertexData(afxVertexData vtd, afxDrawScript dscr);

AFX void*               AfxExposeVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx);

AFX afxError            AfxBuildVertexDatas(afxSimulation sim, afxMeshBuilder const* mshb, afxNat cnt, void *data[], afxVertexData vtxd[]);

AFX void                AfxTransformVertexDatas(afxReal const affine[3], afxReal const linear[3][3], afxReal const invLinear[3][3], afxBool renormalize, afxNat cnt, afxVertexData vtd[]);

#endif//AFX_VERTEX_DATA_H