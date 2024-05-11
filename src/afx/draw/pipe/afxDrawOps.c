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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_DRAW_STREAM_C
#include "qwadro/draw/afxDrawSystem.h"


_AVX afxString const afxDrawCmdStrings[] =
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

_AVX afxCmdId AfxCmdBindPipeline(afxDrawStream diob, afxNat segment, afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return diob->stdCmds->BindPipeline(diob, segment, pip);
}

_AVX afxCmdId AfxCmdBindBuffers(afxDrawStream diob, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertRange(8, baseIdx, cnt);
    return diob->stdCmds->BindBuffers(diob, set, baseIdx, cnt, buf, offset, range);
}

_AVX afxCmdId AfxCmdBindRasters(afxDrawStream diob, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertRange(8, baseIdx, cnt);
    return diob->stdCmds->BindRasters(diob, set, baseIdx, cnt, smp, tex);
}

_AVX afxCmdId AfxCmdExecuteCommands(afxDrawStream diob, afxNat cnt, afxDrawStream aux[])
{
    afxError err = AFX_ERR_NONE;
    return diob->stdCmds->ExecuteCommands(diob, cnt, aux);
}

// Draw

_AVX afxCmdId AfxCmdDraw(afxDrawStream diob, afxNat baseInstIdx, afxNat instCnt, afxNat baseVtxIdx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssert(vtxCnt);
    //AfxAssert(instCnt);
    return diob->stdCmds->Draw(diob, vtxCnt, instCnt, baseVtxIdx, baseInstIdx);
}

_AVX afxCmdId AfxCmdDrawIndirect(afxDrawStream diob, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(drawCnt);
    AfxAssert(stride);
    return diob->stdCmds->DrawIndirect(diob, buf, offset, drawCnt, stride);
}

_AVX afxCmdId AfxCmdDrawIndirectCount(afxDrawStream diob, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);
    AfxAssert(cntBuf);
    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    return diob->stdCmds->DrawIndirectCount(diob, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}

_AVX afxCmdId AfxCmdDrawIndexed(afxDrawStream diob, afxNat vtxOff, afxNat baseInstIdx, afxNat instCnt, afxNat baseIdx, afxNat idxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssert(idxCnt);
    //AfxAssert(instCnt);
    return diob->stdCmds->DrawIndexed(diob, idxCnt, instCnt, baseIdx, vtxOff, baseInstIdx);
}

_AVX afxCmdId AfxCmdDrawIndexedIndirect(afxDrawStream diob, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssert(drawCnt);
    AfxAssert(stride);
    return diob->stdCmds->DrawIndexedIndirect(diob, buf, offset, drawCnt, stride);
}

_AVX afxCmdId AfxCmdDrawIndexedIndirectCount(afxDrawStream diob, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);
    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    return diob->stdCmds->DrawIndexedIndirectCount(diob, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}











_AVX afxCmdId AfxCmdBindFontSIG(afxDrawStream diob, afxNat first, afxNat cnt, afxTypography typ[], afxPipeline pip[], afxSampler smp[], afxRaster ras[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssert(first == 0);
    AfxAssert(cnt == 1);
    AfxCmdBindPipeline(diob, 0, pip[0]);
    AfxCmdBindRasters(diob, 0, 1, 1, smp, ras);
    return 0;
}
