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
#include "../dev/AvxDevKit.h"

_AVX afxCmdId AvxCmdPackRasterRegions(avxCmdb cmdb, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIo const ops[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    /// opCnt must be greater than 0.
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIo const* op = &ops[i];
        AfxAssertRange(AfxCountRasterLods(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertWhd(whd, op->rgn.origin, op->rgn.whd);
        AfxAssert(op->rowStride);
        AfxAssert(op->rowCnt);
    }
    return cmdb->stdCmds->ras.pak(cmdb, ras, buf, opCnt, ops, FALSE);
}

_AVX afxCmdId AvxCmdPackRaster(avxCmdb cmdb, afxRaster ras, afxRasterIo const* op, afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    AfxAssert(op);

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    return AvxCmdPackRasterRegions(cmdb, ras, buf, 1, op);
}

_AVX afxCmdId AvxCmdUnpackRasterRegions(avxCmdb cmdb, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIo const ops[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    /// opCnt must be greater than 0.
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIo const* op = &ops[i];
        AfxAssertRange(AfxCountRasterLods(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertWhd(whd, op->rgn.origin, op->rgn.whd);
        AfxAssert(op->rowStride);
        AfxAssert(op->rowCnt);
    }
    return cmdb->stdCmds->ras.pak(cmdb, ras, buf, opCnt, ops, TRUE);
}

_AVX afxCmdId AvxCmdUnpackRaster(avxCmdb cmdb, afxRaster ras, afxRasterIo const* op, afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    AfxAssert(op);

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    return AvxCmdUnpackRasterRegions(cmdb, ras, buf, 1, op);
}

_AVX afxCmdId AvxCmdCopyRasterRegions(avxCmdb cmdb, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopy const ops[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// opCnt must be greater than 0.
    AfxAssert(opCnt);
    AfxAssert(ops);

    /// dst must be a valid afxRaster handle.
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    /// src must be a valid afxRaster handle.
    AfxAssertObjects(1, &src, afxFcc_RAS);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterCopy const* op = &ops[i];

        AfxGetRasterExtent(src, 0, whd);
        AfxAssertWhd(whd, op->srcOffset, op->dst.whd);
        AfxAssertRange(AfxCountRasterLods(src), op->srcLodIdx, 1);

        AfxGetRasterExtent(dst, 0, whd);
        AfxAssertWhd(whd, op->dst.origin, op->dst.whd);
        AfxAssertRange(AfxCountRasterLods(dst), op->dst.lodIdx, 1);
    }
    return cmdb->stdCmds->ras.cpy(cmdb, src, dst, opCnt, ops);
}

_AVX afxCmdId AvxCmdCopyRaster(avxCmdb cmdb, afxRaster src, afxRasterCopy const* op, afxRaster dst)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// opCnt must be greater than 0.
    AfxAssert(op);

    /// dst must be a valid afxRaster handle.
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    /// src must be a valid afxRaster handle.
    AfxAssertObjects(1, &src, afxFcc_RAS);

    return AvxCmdCopyRasterRegions(cmdb, src, dst, 1, op);
}

AVX afxCmdId AvxCmdSubsampleRaster(avxCmdb cmdb, afxRaster ras, afxNat baseLod, afxNat lodCnt)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssertRange(AfxCountRasterLods(ras), baseLod, lodCnt);
    return cmdb->stdCmds->ras.mip(cmdb, ras, baseLod, lodCnt);
}

_AVX afxCmdId AvxCmdTransformRasterRegions(avxCmdb cmdb, afxRaster ras, afxM4d const m, afxNat rgnCnt, afxRasterRegion const regions[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(m);
    AfxAssert(rgnCnt);
    AfxAssert(regions);

    for (afxNat i = 0; i < rgnCnt; i++)
    {
        afxWhd whd;
        afxRasterRegion const* rgn = &regions[i];

        AfxGetRasterExtent(ras, 0, whd);
        AfxAssertWhd(whd, rgn->origin, rgn->whd);
        AfxAssertRange(AfxCountRasterLods(ras), rgn->lodIdx, 1);
    }
    return cmdb->stdCmds->ras.xform(cmdb, ras, m, rgnCnt, regions);
}

_AVX afxCmdId AvxCmdTransformRaster(avxCmdb cmdb, afxRaster ras, afxM4d const m, afxRasterRegion const* rgn)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(m);
    AfxAssert(rgn);
    return AvxCmdTransformRasterRegions(cmdb, ras, m, 1, rgn);
}

_AVX afxCmdId AvxCmdSwizzleRasterRegions(avxCmdb cmdb, afxRaster ras, avxColorSwizzle a, avxColorSwizzle b, afxNat rgnCnt, afxRasterRegion const regions[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(a || b);
    AfxAssert(rgnCnt);
    AfxAssert(regions);
    
    for (afxNat i = 0; i < rgnCnt; i++)
    {
        afxWhd whd;
        afxRasterRegion const* rgn = &regions[i];
        
        AfxGetRasterExtent(ras, 0, whd);
        AfxAssertWhd(whd, rgn->origin, rgn->whd);
        AfxAssertRange(AfxCountRasterLods(ras), rgn->lodIdx, 1);
    }
    return cmdb->stdCmds->ras.swizzle(cmdb, ras, a, b, rgnCnt, regions);
}

_AVX afxCmdId AvxCmdSwizzleRaster(avxCmdb cmdb, afxRaster ras, avxColorSwizzle a, avxColorSwizzle b, afxRasterRegion const* rgn)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(a || b);
    AfxAssert(rgn);
    return AvxCmdSwizzleRasterRegions(cmdb, ras, a, b, 1, rgn);
}
