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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/**
    When the vertex stage is present in a pipeline, the vertex shader input variables form an interface with the vertex input attributes.
    All vertex shader inputs declared must have a corresponding attribute and binding in the pipeline.
*/

#ifndef AVX_VERTEX_DECL_H
#define AVX_VERTEX_DECL_H

#include "qwadro/inc/draw/avxFormat.h"

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

AFX_DEFINE_STRUCT(avxVertexFetch)
{
    // The binding number which this stream takes its data from.
    afxUnit         pin;
    afxUnit32       minStride;
    // The number of successive instances that will use the same value of the vertex attribute when instanced rendering is enabled.
    // 0 = disabled
    afxUnit         instRate;
    afxUnit         baseAttrIdx;
    afxUnit         attrCnt;
    afxFlags        flags;
};

AFX_DEFINE_STRUCT(avxVertexAttr)
// vertex attribute input stream.
{
    // The shader input location number for this attribute.
    afxUnit         location;
    // A byte offset of this attribute relative to the start of an element in the vertex input binding.
    afxUnit32       offset;
    // Special flags used to opportunistic optimization.
    afxFlags        flags;
    // The size and type of the vertex attribute data.
    avxFormat       fmt;
};

AFX_DEFINE_STRUCT(avxVertexLayout)
{
    // Special flags used to opportunistic optimization.
    afxFlags        flags;
    afxUnit         srcCnt;
    avxVertexFetch  srcs[AVX_MAX_VERTEX_SOURCES];
    avxVertexAttr   attrs[AVX_MAX_VERTEX_ATTRIBS];
    afxString       tag;
    void*           udd;
};

#define AVX_VERTEX_FETCH(uPin, uMinStride, uInstRate, uBaseAttrIdx, uAttrCnt) \
    (avxVertexFetch){ .pin = (uPin), .minStride = (uMinStride), .instRate = (uInstRate), .baseAttrIdx = (uBaseAttrIdx), .attrCnt = (uAttrCnt) }

#define AVX_VERTEX_ATTR(uLocation, uOffset, eFmt) \
    (avxVertexAttr){ .location = (uLocation), .offset = (uOffset), .fmt = (eFmt) }

AVX afxUnit     AvxDescribeVertexLayout(avxVertexInput vin, avxVertexLayout* layout);

AVX afxUnit     AvxQueryVertexStride(avxVertexInput vin, afxUnit baseSrcIdx, afxUnit cnt, afxUnit vtxSrcSiz[]);

////////////////////////////////////////////////////////////////////////////////

AVX afxError    AvxDeclareVertexInputs(afxDrawSystem dsys, afxUnit cnt, avxVertexLayout const layouts[], avxVertexInput declarations[]);

#endif//AVX_VERTEX_DECL_H
