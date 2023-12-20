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

#include "qwadro/sim/modeling/awxMeshBuilder.h"
#include "qwadro/draw/io/afxVertexStream.h"

typedef enum awxVertexUsage
{
    awxVertexUsage_POS          = AfxGetBitOffset(0),
    awxVertexUsage_JNT          = AfxGetBitOffset(1),
    awxVertexUsage_WGT          = AfxGetBitOffset(2),
    awxVertexUsage_BLENDING     = awxVertexUsage_JNT | awxVertexUsage_WGT,
    awxVertexUsage_POSITIONAL   = awxVertexUsage_POS | awxVertexUsage_BLENDING,

    awxVertexUsage_NRM          = AfxGetBitOffset(3),
    awxVertexUsage_TAN          = AfxGetBitOffset(4),
    awxVertexUsage_BIT          = AfxGetBitOffset(5),
    awxVertexUsage_TBC          = AfxGetBitOffset(10), // tangent-binormal cross
    awxVertexUsage_UVN          = AfxGetBitOffset(11),
    awxVertexUsage_TANGENT      = awxVertexUsage_NRM | awxVertexUsage_TAN | awxVertexUsage_BIT | awxVertexUsage_TBC | awxVertexUsage_UVN,
    awxVertexUsage_SPATIAL      = awxVertexUsage_POSITIONAL | awxVertexUsage_TANGENT,

    awxVertexUsage_UV           = AfxGetBitOffset(6),
    awxVertexUsage_VISUAL       = awxVertexUsage_UV,
} awxVertexUsage;

typedef enum awxVertexFlag
{
    awxVertexFlag_DYNAMIC = AfxGetBitOffset(0), // The data store contents will be modified repeatedly and used many times.
    awxVertexFlag_STREAM = AfxGetBitOffset(1), // The data store contents will be modified once and used at most a few times.
    
    awxVertexFlag_POSITIONAL = AfxGetBitOffset(2),
    awxVertexFlag_SPATIAL = AfxGetBitOffset(3),

    awxVertexFlag_NORMALIZED = AfxGetBitOffset(4),
    awxVertexFlag_RASTERIZATION = AfxGetBitOffset(5),

    awxVertexFlag_AFFINE = AfxGetBitOffset(10),
    /// affected by affine transformations (ex.: position). Non-delta spatial attributes should receive affine transformations.

    awxVertexFlag_LINEAR = AfxGetBitOffset(11),
    /// affected by linear transformations (ex.: tangent, binormal). Delta spatial attributes should receive linear transformations (ex.: normal, tangent/binormal cross).

    awxVertexFlag_LINEAR_INV = AfxGetBitOffset(12),
    /// affected by inverse linear transformations. Non-delta spatial attributes should receive inverse linear transformations (ex.: normal, tangent/binormal cross).

    awxVertexFlag_DELTA = AfxGetBitOffset(13), // treat as delta
} awxVertexFlags;

AFX_DEFINE_STRUCT(awxVertexBias)
{
    afxNat pivotIdx;
    afxReal weight;
};

AFX_DEFINE_STRUCT(awxVertexDataComponent)
{
    afxNat                  attrIdx;
    afxNat32                offset; // buffered offset
    afxVertexFormat         fmt; // buffered format
};

AFX_DEFINE_STRUCT(awxVertexDataCache)
{
    afxLinkage              stream;

    afxBuffer               buf;
    afxNat32                base;
    afxNat32                range;
    afxNat32                stride;

    awxVertexData           vtd;
    awxVertexUsage          usage; // combined usage of each component
    afxNat                  compCnt;
    awxVertexDataComponent* comps;
};

#ifdef _AFX_VERTEX_DATA_C

AFX_DEFINE_STRUCT(awxVertexDataAttr)
{
    afxNat          cacheIdx;
    awxVertexUsage  usage;
    awxVertexFlags  flags;
    afxVertexFormat fmt;
    void*           data;
    afxFixedString8      id;
};

AFX_OBJECT(awxVertexData)
{
    afxNat              vtxCnt;
    afxVertex*          vtx; // one for each vertex
    afxNat              biasCnt;
    awxVertexBias*      biases;
    afxNat              attrCnt;
    awxVertexDataAttr*  attrs;
    afxNat              cacheCnt;
    awxVertexDataCache* caches;
    afxVertexInput      layout;
    afxNat                  streamCnt;
    afxVertexBufferSegment* streams;
};

#endif

AFX_DEFINE_STRUCT(awxVertexAttrSpec)
{
    afxChar const*      id;
    afxVertexFormat     fmt;
    awxVertexUsage      usage;
    awxVertexFlags      flags;
    afxNat              cacheIdx;
};

AFX afxNat              AwxFindVertexDataAttributes(awxVertexData vtd, afxNat cnt, afxString const id[], afxNat attrIdx[]);

AFX afxError            AwxGetVertexAttributeInfo(awxVertexData vtd, afxNat attrIdx, afxVertexFormat* fmt, awxVertexUsage* usage, awxVertexFlags* flags, afxNat* cacheIdx);

AFX afxVertexFormat     AwxGetVertexAttributeFormat(awxVertexData vtd, afxNat attrIdx);
AFX awxVertexUsage      AwxGetVertexAttributeUsage(awxVertexData vtd, afxNat attrIdx);
AFX awxVertexFlags      AwxGetVertexAttributeFlags(awxVertexData vtd, afxNat attrIdx);
AFX afxNat              AwxGetVertexAttributeCache(awxVertexData vtd, afxNat attrIdx);

AFX afxError            AwxZeroVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt);
AFX afxError            AwxNormalizeVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt);
AFX afxError            AwxFillVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt, void const* src);
AFX afxError            AwxUpdateVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt, void const* src, afxNat32 srcStride);

AFX awxVertexDataCache* AwxGetVertexDataCache(awxVertexData vtd, afxNat cacheIdx);

AFX afxError            AwxBufferizeVertexData(awxVertexData vtd);
AFX afxError            AwxBindVertexData(awxVertexData vtd, afxDrawScript dscr);

AFX void*               AwxExposeVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX awxVertexData       AwxBuildVertexData(awxSimulation sim, awxMeshBuilder const* mshb);

AFX void                AwxTransformVertexDatas(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxBool renormalize, afxNat cnt, awxVertexData vtd[]);

#endif//AFX_VERTEX_DATA_H
