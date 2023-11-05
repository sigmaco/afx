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

#include "afx/core/afxClass.h"
#include "afx/draw/afxCanvasBlueprint.h"
#include "afx/draw/afxDrawOperation.h"
#include "afx/draw/afxPipeline.h"
#include "afx/draw/afxPipeline.h"
#include "afx/draw/afxShaderBlueprint.h"
#include "afx/draw/afxDrawQueue.h"
#include "afx/draw/afxDrawScript.h"
#include "afx/draw/afxDrawCommands.h"

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
    afxNat                      maxDoutCnt;
    afxNat                      maxDinCnt;
    afxNat                      maxDscrCnt;

    afxSize const*              attrs[2];
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_CONTEXT_C
AFX_OBJECT(afxDrawContext)
#else
struct afxBaseDrawContext
#endif
{
    afxError            (*procCb)(afxDrawContext,afxDrawThread);
    afxBool             running;

    afxChain            classes;

    afxContext          ctx;
    afxArena            aren;
    
    afxNat              lastReqPortIdx;
    afxNat              openPortCnt;
    struct
    {
        
        //afxSlock            recyclSubmChainSlock;
        //afxChain            recyclSubmChain;
        //afxNat              minRecyclSubmCnt;
        afxNat          lastReqQueIdx;
        afxClass        queues;
        afxClass        scripts;
    }*                  openPorts;

    afxChain            inlinks;
    afxChain            outlinks;

    afxClass            textures;
    afxClass            buffers;
    afxClass            surfaces;
    afxClass            shaders;
    afxClass            pipelines;
    afxClass            legos;
    afxClass            canvases;
    afxClass            samplers;

    afxCmd const*       stdCmds;
};
#endif

AFX afxDrawDevice       AfxGetDrawContextDevice(afxDrawContext dctx);
AFX afxContext          AfxGetDrawContextMemory(afxDrawContext dctx);

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

AFX afxNat              AfxEnumerateBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxBuffer buf[]);
AFX afxNat              AfxEnumerateCanvases(afxDrawContext dctx, afxNat first, afxNat cnt, afxCanvas canv[]);
AFX afxNat              AfxEnumeratePipelineRigs(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipelineRig lego[]);
AFX afxNat              AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pip[]);
AFX afxNat              AfxEnumerateSamplers(afxDrawContext dctx, afxNat first, afxNat cnt, afxSampler samp[]);
AFX afxNat              AfxEnumerateShaders(afxDrawContext dctx, afxNat first, afxNat cnt, afxShader shd[]);
AFX afxNat              AfxEnumerateSurfaces(afxDrawContext dctx, afxNat first, afxNat cnt, afxSurface surf[]);
AFX afxNat              AfxEnumerateTextures(afxDrawContext dctx, afxNat first, afxNat cnt, afxTexture tex[]);

#endif//AFX_DRAW_CONTEXT_H