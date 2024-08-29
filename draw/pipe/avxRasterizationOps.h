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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_IMAGING_OPS_H
#define AFX_IMAGING_OPS_H

#include "qwadro/math/afxViewport.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/afxRaster.h"
#include "qwadro/draw/afxBuffer.h"
#include "qwadro/draw/pipe/avxSampler.h"
#include "qwadro/draw/io/afxTypography.h"
#include "qwadro/draw/pipe/avxQueryPool.h"

typedef enum avxStoreOp
/// An enumerated value indicating the load operation to perform on target surface prior to executing the render pass
{
    /// Stores the resulting value of the render pass for this attachment.
    avxStoreOp_STORE,

    /// Discards the resulting value of the render pass for this attachment.
    avxStoreOp_DISCARD,

    avxStoreOp_DONT_CARE,
} avxStoreOp;

typedef enum avxLoadOp
/// An enumerated value indicating the store operation to perform on target surface after executing the render pass
{
    /// Loads the clearValue for this attachment into the render pass.
    avxLoadOp_CLEAR,

    /// Loads the existing value for this attachment into the render pass.
    avxLoadOp_LOAD,

    avxLoadOp_DONT_CARE
} avxLoadOp;

AFX_DEFINE_STRUCT(avxClearRect)
{
    afxRect             area; /// the two-dimensional region to be cleared.
    afxNat32            baseLayer; /// the first layer to be cleared.
    afxNat32            layerCnt; /// the number of layers to clear.
};

AFX_DEFINE_STRUCT(avxDrawTarget)
{
    avxLoadOp           loadOp; /// Indicates the load operation to perform on view prior to executing the render pass.
    avxStoreOp          storeOp; /// The store operation to perform on view after executing the render pass.
    avxClearValue       clearValue; /// Indicates the value to clear view to prior to executing the render pass.
};

AFX_DEFINE_STRUCT(avxSynthesisConfig)
{
    avxCanvas           canv;
    afxRect             area;
    afxNat              layerCnt;
    afxNat              rasterCnt;
    avxDrawTarget const*rasters;
    avxDrawTarget const*depth;
    avxDrawTarget const*stencil;
};

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

/// Set scissor rectangles dynamically for a command buffer.
/// This command sets the scissor rectangles for subsequent drawing commands when drawing using shader objects, or when the graphics pipeline is created without scissor set.

/// The scissor rectangles taken from element #i of @rect replace the current state for the scissor index @baseIdx + #i, for #i in [0, @cnt).

AVX afxCmdId                AvxCmdAdjustScissors
(
    avxCmdb                 cmdb,
    afxNat                  baseIdx, /// is the index of the first scissor whose state is updated by the command.
    afxNat                  cnt, /// is the number of scissors whose rectangles are updated by the command.
    afxRect const           rects[] /// is a pointer to an array of afxRect structures defining scissor rectangles.
);

/// The curtain test behaves like the scissor test, except that the curtain test fails for pixels inside the corresponding rectangle and passes for pixels outside the rectangle. 
/// If the same rectangle is used for both the scissor and curtain tests, the curtain test will pass if and only if the scissor test fails.

AVX afxCmdId                AvxCmdAdjustCurtains
(
    avxCmdb                 cmdb, 
    afxNat                  baseIdx, /// the first curtain rectangle whose state is updated by the command.
    afxNat                  cnt, /// the number of curtain rectangles updated by the command.
    afxRect const           rects[] /// an array of afxRect structures defining curtain rectangles.
);

/// Begin a dynamic render canvas instance
/// After beginning a render canvas instance, the command buffer is ready to record draw commands.

/// If flags includes resuming then this render pass is resumed from a render canvas instance that has been suspended earlier in submission order.

AVX afxCmdId                AvxCmdBeginDrawScope
(
    avxCmdb                 cmdb,
    avxSynthesisConfig const*cfg /// is a pointer to a avxSynthesisConfig structure specifying details of the render pass instance to begin.
);

/// End a dynamic render canvas instance.
/// If the value of flags used to begin this render pass instance included suspending, then this render canvas is suspended and will be resumed later in submission order.

AVX afxCmdId                AvxCmdEndDrawScope
(
    avxCmdb                 cmdb
);

/// Transition to the next pass of a render canvas.
/// The pass index for a render canvas begins at zero when AvxCmdBeginDrawScope is recorded, and increments each time AvxCmdNextPass is recorded.

/// After transitioning to the next pass, the application can record the commands for that pass.

AVX afxCmdId                AvxCmdNextPass
(
    avxCmdb                 cmdb,
    afxBool                 useAuxScripts /// specifies how the commands in the next subpass will be provided, in the same fashion as the corresponding parameter of vkCmdBeginRenderPass.
);

/// Control whether primitives are discarded before the rasterization stage dynamically for a command buffer.

AVX afxCmdId                AvxCmdDisableRasterization
(
    avxCmdb                 cmdb,
    afxBool                 disable /// controls whether primitives are discarded immediately before the rasterization stage.
);

/// Control whether to bias fragment depth values dynamically for a command buffer.

AVX afxCmdId                AvxCmdEnableDepthBias
(
    avxCmdb                 cmdb,
    afxBool                 enable /// controls whether to bias fragment depth values.
);

/// Set depth bias factors and clamp dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetDepthBias
(
    avxCmdb                 cmdb,
    afxReal                 constFactor, /// is a scalar factor controlling the constant depth value added to each fragment.
    afxReal                 clamp, /// is the maximum (or minimum) depth bias of a fragment.
    afxReal                 slopeFactor /// is a scalar factor applied to a fragment’s slope in depth bias calculations.
);

/// Set line width dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetLineWidth
(
    avxCmdb                 cmdb,
    afxReal                 lineWidth /// is the width of rasterized line segments.
);

/// Set depth test enable dynamically for a command buffer.

AVX afxCmdId                AvxCmdEnableDepthTest
(
    avxCmdb                 cmdb,
    afxBool                 enable /// specifies if the depth test is enabled.
);

/// Set depth comparison operator dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetDepthCompareOp
(
    avxCmdb                 cmdb,
    avxCompareOp            op /// is a avxCompareOp value specifying the comparison operator used for the Depth Comparison step of the depth test.
);

/// Set depth write disable dynamically for a command buffer.

AVX afxCmdId                AvxCmdDisableDepthWrite
(
    avxCmdb                 cmdb,
    afxBool                 disable /// specifies if depth writes are disabled.
);

/// Set stencil test enable dynamically for a command buffer.

AVX afxCmdId                AvxCmdEnableStencilTest
(
    avxCmdb                 cmdb,
    afxBool                 enable /// specifies if the stencil test is enabled.
);

/// Set stencil compare mask dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetStencilCompareMask
(
    avxCmdb                 cmdb,
    afxMask                 faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to update the compare mask.
    afxNat32                compareMask /// is the new value to use as the stencil compare mask.
);

/// Set stencil write mask dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetStencilWriteMask
(
    avxCmdb                 cmdb,
    afxMask                 faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to update the write mask, as described above for vkCmdSetStencilCompareMask.
    afxNat32                writeMask /// is the new value to use as the stencil write mask.
);

/// Set stencil reference value dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetStencilReference
(
    avxCmdb                 cmdb,
    afxMask                 faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to
    afxNat32                reference /// is the new value to use as the stencil reference value.
);

/// Set depth bounds test enable dynamically for a command buffer.

AVX afxCmdId                AvxCmdEnableDepthBoundsTest
(
    avxCmdb                 cmdb,
    afxBool                 enable /// specifies if the depth bounds test is enabled.
);

/// Set depth bounds range dynamically for a command buffer.

AVX afxCmdId                AvxCmdSetDepthBounds
(
    avxCmdb                 cmdb,
    afxV2d const            bounds /// is the minimum and maximum depth bounds.
);

/// Set the values of blend constants.

AVX afxCmdId                AvxCmdSetBlendConstants
(
    avxCmdb                 cmdb,
    afxV4d const            blendConstants /// is an array of four values specifying the Rc, Gc, Bc, and Ac components of the blend constant color used in blending, depending on the blend factor.
);

#endif//AFX_IMAGING_OPS_H
