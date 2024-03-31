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

_AFX void GenAcqObj(afxEnvironment env)
{
    afxObject* ph = lunaSetSlotNewForeign(env, 0, 0, sizeof(afxObject));
    *ph = NIL;
}

_AFX void GenRelObj(void* data)
{
    AfxReleaseObjects(1, data);
    *(void**)data = NIL;
}

_AFX void XssPushError(afxEnvironment env, afxNat slot, afxError err)
{
    lunaEnsureSlots(env, 1);
    lunaSetSlotDouble(env, slot, err);
}

_AFX void XssPushBool(afxEnvironment env, afxNat slot, afxBool val)
{
    lunaEnsureSlots(env, 1);
    lunaSetSlotBool(env, slot, val);
}

_AFX void XssPushReal(afxEnvironment env, afxNat slot, afxReal val)
{
    lunaEnsureSlots(env, 1);
    lunaSetSlotDouble(env, slot, val);
}

_AFX void XssPushReal64(afxEnvironment env, afxNat slot, afxReal64 val)
{
    lunaEnsureSlots(env, 1);
    lunaSetSlotDouble(env, slot, val);
}

_AFX void XssPushInt(afxEnvironment env, afxNat slot, afxInt val)
{
    lunaEnsureSlots(env, 1);
    lunaSetSlotDouble(env, slot, val);
}

_AFX void XssPushNat(afxEnvironment env, afxNat slot, afxNat val)
{
    lunaEnsureSlots(env, 1);
    lunaSetSlotDouble(env, slot, val);
}

_AFX void XssPushSize(afxEnvironment env, afxNat slot, afxSize val)
{
    lunaEnsureSlots(env, 1);
    lunaSetSlotDouble(env, slot, val);
}

_AFX afxBool XssPullBool(afxEnvironment env, afxNat slot)
{
    return (afxBool)lunaGetSlotDouble(env, slot);
}

_AFX afxReal XssPullReal(afxEnvironment env, afxNat slot)
{
    return (afxReal)lunaGetSlotDouble(env, slot);
}

_AFX afxReal64 XssPullReal64(afxEnvironment env, afxNat slot)
{
    return (afxReal64)lunaGetSlotDouble(env, slot);
}

_AFX void XssPullReal3(afxEnvironment env, afxNat slot, afxV3d v)
{
    v[0] = (afxReal)lunaGetSlotDouble(env, slot);
    v[1] = (afxReal)lunaGetSlotDouble(env, slot + 1);
    v[2] = (afxReal)lunaGetSlotDouble(env, slot + 2);
}

_AFX afxInt XssPullInt(afxEnvironment env, afxNat slot)
{
    return (afxInt)lunaGetSlotDouble(env, slot);
}

_AFX afxNat XssPullNat(afxEnvironment env, afxNat slot)
{
    return (afxNat)lunaGetSlotDouble(env, slot);
}

_AFX afxSize XssPullSize(afxEnvironment env, afxNat slot)
{
    return (afxSize)lunaGetSlotDouble(env, slot);
}

_AFX afxHandle XssPullHandle(afxEnvironment env, afxNat slot)
{
    return (afxHandle)lunaGetSlotHandle(env, slot);
}

_AFX void XssPushHandle(afxEnvironment env, afxNat slot, afxHandle han)
{
    afxError err;
    lunaSetSlotHandle(env, slot, han);
}

_AFX void* XssPullInstance(afxEnvironment env, afxNat slot)
{
    return *(void**)lunaGetSlotForeign(env, slot);
}

_AFX void XssPushInstance(afxEnvironment env, afxNat slot, void* obj)
{
    afxError err;
    AfxAssert(slot == 0);
    *(void**)lunaSetSlotNewForeign(env, 0, 0, sizeof(afxObject)) = obj;
}

_AFX void XssPullUri(afxEnvironment env, afxNat slot, afxUri* uri)
{
    const char* s = lunaGetSlotString(env, slot);
    AfxMakeUri(uri, s, 0);
}

_AFX void XssPullString(afxEnvironment env, afxNat slot, afxString* s)
{
    const char* src = lunaGetSlotString(env, slot);
    AfxMakeString(s, src, 0);
}

_AFX void XssUnhandle(afxEnvironment env, afxNat cnt, afxHandle han[])
{
    for (afxNat i = cnt; i-- > 0;)
    {
        lunaReleaseHandle(env, han[i]);
        han[i] = NIL;
    }
}

_AFX void XssHandleEachCall(afxEnvironment env, afxNat cnt, afxString const signatures[], afxHandle handles[])
{
    afxString2048 sig;
    
    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMakeString2048(&sig, &signatures[i]);
            
        if (!(handles[i] = (afxHandle)lunaMakeCallHandle(env, sig.buf)))
        {
            XssUnhandle(env, i, handles);
        }
    }
}

_AFX afxNat XssHandleAnyCall(afxEnvironment env, afxNat cnt, afxString const signatures[], afxHandle handles[])
{
    afxNat rslt = 0;

    afxString2048 sig;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMakeString2048(&sig, &signatures[i]);

        if ((handles[i] = (afxHandle)lunaMakeCallHandle(env, sig.buf)))
            ++rslt;
    }
    return rslt;
}

_AFX afxNat XssHandleAnyVar(afxEnvironment env, afxString const* domain, afxNat baseSlot, afxNat cnt, afxString const name[], afxHandle handles[])
{
    afxNat rslt = 0;

    afxString32 dom, nam;
    AfxMakeString32(&dom, domain);

    lunaEnsureSlots(env, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMakeString32(&nam, &name[i]);

        if (lunaHasVariable(env, dom.buf, nam.buf))
        {
            lunaGetVariable(env, dom.buf, nam.buf, baseSlot + i);
            
            if (lunaGetSlotType(env, baseSlot + i) == LUNA_TYPE_UNKNOWN)
            {
                handles[i] = lunaGetSlotHandle(env, baseSlot + i);
                ++rslt;
            }
        }
    }
    return rslt;
}

_AFX afxError XssCall(afxEnvironment env, afxHandle method)
{
    return lunaCall(env, method);
}

_AFX afxEnvironment AfxAcquireEnvironment(void)
{
    afxError err = AFX_ERR_NONE;

    afxManager* cls = AfxGetEnvironmentClass();
    AfxAssertClass(cls, afxFcc_ENV);
    afxEnvironment env;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&env, (void const*[]) { NIL }))
        AfxThrowError();

    return env;
}

_AFX afxError AfxAcquireScript(afxString const* domain, afxString const* code)
{
    afxError err = 0;

    afxEnvironment env;
    AfxGetEnvironment(&env);

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
    XssHandleAnyCall(env, 4, fns, fn);

    LunaHandle* initMethod = NULL;
    lunaEnsureSlots(env, 3);
    initMethod = lunaMakeCallHandle(env, "init()");

    if (!lunaHasVariable(env, domain->start, "Game"))
    {
        AfxThrowError();
    }

    lunaGetVariable(env, domain->start, "Game", 0);

    if (lunaGetSlotType(env, 0) != LUNA_TYPE_UNKNOWN)
    {
        AfxThrowError();
    }

    LunaHandle* gameClass = lunaGetSlotHandle(env, 0);
    LunaHandle* updateMethod = lunaMakeCallHandle(env, "update()");
    LunaHandle* drawMethod = lunaMakeCallHandle(env, "draw(_)");

    lunaSetSlotHandle(env, 0, gameClass);
    xRslt = lunaCall(env, initMethod);

    lunaReleaseHandle(env, initMethod);
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

    afxEnvironment env;
    AfxGetEnvironment(&env);

    afxString32 dom;
    AfxMakeString32(&dom, domain);

    AfxDbgLogf(0, NIL, code->start);
    afxResult xRslt = lunaInterpret(env, dom.buf, code->start);
    
    return xRslt;
}
