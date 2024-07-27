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

// A draw context is created as a connection to a draw device.

#ifndef AVX_DRAW_CONTEXT_H
#define AVX_DRAW_CONTEXT_H

#include "qwadro/draw/afxBuffer.h"
#include "qwadro/draw/afxRaster.h"
#include "qwadro/draw/pipe/avxLigature.h"
#include "qwadro/draw/pipe/avxCanvas.h"
#include "qwadro/draw/pipe/avxPipeline.h"
#include "qwadro/draw/pipe/avxRasterizer.h"
#include "qwadro/draw/pipe/avxSampler.h"
#include "qwadro/draw/avxShader.h"
#include "qwadro/draw/pipe/avxVertexInput.h"
#include "qwadro/draw/pipe/avxDrawOps.h"
#include "qwadro/draw/pipe/avxQueryPool.h"
#include "qwadro/draw/dev/afxDrawOutput.h"
#include "qwadro/draw/dev/afxDrawInput.h"
#include "qwadro/draw/math/avxMatrix.h"
#include "qwadro/draw/dev/afxDrawDevice.h"
#include "qwadro/base/afxClass.h"
#include "qwadro/mem/afxArena.h"

AFX_DEFINE_STRUCT(afxDrawContextConfig)
{
    afxDrawBridgeConfig mainBridge;
    afxNat              auxBridgeCnt;
    afxDrawBridgeConfig*auxBridges;
    
    afxDrawDeviceCaps   requirements;
    void*               udd;
    afxNat              extensionCnt;
    afxString const*    extensions;
};

AVX afxDrawDevice       AfxGetDrawContextDevice(afxDrawContext dctx);

AVX afxNat              AfxCountDrawBridges(afxDrawContext dctx);
AVX afxNat              AfxCountDrawQueues(afxDrawContext dctx, afxNat portIdx);

AVX afxBool             AfxGetDrawBridge
/// Get a bridge handle from a device context.
(
    afxDrawContext      dctx,
    afxNat              portIdx, /// the index of the bridge.
    afxDrawBridge*      bridge /// a pointer to a afxDrawBridge object that will be filled with the handle for the requested bridge.
);

AVX afxBool             AfxGetDrawQueue
/// Get a queue handle from a device context.
(
    afxDrawContext      dctx,
    afxNat              portIdx, /// the index of the bridge to which the queue belongs.
    afxNat              queIdx, /// the index within this bridge of the queue to retrieve.
    afxDrawQueue*       queue /// a pointer to a afxDrawQueue object that will be filled with the handle for the requested queue.
);

AVX afxError            AfxWaitForDrawContext
/// Wait for a device context to become idle.
(
    afxDrawContext      dctx
);

AVX afxError            AfxWaitForIdleDrawBridge
/// Wait for a bridge to become idle. To wait on the host for the completion of outstanding queue operations for a given bridge.
(
    afxDrawContext      dctx,
    afxNat              portIdx /// the bridge on which to wait.
);

AVX afxError            AfxWaitForIdleDrawQueue
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.
(
    afxDrawContext      dctx,
    afxNat              portIdx, /// the bridge on which to wait.
    afxNat              queIdx /// the queue on which to wait.
);

AVX void            AfxCountDrawContextConnections(afxDrawContext dctx, afxNat* inputCnt, afxNat* outputCnt);

AVX afxBool         AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat conNo, afxDrawInput* input);
AVX afxBool         AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat conNo, afxDrawOutput* output);

AVX afxNat          AfxInvokeConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AVX afxNat          AfxInvokeConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxDrawOutput, void*), void *udd);

AVX afxNat          AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxDrawInput inputs[]);
AVX afxNat          AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxDrawOutput outputs[]);

AVX afxError        AfxDisconnectDrawContext(afxDrawContext dctx, afxBool keepInputs, afxBool keepOutputs);

AVX afxClass*       AfxGetLigatureClass(afxDrawContext dctx);
AVX afxClass*       AfxGetBufferClass(afxDrawContext dctx);
AVX afxClass*       AfxGetCanvasClass(afxDrawContext dctx);
AVX afxClass*       AfxGetPipelineClass(afxDrawContext dctx);
AVX afxClass*       AfxGetQueryPoolClass(afxDrawContext dctx);
AVX afxClass*       AfxGetRasterClass(afxDrawContext dctx);
AVX afxClass*       AfxGetRasterizerClass(afxDrawContext dctx);
AVX afxClass*       AfxGetSamplerClass(afxDrawContext dctx);
AVX afxClass*       AfxGetShaderClass(afxDrawContext dctx);
AVX afxClass*       AfxGetVertexInputClass(afxDrawContext dctx);

AVX afxBool         AfxGetShaderStringBase(afxDrawContext dctx, afxStringBase* base);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxConfigureDrawDevice(afxNat ddevId, afxDrawContextConfig* cfg);

AVX afxError        AfxOpenDrawDevice(afxNat ddevId, afxDrawContextConfig const* cfg, afxDrawContext* context);


#endif//AVX_DRAW_CONTEXT_H
