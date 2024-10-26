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

#define _AVX_DRAW_C
#define _AVX_CMD_BUFFER_C
#include "../../dev/AvxImplKit.h"

_AVX afxCmdId AvxCmdEnableDepthBias(avxCmdb cmdb, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    AFX_ASSERT_RANGE(enable, FALSE, TRUE);
    enable = AfxClamp(enable, FALSE, TRUE);

    return cmdb->stdCmds->razr.EnableDepthBias(cmdb, enable);
}

_AVX afxCmdId AvxCmdSetDepthBias(avxCmdb cmdb, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    return cmdb->stdCmds->razr.SetDepthBias(cmdb, constFactor, clamp, slopeFactor);
}

_AVX afxCmdId AvxCmdSetLineWidth(avxCmdb cmdb, afxReal lineWidth)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    return cmdb->stdCmds->razr.SetLineWidth(cmdb, lineWidth);
}

_AVX afxCmdId AvxCmdDisableRasterization(avxCmdb cmdb, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    AFX_ASSERT_RANGE(disable, FALSE, TRUE);
    disable = AfxClamp(disable, FALSE, TRUE);

    return cmdb->stdCmds->razr.DisableRasterization(cmdb, disable);
}

_AVX afxCmdId AvxCmdEnableDepthTest(avxCmdb cmdb, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    AFX_ASSERT_RANGE(enable, FALSE, TRUE);
    enable = AfxClamp(enable, FALSE, TRUE);

    return cmdb->stdCmds->razr.EnableDepthTest(cmdb, enable);
}

_AVX afxCmdId AvxCmdSetDepthCompareOp(avxCmdb cmdb, avxCompareOp op)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    AFX_ASSERT_BOUNDS(op, avxCompareOp_NEVER, avxCompareOp_ALWAYS);

    op = AfxClamp(op, avxCompareOp_NEVER, avxCompareOp_ALWAYS);

    return cmdb->stdCmds->razr.SetDepthCompareOp(cmdb, op);
}

_AVX afxCmdId AvxCmdDisableDepthWrite(avxCmdb cmdb, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);
    
    AFX_ASSERT_RANGE(disable, FALSE, TRUE);
    disable = AfxClamp(disable, FALSE, TRUE);

    return cmdb->stdCmds->razr.DisableDepthWrite(cmdb, disable);
}

_AVX afxCmdId AvxCmdEnableStencilTest(avxCmdb cmdb, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    return cmdb->stdCmds->razr.EnableStencilTest(cmdb, enable);
}

_AVX afxCmdId AvxCmdSetStencilCompareMask(avxCmdb cmdb, afxMask faceMask, afxUnit32 compareMask)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    AFX_ASSERT(faceMask);

    return cmdb->stdCmds->razr.SetStencilCompareMask(cmdb, faceMask, compareMask);
}

_AVX afxCmdId AvxCmdSetStencilWriteMask(avxCmdb cmdb, afxMask faceMask, afxUnit32 writeMask)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    AFX_ASSERT(faceMask);

    return cmdb->stdCmds->razr.SetStencilWriteMask(cmdb, faceMask, writeMask);
}

_AVX afxCmdId AvxCmdSetStencilReference(avxCmdb cmdb, afxMask faceMask, afxUnit32 reference)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    AFX_ASSERT(faceMask);

    return cmdb->stdCmds->razr.SetStencilReference(cmdb, faceMask, reference);
}

_AVX afxCmdId AvxCmdEnableDepthBoundsTest(avxCmdb cmdb, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    AFX_ASSERT_RANGE(enable, FALSE, TRUE);
    enable = AfxClamp(enable, FALSE, TRUE);

    return cmdb->stdCmds->razr.EnableDepthBoundsTest(cmdb, enable);
}

_AVX afxCmdId AvxCmdSetDepthBounds(avxCmdb cmdb, afxV2d const bounds)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);
    
    if (!bounds)
        return cmdb->stdCmds->razr.SetDepthBounds(cmdb, AfxV2d(0, 1));
    else
        return cmdb->stdCmds->razr.SetDepthBounds(cmdb, bounds);
}

_AVX afxCmdId AvxCmdSetBlendConstants(avxCmdb cmdb, afxV4d const blendConstants)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);
    
    if (blendConstants)
        return cmdb->stdCmds->razr.SetBlendConstants(cmdb, blendConstants);
    else
        return cmdb->stdCmds->razr.SetBlendConstants(cmdb, AfxV4d(1, 1, 1, 1));
}

_AVX afxCmdId AvxCmdAdjustScissors(avxCmdb cmdb, afxUnit baseIdx, afxUnit cnt, afxRect const rects[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    /// cnt must be greater than 0.
    AFX_ASSERT(cnt);

    /// rects must be a valid pointer to an array of cnt valid afxRect structures.
    AFX_ASSERT(rects);
    AFX_ASSERT_RANGE(8, baseIdx, cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit scIdx = baseIdx + i;
        afxRect const* rc = &rects[scIdx];

        /// The x and y members of offset member of any element of rects must be greater than or equal to 0.
        AFX_ASSERT(rc->origin[0] >= 0);
        AFX_ASSERT(rc->origin[1] >= 0);

        /// Evaluation of (offset + extent) must not cause a signed integer addition overflow for any element of rects.
        AFX_ASSERT(rc->origin[0] + (afxInt)rc->extent[0] > 0);
        AFX_ASSERT(rc->origin[1] + (afxInt)rc->extent[1] > 0);
    }
    return cmdb->stdCmds->razr.AdjustScissors(cmdb, baseIdx, cnt, rects);
}

_AVX afxCmdId AvxCmdAdjustCurtains(avxCmdb cmdb, afxUnit baseIdx, afxUnit cnt, afxRect const rects[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    /// cnt must be greater than 0.
    AFX_ASSERT(cnt);

    /// rects must be a valid pointer to an array of cnt valid afxRect structures.
    AFX_ASSERT(rects);
    AFX_ASSERT_RANGE(8, baseIdx, cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit scIdx = baseIdx + i;
        afxRect const* rc = &rects[scIdx];

        /// The x and y members of offset member of any element of rects must be greater than or equal to 0.
        AFX_ASSERT(rc->origin[0] >= 0);
        AFX_ASSERT(rc->origin[1] >= 0);

        /// Evaluation of (offset + extent) must not cause a signed integer addition overflow for any element of rects.
        AFX_ASSERT(rc->origin[0] + (afxInt)rc->extent[0] > 0);
        AFX_ASSERT(rc->origin[1] + (afxInt)rc->extent[1] > 0);
    }
    return cmdb->stdCmds->razr.AdjustCurtains(cmdb, baseIdx, cnt, rects);
}

_AVX afxCmdId AvxCmdCommenceDrawScope(avxCmdb cmdb, avxDrawScope const* cfg)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);
    
    /// cfg must be a valid pointer to a valid avxDrawScope structure.
    AFX_ASSERT(cfg);
    avxDrawScope cfg2 = *cfg;
    avxCanvas canv = cfg->canv;

    if (canv)
    {
        AfxAssertObjects(1, &canv, afxFcc_CANV);
        afxWhd whd = AfxGetCanvasExtent(canv);
        AFX_ASSERT_RANGE(whd.w, cfg->area.origin[0], cfg->area.extent[0]);
        AFX_ASSERT_RANGE(whd.h, cfg->area.origin[1], cfg->area.extent[1]);
        
        cfg2.area.x = AfxClamp(cfg->area.x, 0, (afxInt)whd.w);
        cfg2.area.y = AfxClamp(cfg->area.y, 0, (afxInt)whd.h);
        cfg2.area.w = cfg->area.w ? AfxClamp(cfg->area.w, 1, whd.w - cfg2.area.x) : whd.w;
        cfg2.area.h = cfg->area.h ? AfxClamp(cfg->area.h, 1, whd.h - cfg2.area.y) : whd.h;
    }

    if (cfg->depth)
    {
        AFX_ASSERT_BOUNDS(cfg->depth->loadOp, avxLoadOp_CLEAR, avxLoadOp_DONT_CARE);
        AFX_ASSERT_BOUNDS(cfg->depth->storeOp, avxStoreOp_STORE, avxStoreOp_DONT_CARE);
    }

    if (cfg->stencil)
    {
        AFX_ASSERT_BOUNDS(cfg->stencil->loadOp, avxLoadOp_CLEAR, avxLoadOp_DONT_CARE);
        AFX_ASSERT_BOUNDS(cfg->stencil->storeOp, avxStoreOp_STORE, avxStoreOp_DONT_CARE);
    }

    for (afxUnit i = 0; i < cfg->rasterCnt; i++)
    {
        AFX_ASSERT(cfg->rasters);

        AFX_ASSERT_BOUNDS(cfg->rasters[i].loadOp, avxLoadOp_CLEAR, avxLoadOp_DONT_CARE);
        AFX_ASSERT_BOUNDS(cfg->rasters[i].storeOp, avxStoreOp_STORE, avxStoreOp_DONT_CARE);
    }

    return cmdb->stdCmds->razr.CommenceDrawScope(cmdb, &cfg2);
}

_AVX afxCmdId AvxCmdConcludeDrawScope(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    return cmdb->stdCmds->razr.ConcludeDrawScope(cmdb);
}

_AVX afxCmdId AvxCmdNextPass(avxCmdb cmdb, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    return cmdb->stdCmds->razr.NextPass(cmdb, useAuxScripts);
}
