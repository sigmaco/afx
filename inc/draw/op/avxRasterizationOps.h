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

#ifndef AFX_IMAGING_OPS_H
#define AFX_IMAGING_OPS_H

#include "qwadro/inc/math/afxViewport.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/draw/io/afxRaster.h"
#include "qwadro/inc/draw/io/afxBuffer.h"
#include "qwadro/inc/draw/pipe/avxSampler.h"
#include "qwadro/inc/draw/pipe/avxQueryPool.h"
#include "qwadro/inc/draw/op/avxRasterOps.h"

typedef enum avxResolveOp
{
    // no resolve operation is done.
    avxResolveOp_NONE = 0,

    // result of the resolve operation is equal to the value of sample 0.
    avxResolveOp_SAMPLE_ZERO = AFX_BIT(0),

    // result of the resolve operation is the average of the sample values.
    avxResolveOp_AVERAGE = AFX_BIT(1),

    // result of the resolve operation is the minimum of the sample values.
    avxResolveOp_MIN = AFX_BIT(2),

    // result of the resolve operation is the maximum of the sample values.
    avxResolveOp_MAX = AFX_BIT(3),
} avxResolveOp;

typedef enum avxStoreOp
/// An enumerated value indicating the load operation to perform on target surface prior to executing the draw scope.
{
    /// Stores the resulting value of the draw scope for this surface.
    /// The contents generated during the draw scope and within the draw area are written to memory.
    avxStoreOp_STORE,

    /// Discards the resulting value of the draw scope for this surface.
    /// The contents within the draw area are not accessed by the store operation as long as no values are written to the surface during the draw scope.
    avxStoreOp_DISCARD,

    /// The contents within the draw area are not needed after rendering, and may be discarded; the contents of the surface will be undefined inside the draw scope.
    avxStoreOp_DONT_CARE,
} avxStoreOp;

typedef enum avxLoadOp
/// An enumerated value indicating the store operation to perform on target surface after executing the draw scope.
{
    /// Loads the existing value for this surface into the draw scope.
    /// The previous contents of the raster within the draw area will be preserved as the initial values.
    avxLoadOp_LOAD,

    /// Loads the clearValue for this surface into the draw scope.
    /// The contents within the draw area will be cleared to a uniform value, which is specified when a draw scope is begun
    avxLoadOp_CLEAR,

    /// The previous contents within the area need not be preserved; the contents of the surface will be undefined inside the draw area.
    avxLoadOp_DONT_CARE
} avxLoadOp;

AFX_DEFINE_STRUCT(avxClearRect)
{
    afxRect             area; /// the two-dimensional region to be cleared.
    afxUnit32           baseLayer; /// the first layer to be cleared.
    afxUnit32           layerCnt; /// the number of layers to clear.
};

AFX_DEFINE_STRUCT(avxDrawTarget)
{
    avxStoreOp          storeOp; /// The store operation to perform on view after executing the render pass.
    avxLoadOp           loadOp; /// Indicates the load operation to perform on view prior to executing the render pass.
    avxClearValue       clearValue; /// Indicates the value to clear view to prior to executing the render pass.
};

AFX_DEFINE_STRUCT(avxDrawScope)
{
    avxCanvas           canv;
    afxRect             area; ///  the area that is affected by the draw scope.
    afxUnit             baseLayer; /// the index of the first attachment layer that will be drawn.
    afxUnit             layerCnt; /// the number of layers drawn to in each attachment when viewMask is 0.
    afxUnit             targetCnt; /// the number of video surfaces.
    avxDrawTarget const*targets; /// structures describing any color attachments used.
    avxDrawTarget const*depth; /// structure describing a depth attachment.
    avxDrawTarget const*stencil; /// structure describing a stencil attachment.
};

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

/// Set scissor rectangles dynamically for a command buffer.
/// This command sets the scissor rectangles for subsequent drawing commands when drawing using shader objects, or when the graphics pipeline is created without scissor set.

/// The scissor rectangles taken from element #i of @rect replace the current state for the scissor index @baseIdx + #i, for #i in [0, @cnt).

AVX afxCmdId                AvxCmdAdjustScissors
(
    afxDrawContext          dctx,
    afxUnit                 baseIdx, /// is the index of the first scissor whose state is updated by the command.
    afxUnit                 cnt, /// is the number of scissors whose rectangles are updated by the command.
    afxRect const           rects[] /// is a pointer to an array of afxRect structures defining scissor rectangles.
);

/// The curtain test behaves like the scissor test, except that the curtain test fails for pixels inside the corresponding rectangle and passes for pixels outside the rectangle. 
/// If the same rectangle is used for both the scissor and curtain tests, the curtain test will pass if and only if the scissor test fails.

AVX afxCmdId                AvxCmdAdjustCurtains
(
    afxDrawContext          dctx, 
    afxUnit                 baseIdx, /// the first curtain rectangle whose state is updated by the command.
    afxUnit                 cnt, /// the number of curtain rectangles updated by the command.
    afxRect const           rects[] /// an array of afxRect structures defining curtain rectangles.
);

/// Begin a dynamic render canvas instance
/// After beginning a render canvas instance, the command buffer is ready to record draw commands.

/// If flags includes resuming then this render pass is resumed from a render canvas instance that has been suspended earlier in submission order.

AVX afxCmdId                AvxCmdCommenceDrawScope
(
    afxDrawContext          dctx,
    avxDrawScope const*     cfg /// structure specifying details of the draw scope instance to begin.
);

AVX afxCmdId                AvxSuspendDrawPass
(
    afxDrawContext          dctx
);

AVX afxCmdId                AvxResumeDrawPass
(
    afxDrawContext          dctx
);

/// End a dynamic render canvas instance.
/// If the value of flags used to begin this render pass instance included suspending, then this render canvas is suspended and will be resumed later in submission order.

AVX afxCmdId                AvxCmdConcludeDrawScope
(
    afxDrawContext          dctx
);

/// Transition to the next pass of a render canvas.
/// The pass index for a render canvas begins at zero when AvxCmdCommenceDrawScope is recorded, and increments each time AvxCmdNextPass is recorded.

/// After transitioning to the next pass, the application can record the commands for that pass.

AVX afxCmdId                AvxCmdNextPass
(
    afxDrawContext          dctx,
    afxBool                 useAuxScripts /// specifies how the commands in the next subpass will be provided, in the same fashion as the corresponding parameter of vkCmdBeginRenderPass.
);

/// Control whether primitives are discarded before the rasterization stage dynamically for a command buffer.

AVX afxCmdId                AvxCmdDisableRasterization
(
    afxDrawContext          dctx,
    afxBool                 disable /// controls whether primitives are discarded immediately before the rasterization stage.
);

/// Control whether to bias fragment depth values dynamically for a command buffer.

AVX afxCmdId                AvxCmdEnableDepthBias
(
    afxDrawContext          dctx,
    afxBool                 enable /// controls whether to bias fragment depth values.
);

/// Set depth bias factors and clamp dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetDepthBias
(
    afxDrawContext          dctx,
    afxReal                 constFactor, /// is a scalar factor controlling the constant depth value added to each fragment.
    afxReal                 clamp, /// is the maximum (or minimum) depth bias of a fragment.
    afxReal                 slopeFactor /// is a scalar factor applied to a fragment’s slope in depth bias calculations.
);

/// Set line width dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetLineWidth
(
    afxDrawContext          dctx,
    afxReal                 lineWidth /// is the width of rasterized line segments.
);

/// Set depth test enable dynamically for a command buffer.

AVX afxCmdId                AvxCmdEnableDepthTest
(
    afxDrawContext          dctx,
    afxBool                 enable /// specifies if the depth test is enabled.
);

/// Set depth comparison operator dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetDepthCompareOp
(
    afxDrawContext          dctx,
    avxCompareOp            op /// is a avxCompareOp value specifying the comparison operator used for the Depth Comparison step of the depth test.
);

/// Set depth write disable dynamically for a command buffer.

AVX afxCmdId                AvxCmdDisableDepthWrite
(
    afxDrawContext          dctx,
    afxBool                 disable /// specifies if depth writes are disabled.
);

/// Set stencil test enable dynamically for a command buffer.

AVX afxCmdId                AvxCmdEnableStencilTest
(
    afxDrawContext          dctx,
    afxBool                 enable /// specifies if the stencil test is enabled.
);

/// Set stencil compare mask dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetStencilCompareMask
(
    afxDrawContext          dctx,
    afxMask                 faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to update the compare mask.
    afxUnit32               compareMask /// is the new value to use as the stencil compare mask.
);

/// Set stencil write mask dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetStencilWriteMask
(
    afxDrawContext          dctx,
    afxMask                 faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to update the write mask, as described above for vkCmdSetStencilCompareMask.
    afxUnit32               writeMask /// is the new value to use as the stencil write mask.
);

/// Set stencil reference value dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetStencilReference
(
    afxDrawContext          dctx,
    afxMask                 faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to
    afxUnit32               reference /// is the new value to use as the stencil reference value.
);

/// Set depth bounds test enable dynamically for a command buffer.

AVX afxCmdId                AvxCmdEnableDepthBoundsTest
(
    afxDrawContext          dctx,
    afxBool                 enable /// specifies if the depth bounds test is enabled.
);

/// Set depth bounds range dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetDepthBounds
(
    afxDrawContext          dctx,
    afxV2d const            bounds /// is the minimum and maximum depth bounds.
);

/// Set the values of blend constants.

AVX afxCmdId                AvxCmdSetBlendConstants
(
    afxDrawContext          dctx,
    afxV4d const            blendConstants /// is an array of four values specifying the Rc, Gc, Bc, and Ac components of the blend constant color used in blending, depending on the blend factor.
);

#endif//AFX_IMAGING_OPS_H
