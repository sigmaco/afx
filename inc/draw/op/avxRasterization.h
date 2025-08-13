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

#ifndef AVX_RASTERIZATION_H
#define AVX_RASTERIZATION_H

#include "qwadro/inc/math/avxViewport.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/draw/io/avxRaster.h"
#include "qwadro/inc/draw/io/avxBuffer.h"
#include "qwadro/inc/draw/op/avxSampler.h"
#include "qwadro/inc/draw/op/avxQueryPool.h"
#include "qwadro/inc/draw/op/avxTransference.h"
#include "qwadro/inc/draw/video/avxCanvas.h"

// This is the maximum number of viewports and scissors supported by AVX.
// This is the maximum even if the driver offer more than it.
// It is because this value is used to hold values in stack, to avoid allocation.
#define AVX_MAX_VIEWPORTS (16)

typedef enum avxResolveOp
// Bitmask indicating supported depth and stencil resolve modes.
{
    // no resolve operation is done.
    avxResolveOp_NONE = 0,

    // result of the resolve operation is equal to the value of sample 0.
    avxResolveOp_SAMPLE_ZERO = AFX_BITMASK(0),

    // result of the resolve operation is the average of the sample values.
    avxResolveOp_AVERAGE = AFX_BITMASK(1),

    // result of the resolve operation is the minimum of the sample values.
    avxResolveOp_MIN = AFX_BITMASK(2),

    // result of the resolve operation is the maximum of the sample values.
    avxResolveOp_MAX = AFX_BITMASK(3),
} avxResolveOp;

typedef enum avxStoreOp
// An enumerated value indicating the load operation to perform on target surface prior to executing the draw scope.
{
    // Stores the resulting value of the draw scope for this surface.
    // The contents generated during the draw scope and within the draw area are written to memory.
    avxStoreOp_STORE,

    // Discards the resulting value of the draw scope for this surface.
    // The contents within the draw area are not accessed by the store operation as long as no values are written to the surface during the draw scope.
    avxStoreOp_DISCARD,

    // The contents within the draw area are not needed after rendering, and may be discarded; the contents of the surface will be undefined inside the draw scope.
    avxStoreOp_DONT_CARE,
} avxStoreOp;

typedef enum avxLoadOp
// An enumerated value indicating the store operation to perform on target surface after executing the draw scope.
{
    // Loads the existing value for this surface into the draw scope.
    // The previous contents of the raster within the draw area will be preserved as the initial values.
    avxLoadOp_LOAD,

    // Loads the clearValue for this surface into the draw scope.
    // The contents within the draw area will be cleared to a uniform value, which is specified when a draw scope is begun
    avxLoadOp_CLEAR,

    // The previous contents within the area need not be preserved; the contents of the surface will be undefined inside the draw area.
    avxLoadOp_DONT_CARE
} avxLoadOp;

AFX_DEFINE_STRUCT(avxClearRect)
// Structure specifying a clear rectangle.
{
    // the two-dimensional region to be cleared.
    afxRect             area;
    // The first layer to be cleared.
    afxUnit32           baseLayer;
    // The number of layers to clear.
    afxUnit32           layerCnt;
};

AFX_DEFINE_STRUCT(avxDrawTarget)
// Structure specifying draw target information.
{
#if 0 // EXPERIMENTAL
    // The raster that will be used as buffer for drawing.
    avxRaster           buf;
    // An raster used to write resolved data at the end of drawing.
    avxRaster           resolved;
    // An value defining how data written to @ras will be resolved into @resolved.
    avxResolveOp        resolveOp;
#endif
    // The store operation to perform on buffer after executing the draw scope.
    avxStoreOp          storeOp;
    // Indicates the load operation to perform on buffer prior to executing the draw scope.
    avxLoadOp           loadOp;
    // Indicates the value to clear buffer to prior to executing the draw scope.
    avxClearValue       clearVal;
};

AFX_DEFINE_STRUCT(avxDrawScope)
// Structure specifying draw scope begin information.
{
    // The canvas containing the attached buffers that are used with the draw scope.
    avxCanvas           canv;
    // The area that is affected by the draw scope.
    afxRect             area;
    // The index of the first attachment layer that will be drawn.
    afxUnit             baseLayer;
    // The number of layers drawn to in each attachment when viewMask is 0.
    afxUnit             layerCnt;
    // The number of video surfaces.
    afxUnit             targetCnt;
    // Structures describing any color attachments used.
    avxDrawTarget       targets[AVX_MAX_COLOR_BUFFERS];
    // Structure describing a depth attachment.
    avxDrawTarget       depth;
    // Structure describing a stencil attachment.
    avxDrawTarget       stencil;

    afxString           tag;
};

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

/*
    The AvxCmdCommenceDrawScope() operation commences a new drawing scope.
    After beginning a drawing scope, the drawing context is ready to record draw commands.

    If flags includes resuming then this draw scope is resumed from a drawing scope that has been suspended earlier in submission order.
*/

AVX afxCmdId AvxCmdCommenceDrawScope
(
    afxDrawContext      dctx,
    // Structure specifying details of the draw scope instance to begin.
    avxDrawScope const* cfg
);

/*
    The AvxCmdNextPass() operation transitions to the next pass of a drawing scope.

   The pass index for a render canvas begins at zero when AvxCmdCommenceDrawScope is recorded, and increments each time AvxCmdNextPass is recorded.

   After transitioning to the next pass, the application can record the commands for that pass.
*/

AVX afxCmdId AvxCmdNextPass
(
    afxDrawContext      dctx,
    // specifies how the commands in the next subpass will be provided, in the same fashion as the corresponding parameter of vkCmdBeginRenderPass.
    afxBool             useAuxScripts
);

/*
    The AvxCmdConcludeDrawScope() operation concludes the current drawing scope.

    If the value of flags used to begin this draw scope instance included suspending, 
    then this render canvas is suspended and will be resumed later in submission order.
*/

AVX afxCmdId AvxCmdConcludeDrawScope
(
    afxDrawContext      dctx
);

/*
    The AvxCmdAdjustScissors() operation adjusts scissor rectangles dynamically for a draw context.

    This command sets the scissor rectangles for subsequent drawing commands when drawing using shader objects, 
    or when the graphics pipeline is created without scissor set.

    The scissor rectangles taken from element #i of @rect replace the current state for the scissor index @baseIdx + #i, for #i in [0, @cnt).
*/

AVX afxCmdId AvxCmdAdjustScissors
(
    afxDrawContext      dctx,
    // is the index of the first scissor whose state is updated by the command.
    afxUnit             baseIdx,
     // is the number of scissors whose rectangles are updated by the command.
    afxUnit             cnt,
     // is a pointer to an array of afxRect structures defining scissor rectangles.
    afxRect const       rects[]
);

/*
    The AvxCmdSwitchRasterization() operation controls whether primitives are discarded before the 
    rasterization stage dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSwitchRasterization
(
    afxDrawContext      dctx,
    // controls whether primitives are discarded immediately before the rasterization stage.
    afxBool             disabled
);

/*
    The AvxCmdSwitchDepthBias() operation controls whether to bias fragment depth values dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSwitchDepthBias
(
    afxDrawContext      dctx,
    // controls whether to bias fragment depth values.
    afxBool             enabled
);

/*
    The AvxCmdSetDepthBias() operation sets depth bias factors and clamp dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSetDepthBias
(
    afxDrawContext      dctx,
    // is a scalar factor controlling the constant depth value added to each fragment.
    afxReal             constFactor,
     // is the maximum (or minimum) depth bias of a fragment.
    afxReal             clamp,
     // is a scalar factor applied to a fragment’s slope in depth bias calculations.
    afxReal             slopeFactor
);

/*
    The AvxCmdSetLineWidth() operation sets line width dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSetLineWidth
(
    afxDrawContext      dctx,
    // is the width of rasterized line segments.
    afxReal             lineWidth
);

/*
    The AvxCmdSwitchDepthTesting() operation sets depth test enable dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSwitchDepthTesting
(
    afxDrawContext      dctx,
    // specifies if the depth test is enabled.
    afxBool             enable
);

/*
    The AvxCmdSetDepthCompareOp() operation sets depth comparison operator dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSetDepthCompareOp
(
    afxDrawContext      dctx,
    // A value specifying the comparison operator used for the depth comparison step of the depth test.
    avxCompareOp        op
);

/*
    The AvxCmdSwitchDepthWrites() operation sets depth write disable dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSwitchDepthWrites
(
    afxDrawContext      dctx,
    // specifies if depth writes are disabled.
    afxBool             disable
);

/*
    The AvxCmdSwitchStencilTesting() operation sets stencil test enable dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSwitchStencilTesting
(
    afxDrawContext      dctx,
    // specifies if the stencil test is enabled.
    afxBool             enable
);

/*
    The AvxCmdSetStencilCompareMask() operation sets stencil compare mask dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSetStencilCompareMask
(
    afxDrawContext      dctx,
    // is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to update the compare mask.
    avxFaceMask         faceMask,
     // is the new value to use as the stencil compare mask.
    afxMask             compareMask
);

/*
    The AvxCmdSetStencilWriteMask() operation sets stencil write mask dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSetStencilWriteMask
(
    afxDrawContext      dctx,
    // is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to update the write mask, as described above for vkCmdSetStencilCompareMask.
    avxFaceMask         faceMask,
     // is the new value to use as the stencil write mask.
    afxMask             writeMask
);

/*
    The AvxCmdSetStencilReference() operation sets stencil reference value dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSetStencilReference
(
    afxDrawContext      dctx,
    // is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to
    avxFaceMask         faceMask,
    // is the new value to use as the stencil reference value.
    afxUnit32           reference
);

/*
    The AvxCmdSetStencilOp() operation sets stencil operation dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSetStencilOp
(
    afxDrawContext      dctx,
    // is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to
    avxFaceMask         faceMask,
    // The action performed on samples that fail the stencil test.
    avxStencilOp        failOp,
    // The action performed on samples that pass both the depth and stencil tests.
    avxStencilOp        passOp,
    // The action performed on samples that pass the stencil test and fail the depth test.
    avxStencilOp        depthFailOp,
    // The comparison operator used in the stencil test.
    avxCompareOp        compareOp
);

/*
    The AvxCmdSwitchDepthBoundsTesting() operation sets depth bounds test enable dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSwitchDepthBoundsTesting
(
    afxDrawContext      dctx,
    // specifies if the depth bounds test is enabled.
    afxBool             enable
);

/*
    The AvxCmdSetDepthBounds() operation sets depth bounds range dynamically for a draw context.
*/

AVX afxCmdId AvxCmdSetDepthBounds
(
    afxDrawContext      dctx,
    // is the minimum and maximum depth bounds.
    afxV2d const        bounds
);

/*
    The AvxCmdSetBlendConstants() operation sets the values of blend constants.
*/

AVX afxCmdId AvxCmdSetBlendConstants
(
    afxDrawContext      dctx,
    // An array of four values specifying the Rc, Gc, Bc, and Ac components of the 
    // blend constant color used in blending, depending on the blend factor.
    afxV4d const        blendConstants
);

#endif//AVX_RASTERIZATION_H
