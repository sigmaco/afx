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

#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_SYSTEM_C
//#define _AFX_DRAW_DRIVER_C
#include "_classified/afxDrawClassified.h"

AFX afxClassSpecification const _AfxDopClassSpec;
AFX afxClassSpecification const _AfxCanvClassSpec;
AFX afxClassSpecification const _AfxPipClassSpec;
AFX afxClassSpecification const _AfxLegtClassSpec;

AFX afxClassSpecification const _AfxVbufClassSpec;
AFX afxClassSpecification const _AfxIbufClassSpec;
AFX afxClassSpecification const _AfxBufClassSpec;
AFX afxClassSpecification const _AfxSampClassSpec;
AFX afxClassSpecification const _AfxTexClassSpec;
AFX afxClassSpecification const _AfxSurfClassSpec;
AFX afxClassSpecification const _AfxShdClassSpec;

_AFX afxBool _AfxGetDctxD(afxDrawContext dctx, struct _afxDctxD **dctxD, struct _afxDsysD* dsysD)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssert(dctxD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxGetObjectsResidency(1, &dctx, (void**)dctxD, &dsysD->contexts);
}

_AFX void _AfxDctxFreeAllQueueSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    if (dctxD->ports)
    {
        for (afxNat i = 0; i < dctxD->cachedPortCnt; i++)
        {
            for (afxNat j = 0; j < dctxD->ports[i].queueCnt; j++)
            {
                afxDrawQueue dque = dctxD->ports[i].queues[j];

                if (dque)
                {
                    AfxReleaseObject(&dque->obj);
                    dctxD->ports[i].queues[j] = NIL;
                }
            }
            AfxDeallocate(dctxD->mem, dctxD->ports[i].queues);
        }
        AfxDeallocate(dctxD->mem, dctxD->ports);
    }
}

_AFX void _AfxDctxFreeAllInputSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    if (dctxD->inputs)
    {
        for (afxNat i = 0; i < dctxD->inputCap; i++)
        {
            afxDrawInput din = dctxD->inputs[i];

            if (din)
            {
                AfxDisconnectDrawInput(din, NIL);
                dctxD->inputs[i] = NIL;
                --dctxD->inputCnt;
            }
        }
        dctxD->inputCap = 0;
        AfxDeallocate(dctxD->mem, dctxD->inputs);
        dctxD->inputs = NIL;
    }
}

_AFX void _AfxDctxFreeAllOutputSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    if (dctxD->outputs)
    {
        for (afxNat i = 0; i < dctxD->outputCap; i++)
        {
            afxDrawOutput dout = dctxD->outputs[i];

            if (dout)
            {
                AfxDisconnectDrawOutput(dout, NIL);
                dctxD->outputs[i] = NIL;
                --dctxD->outputCnt;
            }
        }
        dctxD->outputCap = 0;
        AfxDeallocate(dctxD->mem, dctxD->outputs);
        dctxD->outputs = NIL;
    }
}

_AFX afxClass* AfxGetTextureClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = &dctxD->texClass;
    AfxAssertClass(cls, AFX_FCC_TEX);
    return cls;
}

_AFX afxClass* AfxGetIndexBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = &dctxD->ibufClass;
    AfxAssertClass(cls, AFX_FCC_IBUF);
    return cls;
}

_AFX afxClass* AfxGetVertexBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = &dctxD->vbufClass;
    AfxAssertClass(cls, AFX_FCC_VBUF);
    return cls;
}

_AFX afxClass* AfxGetBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = &dctxD->bufClass;
    AfxAssertClass(cls, AFX_FCC_BUF);
    return cls;
}

_AFX afxClass* AfxGetSamplerClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = &dctxD->smpClass;
    AfxAssertClass(cls, AFX_FCC_SAMP);
    return cls;
}

_AFX afxClass* AfxGetSurfaceClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = &dctxD->surfClass;
    AfxAssertClass(cls, AFX_FCC_SURF);
    return cls;
}

_AFX afxClass* AfxGetShaderClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = &dctxD->shdClass;
    AfxAssertClass(cls, AFX_FCC_SHD);
    return cls;
}

_AFX afxClass* AfxGetDrawOperationClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = &dctxD->dopClass;
    AfxAssertClass(cls, AFX_FCC_DOP);
    return cls;
}

_AFX afxClass* AfxGetLegoClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = &dctxD->legtClass;
    AfxAssertClass(cls, AFX_FCC_LEGO);
    return cls;
}

_AFX afxClass* AfxGetPipelineClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = &dctxD->pipClass;
    AfxAssertClass(cls, AFX_FCC_PIP);
    return cls;
}

_AFX afxClass* AfxGetCanvasClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = &dctxD->canvClass;
    AfxAssertClass(cls, AFX_FCC_CANV);
    return cls;
}

_AFX afxNat AfxCurateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssertRange(dctxD->outputCnt, first, cnt);
    AfxAssert(f);
    afxNat cnt2 = 0;

    for (afxNat i = 0; i < dctxD->inputCap; i++)
    {
        afxDrawInput din = dctxD->inputs[i];

        if (din)
        {
            //AfxAssertType(dinD, AFX_FCC_DIN);

            ++cnt2;

            if (f(din, udd))
                break;

            if (cnt2 == cnt)
                break;
        }

    }
    return cnt2;
}

_AFX afxNat AfxCurateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssertRange(dctxD->outputCnt, first, cnt);
    AfxAssert(f);
    afxNat cnt2 = 0;

    for (afxNat i = 0; i < dctxD->outputCap; i++)
    {
        afxDrawOutput dout = dctxD->outputs[i];

        if (dout)
        {
            struct _afxDoutD *doutD;
            _AfxGetDoutD(dout, &doutD, dsysD);
            AfxAssertType(doutD, AFX_FCC_DOUT);

            ++cnt2;

            if (f(dout, udd))
                break;

            if (cnt2 == cnt)
                break;
        }

    }
    return cnt2;
}

_AFX afxNat AfxEnumerateTextures(afxDrawContext dctx, afxNat first, afxNat cnt, afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = AfxGetTextureClass(dctx);
    AfxAssertClass(cls, AFX_FCC_TEX);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)tex);
}

_AFX afxNat AfxEnumerateIndexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxIndexBuffer ibuf[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = AfxGetIndexBufferClass(dctx);
    AfxAssertClass(cls, AFX_FCC_IBUF);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)ibuf);
}

_AFX afxNat AfxEnumerateVertexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxVertexBuffer vbuf[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = AfxGetVertexBufferClass(dctx);
    AfxAssertClass(cls, AFX_FCC_VBUF);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)vbuf);
}

_AFX afxNat AfxEnumerateBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxBuffer buf[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = AfxGetBufferClass(dctx);
    AfxAssertClass(cls, AFX_FCC_BUF);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)buf);
}

_AFX afxNat AfxEnumerateSamplers(afxDrawContext dctx, afxNat first, afxNat cnt, afxSampler smp[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = AfxGetSamplerClass(dctx);
    AfxAssertClass(cls, AFX_FCC_SAMP);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)smp);
}

_AFX afxNat AfxEnumerateShaders(afxDrawContext dctx, afxNat first, afxNat cnt, afxShader shd[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = AfxGetShaderClass(dctx);
    AfxAssertClass(cls, AFX_FCC_SHD);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)shd);
}

_AFX afxNat AfxEnumerateSurfaces(afxDrawContext dctx, afxNat first, afxNat cnt, afxSurface surf[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = AfxGetSurfaceClass(dctx);
    AfxAssertClass(cls, AFX_FCC_SURF);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)surf);
}

_AFX afxNat AfxEnumerateDrawOperations(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOperation dop[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = AfxGetDrawOperationClass(dctx);
    AfxAssertClass(cls, AFX_FCC_DOP);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)dop);
}

_AFX afxNat AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pip[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = AfxGetPipelineClass(dctx);
    AfxAssertClass(cls, AFX_FCC_PIP);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)pip);
}

_AFX afxNat AfxEnumerateLegos(afxDrawContext dctx, afxNat first, afxNat cnt, afxLego lego[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = AfxGetLegoClass(dctx);
    AfxAssertClass(cls, AFX_FCC_LEGO);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)lego);
}

_AFX afxNat AfxEnumerateCanvases(afxDrawContext dctx, afxNat first, afxNat cnt, afxCanvas canv[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxClass *cls = AfxGetCanvasClass(dctx);
    AfxAssertClass(cls, AFX_FCC_CANV);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)canv);
}

_AFX afxNat AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOutput dout[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssertRange(dctxD->outputCnt, first, cnt);
    AfxAssert(dout);
    afxNat cnt2 = 0;
    afxNat total = 0;

    for (afxNat i = 0; i < dctxD->outputCap; i++)
    {
        afxDrawOutput dout2 = dctxD->outputs[i];

        if (dout2)
        {
            if (total >= first)
            {
                //AfxAssertObject(dout2, AFX_FCC_DOUT);
                dout[i] = dout2;
                ++cnt2;

            }
            ++total;
        }

    }
    return cnt2;
}

_AFX afxNat AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    AfxAssertRange(dctxD->inputCnt, first, cnt);
    AfxAssert(din);
    afxNat cnt2 = 0;
    afxNat total = 0;

    for (afxNat i = 0; i < dctxD->inputCap; i++)
    {
        afxDrawInput din2 = dctxD->inputs[i];

        if (din2)
        {
            if (total >= first)
            {
                //AfxAssertObject(din2, AFX_FCC_DIN);
                din[i] = din2;
                ++cnt2;

            }
            ++total;
        }

    }
    return cnt2;
}

_AFX afxError AfxDisconnectAllDrawInputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    _AfxDctxFreeAllInputSlots(dctx);
    return err;
}

_AFX afxError AfxDisconnectAllDrawOutputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    _AfxDctxFreeAllOutputSlots(dctx);
    return err;
}

_AFX afxDrawOutput AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssertRange(dctxD->outputCap, slotIdx, 1);
    return dctxD->outputs[slotIdx];
}

_AFX afxDrawInput AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssertRange(dctxD->inputCap, slotIdx, 1);
    return dctxD->inputs[slotIdx];
}

_AFX afxNat AfxGetConnectedDrawOutputCount(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    return dctxD->outputCnt;
}

_AFX afxNat AfxGetConnectedDrawInputCount(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    return dctxD->inputCnt;
}

_AFX afxDrawQueue AfxGetDrawQueue(afxDrawContext dctx, afxNat portIdx, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssertRange(dctxD->cachedPortCnt, portIdx, 1);
    AfxAssertRange(dctxD->ports[portIdx].queueCnt, queIdx, 1);
    return dctxD->ports[portIdx].queues[queIdx];
}

_AFX afxError AfxRequestNextDrawQueue(afxDrawContext dctx, afxNat portIdx, afxTime timeout, afxNat *queIdx)
{
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    AfxAssertRange(dctxD->cachedPortCnt, portIdx, 1);
    AfxAssert(queIdx);
    //AfxAssertRange(dctxD->ports[portIdx].queueCnt, queIdx, 1);
    
    (void)timeout;
    *queIdx = AFX_INVALID_INDEX;

    afxNat idx = dctxD->ports[portIdx].lastReqQueIdx;
    afxTime time, t2;
    AfxGetTime(&time);

    for (;;)
    {
        idx = (idx + 1) % dctxD->ports[portIdx].queueCnt;
        afxDrawQueue dque = dctxD->ports[portIdx].queues[idx];
        AfxAssertObject(dque, AFX_FCC_DQUE);

        if (dque)
        {
            //AfxAssert(!surf->swapchain.chain);
            *queIdx = idx;
            break;
        }

        if (timeout > AfxGetTime(&t2) - time)
        {
            AfxThrowError();
            *queIdx = AFX_INVALID_INDEX;
            break;
        }
    }

    if (!err)
        dctxD->ports[portIdx].lastReqQueIdx = idx;

    return err;
}

_AFX afxNat AfxGetDrawQueueCount(afxDrawContext dctx, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssertRange(dctxD->cachedPortCnt, portIdx, 1);
    return dctxD->ports[portIdx].queueCnt;
}

_AFX afxContext AfxGetDrawContextMemory(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxContext mem = dctxD->mem;
    AfxAssertObjects(1, &mem, AFX_FCC_MEM);
    return mem;
}

_AFX afxDrawDriver AfxGetDrawContextDriver(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxDrawDriver ddrv = dctxD->ddrv;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv;
}

_AFX afxError _AfxDctxDtor(afxDrawContext dctx, struct _afxDctxD *dctxD)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    AfxDisconnectAllDrawInputs(dctx);
    AfxDisconnectAllDrawOutputs(dctx);

    _AfxDropClassChain(&dctxD->provisions);

    _AfxDctxFreeAllQueueSlots(dctx);
    _AfxDctxFreeAllInputSlots(dctx);
    _AfxDctxFreeAllOutputSlots(dctx);
    
    AfxReleaseArena(&dctxD->aren);
    AfxReleaseContexts(1, &dctxD->mem);

    return AFX_SUCCESS;
}

_AFX void _AfxObjDeallocDctx(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("dctx=%p", dctx);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    _AfxDctxDtor(dctx, dctxD);
    AfxDeallocateObjects(1, &dctx, &dsysD->contexts);
}

_AFX void AfxReleaseDrawContexts(afxNat cnt, afxDrawContext dctx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, dctx, AFX_FCC_DCTX);
    AfxReleaseObjects(cnt, dctx, _AfxObjDeallocDctx);
}

_AFX afxError _AfxDctxCtor(afxDrawContext dctx, struct _afxDctxD *dctxD, afxDrawContextConfig const *config)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;

    AfxAssignFcc(dctxD, AFX_FCC_DCTX);
    AfxAssertType(dctxD, AFX_FCC_DCTX);

    afxDrawDriver ddrv;
    AfxGetDrawDriver(config ? config->drvIdx : 0, &ddrv);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    dctxD->ddrv = ddrv;

    dctxD->running = FALSE;

    if (!(dctxD->mem = config->genrlMem))
    {
        dctxD->mem = AfxGetDrawMemory();
    }

    AfxAssertObjects(1, &dctxD->mem, AFX_FCC_MEM);

    AfxAcquireArena(dctxD->mem, &dctxD->aren, NIL, AfxSpawnHint());

    //if (!(AfxObjectReacquire(&dctxD->memD->obj, &dctxD->obj, NIL, NIL, NIL))) AfxThrowError();
    //else
    {
        afxChain *provisions = &dctxD->provisions;
        AfxAcquireChain(provisions, dctx);

        AfxMountClass(&dctxD->bufClass, provisions, &_AfxBufClassSpec);
        AfxMountClassInherited(&dctxD->vbufClass, AfxGetBufferClass(dctx), provisions, &_AfxVbufClassSpec);
        AfxMountClassInherited(&dctxD->ibufClass, AfxGetBufferClass(dctx), provisions, &_AfxIbufClassSpec);
        AfxMountClass(&dctxD->smpClass, provisions, &_AfxSampClassSpec);
        AfxMountClass(&dctxD->texClass, provisions, &_AfxTexClassSpec);
        AfxMountClassInherited(&dctxD->surfClass, AfxGetTextureClass(dctx), provisions, &_AfxSurfClassSpec);
        AfxMountClass(&dctxD->shdClass, provisions, &_AfxShdClassSpec);

        AfxMountClass(&dctxD->legtClass, provisions, &_AfxLegtClassSpec);
        AfxMountClass(&dctxD->canvClass, provisions, &_AfxCanvClassSpec);
        AfxMountClass(&dctxD->pipClass, provisions, &_AfxPipClassSpec);
        AfxMountClass(&dctxD->dopClass, provisions, &_AfxDopClassSpec);

        dctxD->vmt = NIL;
        dctxD->idd = NIL;

        dctxD->cachedPortCnt = AfxGetDrawDriverPortCount(ddrv); //spec && spec->portCnt ? spec->portCnt : 1;
        AfxAssert(dctxD->cachedPortCnt);
        dctxD->inputCnt = 0;
        dctxD->inputCap = 1;
        dctxD->outputCnt = 0;
        dctxD->outputCap = 1;

        dctxD->inputs = NIL;
        dctxD->outputs = NIL;
        dctxD->ports = NIL;

        if (dctxD->inputCap && !(dctxD->inputs = AfxAllocate(dctxD->mem, dctxD->inputCap * sizeof(dctxD->inputs[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < dctxD->inputCap; i++)
                dctxD->inputs[i] = NIL;

            if (dctxD->outputCap && !(dctxD->outputs = AfxAllocate(dctxD->mem, dctxD->outputCap * sizeof(dctxD->outputs[0]), 0, AfxSpawnHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < dctxD->outputCap; i++)
                    dctxD->outputs[i] = NIL;

                if (dctxD->cachedPortCnt && !(dctxD->ports = AfxAllocate(dctxD->mem, dctxD->cachedPortCnt * sizeof(dctxD->ports[0]), 0, AfxSpawnHint()))) AfxThrowError();
                else
                {
                    for (afxNat i = 0; i < dctxD->cachedPortCnt; i++)
                    {
                        dctxD->ports[i].queueCnt = 1;// spec && spec->queuesPerPort && spec->queuesPerPort[i] ? spec->queuesPerPort[i] : 1;
                        dctxD->ports[i].lastReqQueIdx = (dctxD->ports[i].lastReqQueIdx + 1) % dctxD->ports[i].queueCnt;
                        //dctxD->ports[i].minRecyclSubmCnt = 2;
                        dctxD->ports[i].queues = NIL;

                        if (dctxD->ports[i].queueCnt && !(dctxD->ports[i].queues = AfxAllocate(dctxD->mem, dctxD->ports[i].queueCnt * sizeof(dctxD->ports[0].queues), 0, AfxSpawnHint())))
                        {
                            AfxThrowError();

                            for (afxNat j = 0; j < i; j++)
                            {
                                AfxDeallocate(dctxD->mem, dctxD->ports[j].queues);
                                dctxD->ports[j].queues = NIL;
                            }
                        }
                        else
                        {
                            for (afxNat j = 0; j < dctxD->ports[i].queueCnt; j++)
                                dctxD->ports[i].queues[j] = NIL;
                        }
                    }

                    if (!err)
                    {
                        if (ddrv->vmt->dctx && ddrv->vmt->dctx(dctx, config)) AfxThrowError();
                        else
                        {
                            AfxAssert(dctxD->vmt);

                            for (afxNat i = 0; i < dctxD->cachedPortCnt; i++)
                            {
                                afxDrawQueueSpecification dqueSpec = { 0 };
                                dqueSpec.portIdx = i;
                                dqueSpec.owner = dctx;

                                if (AfxAcquireDrawQueues(ddrv, &dqueSpec, dctxD->ports[i].queueCnt, dctxD->ports[i].queues))
                                {
                                    AfxThrowError();

                                    for (afxNat j = 0; j < i; j++)
                                        for (afxNat k = 0; k < dctxD->ports[i].queueCnt; k++)
                                            AfxReleaseObject(&dctxD->ports[j].queues[k]->obj);
                                }
                            }

                            if (!err)
                            {
                                dctxD->running = TRUE;
                            }

                        }
                    }

                    if (err && dctxD->ports)
                        _AfxDctxFreeAllQueueSlots(dctx);
                }

                if (err && dctxD->outputs)
                    _AfxDctxFreeAllOutputSlots(dctx);
            }

            if (err && dctxD->inputs)
                _AfxDctxFreeAllInputSlots(dctx);
        }

        if (err)
        {
            _AfxDropClassChain(&dctxD->provisions);

            AfxReleaseContexts(1, &dctxD->mem);
        }
    }
    return err;
}

_AFX afxError AfxAcquireDrawContexts(afxNat cnt, afxDrawContext dctx[], afxDrawContextConfig const config[])
{
    AfxEntry("cnt=%u,config=%p", cnt, config);
    afxError err = AFX_ERR_NONE;
    afxDrawDriver ddrv;

    if (!AfxGetDrawDriver(config ? config->drvIdx : 0, &ddrv)) AfxThrowError();
    else
    {
        AfxAssertObject(ddrv, AFX_FCC_DDRV);

        afxDrawSystem dsys;
        AfxGetDrawSystem(&dsys);
        AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
        struct _afxDsysD* dsysD;
        _AfxGetDsysD(dsys, &dsysD);
        AfxAssertType(dsysD, AFX_FCC_DSYS);

        if (AfxAcquireNamedObjects(cnt, dctx, AFX_FCC_DCTX, "Draw Context")) AfxThrowError();
        else
        {
            if (AfxAllocateObjects(cnt, dctx, &dsysD->contexts)) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < cnt; i++)
                {
                    struct _afxDctxD *dctxD;
                    _AfxGetDctxD(dctx[i], &dctxD, dsysD);

                    if (_AfxDctxCtor(dctx[i], dctxD, &config[i]))
                    {
                        AfxThrowError();
                        afxNat j = i;

                        for (afxNat k = 0; k < i; k++)
                            if (_AfxDctxDtor(dctx[j], dctxD))
                                AfxThrowError();
                    }
                }
            }

            if (err)
                AfxReleaseObjects(cnt, dctx, NIL);
        }
    }

    if (err)
    {
        for (afxNat i = 0; i < cnt; i++)
            dctx[i] = NIL;
    }
    else
    {
        AfxAssertObjects(cnt, dctx, AFX_FCC_DCTX);
    }
    return err;
}
