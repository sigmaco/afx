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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_SCRIPT_C
#include "qwadro/../_luna/luna_vm.h"
#include "qwadro/../_luna/luna.h"
#include "qwadro/ux/afxShell.h"

_AUX void GenAcqObj(xssVm vm)
{
    afxObject* ph = lunaSetSlotNewForeign(vm, 0, 0, sizeof(afxObject));
    *ph = NIL;
}

_AUX void GenRelObj(void* data)
{
    AfxReleaseObjects(1, data);
    *(void**)data = NIL;
}

_AUX void XssPushError(xssVm vm, afxNat slot, afxError err)
{
    lunaEnsureSlots(vm, 1);
    lunaSetSlotDouble(vm, slot, err);
}

_AUX void XssPushBool(xssVm vm, afxNat slot, afxBool val)
{
    lunaEnsureSlots(vm, 1);
    lunaSetSlotBool(vm, slot, val);
}

_AUX void XssPushReal(xssVm vm, afxNat slot, afxReal val)
{
    lunaEnsureSlots(vm, 1);
    lunaSetSlotDouble(vm, slot, val);
}

_AUX void XssPushReal64(xssVm vm, afxNat slot, afxReal64 val)
{
    lunaEnsureSlots(vm, 1);
    lunaSetSlotDouble(vm, slot, val);
}

_AUX void XssPushInt(xssVm vm, afxNat slot, afxInt val)
{
    lunaEnsureSlots(vm, 1);
    lunaSetSlotDouble(vm, slot, val);
}

_AUX void XssPushNat(xssVm vm, afxNat slot, afxNat val)
{
    lunaEnsureSlots(vm, 1);
    lunaSetSlotDouble(vm, slot, val);
}

_AUX void XssPushSize(xssVm vm, afxNat slot, afxSize val)
{
    lunaEnsureSlots(vm, 1);
    lunaSetSlotDouble(vm, slot, val);
}

_AUX afxBool XssPullBool(xssVm vm, afxNat slot)
{
    return (afxBool)lunaGetSlotDouble(vm, slot);
}

_AUX afxReal XssPullReal(xssVm vm, afxNat slot)
{
    return (afxReal)lunaGetSlotDouble(vm, slot);
}

_AUX afxReal64 XssPullReal64(xssVm vm, afxNat slot)
{
    return (afxReal64)lunaGetSlotDouble(vm, slot);
}

_AUX void XssPullReal3(xssVm vm, afxNat slot, afxV3d v)
{
    v[0] = (afxReal)lunaGetSlotDouble(vm, slot);
    v[1] = (afxReal)lunaGetSlotDouble(vm, slot + 1);
    v[2] = (afxReal)lunaGetSlotDouble(vm, slot + 2);
}

_AUX afxInt XssPullInt(xssVm vm, afxNat slot)
{
    return (afxInt)lunaGetSlotDouble(vm, slot);
}

_AUX afxNat XssPullNat(xssVm vm, afxNat slot)
{
    return (afxNat)lunaGetSlotDouble(vm, slot);
}

_AUX afxSize XssPullSize(xssVm vm, afxNat slot)
{
    return (afxSize)lunaGetSlotDouble(vm, slot);
}

_AUX afxHandle XssPullHandle(xssVm vm, afxNat slot)
{
    return (afxHandle)lunaGetSlotHandle(vm, slot);
}

_AUX void XssPushHandle(xssVm vm, afxNat slot, afxHandle han)
{
    afxError err;
    lunaSetSlotHandle(vm, slot, han);
}

_AUX void* XssPullInstance(xssVm vm, afxNat slot)
{
    return *(void**)lunaGetSlotForeign(vm, slot);
}

_AUX void XssPushInstance(xssVm vm, afxNat slot, void* obj)
{
    afxError err;
    AfxAssert(slot == 0);
    *(void**)lunaSetSlotNewForeign(vm, 0, 0, sizeof(afxObject)) = obj;
}

_AUX void XssPullUri(xssVm vm, afxNat slot, afxUri* uri)
{
    const char* s = lunaGetSlotString(vm, slot);
    AfxMakeUri(uri, s, 0);
}

_AUX void XssPullString(xssVm vm, afxNat slot, afxString* s)
{
    const char* src = lunaGetSlotString(vm, slot);
    AfxMakeString(s, src, 0);
}

_AUX void XssUnhandle(xssVm vm, afxNat cnt, afxHandle han[])
{
    for (afxNat i = cnt; i-- > 0;)
    {
        lunaReleaseHandle(vm, han[i]);
        han[i] = NIL;
    }
}

_AUX void XssHandleEachCall(xssVm vm, afxNat cnt, afxString const signatures[], afxHandle handles[])
{
    afxString2048 sig;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMakeString2048(&sig, &signatures[i]);

        if (!(handles[i] = (afxHandle)lunaMakeCallHandle(vm, sig.buf)))
        {
            XssUnhandle(vm, i, handles);
        }
    }
}

_AUX afxNat XssHandleAnyCall(xssVm vm, afxNat cnt, afxString const signatures[], afxHandle handles[])
{
    afxNat rslt = 0;

    afxString2048 sig;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMakeString2048(&sig, &signatures[i]);

        if ((handles[i] = (afxHandle)lunaMakeCallHandle(vm, sig.buf)))
            ++rslt;
    }
    return rslt;
}

_AUX afxNat XssHandleAnyVar(xssVm vm, afxString const* domain, afxNat baseSlot, afxNat cnt, afxString const name[], afxHandle handles[])
{
    afxNat rslt = 0;

    afxString32 dom, nam;
    AfxMakeString32(&dom, domain);

    lunaEnsureSlots(vm, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMakeString32(&nam, &name[i]);

        if (lunaHasVariable(vm, dom.buf, nam.buf))
        {
            lunaGetVariable(vm, dom.buf, nam.buf, baseSlot + i);

            if (lunaGetSlotType(vm, baseSlot + i) == LUNA_TYPE_UNKNOWN)
            {
                handles[i] = lunaGetSlotHandle(vm, baseSlot + i);
                ++rslt;
            }
        }
    }
    return rslt;
}

_AUX afxError XssCall(xssVm vm, afxHandle method)
{
    return lunaCall(vm, method);
}

_AUX void AfxGetScriptTime(afxScript xss, afxReal64* ct, afxReal64* dt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xss, afxFcc_XSS);
    afxClock currClock;
    AfxGetClock(&currClock);
    AfxAssert(ct);
    *ct = AfxGetSecondsElapsed(&xss->startClock, &currClock);
    AfxAssert(dt);
    *dt = AfxGetSecondsElapsed(&xss->lastClock, &currClock);
    xss->lastClock = currClock;
}

_AUX afxBool _AuxXssEvent(afxScript xss, afxUxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xss, afxFcc_XSS);

    switch (ev->id)
    {
    case afxUxEventId_RUN:
    {
        if (xss->xssInitFn)
        {
            XssPushHandle(xss->vm, 0, xss->xssMainVar);
            XssCall(xss->vm, xss->xssInitFn);
        }
        break;
    }
    case afxUxEventId_QUIT:
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

_AUX afxError _AuxXssCtor(afxScript xss, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xss, afxFcc_XSS);

    afxShell sh = cookie->udd[0];
    AfxAssertObjects(1, &sh, afxFcc_USYS);
    afxString const* domain = ((afxString const*)cookie->udd[1]) + cookie->no;
    afxString const* code = ((afxString const*)cookie->udd[2]) + cookie->no;

    AfxResetString(&xss->path);

    xss->argc = 0;
    xss->argv = NIL;

    xss->vm = sh->vm;

    AfxGetClock(&xss->startClock);
    xss->lastClock = xss->startClock;

    return err;
}

_AUX afxError _AuxXssDtor(afxScript xss)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xss, afxFcc_XSS);

    return err;
}

_AUX afxClassConfig const _AuxXssMgrCfg =
{
    .fcc = afxFcc_XSS,
    .name = "Script",
    .desc = "Cross-System Scripting",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxScript)),
    .mmu = NIL,
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
    afxManager* cls = AfxGetScriptManager();
    AfxAssertClass(cls, afxFcc_XSS);

    if (AfxAcquireObjects(cls, 1, (void*)script, ((void const*[]) { sh, domain, code })))
        AfxThrowError();

    AfxAssertObjects(1, script, afxFcc_XSS);
    afxResult r;

    afxResult xRslt;

    xRslt = AfxInjectScript(domain, code);
    AfxAssert(!xRslt);
#if 0
    afxHandle fn[4];
    afxString fns[4];
    AfxMakeString(&fns[0], "init()", 0);
    AfxMakeString(&fns[1], "quit()", 0);
    AfxMakeString(&fns[2], "step(_)", 0);
    AfxMakeString(&fns[3], "draw(_,_)", 0);
    XssHandleAnyCall(sh->vm, 4, fns, fn);

    LunaHandle* initMethod = NULL;
    lunaEnsureSlots(sh->vm, 3);
    initMethod = lunaMakeCallHandle(sh->vm, "init()");

    if (!lunaHasVariable(sh->vm, domain->start, "Game"))
    {
        AfxThrowError();
    }

    lunaGetVariable(sh->vm, domain->start, "Game", 0);

    if (lunaGetSlotType(sh->vm, 0) != LUNA_TYPE_UNKNOWN)
    {
        AfxThrowError();
    }

    LunaHandle* gameClass = lunaGetSlotHandle(sh->vm, 0);
    LunaHandle* updateMethod = lunaMakeCallHandle(sh->vm, "update()");
    LunaHandle* drawMethod = lunaMakeCallHandle(sh->vm, "draw(_)");

    lunaSetSlotHandle(sh->vm, 0, gameClass);
    xRslt = lunaCall(sh->vm, initMethod);

    lunaReleaseHandle(sh->vm, initMethod);
    initMethod = NULL;
#endif
    return err;
}
#endif

_AUX afxError AfxLoadScript(afxString const* scope, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);

    afxString scope2;

    if (AfxStringIsEmpty(scope))
        AfxPickUriObjectToString(uri, &scope2);
    else
        AfxReflectString(scope, &scope2);

    afxStream file = AfxLoadFile(uri);

    if (!file) AfxThrowError();
    else
    {
        afxString code;
        AfxMakeString(&code, AfxGetStreamBuffer(file, 0), AfxGetStreamBufferCap(file));

        afxResult xRslt;
        xRslt = AfxInjectScript(&scope2, &code);
        AfxAssert(!xRslt);

        if (!xRslt)
        {
            afxShell sh;
            AfxGetShell(&sh);
#if 0
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

        AfxCloseStream(file);
    }
    return err;
}

#if 0
_AUX afxResult AfxRunScript(afxScript xss, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xss, afxFcc_XSS);

    if (uri)
    {
        afxString domain;
        AfxPickUriObjectToString(uri, &domain);

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

_AUX afxNat AfxInvokeScripts(afxNat first, afxNat cnt, afxBool(*f)(afxScript, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetScriptManager();
    AfxAssertClass(cls, afxFcc_XSS);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AUX afxNat AfxEnumerateScripts(afxNat first, afxNat cnt, afxScript applications[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(applications);
    afxManager* cls = AfxGetScriptManager();
    AfxAssertClass(cls, afxFcc_XSS);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)applications);
}

_AUX afxNat AfxCountScripts(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetScriptManager();
    AfxAssertClass(cls, afxFcc_XSS);
    return AfxCountObjects(cls);
}

_AUX afxResult AfxInjectScript(afxString const* scope, afxString const* code)
{
    afxError err = 0;

    afxShell sh;
    AfxGetShell(&sh);

    afxString32 scope2;
    AfxMakeString32(&scope2, scope);

    AfxDbgLogf(0, NIL, code->start);
    afxResult xRslt = lunaInterpret(sh->vm, scope2.buf, code->start);

    return xRslt;
}
