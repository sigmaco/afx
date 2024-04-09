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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#include "qwadro/../_dep/luna.h"
#include "qwadro/core/afxSystem.h"

_AFX void GenAcqObj(afxShell sh)
{
    afxObject* ph = lunaSetSlotNewForeign(sh, 0, 0, sizeof(afxObject));
    *ph = NIL;
}

_AFX void GenRelObj(void* data)
{
    AfxReleaseObjects(1, data);
    *(void**)data = NIL;
}

_AFX void XssPushError(afxShell sh, afxNat slot, afxError err)
{
    lunaEnsureSlots(sh, 1);
    lunaSetSlotDouble(sh, slot, err);
}

_AFX void XssPushBool(afxShell sh, afxNat slot, afxBool val)
{
    lunaEnsureSlots(sh, 1);
    lunaSetSlotBool(sh, slot, val);
}

_AFX void XssPushReal(afxShell sh, afxNat slot, afxReal val)
{
    lunaEnsureSlots(sh, 1);
    lunaSetSlotDouble(sh, slot, val);
}

_AFX void XssPushReal64(afxShell sh, afxNat slot, afxReal64 val)
{
    lunaEnsureSlots(sh, 1);
    lunaSetSlotDouble(sh, slot, val);
}

_AFX void XssPushInt(afxShell sh, afxNat slot, afxInt val)
{
    lunaEnsureSlots(sh, 1);
    lunaSetSlotDouble(sh, slot, val);
}

_AFX void XssPushNat(afxShell sh, afxNat slot, afxNat val)
{
    lunaEnsureSlots(sh, 1);
    lunaSetSlotDouble(sh, slot, val);
}

_AFX void XssPushSize(afxShell sh, afxNat slot, afxSize val)
{
    lunaEnsureSlots(sh, 1);
    lunaSetSlotDouble(sh, slot, val);
}

_AFX afxBool XssPullBool(afxShell sh, afxNat slot)
{
    return (afxBool)lunaGetSlotDouble(sh, slot);
}

_AFX afxReal XssPullReal(afxShell sh, afxNat slot)
{
    return (afxReal)lunaGetSlotDouble(sh, slot);
}

_AFX afxReal64 XssPullReal64(afxShell sh, afxNat slot)
{
    return (afxReal64)lunaGetSlotDouble(sh, slot);
}

_AFX void XssPullReal3(afxShell sh, afxNat slot, afxV3d v)
{
    v[0] = (afxReal)lunaGetSlotDouble(sh, slot);
    v[1] = (afxReal)lunaGetSlotDouble(sh, slot + 1);
    v[2] = (afxReal)lunaGetSlotDouble(sh, slot + 2);
}

_AFX afxInt XssPullInt(afxShell sh, afxNat slot)
{
    return (afxInt)lunaGetSlotDouble(sh, slot);
}

_AFX afxNat XssPullNat(afxShell sh, afxNat slot)
{
    return (afxNat)lunaGetSlotDouble(sh, slot);
}

_AFX afxSize XssPullSize(afxShell sh, afxNat slot)
{
    return (afxSize)lunaGetSlotDouble(sh, slot);
}

_AFX afxHandle XssPullHandle(afxShell sh, afxNat slot)
{
    return (afxHandle)lunaGetSlotHandle(sh, slot);
}

_AFX void XssPushHandle(afxShell sh, afxNat slot, afxHandle han)
{
    afxError err;
    lunaSetSlotHandle(sh, slot, han);
}

_AFX void* XssPullInstance(afxShell sh, afxNat slot)
{
    return *(void**)lunaGetSlotForeign(sh, slot);
}

_AFX void XssPushInstance(afxShell sh, afxNat slot, void* obj)
{
    afxError err;
    AfxAssert(slot == 0);
    *(void**)lunaSetSlotNewForeign(sh, 0, 0, sizeof(afxObject)) = obj;
}

_AFX void XssPullUri(afxShell sh, afxNat slot, afxUri* uri)
{
    const char* s = lunaGetSlotString(sh, slot);
    AfxMakeUri(uri, s, 0);
}

_AFX void XssPullString(afxShell sh, afxNat slot, afxString* s)
{
    const char* src = lunaGetSlotString(sh, slot);
    AfxMakeString(s, src, 0);
}

_AFX void XssUnhandle(afxShell sh, afxNat cnt, afxHandle han[])
{
    for (afxNat i = cnt; i-- > 0;)
    {
        lunaReleaseHandle(sh, han[i]);
        han[i] = NIL;
    }
}

_AFX void XssHandleEachCall(afxShell sh, afxNat cnt, afxString const signatures[], afxHandle handles[])
{
    afxString2048 sig;
    
    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMakeString2048(&sig, &signatures[i]);
            
        if (!(handles[i] = (afxHandle)lunaMakeCallHandle(sh, sig.buf)))
        {
            XssUnhandle(sh, i, handles);
        }
    }
}

_AFX afxNat XssHandleAnyCall(afxShell sh, afxNat cnt, afxString const signatures[], afxHandle handles[])
{
    afxNat rslt = 0;

    afxString2048 sig;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMakeString2048(&sig, &signatures[i]);

        if ((handles[i] = (afxHandle)lunaMakeCallHandle(sh, sig.buf)))
            ++rslt;
    }
    return rslt;
}

_AFX afxNat XssHandleAnyVar(afxShell sh, afxString const* domain, afxNat baseSlot, afxNat cnt, afxString const name[], afxHandle handles[])
{
    afxNat rslt = 0;

    afxString32 dom, nam;
    AfxMakeString32(&dom, domain);

    lunaEnsureSlots(sh, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMakeString32(&nam, &name[i]);

        if (lunaHasVariable(sh, dom.buf, nam.buf))
        {
            lunaGetVariable(sh, dom.buf, nam.buf, baseSlot + i);
            
            if (lunaGetSlotType(sh, baseSlot + i) == LUNA_TYPE_UNKNOWN)
            {
                handles[i] = lunaGetSlotHandle(sh, baseSlot + i);
                ++rslt;
            }
        }
    }
    return rslt;
}

_AFX afxError XssCall(afxShell sh, afxHandle method)
{
    return lunaCall(sh, method);
}

_AFX afxError AfxAcquireShell(afxShell* sh)
{
    afxError err = AFX_ERR_NONE;

    afxManager* cls = AfxGetEnvironmentClass();
    AfxAssertClass(cls, afxFcc_ENV);
    afxShell env2;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&env2, (void const*[]) { NIL }))
        AfxThrowError();

    AfxAssert(sh);
    *sh = env2;

    return err;
}

_AFX afxError AfxAcquireScript(afxString const* domain, afxString const* code)
{
    afxError err = 0;

    afxShell sh;
    AfxGetShell(&sh);

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
    XssHandleAnyCall(sh, 4, fns, fn);

    LunaHandle* initMethod = NULL;
    lunaEnsureSlots(sh, 3);
    initMethod = lunaMakeCallHandle(sh, "init()");

    if (!lunaHasVariable(sh, domain->start, "Game"))
    {
        AfxThrowError();
    }

    lunaGetVariable(sh, domain->start, "Game", 0);

    if (lunaGetSlotType(sh, 0) != LUNA_TYPE_UNKNOWN)
    {
        AfxThrowError();
    }

    LunaHandle* gameClass = lunaGetSlotHandle(sh, 0);
    LunaHandle* updateMethod = lunaMakeCallHandle(sh, "update()");
    LunaHandle* drawMethod = lunaMakeCallHandle(sh, "draw(_)");

    lunaSetSlotHandle(sh, 0, gameClass);
    xRslt = lunaCall(sh, initMethod);

    lunaReleaseHandle(sh, initMethod);
    initMethod = NULL;
#endif
    return err;
}

_AFX afxError AfxLoadScript(afxString const* domain, afxUri const* uri)
{
    afxError err = 0;
    afxString domain2;

    if (!domain)
        AfxPickUriObjectToString(uri, &domain2);
    else
        AfxReflectString(domain, &domain2);

    afxStream file = AfxLoadFile(uri);

    if (!file) AfxThrowError();
    else
    {
        afxString code;
        AfxMakeString(&code, AfxGetStreamBuffer(file, 0), AfxGetStreamBufferCap(file));

        if (AfxAcquireScript(&domain2, &code))
            AfxThrowError();
    }
    return err;
}

_AFX afxResult AfxInjectScript(afxString const* domain, afxString const* code)
{
    afxError err = 0;

    afxShell sh;
    AfxGetShell(&sh);

    afxString32 dom;
    AfxMakeString32(&dom, domain);

    AfxDbgLogf(0, NIL, code->start);
    afxResult xRslt = lunaInterpret(sh, dom.buf, code->start);
    
    return xRslt;
}
