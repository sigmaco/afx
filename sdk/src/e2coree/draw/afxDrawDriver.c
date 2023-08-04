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

#define _AFX_DRAW_DRIVER_C
#include "_classified/afxDrawClassified.h"
#include "afx/core/afxSystem.h"

AFX afxClassSpecification const _AfxDctxClassSpec;
AFX afxClassSpecification const _AfxDoutClassSpec;
AFX afxClassSpecification const _AfxDinClassSpec;
AFX afxClassSpecification const _AfxDqueClassSpec;
AFX afxClassSpecification const _AfxDscrClassSpec;

_AFX afxBool AfxDrawDriverIsRunning(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv->serving;
}

_AFX afxClass* AfxGetDrawScriptClass(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = &ddrv->dscrClass;
    AfxAssertClass(cls, AFX_FCC_DSCR);
    return cls;
}

_AFX afxClass* AfxGetDrawQueueClass(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = &ddrv->dqueClass;
    AfxAssertClass(cls, AFX_FCC_DQUE);
    return cls;
}

_AFX afxClass* AfxGetDrawInputClass(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = &ddrv->dinClass;
    AfxAssertClass(cls, AFX_FCC_DIN);
    return cls;
}

_AFX afxClass* AfxGetDrawOutputClass(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = &ddrv->doutClass;
    AfxAssertClass(cls, AFX_FCC_DOUT);
    return cls;
}

_AFX afxClass* AfxGetDrawContextClass(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = &ddrv->dctxClass;
    AfxAssertClass(cls, AFX_FCC_DCTX);
    return cls;
}

_AFX afxNat AfxGetDrawContextCount(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawContextClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DCTX);
    return AfxGetClassInstanceCount(cls);
}

_AFX afxNat AfxGetDrawOutputCount(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawOutputClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DOUT);
    return AfxGetClassInstanceCount(cls);
}

_AFX afxNat AfxGetDrawInputCount(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawInputClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DIN);
    return AfxGetClassInstanceCount(cls);
}

_AFX afxNat AfxEnumerateDrawScripts(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawScript dscr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawScriptClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DSCR);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)dscr);
}

_AFX afxNat AfxEnumerateDrawQueues(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawQueue dque[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawQueueClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DQUE);
    AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)dque);
}

_AFX afxNat AfxEnumerateDrawInputs(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawInputClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DIN);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)din);
}

_AFX afxNat AfxEnumerateDrawOutputs(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawOutput dout[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawOutputClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DOUT);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)dout);
}

_AFX afxNat AfxEnumerateDrawContexts(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawContext dctx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawContextClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DCTX);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)dctx);
}

_AFX afxNat AfxCurateDrawContexts(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawContextClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DCTX);
    return AfxCurateFirstClassInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxCurateDrawOutputs(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawOutputClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DOUT);
    return AfxCurateFirstClassInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxCurateDrawInputs(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawInputClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DIN);
    return AfxCurateFirstClassInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxGetDrawDriverPortCount(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv->portCnt;
}

_AFX afxDrawQueueFlags AfxGetDrawDriverPortCaps(afxDrawDriver ddrv, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    AfxAssertRange(ddrv->portCnt, portIdx, 1);
    return ddrv->portCaps[portIdx];
}

_AFX void AfxGetDrawDriverFeatures(afxDrawDriver ddrv, afxDrawDriverFeatures *features)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    AfxAssert(features);
    *features = *ddrv->features;
}

_AFX afxModule AfxGetDrawDriverModule(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv->mdle;
}

_AFX afxError AfxRegisterDrawDriver(afxDrawDriverSpecification const *spec, afxDrawDriver *ddrv)
{
    AfxEntry("spec=%p", spec);
    afxError err = AFX_ERR_NONE;

    if (AfxClassAcquireObjects(AfxGetDrawDriverClass(), NIL, 1, spec, (afxObject**)ddrv, AfxSpawnHint()))
    {
        AfxThrowError();
        *ddrv = NIL;
    }
    else
    {
        AfxAssertObject(*ddrv, AFX_FCC_DDRV);
    }
    return err;
}

_AFX afxBool _AfxDdrvEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDriver ddrv = (void*)obj;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxDdrvEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDriver ddrv = (void*)obj;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxDdrvDtor(afxDrawDriver ddrv)
{
    AfxEntry("ddrv=%p", ddrv);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    //afxMemory mem = AfxGetDrawMemory();

    _AfxDropClassChain(&ddrv->provisions);

    if (ddrv->vmt->dtor && ddrv->vmt->dtor(ddrv))
        AfxThrowError();

    AfxAssert(!ddrv->idd); // ddrv->vmt->dtor deveria dar fim nisto.

    AfxReleaseObject(&ddrv->mdle->obj);

    return err;
}

_AFX afxError _AfxDdrvCtor(void *cache, afxNat idx, afxDrawDriver ddrv, afxDrawDriverSpecification const *specs)
{
    AfxEntry("ddrv=%p", ddrv);
    afxError err = AFX_ERR_NONE;
    (void)cache;

    afxDrawDriverSpecification const* spec = &specs[idx];
    AfxAssert(spec);
    
    afxMemory mem = AfxGetDrawMemory();

    AfxAssertObject(spec->mdle, AFX_FCC_MDLE);
    ddrv->mdle = spec->mdle;
    AfxObjectReacquire(&ddrv->mdle->obj, &ddrv->obj, NIL, NIL, NIL);

    AfxExcerptString(&ddrv->name, spec->name);
    AfxExcerptString(&ddrv->author, spec->author);
    AfxExcerptString(&ddrv->website, spec->website);
    AfxExcerptString(&ddrv->note, spec->note);

    ddrv->verMajor = spec->verMajor;
    ddrv->verMinor = spec->verMinor;
    ddrv->verPatch = spec->verPatch;

    AfxAssert(spec->features);
    ddrv->features = (void*) spec->features;

    ddrv->portCnt = AfxMax(1, spec->execPortCnt);
    AfxAssert(ddrv->portCnt);
    ddrv->portCaps = AfxAllocate(mem, ddrv->portCnt * sizeof(ddrv->portCaps[0]), 0, AfxSpawnHint());

    ddrv->serving = FALSE;

    for (afxNat i = 0; i < ddrv->portCnt; i++)
        ddrv->portCaps[i] = (afxDrawQueueFlags) { 0 };

    ddrv->vmt = NIL;
    ddrv->idd = NIL;

    afxChain *provisions = &ddrv->provisions;
    AfxAcquireChain(provisions, ddrv);

    AfxMountClass(&ddrv->dscrClass, provisions, &_AfxDscrClassSpec);
    AfxMountClass(&ddrv->dqueClass, provisions, &_AfxDqueClassSpec);
    AfxMountClass(&ddrv->dctxClass, provisions, &_AfxDctxClassSpec);
    AfxMountClass(&ddrv->doutClass, provisions, &_AfxDoutClassSpec);
    AfxMountClass(&ddrv->dinClass, provisions, &_AfxDinClassSpec);

    if (spec->ctor(ddrv, spec)) AfxThrowError();
    else
    {
        AfxAssert(ddrv->vmt);

        AfxLogMessageFormatted(0xFFFF0000, "\nNew draw driver registered.");
        AfxLogMessageFormatted(0xFFFF0000, "\nName: %.*s", AfxPushString(&ddrv->name));
        AfxLogMessageFormatted(0xFFFF0000, "\nAuthor: %.*s", AfxPushString(&ddrv->author));
        AfxLogMessageFormatted(0xFFFF0000, "\nVersion: %u.%u.%u", ddrv->verMajor, ddrv->verMinor, ddrv->verPatch);
        AfxLogMessageFormatted(0xFFFF0000, "\nNote: %.*s", AfxPushString(&ddrv->note));

        ddrv->serving = TRUE;
    }

    if (err)
        _AfxDropClassChain(&ddrv->provisions);

    return err;
}

AFX afxClassSpecification const _AfxDdrvClassSpec;

afxClassSpecification const _AfxDdrvClassSpec =
{
    AFX_FCC_DDRV,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawDriver)),
    NIL,
    (void*)_AfxDdrvCtor,
    (void*)_AfxDdrvDtor,
    .event = _AfxDdrvEventHandler,
    .eventFilter = _AfxDdrvEventFilter,
    "afxDrawDriver",
    NIL
};
