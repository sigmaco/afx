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

/// Vertex data consists of vertex elements combined to form vertex components.
/// Vertex elements, the smallest unit of a vertex, represent entities such as position, normal, or color.
/// Vertex components are one or more vertex elements stored contiguously (interleaved per vertex) in a single memory buffer.

#ifndef AKX_VERTEX_DATA_H
#define AKX_VERTEX_DATA_H

#include "qwadro/cad/afxMeshBuilder.h"
#include "qwadro/draw/io/afxVertexStream.h"

typedef enum akxVertexUsage
{
    akxVertexUsage_POS          = AFX_BIT(0),
    akxVertexUsage_JNT          = AFX_BIT(1),
    akxVertexUsage_WGT          = AFX_BIT(2),
    akxVertexUsage_BLENDING     = akxVertexUsage_JNT | akxVertexUsage_WGT,
    akxVertexUsage_POSITIONAL   = akxVertexUsage_POS | akxVertexUsage_BLENDING,

    akxVertexUsage_NRM          = AFX_BIT(3),
    akxVertexUsage_TAN          = AFX_BIT(4),
    akxVertexUsage_BIT          = AFX_BIT(5),
    akxVertexUsage_TBC          = AFX_BIT(10), // tangent-binormal cross
    akxVertexUsage_UVN          = AFX_BIT(11),
    akxVertexUsage_TANGENT      = akxVertexUsage_NRM | akxVertexUsage_TAN | akxVertexUsage_BIT | akxVertexUsage_TBC | akxVertexUsage_UVN,
    akxVertexUsage_SPATIAL      = akxVertexUsage_POSITIONAL | akxVertexUsage_TANGENT,

    akxVertexUsage_UV           = AFX_BIT(6),
    akxVertexUsage_VISUAL       = akxVertexUsage_UV,
} akxVertexUsage;

typedef enum akxVertexFlag
{
    akxVertexFlag_DYNAMIC = AFX_BIT(0), // The data store contents will be modified repeatedly and used many times.
    akxVertexFlag_STREAM = AFX_BIT(1), // The data store contents will be modified once and used at most a few times.
    
    akxVertexFlag_POSITIONAL = AFX_BIT(2),
    akxVertexFlag_SPATIAL = AFX_BIT(3),

    akxVertexFlag_NORMALIZED = AFX_BIT(4),
    akxVertexFlag_RASTERIZATION = AFX_BIT(5),

    akxVertexFlag_AFFINE = AFX_BIT(10),
    /// affected by affine transformations (ex.: position). Non-delta spatial attributes should receive affine transformations.

    akxVertexFlag_LINEAR = AFX_BIT(11),
    /// affected by linear transformations (ex.: tangent, binormal). Delta spatial attributes should receive linear transformations (ex.: normal, tangent/binormal cross).

    akxVertexFlag_LINEAR_INV = AFX_BIT(12),
    /// affected by inverse linear transformations. Non-delta spatial attributes should receive inverse linear transformations (ex.: normal, tangent/binormal cross).

    akxVertexFlag_DELTA = AFX_BIT(13), // treat as delta

    akxVertexFlag_RESIDENT = AFX_BIT(14) // allocation for data is resident
} akxVertexFlags;

AFX_DEFINE_STRUCT(akxVertexCache)
{
    afxLinkage          vbuf;
    avxVertexInput      vin;
    afxBuffer           buf;
    struct
    {
        afxNat32        base;
        afxNat32        range;
        afxNat32        stride;
    }                   streams[2];
};

AFX_DEFINE_STRUCT(akxVertexBias)
{
    afxNat              pivotIdx;
    afxReal             weight;
};

AFX_DEFINE_STRUCT(akxVertexDataSpec)
{
    afxNat              vtxCnt;
    akxVertex const*    vtxSrc;
    afxNat              vtxSrcStride;
    afxNat              biasCnt;
    akxVertexBias const*biasSrc;
    afxNat              biasSrcStride;
    afxNat              baseAttrIdx;
    afxNat              attrCnt;
};

AFX_DEFINE_STRUCT(akxVertexAttrSpec)
{
    afxChar const*      id;
    afxVertexFormat     fmt;
    akxVertexUsage      usage;
    akxVertexFlags      flags;
    void const*         src;
    afxNat              srcStride;
};

AKX afxNat              AkxCountVertices(akxVertexData vtd);
AKX afxNat              AkxCountVertexAttributes(akxVertexData vtd);

AKX afxNat              AkxFindVertexAttributes(akxVertexData vtd, afxNat cnt, afxString const id[], afxNat attrIdx[]);

AKX afxError            AkxGetVertexAttributeInfo(akxVertexData vtd, afxNat attrIdx, afxVertexFormat* fmt, akxVertexUsage* usage, akxVertexFlags* flags);

AKX afxVertexFormat     AkxGetVertexAttributeFormat(akxVertexData vtd, afxNat attrIdx);
AKX akxVertexUsage      AkxGetVertexAttributeUsage(akxVertexData vtd, afxNat attrIdx);
AKX akxVertexFlags      AkxGetVertexAttributeFlags(akxVertexData vtd, afxNat attrIdx);

AKX void*               AkxExposeVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx);
AKX afxError            AkxZeroVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt);
AKX afxError            AkxFillVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src);
AKX afxError            AkxUpdateVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src, afxNat32 srcStride);
AKX afxError            AkxNormalizeVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt);
AKX afxError            AkxInvertNormalizedVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxError            AkxAcquireVertexDatas(afxSimulation sim, akxVertexAttrSpec const attrSpec[], afxNat cnt, akxVertexDataSpec const specs[], akxVertexData datas[]);

AKX akxVertexData       AkxBuildVertexData(afxSimulation sim, afxMeshBuilder const* mshb);

AKX void                AkxTransformVertexDatas(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxBool renormalize, afxNat cnt, akxVertexData vtd[]);

#endif//AKX_VERTEX_DATA_H
