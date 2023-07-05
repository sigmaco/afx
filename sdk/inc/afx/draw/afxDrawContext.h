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

#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/draw/pipelining/afxPipeline.h"
#include "afx/draw/afxDrawQueue.h"
#include "afx/core/afxIterator.h"
#include "afx/draw/afxDrawOperation.h"

AFX_DEFINE_STRUCT(afxDrawContextSpecification)
{
    afxNat              driverId; // registered on draw system. TODO
    void                *enabledFeatures; // afxDrawDriverFeatures

    afxNat              queueCnt;
    afxBool             autonomousQueue;
    afxMemory           genrlMem;

    afxNat              maxBufCnt;
    afxNat              maxSmpCnt;
    afxNat              maxTexCnt;
    afxNat              maxCanvCnt;
    afxNat              maxPipCnt;
    afxNat              maxLegoCnt;
    afxNat              maxShdCnt;
    afxNat              maxDoutCnt;
    afxNat              maxDinCnt;
    afxNat              maxDscrCnt;
};

AFX_DEFINE_HANDLE(afxDrawContext);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxDrawContext)
{
    afxObject           obj;
};

#endif

AFX afxDrawQueue    AfxDrawContextAcquireQueue(afxDrawContext dctx, afxDrawQueueSpecification const *spec);
AFX afxDrawInput    AfxDrawContextAcquireInput(afxDrawContext dctx, afxDrawInputSpecification const *spec);
AFX afxDrawOutput   AfxDrawContextAcquireOutput(afxDrawContext dctx, afxDrawOutputSpecification const *spec);

AFX void*               AfxDrawContextGetDriver(afxDrawContext dctx);
AFX void*               AfxDrawContextGetDrawSystem(afxDrawContext dctx);
AFX afxMemory           AfxDrawContextGetMemory(afxDrawContext dctx);

AFX afxClass*           AfxDrawContextGetBufferClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetCanvasClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetIndexBufferClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetInputClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetOutputClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetShaderClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetPipelineClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetLegoClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetQueueClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetSamplerClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetScriptClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetSurfaceClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetOperationClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetTextureClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetVertexBufferClass(afxDrawContext dctx);

AFX afxResult           AfxDrawContextEnumerateQueues(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawQueue dque[]);
AFX afxResult           AfxDrawContextEnumerateInputs(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawInput din[]);
AFX afxResult           AfxDrawContextEnumerateScripts(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawScript dscr[]);
AFX afxResult           AfxDrawContextEnumerateOperations(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawOperation dop[]);
AFX afxResult           AfxDrawContextEnumerateTextures(afxDrawContext dctx, afxNat base, afxNat cnt, afxTexture tex[]);
AFX afxResult           AfxDrawContextEnumerateIndexBuffers(afxDrawContext dctx, afxNat base, afxNat cnt, afxIndexBuffer ibuf[]);
AFX afxResult           AfxDrawContextEnumerateVertexBuffers(afxDrawContext dctx, afxNat base, afxNat cnt, afxVertexBuffer vbuf[]);
AFX afxResult           AfxDrawContextEnumerateBuffers(afxDrawContext dctx, afxNat base, afxNat cnt, afxBuffer buf[]);
AFX afxResult           AfxDrawContextEnumerateSamplers(afxDrawContext dctx, afxNat base, afxNat cnt, afxSampler smp[]);
AFX afxResult           AfxDrawContextEnumerateShaders(afxDrawContext dctx, afxNat base, afxNat cnt, afxShader shd[]);
AFX afxResult           AfxDrawContextEnumeratePipelines(afxDrawContext dctx, afxNat base, afxNat cnt, afxPipeline pip[]);
AFX afxResult           AfxDrawContextEnumerateSurfaces(afxDrawContext dctx, afxNat base, afxNat cnt, afxSurface surf[]);
AFX afxResult           AfxDrawContextEnumerateCanvases(afxDrawContext dctx, afxNat base, afxNat cnt, afxCanvas canv[]);
AFX afxResult           AfxDrawContextEnumerateOutputs(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawOutput dout[]);

AFX afxError            AfxDrawContextAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBufferSpecification const spec[], afxBuffer buf[]);
AFX afxError            AfxDrawContextAcquireSamplers(afxDrawContext dctx, afxNat cnt, afxSamplerSpecification const spec[], afxSampler smp[]);
AFX afxError            AfxDrawContextBuildCanvases(afxDrawContext dctx, afxNat cnt, afxCanvasBlueprint const blueprint[], afxCanvas canv[]);
AFX afxError            AfxDrawContextBuildIndexBuffers(afxDrawContext dctx, afxNat cnt, afxIndexBufferBlueprint const blueprint[], afxIndexBuffer ibuf[]);
AFX afxError            AfxDrawContextBuildVertexBuffers(afxDrawContext dctx, afxNat cnt, afxVertexBufferBlueprint const blueprint[], afxVertexBuffer vbuf[]);

// Texture
AFX afxError            AfxDrawContextAcquireTextures(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxTexture tex[]);
AFX afxError            AfxDrawContextBuildTextures(afxDrawContext dctx, afxNat cnt, afxTextureBlueprint const blueprint[], afxTexture tex[]);
AFX afxResult           AfxDrawContextFindTextures(afxDrawContext dctx, afxNat cnt, afxUri const name[], afxTexture tex[]);
AFX afxError            AfxDrawContextUploadTextures(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxTexture tex[]);

AFX afxSurface          AfxDrawContextAcquireSurface(afxDrawContext dctx, afxPixelFormat fmt, afxWhd const extent, afxFlags usage);

AFX afxError            AfxDrawContextAcquireShaders(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxShader shd[]);
AFX afxError            AfxDrawContextBuildShaders(afxDrawContext dctx, afxNat cnt, afxShaderBlueprint const blueprint[], afxShader shd[]);
AFX afxResult           AfxDrawContextFindShaders(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxShader shd[]);
AFX afxError            AfxDrawContextUploadShaders(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxShader shd[]);

AFX afxError            AfxDrawContextBuildPipelines(afxDrawContext dctx, afxNat cnt, afxPipelineBlueprint const blueprint[], afxPipeline pip[]);
AFX afxError            AfxDrawContextBuildLegos(afxDrawContext dctx, afxNat cnt, afxLegoBlueprint const blueprint[], afxLego legt[]);

AFX afxError            AfxDrawContextAcquireOperations(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxDrawOperation dop[]);
AFX afxError            AfxDrawContextBuildOperations(afxDrawContext dctx, afxNat cnt, afxDrawOperationBlueprint const blueprint[], afxDrawOperation dop[]);
AFX afxResult           AfxDrawContextFindOperations(afxDrawContext dctx, afxNat cnt, afxUri const name[], afxDrawOperation dop[]);
AFX afxError            AfxDrawContextUploadOperations(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxDrawOperation dop[]);

#endif//AFX_DRAW_CONTEXT_H