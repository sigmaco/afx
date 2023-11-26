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

#include "afx/sim/modeling/afxMeshBuilder.h"

typedef enum afxVertexUsage
{
    afxVertexUsage_POS          = AfxGetBitOffset(0),
    afxVertexUsage_JNT          = AfxGetBitOffset(1),
    afxVertexUsage_WGT          = AfxGetBitOffset(2),
    afxVertexUsage_BLENDING     = afxVertexUsage_JNT | afxVertexUsage_WGT,
    afxVertexUsage_POSITIONAL   = afxVertexUsage_POS | afxVertexUsage_BLENDING,

    afxVertexUsage_NRM          = AfxGetBitOffset(3),
    afxVertexUsage_TAN          = AfxGetBitOffset(4),
    afxVertexUsage_BIT          = AfxGetBitOffset(5),
    afxVertexUsage_TBC          = AfxGetBitOffset(10), // tangent-binormal cross
    afxVertexUsage_UVN          = AfxGetBitOffset(11),
    afxVertexUsage_TANGENT      = afxVertexUsage_NRM | afxVertexUsage_TAN | afxVertexUsage_BIT | afxVertexUsage_TBC | afxVertexUsage_UVN,
    afxVertexUsage_SPATIAL      = afxVertexUsage_POSITIONAL | afxVertexUsage_TANGENT,

    afxVertexUsage_UV           = AfxGetBitOffset(6),
    afxVertexUsage_VISUAL       = afxVertexUsage_UV,
} afxVertexUsage;

typedef enum afxVertexFlag
{
    afxVertexFlag_DYNAMIC = AfxGetBitOffset(0), // The data store contents will be modified repeatedly and used many times.
    afxVertexFlag_STREAM = AfxGetBitOffset(1), // The data store contents will be modified once and used at most a few times.
    
    afxVertexFlag_POSITIONAL = AfxGetBitOffset(2),
    afxVertexFlag_SPATIAL = AfxGetBitOffset(3),

    afxVertexFlag_NORMALIZED = AfxGetBitOffset(4),
    afxVertexFlag_RASTERIZATION = AfxGetBitOffset(5),

    afxVertexFlag_AFFINE = AfxGetBitOffset(10),
    /// affected by affine transformations (ex.: position). Non-delta spatial attributes should receive affine transformations.

    afxVertexFlag_LINEAR = AfxGetBitOffset(11),
    /// affected by linear transformations (ex.: tangent, binormal). Delta spatial attributes should receive linear transformations (ex.: normal, tangent/binormal cross).

    afxVertexFlag_LINEAR_INV = AfxGetBitOffset(12),
    /// affected by inverse linear transformations. Non-delta spatial attributes should receive inverse linear transformations (ex.: normal, tangent/binormal cross).

    afxVertexFlag_DELTA = AfxGetBitOffset(13), // treat as delta
} afxVertexFlags;

AFX_DEFINE_STRUCT(afxVertexBias)
{
    afxNat8 pivotIdx;
    afxReal weight;
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

#ifdef _AFX_VERTEX_DATA_C

AFX_DEFINE_STRUCT(afxVertexDataAttr)
{
    afxNat          cacheIdx;
    afxVertexUsage  usage;
    afxVertexFlags  flags;
    afxVertexFormat fmt;
    void*           data;
    afxString8      id;
};

AFX_OBJECT(afxVertexData)
{
    afxNat              vtxCnt;
    afxVertex*          vtx; // one for each vertex
    afxNat              biasCnt;
    afxVertexBias*      biases;
    afxNat              attrCnt;
    afxVertexDataAttr*  attrs;
    afxNat              cacheCnt;
    afxVertexDataCache* caches;
};
#endif

AFX_DEFINE_STRUCT(afxVertexAttrSpec)
{
    afxChar const*      id;
    afxVertexFormat     fmt;
    afxVertexUsage      usage;
    afxVertexFlags      flags;
    afxNat              cacheIdx;
};

AFX afxNat              AfxFindVertexDataAttributes(afxVertexData vtd, afxNat cnt, afxString const id[], afxNat attrIdx[]);

AFX afxError            AfxGetVertexAttributeInfo(afxVertexData vtd, afxNat attrIdx, afxVertexFormat* fmt, afxVertexUsage* usage, afxVertexFlags* flags, afxNat* cacheIdx);

AFX afxVertexFormat     AfxGetVertexAttributeFormat(afxVertexData vtd, afxNat attrIdx);
AFX afxVertexUsage      AfxGetVertexAttributeUsage(afxVertexData vtd, afxNat attrIdx);
AFX afxVertexFlags      AfxGetVertexAttributeFlags(afxVertexData vtd, afxNat attrIdx);
AFX afxNat              AfxGetVertexAttributeCache(afxVertexData vtd, afxNat attrIdx);

AFX afxError            AfxZeroVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt);
AFX afxError            AfxNormalizeVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt);
AFX afxError            AfxFillVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt, void const* src);
AFX afxError            AfxUpdateVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt, void const* src, afxNat32 srcStride);

AFX afxVertexDataCache* AfxGetVertexDataCache(afxVertexData vtd, afxNat cacheIdx);

AFX afxError            AfxBufferizeVertexData(afxVertexData vtd);
AFX afxError            AfxBindVertexData(afxVertexData vtd, afxDrawScript dscr);

AFX void*               AfxExposeVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxVertexData       AfxBuildVertexData(afxSimulation sim, afxMeshBuilder const* mshb);

AFX void                AfxTransformVertexDatas(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxBool renormalize, afxNat cnt, afxVertexData vtd[]);

#endif//AFX_VERTEX_DATA_H
