/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

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

AFX_DEFINE_STRUCT(afxDrawQueueingConfig)
{
    afxNat                  portIdx;
    afxNat                  minQueueCnt;
};

AFX_DEFINE_STRUCT(afxDrawContextConfig)
{
    afxNat                      devId; // registered on draw system.
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

AFX_DECLARE_STRUCT(_afxDctxVmt);

#ifdef _AFX_DRAW_CONTEXT_C
AFX_OBJECT(afxDrawContext)
#else
struct afxBaseDrawContext
#endif
{
    _afxDctxVmt const*      vmt;
    afxBool                 running;

    afxChain                classes;

    afxContext              ctx;
    afxArena                aren;
    
    afxNat                  lastReqPortIdx;
    afxNat                  openPortCnt;
    struct
    {
        
        //afxSlock            recyclSubmChainSlock;
        //afxChain            recyclSubmChain;
        //afxNat              minRecyclSubmCnt;
        afxNat              lastReqQueIdx;
        afxClass            queues;
        afxClass            scripts;
    }*                      openPorts;

    afxNat                  inputCap;
    afxNat                  inputCnt;
    afxDrawInput*           inputs;

    afxNat                  outputCap;
    afxNat                  outputCnt;
    afxDrawOutput*          outputs;

    afxClass           textures;
    afxClass           buffers;
    afxClass           vertices;
    afxClass           indices;
    afxClass           surfaces;
    afxClass           shaders;
    afxClass           pipelines;
    afxClass           legos;
    afxClass           canvases;
    afxClass           samplers;
};

AFX afxClass*           AfxGetDrawContextClass(afxDrawDevice ddev);
AFX afxNat              AfxCountDrawContexts(afxDrawDevice ddev);

AFX afxNat              AfxEnumerateDrawContexts(afxDrawSystem dsys, afxNat devId, afxNat first, afxNat cnt, afxDrawContext dctx[]);
AFX afxNat              AfxCurateDrawContexts(afxDrawSystem dsys, afxNat devId, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd);

AFX afxError            AfxAcquireDrawContexts(afxDrawSystem dsys, afxNat cnt, afxDrawContext dctx[], afxDrawContextConfig const config[]);

AFX afxContext           AfxGetDrawContextMemory(afxDrawContext dctx);

AFX afxNat              AfxGetDrawQueueCount(afxDrawContext dctx, afxNat portIdx);
AFX afxDrawQueue        AfxGetDrawQueue(afxDrawContext dctx, afxNat portIdx, afxNat queIdx);
AFX afxError            AfxRequestNextDrawQueue(afxDrawContext dctx, afxNat portIdx, afxTime timeout, afxNat *queIdx);

AFX afxDrawInput        AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat slotIdx);
AFX afxDrawOutput       AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat slotIdx);

AFX afxNat              AfxCountConnectedDrawInputs(afxDrawContext dctx);
AFX afxNat              AfxCountConnectedDrawOutputs(afxDrawContext dctx);

AFX afxError            AfxDisconnectAllDrawInputs(afxDrawContext dctx);
AFX afxError            AfxDisconnectAllDrawOutputs(afxDrawContext dctx);

AFX afxNat              AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawInput din[]);
AFX afxNat              AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOutput dout[]);

AFX afxNat              AfxCurateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AFX afxNat              AfxCurateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);

AFX afxError            AfxAcquireDrawOperations(afxNat cnt, afxDrawOperation dop[], afxUri const uri[]);
AFX afxError            AfxBuildDrawOperations(afxNat cnt, afxDrawOperation dop[], afxDrawOperationBlueprint const blueprint[]);
AFX afxResult           AfxFindDrawOperations(afxNat cnt, afxDrawOperation dop[], afxUri const name[]);
AFX afxError            AfxUploadDrawOperations(afxNat cnt, afxDrawOperation dop[], afxUri const uri[]);

#endif//AFX_DRAW_CONTEXT_H