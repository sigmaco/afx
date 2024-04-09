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

extern afxClassConfig const _SglBufMgrCfg;
extern afxClassConfig const _SglSampMgrCfg;
extern afxClassConfig const _SglPipMgrCfg;
extern afxClassConfig const _SglVinMgrCfg;
extern afxClassConfig const _SglRazrMgrCfg;
extern afxClassConfig const _SglShdMgrCfg;
extern afxClassConfig const _SglBschMgrCfg;
extern afxClassConfig const _SglCanvMgrCfg;
extern afxClassConfig const _SglRasMgrCfg;

_SGL void _SglDctxFreeAllQueueSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxDrawBridge ddge;
    AfxIterateLinkage(AFX_OBJECT(afxDrawBridge), ddge, &dctx->base.ownedBridges, base.dctx)
    {
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);

        AfxReleaseObjects(1, (void*[]) { ddge });
    }
}

_SGL void _SglDctxFreeAllOwnedQueries(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxQueryPool qryp;
    AfxIterateLinkage(AFX_OBJECT(afxQueryPool), qryp, &dctx->base.ownedQueries, base.dctx)
    {
        AfxAssertObjects(1, &qryp, afxFcc_QRYP);

        AfxReleaseObjects(1, (void*[]) { qryp });
    }
}

_SGL void _SglDctxFreeAllOwnedFences(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxFence fenc;
    AfxIterateLinkage(AFX_OBJECT(afxFence), fenc, &dctx->base.ownedFences, base.dctx)
    {
        AfxAssertObjects(1, &fenc, afxFcc_FENC);

        AfxReleaseObjects(1, (void*[]) { fenc });
    }
}

_SGL void _SglDctxFreeAllOwnedSemaphores(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxSemaphore sem;
    AfxIterateLinkage(AFX_OBJECT(afxSemaphore), sem, &dctx->base.ownedSemaphores, base.dctx)
    {
        AfxAssertObjects(1, &sem, afxFcc_SEM);

        AfxReleaseObjects(1, (void*[]) { sem });
    }
}

_SGL afxError _SglDctxDtor(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    AfxDisconnectDrawInputs(dctx);
    AfxDisconnectDrawOutputs(dctx);

    AfxCleanUpChainedManagers(&dctx->base.classes);

    _SglDctxFreeAllQueueSlots(dctx);
    _SglDctxFreeAllOwnedQueries(dctx);
    _SglDctxFreeAllOwnedFences(dctx);
    _SglDctxFreeAllOwnedSemaphores(dctx);
    
    AfxDeallocateArena(&dctx->base.aren);

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

    AfxAllocateArena(NIL, &dctx->base.aren, NIL, AfxHere());

    dctx->base.clipCfg = AFX_CLIP_SPACE_QWADRO;

    AfxSetUpChain(&dctx->base.ownedBridges, dctx);
    AfxSetUpChain(&dctx->base.ownedSemaphores, dctx);
    AfxSetUpChain(&dctx->base.ownedFences, dctx);
    AfxSetUpChain(&dctx->base.ownedQueries, dctx);

    AfxSetUpChain(&dctx->base.inputs, dctx);
    AfxSetUpChain(&dctx->base.outputs, dctx);

    //if (!(AfxObjectReacquire(&dctx->base.memD->obj, &dctx->base.obj, NIL, NIL, NIL))) AfxThrowError();
    //else
    {
        afxChain *classes = &dctx->base.classes;
        AfxSetUpChain(classes, (void*)dctx);
        afxClassConfig tmpClsConf;

        tmpClsConf = _SglBschMgrCfg;
        AfxEstablishManager(&dctx->base.schemas, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglSampMgrCfg;
        AfxEstablishManager(&dctx->base.samplers, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglBufMgrCfg;
        AfxEstablishManager(&dctx->base.buffers, NIL, classes, &tmpClsConf);
        
        tmpClsConf = _SglRasMgrCfg;
        AfxEstablishManager(&dctx->base.rasters, NIL, classes, &tmpClsConf);
        
        tmpClsConf = _SglCanvMgrCfg;
        AfxEstablishManager(&dctx->base.canvases, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglVinMgrCfg;
        AfxEstablishManager(&dctx->base.vinputs, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglShdMgrCfg;
        AfxEstablishManager(&dctx->base.shaders, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglRazrMgrCfg;
        AfxEstablishManager(&dctx->base.rasterizers, NIL, classes, &tmpClsConf);
        
        tmpClsConf = _SglPipMgrCfg;
        AfxEstablishManager(&dctx->base.pipelines, NIL, classes, &tmpClsConf);
        
#if 0
        tmpClsConf = _AfxVbufMgrCfg;
        AfxEstablishManager(&dctx->base.vbuffers, classes, &tmpClsConf);
        tmpClsConf = _AfxIbufMgrCfg;
        AfxEstablishManager(&dctx->base.ibuffers, classes, &tmpClsConf);
#endif

        afxNat queCnt = AfxCountDrawPorts(ddev);

        for (afxNat j = 0; j < queCnt; j++)
        {
            afxDrawBridge ddge;
            afxDrawBridgeConfig dqueSpec = { 0 };

            if (AfxAcquireObjects(&ddev->ports[j].queues, 1, (afxObject*)&ddge, (void const*[]) { ddev, dctx, &j, &dqueSpec }))
            {
                AfxThrowError();
                _SglDctxFreeAllQueueSlots(dctx);
            }
            else
            {
                AfxAssertObjects(1, &ddge, afxFcc_DDGE);
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
            AfxBuildVertexBuffers(dctx, 1, &dctx->base.presentVbuf, &vbub);
            AfxAssertObjects(1, &dctx->base.presentVbuf, afxFcc_VBUF);

            afxBufferSpecification vbufSpec;
            vbufSpec.siz = sizeof(tristrippedQuad2dPos);
            vbufSpec.src = tristrippedQuad2dPos;
            vbufSpec.usage = afxBufferUsage_VERTEX;

            AfxAcquireBuffers(dctx, 1, &dctx->base.tristrippedQuad2dPosBuf, &vbufSpec);
            AfxAssertObjects(1, &dctx->base.tristrippedQuad2dPosBuf, afxFcc_BUF);

            dctx->base.presentFboGpuHandle = 0;
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

        if (err)
            _SglDctxFreeAllQueueSlots(dctx);

        if (err)
        {
            AfxCleanUpChainedManagers(&dctx->base.classes);
        }
    }

    return err;
}

_SGL afxClassConfig const _SglDctxMgrCfg =
{
    .fcc = afxFcc_DCTX,
    .name = "DrawContext",
    .desc = "Draw Context Management",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawContext)),
    .mmu = NIL,
    .ctor = (void*)_SglDctxCtor,
    .dtor = (void*)_SglDctxDtor
};
