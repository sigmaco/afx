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

_AVX afxCmdId AvxCmdPackRaster(avxCmdb cmdb, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIo const ops[])
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
        AfxAssertRange(AfxCountRasterMipmaps(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertRangeWhd(whd, op->rgn.origin, op->rgn.whd);
        AfxAssert(op->rowStride);
        AfxAssert(op->rowCnt);
    }
    return cmdb->stdCmds->ras.pak(cmdb, ras, buf, opCnt, ops, FALSE);
}

_AVX afxCmdId AvxCmdUnpackRaster(avxCmdb cmdb, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIo const ops[])
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
        AfxAssertRange(AfxCountRasterMipmaps(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertRangeWhd(whd, op->rgn.origin, op->rgn.whd);
        AfxAssert(op->rowStride);
        AfxAssert(op->rowCnt);
    }
    return cmdb->stdCmds->ras.pak(cmdb, ras, buf, opCnt, ops, TRUE);
}

_AVX afxCmdId AvxCmdCopyRaster(avxCmdb cmdb, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopy const ops[])
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
        AfxAssertRangeWhd(whd, op->srcOrigin, op->dst.whd);
        AfxAssertRange(AfxCountRasterMipmaps(src), op->srcLodIdx, 1);

        AfxGetRasterExtent(dst, 0, whd);
        AfxAssertRangeWhd(whd, op->dst.origin, op->dst.whd);
        AfxAssertRange(AfxCountRasterMipmaps(dst), op->dst.lodIdx, 1);
    }
    return cmdb->stdCmds->ras.cpy(cmdb, src, dst, opCnt, ops);
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

    AfxAssertRange(AfxCountRasterMipmaps(ras), baseLod, lodCnt);
    return cmdb->stdCmds->ras.mip(cmdb, ras, baseLod, lodCnt);
}

_AVX afxCmdId AvxCmdTransformRaster(avxCmdb cmdb, afxRaster ras, afxM4d const m, afxNat rgnCnt, afxRasterRegion const rgns[])
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
    AfxAssert(rgns);

    for (afxNat i = 0; i < rgnCnt; i++)
    {
        afxWhd whd;
        afxRasterRegion const* rgn = &rgns[i];

        AfxGetRasterExtent(ras, 0, whd);
        AfxAssertRangeWhd(whd, rgn->origin, rgn->whd);
        AfxAssertRange(AfxCountRasterMipmaps(ras), rgn->lodIdx, 1);
    }
    return cmdb->stdCmds->ras.xform(cmdb, ras, m, rgnCnt, rgns);
}

_AVX afxCmdId AvxCmdSwizzleRaster(avxCmdb cmdb, afxRaster ras, avxColorSwizzle a, avxColorSwizzle b, afxNat rgnCnt, afxRasterRegion const rgns[])
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
    AfxAssert(rgns);
    
    for (afxNat i = 0; i < rgnCnt; i++)
    {
        afxWhd whd;
        afxRasterRegion const* rgn = &rgns[i];
        
        AfxGetRasterExtent(ras, 0, whd);
        AfxAssertRangeWhd(whd, rgn->origin, rgn->whd);
        AfxAssertRange(AfxCountRasterMipmaps(ras), rgn->lodIdx, 1);
    }
    return cmdb->stdCmds->ras.swizzle(cmdb, ras, a, b, rgnCnt, rgns);
}
