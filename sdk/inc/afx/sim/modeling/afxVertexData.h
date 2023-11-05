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

/// Vertex data consists of vertex elements combined to form vertex components.
/// Vertex elements, the smallest unit of a vertex, represent entities such as position, normal, or color.
/// Vertex components are one or more vertex elements stored contiguously (interleaved per vertex) in a single memory buffer.

#ifndef AFX_VERTEX_DATA_H
#define AFX_VERTEX_DATA_H

#include "afx/sim/afxSimDefs.h"
#include "afx/core/afxArray.h"
#include "afx/math/afxAabb.h"
#include "afx/core/afxInstance.h"
#include "afx/sim/afxMaterial.h"
#include "afx/sim/afxSkeleton.h"
#include "afx/core/afxUrd.h"
#include "afx/math/afxVertex.h"

typedef enum afxVertexUsage
{
    afxVertexUsage_POS          = AFX_BIT_OFFSET(0),
    afxVertexUsage_JNT          = AFX_BIT_OFFSET(1),
    afxVertexUsage_WGT          = AFX_BIT_OFFSET(2),
    afxVertexUsage_BLENDING     = afxVertexUsage_JNT | afxVertexUsage_WGT,
    afxVertexUsage_POSITIONAL   = afxVertexUsage_POS | afxVertexUsage_BLENDING,

    afxVertexUsage_NRM          = AFX_BIT_OFFSET(3),
    afxVertexUsage_TAN          = AFX_BIT_OFFSET(4),
    afxVertexUsage_BTN          = AFX_BIT_OFFSET(5),
    afxVertexUsage_LIGHTING     = afxVertexUsage_NRM | afxVertexUsage_TAN | afxVertexUsage_BTN,
    afxVertexUsage_SPATIAL      = afxVertexUsage_POSITIONAL | afxVertexUsage_LIGHTING,

    afxVertexUsage_UV           = AFX_BIT_OFFSET(6),
    afxVertexUsage_VISUAL       = afxVertexUsage_UV,
} afxVertexUsage;

typedef enum afxVertexFlag
{
    afxVertexFlag_DYNAMIC = AFX_BIT_OFFSET(0), // The data store contents will be modified repeatedly and used many times.
    afxVertexFlag_STREAM = AFX_BIT_OFFSET(1), // The data store contents will be modified once and used at most a few times.
    
    afxVertexFlag_POSITIONAL = AFX_BIT_OFFSET(2),
    afxVertexFlag_SPATIAL = AFX_BIT_OFFSET(3),

    afxVertexFlag_NORMALIZED = AFX_BIT_OFFSET(4),
    afxVertexFlag_RASTERIZATION = AFX_BIT_OFFSET(5),

    afxVertexFlag_AFFINE = AFX_BIT_OFFSET(10),
    /// affected by affine transformations (ex.: position). Non-delta spatial attributes should receive affine transformations.

    afxVertexFlag_LINEAR = AFX_BIT_OFFSET(11),
    /// affected by linear transformations (ex.: tangent, binormal). Delta spatial attributes should receive linear transformations (ex.: normal, tangent/binormal cross).

    afxVertexFlag_LINEAR_INV = AFX_BIT_OFFSET(12),
    /// affected by inverse linear transformations. Non-delta spatial attributes should receive inverse linear transformations (ex.: normal, tangent/binormal cross).

    afxVertexFlag_DELTA = AFX_BIT_OFFSET(13), // treat as delta
} afxVertexFlags;

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
        afxString8      id;
    }                  *attrs;
    afxNat              attrCnt;
    afxNat              cacheCnt;
    afxVertexDataCache* caches;
}
#endif
;

AFX_DEFINE_STRUCT(afxVertexAttrSpec)
{
    afxChar const*  id;
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

AFX afxNat              AfxFindVertexDataAttributes(afxVertexData vtd, afxNat cnt, afxString const id[], afxNat attrIdx[]);

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

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AfxBuildVertexDatas(afxSimulation sim, afxMeshBuilder const* mshb, afxNat cnt, void *data[], afxVertexData vtxd[]);

AFX void                AfxTransformVertexDatas(afxReal const affine[3], afxReal const linear[3][3], afxReal const invLinear[3][3], afxBool renormalize, afxNat cnt, afxVertexData vtd[]);

#endif//AFX_VERTEX_DATA_H