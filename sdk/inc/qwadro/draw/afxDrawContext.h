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

// This section is part of SIGMA GL/2.

#ifndef AFX_DRAW_CONTEXT_H
#define AFX_DRAW_CONTEXT_H

#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/draw/pipe/afxBindSchema.h"
#include "qwadro/draw/pipe/afxCanvas.h"
#include "qwadro/draw/pipe/afxFence.h"
#include "qwadro/draw/pipe/afxSemaphore.h"
#include "qwadro/draw/pipe/afxPipeline.h"
#include "qwadro/draw/pipe/afxRasterizer.h"
#include "qwadro/draw/pipe/afxSampler.h"
#include "qwadro/draw/pipe/afxShader.h"
#include "qwadro/draw/pipe/afxVertexInput.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"
#include "qwadro/draw/pipe/afxQueryPool.h"
#include "qwadro/draw/afxDrawOutput.h"

AFX_DEFINE_STRUCT(afxDrawQueueingConfig)
{
    afxNat                  portIdx;
    afxNat                  minQueueCnt;
};

AFX_DEFINE_STRUCT(afxDrawContextConfig)
{
    void const*    enabledFeatures;

    afxNat                      queueingCnt;
    afxDrawQueueingConfig const*queueings;

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
    afxMmu              mmu;
    afxArena            aren;
    
    struct
    {
        
        //afxSlock            recyclSubmChainSlock;
        //afxChain            recyclSubmChain;
        //afxNat              minRecyclSubmCnt;
        afxNat          lastReqQueIdx;
        //afxClass        queues;
        //afxClass        scripts;
        afxNat          dqueCnt;
        afxDrawQueue*   queues;
    }*                  openPorts;
    afxNat              openPortCnt;
    afxNat              lastReqPortIdx;

    struct
    {
        afxDrawOutput   dout;
    }*                  doutSlots;
    afxNat              doutSlotCnt;
    struct
    {
        afxDrawInput    din;
    }*                  dinSlots;
    afxNat              dinSlotCnt;

    afxChain            classes;
    afxClass            rasters;
    afxClass            buffers;
    afxClass            shaders;
    afxClass            fences;
    afxClass            semaphores;
    afxClass            rasterizers;
    afxClass            pipelines;
    afxClass            schemas;
    afxClass            queries;
    afxClass            canvases;
    afxClass            samplers;
    afxClass            vinputs;

    afxCmd const*       stdCmds;

    afxClipSpace        clipCfg;

    afxError            (*waitFenc)(afxBool,afxNat64,afxNat,afxFence[]);
    afxError            (*resetFenc)(afxNat,afxFence[]);
    afxError            (*executeCb)(afxDrawContext, afxNat, afxExecutionRequest const[],afxFence);
    afxError            (*presentCb)(afxDrawContext,afxNat,afxPresentationRequest const[]);
    afxError            (*stampCb)(afxDrawContext,afxNat,afxPresentationRequest const[],afxV2d const,afxString const*);
};
#endif
#endif

AVX afxDrawDevice       AfxGetDrawContextDevice(afxDrawContext dctx);
AVX afxMmu              AfxGetDrawContextMmu(afxDrawContext dctx);

AVX afxNat              AfxGetDrawQueueCount(afxDrawContext dctx, afxNat portIdx);
AVX afxDrawQueue        AfxGetDrawQueue(afxDrawContext dctx, afxNat portIdx, afxNat queIdx);
AVX afxError            AfxRequestNextDrawQueue(afxDrawContext dctx, afxNat portIdx, afxTime timeout, afxNat *queIdx);

AVX afxNat              AfxCountDrawInputConnections(afxDrawContext dctx);
AVX afxNat              AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawInput inputs[]);
AVX afxNat              AfxInvokeConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AVX afxDrawInput        AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat no);
AVX afxError            AfxDisconnectDrawInputs(afxDrawContext dctx);

AVX afxNat              AfxCountDrawOutputConnections(afxDrawContext dctx);
AVX afxNat              AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOutput outputs[]);
AVX afxNat              AfxInvokeConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AVX afxDrawOutput       AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat no);
AVX afxError            AfxDisconnectDrawOutputs(afxDrawContext dctx);

AVX afxClass*           AfxGetBindSchemaClass(afxDrawContext dctx);
AVX afxClass*           AfxGetBufferClass(afxDrawContext dctx);
AVX afxClass*           AfxGetCanvasClass(afxDrawContext dctx);
AVX afxClass*           AfxGetFenceClass(afxDrawContext dctx);
AVX afxClass*           AfxGetPipelineClass(afxDrawContext dctx);
AVX afxClass*           AfxGetQueryPoolClass(afxDrawContext dctx);
AVX afxClass*           AfxGetRasterClass(afxDrawContext dctx);
AVX afxClass*           AfxGetRasterizerClass(afxDrawContext dctx);
AVX afxClass*           AfxGetSamplerClass(afxDrawContext dctx);
AVX afxClass*           AfxGetSemaphoreClass(afxDrawContext dctx);
AVX afxClass*           AfxGetShaderClass(afxDrawContext dctx);
AVX afxClass*           AfxGetVertexInputClass(afxDrawContext dctx);

AVX afxNat              AfxEnumerateBindSchemas(afxDrawContext dctx, afxNat first, afxNat cnt, afxBindSchema schemas[]);
AVX afxNat              AfxEnumerateBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxBuffer bufffers[]);
AVX afxNat              AfxEnumerateCanvases(afxDrawContext dctx, afxNat first, afxNat cnt, afxCanvas canvases[]);
AVX afxNat              AfxEnumerateFences(afxDrawContext dctx, afxNat first, afxNat cnt, afxFence fences[]);
AVX afxNat              AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pipelines[]);
AVX afxNat              AfxEnumerateQueryPools(afxDrawContext dctx, afxNat first, afxNat cnt, afxQueryPool pools[]);
AVX afxNat              AfxEnumerateRasters(afxDrawContext dctx, afxNat first, afxNat cnt, afxRaster rasters[]);
AVX afxNat              AfxEnumerateSamplers(afxDrawContext dctx, afxNat first, afxNat cnt, afxSampler samplers[]);
AVX afxNat              AfxEnumerateShaders(afxDrawContext dctx, afxNat first, afxNat cnt, afxShader shaders[]);

#endif//AFX_DRAW_CONTEXT_H
