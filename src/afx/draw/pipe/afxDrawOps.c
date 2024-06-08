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

_AVX afxCmdId AvxCmdBindPipeline(avxCmdb cmdb, afxNat segment, afxPipeline pip, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    /// pip must be a valid afxPipeline handle.
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    return cmdb->stdCmds->BindPipeline(cmdb, segment, pip, dynamics);
}

_AVX afxCmdId AvxCmdBindRasterizer(avxCmdb cmdb, afxRasterizer razr, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    /// razr must be a valid afxRasterizer handle.
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    return cmdb->stdCmds->BindRasterizer(cmdb, razr, dynamics);
}

_AVX afxCmdId AvxCmdBindBuffers(avxCmdb cmdb, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssertRange(8, baseIdx, cnt);
    return cmdb->stdCmds->BindBuffers(cmdb, set, baseIdx, cnt, buf, offset, range);
}

_AVX afxCmdId AvxCmdBindRasters(avxCmdb cmdb, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster tex[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssertRange(8, baseIdx, cnt);
    return cmdb->stdCmds->BindRasters(cmdb, set, baseIdx, cnt, smp, tex);
}

_AVX afxCmdId AvxCmdExecuteCommands(avxCmdb cmdb, afxNat cnt, avxCmdb aux[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    return cmdb->stdCmds->ExecuteCommands(cmdb, cnt, aux);
}

// Draw

_AVX afxCmdId AvxCmdDraw(avxCmdb cmdb, afxNat baseInstIdx, afxNat instCnt, afxNat baseVtxIdx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssert(vtxCnt);
    //AfxAssert(instCnt);
    return cmdb->stdCmds->Draw(cmdb, vtxCnt, instCnt, baseVtxIdx, baseInstIdx);
}

_AVX afxCmdId AvxCmdDrawIndirect(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(drawCnt);
    AfxAssert(stride);
    return cmdb->stdCmds->DrawIndirect(cmdb, buf, offset, drawCnt, stride);
}

_AVX afxCmdId AvxCmdDrawIndirectCount(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);
    AfxAssert(cntBuf);
    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    return cmdb->stdCmds->DrawIndirectCount(cmdb, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}

_AVX afxCmdId AvxCmdDrawIndexed(avxCmdb cmdb, afxNat vtxOff, afxNat baseInstIdx, afxNat instCnt, afxNat baseIdx, afxNat idxCnt)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssert(idxCnt);
    //AfxAssert(instCnt);
    return cmdb->stdCmds->DrawIndexed(cmdb, idxCnt, instCnt, baseIdx, vtxOff, baseInstIdx);
}

_AVX afxCmdId AvxCmdDrawIndexedIndirect(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssert(drawCnt);
    AfxAssert(stride);
    return cmdb->stdCmds->DrawIndexedIndirect(cmdb, buf, offset, drawCnt, stride);
}

_AVX afxCmdId AvxCmdDrawIndexedIndirectCount(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);
    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    return cmdb->stdCmds->DrawIndexedIndirectCount(cmdb, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}











_AVX afxCmdId AvxCmdBindFontSIG(avxCmdb cmdb, afxNat first, afxNat cnt, afxTypography typ[], afxPipeline pip[], afxSampler smp[], afxRaster ras[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    AfxAssert(first == 0);
    AfxAssert(cnt == 1);
    AvxCmdBindPipeline(cmdb, 0, pip[0], NIL);
    AvxCmdBindRasters(cmdb, 0, 1, 1, smp, ras);
    return 0;
}
