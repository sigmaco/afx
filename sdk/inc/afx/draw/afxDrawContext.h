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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

// Draw context é um objeto que controla o thread de execução. O contexto real está vinculado às draw outputs.

#ifndef AFX_DRAW_CONTEXT_H
#define AFX_DRAW_CONTEXT_H

#include "afxDrawInput.h"
#include "afxDrawOutput.h"
#include "pipelining/afxPipeline.h"
#include "afxDrawEngine.h"
#include "afx/core/base/afxIterator.h"

AFX_DEFINE_STRUCT(afxDrawContextSpecification)
{
    afxNat              driverId; // registered on draw system. TODO
    void                *enabledFeatures; // afxDrawDriverFeatures

    afxNat              queueCnt;
    afxBool             autonomousQueue;

    afxNat              maxBufCnt;
    afxNat              maxSmpCnt;
    afxNat              maxTexCnt;
    afxNat              maxCanvCnt;
    afxNat              maxPipCnt;
    afxNat              maxLegoCnt;
    afxNat              maxPipmCnt;
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

AFX afxDrawEngine       AfxDrawContextAcquireEngine(afxDrawContext dctx, afxNat idx, afxBool autonomous);
AFX afxDrawInput        AfxDrawContextAcquireInput(afxDrawContext dctx, afxDrawInputSpecification const *spec);
AFX afxDrawOutput       AfxDrawContextAcquireOutput(afxDrawContext dctx, afxWhd const extent, afxDrawOutputSpecification const *spec);

AFX void*               AfxDrawContextGetDriver(afxDrawContext dctx);
AFX void*               AfxDrawContextGetDrawSystem(afxDrawContext dctx);

AFX afxAllocator        AfxDrawContextGetAllocator(afxDrawContext dctx);

AFX afxClass*           AfxDrawContextGetBufferClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetCanvasClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetIndexBufferClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetInputClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetOutputClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetPipelineRigClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetPipelineModuleClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetPipelineClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetLegoClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetQueueClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetSamplerClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetScriptClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetSurfaceClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetTextureClass(afxDrawContext dctx);
AFX afxClass*           AfxDrawContextGetVertexBufferClass(afxDrawContext dctx);

AFX afxResult           AfxDrawContextEnumerateEngines(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawEngine deng[]);
AFX afxResult           AfxDrawContextEnumerateInputs(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawInput din[]);
AFX afxResult           AfxDrawContextEnumerateScripts(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawScript dscr[]);
AFX afxResult           AfxDrawContextEnumerateTextures(afxDrawContext dctx, afxNat base, afxNat cnt, afxTexture tex[]);
AFX afxResult           AfxDrawContextEnumerateIndexBuffers(afxDrawContext dctx, afxNat base, afxNat cnt, afxIndexBuffer ibuf[]);
AFX afxResult           AfxDrawContextEnumerateVertexBuffers(afxDrawContext dctx, afxNat base, afxNat cnt, afxVertexBuffer vbuf[]);
AFX afxResult           AfxDrawContextEnumerateBuffers(afxDrawContext dctx, afxNat base, afxNat cnt, afxBuffer buf[]);
AFX afxResult           AfxDrawContextEnumerateSamplers(afxDrawContext dctx, afxNat base, afxNat cnt, afxSampler smp[]);
AFX afxResult           AfxDrawContextEnumeratePipelineModules(afxDrawContext dctx, afxNat base, afxNat cnt, afxPipelineModule pipm[]);
AFX afxResult           AfxDrawContextEnumeratePipelines(afxDrawContext dctx, afxNat base, afxNat cnt, afxPipeline pip[]);
AFX afxResult           AfxDrawContextEnumerateSurfaces(afxDrawContext dctx, afxNat base, afxNat cnt, afxSurface surf[]);
AFX afxResult           AfxDrawContextEnumerateCanvases(afxDrawContext dctx, afxNat base, afxNat cnt, afxCanvas canv[]);
AFX afxResult           AfxDrawContextEnumerateOutputs(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawOutput dout[]);

AFX afxBuffer           AfxDrawContextAcquireBuffer(afxDrawContext dctx, afxBufferSpecification const *spec);
AFX afxCanvas           AfxDrawContextAcquireCanvas(afxDrawContext dctx, afxWhd const extent, afxNat surfaceCnt, afxSurfaceSpecification const *surfSpecs);
AFX afxIndexBuffer      AfxDrawContextAcquireIndexBuffer(afxDrawContext dctx, afxIndexBufferSpecification const *spec);
AFX afxSampler          AfxDrawContextAcquireSampler(afxDrawContext dctx, afxSamplerSpecification const *spec);
AFX afxVertexBuffer     AfxDrawContextAcquireVertexBuffer(afxDrawContext dctx, afxNat vtxCnt, afxNat arrCnt, afxVertexDataSpecification const *specs);

// Texture
AFX afxResult           AfxDrawContextBuildTextures(afxDrawContext dctx, afxNat cnt, afxTextureBlueprint const texb[], afxTexture tex[]);
AFX afxResult           AfxDrawContextFetchTextures(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxTexture tex[]);
AFX afxResult           AfxDrawContextFindTextures(afxDrawContext dctx, afxNat cnt, afxUri const name[], afxTexture tex[]);
AFX afxResult           AfxDrawContextUploadTextures(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxTexture tex[]);

AFX afxSurface          AfxDrawContextAcquireSurface(afxDrawContext dctx, afxPixelFormat fmt, afxWhd const extent, afxFlags usage);

AFX afxPipelineModule   AfxDrawContextBuildPipelineModule(afxDrawContext dctx, afxByte const code[], afxNat len);
AFX afxPipelineModule   AfxDrawContextFetchPipelineModule(afxDrawContext dctx, afxUri const *uri);
AFX afxPipelineModule   AfxDrawContextFindPipelineModule(afxDrawContext dctx, afxUri const *uri);
AFX afxPipelineModule   AfxDrawContextUploadPipelineModule(afxDrawContext dctx, afxUri const *uri);

AFX afxPipelineRig      AfxDrawContextBuildPipelineRig(afxDrawContext dctx, afxNat socketCnt, afxLegoSchema const sockets[]);
AFX afxLego             AfxDrawContextAcquireLego(afxDrawContext dctx, afxLegoSchema const *schema, afxLegoPoint const points[]);

AFX afxPipeline         AfxDrawContextBuildPipeline(afxDrawContext dctx, afxPipelineBlueprint const *pipb);
AFX afxPipeline         AfxDrawContextFetchPipeline(afxDrawContext dctx, afxUri const *uri);
AFX afxPipeline         AfxDrawContextFindPipeline(afxDrawContext dctx, afxUri const *name);
AFX afxPipeline         AfxDrawContextUploadPipeline(afxDrawContext dctx, afxUri const *uri);

#endif//AFX_DRAW_CONTEXT_H