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

// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_RASTERIZATION_H
#define AVX_RASTERIZATION_H

#include "qwadro/draw/avxViewport.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/avxRaster.h"
#include "qwadro/draw/avxBuffer.h"
#include "qwadro/draw/avxSampler.h"
#include "qwadro/draw/avxQueryPool.h"
#include "qwadro/draw/avxTransference.h"
#include "qwadro/draw/avxCanvas.h"

// This is the maximum number of viewports and scissors supported by AVX.
// This is the maximum even if the driver offer more than it.
// It is because this value is used to hold values in stack, to avoid allocation.
#define AVX_MAX_VIEWPORTS (16)
#define AVX_MAX_SAMPLE_MASKS (32)
#define AVX_MAX_COLOR_OUTPUTS (8)

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

AFX_DEFINE_STRUCT(avxDrawTarget)
// Structure specifying draw target information.
{
    // The index value of the canvas' buffer slot.
    afxUnit             bufIdx;
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

typedef enum avxDrawScopeFlag
{
    // The draw scope will be suspended.
    avxDrawScopeFlag_SUSPEND    = AFX_BITMASK(0),
    // The draw scope is resuming an earlier suspended draw scope.
    avxDrawScopeFlag_RESUME     = AFX_BITMASK(1),
} avxDrawScopeFlags;

AFX_DEFINE_STRUCT(avxDrawScope)
// Structure specifying draw scope begin information.
{
    avxDrawScopeFlags   flags;
    // The canvas containing the attached buffers that are used with the draw scope.
    avxCanvas           canv;
    // The area that is affected by the draw scope.
    afxLayeredRect      area;
    // The number of video surfaces.
    afxUnit             targetCnt;
    // Structures describing any color attachments used.
    avxDrawTarget       targets[AVX_MAX_COLOR_BUFFERS];
    // Structure describing a depth/stencil attachments.
    avxDrawTarget       ds[AVX_MAX_AUX_BUFFERS];

    // Debugging tag.
    afxString           tag;
};

AFX_DEFINE_STRUCT(avxStencilConfig)
// Structure specifying stencil operation state.
{
    // The comparison operator used in the stencil test.
    avxCompareOp        compareOp; // avxCompareOp_ALWAYS

    // An integer value that is used as stencil reference in the unsigned stencil comparison. 
    afxUnit32           reference; // 0

    // The bits of the unsigned integer stencil values participating in the stencil test.
    afxUnit32           compareMask; // 1

    // The bits of the unsigned integer stencil values updated by the stencil test in the stencil framebuffer attachment.
    afxUnit32           writeMask; // 1

    // A value specifying the action performed on samples that fail the stencil test.
    avxStencilOp        failOp; // avxStencilOp_KEEP

    // A value specifying the action performed on samples that pass the stencil test and fail the depth test.
    avxStencilOp        depthFailOp; // avxStencilOp_KEEP

    // A value specifying the action performed on samples that pass both the depth and stencil tests.
    avxStencilOp        passOp; // avxStencilOp_KEEP
};

/*
    Multisampling.
    he simplified pipeline placement with multisampling:

    Vertex Processing

    Clipping

    Rasterization
        This is where multisampling kicks in.
        Instead of generating a single fragment per pixel, the rasterizer produces
        multiple coverage samples per pixel (e.g. 2x, 4x, 8x, etc.).

    Early Fragment Tests (Stencil, Depth)
        These are often done per-sample (e.g. each MSAA sample gets its own depth/stencil check).

    Fragment Shader
        By default, it's run once per pixel, not per sample - unless you use per-sample shading.

    Multisample Resolve.
        After shading, the final color for the pixel is resolved from its samples and written to the framebuffer (or kept in MSAA buffers for later).
*/

AFX_DEFINE_STRUCT(avxMultisampling)
{
    // multisampling rasterization
    afxBool             msEnabled;
    // is a value specifying the number of samples used in rasterization. 
    afxUnit             sampleLvl; // 0
    // an array of sample mask values used in the sample mask test. 
    afxMask             sampleMasks[AVX_MAX_SAMPLE_MASKS]; // [ 1, ]

    // post fragment shader

    // controls whether the alpha component of the fragment's first color output is replaced with one.
    afxBool             alphaToOneEnabled; // FALSE
    // controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. 
    afxBool             alphaToCoverageEnabled; // FALSE
     // used to enable Sample Shading.
    afxBool             sampleShadingEnabled; // FALSE
    // specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE.
    afxReal             minSampleShadingValue; // 0.f
};

/*
    Clipping

    Clipping happens after vertex processing but before rasterization. Its job is to cut away parts of primitives
    (triangles, lines, etc.) that lie outside the view frustum - the region visible to the camera.
    Imagine trying to draw a triangle that sticks halfway outside the camera's view - clipping will
    slice it neatly at the edge and only send the visible portion onward.
*/

/*
    Depth clamping occurs during the rasterization stage of the graphics pipeline, specifically after vertex processing
    but before the depth test. It's a feature used in graphics APIs to handle geometry that would normally be clipped by
    the near and far planes of the view frustum.

    How it fits in the pipeline:
        Vertex Shader - Transforms vertex positions into clip space.
        Clipping (normally) - Geometry outside the near/far clip planes is removed.
        Depth Clamping (if enabled) – Instead of clipping geometry that goes beyond the near/far planes,
        the depth values are clamped to the [near, far] range.
        Rasterization - The geometry is converted into fragments (potential pixels).
        Fragment Shading - Color, lighting, etc.
        ...

    Depth clamping is used to avoid clipping artifacts, when it prevents visual popping when geometry crosses the near/far plane.
    It is useful for shadow maps or skyboxes, where you can have geometry extending infinitely (like a skybox or light volume) without getting clipped.
    It also avoids expensive clipping logic in the hardware pipeline.
*/

AFX_DEFINE_STRUCT(avxTransformation)
{
    // NOTE: the following members are ignored if transformationDisabled is TRUE.
    afxBool             transformationDisabled;

    // VERTEX SHADING. Positions go to clip space (x, y, z, w).

    avxVertexInput      vin;

    // PRIMITIVE ASSEMBLING. Triangles/lines formed from vertices.

    // The primitive topology.
    avxTopology         primTop; // avxTopology_TRI_LIST
    // Treat a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) as restarting the assembly of primitives.
    afxBool             primRestartEnabled; // FALSE

    // Enable the programmable primitive tesselation stages in pipeline.
    afxBool             tesselationEnabled; // FALSE
    // The number of control points per patch.
    afxUnit             patchCtrlPoints;

    // Enable the programmable primitive processing stage in pipeline.
    afxBool             primShaderSupported; // FALSE

    /*
        Clipping (Stage 3)
        Operates on clip space coordinates.
        Ensures primitives fit within the frustum.
        Cut primitives outside view frustum (in clip space).
        If a triangle crosses a boundary (e.g. near plane), it's split into new triangle(s) entirely inside the clip volume.
        This is before the perspective divide or viewport transformation.
    */

    // When depth clamp is enabled, the fragment's depth values will be clamped.
    afxBool             depthClampEnabled; // FALSE

    /*
        Face Culling (Stage 4)
        Happens after clipping but still before perspective divide.
        Uses winding order(clockwise / counter - clockwise) to decide what’s a "front" or "back" face.
        Culling removes primitives before rasterization, improving performance.
    */
    // The triangle facing direction used for primitive culling.
    avxCullMode         cullMode; // avxCullMode_BACK
    // When front facing is inverted, a triangle will be considered front-facing if its vertices are clockwise.
    afxBool             frontFacingInverted; // FALSE (CCW)

    /*
        Perspective divide then viewport Transform
        Converts from clip space to normalized device coords (NDC) then to window (screen) coords.
        If a primitive didn't survive clipping or culling, it never reaches this stage.
    */

    // The number of viewports used by the pipeline.
    afxUnit             vpCnt; // At least 1.
};

/*
    Rasterization disabling. When rasterization is disabled:

    The vertex processing still happens. Vertex shaders (and maybe geometry/tessellation shaders) run as usual.
    Primitive assembly and clipping still happens. The GPU assembles vertices into triangles or lines and applies view frustum clipping.
    Depth clamping still applies if clipping is enabled. But this becomes more of a formality if you're not rasterizing anything.
    Rasterization: Skipped. No fragments are generated. That means no fragment shading, depth test, color blending, or writing to the framebuffer.

    There are legit use cases for disabling rasterization.
    In transform feedback if you want to capture vertex data after it's been processed (e.g. post-vertex shader), without drawing anything.
    In mesh processing, doing compute-style work using the vertex pipeline (e.g. preparing LODs or doing physics on the GPU).
    In culling/visibility work, doing some advanced visibility algorithms use this to run through the vertex pipeline and discard geometry before rasterization - or simulate occlusion queries.
    In conditional rendering, sometimes used to simulate rendering for queries or counting primitives.
*/

/*
    Fragment Testing Order
    Stencil Test

    Checks the fragment against the current value in the stencil buffer using the stencil function and reference value.
    If the stencil test fails, the fragment is discarded immediately - no depth test or fragment shader runs.
    If it passes, move to the next step.

    Depth Test

    Compares the fragment's depth value to what's stored in the depth buffer (based on glDepthFunc() or equivalent).
    If it fails, the fragment can be discarded or update the stencil buffer (depending on how you've set stencil ops).

    Stencil and Depth Buffer Updates

    Depending on whether the stencil and/or depth test passed or failed, you can choose how to update the stencil buffer.
    Depth buffer is usually updated only if the depth test passes and depth writing is enabled.
*/

AFX_DEFINE_STRUCT(avxRasterization)
{
    // controls whether primitives are discarded immediately before the rasterization stage. 
    // If FALSE, all following fields in this structure are ignored.
    afxBool             rasterizationDisabled; // FALSE

    // is the triangle rendering mode. 
    avxFillMode         fillMode; // avxFillMode_FACE
    // is the width of rasterized line segments. 
    afxReal             lineWidth; // 1.f
    // depth bias computation
    // controls whether to bias fragment depth values. 
    afxBool             depthBiasEnabled; // FALSE
    // is a scalar factor applied to a fragment's slope in depth bias calculations. 
    afxReal             depthBiasSlopeScale; // 0.f
    // is a scalar factor controlling the constant depth value added to each fragment. 
    afxReal             depthBiasConstFactor; // 0.f
    // is the maximum (or minimum) depth bias of a fragment. 
    afxReal             depthBiasClamp; // 0.f

    avxMultisampling    ms;

    // Fragment testing

    // stencil test
    afxBool             stencilTestEnabled;
    // is the configuration values controlling the corresponding parameters of the stencil test.
    avxStencilConfig    stencilFront;
    // is the configuration controlling the corresponding parameters of the stencil test.
    avxStencilConfig    stencilBack;

    // depth test
    // controls whether depth testing is enabled.
    afxBool             depthTestEnabled; // FALSE
    // is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. 
    avxCompareOp        depthCompareOp; // avxCompareOp_LESS
    // controls whether depth writes are enabled when depthTestEnabled is TRUE. 
    // Depth writes are always disabled when depthTestEnabled is FALSE.
    afxBool             depthWriteDisabled; // FALSE
    // is the format of depth/stencil surface this pipeline will be compatible with.
    avxFormat           dsFmt;
    // depth bounds test
    // controls whether depth bounds testing is enabled.
    afxBool             depthBoundsTestEnabled;
    // is the minimum depth bound used in the depth bounds test.
    afxV2d              depthBounds; // [ min, max ]

    // Fragment & pixel output operations
    // color bending, logical op and color writing
    afxUnit             colorOutCnt;
    avxColorOutput      colorOuts[AVX_MAX_COLOR_OUTPUTS];
    avxColor            blendConstants;
    afxBool             pixelLogicOpEnabled;
    avxLogicOp          pixelLogicOp;
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
    The AvxCmdClearCanvas() operations clears a number of areas in every canvas' annex listed.
*/

AVX afxCmdId AvxCmdClearCanvas
(
    afxDrawContext      dctx,
    // The number of annexes to clear.
    afxUnit             bufCnt,
    // An array of indices to each annex to be cleared.
    // If NIL, it will index sequentially up to @annexCnt.
    afxUnit const       bins[],
    // An array of clear values, one for each annex.
    avxClearValue const values[],
    // The number of areas to be cleared in every annex.
    afxUnit             areaCnt, 
    // An array of areas to be cleared in every annex.
    afxLayeredRect const areas[]
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
