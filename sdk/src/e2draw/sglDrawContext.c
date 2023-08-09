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
#define _AFX_THREAD_C
#define _AFX_DRAW_QUEUE_C
#define _AFX_DRAW_CONTEXT_C
#include "sgl.h"
#include "afx/core/afxTerminal.h"
#include "afx/core/afxSystem.h"
#include "sgl.h"
#include "afx/math/afxVector.h"
#include "afx/core/afxSystem.h"
#include "afx/core/afxString.h"
#include "../e2coree/draw/afxDrawParadigms.h"

_SGL afxBool _SglProcessInputCb(afxDrawInput din, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);

    struct _afxDinD *dinD;
    _AfxGetDinD(din, &dinD,dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);
    afxDrawThread dthr = (afxDrawThread)udd;
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    struct _afxDthrD *dthrD;
    _AfxGetDthrD(dthr, &dthrD, dsysD);
    AfxAssertType(dthrD, AFX_FCC_DTHR);
    
    if (dinD->vmt->proc(din, dthr))
        AfxThrowError();

    return FALSE; // don't interrupt curation;
}

_SGL afxBool _SglProcessOutputCb(afxDrawOutput dout, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);

    afxDrawThread dthr = (afxDrawThread)udd;
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    struct _afxDthrD *dthrD;
    _AfxGetDthrD(dthr, &dthrD,dsysD);
    AfxAssertType(dthrD, AFX_FCC_DTHR);

    struct _afxDoutD *doutD;
    _AfxGetDoutD(dout, &doutD,dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    if (doutD->vmt->proc(dout, dthr))
        AfxThrowError();

    return FALSE; // don't interrupt curation;
}

_SGL afxError _AfxDctxVmtProcCb(afxDrawContext dctx, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    struct _afxDthrD *dthrD;
    _AfxGetDthrD(dthr, &dthrD,dsysD);
    AfxAssertType(dthrD, AFX_FCC_DTHR);

    afxNat unitIdx = dthrD->portIdx;
    AfxAssertRange(dctxD->cachedPortCnt, unitIdx, 1);

    AfxCurateConnectedDrawInputs(dctx, 0, AfxGetConnectedDrawInputCount(dctx), _SglProcessInputCb, (void*)dthr);

    for (afxNat i = 0; i < dctxD->ports[unitIdx].queueCnt; i++)
    {
        afxDrawQueue dque = dctxD->ports[unitIdx].queues[i];
        AfxAssertObject(dque, AFX_FCC_DQUE);

        dthrD->dque = dque;
        dthrD->queueIdx = i;

        if (dque->vmt->proc(dque, dthr))
            AfxThrowError();

        dthrD->dque = NIL;
    }

    AfxCurateConnectedDrawOutputs(dctx, 0, AfxGetConnectedDrawOutputCount(dctx), _SglProcessOutputCb, (void*)dthr);

    return err;
}

_SGL afxError _AfxDctxVmtDinCb(afxDrawContext dctx, afxDrawInput din, afxBool connect, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    
    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    afxNat slotIdx2 = AFX_INVALID_INDEX;

    if (dctxD->inputs)
    {
        for (afxNat i = 0; i < dctxD->inputCap; i++)
        {
            if (din == dctxD->inputs[i])
            {
                slotIdx2 = i;
                break;
            }
        }
    }

    if (slotIdx2 != AFX_INVALID_INDEX)
    {
        AfxAssert(dctxD->inputs);

        if (connect)
        {
            AfxThrowError(); // already connected to connect
        }
        else
        {
            dctxD->inputs[slotIdx2] = NIL; // disconnect
            --dctxD->inputCnt;
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
            if (dctxD->inputCap > dctxD->inputCnt)
            {
                for (afxNat i = 0; i < dctxD->inputCap; i++)
                {
                    if (!dctxD->inputs[i])
                    {
                        slotIdx2 = i;
                        dctxD->inputs[slotIdx2] = din;
                        ++dctxD->inputCnt;
                        break;
                    }
                }
            }
            else
            {
                AfxAssert(dctxD->inputCap == dctxD->inputCnt);
                afxContext mem = AfxGetDrawContextMemory(dctx);
                AfxAssertObjects(1, &mem, AFX_FCC_MEM);
                void *boundDins = AfxReallocate(mem, dctxD->inputs, (dctxD->inputCap + 1) * sizeof(dctxD->inputs[0]), 0, AfxSpawnHint());

                if (!boundDins) AfxThrowError();
                else
                {
                    dctxD->inputs = boundDins;
                    slotIdx2 = dctxD->inputCnt;
                    dctxD->inputs[slotIdx2] = din;
                    ++dctxD->inputCap;
                    ++dctxD->inputCnt;
                }
            }
        }
    }

    if (slotIdx)
        *slotIdx = slotIdx2;

    return err;
}

_SGL afxError _AfxDctxVmtDoutCb(afxDrawContext dctx, afxDrawOutput dout, afxBool connect, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);


    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

    afxNat slotIdx2 = AFX_INVALID_INDEX;

    if (dctxD->outputs)
    {
        for (afxNat i = 0; i < dctxD->outputCap; i++)
        {
            if (dout == dctxD->outputs[i])
            {
                slotIdx2 = i;
                break;
            }
        }
    }

    if (slotIdx2 != AFX_INVALID_INDEX)
    {
        AfxAssert(dctxD->outputs);

        if (connect)
        {
            AfxThrowError(); // already connected to connect
        }
        else
        {
            dctxD->outputs[slotIdx2] = NIL; // disconnect
            --dctxD->outputCnt;
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
            if (dctxD->outputCap > dctxD->outputCnt)
            {
                for (afxNat i = 0; i < dctxD->outputCap; i++)
                {
                    if (!dctxD->outputs[i])
                    {
                        slotIdx2 = i;
                        dctxD->outputs[slotIdx2] = dout;
                        ++dctxD->outputCnt;
                        break;
                    }
                }
            }
            else
            {
                AfxAssert(dctxD->outputCap == dctxD->outputCnt);
                afxContext mem = AfxGetDrawContextMemory(dctx);
                AfxAssertObjects(1, &mem, AFX_FCC_MEM);
                void *boundDouts = AfxReallocate(mem, dctxD->outputs, (dctxD->outputCap + 1) * sizeof(dctxD->outputs[0]), 0, AfxSpawnHint());

                if (!boundDouts) AfxThrowError();
                else
                {
                    dctxD->outputs = boundDouts;
                    slotIdx2 = dctxD->outputCnt;
                    dctxD->outputs[slotIdx2] = dout;
                    ++dctxD->outputCap;
                    ++dctxD->outputCnt;
                }
            }
        }
    }

    if (slotIdx)
        *slotIdx = slotIdx2;

    return err;
}

_SGL afxError _SglDctxVmtDtor(afxDrawContext dctx)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    sglDctxIdd*idd = dctxD->idd;

    /// Deinstantiate all items here

    AfxDeallocate(dctxD->mem, dctxD->idd);
    dctxD->idd = NIL;

    return err;
}

_SGL _afxDctxVmt SglDctxVmt = 
{
    _SglDctxVmtDtor,
    _AfxDctxVmtProcCb,
    _AfxDctxVmtDoutCb,
    _AfxDctxVmtDinCb,
    _SglDctxVmtDscrCb,
    _AfxCanvCtor,
    _SglTexCtor,
    _AfxSurfCtor,
    _AfxShdCtor,
    _AfxPipCtor,
    _SglBufCtor,
    NIL,
    _AfxSmpCtor
};

_SGL afxError _SglDdrvVmtDctxCb(afxDrawContext dctx, afxDrawContextConfig const *spec)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    //AfxAssert(spec);

    afxDrawDriver ddrv = AfxGetDrawContextDriver(dctx);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    
    dctxD->vmt = &SglDctxVmt;

    sglDctxIdd *idd = AfxAllocate(dctxD->mem, sizeof(*idd), 0, AfxSpawnHint());
    dctxD->idd = idd;
    *idd = (sglDctxIdd) { 0 };

    afxUri uri;
    AfxUriWrapLiteral(&uri, "data/pipeline/rgbaToRgba/rgbaToRgbaPip.xml?yFlipped", 0);
    //AfxUriWrapLiteral(&uri, "data/pipeline/rgbaToRgbaYFlippedBrokenLens.pip.xml", 0);
    //idd->presentPip = AfxDrawContextFetchPipeline(dctx, &uri);

    
    AfxUploadPipelines(dctx, 1, &uri, &idd->presentPip);

    AfxAssertObject(idd->presentPip, AFX_FCC_PIP);

    afxSamplerConfig smpSpec = { 0 };
    smpSpec.magFilter = AFX_TEXEL_FLT_POINT;
    smpSpec.minFilter = AFX_TEXEL_FLT_POINT;
    smpSpec.mipmapFilter = AFX_TEXEL_FLT_POINT;
    smpSpec.uvw[0] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit
    smpSpec.uvw[1] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit
    smpSpec.uvw[2] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit

    AfxAcquireSamplers(dctx, 1, &smpSpec, &idd->presentSmp);
    AfxAssertObject(idd->presentSmp, AFX_FCC_SAMP);

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
    AfxBuildVertexBuffers(dctx, 1, &vbub, &idd->presentVbuf);
    AfxAssertObject(idd->presentVbuf, AFX_FCC_VBUF);

    idd->presentFboGpuHandle = 0;


    return err;
}
