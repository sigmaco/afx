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

#ifndef AFX_DRAW_CONTEXT_H
#define AFX_DRAW_CONTEXT_H

#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/draw/pipe/afxBindSchema.h"
#include "qwadro/draw/pipe/afxCanvas.h"
#include "qwadro/draw/dev/afxFence.h"
#include "qwadro/draw/dev/afxSemaphore.h"
#include "qwadro/draw/pipe/afxPipeline.h"
#include "qwadro/draw/pipe/afxRasterizer.h"
#include "qwadro/draw/pipe/afxSampler.h"
#include "qwadro/draw/io/afxShader.h"
#include "qwadro/draw/pipe/afxVertexInput.h"
#include "qwadro/draw/pipe/afxDrawOps.h"
#include "qwadro/draw/pipe/afxQueryPool.h"
#include "qwadro/draw/dev/afxDrawOutput.h"
#include "qwadro/draw/dev/afxDrawInput.h"
#include "qwadro/math/afxOpticalMatrix.h"
#include "qwadro/core/afxManager.h"
#include "qwadro/mem/afxArena.h"

AFX_DEFINE_STRUCT(afxDrawContextConfig)
{
    void const*    enabledFeatures;

    afxNat                      queueCnt;
    afxDrawBridgeConfig const*   queues;

    afxNat                      maxBufCnt;
    afxNat                      maxSmpCnt;
    afxNat                      maxTexCnt;
    afxNat                      maxCanvCnt;
    afxNat                      maxPipCnt;
    afxNat                      maxLegoCnt;
    afxNat                      maxShdCnt;

    afxSize const*              attrs[2];
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_CONTEXT_C
#ifndef _AFX_DRAW_CONTEXT_IMPL
AFX_OBJECT(afxDrawContext)
#else
struct afxBaseDrawContext
#endif
{
    afxBool             running;
    afxArena            aren;
    
    afxChain            ownedBridges;
    afxChain            ownedSemaphores;
    afxChain            ownedFences;
    afxChain            ownedQueries;
    afxChain            inputs;
    afxChain            outputs;

    afxChain            classes;
    afxManager          buffers;
    afxManager          rasters;
    afxManager          shaders;

    afxManager          rasterizers;
    afxManager          pipelines;
    afxManager          schemas;
    afxManager          canvases;
    afxManager          samplers;
    afxManager          vinputs;

    afxCmd const*       stdCmds;

    afxClipSpace        clipCfg;

    afxError            (*waitFenc)(afxBool,afxNat64,afxNat,const afxFence[]);
    afxError            (*resetFenc)(afxNat,afxFence const[]);
    
    struct _afxDctxIdd* idd;
    void*               udd; // user-defined data
};
#endif
#endif

AVX afxDrawDevice       AfxGetDrawContextDevice(afxDrawContext dctx);

AVX afxError            AfxRequestNextDrawQueue(afxDrawContext dctx, afxNat portIdx, afxTime timeout, afxNat *queIdx);
AVX afxNat              AfxCountDrawBridges(afxDrawContext dctx);
AVX afxDrawBridge       AfxGetDrawBridge(afxDrawContext dctx, afxNat portIdx);

AVX afxNat              AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxDrawInput inputs[]);
AVX afxNat              AfxInvokeConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AVX afxNat              AfxCountDrawInputConnections(afxDrawContext dctx);
AVX afxDrawInput        AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat conNo);
AVX afxError            AfxDisconnectDrawInputs(afxDrawContext dctx);

AVX afxNat              AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxDrawOutput outputs[]);
AVX afxNat              AfxInvokeConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AVX afxNat              AfxCountDrawOutputConnections(afxDrawContext dctx);
AVX afxDrawOutput       AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat conNo);
AVX afxError            AfxDisconnectDrawOutputs(afxDrawContext dctx);

AVX afxManager*         AfxGetBindSchemaClass(afxDrawContext dctx);
AVX afxManager*         AfxGetBufferClass(afxDrawContext dctx);
AVX afxManager*         AfxGetCanvasClass(afxDrawContext dctx);
AVX afxManager*         AfxGetPipelineClass(afxDrawContext dctx);
AVX afxManager*         AfxGetRasterClass(afxDrawContext dctx);
AVX afxManager*         AfxGetRasterizerClass(afxDrawContext dctx);
AVX afxManager*         AfxGetSamplerClass(afxDrawContext dctx);
AVX afxManager*         AfxGetShaderClass(afxDrawContext dctx);
AVX afxManager*         AfxGetVertexInputClass(afxDrawContext dctx);

AVX afxNat              AfxEnumerateBindSchemas(afxDrawContext dctx, afxNat first, afxNat cnt, afxBindSchema schemas[]);
AVX afxNat              AfxEnumerateBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxBuffer bufffers[]);
AVX afxNat              AfxEnumerateCanvases(afxDrawContext dctx, afxNat first, afxNat cnt, afxCanvas canvases[]);
AVX afxNat              AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pipelines[]);
AVX afxNat              AfxEnumerateRasters(afxDrawContext dctx, afxNat first, afxNat cnt, afxRaster rasters[]);
AVX afxNat              AfxEnumerateSamplers(afxDrawContext dctx, afxNat first, afxNat cnt, afxSampler samplers[]);
AVX afxNat              AfxEnumerateShaders(afxDrawContext dctx, afxNat first, afxNat cnt, afxShader shaders[]);

////////////////////////////////////////////////////////////////////////////////

AVX afxError            AfxAcquireDrawContexts(afxNat ddevId, afxNat cnt, afxDrawContextConfig const cfg[], afxDrawContext contexts[]);


#endif//AFX_DRAW_CONTEXT_H
