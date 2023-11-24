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
#include "afx/core/afxTerminal.h"
#include "afx/core/afxSystem.h"
#include "sgl.h"
#include "afx/math/afxVector.h"
#include "afx/core/afxSystem.h"
#include "afx/core/afxString.h"

extern afxClassConfig _SglBufClsConfig;
extern afxClassConfig _SglSampClsConfig;
extern afxClassConfig _SglPipClsConfig;
extern afxClassConfig _SglPiprClsConfig;
extern afxClassConfig _SglShdClsConfig;
extern afxClassConfig _SglLegoClsConfig;
extern afxClassConfig _SglCanvClsConfig;
extern afxClassConfig _SglTexClsConfig;
extern afxClassConfig _SglSurfClsConfig;
extern afxClassConfig _SglDscrClsConfig;
extern afxClassConfig _SglDqueClsConfig;

_SGL void _AfxDctxFreeAllQueueSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (dctx->base.openPorts)
    {
        for (afxNat i = 0; i < dctx->base.openPortCnt; i++)
        {
            AfxDismountClass(&dctx->base.openPorts[i].queues);
            AfxDismountClass(&dctx->base.openPorts[i].scripts);
        }
        AfxDeallocate(dctx->base.ctx, dctx->base.openPorts);
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
    while (AfxEnumerateInstances(&dctx->base.openPorts[unitIdx].queues, i, 1, (afxHandle*)&dque))
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

    _AfxDctxFreeAllQueueSlots(dctx);
    
    AfxDeallocateArena(&dctx->base.aren);
    AfxReleaseObjects(1, (void*[]) { dctx->base.ctx });

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

    dctx->base.ctx = AfxGetDrawSystemMemory(dsys);
    

    AfxAssertObjects(1, &dctx->base.ctx, afxFcc_CTX);

    afxContext ctx = dctx->base.ctx;

    AfxAllocateArena(ctx, &dctx->base.aren, NIL, AfxHint());

    //if (!(AfxObjectReacquire(&dctx->base.memD->obj, &dctx->base.obj, NIL, NIL, NIL))) AfxThrowError();
    //else
    {
        afxChain *classes = &dctx->base.classes;
        AfxAcquireChain(classes, (void*)dctx);

        afxClassConfig tmpClsConf;

        tmpClsConf = _SglBufClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.buffers, classes, &tmpClsConf);
        tmpClsConf = _SglTexClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.rasters, classes, &tmpClsConf);
        tmpClsConf = _SglSurfClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.surfaces, classes, &tmpClsConf);
        tmpClsConf = _SglSampClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.samplers, classes, &tmpClsConf);
        tmpClsConf = _SglCanvClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.canvases, classes, &tmpClsConf);
        tmpClsConf = _SglLegoClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.legos, classes, &tmpClsConf);
        tmpClsConf = _SglShdClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.shaders, classes, &tmpClsConf);
        tmpClsConf = _SglPipClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.pipelines, classes, &tmpClsConf);
        tmpClsConf = _SglPiprClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.rasterizers, classes, &tmpClsConf);

        dctx->base.openPortCnt = AfxCountDrawPorts(ddev); //spec && spec->portCnt ? spec->portCnt : 1;

        if (config && config->queueingCnt)
            dctx->base.openPortCnt = config->queueingCnt;

        AfxAcquireChain(&dctx->base.inlinks, dctx);
        AfxAcquireChain(&dctx->base.outlinks, dctx);

        dctx->base.openPorts = NIL;

        if (dctx->base.openPortCnt && !(dctx->base.openPorts = AfxAllocate(ctx, sizeof(dctx->base.openPorts[0]), dctx->base.openPortCnt, 0, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < dctx->base.openPortCnt; i++)
            {
                afxDrawQueueingConfig const*queueing = config && config->queueings ? &config->queueings[i] : (afxDrawQueueingConfig[]) { {i, 2 } };
                tmpClsConf = _SglDqueClsConfig;
                tmpClsConf.unitsPerPage = queueing->minQueueCnt;
                tmpClsConf.maxCnt = tmpClsConf.unitsPerPage;
                tmpClsConf.ctx = ctx;
                AfxMountClass(&dctx->base.openPorts[i].queues, classes, &_SglDqueClsConfig);
                tmpClsConf = _SglDscrClsConfig;
                tmpClsConf.ctx = ctx;
                AfxMountClass(&dctx->base.openPorts[i].scripts, classes, &_SglDscrClsConfig);

                dctx->base.openPorts[i].lastReqQueIdx = (dctx->base.openPorts[i].lastReqQueIdx + 1) % queueing->minQueueCnt;
                //dctx->base.ports[i].minRecyclSubmCnt = 2;
                        
                for (afxNat j = 0; j < queueing->minQueueCnt; j++)
                {
                    afxDrawQueue dque;
                    afxDrawQueueSpecification dqueSpec = { 0 };
                    dqueSpec.portIdx = i;
                    //dqueSpec.dctx = dctx;

                    if (AfxAcquireObjects(&dctx->base.openPorts[i].queues, 1, (afxHandle*)&dque, (void*[]) { &dqueSpec }))
                        AfxThrowError();

                }

                if (err)
                {
                    for (afxNat j = 0; j < i; j++)
                    {
                        AfxDismountClass(&dctx->base.openPorts[i].scripts);
                        AfxDismountClass(&dctx->base.openPorts[i].queues);
                    }
                    AfxDeallocate(ctx, dctx->base.openPorts);
                    dctx->base.openPorts = NIL;
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
                vtxAttrSpec.usage = afxVertexUsage_POS;
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
                _AfxDctxFreeAllQueueSlots(dctx);
        }

        if (err)
        {
            _AfxUninstallChainedClasses(&dctx->base.classes);

            AfxReleaseObjects(1, (void*[]) { dctx->base.ctx });
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
    .ctx = NIL,
    .ctor = (void*)_SglDctxCtor,
    .dtor = (void*)_SglDctxDtor
};
