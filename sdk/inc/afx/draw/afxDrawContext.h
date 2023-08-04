/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

// Draw context é um objeto que controla o thread de execução. O contexto real está vinculado às draw outputs.

#ifndef AFX_DRAW_CONTEXT_H
#define AFX_DRAW_CONTEXT_H

#include "afx/core/afxClass.h"
#include "afx/draw/pipelining/afxCanvasBlueprint.h"
#include "afx/draw/pipelining/afxDrawOperation.h"
#include "afx/draw/pipelining/afxPipeline.h"
#include "afx/draw/pipelining/afxPipelineBlueprint.h"
#include "afx/draw/res/afxShaderBlueprint.h"
#include "afx/draw/res/afxTextureBlueprint.h"
#include "afx/draw/res/afxVertexBufferBlueprint.h"
#include "afx/draw/res/afxIndexBuffer.h"
#include "afx/draw/afxDrawQueue.h"
#include "afx/draw/afxDrawScript.h"

AFX_DEFINE_STRUCT(afxDrawContextSpecification)
{
    afxNat                      drvIdx; // registered on draw system.
    void const*                 enabledFeatures; // afxDrawDriverFeatures

    //afxNat                      portCnt;
    //afxDrawQueueCapFlags const* portCaps;
    //afxNat const*               queuesPerPort;

    afxMemory                   genrlMem;

    afxNat                      maxBufCnt;
    afxNat                      maxSmpCnt;
    afxNat                      maxTexCnt;
    afxNat                      maxCanvCnt;
    afxNat                      maxPipCnt;
    afxNat                      maxLegoCnt;
    afxNat                      maxShdCnt;
    afxNat                      maxDoutCnt;
    afxNat                      maxDinCnt;
    afxNat                      maxDscrCnt;
};

AFX_DECLARE_STRUCT(_afxDctxVmt);

AFX_OBJECT(afxDrawContext)
{
    afxObject               obj;
    _afxDctxVmt const*      vmt;
    void*                   idd;
#ifdef _AFX_DRAW_CONTEXT_C
    afxBool                 running;

    afxChain                provisions;

    afxClass                canvClass; // uses surf
    afxClass                pipClass; // uses shd, pipa, canv
    afxClass                legtClass;
    afxClass                dopClass;

    afxClass            bufClass;
    afxClass            vbufClass;
    afxClass            ibufClass;
    afxClass            smpClass;
    afxClass            texClass;
    afxClass            surfClass;
    afxClass            shdClass;

    afxMemory               mem;
    afxArena                aren;
    
    afxNat                  lastReqPortIdx;
    afxNat                  cachedPortCnt;
    struct
    {
        
        //afxSlock            recyclSubmChainSlock;
        //afxChain            recyclSubmChain;
        //afxNat              minRecyclSubmCnt;
        afxNat              lastReqQueIdx;
        afxNat              queueCnt;
        afxDrawQueue*       queues;
    }*                      ports;

    afxNat                  inputCap;
    afxNat                  inputCnt;
    afxDrawInput*           inputs;

    afxNat                  outputCap;
    afxNat                  outputCnt;
    afxDrawOutput*          outputs;
#endif
};

AFX afxDrawDriver       AfxGetDrawContextDriver(afxDrawContext dctx);
AFX afxMemory           AfxGetDrawContextMemory(afxDrawContext dctx);

AFX afxNat              AfxGetDrawQueueCount(afxDrawContext dctx, afxNat portIdx);
AFX afxDrawQueue        AfxGetDrawQueue(afxDrawContext dctx, afxNat portIdx, afxNat queIdx);
AFX afxError            AfxRequestNextDrawQueue(afxDrawContext dctx, afxNat portIdx, afxTime timeout, afxNat *queIdx);

AFX afxDrawInput        AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat slotIdx);
AFX afxDrawOutput       AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat slotIdx);

AFX afxNat              AfxGetConnectedDrawInputCount(afxDrawContext dctx);
AFX afxNat              AfxGetConnectedDrawOutputCount(afxDrawContext dctx);

AFX afxError            AfxDisconnectAllDrawInputs(afxDrawContext dctx);
AFX afxError            AfxDisconnectAllDrawOutputs(afxDrawContext dctx);

AFX afxNat              AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawInput din[]);
AFX afxNat              AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOutput dout[]);

AFX afxNat              AfxCurateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AFX afxNat              AfxCurateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);

AFX afxClass*           AfxGetBufferClass(afxDrawContext dctx);
AFX afxClass*           AfxGetCanvasClass(afxDrawContext dctx);
AFX afxClass*           AfxGetDrawOperationClass(afxDrawContext dctx);
AFX afxClass*           AfxGetIndexBufferClass(afxDrawContext dctx);
AFX afxClass*           AfxGetLegoClass(afxDrawContext dctx);
AFX afxClass*           AfxGetPipelineClass(afxDrawContext dctx);
AFX afxClass*           AfxGetShaderClass(afxDrawContext dctx);
AFX afxClass*           AfxGetSamplerClass(afxDrawContext dctx);
AFX afxClass*           AfxGetSurfaceClass(afxDrawContext dctx);
AFX afxClass*           AfxGetTextureClass(afxDrawContext dctx);
AFX afxClass*           AfxGetVertexBufferClass(afxDrawContext dctx);

AFX afxNat              AfxEnumerateBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxBuffer buf[]);
AFX afxNat              AfxEnumerateCanvases(afxDrawContext dctx, afxNat first, afxNat cnt, afxCanvas canv[]);
AFX afxNat              AfxEnumerateDrawOperations(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOperation dop[]);
AFX afxNat              AfxEnumerateIndexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxIndexBuffer ibuf[]);
AFX afxNat              AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pip[]);
AFX afxNat              AfxEnumerateLegos(afxDrawContext dctx, afxNat first, afxNat cnt, afxLego lego[]);
AFX afxNat              AfxEnumerateSamplers(afxDrawContext dctx, afxNat first, afxNat cnt, afxSampler smp[]);
AFX afxNat              AfxEnumerateShaders(afxDrawContext dctx, afxNat first, afxNat cnt, afxShader shd[]);
AFX afxNat              AfxEnumerateSurfaces(afxDrawContext dctx, afxNat first, afxNat cnt, afxSurface surf[]);
AFX afxNat              AfxEnumerateTextures(afxDrawContext dctx, afxNat first, afxNat cnt, afxTexture tex[]);
AFX afxNat              AfxEnumerateVertexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxVertexBuffer vbuf[]);







AFX afxError            AfxBuildPipelines(afxDrawContext dctx, afxNat cnt, afxPipelineBlueprint const blueprint[], afxPipeline pip[]);
AFX afxError            AfxUploadPipelines(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxPipeline pip[]);
AFX afxError            AfxBuildLegos(afxDrawContext dctx, afxNat cnt, afxLegoBlueprint const blueprint[], afxLego legt[]);

AFX afxError            AfxAcquireDrawOperations(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxDrawOperation dop[]);
AFX afxError            AfxBuildDrawOperations(afxDrawContext dctx, afxNat cnt, afxDrawOperationBlueprint const blueprint[], afxDrawOperation dop[]);
AFX afxResult           AfxFindDrawOperations(afxDrawContext dctx, afxNat cnt, afxUri const name[], afxDrawOperation dop[]);
AFX afxError            AfxUploadDrawOperations(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxDrawOperation dop[]);

AFX afxError            AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBufferSpecification const spec[], afxBuffer buf[]);
AFX afxError            AfxBuildIndexBuffers(afxDrawContext dctx, afxNat cnt, afxIndexBufferBlueprint const blueprint[], afxIndexBuffer ibuf[]);
AFX afxError            AfxBuildVertexBuffers(afxDrawContext dctx, afxNat cnt, afxVertexBufferBlueprint const blueprint[], afxVertexBuffer vbuf[]);

AFX afxSurface          AfxAcquireSurface(afxDrawContext dctx, afxPixelFormat fmt, afxWhd const extent, afxFlags usage);

AFX afxError            AfxAcquireSamplers(afxDrawContext dctx, afxNat cnt, afxSamplerSpecification const spec[], afxSampler smp[]);

AFX afxError            AfxAcquireShaders(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxShader shd[]);
AFX afxError            AfxBuildShaders(afxDrawContext dctx, afxNat cnt, afxShaderBlueprint const blueprint[], afxShader shd[]);
AFX afxResult           AfxFindShaders(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxShader shd[]);
AFX afxError            AfxUploadShaders(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxShader shd[]);

#endif//AFX_DRAW_CONTEXT_H