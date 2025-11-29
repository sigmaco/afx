/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_IMPL___EXECUTOR_H
#define AVX_IMPL___EXECUTOR_H

#include "../avxBridgeDDK.h"

AFX_DEFINE_STRUCT(avxDpu)
{
    afxUnit         portId; // exuIdx
    afxUnit         exuIdx;
    afxDrawBridge   dexu;
    afxThread       thr;
    afxDrawSystem   dsys;

    afxBool         running;
    afxBool         instanced;
    afxSize         numOfFedVertices;
    afxSize         numOfFedIndices;
    afxSize         numOfFedInstances;

    // draw scope
    afxBool         inDrawScope;
    avxCanvas           canv;
    afxRect             area; //  the area that is affected by the draw scope.
    afxUnit             baseLayer; // the index of the first attachment layer that will be drawn.
    afxUnit             layerCnt; // the number of layers drawn to in each attachment when viewMask is 0.
    afxUnit             targetCnt; // the number of video surfaces.
    avxDrawTarget       targets[AVX_MAX_CANVAS_BUFFERS]; // structures describing any color attachments used.
    afxBool             useDepth;
    avxDrawTarget       depth; // structure describing a depth attachment.
    afxBool             useStencil;
    avxDrawTarget       stencil; // structure describing a stencil attachment.

    // transform scope
    afxUnit     vpCnt;
    avxViewport vps[AVX_MAX_VIEWPORTS];
    afxRect     scissors[AVX_MAX_VIEWPORTS];

    avxBuffer   ibo;
    afxSize     iboBase;
    afxUnit     iboRange;
    afxUnit     iboStride; // size/type of vertex index.
    void*       iboPtr;
};

AVX afxClass const* _AvxGetDrawThreadClass(afxDrawBridge dexu);
AVX afxClassConfig const _AVX_DTHR_CLASS_CONFIG;

AVX _avxCmdLut const _AVX_DPU_CMD_VMT;

AVX afxError _AvxDpuWork_CallbackCb(avxDpu* dpu, _avxIoReqPacket* work);
AVX afxError _AvxDpuWork_ExecuteCb(avxDpu* dpu, _avxIoReqPacket* work);

AVX afxInt   _AVX_DPU_THREAD_PROC(afxDrawBridge dexu);
AVX afxBool  _AvxDpu_ProcCb(avxDpu* dpu);

AVX afxError _AvxDpuRollContext(avxDpu* dpu, afxDrawContext dctx, afxUnit batchId);

#endif//AVX_IMPL___EXECUTOR_H
