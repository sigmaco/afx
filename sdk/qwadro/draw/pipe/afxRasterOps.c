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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_DRAW_C
#define _AFX_DRAW_STREAM_C
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxCmdId AfxCmdOutputRaster(afxDrawStream diob, afxRaster ras, afxStream dst, afxNat opCnt, afxRasterIoOp const ops[], afxCodec cdc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &dst, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIoOp const* op = &ops[i];
        AfxAssertRange(AfxCountRasterLods(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertRange(AfxCountRasterLayers(ras), op->rgn.baseLayer, op->rgn.layerCnt);
        AfxAssertWhd(whd, op->rgn.offset, op->rgn.whd);
        AfxAssert(op->bufRowSiz);
        AfxAssert(op->bufRowCnt);
    }
    return diob->stdCmds->ras.io(diob, ras, dst, opCnt, ops, TRUE, cdc);
}

_AVX afxCmdId AfxCmdOutputRasterRegion(afxDrawStream diob, afxRaster ras, afxRasterRegion const* rgn, afxStream dst, afxNat dstOffset, afxNat dstRowSiz, afxNat dstRowCnt, afxCodec cdc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &dst, afxFcc_BUF);    
    afxRasterIoOp op;
    AfxAssert(rgn);
    op.rgn = *rgn;
    op.bufOffset = dstOffset;
    op.bufRowSiz = dstRowSiz;
    op.bufRowCnt = dstRowCnt;
    return AfxCmdOutputRaster(diob, ras, dst, 1, &op, cdc);
}

_AVX afxCmdId AfxCmdInputRaster(afxDrawStream diob, afxRaster ras, afxStream src, afxNat opCnt, afxRasterIoOp const ops[], afxCodec cdc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_IOB);
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIoOp const* op = &ops[i];
        AfxAssertRange(AfxCountRasterLods(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertRange(AfxCountRasterLayers(ras), op->rgn.baseLayer, op->rgn.layerCnt);
        AfxAssertWhd(whd, op->rgn.offset, op->rgn.whd);
        AfxAssert(op->bufRowSiz);
        AfxAssert(op->bufRowCnt);
    }
    return diob->stdCmds->ras.io(diob, ras, src, opCnt, ops, FALSE, cdc);
}

_AVX afxCmdId AfxCmdInputRasterRegion(afxDrawStream diob, afxRaster ras, afxRasterRegion const* rgn, afxStream src, afxNat srcOffset, afxNat srcRowSiz, afxNat srcRowCnt, afxCodec cdc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_IOB);
    afxRasterIoOp op;
    AfxAssert(rgn);
    op.rgn = *rgn;
    op.bufOffset = srcOffset;
    op.bufRowSiz = srcRowSiz;
    op.bufRowCnt = srcRowCnt;
    return AfxCmdInputRaster(diob, ras, src, 1, &op, cdc);
}

_AVX afxCmdId AfxCmdPackRaster(afxDrawStream diob, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIoOp const* op = &ops[i];
        AfxAssertRange(AfxCountRasterLods(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertRange(AfxCountRasterLayers(ras), op->rgn.baseLayer, op->rgn.layerCnt);
        AfxAssertWhd(whd, op->rgn.offset, op->rgn.whd);
        AfxAssert(op->bufRowSiz);
        AfxAssert(op->bufRowCnt);
    }
    return diob->stdCmds->ras.pak(diob, ras, buf, opCnt, ops, FALSE);
}

_AVX afxCmdId AfxCmdPackRasterRegion(afxDrawStream diob, afxRaster ras, afxRasterRegion const* rgn, afxBuffer buf, afxNat bufOffset, afxNat bufRowSiz, afxNat bufRowCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    afxRasterIoOp op;
    AfxAssert(rgn);
    op.rgn = *rgn;
    op.bufOffset = bufOffset;
    op.bufRowSiz = bufRowSiz;
    op.bufRowCnt = bufRowCnt;
    return AfxCmdPackRaster(diob, ras, buf, 1, &op);
}

_AVX afxCmdId AfxCmdUnpackRaster(afxDrawStream diob, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIoOp const* op = &ops[i];
        AfxAssertRange(AfxCountRasterLods(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertRange(AfxCountRasterLayers(ras), op->rgn.baseLayer, op->rgn.layerCnt);
        AfxAssertWhd(whd, op->rgn.offset, op->rgn.whd);
        AfxAssert(op->bufRowSiz);
        AfxAssert(op->bufRowCnt);
    }
    return diob->stdCmds->ras.pak(diob, ras, buf, opCnt, ops, TRUE);
}

_AVX afxCmdId AfxCmdUnpackRasterRegion(afxDrawStream diob, afxRaster ras, afxRasterRegion const* rgn, afxBuffer buf, afxNat bufOffset, afxNat bufRowSiz, afxNat bufRowCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);    
    afxRasterIoOp op;
    AfxAssert(rgn);
    op.rgn = *rgn;
    op.bufOffset = bufOffset;
    op.bufRowSiz = bufRowSiz;
    op.bufRowCnt = bufRowCnt;
    return AfxCmdUnpackRaster(diob, ras, buf, 1, &op);
}

_AVX afxCmdId AfxCmdCopyRaster(afxDrawStream diob, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_RAS);
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterCopyOp const* op = &ops[i];

        AfxGetRasterExtent(src, 0, whd);
        AfxAssertWhd(whd, op->srcOffset, op->dst.whd);
        AfxAssertRange(AfxCountRasterLods(src), op->srcLodIdx, 1);
        AfxAssertRange(AfxCountRasterLayers(src), op->srcBaseLayer, op->srcLayerCnt);

        AfxGetRasterExtent(dst, 0, whd);
        AfxAssertWhd(whd, op->dst.offset, op->dst.whd);
        AfxAssertRange(AfxCountRasterLods(dst), op->dst.lodIdx, 1);
        AfxAssertRange(AfxCountRasterLayers(dst), op->dst.baseLayer, op->dst.layerCnt);
    }
    return diob->stdCmds->ras.cpy(diob, src, dst, opCnt, ops);
}

_AVX afxCmdId AfxCmdCopyRasterRegion(afxDrawStream diob, afxRaster src, afxNat srcLodIdx, afxNat srcBaseLayer, afxNat srcLayerCnt, afxWhd srcOffset, afxRaster dst, afxRasterRegion const* dstRgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_RAS);    
    afxRasterCopyOp op;
    op.srcLodIdx = srcLodIdx;
    op.srcBaseLayer = srcBaseLayer;
    op.srcLayerCnt = srcLayerCnt;
    AfxCopyWhd(op.srcOffset, srcOffset);
    AfxAssert(dstRgn);
    op.dst = *dstRgn;
    return AfxCmdCopyRaster(diob, src, dst, 1, &op);
}

AVX afxCmdId AfxCmdSubsampleRaster(afxDrawStream diob, afxRaster ras, afxNat baseLod, afxNat lodCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(AfxCountRasterLods(ras), baseLod, lodCnt);
    return diob->stdCmds->ras.mip(diob, ras, baseLod, lodCnt);
}

_AVX afxCmdId AfxCmdTransformRaster(afxDrawStream diob, afxRaster ras, afxM4d const m, afxNat rgnCnt, afxRasterRegion const regions[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(m);
    AfxAssert(rgnCnt);
    AfxAssert(regions);

    for (afxNat i = 0; i < rgnCnt; i++)
    {
        afxWhd whd;
        afxRasterRegion const* rgn = &regions[i];

        AfxGetRasterExtent(ras, 0, whd);
        AfxAssertWhd(whd, rgn->offset, rgn->whd);
        AfxAssertRange(AfxCountRasterLods(ras), rgn->lodIdx, 1);
        AfxAssertRange(AfxCountRasterLayers(ras), rgn->baseLayer, rgn->layerCnt);
    }
    return diob->stdCmds->ras.xform(diob, ras, m, rgnCnt, regions);
}

_AVX afxCmdId AfxCmdTransformRasterRegion(afxDrawStream diob, afxRaster ras, afxM4d const m, afxRasterRegion const* rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(m);
    AfxAssert(rgn);
    return AfxCmdTransformRaster(diob, ras, m, 1, rgn);
}

_AVX afxCmdId AfxCmdSwizzleRaster(afxDrawStream diob, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxNat rgnCnt, afxRasterRegion const regions[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(a || b);
    AfxAssert(rgnCnt);
    AfxAssert(regions);
    
    for (afxNat i = 0; i < rgnCnt; i++)
    {
        afxWhd whd;
        afxRasterRegion const* rgn = &regions[i];
        
        AfxGetRasterExtent(ras, 0, whd);
        AfxAssertWhd(whd, rgn->offset, rgn->whd);
        AfxAssertRange(AfxCountRasterLods(ras), rgn->lodIdx, 1);
        AfxAssertRange(AfxCountRasterLayers(ras), rgn->baseLayer, rgn->layerCnt);
    }
    return diob->stdCmds->ras.swizzle(diob, ras, a, b, rgnCnt, regions);
}

_AVX afxCmdId AfxCmdSwizzleRasterRegion(afxDrawStream diob, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxRasterRegion const* rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(a || b);
    AfxAssert(rgn);
    return AfxCmdSwizzleRaster(diob, ras, a, b, 1, rgn);
}
