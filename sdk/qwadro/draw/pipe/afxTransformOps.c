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

_AVX afxCmdId AfxCmdBindVertexInput(afxDrawStream diob, afxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    return diob->stdCmds->Transformation.BindVertexInput(diob, vin);
}

_AVX afxCmdId AfxCmdSwitchFrontFace(afxDrawStream diob, afxBool cw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Transformation.SwitchFrontFace(diob, cw);
}

_AVX afxCmdId AfxCmdSetCullMode(afxDrawStream diob, afxCullMode mode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->stdCmds->Transformation.SetCullMode(diob, mode);
}

_AVX afxCmdId AfxCmdResetViewports(afxDrawStream diob, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssert(!cnt || vp);
    return diob->stdCmds->Transformation.ResetViewports(diob, cnt, vp);
}

_AVX afxCmdId AfxCmdReadjustViewports(afxDrawStream diob, afxNat baseIdx, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertRange(8, baseIdx, cnt);
    AfxAssert(!cnt || vp);
    return diob->stdCmds->Transformation.ReadjustViewports(diob, baseIdx, cnt, vp);
}

_AVX afxCmdId AfxCmdBindVertexSources(afxDrawStream diob, afxNat baseSlotIdx, afxNat slotCnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[], afxNat32 const stride[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertRange(8, baseSlotIdx, slotCnt);
    AfxAssert(stride);
    
    for (afxNat i = 0; i < slotCnt; i++)
    {
        AfxAssert(stride[i]);
    }
    return diob->stdCmds->Transformation.BindVertexSources(diob, baseSlotIdx, slotCnt, buf, offset, range, stride);
}

#if 0
_AVX afxCmdId AfxCmdResetVertexStreams(afxDrawStream diob, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertRange(8, 0, cnt);
    return diob->stdCmds->ResetVertexStreams(diob, cnt, srcIdx, stride, instance);
}

_AVX afxCmdId AfxCmdResetVertexAttributes(afxDrawStream diob, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const srcIdx[], afxNat32 const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertRange(8, 0, cnt);
    return diob->stdCmds->ResetVertexAttributes(diob, cnt, location, fmt, srcIdx, offset);
}
#endif

_AVX afxCmdId AfxCmdBindIndexSource(afxDrawStream diob, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, range);
    AfxAssert(range);
    AfxAssert(idxSiz);
    return diob->stdCmds->Transformation.BindIndexSource(diob, buf, offset, range, idxSiz);
}

_AVX afxCmdId AfxCmdSetPrimitiveTopology(afxDrawStream diob, afxPrimTopology topology)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssert(topology < afxPrimTopology_TOTAL);
    return diob->stdCmds->Transformation.SetPrimitiveTopology(diob, topology);
}
