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
#include "afx/draw/afxDrawCommands.h"
#include "afx/draw/afxDrawScript.h"


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

    AFX_STRING("BindIndexSource"),
    AFX_STRING("BindVertexSources"),

    AFX_STRING("Draw"),
    AFX_STRING("DrawIndirect"),
    AFX_STRING("DrawIndirectCount"),
    AFX_STRING("DrawIndexed"),
    AFX_STRING("DrawIndexedIndirect"),
    AFX_STRING("DrawIndexedIndirectCount"),
};

_AFX void AfxCmdBindPipeline(afxDrawScript dscr, afxNat level, afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    dscr->stdCmds->BindPipeline(dscr, level, pip);
}

_AFX void AfxCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    dscr->stdCmds->BindBuffers(dscr, set, baseIdx, cnt, buf, offset, range);
}

_AFX void AfxCmdBindTextures(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    dscr->stdCmds->BindTextures(dscr, set, baseIdx, cnt, smp, tex);
}

// Draw rendering state

_AFX void AfxCmdSwitchFrontFace(afxDrawScript dscr, afxBool cw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SwitchFrontFace(dscr, cw);
}

_AFX void AfxCmdSetCullMode(afxDrawScript dscr, afxCullMode mode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetCullMode(dscr, mode);
}

_AFX void AfxCmdEnableDepthBias(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EnableDepthBias(dscr, enable);
}

_AFX void AfxCmdSetDepthBias(afxDrawScript dscr, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    afxDrawContext dctx = AfxGetObjectProvider(dscr);
    dscr->stdCmds->SetDepthBias(dscr, constFactor, clamp, slopeFactor);
}

_AFX void AfxCmdSetLineWidth(afxDrawScript dscr, afxReal lineWidth)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetLineWidth(dscr, lineWidth);
}

_AFX void AfxCmdDisableRasterization(afxDrawScript dscr, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->DisableRasterization(dscr, disable);
}

_AFX void AfxCmdEnableDepthTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EnableDepthTest(dscr, enable);
}

_AFX void AfxCmdSetDepthCompareOp(afxDrawScript dscr, afxCompareOp op)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetDepthCompareOp(dscr, op);
}

_AFX void AfxCmdEnableDepthWrite(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EnableDepthWrite(dscr, enable);
}

_AFX void AfxCmdEnableStencilTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EnableStencilTest(dscr, enable);
}

_AFX void AfxCmdSetStencilCompareMask(afxDrawScript dscr, afxMask faceMask, afxNat32 compareMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetStencilCompareMask(dscr, faceMask, compareMask);
}

_AFX void AfxCmdSetStencilWriteMask(afxDrawScript dscr, afxMask faceMask, afxNat32 writeMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetStencilWriteMask(dscr, faceMask, writeMask);
}

_AFX void AfxCmdSetStencilReference(afxDrawScript dscr, afxMask faceMask, afxNat32 reference)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetStencilReference(dscr, faceMask, reference);
}

_AFX void AfxCmdEnableDepthBoundsTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EnableDepthBoundsTest(dscr, enable);
}

_AFX void AfxCmdSetDepthBounds(afxDrawScript dscr, afxReal const bounds[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(bounds);
    dscr->stdCmds->SetDepthBounds(dscr, bounds);
}

_AFX void AfxCmdSetBlendConstants(afxDrawScript dscr, afxReal const blendConstants[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(blendConstants);
    dscr->stdCmds->SetBlendConstants(dscr, blendConstants);
}

// Viewport and scissor

_AFX void AfxCmdResetViewports(afxDrawScript dscr, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(!cnt || vp);
    dscr->stdCmds->ResetViewports(dscr, cnt, vp);
}

_AFX void AfxCmdUpdateViewports(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    AfxAssert(!cnt || vp);
    dscr->stdCmds->UpdateViewports(dscr, baseIdx, cnt, vp);
}

_AFX void AfxCmdResetScissors(afxDrawScript dscr, afxNat cnt, afxRect const rc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(!cnt || rc);
    dscr->stdCmds->ResetScissors(dscr, cnt, rc);
}

_AFX void AfxCmdUpdateScissors(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    AfxAssert(!cnt || rect);
    dscr->stdCmds->UpdateScissors(dscr, baseIdx, cnt, rect);
}

// Vertex input source

_AFX void AfxCmdBindVertexSources(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    dscr->stdCmds->BindVertexSources(dscr, baseIdx, cnt, buf, offset, range);
}

_AFX void AfxCmdResetVertexStreams(afxDrawScript dscr, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, 0, cnt);
    dscr->stdCmds->ResetVertexStreams(dscr, cnt, srcIdx, stride, instance);
}

_AFX void AfxCmdResetVertexAttributes(afxDrawScript dscr, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const srcIdx[], afxNat32 const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, 0, cnt);
    dscr->stdCmds->ResetVertexAttributes(dscr, cnt, location, fmt, srcIdx, offset);
}

_AFX void AfxCmdBindIndexSource(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferSize(buf), offset, 1);
    AfxAssert(idxSiz);
    dscr->stdCmds->BindIndexSource(dscr, buf, offset, idxSiz);
}

_AFX void AfxCmdSetPrimitiveTopology(afxDrawScript dscr, afxPrimTopology topology)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(topology < afxPrimTopology_TOTAL);
    dscr->stdCmds->SetPrimitiveTopology(dscr, topology);
}

// Canvas

_AFX void AfxCmdBeginCanvas(afxDrawScript dscr, afxCanvasConfig const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(state);
    dscr->stdCmds->BeginCanvas(dscr, state);
}

_AFX void AfxCmdEndCanvas(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EndCanvas(dscr);
}

_AFX void AfxCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->NextPass(dscr, useAuxScripts);
}

_AFX void AfxCmdExecuteCommands(afxDrawScript dscr, afxNat cnt, afxDrawScript aux[])
{
    afxError err = AFX_ERR_NONE;
    dscr->stdCmds->ExecuteCommands(dscr, cnt, aux);
}

// Draw

_AFX void AfxCmdDraw(afxDrawScript dscr, afxNat vtxCnt, afxNat instCnt, afxNat baseVtxIdx, afxNat baseInstIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(vtxCnt);
    AfxAssert(instCnt);
    dscr->stdCmds->Draw(dscr, vtxCnt, instCnt, baseVtxIdx, baseInstIdx);
}

_AFX void AfxCmdDrawIndirect(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(drawCnt);
    AfxAssert(stride);
    dscr->stdCmds->DrawIndirect(dscr, buf, offset, drawCnt, stride);
}

_AFX void AfxCmdDrawIndirectCount(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);
    AfxAssert(cntBuf);
    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    dscr->stdCmds->DrawIndirectCount(dscr, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}

_AFX void AfxCmdDrawIndexed(afxDrawScript dscr, afxNat idxCnt, afxNat instCnt, afxNat baseIdx, afxNat vtxOff, afxNat baseInstIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(idxCnt);
    AfxAssert(instCnt);
    dscr->stdCmds->DrawIndexed(dscr, idxCnt, instCnt, baseIdx, vtxOff, baseInstIdx);
}

_AFX void AfxCmdDrawIndexedIndirect(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(drawCnt);
    AfxAssert(stride);
    dscr->stdCmds->DrawIndexedIndirect(dscr, buf, offset, drawCnt, stride);
}

_AFX void AfxCmdDrawIndexedIndirectCount(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);
    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    dscr->stdCmds->DrawIndexedIndirectCount(dscr, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}

_AFX void AfxCmdDrawPrefab(afxDrawScript dscr, afxDrawPrefab prefab, afxNat instCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(afxDrawPrefab_TOTAL, prefab, 1);
    AfxAssert(instCnt);
    dscr->stdCmds->DrawPrefab(dscr, prefab, instCnt);
}
