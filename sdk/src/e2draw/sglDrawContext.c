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
    AfxAssertObject(din, AFX_FCC_DIN);
    afxDrawThread dthr = udd;
    AfxAssertObject(dthr, AFX_FCC_DTHR);
    
    if (din->vmt->proc(din, dthr))
        AfxThrowError();

    return FALSE; // don't interrupt curation;
}

_SGL afxBool _SglProcessOutputCb(afxDrawOutput dout, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    afxDrawThread dthr = udd;
    AfxAssertObject(dthr, AFX_FCC_DTHR);

    if (dout->vmt->proc(dout, dthr))
        AfxThrowError();

    return FALSE; // don't interrupt curation;
}

_SGL afxError _AfxDctxVmtProcCb(afxDrawContext dctx, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertObject(dthr, AFX_FCC_DTHR);

    afxNat unitIdx = dthr->portIdx;
    AfxAssertRange(dctx->cachedPortCnt, unitIdx, 1);

    AfxCurateConnectedDrawInputs(dctx, 0, AfxGetConnectedDrawInputCount(dctx), _SglProcessInputCb, dthr);

    for (afxNat i = 0; i < dctx->ports[unitIdx].queueCnt; i++)
    {
        afxDrawQueue dque = dctx->ports[unitIdx].queues[i];
        AfxAssertObject(dque, AFX_FCC_DQUE);

        dthr->dque = dque;
        dthr->queueIdx = i;

        if (dque->vmt->proc(dque, dthr))
            AfxThrowError();

        dthr->dque = NIL;
    }

    AfxCurateConnectedDrawOutputs(dctx, 0, AfxGetConnectedDrawOutputCount(dctx), _SglProcessOutputCb, dthr);

    return err;
}

_SGL afxError _AfxDctxVmtDinCb(afxDrawContext dctx, afxDrawInput din, afxBool connect)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertObject(din, AFX_FCC_DIN);
    afxNat slotIdx = AFX_INVALID_INDEX;

    if (dctx->inputs)
    {
        for (afxNat i = 0; i < dctx->inputCap; i++)
        {
            if (din == dctx->inputs[i])
            {
                slotIdx = i;
                break;
            }
        }
    }

    if (slotIdx != AFX_INVALID_INDEX)
    {
        AfxAssert(dctx->inputs);

        if (connect)
        {
            AfxThrowError(); // already connected to connect
        }
        else
        {
            dctx->inputs[slotIdx] = NIL; // disconnect
            --dctx->inputCnt;
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
            if (dctx->inputCap > dctx->inputCnt)
            {
                for (afxNat i = 0; i < dctx->inputCap; i++)
                {
                    if (!dctx->inputs[i])
                    {
                        dctx->inputs[i] = din;
                        ++dctx->inputCnt;
                        break;
                    }
                }
            }
            else
            {
                AfxAssert(dctx->inputCap == dctx->inputCnt);
                afxMemory mem = AfxGetDrawContextMemory(dctx);
                AfxAssertObject(mem, AFX_FCC_MEM);
                void *boundDins = AfxReallocate(mem, dctx->inputs, (dctx->inputCap + 1) * sizeof(dctx->inputs[0]), 0, AfxSpawnHint());

                if (!boundDins) AfxThrowError();
                else
                {
                    dctx->inputs = boundDins;
                    dctx->inputs[dctx->inputCnt] = din;
                    ++dctx->inputCap;
                    ++dctx->inputCnt;
                }
            }
        }
    }
    return err;
}

_SGL afxError _AfxDctxVmtDoutCb(afxDrawContext dctx, afxDrawOutput dout, afxBool connect)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertObject(dout, AFX_FCC_DOUT);
    afxNat slotIdx = AFX_INVALID_INDEX;

    if (dctx->outputs)
    {
        for (afxNat i = 0; i < dctx->outputCap; i++)
        {
            if (dout == dctx->outputs[i])
            {
                slotIdx = i;
                break;
            }
        }
    }

    if (slotIdx != AFX_INVALID_INDEX)
    {
        AfxAssert(dctx->outputs);

        if (connect)
        {
            AfxThrowError(); // already connected to connect
        }
        else
        {
            dctx->outputs[slotIdx] = NIL; // disconnect
            --dctx->outputCnt;
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
            if (dctx->outputCap > dctx->outputCnt)
            {
                for (afxNat i = 0; i < dctx->outputCap; i++)
                {
                    if (!dctx->outputs[i])
                    {
                        dctx->outputs[i] = dout;
                        ++dctx->outputCnt;
                        break;
                    }
                }
            }
            else
            {
                AfxAssert(dctx->outputCap == dctx->outputCnt);
                afxMemory mem = AfxGetDrawContextMemory(dctx);
                AfxAssertObject(mem, AFX_FCC_MEM);
                void *boundDouts = AfxReallocate(mem, dctx->outputs, (dctx->outputCap + 1) * sizeof(dctx->outputs[0]), 0, AfxSpawnHint());

                if (!boundDouts) AfxThrowError();
                else
                {
                    dctx->outputs = boundDouts;
                    dctx->outputs[dctx->outputCnt] = dout;
                    ++dctx->outputCap;
                    ++dctx->outputCnt;
                }
            }
        }
    }
    return err;
}

_SGL afxError _SglDctxVmtDtor(afxDrawContext dctx)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    sglDctxIdd*idd = dctx->idd;

    /// Deinstantiate all items here

    AfxDeallocate(dctx->mem, dctx->idd);
    dctx->idd = NIL;

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

_SGL afxError _SglDdrvVmtDctxCb(afxDrawContext dctx, afxDrawContextSpecification const *spec)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    //AfxAssert(spec);

    afxDrawDriver ddrv = AfxGetDrawContextDriver(dctx);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    
    dctx->vmt = &SglDctxVmt;

    sglDctxIdd *idd = AfxAllocate(dctx->mem, sizeof(*idd), 0, AfxSpawnHint());
    dctx->idd = idd;

    afxUri uri;
    AfxUriWrapLiteral(&uri, "data/pipeline/rgbaToRgba.xml", 0);
    //AfxUriWrapLiteral(&uri, "data/pipeline/rgbaToRgbaYFlippedBrokenLens.pip.xml", 0);
    //idd->presentPip = AfxDrawContextFetchPipeline(dctx, &uri);

    AfxAcquireDrawOperations(dctx, 1, &uri, &idd->presentDop);

    AfxAssertObject(idd->presentDop, AFX_FCC_DOP);

    afxSamplerSpecification smpSpec = { 0 };
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
