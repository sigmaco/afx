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

#ifndef AVX_TRANSFORM_OPS_H
#define AVX_TRANSFORM_OPS_H

#include "qwadro/inc/draw/math/afxViewport.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/draw/io/afxRaster.h"
#include "qwadro/inc/draw/io/afxBuffer.h"
#include "qwadro/inc/draw/pipe/avxSampler.h"
#include "qwadro/inc/draw/pipe/avxQueryPool.h"

AFX_DEFINE_STRUCT(avxBufferedStream)
/// Structured specifying a afxBuffer-backed stream.
{
    afxBuffer   buf; /// is an array of buffer handles.
    afxSize     offset; /// the start of buffer.
    afxUnit     range; /// the size in bytes of data bound from buffer.
    afxUnit     stride; /// the byte stride between consecutive elements within the buffer.
};

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

/// Set the viewport dynamically for a command buffer.
/// This command sets the viewport transformation parameters state for subsequent drawing commands when the graphics pipeline is created without viewport set.

/// The viewport parameters taken from element #i of @vp replace the current state for the viewport index @baseIdx + #i, for #i in[0, @cnt).

AVX afxCmdId            AvxCmdAdjustViewports
(
    afxDrawContext      dctx,
    afxUnit             baseIdx, /// is the index of the first viewport whose parameters are updated by the command.
    afxUnit             cnt, /// is the number of viewports whose parameters are updated by the command.
    afxViewport const   viewports[] /// is a pointer to an array of afxViewport structures specifying viewport parameters.
);

/// Bind vertex buffers to a command buffer and dynamically set strides.
/// The values taken from elements #i of @buf and @offset replace the current state for the vertex input binding @baseIdx + #i, for #i in [0, @cnt].
/// The vertex input binding is updated to start at the offset indicated by @offset[#i] from the start of the buffer @buf[#i].
/// If @range is not NIL then @range[#i] specifies the bound size of the vertex buffer starting from the corresponding elements of @buf[#i] plus @offset[#i].
/// If @range[#i] is WHOLE_SIZE then the bound size is from @buf[#i] plus @offset[#i] to the end of the buffer @buf[#i].
/// All vertex input attributes that use each of these bindings will use these updated addresses in their address calculations for subsequent drawing commands.
/// If the null descriptor feature is enabled, elements of @buf can be NIL, and can be used by the vertex shader.
/// If a vertex input attribute is bound to a vertex input binding that is NIL, the values taken from memory are considered to be zero, and missing G, B, or A components are filled with (0,0,1).

/// This command also dynamically sets the byte strides between consecutive elements within buffer @buf[#i] to the corresponding @stride[#i] value when drawing using shader objects, or when the graphics pipeline is created without vertex input binding stride set.

AVX afxCmdId            AvxCmdBindVertexBuffers
(
    afxDrawContext      dctx,
    afxUnit             baseSlotIdx, /// is the index of the first vertex input binding whose state is updated by the command.
    afxUnit             cnt, /// is the number of vertex input bindings whose state is updated by the command.
    avxBufferedStream const streams[] /// an array of info to set up the afxBuffer-backed streams.
);

/// Bind an index buffer to a command buffer.

AVX afxCmdId            AvxCmdBindIndexBuffer
(
    afxDrawContext      dctx,
    afxBuffer           buf, /// is the buffer being bound.
    afxUnit32           offset, /// is the starting offset in bytes within buffer used in index buffer address calculations.
    afxUnit32           range, /// is the size in bytes of index data bound from buffer.
    afxUnit32           idxSiz /// is a value specifying the size of the indices.
);

/// Set primitive topology state dynamically for a command buffer.
/// This command sets the primitive topology for subsequent drawing commands when drawing using shader objects, or when the graphics pipeline is created without primitive topology set.

AVX afxCmdId            AvxCmdSetPrimitiveTopology
(
    afxDrawContext      dctx,
    avxTopology         topology /// specifies the primitive topology to use for drawing.
);

/// Set front face orientation dynamically for a command buffer.

AVX afxCmdId            AvxCmdSwitchFrontFace
(
    afxDrawContext      dctx,
    afxBool             cw /// is a value specifying the front-facing triangle orientation to be used for culling.
);

/// Set cull mode dynamically for a command buffer.

AVX afxCmdId            AvxCmdSetCullMode
(
    afxDrawContext      dctx,
    avxCullMode         mode /// specifies the cull mode property to use for drawing.
);

AVX afxCmdId            AvxCmdDeclareVertex(afxDrawContext dctx, avxVertexDecl vin);

#endif//AVX_TRANSFORM_OPS_H
