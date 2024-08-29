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
#include "../../dev/AvxImplKit.h"

_AVX afxCmdId AvxCmdBindVertexInput(avxCmdb cmdb, avxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    AfxTryAssertObjects(1, &vin, afxFcc_VIN);
    return cmdb->stdCmds->xfrm.BindVertexInput(cmdb, vin);
}

_AVX afxCmdId AvxCmdSwitchFrontFace(avxCmdb cmdb, afxBool cw)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);
    
    AfxAssertRange(cw, FALSE, TRUE);
    cw = AfxClamp(cw, FALSE, TRUE);

    return cmdb->stdCmds->xfrm.SwitchFrontFace(cmdb, cw);
}

_AVX afxCmdId AvxCmdSetCullMode(avxCmdb cmdb, avxCullMode mode)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// mode must be a valid combination of avxCullMode values.
    AfxAssertBounds(mode, avxCullMode_NONE, avxCullMode_BOTH);
    mode = AfxClamp(mode, avxCullMode_NONE, avxCullMode_BOTH);

    return cmdb->stdCmds->xfrm.SetCullMode(cmdb, mode);
}

_AVX afxCmdId AvxCmdAdjustViewports(avxCmdb cmdb, afxNat baseIdx, afxNat cnt, afxViewport const viewports[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// cnt must be greater than 0.
    AfxAssert(cnt);

    /// viewports must be a valid pointer to an array of cnt valid afxViewport structures.
    AfxAssert(viewports);    
    AfxAssertRange(8, baseIdx, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat vpIdx = /*baseIdx +*/ i;
        afxViewport const* vp = &viewports[vpIdx];
        AfxAssert(vp->extent[0]);
        AfxAssert(vp->extent[1]);
        AfxAssertBounds(vp->depth[0], 0.0, 1.0);
        AfxAssertBounds(vp->depth[1], 0.0, 1.0);
    }
    return cmdb->stdCmds->xfrm.AdjustViewports(cmdb, baseIdx, cnt, viewports);
}

_AVX afxCmdId AvxCmdBindVertexSources(avxCmdb cmdb, afxNat baseSlotIdx, afxNat slotCnt, afxBuffer buffers[], afxNat32 const offsets[], afxNat32 const ranges[], afxNat32 const strides[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    AfxAssertRange(8, baseSlotIdx, slotCnt);
    AfxAssert(strides);
    
    for (afxNat i = 0; i < slotCnt; i++)
    {
        if (buffers)
        {
            afxBuffer buf = buffers[i];

            if (buf)
            {
                /// If buf is not NIL, buffer must be a valid afxBuffer handle.
                AfxAssertObjects(1, &buf, afxFcc_BUF);

                /// All elements of buffers must have been created with the afxBufferUsage_VERTEX flag.
                AfxAssert(AfxTestBufferUsage(buf, afxBufferUsage_VERTEX));

                afxNat bufCap = AfxGetBufferCapacity(buf, 0);

                /// If sizes is not NIL, all elements of offsets must be less than the size of the corresponding element in buffers.
                /// If sizes is not NIL, all elements of offsets plus pSizes , where sizes is not zero, must be less than or equal to the size of the corresponding element in buffers.
                afxNat offset = offsets ? offsets[i] : 0;
                afxNat range = ranges ? ranges[i] : bufCap - offset;
                AfxAssertRange(bufCap, offset, range);
                AfxAssert(range);

                AfxAssert(strides && strides[i]);
            }
        }
        else
        {
            /// If an element of buffers is NIL, then the corresponding element of offsets must be zero.
            AfxAssert(!ranges || !ranges[i]);
            AfxAssert(!offsets || !offsets[i]);
            AfxAssert(!strides || !strides[i]);
        }
    }
    return cmdb->stdCmds->xfrm.BindVertexSources(cmdb, baseSlotIdx, slotCnt, buffers, offsets, ranges, strides);
}

_AVX afxCmdId AvxCmdBindIndexSource(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);
    
    /// idxSiz must not be zero.
    AfxAssert(idxSiz);

    if (!buf)
    {
        AfxAssert(!offset);
        AfxAssert(!range);
        offset = 0;
        range = 0;
    }
    else
    {
        /// If buf is not NIL, buffer must be a valid afxBuffer handle.
        AfxAssertObjects(1, &buf, afxFcc_BUF);

        /// buffer must have been created with the afxBufferUsage_INDEX flag.
        AfxAssert(AfxTestBufferUsage(buf, afxBufferUsage_INDEX));

        /// offset must be less than the size of buffer.
        afxNat bufCap = AfxGetBufferCapacity(buf, 0);
        AfxAssertRange(bufCap, offset, range);
        AfxAssert(range);

        offset = AfxClamp(offset, 0, bufCap - 1);
        range = AfxClamp(range, 0, bufCap - offset);
    }
    return cmdb->stdCmds->xfrm.BindIndexSource(cmdb, buf, offset, range, idxSiz);
}

_AVX afxCmdId AvxCmdSetPrimitiveTopology(avxCmdb cmdb, avxTopology topology)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// topology must be a valid avxTopology value.
    AfxAssert(topology < avxTopology_TOTAL);

    return cmdb->stdCmds->xfrm.SetPrimitiveTopology(cmdb, topology);
}
