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

_AVX afxCmdId AvxCmdPackRaster(avxCmdb cmdb, afxRaster ras, afxBuffer buf, afxUnit opCnt, afxRasterIo const ops[])
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

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    /// opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    afxRasterInfo rasi;
    AfxDescribeRaster(ras, &rasi);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(rasi.lodCnt, op->rgn.lodIdx, 1);
        whd = AfxGetRasterExtent(ras, op->rgn.lodIdx);
        AfxAssertRangeWhd(whd, op->rgn.origin, op->rgn.extent);
        AFX_ASSERT(op->rowStride);
        AFX_ASSERT(op->rowsPerImg);
    }
    return cmdb->stdCmds->ras.pak(cmdb, ras, buf, opCnt, ops, FALSE);
}

_AVX afxCmdId AvxCmdUnpackRaster(avxCmdb cmdb, afxRaster ras, afxBuffer buf, afxUnit opCnt, afxRasterIo const ops[])
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

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    /// opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    afxRasterInfo rasi;
    AfxDescribeRaster(ras, &rasi);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(rasi.lodCnt, op->rgn.lodIdx, 1);
        whd = AfxGetRasterExtent(ras, op->rgn.lodIdx);
        AfxAssertRangeWhd(whd, op->rgn.origin, op->rgn.extent);
        AFX_ASSERT(op->rowStride);
        AFX_ASSERT(op->rowsPerImg);
    }
    return cmdb->stdCmds->ras.pak(cmdb, ras, buf, opCnt, ops, TRUE);
}

_AVX afxCmdId AvxCmdCopyRaster(avxCmdb cmdb, afxRaster src, afxRaster dst, afxUnit opCnt, afxRasterCopy const ops[])
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

    /// opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    /// dst must be a valid afxRaster handle.
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    /// src must be a valid afxRaster handle.
    AfxAssertObjects(1, &src, afxFcc_RAS);

    afxRasterInfo rasi, rasi2;
    AfxDescribeRaster(src, &rasi);
    AfxDescribeRaster(dst, &rasi2);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxRasterCopy const* op = &ops[i];

        AfxAssertRangeWhd(rasi.extent, op->srcOrigin, op->dst.extent);
        AFX_ASSERT_RANGE(rasi.lodCnt, op->srcLodIdx, 1);

        AfxAssertRangeWhd(rasi2.extent, op->dst.origin, op->dst.extent);
        AFX_ASSERT_RANGE(rasi2.lodCnt, op->dst.lodIdx, 1);
    }
    return cmdb->stdCmds->ras.cpy(cmdb, src, dst, opCnt, ops);
}

AVX afxCmdId AvxCmdSubsampleRaster(avxCmdb cmdb, afxRaster ras, afxUnit baseLod, afxUnit lodCnt)
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

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxRasterInfo rasi;
    AfxDescribeRaster(ras, &rasi);

    AFX_ASSERT_RANGE(rasi.lodCnt, baseLod, lodCnt);
    return cmdb->stdCmds->ras.mip(cmdb, ras, baseLod, lodCnt);
}

_AVX afxCmdId AvxCmdTransformRaster(avxCmdb cmdb, afxRaster ras, afxM4d const m, afxUnit rgnCnt, afxRasterRegion const rgns[])
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

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AFX_ASSERT(m);
    AFX_ASSERT(rgnCnt);
    AFX_ASSERT(rgns);

    afxRasterInfo rasi;
    AfxDescribeRaster(ras, &rasi);

    for (afxUnit i = 0; i < rgnCnt; i++)
    {
        afxRasterRegion const* rgn = &rgns[i];

        AfxAssertRangeWhd(rasi.extent, rgn->origin, rgn->extent);
        AFX_ASSERT_RANGE(rasi.lodCnt, rgn->lodIdx, 1);
    }
    return cmdb->stdCmds->ras.xform(cmdb, ras, m, rgnCnt, rgns);
}

_AVX afxCmdId AvxCmdSwizzleRaster(avxCmdb cmdb, afxRaster ras, avxColorSwizzle a, avxColorSwizzle b, afxUnit rgnCnt, afxRasterRegion const rgns[])
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

    /// ras must be a valid afxRaster handle.
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AFX_ASSERT(a || b);
    AFX_ASSERT(rgnCnt);
    AFX_ASSERT(rgns);

    afxRasterInfo rasi;
    AfxDescribeRaster(ras, &rasi);

    for (afxUnit i = 0; i < rgnCnt; i++)
    {
        afxRasterRegion const* rgn = &rgns[i];
        AfxAssertRangeWhd(rasi.extent, rgn->origin, rgn->extent);
        AFX_ASSERT_RANGE(rasi.lodCnt, rgn->lodIdx, 1);
    }
    return cmdb->stdCmds->ras.swizzle(cmdb, ras, a, b, rgnCnt, rgns);
}
