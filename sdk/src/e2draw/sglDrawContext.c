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
SGL afxClassSpecification const _AfxLegtClassSpec;
SGL afxClassSpecification const _AfxDscrClassSpec;
SGL afxClassSpecification const _AfxDinClassSpec;
SGL afxClassSpecification const _AfxDqueClassSpec;
SGL afxClassSpecification const _AfxDoutClassSpec;

// GET CLASS

_SGL afxClass* _AfxDrawContextGetOperationClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dopClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}


_SGL afxClass* _AfxDrawContextGetSurfaceClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->surfClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetOutputClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->doutClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetScriptClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dscrClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetQueueClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dengClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetInputClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dinClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetTextureClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->texClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetIndexBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->ibufClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetVertexBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->vbufClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->bufClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetSamplerClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->smpClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetShaderClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->shdClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetLegoClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->legtClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetPipelineClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->pipClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxClass* _AfxDrawContextGetCanvasClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->canvClass;
    AfxAssertType(cls, AFX_FCC_CLS);
    return cls;
}

_SGL afxMemory _AfxDrawContextGetMemory(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    return dctx->genrlMem;
}

_SGL afxBool _SglDctxEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = (void*)obj;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    (void)ev;
    return FALSE;
}

_SGL afxBool _SglDctxEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = (void*)obj;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    (void)watched;
    (void)ev;
    return FALSE;
}

_SGL afxError _AfxDctxDtor(afxDrawContext dctx)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
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
    .getClass =
    {
    _AfxDrawContextGetBufferClass,
    _AfxDrawContextGetCanvasClass,
    _AfxDrawContextGetIndexBufferClass,
    _AfxDrawContextGetInputClass,
    _AfxDrawContextGetOutputClass,
    _AfxDrawContextGetShaderClass,
    _AfxDrawContextGetPipelineClass,
    _AfxDrawContextGetLegoClass,
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
        _SglDrawContextFindLego
    },
    _AfxDrawContextGetMemory
};

_SGL afxError _AfxDctxCtor(void *cache, afxNat idx, afxDrawContext dctx, _afxDctxCtorArgs const *args)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawContextSpecification const *spec = args[idx].spec;
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

        AfxClassMount(&dctx->dscrClass, provisions, &_AfxDscrClassSpec);
        AfxClassMount(&dctx->dinClass, provisions, &_AfxDinClassSpec);
        AfxClassMount(&dctx->dengClass, provisions, &tmp);
        AfxClassMount(&dctx->bufClass, provisions, &_AfxBufClassSpec);
        tmp = _AfxVbufClassSpec;
        tmp.vmt = &_SglBufImpl;
        AfxClassMountInherited(&dctx->vbufClass, AfxDrawContextGetBufferClass(dctx), provisions, &tmp);
        tmp = _AfxIbufClassSpec;
        tmp.vmt = &_SglBufImpl;
        AfxClassMountInherited(&dctx->ibufClass, AfxDrawContextGetBufferClass(dctx), provisions, &tmp);
        AfxClassMount(&dctx->smpClass, provisions, &_AfxSmpClassSpec);
        AfxClassMount(&dctx->texClass, provisions, &_AfxTexClassSpec);
        AfxClassMountInherited(&dctx->surfClass, AfxDrawContextGetTextureClass(dctx), provisions, &_AfxSurfClassSpec);
        AfxClassMount(&dctx->canvClass, provisions, &_AfxCanvClassSpec);
        AfxClassMount(&dctx->shdClass, provisions, &_AfxShdClassSpec);
        AfxClassMount(&dctx->pipClass, provisions, &_AfxPipClassSpec);
        AfxClassMount(&dctx->dopClass, provisions, &_AfxDopClassSpec);
        AfxClassMount(&dctx->legtClass, provisions, &_AfxLegtClassSpec);
        AfxClassMount(&dctx->doutClass, provisions, &_AfxDoutClassSpec);

        dctx->queueCnt = 0; // increased by init on each one

        if (!(dctx->queues = AfxAllocate(dctx->genrlMem, sizeof(dctx->queues[0]) * queueCnt, AfxSpawnHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < queueCnt; i++)
            {
                afxDrawQueueSpecification dqueSpec = { 0 };
                dqueSpec.idx = i;
                dqueSpec.autonomous = !!spec->autonomousQueue;

                if (!(dctx->queues[i] = AfxDrawContextAcquireQueue(dctx, &dqueSpec))) AfxThrowError();
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
