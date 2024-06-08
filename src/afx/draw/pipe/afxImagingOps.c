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
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxCmdId AvxCmdEnableDepthBias(avxCmdb cmdb, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssertRange(enable, FALSE, TRUE);
    enable = AfxClamp(enable, FALSE, TRUE);

    return cmdb->stdCmds->Rasterization.EnableDepthBias(cmdb, enable);
}

_AVX afxCmdId AvxCmdSetDepthBias(avxCmdb cmdb, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    return cmdb->stdCmds->Rasterization.SetDepthBias(cmdb, constFactor, clamp, slopeFactor);
}

_AVX afxCmdId AvxCmdSetLineWidth(avxCmdb cmdb, afxReal lineWidth)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    return cmdb->stdCmds->Rasterization.SetLineWidth(cmdb, lineWidth);
}

_AVX afxCmdId AvxCmdDisableRasterization(avxCmdb cmdb, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssertRange(disable, FALSE, TRUE);
    disable = AfxClamp(disable, FALSE, TRUE);

    return cmdb->stdCmds->Rasterization.DisableRasterization(cmdb, disable);
}

_AVX afxCmdId AvxCmdEnableDepthTest(avxCmdb cmdb, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssertRange(enable, FALSE, TRUE);
    enable = AfxClamp(enable, FALSE, TRUE);

    return cmdb->stdCmds->Rasterization.EnableDepthTest(cmdb, enable);
}

_AVX afxCmdId AvxCmdSetDepthCompareOp(avxCmdb cmdb, afxCompareOp op)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    AfxAssertBounds(op, afxCompareOp_NEVER, afxCompareOp_ALWAYS);

    op = AfxClamp(op, afxCompareOp_NEVER, afxCompareOp_ALWAYS);

    return cmdb->stdCmds->Rasterization.SetDepthCompareOp(cmdb, op);
}

_AVX afxCmdId AvxCmdDisableDepthWrite(avxCmdb cmdb, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    
    AfxAssertRange(disable, FALSE, TRUE);
    disable = AfxClamp(disable, FALSE, TRUE);

    return cmdb->stdCmds->Rasterization.DisableDepthWrite(cmdb, disable);
}

_AVX afxCmdId AvxCmdEnableStencilTest(avxCmdb cmdb, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    return cmdb->stdCmds->Rasterization.EnableStencilTest(cmdb, enable);
}

_AVX afxCmdId AvxCmdSetStencilCompareMask(avxCmdb cmdb, afxMask faceMask, afxNat32 compareMask)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssert(faceMask);

    return cmdb->stdCmds->Rasterization.SetStencilCompareMask(cmdb, faceMask, compareMask);
}

_AVX afxCmdId AvxCmdSetStencilWriteMask(avxCmdb cmdb, afxMask faceMask, afxNat32 writeMask)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssert(faceMask);

    return cmdb->stdCmds->Rasterization.SetStencilWriteMask(cmdb, faceMask, writeMask);
}

_AVX afxCmdId AvxCmdSetStencilReference(avxCmdb cmdb, afxMask faceMask, afxNat32 reference)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssert(faceMask);

    return cmdb->stdCmds->Rasterization.SetStencilReference(cmdb, faceMask, reference);
}

_AVX afxCmdId AvxCmdEnableDepthBoundsTest(avxCmdb cmdb, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssertRange(enable, FALSE, TRUE);
    enable = AfxClamp(enable, FALSE, TRUE);

    return cmdb->stdCmds->Rasterization.EnableDepthBoundsTest(cmdb, enable);
}

_AVX afxCmdId AvxCmdSetDepthBounds(avxCmdb cmdb, afxV2d const bounds)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    
    if (!bounds)
        return cmdb->stdCmds->Rasterization.SetDepthBounds(cmdb, AfxSpawnV2d(0, 1));
    else
        return cmdb->stdCmds->Rasterization.SetDepthBounds(cmdb, bounds);
}

_AVX afxCmdId AvxCmdSetBlendConstants(avxCmdb cmdb, afxV4d const blendConstants)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    
    if (blendConstants)
        return cmdb->stdCmds->Rasterization.SetBlendConstants(cmdb, blendConstants);
    else
        return cmdb->stdCmds->Rasterization.SetBlendConstants(cmdb, AfxSpawnV4d(1, 1, 1, 1));
}

_AVX afxCmdId AvxCmdAdjustScissors(avxCmdb cmdb, afxNat baseIdx, afxNat cnt, afxRect const rects[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    /// cnt must be greater than 0.
    AfxAssert(cnt);

    /// rects must be a valid pointer to an array of cnt valid afxRect structures.
    AfxAssert(rects);
    AfxAssertRange(8, baseIdx, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat scIdx = baseIdx + i;
        afxRect const* rc = &rects[scIdx];

        /// The x and y members of offset member of any element of rects must be greater than or equal to 0.
        AfxAssert(rc->offset[0] >= 0);
        AfxAssert(rc->offset[1] >= 0);

        /// Evaluation of (offset + extent) must not cause a signed integer addition overflow for any element of rects.
        AfxAssert(rc->offset[0] + (afxInt)rc->extent[0] > 0);
        AfxAssert(rc->offset[1] + (afxInt)rc->extent[1] > 0);
    }
    return cmdb->stdCmds->Rasterization.AdjustScissors(cmdb, baseIdx, cnt, rects);
}

_AVX afxCmdId AvxCmdAdjustCurtains(avxCmdb cmdb, afxNat baseIdx, afxNat cnt, afxRect const rects[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    /// cnt must be greater than 0.
    AfxAssert(cnt);

    /// rects must be a valid pointer to an array of cnt valid afxRect structures.
    AfxAssert(rects);
    AfxAssertRange(8, baseIdx, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat scIdx = baseIdx + i;
        afxRect const* rc = &rects[scIdx];

        /// The x and y members of offset member of any element of rects must be greater than or equal to 0.
        AfxAssert(rc->offset[0] >= 0);
        AfxAssert(rc->offset[1] >= 0);

        /// Evaluation of (offset + extent) must not cause a signed integer addition overflow for any element of rects.
        AfxAssert(rc->offset[0] + (afxInt)rc->extent[0] > 0);
        AfxAssert(rc->offset[1] + (afxInt)rc->extent[1] > 0);
    }
    return cmdb->stdCmds->Rasterization.AdjustCurtains(cmdb, baseIdx, cnt, rects);
}

_AVX afxCmdId AvxCmdBeginSynthesis(avxCmdb cmdb, afxSynthesisConfig const* cfg)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    
    /// cfg must be a valid pointer to a valid afxSynthesisConfig structure.
    AfxAssert(cfg);
    afxSynthesisConfig cfg2 = *cfg;
    afxCanvas canv = cfg->canv;

    if (canv)
    {
        AfxAssertObjects(1, &canv, afxFcc_CANV);
        afxWhd whd;
        AfxGetCanvasExtent(canv, whd);
        AfxAssertRange(whd[0], cfg->area.offset[0], cfg->area.extent[0]);
        AfxAssertRange(whd[1], cfg->area.offset[1], cfg->area.extent[1]);
        
        cfg2.area.x = AfxClamp(cfg->area.x, 0, (afxInt)whd[0]);
        cfg2.area.y = AfxClamp(cfg->area.y, 0, (afxInt)whd[1]);
        cfg2.area.w = cfg->area.w ? AfxClamp(cfg->area.w, 1, whd[0] - cfg2.area.x) : whd[0];
        cfg2.area.h = cfg->area.h ? AfxClamp(cfg->area.h, 1, whd[1] - cfg2.area.y) : whd[1];
    }

    if (cfg->depth)
    {
        AfxAssertBounds(cfg->depth->loadOp, afxSurfaceLoadOp_CLEAR, afxSurfaceLoadOp_DONT_CARE);
        AfxAssertBounds(cfg->depth->storeOp, afxSurfaceStoreOp_STORE, afxSurfaceStoreOp_DONT_CARE);
    }

    if (cfg->stencil)
    {
        AfxAssertBounds(cfg->stencil->loadOp, afxSurfaceLoadOp_CLEAR, afxSurfaceLoadOp_DONT_CARE);
        AfxAssertBounds(cfg->stencil->storeOp, afxSurfaceStoreOp_STORE, afxSurfaceStoreOp_DONT_CARE);
    }

    for (afxNat i = 0; i < cfg->rasterCnt; i++)
    {
        AfxAssert(cfg->rasters);

        AfxAssertBounds(cfg->rasters[i].loadOp, afxSurfaceLoadOp_CLEAR, afxSurfaceLoadOp_DONT_CARE);
        AfxAssertBounds(cfg->rasters[i].storeOp, afxSurfaceStoreOp_STORE, afxSurfaceStoreOp_DONT_CARE);
    }

    return cmdb->stdCmds->Rasterization.BeginSynthesis(cmdb, &cfg2);
}

_AVX afxCmdId AvxCmdFinishSynthesis(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    return cmdb->stdCmds->Rasterization.FinishSynthesis(cmdb);
}

_AVX afxCmdId AvxCmdNextPass(avxCmdb cmdb, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    return cmdb->stdCmds->Rasterization.NextPass(cmdb, useAuxScripts);
}
