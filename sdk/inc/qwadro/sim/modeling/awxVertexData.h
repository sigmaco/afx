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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

/// Vertex data consists of vertex elements combined to form vertex components.
/// Vertex elements, the smallest unit of a vertex, represent entities such as position, normal, or color.
/// Vertex components are one or more vertex elements stored contiguously (interleaved per vertex) in a single memory buffer.

#ifndef AFX_VERTEX_DATA_H
#define AFX_VERTEX_DATA_H

#include "qwadro/sim/modeling/afxMeshBuilder.h"
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

AFX_DEFINE_STRUCT(afxVertexBias)
{
    afxNat              pivotIdx;
    afxReal             weight;
};

AFX_DEFINE_STRUCT(awxVertexDataCache)
{
    afxLinkage          vbuf;
    afxNat32            base;
    afxNat32            range;
    afxNat32            stride;
};

#ifdef _AFX_VERTEX_DATA_C

AFX_DEFINE_STRUCT(awxVertexDataAttr)
{
    awxVertexUsage      usage;
    awxVertexFlags      flags;
    afxVertexFormat     fmt;
    void*               data;
    afxFixedString8     id;
    afxNat              idStrIdx;

    afxNat              cacheIdx;
    afxNat32            cachedOffset;
    afxVertexFormat     cachedFmt;
};

AFX_OBJECT(awxVertexData)
{
    afxNat              biasCnt;
    afxVertexBias*      biases;
    afxNat              vtxCnt;
    afxVertex*          vtx; // one for each vertex
    afxNat              attrCnt;
    awxVertexDataAttr*  attrs;
    afxAabb             aabb;

    afxBool             cached;
    awxVertexDataCache  cache[2];

    afxUri              urd;
    afxNat              urdEntryIdx;

    afxStringCatalog    strc;
};

#endif

AFX_DEFINE_STRUCT(awxVertexDataSpec)
{
    afxNat              vtxCnt;
    afxVertex const*    vtxSrc;
    afxNat              vtxSrcStride;
    afxNat              biasCnt;
    afxVertexBias const*biasSrc;
    afxNat              biasSrcStride;
    afxNat              baseAttrIdx;
    afxNat              attrCnt;
};

AFX_DEFINE_STRUCT(awxVertexAttrSpec)
{
    afxChar const*      id;
    afxVertexFormat     fmt;
    awxVertexUsage      usage;
    awxVertexFlags      flags;
    void const*         src;
    afxNat              srcStride;
};

AFX afxNat              AwxCountVertices(awxVertexData vtd);

AFX afxNat              AwxFindVertexDataAttributes(awxVertexData vtd, afxNat cnt, afxString const id[], afxNat attrIdx[]);

AFX afxError            AwxGetVertexAttributeInfo(awxVertexData vtd, afxNat attrIdx, afxVertexFormat* fmt, awxVertexUsage* usage, awxVertexFlags* flags);

AFX afxVertexFormat     AwxGetVertexAttributeFormat(awxVertexData vtd, afxNat attrIdx);
AFX awxVertexUsage      AwxGetVertexAttributeUsage(awxVertexData vtd, afxNat attrIdx);
AFX awxVertexFlags      AwxGetVertexAttributeFlags(awxVertexData vtd, afxNat attrIdx);

AFX afxNat              AwxCountVertexBiases(awxVertexData vtd);
AFX afxVertexBias const*AwxGetVertexBiases(awxVertexData vtd, afxNat baseBiasIdx);

AFX void*               AwxExposeVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx);
AFX afxError            AwxZeroVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt);
AFX afxError            AwxNormalizeVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt);
AFX afxError            AwxFillVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src);
AFX afxError            AwxUpdateVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src, afxNat32 srcStride);

AFX afxError            AwxBufferizeVertexData(afxDrawInput din, awxVertexData vtd);
AFX afxError            AwxCmdBindVertexDataCache(afxDrawScript dscr, afxNat slotIdx, awxVertexData vtd);

AFX afxError            AwxUpdateVertices(awxVertexData vtd, afxNat baseVtx, afxNat vtxCnt, afxVertex const src[], afxNat srcStride);
AFX afxError            AwxUpdateVertexBiases(awxVertexData vtd, afxNat baseBiasIdx, afxNat biasCnt, afxVertexBias const src[], afxNat srcStride);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AwxAcquireVertexDatas(afxSimulation sim, afxStringCatalog strc, awxVertexAttrSpec const attrSpec[], afxNat cnt, awxVertexDataSpec const specs[], awxVertexData datas[]);

AFX awxVertexData       AwxBuildVertexData(afxSimulation sim, afxStringCatalog strc, afxMeshBuilder const* mshb);

AFX void                AwxTransformVertexDatas(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxBool renormalize, afxNat cnt, awxVertexData vtd[]);

#endif//AFX_VERTEX_DATA_H
