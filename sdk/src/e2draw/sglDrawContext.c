#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "sgl.h"
#include "afx/core/afxTerminal.h"
#include "afx/core/afxSystem.h"
#include "sgl.h"
#include "afx/math/afxVector.h"
#include "afx/core/afxSystem.h"
#include "afx/core/base/afxString.h"
#include "../e2coree/draw/afxDrawParadigms.h"

SGL afxClassSpecification const _AfxBufClassSpec;
SGL afxClassSpecification const _AfxVbufClassSpec;
SGL afxClassSpecification const _AfxIbufClassSpec;
SGL afxClassSpecification const _AfxSmpClassSpec;
SGL afxClassSpecification const _AfxTexClassSpec;
SGL afxClassSpecification const _AfxSurfClassSpec;
SGL afxClassSpecification const _AfxCanvClassSpec;
SGL afxClassSpecification const _AfxPipClassSpec;
SGL afxClassSpecification const _AfxPipmClassSpec;
SGL afxClassSpecification const _AfxPiprClassSpec;
SGL afxClassSpecification const _AfxLegoClassSpec;
SGL afxClassSpecification const _AfxDscrClassSpec;
SGL afxClassSpecification const _AfxDinClassSpec;
SGL afxClassSpecification const _AfxDqueClassSpec;
SGL afxClassSpecification const _AfxDoutClassSpec;

// FOR EACH

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

_SGL afxResult _AfxDrawContextEnumeratePipelineModules(afxDrawContext dctx, afxNat base, afxNat cnt, afxPipelineModule pipm[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->pipmClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    AfxAssert(AfxClassGetInstanceCount(cls) >= base + cnt);
    return AfxClassEnumerateInstances(cls, FALSE, base, cnt, (afxObject**)pipm);
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

_SGL afxClass* _AfxDrawContextGetPipelineModuleClass(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->pipmClass;
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

_SGL afxAllocator _AfxDrawContextGetAllocator(afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    return dctx->genrlAll;
}

_SGL afxError _AfxDctxDtor(afxDrawContext dctx)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    _AfxDropClassChain(&dctx->provisions);

    if (dctx->queues)
    {
        AfxDeallocate(dctx->genrlAll, dctx->queues);
    }
    
    AfxObjectRelease(&(dctx->genrlAll->obj));

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
    _AfxDrawContextEnumeratePipelineModules,
    _AfxDrawContextEnumeratePipelines,
    _AfxDrawContextEnumerateQueues,
    _AfxDrawContextEnumerateSamplers,
    _AfxDrawContextEnumerateScripts,
    _AfxDrawContextEnumerateSurfaces,
    _AfxDrawContextEnumerateTextures,
    _AfxDrawContextEnumerateVertexBuffers
    },
    .getClass =
    {
    _AfxDrawContextGetBufferClass,
    _AfxDrawContextGetCanvasClass,
    _AfxDrawContextGetIndexBufferClass,
    _AfxDrawContextGetInputClass,
    _AfxDrawContextGetOutputClass,
    _AfxDrawContextGetPipelineRigClass,
    _AfxDrawContextGetPipelineModuleClass,
    _AfxDrawContextGetPipelineClass,
    _AfxDrawContextGetLegoClass,
    _AfxDrawContextGetQueueClass,
    _AfxDrawContextGetSamplerClass,
    _AfxDrawContextGetScriptClass,
    _AfxDrawContextGetSurfaceClass,
    _AfxDrawContextGetTextureClass,
    _AfxDrawContextGetVertexBufferClass
    },
    .other =
    {
        _AfxDrawContextAcquireBuffer,
        _AfxDrawContextAcquireCanvas,
        _AfxDrawContextAcquireIndexBuffer,
        _AfxDrawContextAcquireSampler,
        _AfxDrawContextBuildVertexBuffer,
        _AfxDrawContextBuildTextures,
        _AfxDrawContextFindTextures,
        _AfxDrawContextAcquireSurface,
        _AfxDrawContextBuildPipelineModule,
        _AfxDrawContextFetchPipelineModule,
        _AfxDrawContextFindPipelineModule,
        _AfxDrawContextUploadPipelineModule,
        _AfxDrawContextBuildPipelineRig,
        _AfxDrawContextAcquireLego,
        _AfxDrawContextBuildPipeline,
        _AfxDrawContextFetchPipeline,
        _AfxDrawContextFindPipeline,
        _AfxDrawContextUploadPipeline,
        _AfxDrawContextAcquireQueue,
        _AfxDrawContextAcquireInput,
        _AfxDrawContextAcquireOutput
    },
    _AfxDrawContextGetAllocator
};

_SGL afxError _AfxDctxCtor(afxDrawContext dctx, afxDrawContextSpecification const *spec)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(spec);

    afxDrawDriver ddrv = AfxObjectGetProvider(&dctx->obj);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxDrawSystem dsys = AfxObjectGetProvider(&ddrv->obj);
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    afxSystem sys = AfxObjectGetProvider(&dsys->obj);
    AfxAssertObject(sys, AFX_FCC_SYS);

    afxChain *provisions = &dctx->provisions;
    AfxChainDeploy(provisions, dctx);

    afxAllocationStrategy as;
    as.size = sizeof(afxByte);
    as.align = AFX_SIMD_ALIGN;
    as.cap = 0;
    as.duration = AFX_ALL_DUR_TRANSIENT;
    as.resizable = TRUE;
    as.stock = sys->memPageSize * 8192; // 32MB

    if (!(dctx->genrlAll = AfxSystemAcquireAllocator(sys, &as, AfxSpawnHint()))) AfxThrowError();
    else
    {
        afxNat queueCnt = spec->queueCnt ? spec->queueCnt : 1;
        AfxAssert(queueCnt);
        afxClassSpecification tmp = _AfxDqueClassSpec;
        tmp.maxCnt = queueCnt;

        AfxClassRegister(&dctx->dscrClass, provisions, NIL, &_AfxDscrClassSpec);
        AfxClassRegister(&dctx->dinClass, provisions, NIL, &_AfxDinClassSpec);
        AfxClassRegister(&dctx->dengClass, provisions, NIL, &tmp);
        AfxClassRegister(&dctx->bufClass, provisions, NIL, &_AfxBufClassSpec);
        AfxClassRegister(&dctx->vbufClass, provisions, AfxDrawContextGetBufferClass(dctx), &_AfxVbufClassSpec);
        AfxClassRegister(&dctx->ibufClass, provisions, AfxDrawContextGetBufferClass(dctx), &_AfxIbufClassSpec);
        AfxClassRegister(&dctx->smpClass, provisions, NIL, &_AfxSmpClassSpec);
        AfxClassRegister(&dctx->texClass, provisions, AfxSystemGetResourceClass(sys), &_AfxTexClassSpec);
        AfxClassRegister(&dctx->surfClass, provisions, AfxDrawContextGetTextureClass(dctx), &_AfxSurfClassSpec);
        AfxClassRegister(&dctx->canvClass, provisions, NIL, &_AfxCanvClassSpec);
        AfxClassRegister(&dctx->pipmClass, provisions, NIL, &_AfxPipmClassSpec);
        AfxClassRegister(&dctx->pipaClass, provisions, NIL, &_AfxPiprClassSpec);
        AfxClassRegister(&dctx->pipClass, provisions, AfxSystemGetResourceClass(sys), &_AfxPipClassSpec);
        AfxClassRegister(&dctx->legoClass, provisions, NIL, &_AfxLegoClassSpec);
        AfxClassRegister(&dctx->doutClass, provisions, NIL, &_AfxDoutClassSpec);

        dctx->queueCnt = 0; // increased by init on each one

        if (!(dctx->queues = AfxAllocate(dctx->genrlAll, sizeof(dctx->queues[0]) * queueCnt, AfxSpawnHint()))) AfxThrowError();
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
            AfxTransistorDeploy(&dctx->pipmInstantiationLock);

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

                AfxDeallocate(dctx->genrlAll, dctx->queues);
            }
        }

        if (err)
        {
            _AfxDropClassChain(&dctx->provisions);
        }

        if (err)
            AfxObjectRelease(&(dctx->genrlAll->obj));
    }
    return err;
}
