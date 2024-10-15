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

/*
    MESH GEOMETRY

    Mesh geometry refers to the spatial arrangement and positions of the vertices in a mesh. It deals with the actual coordinates of the vertices in 3D space, which define the shape of the object.

    Key aspects of geometry:
        Vertex Positions: The 3D coordinates (x, y, z) of each vertex in the mesh.
        Face Normals: The direction perpendicular to the surface of a face, used for lighting and rendering.
        Vertex Normals: The average direction perpendicular to the surface at a vertex, used for smooth shading.
        Texture Coordinates: Mapping of vertices to 2D texture space, which affects how textures are applied.

    Examples:
        Vertex Array: A list of vertices with their 3D coordinates.
        Normal Vectors: Vectors perpendicular to each face or vertex, used for rendering lighting effects.
        UV Mapping: Coordinates that map 2D textures to the 3D surface of the mesh.

    Geometry is concerned with the shape of the mesh. It provides the exact positions of vertices in space, the orientation of faces, and how the surface is textured.
*/

#ifndef AVX_GEOMETRY_H
#define AVX_GEOMETRY_H

#include "qwadro/inc/cad/afxMeshBuilder.h"
#include "qwadro/inc/draw/io/afxVertexStream.h"

/// Vertex data consists of vertex elements combined to form vertex components.
/// Vertex elements, the smallest unit of a vertex, represent entities such as position, normal, or color.
/// Vertex components are one or more vertex elements stored contiguously (interleaved per vertex) in a single memory buffer.

#if 0
typedef enum afxVertexUsage
{
    afxVertexUsage_POS          = AFX_BIT(0),
    afxVertexUsage_JNT          = AFX_BIT(1),
    afxVertexUsage_WGT          = AFX_BIT(2),
    afxVertexUsage_BLENDING     = afxVertexUsage_JNT | afxVertexUsage_WGT,
    afxVertexUsage_POSITIONAL   = afxVertexUsage_POS | afxVertexUsage_BLENDING,

    afxVertexUsage_NRM          = AFX_BIT(3),
    afxVertexUsage_TAN          = AFX_BIT(4),
    afxVertexUsage_BIT          = AFX_BIT(5),
    afxVertexUsage_TBC          = AFX_BIT(10), // tangent-binormal cross
    afxVertexUsage_UVN          = AFX_BIT(11),
    afxVertexUsage_TANGENT      = afxVertexUsage_NRM | afxVertexUsage_TAN | afxVertexUsage_BIT | afxVertexUsage_TBC | afxVertexUsage_UVN,
    afxVertexUsage_SPATIAL      = afxVertexUsage_POSITIONAL | afxVertexUsage_TANGENT,

    afxVertexUsage_UV           = AFX_BIT(6),
    afxVertexUsage_PRELIT       = AFX_BIT(7),
    afxVertexUsage_VISUAL       = afxVertexUsage_UV | afxVertexUsage_PRELIT,
} afxVertexUsage;
#endif

typedef enum afxVertexFlag
{
    afxVertexFlag_DYNAMIC = AFX_BIT(0), // The data store contents will be modified repeatedly and used many times.
    afxVertexFlag_STREAM = AFX_BIT(1), // The data store contents will be modified once and used at most a few times.
    
    afxVertexFlag_POSITIONAL = AFX_BIT(2),
    afxVertexFlag_SPATIAL = AFX_BIT(3),

    afxVertexFlag_NORMALIZED = AFX_BIT(4),
    afxVertexFlag_RASTERIZATION = AFX_BIT(5),

    afxVertexFlag_AFFINE = AFX_BIT(10),
    /// affected by affine transformations (ex.: position). Non-delta spatial attributes should receive affine transformations.

    afxVertexFlag_LINEAR = AFX_BIT(11),
    /// affected by linear transformations (ex.: tangent, binormal). Delta spatial attributes should receive linear transformations (ex.: normal, tangent/binormal cross).

    afxVertexFlag_LINEAR_INV = AFX_BIT(12),
    /// affected by inverse linear transformations. Non-delta spatial attributes should receive inverse linear transformations (ex.: normal, tangent/binormal cross).

    afxVertexFlag_DELTA = AFX_BIT(13), // treat as delta

    afxVertexFlag_RESIDENT = AFX_BIT(14) // allocation for data is resident
} afxVertexFlags;

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

AFX_DEFINE_STRUCT(afxVertexBias)
{
    afxNat              pivotIdx;
    afxReal             weight;
};

AFX_DEFINE_STRUCT(afxGeometrySpec)
{
    afxNat              vtxCnt;
    akxVertex const*    vtxSrc;
    afxNat              vtxSrcStride;
    afxNat              biasCnt;
    afxVertexBias const*biasSrc;
    afxNat              biasSrcStride;
    afxNat              baseAttrIdx;
    afxNat              attrCnt;
};

AFX_DEFINE_STRUCT(akxVertexAttrSpec)
{
    afxVertexFormat     fmt;
    afxVertexFlags      flags;
    afxChar const*      usage;
};

AVX afxNat              AfxCountVertices(afxGeometry geo);
AVX afxNat              AkxCountVertexAttributes(afxGeometry geo);

AVX afxNat              AkxFindVertexAttributes(afxGeometry geo, afxNat cnt, afxString const id[], afxNat attrIdx[]);

AVX afxError            AkxGetVertexAttributeInfo(afxGeometry geo, afxNat attrIdx, afxVertexFormat* fmt, afxString* usage, afxVertexFlags* flags);

AVX afxVertexFormat     AkxGetVertexAttributeFormat(afxGeometry geo, afxNat attrIdx);
AVX afxString           AkxGetVertexAttributeUsage(afxGeometry geo, afxNat attrIdx);
AVX afxVertexFlags      AkxGetVertexAttributeFlags(afxGeometry geo, afxNat attrIdx);

AVX void*               AfxExposeGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx);
AVX afxError            AfxZeroGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt);
AVX afxError            AfxFillGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src);
AVX afxError            AfxUpdateGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src, afxNat32 srcStride);
AVX afxError            AfxNormalizeGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt);
AVX afxError            AfxInvertNormalizedGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt);

AVX afxError            AfxUploadGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, afxStream in, afxNat stride);
AVX afxError            AfxDownloadGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, afxStream out, afxNat stride);


AFX_DEFINE_STRUCT(afxGeometryRegion)
{
    afxNat attrIdx;
    afxNat baseVtx;
    afxNat vtxCnt;
};

AFX_DEFINE_STRUCT(afxGeometryIo)
{
    afxNat attrIdx;
    afxNat baseVtx;
    afxNat vtxCnt;
    afxNat rowStride;
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AVX afxError            AfxAcquireGeometries(afxDrawInput din, akxVertexAttrSpec const attrSpec[], afxNat cnt, afxGeometrySpec const specs[], afxGeometry datas[]);

AVX afxGeometry         AfxBuildGeometry(afxDrawInput din, afxMeshBuilder const* mshb);

AVX void                AfxTransformGeometries(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxBool renormalize, afxNat cnt, afxGeometry geo[]);

#endif//AVX_GEOMETRY_H
