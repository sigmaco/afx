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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include "sgl.h"
#include "afx/core/afxTerminal.h"
#include "afx/core/afxSystem.h"
#include "sgl.h"
#include "afx/math/afxVector.h"
#include "afx/core/afxSystem.h"
#include "afx/core/afxString.h"
#include "../e2coree/draw/afxDrawParadigms.h"

AFX afxClassSpecification const _AfxVbufClassSpec;
AFX afxClassSpecification const _AfxIbufClassSpec;
AFX afxClassSpecification const _AfxDopClassSpec;
SGL afxClassSpecification const _AfxBufClassSpec;
SGL afxClassSpecification const _AfxSmpClassSpec;
SGL afxClassSpecification const _AfxTexClassSpec;
SGL afxClassSpecification const _AfxSurfClassSpec;
SGL afxClassSpecification const _AfxCanvClassSpec;
SGL afxClassSpecification const _AfxPipClassSpec;
SGL afxClassSpecification const _AfxShdClassSpec;
SGL afxClassSpecification const _AfxPiprClassSpec;
SGL afxClassSpecification const _AfxLegoClassSpec;
SGL afxClassSpecification const _AfxLegtClassSpec;
SGL afxClassSpecification const _AfxDscrClassSpec;
SGL afxClassSpecification const _AfxDinClassSpec;
SGL afxClassSpecification const _AfxDqueClassSpec;
SGL afxClassSpecification const _AfxDoutClassSpec;

// FOR EACH

_SGL afxResult _AfxDrawContextEnumerateOperations(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawOperation dop[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dopClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)dop);
}

_SGL afxResult _AfxDrawContextEnumerateQueues(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawQueue dque[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dengClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)dque);
}

_SGL afxResult _AfxDrawContextEnumerateInputs(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawInput din[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dinClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)din);
}

_SGL afxResult _AfxDrawContextEnumerateScripts(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawScript dscr[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dscrClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)dscr);
}

_SGL afxResult _AfxDrawContextEnumerateTextures(afxDrawContext dctx, afxNat base, afxNat cnt, afxTexture tex[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->texClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)tex);
}

_SGL afxResult _AfxDrawContextEnumerateIndexBuffers(afxDrawContext dctx, afxNat base, afxNat cnt, afxIndexBuffer ibuf[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->ibufClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)ibuf);
}

_SGL afxResult _AfxDrawContextEnumerateVertexBuffers(afxDrawContext dctx, afxNat base, afxNat cnt, afxVertexBuffer vbuf[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->vbufClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)vbuf);
}

_SGL afxResult _AfxDrawContextEnumerateBuffers(afxDrawContext dctx, afxNat base, afxNat cnt, afxBuffer buf[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->bufClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)buf);
}

_SGL afxResult _AfxDrawContextEnumerateSamplers(afxDrawContext dctx, afxNat base, afxNat cnt, afxSampler smp[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->smpClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)smp);
}

_SGL afxResult _AfxDrawContextEnumerateShaders(afxDrawContext dctx, afxNat base, afxNat cnt, afxShader shd[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->shdClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)shd);
}

_SGL afxResult _AfxDrawContextEnumeratePipelines(afxDrawContext dctx, afxNat base, afxNat cnt, afxPipeline pip[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->pipClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)pip);
}

_SGL afxResult _AfxDrawContextEnumerateSurfaces(afxDrawContext dctx, afxNat base, afxNat cnt, afxSurface surf[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->surfClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)surf);
}

_SGL afxResult _AfxDrawContextEnumerateCanvases(afxDrawContext dctx, afxNat base, afxNat cnt, afxCanvas canv[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->canvClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)canv);
}

_SGL afxResult _AfxDrawContextEnumerateOutputs(afxDrawContext dctx, afxNat base, afxNat cnt, afxDrawOutput dout[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->doutClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)dout);
}

// GET CLASS

_SGL afxClass* _AfxDrawContextGetOperationClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dopClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}


_SGL afxClass* _AfxDrawContextGetSurfaceClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->surfClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetOutputClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->doutClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetScriptClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dscrClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetQueueClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dengClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetInputClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dinClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetTextureClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->texClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetIndexBufferClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->ibufClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetVertexBufferClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->vbufClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetBufferClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->bufClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetSamplerClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->smpClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetShaderClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->shdClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetPipelineRigClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->pipaClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetLegoClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->legoClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetLegoTemplateClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->legtClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetPipelineClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->pipClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetCanvasClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->canvClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxMemory _AfxDrawContextGetMemory(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    return dctx->genrlMem;
}

_SGL afxBool _SglDctxEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = NIL;
    afxDrawContext dctx = (void*)obj;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    (void)ev;
    return FALSE;
}

_SGL afxBool _SglDctxEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = NIL;
    afxDrawContext dctx = (void*)obj;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    (void)watched;
    (void)ev;
    return FALSE;
}

_SGL afxError _AfxDctxDtor(afxDrawContext dctx)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    _AfxDropClassChain(&dctx->provisions);

    if (dctx->queues)
    {
        AfxDeallocate(dctx->genrlMem, dctx->queues);
    }
    
    AfxObjectRelease(&(dctx->genrlMem->obj));

    return AFX_SUCCESS;
}

_SGL afxDctxImpl const _AfxStdDctxImpl;
afxDctxImpl const _AfxStdDctxImpl =
{
    .forEach =
    {
    _AfxDrawContextEnumerateBuffers,
    _AfxDrawContextEnumerateCanvases,
    _AfxDrawContextEnumerateIndexBuffers,
    _AfxDrawContextEnumerateInputs,
    _AfxDrawContextEnumerateOutputs,
    _AfxDrawContextEnumerateShaders,
    _AfxDrawContextEnumeratePipelines,
    _AfxDrawContextEnumerateQueues,
    _AfxDrawContextEnumerateSamplers,
    _AfxDrawContextEnumerateScripts,
    _AfxDrawContextEnumerateSurfaces,
    _AfxDrawContextEnumerateTextures,
    _AfxDrawContextEnumerateVertexBuffers,
    _AfxDrawContextEnumerateOperations
    },
    .getClass =
    {
    _AfxDrawContextGetBufferClass,
    _AfxDrawContextGetCanvasClass,
    _AfxDrawContextGetIndexBufferClass,
    _AfxDrawContextGetInputClass,
    _AfxDrawContextGetOutputClass,
    _AfxDrawContextGetPipelineRigClass,
    _AfxDrawContextGetShaderClass,
    _AfxDrawContextGetPipelineClass,
    _AfxDrawContextGetLegoClass,
    _AfxDrawContextGetLegoTemplateClass,
    _AfxDrawContextGetQueueClass,
    _AfxDrawContextGetSamplerClass,
    _AfxDrawContextGetScriptClass,
    _AfxDrawContextGetSurfaceClass,
    _AfxDrawContextGetTextureClass,
    _AfxDrawContextGetVertexBufferClass,
    _AfxDrawContextGetOperationClass,
    },
    .other =
    {
        _AfxDrawContextAcquireBuffer,
        _AfxDrawContextBuildCanvases,
        _AfxDrawContextAcquireSampler,
        _AfxDrawContextBuildTextures,
        _AfxDrawContextFindTextures,
        _AfxDrawContextAcquireSurface,
        _AfxDrawContextBuildShaders,
        _AfxDrawContextFindShaders,
        _AfxDrawContextAcquirePipelineRig,
        _AfxDrawContextAcquireLegos,
        _AfxDrawContextAcquireLegoTemplate,
        _AfxDrawContextBuildPipeline,
        _AfxDrawContextFindPipeline,
        _AfxDrawContextAcquireQueue,
        _AfxDrawContextAcquireInput,
        _AfxDrawContextAcquireOutput
    },
    _AfxDrawContextGetMemory
};

_SGL afxError _AfxDctxCtor(afxDrawContext dctx, _afxDctxCtorArgs const *args)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawContextSpecification const *spec = args->spec;
    AfxAssert(spec);

    if (!(dctx->genrlMem = spec->genrlMem))
    {
        afxDrawSystem dsys = AfxDrawContextGetDrawSystem(dctx);
        AfxAssertObject(dsys, AFX_FCC_DSYS);
        dctx->genrlMem = AfxDrawSystemGetMemory(dsys);
    }

    AfxAssertObject(dctx->genrlMem, AFX_FCC_MEM);

    if (!(AfxObjectReacquire(&dctx->genrlMem->obj, &dctx->obj, NIL, NIL, NIL))) AfxThrowError();
    else
    {
        afxDrawDriver ddrv = AfxObjectGetProvider(&dctx->obj);
        AfxAssertObject(ddrv, AFX_FCC_DDRV);
        afxDrawSystem dsys = AfxObjectGetProvider(&ddrv->obj);
        AfxAssertObject(dsys, AFX_FCC_DSYS);
        afxSystem sys = AfxObjectGetProvider(&dsys->obj);
        AfxAssertObject(sys, AFX_FCC_SYS);

        afxFileSystem fsys = AfxDrawSystemGetFileSystem(dsys);
        AfxAssertObject(fsys, AFX_FCC_FSYS);

        afxChain *provisions = &dctx->provisions;
        AfxChainDeploy(provisions, dctx);

        afxNat queueCnt = spec->queueCnt ? spec->queueCnt : 1;
        AfxAssert(queueCnt);
        afxClassSpecification tmp = _AfxDqueClassSpec;
        tmp.maxCnt = queueCnt;

        AfxClassRegister(&dctx->dscrClass, provisions, NIL, &_AfxDscrClassSpec);
        AfxClassRegister(&dctx->dinClass, provisions, NIL, &_AfxDinClassSpec);
        AfxClassRegister(&dctx->dengClass, provisions, NIL, &tmp);
        AfxClassRegister(&dctx->bufClass, provisions, NIL, &_AfxBufClassSpec);
        tmp = _AfxVbufClassSpec;
        tmp.vmt = &_SglBufImpl;
        AfxClassRegister(&dctx->vbufClass, provisions, AfxDrawContextGetBufferClass(dctx), &tmp);
        tmp = _AfxIbufClassSpec;
        tmp.vmt = &_SglBufImpl;
        AfxClassRegister(&dctx->ibufClass, provisions, AfxDrawContextGetBufferClass(dctx), &tmp);
        AfxClassRegister(&dctx->smpClass, provisions, NIL, &_AfxSmpClassSpec);
        AfxClassRegister(&dctx->texClass, provisions, AfxFileSystemGetResourceClass(fsys), &_AfxTexClassSpec);
        AfxClassRegister(&dctx->surfClass, provisions, AfxDrawContextGetTextureClass(dctx), &_AfxSurfClassSpec);
        AfxClassRegister(&dctx->canvClass, provisions, NIL, &_AfxCanvClassSpec);
        AfxClassRegister(&dctx->shdClass, provisions, NIL, &_AfxShdClassSpec);
        AfxClassRegister(&dctx->pipaClass, provisions, NIL, &_AfxPiprClassSpec);
        AfxClassRegister(&dctx->pipClass, provisions, AfxFileSystemGetResourceClass(fsys), &_AfxPipClassSpec);
        AfxClassRegister(&dctx->dopClass, provisions, NIL, &_AfxDopClassSpec);
        AfxClassRegister(&dctx->legtClass, provisions, NIL, &_AfxLegtClassSpec);
        AfxClassRegister(&dctx->legoClass, provisions, NIL, &_AfxLegoClassSpec);
        AfxClassRegister(&dctx->doutClass, provisions, NIL, &_AfxDoutClassSpec);

        dctx->queueCnt = 0; // increased by init on each one

        if (!(dctx->queues = AfxAllocate(dctx->genrlMem, sizeof(dctx->queues[0]) * queueCnt, AfxSpawnHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < queueCnt; i++)
            {
                if (!(dctx->queues[i] = _AfxDrawContextAcquireQueue(dctx, i, !!spec->autonomousQueue))) AfxThrowError();
                else
                {
                    ++dctx->queueCnt;
                }
            }

            AfxTransistorDeploy(&dctx->texInstantiationLock);
            AfxTransistorDeploy(&dctx->shdInstantiationLock);

            dctx->open = TRUE;

            if (spec->autonomousQueue)
            {
                for (afxNat i = 0; i < dctx->queueCnt; i++)
                {
                    AfxThreadStart(dctx->queues[i]->thread);
                    AfxYield();
                }
            }

            if (err)
            {
                AfxAssert(dctx->queues);

                for (afxNat i = 0; i < dctx->queueCnt; i++)
                    while (0 < AfxObjectRelease(&dctx->queues[i]->obj));

                AfxDeallocate(dctx->genrlMem, dctx->queues);
            }
        }

        if (err)
        {
            _AfxDropClassChain(&dctx->provisions);

            AfxObjectRelease(&(dctx->genrlMem->obj));
        }
    }
    return err;
}
