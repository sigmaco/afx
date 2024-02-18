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

#define _AFX_DRAW_C
#define _AFX_DRAW_SCRIPT_C
#include "qwadro/mem/afxArena.h"
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

_AFX afxCmdId AfxCmdExecuteCommands(afxDrawScript dscr, afxNat cnt, afxDrawScript aux[])
{
    afxError err = AFX_ERR_NONE;
    return dscr->stdCmds->ExecuteCommands(dscr, cnt, aux);
}

// Draw

_AFX afxCmdId AfxCmdDraw(afxDrawScript dscr, afxNat baseInstIdx, afxNat instCnt, afxNat baseVtxIdx, afxNat vtxCnt)
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

_AFX afxCmdId AfxCmdDrawIndexed(afxDrawScript dscr, afxNat vtxOff, afxNat baseInstIdx, afxNat instCnt, afxNat baseIdx, afxNat idxCnt)
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











_AFX afxCmdId AfxCmdBindFontSIG(afxDrawScript dscr, afxNat first, afxNat cnt, afxTypography typ[], afxPipeline pip[], afxSampler smp[], afxRaster ras[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(first == 0);
    AfxAssert(cnt == 1);
    AfxCmdBindPipeline(dscr, 0, pip[0]);
    AfxCmdBindRasters(dscr, 0, 1, 1, smp, ras);
}
