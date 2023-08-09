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

#define _AFX_DRAW_DRIVER_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_OUTPUT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_SYSTEM_C
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

_AFX afxNat AfxEnumerateDrawScripts(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawScript dscr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawScriptClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DSCR);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)dscr);
}

_AFX afxNat AfxEnumerateDrawQueues(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawQueue dque[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxClass *cls = AfxGetDrawQueueClass(ddrv);
    AfxAssertClass(cls, AFX_FCC_DQUE);
    AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)dque);
}

struct _DdrvCurateProxyCb
{
    afxDrawDriver ddrv;
    union
    {
        afxBool(*f)(void*, void*);
        afxBool(*fdctx)(afxDrawContext, void*);
        afxBool(*fdout)(afxDrawOutput, void*);
        afxBool(*fdin)(afxDrawInput, void*);
    };
    void *udd;
};

_AFX afxBool _AfxDdrvCurateProxyDctxCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = (afxDrawContext)obj;
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
    struct _DdrvCurateProxyCb const *proxy = udd;
    return dctxD->ddrv != proxy->ddrv || proxy->fdctx(dctx, proxy->udd); // don't interrupt curation;
}

_AFX afxBool _AfxDdrvCurateProxyDoutCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (afxDrawOutput)obj;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    struct _afxDoutD *doutD;
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return doutD->ddrv != proxy->ddrv || proxy->fdout(dout, proxy->udd); // don't interrupt curation;
}

_AFX afxBool _AfxDdrvCurateProxyDinCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawInput din = (afxDrawInput)obj;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    struct _afxDinD *dinD;
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return dinD->ddrv != proxy->ddrv || proxy->fdin(din, proxy->udd); // don't interrupt curation;
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

    if (AfxClassAcquireObjects(AfxGetDrawDriverClass(), NIL, 1, spec, (afxInstance**)ddrv, AfxSpawnHint()))
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

_AFX afxBool _AfxDdrvEventHandler(afxInstance *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDriver ddrv = (void*)obj;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxDdrvEventFilter(afxInstance *obj, afxInstance *watched, afxEvent *ev)
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

    //afxContext mem = AfxGetDrawMemory();

    _AfxDropClassChain(&ddrv->provisions);

    if (ddrv->vmt->dtor && ddrv->vmt->dtor(ddrv))
        AfxThrowError();

    AfxAssert(!ddrv->idd); // ddrv->vmt->dtor deveria dar fim nisto.

    AfxReleaseModules(1, &ddrv->mdle);

    return err;
}

_AFX afxError _AfxDdrvCtor(void *cache, afxNat idx, afxDrawDriver ddrv, afxDrawDriverSpecification const *specs)
{
    AfxEntry("ddrv=%p", ddrv);
    afxError err = AFX_ERR_NONE;
    (void)cache;

    afxDrawDriverSpecification const* spec = &specs[idx];
    AfxAssert(spec);
    
    afxContext mem = AfxGetDrawMemory();

    AfxAssertObjects(1, &spec->mdle, AFX_FCC_MDLE);
    ddrv->mdle = spec->mdle;
    AfxReacquireObjects(1, &ddrv->mdle);

    AfxExcerptString(&ddrv->name, spec->name);
    AfxExcerptString(&ddrv->author, spec->author);
    AfxExcerptString(&ddrv->website, spec->website);
    AfxExcerptString(&ddrv->note, spec->note);

    ddrv->verMajor = spec->verMajor;
    ddrv->verMinor = spec->verMinor;
    ddrv->verPatch = spec->verPatch;

    AfxAssert(spec->features);
    ddrv->features = (void*) spec->features;

    ddrv->portCnt = AfxMaxi(1, spec->execPortCnt);
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
