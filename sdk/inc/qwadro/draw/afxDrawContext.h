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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_DRAW_CONTEXT_H
#define AFX_DRAW_CONTEXT_H

#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/draw/pipe/afxBindSchema.h"
#include "qwadro/draw/pipe/afxCanvas.h"
#include "qwadro/draw/pipe/afxFence.h"
#include "qwadro/draw/pipe/afxPipeline.h"
#include "qwadro/draw/pipe/afxRasterizer.h"
#include "qwadro/draw/pipe/afxSampler.h"
#include "qwadro/draw/pipe/afxShader.h"
#include "qwadro/draw/pipe/afxVertexInput.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"

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
    afxError            (*procCb)(afxDrawContext,afxDrawThread);
    afxBool             running;

    afxChain            classes;

    afxMmu              mmu;
    afxArena            aren;
    
    afxNat              lastReqPortIdx;
    afxNat              openPortCnt;
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

    afxChain            inlinks;
    afxChain            outlinks;

    afxClass            rasters;
    afxClass            buffers;
    afxClass            shaders;
    afxClass            rasterizers;
    afxClass            pipelines;
    afxClass            legos;
    afxClass            canvases;
    afxClass            samplers;
    afxClass            vinputs;

    afxCmd const*       stdCmds;
};
#endif
#endif

AFX afxDrawDevice       AfxGetDrawContextDevice(afxDrawContext dctx);
AFX afxMmu              AfxGetDrawContextMmu(afxDrawContext dctx);

AFX afxNat              AfxGetDrawQueueCount(afxDrawContext dctx, afxNat portIdx);
AFX afxDrawQueue        AfxGetDrawQueue(afxDrawContext dctx, afxNat portIdx, afxNat queIdx);
AFX afxError            AfxRequestNextDrawQueue(afxDrawContext dctx, afxNat portIdx, afxTime timeout, afxNat *queIdx);

AFX afxNat              AfxCountConnectedDrawInputs(afxDrawContext dctx);
AFX afxNat              AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawInput din[]);
AFX afxNat              AfxCurateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AFX afxDrawInput        AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat no);
AFX afxError            AfxDisconnectDrawInputs(afxDrawContext dctx);

AFX afxNat              AfxCountConnectedDrawOutputs(afxDrawContext dctx);
AFX afxNat              AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOutput dout[]);
AFX afxNat              AfxCurateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AFX afxDrawOutput       AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat no);
AFX afxError            AfxDisconnectDrawOutputs(afxDrawContext dctx);

AFX afxClass*           AfxGetBindSchemaClass(afxDrawContext dctx);
AFX afxClass*           AfxGetBufferClass(afxDrawContext dctx);
AFX afxClass*           AfxGetCanvasClass(afxDrawContext dctx);
AFX afxClass*           AfxGetPipelineClass(afxDrawContext dctx);
AFX afxClass*           AfxGetRasterClass(afxDrawContext dctx);
AFX afxClass*           AfxGetRasterizerClass(afxDrawContext dctx);
AFX afxClass*           AfxGetSamplerClass(afxDrawContext dctx);
AFX afxClass*           AfxGetShaderClass(afxDrawContext dctx);
AFX afxClass*           AfxGetVertexInputClass(afxDrawContext dctx);

AFX afxNat              AfxEnumerateBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxBuffer bufffers[]);
AFX afxNat              AfxEnumerateCanvases(afxDrawContext dctx, afxNat first, afxNat cnt, afxCanvas canvases[]);
AFX afxNat              AfxEnumerateBindSchemas(afxDrawContext dctx, afxNat first, afxNat cnt, afxBindSchema schemas[]);
AFX afxNat              AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pipelines[]);
AFX afxNat              AfxEnumerateSamplers(afxDrawContext dctx, afxNat first, afxNat cnt, afxSampler samplers[]);
AFX afxNat              AfxEnumerateShaders(afxDrawContext dctx, afxNat first, afxNat cnt, afxShader shader[]);
AFX afxNat              AfxEnumerateRasters(afxDrawContext dctx, afxNat first, afxNat cnt, afxRaster rasters[]);

#endif//AFX_DRAW_CONTEXT_H
