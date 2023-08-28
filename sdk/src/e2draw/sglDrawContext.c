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
#include "../e2coree/draw/afxDrawParadigms.h"

extern afxClassConfig _SglBufClsConfig;
extern afxClassConfig _SglVbufClsConfig;
extern afxClassConfig _SglIbufClsConfig;
extern afxClassConfig _SglSampClsConfig;
extern afxClassConfig _SglPipClsConfig;
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
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

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

_SGL void _AfxDctxFreeAllInputSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    if (dctx->base.inputs)
    {
        for (afxNat i = 0; i < dctx->base.inputCap; i++)
        {
            afxDrawInput din = dctx->base.inputs[i];

            if (din)
            {
                AfxDisconnectDrawInput(din, NIL);
                dctx->base.inputs[i] = NIL;
                --dctx->base.inputCnt;
            }
        }
        dctx->base.inputCap = 0;
        AfxDeallocate(dctx->base.ctx, dctx->base.inputs);
        dctx->base.inputs = NIL;
    }
}

_SGL void _AfxDctxFreeAllOutputSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    if (dctx->base.outputs)
    {
        for (afxNat i = 0; i < dctx->base.outputCap; i++)
        {
            afxDrawOutput dout = dctx->base.outputs[i];

            if (dout)
            {
                AfxDisconnectDrawOutput(dout, NIL);
                dctx->base.outputs[i] = NIL;
                --dctx->base.outputCnt;
            }
        }
        dctx->base.outputCap = 0;
        AfxDeallocate(dctx->base.ctx, dctx->base.outputs);
        dctx->base.outputs = NIL;
    }
}

_SGL afxBool _SglProcessInputCb(afxDrawInput din, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    afxDrawThread dthr = (afxDrawThread)udd;
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    
    if (_SglDinVmtProcCb(dthr, din))
        AfxThrowError();

    return FALSE; // don't interrupt curation;
}

_SGL afxBool _SglProcessOutputCb(afxDrawOutput dout, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

    afxDrawThread dthr = (afxDrawThread)udd;
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);

    if (_SglDoutVmtProcCb(dthr, dout))
        AfxThrowError();

    return FALSE; // don't interrupt curation;
}

_SGL afxError _SglDctxVmtProcCb(afxDrawThread dthr, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);

    afxNat unitIdx = dthr->portIdx;
    AfxAssertRange(dctx->base.openPortCnt, unitIdx, 1);

    AfxCurateConnectedDrawInputs(dctx, 0, AfxCountConnectedDrawInputs(dctx), _SglProcessInputCb, (void*)dthr);

    afxNat i = 0;
    afxDrawQueue dque;
    while (AfxEnumerateInstances(&dctx->base.openPorts[unitIdx].queues, i, 1, (afxHandle*)&dque))
    {
        AfxAssertObjects(1, &dque, AFX_FCC_DQUE);

        dthr->dque = dque;
        dthr->queueIdx = i;

        if (_SglDqueVmtProcCb(dthr, dctx, dque))
            AfxThrowError();

        ++i;
    }

    AfxCurateConnectedDrawOutputs(dctx, 0, AfxCountConnectedDrawOutputs(dctx), _SglProcessOutputCb, (void*)dthr);

    return err;
}

_SGL afxError _SglDctxVmtDinCb(afxDrawContext dctx, afxDrawInput din, afxBool connect, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    afxNat slotIdx2 = AFX_INVALID_INDEX;

    if (dctx->base.inputs)
    {
        for (afxNat i = 0; i < dctx->base.inputCap; i++)
        {
            if (din == dctx->base.inputs[i])
            {
                slotIdx2 = i;
                break;
            }
        }
    }

    if (slotIdx2 != AFX_INVALID_INDEX)
    {
        AfxAssert(dctx->base.inputs);

        if (connect)
        {
            AfxThrowError(); // already connected to connect
        }
        else
        {
            dctx->base.inputs[slotIdx2] = NIL; // disconnect
            --dctx->base.inputCnt;
        }
    }
    else
    {
        if (!connect)
        {
            AfxThrowError(); // not connected to disconnect
        }
        else
        {
            if (dctx->base.inputCap > dctx->base.inputCnt)
            {
                for (afxNat i = 0; i < dctx->base.inputCap; i++)
                {
                    if (!dctx->base.inputs[i])
                    {
                        slotIdx2 = i;
                        dctx->base.inputs[slotIdx2] = din;
                        ++dctx->base.inputCnt;
                        break;
                    }
                }
            }
            else
            {
                AfxAssert(dctx->base.inputCap == dctx->base.inputCnt);
                afxContext mem = AfxGetDrawContextMemory(dctx);
                AfxAssertObjects(1, &mem, AFX_FCC_CTX);
                void *boundDins = AfxReallocate(mem, dctx->base.inputs, (dctx->base.inputCap + 1) * sizeof(dctx->base.inputs[0]), 0, AfxSpawnHint());

                if (!boundDins) AfxThrowError();
                else
                {
                    dctx->base.inputs = boundDins;
                    slotIdx2 = dctx->base.inputCnt;
                    dctx->base.inputs[slotIdx2] = din;
                    ++dctx->base.inputCap;
                    ++dctx->base.inputCnt;
                }
            }
        }
    }

    if (slotIdx)
        *slotIdx = slotIdx2;

    return err;
}

_SGL afxError _SglDctxVmtDoutCb(afxDrawContext dctx, afxDrawOutput dout, afxBool connect, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

    afxNat slotIdx2 = AFX_INVALID_INDEX;

    if (dctx->base.outputs)
    {
        for (afxNat i = 0; i < dctx->base.outputCap; i++)
        {
            if (dout == dctx->base.outputs[i])
            {
                slotIdx2 = i;
                break;
            }
        }
    }

    if (slotIdx2 != AFX_INVALID_INDEX)
    {
        AfxAssert(dctx->base.outputs);

        if (connect)
        {
            AfxThrowError(); // already connected to connect
        }
        else
        {
            dctx->base.outputs[slotIdx2] = NIL; // disconnect
            --dctx->base.outputCnt;
        }
    }
    else
    {
        if (!connect)
        {
            AfxThrowError(); // not connected to disconnect
        }
        else
        {
            if (dctx->base.outputCap > dctx->base.outputCnt)
            {
                for (afxNat i = 0; i < dctx->base.outputCap; i++)
                {
                    if (!dctx->base.outputs[i])
                    {
                        slotIdx2 = i;
                        dctx->base.outputs[slotIdx2] = dout;
                        ++dctx->base.outputCnt;
                        break;
                    }
                }
            }
            else
            {
                AfxAssert(dctx->base.outputCap == dctx->base.outputCnt);
                afxContext mem = AfxGetDrawContextMemory(dctx);
                AfxAssertObjects(1, &mem, AFX_FCC_CTX);
                void *boundDouts = AfxReallocate(mem, dctx->base.outputs, (dctx->base.outputCap + 1) * sizeof(dctx->base.outputs[0]), 0, AfxSpawnHint());

                if (!boundDouts) AfxThrowError();
                else
                {
                    dctx->base.outputs = boundDouts;
                    slotIdx2 = dctx->base.outputCnt;
                    dctx->base.outputs[slotIdx2] = dout;
                    ++dctx->base.outputCap;
                    ++dctx->base.outputCnt;
                }
            }
        }
    }

    if (slotIdx)
        *slotIdx = slotIdx2;

    return err;
}

_SGL afxError _SglDctxDtor(afxDrawContext dctx)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    AfxDisconnectAllDrawInputs(dctx);
    AfxDisconnectAllDrawOutputs(dctx);

    _AfxUninstallChainedClasses(&dctx->base.classes);

    _AfxDctxFreeAllQueueSlots(dctx);
    _AfxDctxFreeAllInputSlots(dctx);
    _AfxDctxFreeAllOutputSlots(dctx);

    AfxReleaseArena(&dctx->base.aren);
    AfxReleaseObjects(1, (void*[]) { dctx->base.ctx });

    return err;
}

_SGL afxError _SglDctxCtor(afxDrawContext dctx, afxCookie const* cookie)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    //AfxAssert(spec);

    afxDrawContextConfig const *config = ((afxDrawContextConfig const *)cookie->udd[0]) + cookie->no;

    afxDrawDevice ddev = AfxGetObjectProvider(dctx);
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    afxDrawIcd ddrv = AfxGetObjectProvider(ddev);
    afxDrawSystem dsys = AfxGetObjectProvider(ddrv);

    dctx->base.running = FALSE;

    dctx->base.ctx = AfxGetDrawSystemContext(dsys);
    

    AfxAssertObjects(1, &dctx->base.ctx, AFX_FCC_CTX);

    afxContext ctx = dctx->base.ctx;

    AfxAcquireArena(ctx, &dctx->base.aren, NIL, AfxSpawnHint());

    //if (!(AfxObjectReacquire(&dctx->base.memD->obj, &dctx->base.obj, NIL, NIL, NIL))) AfxThrowError();
    //else
    {
        afxChain *classes = &dctx->base.classes;
        AfxAcquireChain(classes, (void*)dctx);

        dctx->base.vmt = NIL;

        afxClassConfig tmpClsConf;

        tmpClsConf = _SglBufClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.buffers, classes, &tmpClsConf);
        tmpClsConf = _SglVbufClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.vertices, classes, &tmpClsConf);
        tmpClsConf = _SglIbufClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.indices, classes, &tmpClsConf);
        tmpClsConf = _SglTexClsConfig;
        tmpClsConf.ctx = ctx;
        AfxMountClass(&dctx->base.textures, classes, &tmpClsConf);
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

        dctx->base.openPortCnt = AfxCountDrawPorts(ddev); //spec && spec->portCnt ? spec->portCnt : 1;

        if (config && config->queueingCnt)
            dctx->base.openPortCnt = config->queueingCnt;

        dctx->base.inputCnt = 0;
        dctx->base.inputCap = 1;
        dctx->base.outputCnt = 0;
        dctx->base.outputCap = 1;

        dctx->base.inputs = NIL;
        dctx->base.outputs = NIL;
        dctx->base.openPorts = NIL;

        if (dctx->base.inputCap && !(dctx->base.inputs = AfxAllocate(ctx, dctx->base.inputCap * sizeof(dctx->base.inputs[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < dctx->base.inputCap; i++)
                dctx->base.inputs[i] = NIL;

            if (dctx->base.outputCap && !(dctx->base.outputs = AfxAllocate(ctx, dctx->base.outputCap * sizeof(dctx->base.outputs[0]), 0, AfxSpawnHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < dctx->base.outputCap; i++)
                    dctx->base.outputs[i] = NIL;

                if (dctx->base.openPortCnt && !(dctx->base.openPorts = AfxAllocate(ctx, dctx->base.openPortCnt * sizeof(dctx->base.openPorts[0]), 0, AfxSpawnHint()))) AfxThrowError();
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

                        dctx->base.vmt = NIL;// &SglDctxVmt;

                        afxUri uri;
                        AfxUriWrapLiteral(&uri, "data/pipeline/rgbaToRgba/rgbaToRgbaPip.xml?yFlipped", 0);
                        //AfxUriWrapLiteral(&uri, "data/pipeline/rgbaToRgbaYFlippedBrokenLens.pip.xml", 0);
                        //dctx->base.presentPip = AfxDrawContextFetchPipeline(dctx, &uri);


                        AfxUploadPipelines(dctx, 1, &dctx->presentPip, &uri);

                        AfxAssertObjects(1, &dctx->presentPip, AFX_FCC_PIP);

                        afxSamplerConfig smpSpec = { 0 };
                        smpSpec.magFilter = AFX_TEXEL_FLT_POINT;
                        smpSpec.minFilter = AFX_TEXEL_FLT_POINT;
                        smpSpec.mipmapFilter = AFX_TEXEL_FLT_POINT;
                        smpSpec.uvw[0] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit
                        smpSpec.uvw[1] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit
                        smpSpec.uvw[2] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit

                        AfxAcquireSamplers(dctx, 1, &dctx->presentSmp, &smpSpec);
                        AfxAssertObjects(1, &dctx->presentSmp, AFX_FCC_SAMP);

                        afxString tmpStr;
                        AfxWrapStringLiteral(&tmpStr, "a_xy", 0);
                        const afxV2d vtxPos[] =
                        {
                            { -1.0,  1.0 },
                            { -1.0, -1.0 },
                            {  1.0,  1.0 },
                            {  1.0, -1.0 },
                        };

                        afxVertexBufferBlueprint vbub;
                        AfxVertexBufferBlueprintReset(&vbub, 4);
                        AfxVertexBufferBlueprintAddRow(&vbub, &tmpStr, AFX_VTX_FMT_V2D, AFX_VTX_USAGE_POS, vtxPos, AFX_VTX_FMT_V2D);
                        AfxBuildVertexBuffers(dctx, 1, &dctx->presentVbuf, &vbub);
                        AfxAssertObjects(1, &dctx->presentVbuf, AFX_FCC_VBUF);

                        dctx->presentFboGpuHandle = 0;

                        //AfxAssert(dctx->base.vmt);

                        if (!err)
                        {
                            dctx->base.running = TRUE;
                        }
                    }

                    if (err && dctx->base.openPorts)
                        _AfxDctxFreeAllQueueSlots(dctx);
                }

                if (err && dctx->base.outputs)
                    _AfxDctxFreeAllOutputSlots(dctx);
            }

            if (err && dctx->base.inputs)
                _AfxDctxFreeAllInputSlots(dctx);
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
    .fcc = AFX_FCC_DCTX,
    .name = "Draw Context",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawContext)),
    .ctx = NIL,
    .ctor = (void*)_SglDctxCtor,
    .dtor = (void*)_SglDctxDtor
};
