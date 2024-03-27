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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#define _AFX_CORE_C
#define _AFX_THREAD_C
#define _AFX_DRAW_THREAD_C
#include "sgl.h"
#include "qwadro/core/afxTerminal.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/sim/afxSimulation.h"
#include "sgl.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/core/afxString.h"
#include "qwadro/draw/io/afxVertexStream.h"

extern afxClassConfig const _SglBufClsConfig;
extern afxClassConfig const _SglSampClsConfig;
extern afxClassConfig const _SglPipClsConfig;
extern afxClassConfig const _SglVinClsConfig;
extern afxClassConfig const _SglRazrClsConfig;
extern afxClassConfig const _SglShdClsConfig;
extern afxClassConfig const _SglBschClsConfig;
extern afxClassConfig const _SglCanvClsConfig;
extern afxClassConfig const _SglRasClsConfig;
extern afxClassConfig const _SglFencClsConfig;
extern afxClassConfig const _SglSemClsConfig;
extern afxClassConfig const _SglQrypClsConfig;

extern afxError _SglDctxExecute(afxDrawContext dctx, afxNat cnt, afxExecutionRequest const req[],afxFence);
extern afxError _SglDctxPresent(afxDrawContext dctx, afxNat cnt, afxPresentationRequest const req[]);
extern afxError _SglDctxStamp(afxDrawContext dctx, afxNat cnt, afxPresentationRequest const req[], afxV2d const origin, afxString const* caption);

_SGL void _SglDctxFreeAllQueueSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (dctx->base.openPorts)
    {
        for (afxNat i = 0; i < dctx->base.openPortCnt; i++)
        {
            AfxReleaseObjects(dctx->base.openPorts[i].dqueCnt, (void**)dctx->base.openPorts[i].queues);
            AfxDeallocate(dctx->base.openPorts[i].queues);
            dctx->base.openPorts[i].queues = NIL;
        }
        AfxDeallocate(dctx->base.openPorts);
    }
}

_SGL afxError _SglDctxDtor(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    AfxDisconnectDrawInputs(dctx);
    AfxDisconnectDrawOutputs(dctx);

    _AfxUninstallChainedClasses(&dctx->base.classes);

    _SglDctxFreeAllQueueSlots(dctx);
    
    AfxDeallocateArena(&dctx->base.aren);
    AfxReleaseObjects(1, (void*[]) { dctx->base.mmu });

    return err;
}

_SGL afxError _SglDctxCtor(afxDrawContext dctx, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    //AfxAssert(spec);

    afxDrawDevice ddev = cookie->udd[0];
    afxDrawContextConfig const *config = ((afxDrawContextConfig const *)cookie->udd[1]) + cookie->no;

    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    dctx->base.running = FALSE;

    dctx->base.mmu = AfxGetDrawSystemMmu();
    

    AfxAssertObjects(1, &dctx->base.mmu, afxFcc_MMU);

    afxMmu mmu = dctx->base.mmu;

    dctx->base.executeCb = _SglDctxExecute;
    dctx->base.presentCb = _SglDctxPresent;
    dctx->base.stampCb = _SglDctxStamp;

    AfxAllocateArena(mmu, &dctx->base.aren, NIL, AfxHint());

    //if (!(AfxObjectReacquire(&dctx->base.memD->obj, &dctx->base.obj, NIL, NIL, NIL))) AfxThrowError();
    //else
    {
        afxChain *classes = &dctx->base.classes;
        AfxSetUpChain(classes, (void*)dctx);
        afxClassConfig tmpClsConf;

        tmpClsConf = _SglSemClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.semaphores, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglFencClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.fences, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglQrypClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.queries, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglBschClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.schemas, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglSampClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.samplers, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglBufClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.buffers, NIL, classes, &tmpClsConf);
        
        tmpClsConf = _SglRasClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.rasters, NIL, classes, &tmpClsConf);
        
        tmpClsConf = _SglCanvClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.canvases, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglVinClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.vinputs, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglShdClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.shaders, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglRazrClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.rasterizers, NIL, classes, &tmpClsConf);
        
        tmpClsConf = _SglPipClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.pipelines, NIL, classes, &tmpClsConf);
        
#if 0
        tmpClsConf = _AfxVbufClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.vbuffers, classes, &tmpClsConf);
        tmpClsConf = _AfxIbufClsConfig;
        tmpClsConf.mmu = mmu;
        AfxSetUpManager(&dctx->base.ibuffers, classes, &tmpClsConf);
#endif

        dctx->base.openPortCnt = AfxCountDrawPorts(ddev);

        dctx->base.dinSlotCnt = 0;
        dctx->base.dinSlots = NIL;
        dctx->base.doutSlotCnt = 0;
        dctx->base.doutSlots = NIL;

        dctx->base.openPorts = NIL;

        if (dctx->base.openPortCnt && !(dctx->base.openPorts = AfxAllocate(dctx->base.openPortCnt, sizeof(dctx->base.openPorts[0]), 0, AfxHint()))) AfxThrowError();
        else
        {
            AfxZero(dctx->base.openPortCnt, sizeof(dctx->base.openPorts[0]), dctx->base.openPorts);
            AfxAssert(dctx->base.openPortCnt);

            for (afxNat i = 0; i < dctx->base.openPortCnt; i++)
            {
                afxDrawQueueingConfig const*queueing = config && config->queueings ? &config->queueings[i] : (afxDrawQueueingConfig[]) { { .portIdx = i, .minQueueCnt = 2 } };
                
                dctx->base.openPorts[i].lastReqQueIdx = (dctx->base.openPorts[i].lastReqQueIdx + 1) % queueing->minQueueCnt;
                //dctx->base.ports[i].minRecyclSubmCnt = 2;
                dctx->base.openPorts[i].dqueCnt = AfxMax(2, queueing->minQueueCnt);

                if (!(dctx->base.openPorts[i].queues = AfxAllocate(dctx->base.openPorts[i].dqueCnt, sizeof(dctx->base.openPorts[i].queues[0]), 0, AfxHint()))) AfxThrowError();
                else
                {
                    for (afxNat j = 0; j < queueing->minQueueCnt; j++)
                    {
                        afxDrawQueue dque;
                        sglDrawQueueSpecification dqueSpec = { 0 };
                        
                        if (AfxAcquireObjects(&ddev->ports[i].queues, 1, (afxObject*)&dque, (void const*[]) { dctx, &i, &dqueSpec }))
                        {
                            AfxThrowError();
                            AfxReleaseObjects(j, (void**)dctx->base.openPorts[i].queues);
                        }

                        AfxAssertObjects(1, &dque, afxFcc_DQUE);
                        dctx->base.openPorts[i].queues[j] = dque;
                    }

                    if (err)
                    {
                        AfxDeallocate(dctx->base.openPorts[i].queues);
                        dctx->base.openPorts[i].queues = NIL;
                    }
                }

                if (err)
                {
                    for (afxNat j = i; j-- > 0;)
                    {
                        AfxReleaseObjects(dctx->base.openPorts[j].dqueCnt, (void**)dctx->base.openPorts[j].queues);
                        AfxDeallocate(dctx->base.openPorts[j].queues);
                        dctx->base.openPorts[j].queues = NIL;
                    }
                }
            }

            if (!err)
            {
                dctx->base.clipCfg = ddev->clipCfg;
                
                afxUri uri;
                AfxMakeUri(&uri, "system/video/sampleOutRgba.xsh.xml", 0);
                //AfxMakeUri(&uri, "data/pipeline/rgbaToRgba.xsh.xml?yFlipped", 0);
                //AfxMakeUri(&uri, "data/pipeline/rgbaToRgbaYFlippedBrokenLens.pip.xml", 0);
                //dctx->base.presentPip = AfxDrawContextFetchPipeline(dctx, &uri);


                dctx->presentPip = AfxAssemblePipelineFromXsh(dctx, NIL, &uri);

                AfxAssertObjects(1, &dctx->presentPip, afxFcc_PIP);

                afxSamplerConfig smpSpec = { 0 };
                smpSpec.magFilter = afxTexelFilter_POINT;
                smpSpec.minFilter = afxTexelFilter_POINT;
                smpSpec.mipmapFilter = afxTexelFilter_POINT;
                smpSpec.uvw[0] = afxTexelAddress_REPEAT; // EDGE fucks this shit
                smpSpec.uvw[1] = afxTexelAddress_REPEAT; // EDGE fucks this shit
                smpSpec.uvw[2] = afxTexelAddress_REPEAT; // EDGE fucks this shit

                AfxAcquireSamplers(dctx, 1, &smpSpec, &dctx->presentSmp);
                AfxAssertObjects(1, &dctx->presentSmp, afxFcc_SAMP);
#if 0
                afxString tmpStr;
                AfxMakeString(&tmpStr, "a_xy", 0);
                const afxV2d tristrippedQuad2dPos[] =
                {
                    { -1.0,  1.0 },
                    { -1.0, -1.0 },
                    {  1.0,  1.0 },
                    {  1.0, -1.0 },
                };

                afxVertexBufferBlueprint vbub;
                AfxVertexBufferBlueprintReset(&vbub, 4);
                afxVertexSpec vtxAttrSpec = { 0 };
                vtxAttrSpec.secIdx = 0;
                vtxAttrSpec.semantic = &tmpStr;
                vtxAttrSpec.fmt = afxVertexFormat_V2D;
                vtxAttrSpec.usage = awxVertexUsage_POS;
                vtxAttrSpec.src = vtxPos;
                vtxAttrSpec.srcFmt = afxVertexFormat_V2D;
                vtxAttrSpec.srcStride = sizeof(afxV2d);
                AfxVertexBufferBlueprintAddAttributes(&vbub, 1, &vtxAttrSpec);
                AfxBuildVertexBuffers(dctx, 1, &dctx->presentVbuf, &vbub);
                AfxAssertObjects(1, &dctx->presentVbuf, afxFcc_VBUF);

                afxBufferSpecification vbufSpec;
                vbufSpec.siz = sizeof(tristrippedQuad2dPos);
                vbufSpec.src = tristrippedQuad2dPos;
                vbufSpec.usage = afxBufferUsage_VERTEX;

                AfxAcquireBuffers(dctx, 1, &dctx->tristrippedQuad2dPosBuf, &vbufSpec);
                AfxAssertObjects(1, &dctx->tristrippedQuad2dPosBuf, afxFcc_BUF);

                dctx->presentFboGpuHandle = 0;
#endif

                // DEVICE FONT STUFF

                afxUri uri2;
                AfxMakeUri(&uri2, "system/video/font-256.tga", 0);
                AfxLoadRastersFromTarga(dctx, afxRasterUsage_SAMPLING, NIL, 1, &uri2, &dctx->fntRas);

                afxVertexInputStream const vinStreams[] =
                {
                    {
                        .instanceRate = 1,
                        .slotIdx = 0
                    }
                };

                afxVertexInputAttr const vinAttrs[] =
                {
                    {
                        .location = 0,
                        .streamIdx = 0,
                        .offset = 0,
                        .fmt = afxVertexFormat_V2D
                    },
                    {
                        .location = 1,
                        .streamIdx = 0,
                        .offset = 8,
                        .fmt = afxVertexFormat_V2D
                    },
                    {
                        .location = 2,
                        .streamIdx = 0,
                        .offset = 16,
                        .fmt = afxVertexFormat_V4B,
                        .normalized = TRUE
                    }
                };

                afxVertexInput vin = AfxAcquireVertexInput(dctx, AFX_COUNTOF(vinStreams), vinStreams, AFX_COUNTOF(vinAttrs), vinAttrs);
                AfxAssertObjects(1, &vin, afxFcc_VIN);

                AfxMakeUri(&uri2, "system/video/font.xsh.xml?instanced", 0);
                dctx->fntPip = AfxAssemblePipelineFromXsh(dctx, vin, &uri2);
                AfxAssertObjects(1, &dctx->fntPip, afxFcc_PIP);
                AfxReleaseObjects(1, (void*[]) { vin });

                afxBufferSpecification vboSpec = { 0 };
                vboSpec.access = afxBufferAccess_W;
                vboSpec.usage = afxBufferUsage_VERTEX;
                vboSpec.src = NIL;
                vboSpec.siz = 2048;
                AfxAcquireBuffers(dctx, 1, &vboSpec, &dctx->fntDataBuf);
                AfxAssertObjects(1, &dctx->fntDataBuf, afxFcc_BUF);

                afxSamplerConfig smpCnf = { 0 };
                AfxDescribeDefaultSampler(&smpCnf);
                smpCnf.minFilter = afxTexelFilter_LINEAR;
                smpCnf.uvw[0] = afxTexelAddress_CLAMP;
                smpCnf.uvw[1] = afxTexelAddress_CLAMP;
                smpCnf.uvw[2] = afxTexelAddress_CLAMP;
                AfxAcquireSamplers(dctx, 1, &smpCnf, &dctx->fntSamp);
                AfxAssertObjects(1, &dctx->fntSamp, afxFcc_SAMP);

                afxBufferSpecification bufSpec = { 0 };
                bufSpec.siz = sizeof(awxViewConstants);
                bufSpec.usage = afxBufferUsage_UNIFORM;
                bufSpec.access = afxBufferAccess_W;
                bufSpec.src = NIL;
                AfxAcquireBuffers(dctx, 1, &bufSpec, &dctx->fntUnifBuf);
                AfxAssertObjects(1, &dctx->fntDataBuf, afxFcc_BUF);

                //AfxAssert(dctx->base.vmt);

                if (!err)
                {
                    dctx->base.running = TRUE;
                }
            }

            if (err && dctx->base.openPorts)
                _SglDctxFreeAllQueueSlots(dctx);
        }

        if (err)
        {
            _AfxUninstallChainedClasses(&dctx->base.classes);

            AfxReleaseObjects(1, (void*[]) { dctx->base.mmu });
        }
    }

    return err;
}

_SGL afxClassConfig _SglDctxClsConfig =
{
    .fcc = afxFcc_DCTX,
    .name = "Draw Context",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawContext)),
    .mmu = NIL,
    .ctor = (void*)_SglDctxCtor,
    .dtor = (void*)_SglDctxDtor
};
