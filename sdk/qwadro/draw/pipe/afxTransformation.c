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

_AVX afxCmdId AfxCmdBindVertexInput(afxDrawScript dscr, afxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    return dscr->stdCmds->Transformation.BindVertexInput(dscr, vin);
}

_AVX afxCmdId AfxCmdSwitchFrontFace(afxDrawScript dscr, afxBool cw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Transformation.SwitchFrontFace(dscr, cw);
}

_AVX afxCmdId AfxCmdSetCullMode(afxDrawScript dscr, afxCullMode mode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Transformation.SetCullMode(dscr, mode);
}

_AVX afxCmdId AfxCmdResetViewports(afxDrawScript dscr, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(!cnt || vp);
    return dscr->stdCmds->Transformation.ResetViewports(dscr, cnt, vp);
}

_AVX afxCmdId AfxCmdReadjustViewports(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    AfxAssert(!cnt || vp);
    return dscr->stdCmds->Transformation.ReadjustViewports(dscr, baseIdx, cnt, vp);
}

_AVX afxCmdId AfxCmdBindVertexSources(afxDrawScript dscr, afxNat baseSlotIdx, afxNat slotCnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[], afxNat32 const stride[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseSlotIdx, slotCnt);
    AfxAssert(stride);
    
    for (afxNat i = 0; i < slotCnt; i++)
    {
        AfxAssert(stride[i]);
    }
    return dscr->stdCmds->Transformation.BindVertexSources(dscr, baseSlotIdx, slotCnt, buf, offset, range, stride);
}

#if 0
_AVX afxCmdId AfxCmdResetVertexStreams(afxDrawScript dscr, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, 0, cnt);
    return dscr->stdCmds->ResetVertexStreams(dscr, cnt, srcIdx, stride, instance);
}

_AVX afxCmdId AfxCmdResetVertexAttributes(afxDrawScript dscr, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const srcIdx[], afxNat32 const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, 0, cnt);
    return dscr->stdCmds->ResetVertexAttributes(dscr, cnt, location, fmt, srcIdx, offset);
}
#endif

_AVX afxCmdId AfxCmdBindIndexSource(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, range);
    AfxAssert(range);
    AfxAssert(idxSiz);
    return dscr->stdCmds->Transformation.BindIndexSource(dscr, buf, offset, range, idxSiz);
}

_AVX afxCmdId AfxCmdSetPrimitiveTopology(afxDrawScript dscr, afxPrimTopology topology)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(topology < afxPrimTopology_TOTAL);
    return dscr->stdCmds->Transformation.SetPrimitiveTopology(dscr, topology);
}
