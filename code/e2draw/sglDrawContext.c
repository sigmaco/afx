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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#define _AFX_DRAW_THREAD_C
#include "sgl.h"
#include "qwadro/core/afxTerminal.h"
#include "qwadro/core/afxSystem.h"
#include "sgl.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/core/afxString.h"
#include "qwadro/draw/io/afxVertexStream.h"

extern afxClassConfig _SglBufClsConfig;
extern afxClassConfig _SglSampClsConfig;
extern afxClassConfig _SglPipClsConfig;
extern afxClassConfig _SglVinClsConfig;
extern afxClassConfig _SglRazrClsConfig;
extern afxClassConfig _SglShdClsConfig;
extern afxClassConfig _SglBschClsConfig;
extern afxClassConfig _SglCanvClsConfig;
extern afxClassConfig _SglRasClsConfig;

//AFX afxClassConfig const _AfxVbufClsConfig;
//AFX afxClassConfig const _AfxIbufClsConfig;

_SGL void _SglDctxFreeAllQueueSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (dctx->base.openPorts)
    {
        for (afxNat i = 0; i < dctx->base.openPortCnt; i++)
        {
            AfxReleaseObjects(dctx->base.openPorts[i].dqueCnt, (void**)dctx->base.openPorts[i].queues);
            AfxDeallocate(dctx->base.mmu, dctx->base.openPorts[i].queues);
            dctx->base.openPorts[i].queues = NIL;
        }
        AfxDeallocate(dctx->base.mmu, dctx->base.openPorts);
    }
}

_SGL afxBool _SglProcessInputCb(afxDrawInput din, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxDrawThread dthr = (afxDrawThread)udd;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    if (din->base.prefetchEnabled)
    {
        //if (AfxTryEnterSlockExclusive(&dinD->prefetchSlock))
        {
            afxNat unitIdx = dthr->portIdx;

            if (din->base.prefetchCb && din->base.prefetchCb(din, unitIdx))
                AfxThrowError();

            //AfxExitSlockExclusive(&dinD->prefetchSlock);
        }
    }
    
    return FALSE; // don't interrupt curation;
}

_SGL afxBool _SglProcessOutputCb(afxDrawOutput dout, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    afxDrawThread dthr = (afxDrawThread)udd;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    
    afxNat unitIdx = dthr->portIdx;

    if (dout->base.procCb && dout->base.procCb(dout, unitIdx))
        AfxThrowError();

    return FALSE; // don't interrupt curation;
}

_SGL afxError _SglDctxProcCb(afxDrawContext dctx, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    afxNat unitIdx = dthr->portIdx;
    AfxAssertRange(dctx->base.openPortCnt, unitIdx, 1);

    AfxCurateConnectedDrawInputs(dctx, 0, AfxCountConnectedDrawInputs(dctx), _SglProcessInputCb, (void*)dthr);

    afxNat i = 0;
    afxDrawQueue dque;
    while ((i < dctx->base.openPorts[unitIdx].dqueCnt) && (dque = dctx->base.openPorts[unitIdx].queues[i]))
    {
        AfxAssertObjects(1, &dque, afxFcc_DQUE);

        dthr->dque = dque;
        dthr->queueIdx = i;

        if (dque->base.procCb(dque, dctx, dthr))
            AfxThrowError();

        ++i;
    }

    AfxCurateConnectedDrawOutputs(dctx, 0, AfxCountConnectedDrawOutputs(dctx), _SglProcessOutputCb, (void*)dthr);

    return err;
}

_SGL afxError _SglDctxDtor(afxDrawContext dctx)
{
    AfxEntry("dctx=%p", dctx);
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
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    //AfxAssert(spec);

    afxNat devId = *(afxNat const *)(cookie->udd[0]);
    afxDrawContextConfig const *config = ((afxDrawContextConfig const *)cookie->udd[1]) + cookie->no;

    afxDrawDevice ddev = AfxGetObjectProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawIcd ddrv = AfxGetObjectProvider(ddev);
    afxDrawSystem dsys = AfxGetObjectProvider(ddrv);

    dctx->base.running = FALSE;

    dctx->base.mmu = AfxGetDrawSystemMmu();
    

    AfxAssertObjects(1, &dctx->base.mmu, afxFcc_MMU);

    afxMmu mmu = dctx->base.mmu;

    AfxAllocateArena(mmu, &dctx->base.aren, NIL, AfxHint());

    //if (!(AfxObjectReacquire(&dctx->base.memD->obj, &dctx->base.obj, NIL, NIL, NIL))) AfxThrowError();
    //else
    {
        afxChain *classes = &dctx->base.classes;
        AfxTakeChain(classes, (void*)dctx);

        afxClassConfig tmpClsConf;

        tmpClsConf = _SglBufClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&dctx->base.buffers, classes, &tmpClsConf);
        tmpClsConf = _SglRasClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&dctx->base.rasters, classes, &tmpClsConf);
        tmpClsConf = _SglSampClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&dctx->base.samplers, classes, &tmpClsConf);
        tmpClsConf = _SglCanvClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&dctx->base.canvases, classes, &tmpClsConf);
        tmpClsConf = _SglBschClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&dctx->base.legos, classes, &tmpClsConf);
        tmpClsConf = _SglShdClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&dctx->base.shaders, classes, &tmpClsConf);
        tmpClsConf = _SglVinClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&dctx->base.vinputs, classes, &tmpClsConf);
        tmpClsConf = _SglRazrClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&dctx->base.rasterizers, classes, &tmpClsConf);
        tmpClsConf = _SglPipClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&dctx->base.pipelines, classes, &tmpClsConf);
        
#if 0
        tmpClsConf = _AfxVbufClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&dctx->base.vbuffers, classes, &tmpClsConf);
        tmpClsConf = _AfxIbufClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&dctx->base.ibuffers, classes, &tmpClsConf);
#endif

        dctx->base.openPortCnt = AfxCountDrawPorts(ddev); //spec && spec->portCnt ? spec->portCnt : 1;

        if (config && config->queueingCnt)
            dctx->base.openPortCnt = AfxMax(1, AfxMin(AfxCountDrawPorts(ddev), config->queueingCnt));

        AfxTakeChain(&dctx->base.inlinks, dctx);
        AfxTakeChain(&dctx->base.outlinks, dctx);

        dctx->base.openPorts = NIL;

        if (dctx->base.openPortCnt && !(dctx->base.openPorts = AfxAllocate(mmu, sizeof(dctx->base.openPorts[0]), dctx->base.openPortCnt, 0, AfxHint()))) AfxThrowError();
        else
        {
            AfxAssert(dctx->base.openPortCnt);

            for (afxNat i = 0; i < dctx->base.openPortCnt; i++)
            {
                afxDrawQueueingConfig const*queueing = config && config->queueings ? &config->queueings[i] : (afxDrawQueueingConfig[]) { {i, 2 } };
                
                dctx->base.openPorts[i].lastReqQueIdx = (dctx->base.openPorts[i].lastReqQueIdx + 1) % queueing->minQueueCnt;
                //dctx->base.ports[i].minRecyclSubmCnt = 2;
                dctx->base.openPorts[i].dqueCnt = AfxMax(1, queueing->minQueueCnt);

                if (!(dctx->base.openPorts[i].queues = AfxAllocate(mmu, sizeof(dctx->base.openPorts[i].queues[0]), dctx->base.openPorts[i].dqueCnt, 0, AfxHint()))) AfxThrowError();
                else
                {
                    for (afxNat j = 0; j < queueing->minQueueCnt; j++)
                    {
                        afxDrawQueue dque;
                        afxDrawQueueSpecification dqueSpec = { 0 };
                        dqueSpec.dctx = dctx;

                        if (AfxAcquireObjects(&ddev->base.ports[i].queues, 1, (afxObject*)&dque, (void const*[]) { ddev, &i, &dqueSpec }))
                        {
                            AfxThrowError();
                            AfxReleaseObjects(j, (void**)dctx->base.openPorts[i].queues);
                        }

                        AfxAssertObjects(1, &dque, afxFcc_DQUE);
                        dctx->base.openPorts[i].queues[j] = dque;
                    }

                    if (err)
                    {
                        AfxDeallocate(mmu, dctx->base.openPorts[i].queues);
                        dctx->base.openPorts[i].queues = NIL;
                    }
                }

                if (err)
                {
                    for (afxNat j = i; j-- > 0;)
                    {
                        AfxReleaseObjects(dctx->base.openPorts[j].dqueCnt, (void**)dctx->base.openPorts[j].queues);
                        AfxDeallocate(mmu, dctx->base.openPorts[j].queues);
                        dctx->base.openPorts[j].queues = NIL;
                    }
                }
            }

            if (!err)
            {
                afxUri uri;
                AfxMakeUri(&uri, "data/pipeline/rgbaToRgba.xsh.xml", 0);
                //AfxMakeUri(&uri, "data/pipeline/rgbaToRgba.xsh.xml?yFlipped", 0);
                //AfxMakeUri(&uri, "data/pipeline/rgbaToRgbaYFlippedBrokenLens.pip.xml", 0);
                //dctx->base.presentPip = AfxDrawContextFetchPipeline(dctx, &uri);


                dctx->presentPip = AfxLoadPipelineFromXsh(dctx, &uri);

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

                //AfxAssert(dctx->base.vmt);

                dctx->base.procCb = _SglDctxProcCb;

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
