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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_DRAW_CONTEXT_H
#define AVX_DRAW_CONTEXT_H

#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/draw/pipe/afxBindSchema.h"
#include "qwadro/draw/pipe/afxCanvas.h"
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
    void const*         enabledFeatures;

    afxNat              brigeCnt;
    afxDrawBridgeConfig const* bridges;

    afxNat              maxBufCnt;
    afxNat              maxSmpCnt;
    afxNat              maxTexCnt;
    afxNat              maxCanvCnt;
    afxNat              maxPipCnt;
    afxNat              maxLegoCnt;
    afxNat              maxShdCnt;

    afxSize const*      attrs[2];
};

#ifdef _AVX_DRAW_C
#ifdef _AVX_DRAW_CONTEXT_C
#ifdef _AVX_DRAW_CONTEXT_IMPL
struct _afxBaseDrawContext    
#else
AFX_OBJECT(afxDrawContext)
#endif
{
    AFX_OBJECT(afxContext) ctx;
    afxBool             running;
    afxArena            aren;
    
    afxNat              ownedBridgeCnt;
    afxDrawBridge*      ownedBridges;

    afxChain            inputs;
    afxChain            outputs;

    //afxChain            classes;
    afxManager          queries;
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

    afxError            (*waitCb)(afxDrawContext);

    struct _afxDctxIdd* idd;
    void*               udd; // user-defined data
};

#ifdef _AVX_DRAW_CONTEXT_IMPL
AFX_STATIC_ASSERT(offsetof(struct _afxBaseDrawContext, ctx) == 0x00, "");
#else
AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxDrawContext), ctx) == 0x00, "");
#endif

#endif//_AVX_DRAW_CONTEXT_C
#endif//_AVX_DRAW_C

AVX afxDrawDevice       AfxGetDrawContextDevice(afxDrawContext dctx);

AVX afxNat              AfxCountDrawBridges(afxDrawContext dctx);
AVX afxDrawBridge       AfxGetDrawBridge(afxDrawContext dctx, afxNat portIdx);

AVX afxError            AfxWaitForIdleDrawContext(afxDrawContext dctx);

AVX afxNat              AfxCountDrawInputConnections(afxDrawContext dctx);
AVX afxNat              AfxCountDrawOutputConnections(afxDrawContext dctx);
AVX afxBool             AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat conNo, afxDrawInput* input);
AVX afxBool             AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat conNo, afxDrawOutput* output);
AVX afxNat              AfxInvokeConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AVX afxNat              AfxInvokeConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AVX afxNat              AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxDrawInput inputs[]);
AVX afxNat              AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxDrawOutput outputs[]);
AVX afxError            AfxDisconnectDrawContext(afxDrawContext dctx, afxBool inputs, afxBool outputs);

AVX afxManager*         AfxGetBindSchemaClass(afxDrawContext dctx);
AVX afxManager*         AfxGetBufferClass(afxDrawContext dctx);
AVX afxManager*         AfxGetCanvasClass(afxDrawContext dctx);
AVX afxManager*         AfxGetPipelineClass(afxDrawContext dctx);
AVX afxManager*         AfxGetQueryPoolClass(afxDrawContext dctx);
AVX afxManager*         AfxGetRasterClass(afxDrawContext dctx);
AVX afxManager*         AfxGetRasterizerClass(afxDrawContext dctx);
AVX afxManager*         AfxGetSamplerClass(afxDrawContext dctx);
AVX afxManager*         AfxGetShaderClass(afxDrawContext dctx);
AVX afxManager*         AfxGetVertexInputClass(afxDrawContext dctx);

////////////////////////////////////////////////////////////////////////////////

AVX afxError            AfxAcquireDrawContext(afxNat ddevId, afxDrawContextConfig const* cfg, afxDrawContext* context);


#endif//AVX_DRAW_CONTEXT_H
