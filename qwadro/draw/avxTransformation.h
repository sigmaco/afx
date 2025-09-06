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
 // GEOMETRIC PRIMITIVE TRANSFORMATION PIPELINE OPERATIONS                   //
//////////////////////////////////////////////////////////////////////////////

// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_TRANSFORMATION_H
#define AVX_TRANSFORMATION_H

#include "qwadro/draw/avxViewport.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/avxRaster.h"
#include "qwadro/draw/avxBuffer.h"
#include "qwadro/draw/avxSampler.h"
#include "qwadro/draw/avxQueryPool.h"

// Specify the vertex input layout for consequent vertex fetching for a draw context.

AVX afxCmdId            AvxCmdUseVertexInput
(
    afxDrawContext      dctx,
    avxVertexInput      vin
);

/*
    Bind vertex buffers to a draw context and dynamically set strides.
    The values taken from elements #i of @buf and @offset replace the current state for the vertex input binding @basePin + #i, for #i in [0, @cnt].
    The vertex input binding is updated to start at the offset indicated by @offset[#i] from the start of the buffer @buf[#i].
    If @range is not NIL then @range[#i] specifies the bound size of the vertex buffer starting from the corresponding elements of @buf[#i] plus @offset[#i].
    If @range[#i] is WHOLE_SIZE then the bound size is from @buf[#i] plus @offset[#i] to the end of the buffer @buf[#i].
    All vertex input attributes that use each of these bindings will use these updated addresses in their address calculations for subsequent drawing commands.
    If the null descriptor feature is enabled, elements of @buf can be NIL, and can be used by the vertex shader.
    If a vertex input attribute is bound to a vertex input binding that is NIL, the values taken from memory are considered to be zero, and missing G, B, or A components are filled with (0,0,1).

    This command also dynamically sets the byte strides between consecutive elements within buffer @buf[#i] to the corresponding @stride[#i] value when drawing using shader objects, or when the graphics pipeline is created without vertex input binding stride set.

    The ranges inside the buffers are not required, but they should be provided to help debugging buffer access and hinting the implementation about usage behavior.
*/

AVX afxCmdId            AvxCmdBindVertexBuffers
(
    afxDrawContext      dctx,
    // The index of the first vertex input binding whose state is updated by the command.
    afxUnit             basePin,
    // The number of vertex input bindings whose state is updated by the command.
    afxUnit             cnt,
    // An array of info to set up the avxBuffer-backed streams.
    avxBufferedStream const streams[]
);

/*
    The AvxCmdBindIndexBuffer() command binds an index buffer to a draw context for use by consequent indexed drawing commands.

    The range inside the buffer is not required, but it should be provided to help debugging buffer access and hinting the implementation about usage behavior.
*/

AVX afxCmdId            AvxCmdBindIndexBuffer
(
    afxDrawContext      dctx,
    // The buffer being bound.
    avxBuffer           buf,
    // The starting offset in bytes within buffer used in index buffer address calculations.
    afxUnit32           offset,
    // The size in bytes of index data bound from buffer.
    afxUnit32           range,
    // A value specifying the size of the indices.
    afxUnit32           idxSiz
);

// Set the viewport dynamically for a draw context.
// This command sets the viewport transformation parameters state for subsequent drawing commands when the graphics pipeline is created without viewport set.

// The viewport parameters taken from element #i of @vp replace the current state for the viewport index @baseIdx + #i, for #i in[0, @cnt).

AVX afxCmdId            AvxCmdAdjustViewports
(
    afxDrawContext      dctx,
    // The index of the first viewport whose parameters are updated by the command.
    afxUnit             baseIdx,
     // The number of viewports whose parameters are updated by the command.
    afxUnit             cnt,
     // An array of avxViewport structures specifying viewport parameters.
    avxViewport const   viewports[]
);

/*
    Set primitive topology state dynamically for a draw context.
    This command sets the primitive topology for subsequent drawing commands when drawing using shader objects, or when the graphics pipeline is created without primitive topology set.
*/

AVX afxCmdId            AvxCmdSetPrimitiveTopology
(
    afxDrawContext      dctx,
    // The primitive topology to use for drawing.
    avxTopology         topology
);

// Set cull mode dynamically for a draw context.

AVX afxCmdId            AvxCmdChangeCullMode
(
    afxDrawContext      dctx,
    // specifies the cull mode property to use for drawing.
    avxCullMode         mode
);

// Set front face orientation dynamically for a draw context.

AVX afxCmdId            AvxCmdSwitchFrontFace
(
    afxDrawContext      dctx,
    // Specifying if the front-facing triangle orientation to be used for culling is CW otherwise it is CCW.
    afxBool             cw
);

#endif//AVX_TRANSFORMATION_H
