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
    awxVertexUsage_POS          = AFX_BIT_OFFSET(0),
    awxVertexUsage_JNT          = AFX_BIT_OFFSET(1),
    awxVertexUsage_WGT          = AFX_BIT_OFFSET(2),
    awxVertexUsage_BLENDING     = awxVertexUsage_JNT | awxVertexUsage_WGT,
    awxVertexUsage_POSITIONAL   = awxVertexUsage_POS | awxVertexUsage_BLENDING,

    awxVertexUsage_NRM          = AFX_BIT_OFFSET(3),
    awxVertexUsage_TAN          = AFX_BIT_OFFSET(4),
    awxVertexUsage_BIT          = AFX_BIT_OFFSET(5),
    awxVertexUsage_TBC          = AFX_BIT_OFFSET(10), // tangent-binormal cross
    awxVertexUsage_UVN          = AFX_BIT_OFFSET(11),
    awxVertexUsage_TANGENT      = awxVertexUsage_NRM | awxVertexUsage_TAN | awxVertexUsage_BIT | awxVertexUsage_TBC | awxVertexUsage_UVN,
    awxVertexUsage_SPATIAL      = awxVertexUsage_POSITIONAL | awxVertexUsage_TANGENT,

    awxVertexUsage_UV           = AFX_BIT_OFFSET(6),
    awxVertexUsage_VISUAL       = awxVertexUsage_UV,
} awxVertexUsage;

typedef enum awxVertexFlag
{
    awxVertexFlag_DYNAMIC = AFX_BIT_OFFSET(0), // The data store contents will be modified repeatedly and used many times.
    awxVertexFlag_STREAM = AFX_BIT_OFFSET(1), // The data store contents will be modified once and used at most a few times.
    
    awxVertexFlag_POSITIONAL = AFX_BIT_OFFSET(2),
    awxVertexFlag_SPATIAL = AFX_BIT_OFFSET(3),

    awxVertexFlag_NORMALIZED = AFX_BIT_OFFSET(4),
    awxVertexFlag_RASTERIZATION = AFX_BIT_OFFSET(5),

    awxVertexFlag_AFFINE = AFX_BIT_OFFSET(10),
    /// affected by affine transformations (ex.: position). Non-delta spatial attributes should receive affine transformations.

    awxVertexFlag_LINEAR = AFX_BIT_OFFSET(11),
    /// affected by linear transformations (ex.: tangent, binormal). Delta spatial attributes should receive linear transformations (ex.: normal, tangent/binormal cross).

    awxVertexFlag_LINEAR_INV = AFX_BIT_OFFSET(12),
    /// affected by inverse linear transformations. Non-delta spatial attributes should receive inverse linear transformations (ex.: normal, tangent/binormal cross).

    awxVertexFlag_DELTA = AFX_BIT_OFFSET(13), // treat as delta

    awxVertexFlag_RESIDENT = AFX_BIT_OFFSET(14) // allocation for data is resident
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
    union
    {
        void*           data;
        afxByte*        dataBytemap;
    };
    afxString           id; // 8

    afxNat              cacheIdx;
    afxNat32            cachedOffset;
    afxVertexFormat     cachedFmt;
};

AFX_OBJECT(awxVertexData)
{
    //afxNat              biasCnt;
    //afxVertexBias*      biases;
    afxNat              vtxCnt;
    //afxVertex*          vtx; // one for each vertex
    afxNat              attrCnt;
    awxVertexDataAttr*  attrs;
    afxAabb             aabb;

    afxBool             cached;
    awxVertexDataCache  cache[2];

    afxUri              urd;
    afxNat              urdEntryIdx;

    afxStringBase    strb;
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

AKX afxNat              AwxCountVertices(awxVertexData vtd);

AKX afxNat              AwxFindVertexAttributes(awxVertexData vtd, afxNat cnt, afxString const id[], afxNat attrIdx[]);

AKX afxError            AwxGetVertexAttributeInfo(awxVertexData vtd, afxNat attrIdx, afxVertexFormat* fmt, awxVertexUsage* usage, awxVertexFlags* flags);

AKX afxVertexFormat     AwxGetVertexAttributeFormat(awxVertexData vtd, afxNat attrIdx);
AKX awxVertexUsage      AwxGetVertexAttributeUsage(awxVertexData vtd, afxNat attrIdx);
AKX awxVertexFlags      AwxGetVertexAttributeFlags(awxVertexData vtd, afxNat attrIdx);

AKX void*               AwxExposeVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx);
AKX afxError            AwxZeroVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt);
AKX afxError            AwxFillVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src);
AKX afxError            AwxUpdateVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src, afxNat32 srcStride);
AKX afxError            AwxNormalizeVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt);

AKX afxError            AwxBufferizeVertexData(afxDrawInput din, awxVertexData vtd);
AKX afxError            AwxCmdBindVertexDataCache(afxDrawStream diob, afxNat slotIdx, awxVertexData vtd);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxError            AwxAcquireVertexDatas(afxSimulation sim, afxStringBase strb, awxVertexAttrSpec const attrSpec[], afxNat cnt, awxVertexDataSpec const specs[], awxVertexData datas[]);

AKX awxVertexData       AwxBuildVertexData(afxSimulation sim, afxStringBase strb, afxMeshBuilder const* mshb);

AKX void                AwxTransformVertexDatas(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxBool renormalize, afxNat cnt, awxVertexData vtd[]);

#endif//AFX_VERTEX_DATA_H
