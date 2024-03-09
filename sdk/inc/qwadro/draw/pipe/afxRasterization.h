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

// This section is part of SIGMA GL/2.

#ifndef AFX_RASTERIZATION_H
#define AFX_RASTERIZATION_H

#include "qwadro/math/afxViewport.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/draw/pipe/afxSampler.h"
#include "qwadro/draw/font/afxTypography.h"
#include "qwadro/draw/pipe/afxQueryPool.h"

typedef enum afxSurfaceStoreOp
/// An enumerated value indicating the load operation to perform on target surface prior to executing the render pass
{
    /// Stores the resulting value of the render pass for this attachment.
    afxSurfaceStoreOp_STORE,

    /// Discards the resulting value of the render pass for this attachment.
    afxSurfaceStoreOp_DISCARD,

    afxSurfaceStoreOp_DONT_CARE,
} afxSurfaceStoreOp;

typedef enum afxSurfaceLoadOp
/// An enumerated value indicating the store operation to perform on target surface after executing the render pass
{
    /// Loads the clearValue for this attachment into the render pass.
    afxSurfaceLoadOp_CLEAR,

    /// Loads the existing value for this attachment into the render pass.
    afxSurfaceLoadOp_LOAD,

    afxSurfaceLoadOp_DONT_CARE
} afxSurfaceLoadOp;

AFX_DEFINE_STRUCT(afxClearRect)
{
    afxRect             rect; /// the two-dimensional region to be cleared.
    afxNat32            baseLayer; /// the first layer to be cleared.
    afxNat32            layerCnt; /// the number of layers to clear.
};

AFX_DEFINE_STRUCT(afxDrawTarget)
{
    afxSurfaceLoadOp    loadOp; /// Indicates the load operation to perform on view prior to executing the render pass.
    afxSurfaceStoreOp   storeOp; /// The store operation to perform on view after executing the render pass.
    afxClearValue       clearValue; /// Indicates the value to clear view to prior to executing the render pass.
};

AFX_DEFINE_STRUCT(afxSynthesisConfig)
{
    afxCanvas           canv;
    afxRect             area;
    afxNat              layerCnt;
    afxNat              rasterCnt;
    afxDrawTarget const*rasters;
    afxDrawTarget const*depth;
    afxDrawTarget const*stencil;
};

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

/// Set the scissor count and scissor rectangular bounds dynamically for a command buffer.
/// This command sets the scissor count and scissor rectangular bounds state for subsequent drawing commands when the graphics pipeline is created without scissor set.

AVX afxCmdId                AfxCmdResetScissors
(
    afxDrawScript           dscr,
    afxNat                  cnt, /// specifies the scissor count.
    afxRect const           rc[] /// specifies the scissors to use for drawing.
);

/// Set scissor rectangles dynamically for a command buffer.
/// This command sets the scissor rectangles for subsequent drawing commands when drawing using shader objects, or when the graphics pipeline is created without scissor set.

/// The scissor rectangles taken from element #i of @rect replace the current state for the scissor index @baseIdx + #i, for #i in [0, @cnt).

AVX afxCmdId                AfxCmdReadjustScissors
(
    afxDrawScript           dscr,
    afxNat                  baseIdx, /// is the index of the first scissor whose state is updated by the command.
    afxNat                  cnt, /// is the number of scissors whose rectangles are updated by the command.
    afxRect const           rect[] /// is a pointer to an array of afxRect structures defining scissor rectangles.
);

/// Begin a dynamic render canvas instance
/// After beginning a render canvas instance, the command buffer is ready to record draw commands.

/// If flags includes resuming then this render pass is resumed from a render canvas instance that has been suspended earlier in submission order.

AVX afxCmdId                AfxCmdBeginSynthesis
(
    afxDrawScript           dscr,
    afxSynthesisConfig const*state /// is a pointer to a afxSynthesisConfig structure specifying details of the render pass instance to begin.
);

/// End a dynamic render canvas instance.
/// If the value of flags used to begin this render pass instance included suspending, then this render canvas is suspended and will be resumed later in submission order.

AVX afxCmdId                AfxCmdFinishSynthesis
(
    afxDrawScript           dscr
);

/// Transition to the next pass of a render canvas.
/// The pass index for a render canvas begins at zero when AfxCmdBeginSynthesis is recorded, and increments each time AfxCmdNextPass is recorded.

/// After transitioning to the next pass, the application can record the commands for that pass.

AVX afxCmdId                AfxCmdNextPass
(
    afxDrawScript           dscr,
    afxBool                 useAuxScripts /// specifies how the commands in the next subpass will be provided, in the same fashion as the corresponding parameter of vkCmdBeginRenderPass.
);

AVX afxCmdId                AfxCmdDisableRasterization
(
    afxDrawScript           dscr,
    afxBool                 disable /// controls whether primitives are discarded immediately before the rasterization stage.
);

AVX afxCmdId                AfxCmdEnableDepthBias
(
    afxDrawScript           dscr,
    afxBool                 enable /// controls whether to bias fragment depth values.
);

AVX afxCmdId                AfxCmdSetDepthBias
(
    afxDrawScript           dscr,
    afxReal                 constFactor, /// is a scalar factor controlling the constant depth value added to each fragment.
    afxReal                 clamp, /// is the maximum (or minimum) depth bias of a fragment.
    afxReal                 slopeFactor /// is a scalar factor applied to a fragment’s slope in depth bias calculations.
);

AVX afxCmdId                AfxCmdSetLineWidth
(
    afxDrawScript           dscr,
    afxReal                 lineWidth /// is the width of rasterized line segments.
);

AVX afxCmdId                AfxCmdEnableDepthTest
(
    afxDrawScript           dscr,
    afxBool                 enable /// specifies if the depth test is enabled.
);

AVX afxCmdId                AfxCmdSetDepthCompareOp
(
    afxDrawScript           dscr,
    afxCompareOp            op /// is a afxCompareOp value specifying the comparison operator used for the Depth Comparison step of the depth test.
);

AVX afxCmdId                AfxCmdDisableDepthWrite
(
    afxDrawScript           dscr,
    afxBool                 enable /// specifies if depth writes are enabled.
);

AVX afxCmdId                AfxCmdEnableStencilTest
(
    afxDrawScript           dscr,
    afxBool                 enable /// specifies if the stencil test is enabled.
);

AVX afxCmdId                AfxCmdSetStencilCompareMask
(
    afxDrawScript           dscr,
    afxMask                 faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to update the compare mask.
    afxNat32                compareMask /// is the new value to use as the stencil compare mask.
);

AVX afxCmdId                AfxCmdSetStencilWriteMask
(
    afxDrawScript           dscr,
    afxMask                 faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to update the write mask, as described above for vkCmdSetStencilCompareMask.
    afxNat32                writeMask /// is the new value to use as the stencil write mask.
);

AVX afxCmdId                AfxCmdSetStencilReference
(
    afxDrawScript           dscr,
    afxMask                 faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to
    afxNat32                reference /// is the new value to use as the stencil reference value.
);

AVX afxCmdId                AfxCmdEnableDepthBoundsTest
(
    afxDrawScript           dscr,
    afxBool                 enable /// specifies if the depth bounds test is enabled.
);

AVX afxCmdId                AfxCmdSetDepthBounds
(
    afxDrawScript           dscr,
    afxReal const           bounds[2] /// is the minimum and maximum depth bounds.
);

AVX afxCmdId                AfxCmdSetBlendConstants
(
    afxDrawScript           dscr,
    afxReal const           blendConstants[4] /// is an array of four values specifying the Rc, Gc, Bc, and Ac components of the blend constant color used in blending, depending on the blend factor.
);

#endif//AFX_RASTERIZATION_H
