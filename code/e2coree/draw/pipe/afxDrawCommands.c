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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_SCRIPT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_INDEX_BUFFER_C
#define _AFX_VERTEX_BUFFER_C
#include "qwadro/draw/pipe/afxDrawCommands.h"
#include "qwadro/draw/pipe/afxDrawScript.h"


_AFX afxString const afxDrawCmdStrings[] =
{
    AFX_STRING("PushPipeline"),
    AFX_STRING("PopPipeline"),

    AFX_STRING("EnableRasterization"),

    AFX_STRING("SwitchFrontFace"),
    AFX_STRING("SetCullMode"),

    AFX_STRING("EnableDepthBias"),
    AFX_STRING("SetDepthBias"),

    AFX_STRING("SetLineWidth"),
    
    AFX_STRING("EnableDepthTest"),
    AFX_STRING("EnableDepthWrite"),

    AFX_STRING("EnableStencilTest"),
    AFX_STRING("SetStencilCompareMask"),
    AFX_STRING("SetStencilWriteMask"),
    AFX_STRING("SetStencilReference"),

    AFX_STRING("EnableDepthBoundsTest"),
    AFX_STRING("SetDepthBounds"),

    AFX_STRING("SetBlendConstants"),

    AFX_STRING("ResetViewports"),
    AFX_STRING("UpdateViewports"),
    AFX_STRING("ResetScissors"),
    AFX_STRING("UpdateScissors"),
    AFX_STRING("ResetAreas"),
    AFX_STRING("UpdateAreas"),

    AFX_STRING("BindIndexSource"),
    AFX_STRING("BindVertexSources"),

    AFX_STRING("Draw"),
    AFX_STRING("DrawIndirect"),
    AFX_STRING("DrawIndirectCount"),
    AFX_STRING("DrawIndexed"),
    AFX_STRING("DrawIndexedIndirect"),
    AFX_STRING("DrawIndexedIndirectCount"),
};

_AFX afxCmdId AfxCmdBindVertexInput(afxDrawScript dscr, afxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    return dscr->stdCmds->BindVertexInput(dscr, vin);
}

_AFX afxCmdId AfxCmdBindPipeline(afxDrawScript dscr, afxNat segment, afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return dscr->stdCmds->BindPipeline(dscr, segment, pip);
}

_AFX afxCmdId AfxCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    return dscr->stdCmds->BindBuffers(dscr, set, baseIdx, cnt, buf, offset, range);
}

_AFX afxCmdId AfxCmdBindRasters(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    return dscr->stdCmds->BindRasters(dscr, set, baseIdx, cnt, smp, tex);
}

// Draw rendering state

_AFX afxCmdId AfxCmdSwitchFrontFace(afxDrawScript dscr, afxBool cw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->SwitchFrontFace(dscr, cw);
}

_AFX afxCmdId AfxCmdSetCullMode(afxDrawScript dscr, afxCullMode mode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->SetCullMode(dscr, mode);
}

_AFX afxCmdId AfxCmdEnableDepthBias(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->EnableDepthBias(dscr, enable);
}

_AFX afxCmdId AfxCmdSetDepthBias(afxDrawScript dscr, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    afxDrawContext dctx = AfxGetObjectProvider(dscr);
    return dscr->stdCmds->SetDepthBias(dscr, constFactor, clamp, slopeFactor);
}

_AFX afxCmdId AfxCmdSetLineWidth(afxDrawScript dscr, afxReal lineWidth)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->SetLineWidth(dscr, lineWidth);
}

_AFX afxCmdId AfxCmdDisableRasterization(afxDrawScript dscr, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->DisableRasterization(dscr, disable);
}

_AFX afxCmdId AfxCmdEnableDepthTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->EnableDepthTest(dscr, enable);
}

_AFX afxCmdId AfxCmdSetDepthCompareOp(afxDrawScript dscr, afxCompareOp op)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->SetDepthCompareOp(dscr, op);
}

_AFX afxCmdId AfxCmdEnableDepthWrite(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->EnableDepthWrite(dscr, enable);
}

_AFX afxCmdId AfxCmdEnableStencilTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->EnableStencilTest(dscr, enable);
}

_AFX afxCmdId AfxCmdSetStencilCompareMask(afxDrawScript dscr, afxMask faceMask, afxNat32 compareMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->SetStencilCompareMask(dscr, faceMask, compareMask);
}

_AFX afxCmdId AfxCmdSetStencilWriteMask(afxDrawScript dscr, afxMask faceMask, afxNat32 writeMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->SetStencilWriteMask(dscr, faceMask, writeMask);
}

_AFX afxCmdId AfxCmdSetStencilReference(afxDrawScript dscr, afxMask faceMask, afxNat32 reference)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->SetStencilReference(dscr, faceMask, reference);
}

_AFX afxCmdId AfxCmdEnableDepthBoundsTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->EnableDepthBoundsTest(dscr, enable);
}

_AFX afxCmdId AfxCmdSetDepthBounds(afxDrawScript dscr, afxReal const bounds[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(bounds);
    return dscr->stdCmds->SetDepthBounds(dscr, bounds);
}

_AFX afxCmdId AfxCmdSetBlendConstants(afxDrawScript dscr, afxReal const blendConstants[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(blendConstants);
    return dscr->stdCmds->SetBlendConstants(dscr, blendConstants);
}

// Viewport and scissor

_AFX afxCmdId AfxCmdResetViewports(afxDrawScript dscr, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(!cnt || vp);
    return dscr->stdCmds->ResetViewports(dscr, cnt, vp);
}

_AFX afxCmdId AfxCmdReadjustViewports(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    AfxAssert(!cnt || vp);
    return dscr->stdCmds->ReadjustViewports(dscr, baseIdx, cnt, vp);
}

_AFX afxCmdId AfxCmdResetScissors(afxDrawScript dscr, afxNat cnt, afxRect const rc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(!cnt || rc);
    return dscr->stdCmds->ResetScissors(dscr, cnt, rc);
}

_AFX afxCmdId AfxCmdReadjustScissors(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    AfxAssert(!cnt || rect);
    return dscr->stdCmds->ReadjustScissors(dscr, baseIdx, cnt, rect);
}

_AFX afxCmdId AfxCmdResetAreas(afxDrawScript dscr, afxBool exclusive, afxNat cnt, afxRect const rc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(!cnt || rc);
    return dscr->stdCmds->ResetAreas(dscr, exclusive, cnt, rc);
}

_AFX afxCmdId AfxCmdReadjustAreas(afxDrawScript dscr, afxBool exclusive, afxNat baseIdx, afxNat cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    AfxAssert(!cnt || rect);
    return dscr->stdCmds->ReadjustAreas(dscr, exclusive, baseIdx, cnt, rect);
}

// Vertex input source

_AFX afxCmdId AfxCmdBindVertexSources(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    return dscr->stdCmds->BindVertexSources(dscr, baseIdx, cnt, buf, offset, range);
}

#if 0
_AFX afxCmdId AfxCmdResetVertexStreams(afxDrawScript dscr, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, 0, cnt);
    return dscr->stdCmds->ResetVertexStreams(dscr, cnt, srcIdx, stride, instance);
}

_AFX afxCmdId AfxCmdResetVertexAttributes(afxDrawScript dscr, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const srcIdx[], afxNat32 const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, 0, cnt);
    return dscr->stdCmds->ResetVertexAttributes(dscr, cnt, location, fmt, srcIdx, offset);
}
#endif

_AFX afxCmdId AfxCmdBindIndexSource(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferSize(buf), offset, range);
    AfxAssert(range);
    AfxAssert(idxSiz);
    return dscr->stdCmds->BindIndexSource(dscr, buf, offset, range, idxSiz);
}

_AFX afxCmdId AfxCmdSetPrimitiveTopology(afxDrawScript dscr, afxPrimTopology topology)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(topology < afxPrimTopology_TOTAL);
    return dscr->stdCmds->SetPrimitiveTopology(dscr, topology);
}

// Canvas

_AFX afxCmdId AfxCmdBeginSynthesis(afxDrawScript dscr, afxSynthesisConfig const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(state);
    return dscr->stdCmds->BeginSynthesis(dscr, state);
}

_AFX afxCmdId AfxCmdFinishSynthesis(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->FinishSynthesis(dscr);
}

_AFX afxCmdId AfxCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->NextPass(dscr, useAuxScripts);
}

_AFX afxCmdId AfxCmdExecuteCommands(afxDrawScript dscr, afxNat cnt, afxDrawScript aux[])
{
    afxError err = AFX_ERR_NONE;
    return dscr->stdCmds->ExecuteCommands(dscr, cnt, aux);
}

// Draw

_AFX afxCmdId AfxCmdDraw(afxDrawScript dscr, afxNat vtxCnt, afxNat instCnt, afxNat baseVtxIdx, afxNat baseInstIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(vtxCnt);
    //AfxAssert(instCnt);
    return dscr->stdCmds->Draw(dscr, vtxCnt, instCnt, baseVtxIdx, baseInstIdx);
}

_AFX afxCmdId AfxCmdDrawIndirect(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(drawCnt);
    AfxAssert(stride);
    return dscr->stdCmds->DrawIndirect(dscr, buf, offset, drawCnt, stride);
}

_AFX afxCmdId AfxCmdDrawIndirectCount(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);
    AfxAssert(cntBuf);
    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    return dscr->stdCmds->DrawIndirectCount(dscr, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}

_AFX afxCmdId AfxCmdDrawIndexed(afxDrawScript dscr, afxNat idxCnt, afxNat instCnt, afxNat baseIdx, afxNat vtxOff, afxNat baseInstIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(idxCnt);
    //AfxAssert(instCnt);
    return dscr->stdCmds->DrawIndexed(dscr, idxCnt, instCnt, baseIdx, vtxOff, baseInstIdx);
}

_AFX afxCmdId AfxCmdDrawIndexedIndirect(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(drawCnt);
    AfxAssert(stride);
    return dscr->stdCmds->DrawIndexedIndirect(dscr, buf, offset, drawCnt, stride);
}

_AFX afxCmdId AfxCmdDrawIndexedIndirectCount(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);
    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    return dscr->stdCmds->DrawIndexedIndirectCount(dscr, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}

// Raster ops

_AFX afxCmdId AfxCmdPackRaster(afxDrawScript dscr, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);
    return dscr->stdCmds->PackRaster(dscr, ras, buf, opCnt, ops);
}

_AFX afxCmdId AfxCmdPackRasterRegion(afxDrawScript dscr, afxRaster ras, afxBuffer buf, afxRasterRegion const* rgn, afxSize bufOffset, afxNat bufRowSiz, afxNat bufRowCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    AfxAssert(rgn);
    afxRasterIoOp op;
    op.rgn = *rgn;
    op.bufOffset = bufOffset;
    op.bufRowSiz = bufRowSiz;
    op.bufRowCnt = bufRowCnt;

    afxWhd whd;
    AfxGetRasterExtent(ras, 0, whd);
    AfxAssertWhd(whd, op.rgn.offset, op.rgn.whd);
    AfxAssertRange(AfxGetBufferSize(buf), bufOffset, bufRowSiz * bufRowCnt);
    AfxAssertRange(AfxCountRasterLods(ras), op.rgn.lodIdx, 1);
    AfxAssertRange(AfxCountRasterLayers(ras), op.rgn.baseLayer, op.rgn.layerCnt);

    return dscr->stdCmds->PackRaster(dscr, ras, buf, 1, &op);
}

_AFX afxCmdId AfxCmdUnpackRaster(afxDrawScript dscr, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);
    return dscr->stdCmds->UnpackRaster(dscr, ras, buf, opCnt, ops);
}

_AFX afxCmdId AfxCmdUnpackRasterRegion(afxDrawScript dscr, afxRaster ras, afxBuffer buf, afxRasterRegion const* rgn, afxSize bufOffset, afxNat bufRowSiz, afxNat bufRowCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    
    AfxAssert(rgn);
    afxRasterIoOp op;
    op.rgn = *rgn;
    op.bufOffset = bufOffset;
    op.bufRowSiz = bufRowSiz;
    op.bufRowCnt = bufRowCnt;

    afxWhd whd;
    AfxGetRasterExtent(ras, 0, whd);
    AfxAssertWhd(whd, op.rgn.offset, op.rgn.whd);
    AfxAssertRange(AfxGetBufferSize(buf), bufOffset, bufRowSiz * bufRowCnt);
    AfxAssertRange(AfxCountRasterLods(ras), op.rgn.lodIdx, 1);
    AfxAssertRange(AfxCountRasterLayers(ras), op.rgn.baseLayer, op.rgn.layerCnt);

    return dscr->stdCmds->UnpackRaster(dscr, ras, buf, 1, &op);
}

_AFX afxCmdId AfxCmdCopyRaster(afxDrawScript dscr, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_RAS);
    AfxAssert(opCnt);
    AfxAssert(ops);
    return dscr->stdCmds->CopyRaster(dscr, src, dst, opCnt, ops);
}

_AFX afxCmdId AfxCmdCopyRasterRegion(afxDrawScript dscr, afxRaster src, afxNat srcLodIdx, afxNat srcBaseLayer, afxNat srcLayerCnt, afxWhd srcOffset, afxRaster dst, afxRasterRegion const* dstRgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_RAS);
    
    AfxAssert(dstRgn);
    afxRasterCopyOp op;
    op.srcLodIdx = srcLodIdx;
    op.srcBaseLayer = srcBaseLayer;
    op.srcLayerCnt = srcLayerCnt;
    AfxCopyWhd(op.srcOffset, srcOffset);
    op.dst = *dstRgn;

    afxWhd whd;
    AfxGetRasterExtent(src, 0, whd);
    AfxAssertWhd(whd, op.srcOffset, op.dst.whd);
    AfxAssertRange(AfxCountRasterLods(src), op.srcLodIdx, 1);
    AfxAssertRange(AfxCountRasterLayers(src), op.srcBaseLayer, op.srcLayerCnt);

    AfxGetRasterExtent(dst, 0, whd);
    AfxAssertWhd(whd, op.dst.offset, op.dst.whd);
    AfxAssertRange(AfxCountRasterLods(dst), op.dst.lodIdx, 1);
    AfxAssertRange(AfxCountRasterLayers(dst), op.dst.baseLayer, op.dst.layerCnt);

    return dscr->stdCmds->CopyRaster(dscr, src, dst, 1, &op);
}

AFX afxCmdId AfxCmdRegenerateMipmaps(afxDrawScript dscr, afxRaster ras, afxNat baseLodIdx, afxNat lodCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(AfxCountRasterLods(ras), baseLodIdx, lodCnt);
    return dscr->stdCmds->RegenerateMipmaps(dscr, ras, baseLodIdx, lodCnt);
}

_AFX afxCmdId AfxCmdFlipRaster(afxDrawScript dscr, afxRaster ras, afxBool rows, afxBool columns, afxNat rgnCnt, afxRasterRegion const rgn[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(rows || columns);
    AfxAssert(rgnCnt);
    AfxAssert(rgn);
    return dscr->stdCmds->FlipRaster(dscr, ras, rows, columns, rgnCnt, rgn);
}

_AFX afxCmdId AfxCmdFlipRasterRegion(afxDrawScript dscr, afxRaster ras, afxBool rows, afxBool columns, afxRasterRegion const* rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(rows || columns);
    AfxAssert(rgn);

    afxWhd whd;
    AfxGetRasterExtent(ras, 0, whd);
    AfxAssertWhd(whd, rgn->offset, rgn->whd);
    AfxAssertRange(AfxCountRasterLods(ras), rgn->lodIdx, 1);
    AfxAssertRange(AfxCountRasterLayers(ras), rgn->baseLayer, rgn->layerCnt);

    return dscr->stdCmds->FlipRaster(dscr, ras, rows, columns, 1, rgn);
}

_AFX afxCmdId AfxCmdSwizzleRaster(afxDrawScript dscr, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxNat rgnCnt, afxRasterRegion const rgn[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(a || b);
    AfxAssert(rgnCnt);
    AfxAssert(rgn);
    return dscr->stdCmds->SwizzleRaster(dscr, ras, a, b, rgnCnt, rgn);
}

_AFX afxCmdId AfxCmdSwizzleRasterRegion(afxDrawScript dscr, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxRasterRegion const* rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(a || b);
    AfxAssert(rgn);

    afxWhd whd;
    AfxGetRasterExtent(ras, 0, whd);
    AfxAssertWhd(whd, rgn->offset, rgn->whd);
    AfxAssertRange(AfxCountRasterLods(ras), rgn->lodIdx, 1);
    AfxAssertRange(AfxCountRasterLayers(ras), rgn->baseLayer, rgn->layerCnt);

    return dscr->stdCmds->SwizzleRaster(dscr, ras, a, b, 1, rgn);
}

// buffer ops

_AFX afxCmdId AfxCmdCopyBuffer(afxDrawScript dscr, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &src, afxFcc_BUF);
    AfxAssertObjects(1, &dst, afxFcc_BUF);
    return dscr->stdCmds->CopyBuffer(dscr, src, dst, opCnt, ops);
}

_AFX afxCmdId AfxCmdCopyBufferRegion(afxDrawScript dscr, afxBuffer src, afxSize srcOffset, afxBuffer dst, afxSize dstOffset, afxSize range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &src, afxFcc_BUF);
    AfxAssertObjects(1, &dst, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferSize(src), srcOffset, range);
    AfxAssertRange(AfxGetBufferSize(dst), dstOffset, range);
    AfxAssert(range);
    
    afxBufferCopyOp op;
    op.srcOffset = srcOffset;
    op.dstOffset = dstOffset;
    op.range = range;
    return dscr->stdCmds->CopyBuffer(dscr, src, dst, 1, &op);
}

_AFX afxCmdId AfxCmdFillBuffer(afxDrawScript dscr, afxBuffer buf, afxSize offset, afxSize range, afxNat32 value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferSize(buf), offset, range);
    return dscr->stdCmds->FillBuffer(dscr, buf, offset, range, value);
}

_AFX afxCmdId AfxCmdClearBuffer(afxDrawScript dscr, afxBuffer buf, afxSize offset, afxSize range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferSize(buf), offset, range);
    return dscr->stdCmds->FillBuffer(dscr, buf, offset, range, 0);
}

_AFX afxCmdId AfxCmdUpdateBuffer(afxDrawScript dscr, afxBuffer buf, afxSize offset, afxSize range, void const* data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferSize(buf), offset, range);
    AfxAssert(data);
    return dscr->stdCmds->UpdateBuffer(dscr, buf, offset, range, data);
}











_AFX afxCmdId AfxCmdBindFontSIG(afxDrawScript dscr, afxNat first, afxNat cnt, afxTypography typ[], afxPipeline pip[], afxSampler smp[], afxRaster ras[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(first == 0);
    AfxAssert(cnt == 1);
    AfxCmdBindPipeline(dscr, 0, pip[0]);
    AfxCmdBindRasters(dscr, 0, 1, 1, smp, ras);
}