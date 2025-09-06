/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO PIPELINED VERTEX INPUT DECLARATION                                //
//////////////////////////////////////////////////////////////////////////////

// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_VERTEX_DECL_H
#define AVX_VERTEX_DECL_H

/**
    When the vertex stage is present in a pipeline, the vertex shader input variables form an interface with the vertex input attributes.
    All vertex shader inputs declared must have a corresponding attribute and binding in the pipeline.
*/

#include "qwadro/draw/avxFormat.h"

#define AVX_MAX_VERTEX_SOURCES (32)
#define AVX_MAX_VERTEX_ATTRIBS (32)

AFX_DEFINE_STRUCT(avxVertexSource)
{
    avxBuffer       buf;
    afxUnit32       offset; // the start of buffer.
    afxUnit32       range; // the size in bytes of vertex data bound from buffer.
    afxUnit32       stride; // the byte stride between consecutive elements within the buffer.
  //afxUnit32       instRate; // the number of successive instances that will use the same value of the vertex attribute when instanced rendering is enabled.
};

AFX_DEFINE_STRUCT(avxVertexStream)
{
    // Binding index (matches the vertex buffer bound) which this stream takes its data from.
    afxUnit         pin;
    // Minimum stride (in bytes) between consecutive vertices.
    afxUnit32       minStride;
    // Instance rate divisor (0 = per-vertex, >0 = per-instance).
    afxUnit         instRate;
    // Index into 'attrs' where attributes for this fetch begin.
    afxUnit         baseAttrIdx;
    // Number of attributes sourced from this fetch.
    afxUnit         attrCnt;
    // Misc. flags (e.g., dynamic/static buffer, etc.).
    afxFlags        flags;
};

#define AVX_VERTEX_STREAM(uPin, uMinStride, uInstRate, uBaseAttrIdx, uAttrCnt) (avxVertexStream){ \
    .pin = (uPin), \
    .minStride = (uMinStride), \
    .instRate = (uInstRate), \
    .baseAttrIdx = (uBaseAttrIdx), \
    .attrCnt = (uAttrCnt) }

typedef enum avxVertexAttrFlag
{
    avxVertexAttrFlag_POS       = AFX_BITMASK(0),
    avxVertexAttrFlag_NORMAL    = AFX_BITMASK(1),
    avxVertexAttrFlag_WRAP      = AFX_BITMASK(2),
    avxVertexAttrFlag_COLOR     = AFX_BITMASK(3),
    avxVertexAttrFlag_MATRIX    = AFX_BITMASK(4),
} avxVertexAttrFlags;

AFX_DEFINE_STRUCT(avxVertexAttr)
// Describes a single vertex attribute (like position, normal, texcoord, etc.).
{
    // Location in shader (e.g., layout(location = x) in GLSL).
    afxUnit         location;
    // A byte offset of this attribute relative to the start of an element in the bound vertex buffer.
    afxUnit32       offset;
    // Custom flags for optimization/metadata.
    afxFlags        flags;
    // Format of the data (e.g., vec3 float, vec2 half, etc.).
    avxFormat       fmt;
};

#define AVX_VERTEX_ATTR(uLocation, uOffset, eFmt) (avxVertexAttr){ \
    .location = (uLocation), \
    .offset = (uOffset), \
    .fmt = (eFmt) }

AFX_DEFINE_STRUCT(avxVertexLayout)
// Represents a complete description of how vertex data is fed into the graphics pipeline.
{
    // Custom flags for engine-specific usage or optimizations.
    afxFlags        flags;
    // Number of vertex fetch (binding) sources.
    afxUnit         binCnt;
    // An array of fetch sources (bindings).
    avxVertexStream bins[AVX_MAX_VERTEX_SOURCES];
    // An array of attributes (layout descriptions).
    avxVertexAttr   attrs[AVX_MAX_VERTEX_ATTRIBS];
    // Optional string for identifying or tagging the object.
    afxString       tag;
    // User-defined data (opaque pointer for engine/client use).
    void*           udd;
};

AVX afxError    AvxDeclareVertexInputs
(
    afxDrawSystem dsys, 
    afxUnit cnt, 
    avxVertexLayout const layouts[], 
    avxVertexInput declarations[]
);

////////////////////////////////////////////////////////////////////////////////

AVX afxDrawSystem   AvxGetVertexInputHost
(
    avxVertexInput  vin
);

AVX afxFlags    AvxGetVertexInputFlags
(
    avxVertexInput vin, 
    afxFlags mask
);

AVX afxUnit     AvxDescribeVertexLayout
(
    avxVertexInput vin, 
    avxVertexLayout* layout
);

AVX afxUnit     AvxQueryVertexStride
(
    avxVertexInput vin, 
    afxUnit baseSrcIdx, 
    afxUnit cnt, 
    afxUnit vtxSrcSiz[]
);

AFX_DEFINE_STRUCT(avxVertexData)
// Structure holding the processed vertex input data per vertex.
{
    union
    {
        // floating-point view (e.g., for vec2, vec3, vec4)
        afxReal fv[4];
        // integer view
        afxInt  iv[4];
        // unsigned integer view (e.g., for indices, IDs)
        afxUnit uv[4];
    } va[AVX_MAX_VERTEX_ATTRIBS];
};

#endif//AVX_VERTEX_DECL_H
