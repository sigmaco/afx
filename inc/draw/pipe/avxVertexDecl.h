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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/**
    When the vertex stage is present in a pipeline, the vertex shader input variables form an interface with the vertex input attributes.
    All vertex shader inputs declared must have a corresponding attribute and binding in the pipeline.
*/

#ifndef AVX_VERTEX_INPUT_H
#define AVX_VERTEX_INPUT_H

#include "qwadro/inc/draw/io/afxPixel.h"

AFX_DEFINE_STRUCT(avxVertexSource)
{
    afxBuffer       buf;
    afxUnit32       offset; // the start of buffer.
    afxUnit32       range; // the size in bytes of vertex data bound from buffer.
    afxUnit32       stride; // the byte stride between consecutive elements within the buffer.
    //afxBool8      instance; // addressing is a function of the instance index, else case, it is of the vertex index.
    //afxUnit32     instDivisor; // the number of successive instances that will use the same value of the vertex attribute when instanced rendering is enabled.
};

AFX_DEFINE_STRUCT(avxVertexFetch)
{
    afxUnit         srcIdx;
    //afxUnit32     stride;
    afxUnit         instanceRate; // 0 = disabled /// the number of successive instances that will use the same value of the vertex attribute when instanced rendering is enabled.
};

AFX_DEFINE_STRUCT(avxVertexInput)
/// vertex attribute input stream
{
    afxUnit         location; // is the shader input location number for this attribute.
    afxUnit         srcIdx; // is the binding number which this attribute takes its data from.
    afxUnit32       offset; // is a byte offset of this attribute relative to the start of an element in the vertex input binding.
    avxFormat       fmt; // is the size and type of the vertex attribute data.
    //afxVertexUsage      usage; // special flags used to opportunistic optimization
};

AVX afxUnit     AfxCountVertexInputStreams(avxVertexDecl vin);
AVX afxUnit     AfxCountVertexInputAttributes(avxVertexDecl vin);

AVX afxUnit     AfxMeasureVertexInputStream(avxVertexDecl vin, afxUnit srcIdx);

////////////////////////////////////////////////////////////////////////////////

AVX afxError    AfxDeclareVertexLayout(afxDrawSystem dsys, afxUnit streamCnt, avxVertexFetch const streams[], afxUnit attrCnt, avxVertexInput const attrs[], avxVertexDecl* decl);

#endif//AVX_VERTEX_INPUT_H
