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

#define _SGL_DBG_IGNORE_RASTERIZER_DISCARD
#define _SGL_DBG_IGNORE_DEPTH_BIAS
 //#define _SGL_DBG_IGNORE_DEPTH_TEST
 //#define _SGL_DBG_IGNORE_DEPTH_WRITE
#define _SGL_DBG_IGNORE_DEPTH_BOUNDS_TEST
#define _SGL_DBG_IGNORE_LINE_WIDTH
#define _SGL_DBG_IGNORE_STENCIL_TEST
#define _SGL_DBG_IGNORE_MULTISAMPLING
#define _SGL_DBG_IGNORE_BLEND
#define _SGL_DBG_IGNORE_LOGICAL_OP
#define _SGL_DBG_IGNORE_COLOR_MASK
#define _SGL_DBG_IGNORE_SCISSOR_TEST

#include "sgl.h"
#include "qwadro/afxQwadro.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"

_SGL void _SglDpuDisableRasterization(sglDpuIdd* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.rasterizationDisabled = cmd->value;
}

_SGL void _SglDpuEnableDepthBias(sglDpuIdd* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.depthBiasEnabled = cmd->value;
}

_SGL void _SglDpuSetDepthBias(sglDpuIdd* dpu, _sglCmdReal3 const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.depthBiasConstFactor = cmd->value[0];
    dpu->nextRasterState.depthBiasClamp = cmd->value[1];
    dpu->nextRasterState.depthBiasSlopeScale = cmd->value[2];
}

_SGL void _SglDpuSetLineWidth(sglDpuIdd* dpu, _sglCmdReal const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.lineWidth = cmd->value;
}

_SGL void _SglDpuEnableStencilTest(sglDpuIdd* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.stencilTestEnabled = cmd->value;
}

_SGL void _SglDpuSetStencilCompareMask(sglDpuIdd* dpu, _sglCmdBitmaskNat32 const *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertRange((AfxGetBitOffset(0) | AfxGetBitOffset(1)), AfxGetBitOffset(0), cmd->mask);

    if (cmd->mask & AfxGetBitOffset(0))
        dpu->nextRasterState.stencilFront.compareMask = cmd->value;

    if (cmd->mask & AfxGetBitOffset(1))
        dpu->nextRasterState.stencilBack.compareMask = cmd->value;
}

_SGL void _SglDpuSetStencilWriteMask(sglDpuIdd* dpu, _sglCmdBitmaskNat32 const *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertRange((AfxGetBitOffset(0) | AfxGetBitOffset(1)), AfxGetBitOffset(0), cmd->mask);

    if (cmd->mask & AfxGetBitOffset(0))
        dpu->nextRasterState.stencilFront.writeMask = cmd->value;

    if (cmd->mask & AfxGetBitOffset(1))
        dpu->nextRasterState.stencilBack.writeMask = cmd->value;
}

_SGL void _SglDpuSetStencilReference(sglDpuIdd* dpu, _sglCmdBitmaskNat32 const *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertRange((AfxGetBitOffset(0) | AfxGetBitOffset(1)), AfxGetBitOffset(0), cmd->mask);

    if (cmd->mask & AfxGetBitOffset(0))
        dpu->nextRasterState.stencilFront.reference = cmd->value;

    if (cmd->mask & AfxGetBitOffset(1))
        dpu->nextRasterState.stencilBack.reference = cmd->value;
}

_SGL void _SglDpuEnableDepthTest(sglDpuIdd* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.depthTestEnabled = cmd->value;
}

_SGL void _SglDpuSetDepthCompareOp(sglDpuIdd* dpu, _sglCmdNat const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.depthCompareOp = cmd->value;
}

_SGL void _SglDpuDisableDepthWrite(sglDpuIdd* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.depthWriteDisabled = cmd->value;
}

_SGL void _SglDpuSetBlendConstants(sglDpuIdd* dpu, _sglCmdReal4 const *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxCopyV4d(dpu->nextRasterState.blendConstants, cmd->value);
    dpu->nextBlendConstUpd = TRUE;
}

_SGL void _SglDpuSetScissors(sglDpuIdd* dpu, _sglCmdScissor const *cmd)
{
    afxError err = AFX_ERR_NONE;
    afxNat first = cmd->first;
    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VIEWPORTS, first, cnt);

    for (afxNat i = 0; i < cnt; i++)
        dpu->nextRasterState.scisRects[first + i] = cmd->rect[i], dpu->nextScissorUpdMask |= AfxGetBitOffset(first + i);

    dpu->nextScissorUpdCnt = AfxMax(dpu->nextScissorUpdCnt, cnt);

    if (cmd->reset)
        dpu->nextRasterState.scisCnt = cnt;
}

_SGL void _SglDpuSetAreas(sglDpuIdd* dpu, _sglCmdArea const *cmd)
{
    afxError err = AFX_ERR_NONE;
    afxNat first = cmd->first;
    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VIEWPORTS, first, cnt);

    for (afxNat i = 0; i < cnt; i++)
        dpu->nextRasterState.areaRects[first + i] = cmd->rect[i], dpu->nextAreaUpdMask |= AfxGetBitOffset(first + i);

    dpu->nextAreaUpdCnt = AfxMax(dpu->nextAreaUpdCnt, cnt);

    if (cmd->reset)
        dpu->nextRasterState.areaCnt = cnt;
}

_SGL void SglFlushRasterizationStateChanges(sglDpuIdd* dpu)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

#ifndef _SGL_DBG_IGNORE_RASTERIZER_DISCARD

    afxBool rasterizationDisabled = dpu->nextRasterState.rasterizationDisabled;

    if (dpu->activeRasterState.rasterizationDisabled != rasterizationDisabled)
    {
        /*
            GL_RASTERIZER_DISCARD
            If enabled, primitives are discarded after the optional transform feedback stage, but before rasterization.
            Furthermore, when enabled, glClear, glClearBufferData, glClearBufferSubData, glClearTexImage, and glClearTexSubImage are ignored.
        */

        if (rasterizationDisabled)
        {
            gl->Enable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
        }
        dpu->activeRasterState.rasterizationDisabled = rasterizationDisabled;
    }
#endif

    afxFillMode fillMode = dpu->nextRasterState.fillMode;

    if (dpu->activeRasterState.fillMode != fillMode)
    {
        /*
            glPolygonMode — select a polygon rasterization mode.
            void glPolygonMode(	GLenum face, GLenum mode);

            face = Specifies the polygons that mode applies to. Must be GL_FRONT_AND_BACK for front- and back-facing polygons.
            mode = Specifies how polygons will be rasterized. Accepted values are GL_POINT, GL_LINE, and GL_FILL. The initial value is GL_FILL for both front- and back-facing polygons.

            glPolygonMode controls the interpretation of polygons for rasterization.
            face describes which polygons mode applies to: both front and back-facing polygons (GL_FRONT_AND_BACK).
            The polygon mode affects only the final rasterization of polygons.
            In particular, a polygon's vertices are lit and the polygon is clipped and possibly culled before these modes are applied.

            Three modes are defined and can be specified in mode:

            GL_POINT Polygon vertices that are marked as the start of a boundary edge are drawn as points. Point attributes such as GL_POINT_SIZE and GL_POINT_SMOOTH control the rasterization of the points. Polygon rasterization attributes other than GL_POLYGON_MODE have no effect.
            GL_LINE Boundary edges of the polygon are drawn as line segments. Line attributes such as GL_LINE_WIDTH and GL_LINE_SMOOTH control the rasterization of the lines. Polygon rasterization attributes other than GL_POLYGON_MODE have no effect.
            GL_FILL The interior of the polygon is filled. Polygon attributes such as GL_POLYGON_SMOOTH control the rasterization of the polygon.

            Vertices are marked as boundary or nonboundary with an edge flag. Edge flags are generated internally by the GL when it decomposes triangle stips and fans.
        */
        gl->PolygonMode(GL_FRONT_AND_BACK, AfxToGlFillMode(fillMode)); _SglThrowErrorOccuried();
        dpu->activeRasterState.fillMode = fillMode;
    }

    // DEPTH BIAS
#ifndef _SGL_DBG_IGNORE_DEPTH_BIAS

    afxBool depthBiasEnabled = dpu->nextRasterState.depthBiasEnabled;

    if (dpu->activeRasterState.depthBiasEnabled != depthBiasEnabled)
    {
        /*
            GL_POLYGON_OFFSET_FILL If enabled, and if the polygon is rendered in GL_FILL mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed.
            See glPolygonOffset.

            GL_POLYGON_OFFSET_LINE If enabled, and if the polygon is rendered in GL_LINE mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed.
            See glPolygonOffset.

            GL_POLYGON_OFFSET_POINT If enabled, an offset is added to depth values of a polygon's fragments before the depth comparison is performed, if the polygon is rendered in GL_POINT mode.
            See glPolygonOffset.
        */

        static int const QwadroToGlPolygonModeBasedOffset[] =
        {
            GL_POLYGON_OFFSET_FILL,
            GL_POLYGON_OFFSET_LINE,
            GL_POLYGON_OFFSET_POINT
        };

        if (depthBiasEnabled)
        {
            gl->Enable(QwadroToGlPolygonModeBasedOffset[dpu->activeRasterState.fillMode]); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(QwadroToGlPolygonModeBasedOffset[dpu->activeRasterState.fillMode]); _SglThrowErrorOccuried();
        }
        dpu->activeRasterState.depthBiasEnabled = depthBiasEnabled;
    }

    if (
        (dpu->activeRasterState.depthBiasConstFactor != dpu->nextRasterState.depthBiasConstFactor) ||
        (dpu->activeRasterState.depthBiasSlopeScale != dpu->nextRasterState.depthBiasSlopeScale) ||
        (dpu->activeRasterState.depthBiasClamp != dpu->nextRasterState.depthBiasClamp)
        )
    {
        /*
            glPolygonOffset — set the scale and units used to calculate depth values
            void glPolygonOffset(GLfloat factor, GLfloat units);

            factor Specifies a scale factor that is used to create a variable depth offset for each polygon. The initial value is 0.
            units Is multiplied by an implementation-specific value to create a constant depth offset. The initial value is 0.

            When GL_POLYGON_OFFSET_FILL, GL_POLYGON_OFFSET_LINE, or GL_POLYGON_OFFSET_POINT is enabled, each fragment's depth value will be offset after it is interpolated from the depth values of the appropriate vertices.
            The value of the offset is factor×DZ+r×units, where DZ is a measurement of the change in depth relative to the screen area of the polygon, and r is the smallest value that is guaranteed to produce a resolvable offset for a given implementation.
            The offset is added before the depth test is performed and before the value is written into the depth buffer.

            glPolygonOffset is useful for rendering hidden-line images, for applying decals to surfaces, and for rendering solids with highlighted edges.
        */

        if (gl->PolygonOffsetClamp)
        {
            gl->PolygonOffsetClamp(dpu->nextRasterState.depthBiasSlopeScale, dpu->nextRasterState.depthBiasConstFactor, dpu->nextRasterState.depthBiasClamp); _SglThrowErrorOccuried();
        }
        else
        {
            gl->PolygonOffset(dpu->nextRasterState.depthBiasSlopeScale, dpu->nextRasterState.depthBiasConstFactor); _SglThrowErrorOccuried();
        }

        dpu->activeRasterState.depthBiasConstFactor != dpu->nextRasterState.depthBiasConstFactor;
        dpu->activeRasterState.depthBiasSlopeScale != dpu->nextRasterState.depthBiasSlopeScale;
        dpu->activeRasterState.depthBiasClamp != dpu->nextRasterState.depthBiasClamp;
    }
#endif

#ifndef _SGL_DBG_IGNORE_LINE_WIDTH
    /*
        glLineWidth — specify the width of rasterized lines.
        void glLineWidth(GLfloat width);

        width = Specifies the width of rasterized lines. The initial value is 1.

        glLineWidth specifies the rasterized width of both aliased and antialiased lines.
        Using a line width other than 1 has different effects, depending on whether line antialiasing is enabled.
        To enable and disable line antialiasing, call glEnable and glDisable with argument GL_LINE_SMOOTH.
        Line antialiasing is initially disabled.

        If line antialiasing is disabled, the actual width is determined by rounding the supplied width to the nearest integer.
        (If the rounding results in the value 0, it is as if the line width were 1.) If |delta x|>=|delta y|, i pixels are filled in each column that is rasterized, where i is the rounded value of width.
        Otherwise, i pixels are filled in each row that is rasterized.

        If antialiasing is enabled, line rasterization produces a fragment for each pixel square that intersects the region lying within the rectangle having width equal to the current line width, length equal to the actual length of the line, and centered on the mathematical line segment.
        The coverage value for each fragment is the window coordinate area of the intersection of the rectangular region with the corresponding pixel square.
        This value is saved and used in the final rasterization step.

        Not all widths can be supported when line antialiasing is enabled.
        If an unsupported width is requested, the nearest supported width is used.
        Only width 1 is guaranteed to be supported; others depend on the implementation.
        Likewise, there is a range for aliased line widths as well.
        To query the range of supported widths and the size difference between supported widths within the range, call glGet with arguments GL_ALIASED_LINE_WIDTH_RANGE, GL_SMOOTH_LINE_WIDTH_RANGE, and GL_SMOOTH_LINE_WIDTH_GRANULARITY.

        The line width specified by glLineWidth is always returned when GL_LINE_WIDTH is queried.
        Clamping and rounding for aliased and antialiased lines have no effect on the specified value.

        Nonantialiased line width may be clamped to an implementation-dependent maximum.
        Call glGet with GL_ALIASED_LINE_WIDTH_RANGE to determine the maximum width.

        In OpenGL 1.2, the tokens GL_LINE_WIDTH_RANGE and GL_LINE_WIDTH_GRANULARITY were replaced by GL_ALIASED_LINE_WIDTH_RANGE, GL_SMOOTH_LINE_WIDTH_RANGE, and GL_SMOOTH_LINE_WIDTH_GRANULARITY.
        The old names are retained for backward compatibility, but should not be used in new code.
    */

    afxReal lineWidth = dpu->nextRasterState.lineWidth;

    if (dpu->activeRasterState.lineWidth != lineWidth)
    {
        gl->LineWidth(lineWidth); _SglThrowErrorOccuried();
        dpu->activeRasterState.lineWidth = lineWidth;
    }
#endif

    // DEPTH TEST
#ifndef _SGL_DBG_IGNORE_DEPTH_TEST

    afxBool depthTestEnabled = dpu->nextRasterState.depthTestEnabled;

    if (dpu->activeRasterState.depthTestEnabled != depthTestEnabled)
    {
        /*
            GL_DEPTH_TEST
            If enabled, do depth comparisons and update the depth buffer.
            Note that even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
            See glDepthFunc and glDepthRange.
        */

        if (depthTestEnabled)
        {
            AfxAssert(!dpu->activeRasterState.depthTestEnabled);
            gl->Enable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeRasterState.depthTestEnabled);
            gl->Disable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
        }
        dpu->activeRasterState.depthTestEnabled = depthTestEnabled;
    }

    if (dpu->activeRasterState.depthTestEnabled)
    {
        afxCompareOp depthCompareOp = dpu->nextRasterState.depthCompareOp;

        if (dpu->activeRasterState.depthCompareOp != depthCompareOp)
        {
            /*
                glDepthFunc — specify the value used for depth buffer comparisons
                void glDepthFunc(GLenum func);

                func = Specifies the depth comparison function. Symbolic constants GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, and GL_ALWAYS are accepted. The initial value is GL_LESS.

                glDepthFunc specifies the function used to compare each incoming pixel depth value with the depth value present in the depth buffer.
                The comparison is performed only if depth testing is enabled. (See glEnable and glDisable of GL_DEPTH_TEST.)

                func specifies the conditions under which the pixel will be drawn. The comparison functions are as follows:

                GL_NEVER Never passes.
                GL_LESS Passes if the incoming depth value is less than the stored depth value.
                GL_EQUAL Passes if the incoming depth value is equal to the stored depth value.
                GL_LEQUAL Passes if the incoming depth value is less than or equal to the stored depth value.
                GL_GREATER Passes if the incoming depth value is greater than the stored depth value.
                GL_NOTEQUAL Passes if the incoming depth value is not equal to the stored depth value.
                GL_GEQUAL Passes if the incoming depth value is greater than or equal to the stored depth value.
                GL_ALWAYS Always passes.

                The initial value of func is GL_LESS. Initially, depth testing is disabled. If depth testing is disabled or if no depth buffer exists, it is as if the depth test always passes.

                Even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
                In order to unconditionally write to the depth buffer, the depth test should be enabled and set to GL_ALWAYS.
            */

            AfxAssert(depthCompareOp < afxCompareOp_TOTAL);
            gl->DepthFunc(SglToGlCompareOp(depthCompareOp)); _SglThrowErrorOccuried();
            dpu->activeRasterState.depthCompareOp = depthCompareOp;
        }
    }
#endif

#ifndef _SGL_DBG_IGNORE_DEPTH_WRITE
    afxBool depthWriteDisabled = dpu->nextRasterState.depthWriteDisabled;

    if (dpu->activeRasterState.depthWriteDisabled != depthWriteDisabled)
    {
        /*
            glDepthMask — enable or disable writing into the depth buffer.
            void glDepthMask(GLboolean flag);

            flag = Specifies whether the depth buffer is enabled for writing. If flag is GL_FALSE, depth buffer writing is disabled. Otherwise, it is enabled. Initially, depth buffer writing is enabled.

            glDepthMask specifies whether the depth buffer is enabled for writing. If flag is GL_FALSE, depth buffer writing is disabled. Otherwise, it is enabled. Initially, depth buffer writing is enabled.

            Even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
            In order to unconditionally write to the depth buffer, the depth test should be enabled and set to GL_ALWAYS (see glDepthFunc).
        */

        if (depthWriteDisabled)
        {
            AfxAssert(dpu->activeRasterState.depthWriteDisabled);
            gl->DepthMask(GL_FALSE); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(!dpu->activeRasterState.depthWriteDisabled);
            gl->DepthMask(GL_TRUE); _SglThrowErrorOccuried();            
        }
        dpu->activeRasterState.depthWriteDisabled = depthWriteDisabled;
    }
#endif

#ifndef _SGL_DBG_IGNORE_DEPTH_BOUNDS_TEST

    afxBool depthBoundsTestEnabled = dpu->nextRasterState.depthBoundsTestEnabled;

    if (dpu->activeRasterState.depthBoundsTestEnabled != depthBoundsTestEnabled)
    {
        /*
            EXT_depth_bounds_test

            The depth bounds test determines whether the depth value (Zpixel) stored at the location given by the incoming fragment's (xw,yw) location lies within the depth bounds range defined by two values.
            These values are set with

            void DepthBoundsEXT(clampd zmin, clampd zmax);

            Each of zmin and zmax are clamped to lie within [0,1] (being of type clampd).  If zmin <= Zpixel <= zmax, then the depth bounds test passes.  Otherwise, the test fails and the fragment is discarded.
            The test is enabled or disabled using Enable or Disable using the constant DEPTH_BOUNDS_TEST_EXT.  When disabled, it is as if the depth bounds test always passes.  If zmin is greater than zmax, then the error INVALID_VALUE is generated.
            The state required consists of two floating-point values and a bit indicating whether the test is enabled or disabled.  In the initial state, zmin and zmax are set to 0.0 and 1.0 respectively; and the depth bounds test is disabled.

            If there is no depth buffer, it is as if the depth bounds test always passes.
        */

        if (depthBoundsTestEnabled)
        {
            gl->Enable(DEPTH_BOUNDS_TEST_EXT); _SglThrowErrorOccuried();
            gl->DepthBoundsEXT(dpu->state.depthBounds[0], dpu->state.depthBounds[1]); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(DEPTH_BOUNDS_TEST_EXT); _SglThrowErrorOccuried();
        }
    }
#endif

    // STENCIL TEST

#ifndef _SGL_DBG_IGNORE_STENCIL_TEST

    afxBool stencilTestEnabled = dpu->nextRasterState.stencilTestEnabled;

    if (dpu->activeRasterState.stencilTestEnabled != stencilTestEnabled)
    {
        /*
            GL_STENCIL_TEST
            If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp.
        */

        if (stencilTestEnabled)
        {
            AfxAssert(!dpu->activeRasterState.stencilTestEnabled);
            gl->Enable(GL_STENCIL_TEST); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeRasterState.stencilTestEnabled);
            gl->Disable(GL_STENCIL_TEST); _SglThrowErrorOccuried();
        }
        dpu->activeRasterState.stencilTestEnabled = stencilTestEnabled;
    }

    // STENCIL FUNC

    {
        /*
            glStencilFuncSeparate — set front and/or back function and reference value for stencil testing
            void glStencilFuncSeparate(	GLenum face, GLenum func, GLint ref, GLuint mask);

            face = Specifies whether front and/or back stencil state is updated. Three symbolic constants are valid: GL_FRONT, GL_BACK, and GL_FRONT_AND_BACK.
            func = Specifies the test function. Eight symbolic constants are valid: GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL, and GL_ALWAYS. The initial value is GL_ALWAYS.
            ref = Specifies the reference value for the stencil test. ref is clamped to the range [0, 2^n - 1], where n is the number of bitplanes in the stencil buffer. The initial value is 0.
            mask = Specifies a mask that is ANDed with both the reference value and the stored stencil value when the test is done. The initial value is all 1's.

            Stenciling, like depth-buffering, enables and disables drawing on a per-pixel basis.
            You draw into the stencil planes using GL drawing primitives, then render geometry and images, using the stencil planes to mask out portions of the screen.
            Stenciling is typically used in multipass rendering algorithms to achieve special effects, such as decals, outlining, and constructive solid geometry rendering.

            The stencil test conditionally eliminates a pixel based on the outcome of a comparison between the reference value and the value in the stencil buffer.
            To enable and disable the test, call glEnable and glDisable with argument GL_STENCIL_TEST.
            To specify actions based on the outcome of the stencil test, call glStencilOp or glStencilOpSeparate.

            There can be two separate sets of func, ref, and mask parameters; one affects back-facing polygons, and the other affects front-facing polygons as well as other non-polygon primitives.
            glStencilFunc sets both front and back stencil state to the same values, as if glStencilFuncSeparate were called with face set to GL_FRONT_AND_BACK.

            func is a symbolic constant that determines the stencil comparison function. It accepts one of eight values, shown in the following list.
            ref is an integer reference value that is used in the stencil comparison. It is clamped to the range [0, 2^n - 1], where n is the number of bitplanes in the stencil buffer.
            mask is bitwise ANDed with both the reference value and the stored stencil value, with the ANDed values participating in the comparison.

            If stencil represents the value stored in the corresponding stencil buffer location, the following list shows the effect of each comparison function that can be specified by func.
            Only if the comparison succeeds is the pixel passed through to the next stage in the rasterization process (see glStencilOp).
            All tests treat stencil values as unsigned integers in the range [0,2^n - 1], where n is the number of bitplanes in the stencil buffer.

            The following values are accepted by func:

            GL_NEVER Always fails.
            GL_LESS Passes if ( ref & mask ) < ( stencil & mask ).
            GL_LEQUAL Passes if ( ref & mask ) <= ( stencil & mask ).
            GL_GREATER Passes if ( ref & mask ) > ( stencil & mask ).
            GL_GEQUAL Passes if ( ref & mask ) >= ( stencil & mask ).
            GL_EQUAL Passes if ( ref & mask ) = ( stencil & mask ).
            GL_NOTEQUAL Passes if ( ref & mask ) != ( stencil & mask ).
            GL_ALWAYS Always passes.

            Initially, the stencil test is disabled.
            If there is no stencil buffer, no stencil modification can occur and it is as if the stencil test always passes.
        */

        afxNat32 compareMask = dpu->nextRasterState.stencilFront.compareMask;
        afxCompareOp compareOp = dpu->nextRasterState.stencilFront.compareOp;
        afxNat32 reference = dpu->nextRasterState.stencilFront.reference;

        if (
            (dpu->activeRasterState.stencilFront.compareMask != compareMask) ||
            (dpu->activeRasterState.stencilFront.compareOp != compareOp) ||
            (dpu->activeRasterState.stencilFront.reference != reference)
            )
        {
            gl->StencilFuncSeparate(GL_FRONT, SglToGlCompareOp(compareOp), reference, compareMask); _SglThrowErrorOccuried();

            dpu->activeRasterState.stencilFront.compareMask = compareMask;
            dpu->activeRasterState.stencilFront.compareOp = compareOp;
            dpu->activeRasterState.stencilFront.reference = reference;
        }

        compareMask = dpu->nextRasterState.stencilBack.compareMask;
        compareOp = dpu->nextRasterState.stencilBack.compareOp;
        reference = dpu->nextRasterState.stencilBack.reference;

        if (
            (dpu->activeRasterState.stencilBack.compareMask != compareMask) ||
            (dpu->activeRasterState.stencilBack.compareOp != compareOp) ||
            (dpu->activeRasterState.stencilBack.reference != reference)
            )
        {
            gl->StencilFuncSeparate(GL_BACK, SglToGlCompareOp(compareOp), reference, compareMask); _SglThrowErrorOccuried();

            dpu->activeRasterState.stencilBack.compareMask = compareMask;
            dpu->activeRasterState.stencilBack.compareOp = compareOp;
            dpu->activeRasterState.stencilBack.reference = reference;
        }
    }

    // STENCIL WRITE

    {
        /*
            glStencilMaskSeparate — control the front and/or back writing of individual bits in the stencil planes.
            void glStencilMaskSeparate(	GLenum face, GLuint mask);

            face = Specifies whether the front and/or back stencil writemask is updated. Three symbolic constants are valid: GL_FRONT, GL_BACK, and GL_FRONT_AND_BACK.
            mask = Specifies a bit mask to enable and disable writing of individual bits in the stencil planes. Initially, the mask is all 1's.

            glStencilMaskSeparate controls the writing of individual bits in the stencil planes.
            The least significant n bits of mask, where n is the number of bits in the stencil buffer, specify a mask.
            Where a 1 appears in the mask, it's possible to write to the corresponding bit in the stencil buffer.
            Where a 0 appears, the corresponding bit is write-protected.
            Initially, all bits are enabled for writing.

            There can be two separate mask writemasks; one affects back-facing polygons, and the other affects front-facing polygons as well as other non-polygon primitives.
            glStencilMask sets both front and back stencil writemasks to the same values, as if glStencilMaskSeparate were called with face set to GL_FRONT_AND_BACK.
        */

        static GLenum const faces[] =
        {
            GL_INVALID_ENUM,
            GL_FRONT,
            GL_BACK,
            GL_FRONT_AND_BACK
        };

        afxMask facesAffected;
        afxNat32 writeMask = dpu->nextRasterState.stencilFront.writeMask;

        if (dpu->activeRasterState.stencilFront.writeMask != writeMask)
        {
            gl->StencilMaskSeparate(GL_FRONT, writeMask); _SglThrowErrorOccuried();
            dpu->activeRasterState.stencilFront.writeMask = writeMask;
        }

        writeMask = dpu->nextRasterState.stencilBack.writeMask;

        if (dpu->activeRasterState.stencilBack.writeMask != writeMask)
        {
            gl->StencilMaskSeparate(GL_BACK, writeMask); _SglThrowErrorOccuried();
            dpu->activeRasterState.stencilBack.writeMask = writeMask;
        }
    }
#endif


#ifndef _SGL_DBG_IGNORE_MULTISAMPLING

    afxBool msEnabled = dpu->nextRasterState.msEnabled;

    if (dpu->activeRasterState.msEnabled != msEnabled)
    {
        /*
            GL_MULTISAMPLE
            If enabled, use multiple fragment samples in computing the final color of a pixel. See glSampleCoverage.
        */

        if (msEnabled)
        {
            AfxAssert(!dpu->activeRasterState.msEnabled);
            gl->Enable(GL_MULTISAMPLE); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeRasterState.msEnabled);
            gl->Disable(GL_MULTISAMPLE); _SglThrowErrorOccuried();

        }
        dpu->activeRasterState.msEnabled = msEnabled;
    }

    if (msEnabled)
    {
        afxNat sampleCnt = dpu->nextRasterState.sampleCnt;

        if (dpu->activeRasterState.sampleCnt != sampleCnt)
        {
            /*
                GL_SAMPLE_MASK
                If enabled, the sample coverage mask generated for a fragment during rasterization will be ANDed with the value of GL_SAMPLE_MASK_VALUE before shading occurs. See glSampleMaski.
            */

            if (sampleCnt)
            {
                AfxAssert(!dpu->activeRasterState.sampleCnt);
                gl->Enable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();

                for (afxNat i = 0; i < sampleCnt; i++)
                {
                    dpu->activeRasterState.sampleMasks[i] = dpu->nextRasterState.sampleMasks[i];
                    gl->SampleMaski(i, dpu->nextRasterState.sampleMasks[i]); _SglThrowErrorOccuried();
                }
            }
            else
            {
                AfxAssert(dpu->activeRasterState.sampleCnt);
                gl->Disable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();
            }
            dpu->activeRasterState.sampleCnt = sampleCnt;
        }

        afxReal minSampleShadingValue = dpu->nextRasterState.minSampleShadingValue;
        afxBool sampleShadingEnabled = dpu->nextRasterState.sampleShadingEnabled;

        if (sampleShadingEnabled != dpu->activeRasterState.sampleShadingEnabled)
        {
            /*
                GL_SAMPLE_SHADING
                If enabled, the active fragment shader is run once for each covered sample, or at fraction of this rate as determined by the current value of GL_MIN_SAMPLE_SHADING_VALUE. See glMinSampleShading.
            */

            if (sampleShadingEnabled)
            {
                AfxAssert(dpu->activeRasterState.sampleShadingEnabled);
                gl->Enable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
            }
            else
            {
                AfxAssert(!dpu->activeRasterState.sampleShadingEnabled);
                gl->Disable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
            }

            if (gl->MinSampleShading) // gl v4.0
            {
                gl->MinSampleShading(minSampleShadingValue); _SglThrowErrorOccuried();
            }

            dpu->activeRasterState.sampleShadingEnabled = sampleShadingEnabled;
            dpu->activeRasterState.minSampleShadingValue = minSampleShadingValue;
        }

        afxBool alphaToOneEnabled = dpu->nextRasterState.alphaToOneEnabled;

        if (dpu->activeRasterState.alphaToOneEnabled != alphaToOneEnabled)
        {
            /*
                GL_SAMPLE_ALPHA_TO_ONE
                If enabled, each sample alpha value is replaced by the maximum representable alpha value.
            */

            if (alphaToOneEnabled)
            {
                AfxAssert(!dpu->activeRasterState.alphaToOneEnabled);
                gl->Enable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
            }
            else
            {
                AfxAssert(dpu->activeRasterState.alphaToOneEnabled);
                gl->Disable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
            }
            dpu->activeRasterState.alphaToOneEnabled = alphaToOneEnabled;
        }

        afxBool alphaToCoverageEnabled = dpu->nextRasterState.alphaToCoverageEnabled;

        if (dpu->activeRasterState.alphaToCoverageEnabled != alphaToCoverageEnabled)
        {
            /*
                GL_SAMPLE_ALPHA_TO_COVERAGE
                If enabled, compute a temporary coverage value where each bit is determined by the alpha value at the corresponding sample location.
                The temporary coverage value is then ANDed with the fragment coverage value.
            */

            if (alphaToCoverageEnabled)
            {
                AfxAssert(!dpu->activeRasterState.alphaToCoverageEnabled);
                gl->Enable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
            }
            else
            {
                AfxAssert(dpu->activeRasterState.alphaToCoverageEnabled);
                gl->Disable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
            }
            dpu->activeRasterState.alphaToCoverageEnabled = alphaToCoverageEnabled;
        }

    }
#endif

    // SCISSOR
#ifndef _SGL_DBG_IGNORE_SCISSOR_TEST

    if (dpu->nextScissorUpdMask)
    {
        afxNat cnt = dpu->nextScissorUpdCnt;
        afxBool enabled = dpu->scissorTestEnabled;

        if (!cnt)
        {
            if (enabled)
            {
                gl->Disable(GL_SCISSOR_TEST); _SglThrowErrorOccuried();
                dpu->scissorTestEnabled = TRUE;
            }
        }
        else
        {
            if (!enabled)
            {
                gl->Enable(GL_SCISSOR_TEST); _SglThrowErrorOccuried();
                dpu->scissorTestEnabled = TRUE;
            }

            afxMask updMask = dpu->nextScissorUpdMask;

            for (afxNat i = 0; i < SGL_MAX_VIEWPORTS; i++)
            {
                AfxAssertRange(SGL_MAX_VIEWPORTS, i, 1);

                if (AfxTestBitEnabled(updMask, i))
                {
                    if (gl->ScissorArrayv)
                    {
                        GLint v[SGL_MAX_VIEWPORTS][4];
                        AfxAssert(SGL_MAX_VIEWPORTS >= cnt);
                        v[0][0] = dpu->nextRasterState.scisRects[i].offset[0],
                            v[0][1] = dpu->nextRasterState.scisRects[i].offset[1],
                            v[0][2] = dpu->nextRasterState.scisRects[i].extent[0],
                            v[0][3] = dpu->nextRasterState.scisRects[i].extent[1];

                        gl->ScissorArrayv(i, 1, &v[0][0]); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        GLint x = (GLint)(dpu->nextRasterState.scisRects[0].offset[0]);
                        GLint y = (GLint)(dpu->nextRasterState.scisRects[0].offset[1]);
                        GLsizei w = (GLsizei)(dpu->nextRasterState.scisRects[0].extent[0]);
                        GLsizei h = (GLsizei)(dpu->nextRasterState.scisRects[0].extent[1]);

                        AfxAssert(i == 0);
                        gl->Scissor(x, y, w, h); _SglThrowErrorOccuried();
                    }
                    AfxRectCopy(&(dpu->activeRasterState.scisRects[i]), &dpu->nextRasterState.scisRects[i]);
                }
            }
        }
        dpu->nextScissorUpdMask = NIL;
    }

#endif

    // BLEND CONSTANT
#ifndef _SGL_DBG_IGNORE_BLEND

    if (dpu->nextBlendConstUpd)
    {
        gl->BlendColor(dpu->activeRasterState.blendConstants[0], dpu->activeRasterState.blendConstants[1], dpu->activeRasterState.blendConstants[2], dpu->activeRasterState.blendConstants[3]);
        AfxCopyColor(dpu->activeRasterState.blendConstants, dpu->nextRasterState.blendConstants);
        dpu->nextBlendConstUpd = NIL;
    }
#endif

    afxBool blendNoUsed = TRUE;

    afxNat outCnt = dpu->nextRasterState.outCnt;

    for (afxNat i = 0; i < outCnt; i++)
    {
        afxColorOutputChannel const*ch = &dpu->nextRasterState.outs[i];
        //AfxGetColorOutputChannels(ras, 0, outCnt, ch);

#ifndef _SGL_DBG_IGNORE_BLEND
        if (ch->blendEnabled)
        {
            if (ch->blendEnabled != dpu->activeRasterState.outs[i].blendEnabled)
            {
                if (ch->blendEnabled)
                {
                    if (blendNoUsed)
                    {
                        blendNoUsed = FALSE;

                        /*
                            GL_BLEND
                            If enabled, blend the computed fragment color values with the values in the color buffers. See glBlendFunc.
                        */

                        if (!dpu->activeRasterState.anyBlendEnabled)
                        {
                            gl->Enable(GL_BLEND); _SglThrowErrorOccuried();
                            dpu->activeRasterState.anyBlendEnabled = TRUE;
                        }
                        /*
                            glBlendColor — set the blend color.

                            The GL_BLEND_COLOR may be used to calculate the source and destination blending factors. The color components are clamped to the range [0,1] before being stored.
                            See glBlendFunc for a complete description of the blending operations. Initially the GL_BLEND_COLOR is set to (0, 0, 0, 0).
                        */
                        if (AfxGetColorBlendConstants(ras, dpu->activeRasterState.blendConstants))
                        {
                            gl->BlendColor(dpu->activeRasterState.blendConstants[0], dpu->activeRasterState.blendConstants[1], dpu->activeRasterState.blendConstants[2], dpu->activeRasterState.blendConstants[3]); _SglThrowErrorOccuried();
                        }
                    }

                    if ((dpu->activeRasterState.outs[i].blendConfig.rgbBlendOp != ch->blendConfig.rgbBlendOp) || (dpu->activeRasterState.outs[i].blendConfig.aBlendOp != ch->blendConfig.aBlendOp))
                    {
                        /*
                            glBlendEquationSeparate — set the RGB blend equation and the alpha blend equation separately.

                            The blend equations determines how a new pixel (the source color) is combined with a pixel already in the framebuffer (the destination color).
                            These functions specify one blend equation for the RGB-color components and one blend equation for the alpha component.
                            glBlendEquationSeparatei specifies the blend equations for a single draw buffer whereas glBlendEquationSeparate sets the blend equations for all draw buffers.

                            The blend equations use the source and destination blend factors specified by either glBlendFunc or glBlendFuncSeparate.
                            See glBlendFunc or glBlendFuncSeparate for a description of the various blend factors.
                        */
                        gl->BlendEquationSeparatei(i, AfxToGlBlendOp(ch->blendConfig.rgbBlendOp), AfxToGlBlendOp(ch->blendConfig.aBlendOp)); _SglThrowErrorOccuried();
                        dpu->activeRasterState.outs[i].blendConfig.rgbBlendOp = ch->blendConfig.rgbBlendOp;
                        dpu->activeRasterState.outs[i].blendConfig.aBlendOp = ch->blendConfig.aBlendOp;
                    }

                    if ((dpu->activeRasterState.outs[i].blendConfig.rgbSrcFactor != ch->blendConfig.rgbSrcFactor) || (dpu->activeRasterState.outs[i].blendConfig.rgbDstFactor != ch->blendConfig.rgbDstFactor) || (dpu->activeRasterState.outs[i].blendConfig.aSrcFactor != ch->blendConfig.aSrcFactor) || (dpu->activeRasterState.outs[i].blendConfig.aDstFactor != ch->blendConfig.aDstFactor))
                    {
                        /*
                            glBlendFuncSeparate — specify pixel arithmetic for RGB and alpha components separately.

                            Pixels can be drawn using a function that blends the incoming (source) RGBA values with the RGBA values that are already in the frame buffer (the destination values).
                            Blending is initially disabled. Use glEnable and glDisable with argument GL_BLEND to enable and disable blending.

                            glBlendFuncSeparate defines the operation of blending for all draw buffers when it is enabled.
                            glBlendFuncSeparatei defines the operation of blending for a single draw buffer specified by buf when enabled for that draw buffer.
                            srcRGB specifies which method is used to scale the source RGB-color components. dstRGB specifies which method is used to scale the destination RGB-color components.
                            Likewise, srcAlpha specifies which method is used to scale the source alpha color component, and dstAlpha specifies which method is used to scale the destination alpha component.
                            The possible methods are described in the following table. Each method defines four scale factors, one each for red, green, blue, and alpha.
                        */
                        gl->BlendFuncSeparatei(i, AfxToGlBlendFactor(ch->blendConfig.rgbSrcFactor), AfxToGlBlendFactor(ch->blendConfig.rgbDstFactor), AfxToGlBlendFactor(ch->blendConfig.aSrcFactor), AfxToGlBlendFactor(ch->blendConfig.aDstFactor)); _SglThrowErrorOccuried();
                        dpu->activeRasterState.outs[i].blendConfig.rgbSrcFactor = ch->blendConfig.rgbSrcFactor;
                        dpu->activeRasterState.outs[i].blendConfig.rgbDstFactor = ch->blendConfig.rgbDstFactor;
                        dpu->activeRasterState.outs[i].blendConfig.aSrcFactor = ch->blendConfig.aSrcFactor;
                        dpu->activeRasterState.outs[i].blendConfig.aDstFactor = ch->blendConfig.aDstFactor;
                    }
                }
                dpu->activeRasterState.outs[i].blendEnabled = ch->blendEnabled;
            }

        }
#endif

#ifndef _SGL_DBG_IGNORE_COLOR_MASK

        if (ch->discardMask != dpu->nextRasterState.outs[i].discardMask)
        {
            /*
                glColorMask, glColorMaski — enable and disable writing of frame buffer color components.

                glColorMask and glColorMaski specify whether the individual color components in the frame buffer can or cannot be written.
                glColorMaski sets the mask for a specific draw buffer, whereas glColorMask sets the mask for all draw buffers.
                If red is GL_FALSE, for example, no change is made to the red component of any pixel in any of the color buffers, regardless of the drawing operation attempted.

                Changes to individual bits of components cannot be controlled. Rather, changes are either enabled or disabled for entire color components.
            */
            gl->ColorMaski(i, !(ch->discardMask & afxRgbaMask_R), !(ch->discardMask & afxRgbaMask_G), !(ch->discardMask & afxRgbaMask_B), !(ch->discardMask & afxRgbaMask_A)); _SglThrowErrorOccuried();
            dpu->nextRasterState.outs[i].discardMask = ch->discardMask;
        }
#endif
    }

#ifndef _SGL_DBG_IGNORE_BLEND
    if (blendNoUsed)
    {
        if (dpu->activeRasterState.anyBlendEnabled)
        {
            gl->Disable(GL_BLEND); _SglThrowErrorOccuried();
            dpu->activeRasterState.anyBlendEnabled = FALSE;
        }
    }
#endif

#ifndef _SGL_DBG_IGNORE_LOGICAL_OP

    afxLogicOp logicOp = dpu->nextRasterState.logicOp;
    afxBool logicOpEnabled = dpu->nextRasterState.logicOpEnabled;

    if (dpu->activeRasterState.logicOpEnabled != logicOpEnabled)
    {
        if (logicOpEnabled)
        {
            AfxAssert(!dpu->activeRasterState.logicOpEnabled);
            gl->Enable(GL_COLOR_LOGIC_OP); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeRasterState.logicOpEnabled);
            gl->Disable(GL_COLOR_LOGIC_OP); _SglThrowErrorOccuried();
        }
        dpu->activeRasterState.logicOpEnabled = logicOpEnabled;
    }

    if (logicOpEnabled)
    {
        if (dpu->activeRasterState.logicOp != logicOp)
        {
            /*
                glLogicOp — specify a logical pixel operation for rendering.

                glLogicOp specifies a logical operation that, when enabled, is applied between the incoming RGBA color and the RGBA color at the corresponding location in the frame buffer.
                To enable or disable the logical operation, call glEnable and glDisable using the symbolic constant GL_COLOR_LOGIC_OP. The initial value is disabled.

                Th opcode is a symbolic constant. The logical operation is applied independently to each bit pair of the source and destination colors.

                When more than one RGBA color buffer is enabled for drawing, logical operations are performed separately for each enabled buffer, using for the destination value the contents of that buffer (see glDrawBuffer).

                Logic operations have no effect on floating point draw buffers.
                However, if GL_COLOR_LOGIC_OP is enabled, blending is still disabled in this case.
            */
            gl->LogicOp(AfxToGlLogicOp(logicOp)); _SglThrowErrorOccuried();
            dpu->activeRasterState.logicOp = logicOp;
        }
    }
#endif
}

_SGL afxCmdId _SglEncodeCmdReadjustScissors(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdScissor *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd) + (cnt * sizeof(cmd->rect[0])));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;
    cmd->reset = FALSE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.ReadjustScissors) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdResetScissors(afxDrawScript dscr, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdScissor *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd) + (cnt * sizeof(cmd->rect[0])));
    AfxAssert(cmd);
    cmd->first = 0;
    cmd->cnt = cnt;
    cmd->reset = TRUE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.ResetScissors) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdReadjustAreas(afxDrawScript dscr, afxBool exclusive, afxNat32 first, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdArea *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd) + (cnt * sizeof(cmd->rect[0])));
    AfxAssert(cmd);
    cmd->exclusive = exclusive;
    cmd->first = first;
    cmd->cnt = cnt;
    cmd->reset = FALSE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.ReadjustAreas) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdResetAreas(afxDrawScript dscr, afxBool exclusive, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdArea *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd) + (cnt * sizeof(cmd->rect[0])));
    AfxAssert(cmd);
    cmd->exclusive = exclusive;
    cmd->first = 0;
    cmd->cnt = cnt;
    cmd->reset = TRUE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.ResetAreas) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdDisableRasterization(afxDrawScript dscr, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = disable;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.DisableRasterization) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdEnableDepthBias(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.EnableDepthBias) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetDepthBias(afxDrawScript dscr, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdReal3 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value[0] = constFactor;
    cmd->value[1] = clamp;
    cmd->value[2] = slopeFactor;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.SetDepthBias) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetLineWidth(afxDrawScript dscr, afxReal lineWidth)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdReal *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = lineWidth;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.SetLineWidth) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdEnableDepthBoundsTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.EnableDepthBoundsTest) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetDepthBounds(afxDrawScript dscr, afxReal const bounds[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdReal2 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value[0] = bounds[0];
    cmd->value[1] = bounds[1];

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.SetDepthBounds) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdEnableStencilTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.EnableStencilTest) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetStencilCompareMask(afxDrawScript dscr, afxMask faceMask, afxNat32 compareMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBitmaskNat32 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->mask = faceMask;
    cmd->value = compareMask;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.SetStencilCompareMask) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetStencilWriteMask(afxDrawScript dscr, afxMask faceMask, afxNat32 writeMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBitmaskNat32 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->mask = faceMask;
    cmd->value = writeMask;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.SetStencilWriteMask) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetStencilReference(afxDrawScript dscr, afxMask faceMask, afxNat32 reference)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBitmaskNat32 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->mask = faceMask;
    cmd->value = reference;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.SetStencilReference) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdEnableDepthTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.EnableDepthTest) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetDepthCompareOp(afxDrawScript dscr, afxCompareOp op)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdNat *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = op;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.SetDepthCompareOp) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdDisableDepthWrite(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.DisableDepthWrite) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetBlendConstants(afxDrawScript dscr, afxReal const blendConstants[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdReal4 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value[0] = blendConstants[0];
    cmd->value[1] = blendConstants[1];
    cmd->value[2] = blendConstants[2];
    cmd->value[3] = blendConstants[3];

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.SetBlendConstants) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdBeginSynthesis(afxDrawScript dscr, afxSynthesisConfig const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    afxNat extraRasCnt = state->rasterCnt;

    if (extraRasCnt)
        --extraRasCnt;

    _sglCmdBeginSynthesis *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd) + (extraRasCnt * sizeof(cmd->raster_[0])));
    AfxAssert(cmd);

    AfxRectCopy(&cmd->area, &state->area);
    cmd->layerCnt = state->layerCnt;
    cmd->rasterCnt = state->rasterCnt;
    cmd->canv = state->canv;
    cmd->defFbo = FALSE;

    cmd->raster0 = state->rasters[0];

    for (afxNat i = 0; i < extraRasCnt; i++)
        cmd->raster_[i] = state->rasters[1 + i];

    if (state->depth)
        cmd->depth = *state->depth;
    else
        cmd->depth = (afxDrawTarget) { 0 };

    if (state->stencil)
        cmd->stencil = *state->stencil;
    else
        cmd->stencil = (afxDrawTarget) { 0 };

    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.BeginSynthesis) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdFinsihSynthesis(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmd *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.FinishSynthesis) / sizeof(void*)), sizeof(cmd), cmd);
}

_SGL afxCmdId _SglEncodeCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdNextPass *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->useAuxScripts = !!useAuxScripts;
    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, Rasterization.NextPass) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdRasterization const _SglEncodeCmdRasterizationVmt =
{
    .DisableRasterization = _SglEncodeCmdDisableRasterization,
    .EnableDepthBias = _SglEncodeCmdEnableDepthBias,
    .SetDepthBias = _SglEncodeCmdSetDepthBias,
    .SetLineWidth = _SglEncodeCmdSetLineWidth,

    .ResetScissors = _SglEncodeCmdResetScissors,
    .ReadjustScissors = _SglEncodeCmdReadjustScissors,

    .EnableDepthBoundsTest = _SglEncodeCmdEnableDepthBoundsTest,
    .SetDepthBounds = _SglEncodeCmdSetDepthBounds,

    .EnableStencilTest = _SglEncodeCmdEnableStencilTest,
    .SetStencilCompareMask = _SglEncodeCmdSetStencilCompareMask,
    .SetStencilWriteMask = _SglEncodeCmdSetStencilWriteMask,
    .SetStencilReference = _SglEncodeCmdSetStencilReference,

    .EnableDepthTest = _SglEncodeCmdEnableDepthTest,
    .SetDepthCompareOp = _SglEncodeCmdSetDepthCompareOp,
    .DisableDepthWrite = _SglEncodeCmdDisableDepthWrite,

    .SetBlendConstants = _SglEncodeCmdSetBlendConstants,

    .BeginSynthesis = _SglEncodeCmdBeginSynthesis,
    .NextPass = _SglEncodeCmdNextPass,
    .FinishSynthesis = _SglEncodeCmdFinsihSynthesis
};
