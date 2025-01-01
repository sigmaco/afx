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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_CORE_C
#define _AFX_SCRIPT_C
#include "../dev/afxIoImplKit.h"
#include "qwadro/inc/xss/xss.h"
#include "qwadro/inc/xss/afxScript.h"

AUX xssVm xssCreateVm(LunaConfiguration const* config);
AUX afxError xssDestroyVm(xssVm vm);

_AUX void GenAcqObj(xssVm vm)
{
    afxObject* ph = xssSetSlotNewForeign(vm, 0, 0, sizeof(afxObject));
    *ph = NIL;
}

_AUX void GenRelObj(void* data)
{
    AfxDisposeObjects(1, data);
    *(void**)data = NIL;
}

_AUX void XssPushError(xssVm vm, afxUnit slot, afxError err)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, err);
}

_AUX void XssPushBool(xssVm vm, afxUnit slot, afxBool val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotBool(vm, slot, val);
}

_AUX void XssPushReal(xssVm vm, afxUnit slot, afxReal val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, val);
}

_AUX void XssPushReal64(xssVm vm, afxUnit slot, afxReal64 val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, val);
}

_AUX void XssPushInt(xssVm vm, afxUnit slot, afxInt val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, val);
}

_AUX void XssPushNat(xssVm vm, afxUnit slot, afxUnit val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, val);
}

_AUX void XssPushSize(xssVm vm, afxUnit slot, afxSize val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, val);
}

_AUX afxBool XssPullBool(xssVm vm, afxUnit slot)
{
    return (afxBool)xssGetSlotDouble(vm, slot);
}

_AUX afxReal XssPullReal(xssVm vm, afxUnit slot)
{
    return (afxReal)xssGetSlotDouble(vm, slot);
}

_AUX afxReal64 XssPullReal64(xssVm vm, afxUnit slot)
{
    return (afxReal64)xssGetSlotDouble(vm, slot);
}

_AUX void XssPullReal3(xssVm vm, afxUnit slot, afxV3d v)
{
    v[0] = (afxReal)xssGetSlotDouble(vm, slot);
    v[1] = (afxReal)xssGetSlotDouble(vm, slot + 1);
    v[2] = (afxReal)xssGetSlotDouble(vm, slot + 2);
}

_AUX afxInt XssPullInt(xssVm vm, afxUnit slot)
{
    return (afxInt)xssGetSlotDouble(vm, slot);
}

_AUX afxUnit XssPullNat(xssVm vm, afxUnit slot)
{
    return (afxUnit)xssGetSlotDouble(vm, slot);
}

_AUX afxSize XssPullSize(xssVm vm, afxUnit slot)
{
    return (afxSize)xssGetSlotDouble(vm, slot);
}

_AUX afxHandle XssPullHandle(xssVm vm, afxUnit slot)
{
    return (afxHandle)xssGetSlotHandle(vm, slot);
}

_AUX void XssPushHandle(xssVm vm, afxUnit slot, afxHandle han)
{
    afxError err;
    xssSetSlotHandle(vm, slot, han);
}

_AUX void* XssPullInstance(xssVm vm, afxUnit slot)
{
    return *(void**)xssGetSlotForeign(vm, slot);
}

_AUX void XssPushInstance(xssVm vm, afxUnit slot, void* obj)
{
    afxError err;
    AFX_ASSERT(slot == 0);
    *(void**)xssSetSlotNewForeign(vm, 0, 0, sizeof(afxObject)) = obj;
}

_AUX void XssPullUri(xssVm vm, afxUnit slot, afxUri* uri)
{
    const char* s = xssGetSlotString(vm, slot);
    AfxMakeUri(uri, 0, s, 0);
}

_AUX void XssPullString(xssVm vm, afxUnit slot, afxString* s)
{
    const char* src = xssGetSlotString(vm, slot);
    AfxMakeString(s, 0, src, 0);
}

_AUX void XssUnhandle(xssVm vm, afxUnit cnt, afxHandle han[])
{
    for (afxUnit i = cnt; i-- > 0;)
    {
        xssReleaseHandle(vm, han[i]);
        han[i] = NIL;
    }
}

_AUX void XssHandleEachCall(xssVm vm, afxUnit cnt, afxString const signatures[], afxHandle handles[])
{
    afxString2048 sig;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxMakeString2048(&sig, &signatures[i]);

        if (!(handles[i] = (afxHandle)xssMakeCallHandle(vm, sig.buf)))
        {
            XssUnhandle(vm, i, handles);
        }
    }
}

_AUX afxUnit XssHandleAnyCall(xssVm vm, afxUnit cnt, afxString const signatures[], afxHandle handles[])
{
    afxUnit rslt = 0;

    afxString2048 sig;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxMakeString2048(&sig, &signatures[i]);

        if ((handles[i] = (afxHandle)xssMakeCallHandle(vm, sig.buf)))
            ++rslt;
    }
    return rslt;
}

_AUX afxUnit XssHandleAnyVar(xssVm vm, afxString const* domain, afxUnit baseSlot, afxUnit cnt, afxString const name[], afxHandle handles[])
{
    afxUnit rslt = 0;

    afxString32 dom, nam;
    AfxMakeString32(&dom, domain);

    xssEnsureSlots(vm, cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxMakeString32(&nam, &name[i]);

        if (xssHasVariable(vm, dom.buf, nam.buf))
        {
            xssGetVariable(vm, dom.buf, nam.buf, baseSlot + i);

            if (xssGetSlotType(vm, baseSlot + i) == LUNA_TYPE_UNKNOWN)
            {
                handles[i] = xssGetSlotHandle(vm, baseSlot + i);
                ++rslt;
            }
        }
    }
    return rslt;
}

_AUX afxError XssCall(xssVm vm, afxHandle method)
{
    return xssCall(vm, method);
}

_AUX void AfxGetScriptTime(afxScript xss, afxReal64* ct, afxReal64* dt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_XSS, 1, &xss);
    afxClock currClock;
    AfxGetClock(&currClock);
    AFX_ASSERT(ct);
    *ct = AfxGetSecondsElapsed(&xss->startClock, &currClock);
    AFX_ASSERT(dt);
    *dt = AfxGetSecondsElapsed(&xss->lastClock, &currClock);
    xss->lastClock = currClock;
}

_AUX afxBool _AuxXssEvent(afxScript xss, auxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_XSS, 1, &xss);

    switch (ev->id)
    {
    case auxEventId_RUN:
    {
        if (xss->xssInitFn)
        {
            XssPushHandle(xss->vm, 0, xss->xssMainVar);
            XssCall(xss->vm, xss->xssInitFn);
        }
        break;
    }
    case auxEventId_QUIT:
    {
        if (xss->xssQuitFn)
        {
            XssPushHandle(xss->vm, 0, xss->xssMainVar);
            XssCall(xss->vm, xss->xssQuitFn);
        }
        break;
    }
    default:
    {
        if (xss->xssStepFn)
        {
            afxReal64 ct, dt;
            AfxGetScriptTime(xss, &ct, &dt);

            XssPushHandle(xss->vm, 0, xss->xssMainVar);
            XssPushReal(xss->vm, 1, dt);
            XssCall(xss->vm, xss->xssStepFn);
        }
        break;
    }
    }

    return err;
}

_AUX afxError _AuxXssCtor(afxScript xss, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_XSS, 1, &xss);

    afxShell sh = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_USYS, 1, &sh);
    afxString const* domain = ((afxString const*)args[1]) + invokeNo;
    afxString const* code = ((afxString const*)args[2]) + invokeNo;

    AfxResetStrings(1, &xss->path);

    xss->argc = 0;
    xss->argv = NIL;

    LunaConfiguration cfg = { 0 };    
    xss->vm = xssCreateVm(&cfg);

    AfxGetClock(&xss->startClock);
    xss->lastClock = xss->startClock;

    return err;
}

_AUX afxError _AuxXssDtor(afxScript xss)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_XSS, 1, &xss);

    if (xss->vm)
        xssDestroyVm(xss->vm);

    return err;
}

_AUX afxClassConfig const _AuxXssStdImplementation =
{
    .fcc = afxFcc_XSS,
    .name = "Script",
    .desc = "Cross-System Scripting Execution Environment",
    .fixedSiz = sizeof(AFX_OBJECT(afxScript)),
    .ctor = (void*)_AuxXssCtor,
    .dtor = (void*)_AuxXssDtor,
    .event = (void*)_AuxXssEvent
};

////////////////////////////////////////////////////////////////////////////////

#if 0
_AUX afxError AfxAcquireScript(afxString const* domain, afxString const* code, afxScript* script)
{
    afxError err = AFX_ERR_NONE;

    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, sh, afxFcc_USYS);
    afxClass* cls = AfxGetScriptClass();
    AFX_ASSERT_CLASS(cls, afxFcc_XSS);

    if (AfxAcquireObjects(cls, 1, (void*)script, ((void const*[]) { sh, domain, code })))
        AfxThrowError();

    AfxAssertObjects(1, script, afxFcc_XSS);
    afxResult r;

    afxResult xRslt;

    xRslt = AfxInjectScript(domain, code);
    AFX_ASSERT(!xRslt);
#if 0
    afxHandle fn[4];
    afxString fns[4];
    AfxMakeString(&fns[0], "init()", 0);
    AfxMakeString(&fns[1], "quit()", 0);
    AfxMakeString(&fns[2], "step(_)", 0);
    AfxMakeString(&fns[3], "draw(_,_)", 0);
    XssHandleAnyCall(sh->vm, 4, fns, fn);

    LunaHandle* initMethod = NULL;
    xssEnsureSlots(sh->vm, 3);
    initMethod = xssMakeCallHandle(sh->vm, "init()");

    if (!xssHasVariable(sh->vm, domain->start, "Game"))
    {
        AfxThrowError();
    }

    xssGetVariable(sh->vm, domain->start, "Game", 0);

    if (xssGetSlotType(sh->vm, 0) != LUNA_TYPE_UNKNOWN)
    {
        AfxThrowError();
    }

    LunaHandle* gameClass = xssGetSlotHandle(sh->vm, 0);
    LunaHandle* updateMethod = xssMakeCallHandle(sh->vm, "update()");
    LunaHandle* drawMethod = xssMakeCallHandle(sh->vm, "draw(_)");

    xssSetSlotHandle(sh->vm, 0, gameClass);
    xRslt = xssCall(sh->vm, initMethod);

    xssReleaseHandle(sh->vm, initMethod);
    initMethod = NULL;
#endif
    return err;
}
#endif

_AUX afxError AfxLoadScript(afxString const* scope, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(uri);

    afxString scope2;

    if (AfxIsStringEmpty(scope))
        AfxGetUriTargetString(uri, &scope2);
    else
        AfxReflectString(scope, &scope2);

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);
    AfxReloadFile(file, uri);

    if (!file) AfxThrowError();
    else
    {
        afxString code;
        AfxMakeString(&code, 0, AfxGetStreamBuffer(file, 0), AfxGetStreamCapacity(file));

        afxResult xRslt;
        xRslt = AfxInjectScript(&scope2, &code);
        AFX_ASSERT(!xRslt);

        if (!xRslt)
        {
#if 0
            afxShell sh;
            AfxGetShell(&sh);

            afxHandle fn[4];
            afxString fns[4];
            AfxMakeString(&fns[0], "init()", 0);
            AfxMakeString(&fns[1], "quit()", 0);
            AfxMakeString(&fns[2], "step(_)", 0);
            AfxMakeString(&fns[3], "draw(_,_)", 0);
            XssHandleAnyCall(sh->vm, 4, fns, fn);

            afxHandle var;
            afxString vars;
            AfxMakeString(&vars, "Game", 0);
            XssHandleAnyVar(sh->vm, &domain, 0, 1, &vars, &var);

            sh->xssInitFn = fn[0];
            sh->xssQuitFn = fn[1];
            sh->xssStepFn = fn[2];
            sh->xssDrawFn = fn[3];
            sh->xssMainVar = var;

            XssPushHandle(sh->vm, 0, var);
            XssCall(sh->vm, fn[0]); // init
            XssUnhandle(sh->vm, 1, &fn[0]);
#endif
        }

        AfxDisposeObjects(1, &file);
    }
    return err;
}

#if 0
_AUX afxResult AfxRunScript(afxScript xss, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_XSS, 1, &xss);

    if (uri)
    {
        afxString domain;
        AfxGetUriTargetString(uri, &domain);

        if (AfxLoadScript(&domain, uri)) AfxThrowError();
        else
        {
            afxHandle fn[4];
            afxString fns[4];
            AfxMakeString(&fns[0], "init()", 0);
            AfxMakeString(&fns[1], "quit()", 0);
            AfxMakeString(&fns[2], "step(_)", 0);
            AfxMakeString(&fns[3], "draw(_,_)", 0);
            XssHandleAnyCall(xss->vm, 4, fns, fn);

            afxHandle var;
            afxString vars;
            AfxMakeString(&vars, "Game", 0);
            XssHandleAnyVar(xss->vm, &domain, 0, 1, &vars, &var);

            xss->xssInitFn = fn[0];
            xss->xssQuitFn = fn[1];
            xss->xssStepFn = fn[2];
            xss->xssDrawFn = fn[3];
            xss->xssMainVar = var;

            XssPushHandle(xss->vm, 0, var);
            XssCall(xss->vm, fn[0]); // init
            XssUnhandle(xss->vm, 1, &fn[0]);
        }
    }

    AfxGetClock(&xss->startClock);
    xss->lastClock = xss->startClock;
    return 0;
}
#endif

_AUX afxResult AfxInjectScript(afxString const* scope, afxString const* code)
{
    afxError err = 0;

    afxString32 scope2;
    AfxMakeString32(&scope2, scope);

    void* vm = NIL;

    AfxDbgLogf(0, NIL, code->start);
    afxResult xRslt = xssInterpret(vm, scope2.buf, code->start);

    return xRslt;
}
