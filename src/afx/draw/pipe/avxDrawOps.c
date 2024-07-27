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
#include "../dev/AvxDevKit.h"


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

_AVX afxCmdId AvxCmdBindPipeline(avxCmdb cmdb, afxNat segment, avxPipeline pip, avxVertexInput vin, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// pip must be a valid avxPipeline handle.
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    return cmdb->stdCmds->BindPipeline(cmdb, segment, pip, vin, dynamics);
}

_AVX afxCmdId AvxCmdBindRasterizer(avxCmdb cmdb, avxRasterizer razr, avxVertexInput vin, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);

    /// razr must be a valid avxRasterizer handle.
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    return cmdb->stdCmds->BindRasterizer(cmdb, razr, vin, dynamics);
}

_AVX afxCmdId AvxCmdBindBuffers(avxCmdb cmdb, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buffers[], afxNat offsets[], afxNat ranges[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    AfxAssertRange(8, baseIdx, cnt);
    return cmdb->stdCmds->BindBuffers(cmdb, set, baseIdx, cnt, buffers, offsets, ranges);
}

_AVX afxCmdId AvxCmdBindRasters(avxCmdb cmdb, afxNat set, afxNat baseIdx, afxNat cnt, afxRaster rasters[], afxNat const subIdx[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    AfxAssertRange(8, baseIdx, cnt);
    return cmdb->stdCmds->BindRasters(cmdb, set, baseIdx, cnt, rasters, subIdx);
}

_AVX afxCmdId AvxCmdBindSamplers(avxCmdb cmdb, afxNat set, afxNat baseIdx, afxNat cnt, avxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    AfxAssertRange(8, baseIdx, cnt);
    return cmdb->stdCmds->BindSamplers(cmdb, set, baseIdx, cnt, samplers);
}

_AVX afxCmdId AvxCmdExecuteCommands(avxCmdb cmdb, afxNat cnt, avxCmdb aux[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    return cmdb->stdCmds->ExecuteCommands(cmdb, cnt, aux);
}

// Draw

_AVX afxCmdId AvxCmdDraw(avxCmdb cmdb, afxNat vtxCnt, afxNat instCnt, afxNat firstVtxIdx, afxNat firstInstIdx)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called inside of a render pass instance.
    AfxAssert(cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    AfxAssert(vtxCnt);
    //AfxAssert(instCnt);
    return cmdb->stdCmds->Draw(cmdb, vtxCnt, instCnt, firstVtxIdx, firstInstIdx);
}

_AVX afxCmdId AvxCmdDrawIndirect(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called inside of a render pass instance.
    AfxAssert(cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
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
    /// This command must only be called inside of a render pass instance.
    AfxAssert(cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    /// cntBuf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);

    AfxAssert(cntBuf);
    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    return cmdb->stdCmds->DrawIndirectCount(cmdb, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}

_AVX afxCmdId AvxCmdDrawIndexed(avxCmdb cmdb, afxNat idxCnt, afxNat instCnt, afxNat firstIdx, afxNat vtxOff, afxNat firstInstIdx)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called inside of a render pass instance.
    AfxAssert(cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    AfxAssert(idxCnt);
    //AfxAssert(instCnt);
    return cmdb->stdCmds->DrawIndexed(cmdb, idxCnt, instCnt, firstIdx, vtxOff, firstInstIdx);
}

_AVX afxCmdId AvxCmdDrawIndexedIndirect(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called inside of a render pass instance.
    AfxAssert(cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);

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
    /// This command must only be called inside of a render pass instance.
    AfxAssert(cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    /// cntBuf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);

    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    return cmdb->stdCmds->DrawIndexedIndirectCount(cmdb, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}

_AVX afxCmdId AvxCmdDispatch(avxCmdb cmdb, afxNat grpCntX, afxNat grpCntY, afxNat grpCntZ)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    return cmdb->stdCmds->Dispatch(cmdb, grpCntX, grpCntY, grpCntZ);
}

_AVX afxCmdId AvxCmdDispatchIndirect(avxCmdb cmdb, afxBuffer buf, afxNat32 offset)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    return cmdb->stdCmds->DispatchIndirect(cmdb, buf, offset);
}







_AVX afxCmdId AvxCmdBindFontSIG(avxCmdb cmdb, afxNat first, afxNat cnt, afxTypography typ[], avxPipeline pip[], avxSampler smp[], afxRaster ras[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    AfxAssert(first == 0);
    AfxAssert(cnt == 1);
    AvxCmdBindPipeline(cmdb, 0, pip[0], NIL, NIL);
    AvxCmdBindSamplers(cmdb, 0, 1, 1, smp);
    AvxCmdBindRasters(cmdb, 0, 1, 1, ras, NIL);
    return 0;
}
