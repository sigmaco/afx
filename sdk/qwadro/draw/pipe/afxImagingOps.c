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

_AVX afxCmdId AfxCmdEnableDepthBias(afxDrawStream diob, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.EnableDepthBias(diob, enable);
}

_AVX afxCmdId AfxCmdSetDepthBias(afxDrawStream diob, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    afxDrawContext dctx = AfxGetObjectProvider(diob);
    return diob->stdCmds->Rasterization.SetDepthBias(diob, constFactor, clamp, slopeFactor);
}

_AVX afxCmdId AfxCmdSetLineWidth(afxDrawStream diob, afxReal lineWidth)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.SetLineWidth(diob, lineWidth);
}

_AVX afxCmdId AfxCmdDisableRasterization(afxDrawStream diob, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.DisableRasterization(diob, disable);
}

_AVX afxCmdId AfxCmdEnableDepthTest(afxDrawStream diob, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.EnableDepthTest(diob, enable);
}

_AVX afxCmdId AfxCmdSetDepthCompareOp(afxDrawStream diob, afxCompareOp op)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.SetDepthCompareOp(diob, op);
}

_AVX afxCmdId AfxCmdDisableDepthWrite(afxDrawStream diob, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.DisableDepthWrite(diob, enable);
}

_AVX afxCmdId AfxCmdEnableStencilTest(afxDrawStream diob, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.EnableStencilTest(diob, enable);
}

_AVX afxCmdId AfxCmdSetStencilCompareMask(afxDrawStream diob, afxMask faceMask, afxNat32 compareMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.SetStencilCompareMask(diob, faceMask, compareMask);
}

_AVX afxCmdId AfxCmdSetStencilWriteMask(afxDrawStream diob, afxMask faceMask, afxNat32 writeMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.SetStencilWriteMask(diob, faceMask, writeMask);
}

_AVX afxCmdId AfxCmdSetStencilReference(afxDrawStream diob, afxMask faceMask, afxNat32 reference)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.SetStencilReference(diob, faceMask, reference);
}

_AVX afxCmdId AfxCmdEnableDepthBoundsTest(afxDrawStream diob, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.EnableDepthBoundsTest(diob, enable);
}

_AVX afxCmdId AfxCmdSetDepthBounds(afxDrawStream diob, afxReal const bounds[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssert(bounds);
    return diob->stdCmds->Rasterization.SetDepthBounds(diob, bounds);
}

_AVX afxCmdId AfxCmdSetBlendConstants(afxDrawStream diob, afxReal const blendConstants[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssert(blendConstants);
    return diob->stdCmds->Rasterization.SetBlendConstants(diob, blendConstants);
}

_AVX afxCmdId AfxCmdResetScissors(afxDrawStream diob, afxNat cnt, afxRect const rc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssert(!cnt || rc);
    return diob->stdCmds->Rasterization.ResetScissors(diob, cnt, rc);
}

_AVX afxCmdId AfxCmdReadjustScissors(afxDrawStream diob, afxNat baseIdx, afxNat cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertRange(8, baseIdx, cnt);
    AfxAssert(!cnt || rect);
    return diob->stdCmds->Rasterization.ReadjustScissors(diob, baseIdx, cnt, rect);
}

_AVX afxCmdId AfxCmdResetAreas(afxDrawStream diob, afxBool exclusive, afxNat cnt, afxRect const rc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssert(!cnt || rc);
    return diob->stdCmds->Rasterization.ResetAreas(diob, exclusive, cnt, rc);
}

_AVX afxCmdId AfxCmdReadjustAreas(afxDrawStream diob, afxBool exclusive, afxNat baseIdx, afxNat cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertRange(8, baseIdx, cnt);
    AfxAssert(!cnt || rect);
    return diob->stdCmds->Rasterization.ReadjustAreas(diob, exclusive, baseIdx, cnt, rect);
}

_AVX afxCmdId AfxCmdBeginSynthesis(afxDrawStream diob, afxSynthesisConfig const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssert(state);
    return diob->stdCmds->Rasterization.BeginSynthesis(diob, state);
}

_AVX afxCmdId AfxCmdFinishSynthesis(afxDrawStream diob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.FinishSynthesis(diob);
}

_AVX afxCmdId AfxCmdNextPass(afxDrawStream diob, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Rasterization.NextPass(diob, useAuxScripts);
}
