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

#define _AFX_UX_C
#define _AFX_SYSTEM_C
#include "qwadro/../_luna/luna_vm.h"
#include "qwadro/../_luna/luna.h"
#include "qwadro/ux/afxShell.h"

_AUX afxBool shReady = FALSE;
_AUX afxByte theShData[AFX_ALIGN(sizeof(afxObjectBase), 16) + AFX_ALIGN(sizeof(AFX_OBJECT(afxShell)), 16)] = { 0 };
_AUX afxShell TheShell = (void*)&theShData;
static_assert(sizeof(theShData) >= (sizeof(afxObjectBase) + sizeof(TheShell[0])), "");

extern afxClassConfig const _AfxCtrlMgrCfg;
extern afxClassConfig const _AfxHidMgrCfg;

extern xssVm xssCreateVm(LunaConfiguration const* config);
extern afxError xssDestroyVm(xssVm env);

_AUX afxBool AfxGetShell(afxShell* shell)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(shell);
    *shell = TheShell;
    return shReady;
}

_AUX afxManager* AfxGetControllerManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, &sh, afxFcc_ENV);
    afxManager *cls = &sh->ctrlMgr;
    AfxAssertClass(cls, afxFcc_CTRL);
    return cls;
}

_AUX afxManager* AfxGetHidManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, &sh, afxFcc_ENV);
    afxManager *cls = &sh->hidMgr;
    AfxAssertClass(cls, afxFcc_HID);
    return cls;
}

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

_AUX afxError AfxAcquireScript(afxString const* domain, afxString const* code)
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

_AUX afxError AfxLoadScript(afxString const* domain, afxUri const* uri)
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

_AUX afxResult AfxInjectScript(afxString const* domain, afxString const* code)
{
    afxError err = 0;

    afxShell sh;
    AfxGetShell(&sh);

    afxString32 dom;
    AfxMakeString32(&dom, domain);

    AfxDbgLogf(0, NIL, code->start);
    afxResult xRslt = lunaInterpret(sh->vm, dom.buf, code->start);
    
    return xRslt;
}

_AUX afxError _AfxShCtor(afxShell sh, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sh, afxFcc_ENV);

    if (!(sh->vm = xssCreateVm(NIL)))
        AfxThrowError();

    afxChain* mgrChn = &sh->mgrChn;
    AfxSetUpChain(mgrChn, sh);
    afxClassConfig clsCfg;

    AfxEstablishManager(&sh->hidMgr, /*AfxGetDeviceClass()*/NIL, mgrChn, &_AfxHidMgrCfg);
    AfxEstablishManager(&sh->ctrlMgr, AfxGetHidManager(), mgrChn, &_AfxCtrlMgrCfg); // require hid

    afxHid hid;

    if (AfxAcquireHid(0, &hid)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);
        sh->stdHid = hid;

        afxUri point, location;
        AfxMakeUri(&point, "code", 0);
        AfxMakeUri(&location, "system", 0);

        if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX)) AfxThrowError();
        else
        {
            afxUri uri;
            AfxMakeUri(&uri, "system/qwadro.xss", 0);

            afxString s;
            AfxMakeString(&s, "qwadro", 0);
            AfxLoadScript(&s, &uri);

            if (err)
                AfxDismountStorageUnit(&point, &location, afxFileFlag_RWX);
        }

        if (err)
        {
            AfxReleaseObjects(1, (void*[]) { hid });
        }
    }
    return err;
}

_AUX afxError _AfxShDtor(afxShell sh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sh, afxFcc_ENV);

    if (sh->stdHid)
    {
        AfxReleaseObjects(1, (void*[]) { sh->stdHid });
        sh->stdHid = NIL;
    }

    AfxCleanUpChainedManagers(&sh->mgrChn);

    afxUri point, location;
    AfxMakeUri(&point, "code", 0);
    AfxMakeUri(&location, "system", 0);
    AfxDismountStorageUnit(&point, &location, afxFileFlag_RWX);

    if (xssDestroyVm(sh->vm))
        AfxThrowError();

    return err;
}

_AUX afxManager* AfxGetShellClass(void)
{
    afxError err = AFX_ERR_NONE;
    static afxManager shMgr = { 0 };
    static afxBool shMgrMounted = FALSE;
    static afxClassConfig shMgrCfg =
    {
        .fcc = afxFcc_ENV,
        .name = "Shell",
        .desc = "Execution Environment",
        .unitsPerPage = 1,
        .size = sizeof(AFX_OBJECT(afxShell)),
        .ctor = (void*)_AfxShCtor,
        .dtor = (void*)_AfxShDtor
    };

    if (shMgr.fcc != afxFcc_CLS)
    {
        AfxEstablishManager(&shMgr, NIL, /*_AfxGetSystemClassChain()*/NIL, &shMgrCfg);
        shMgrMounted = TRUE;
    }
    AfxAssertClass(&shMgr, afxFcc_ENV);
    return &shMgr;
}

_AUX afxError AfxEntryPoint(afxModule mdle, afxNat reqCode, void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    switch (reqCode)
    {
    case afxFcc_SYS:
    {
        afxShell sh;

        if (!AfxGetShell(&sh))
        {
            AfxAssert(TheShell == sh);
            AfxZero(TheShell, sizeof(afxObjectBase));

            afxManager* mgr = AfxGetShellClass();
            AfxAssertClass(mgr, afxFcc_ENV);

            if (_AfxConstructObjects(mgr, 1, (void**)&TheShell, udd)) AfxThrowError();
            else
            {
                AfxAssert(TheShell != sh); // Attention! Ctor moves the object pointer to hide out the object base.
                sh = TheShell;
                AfxAssertObjects(1, &sh, afxFcc_ENV);
                shReady = TRUE;
            }
        }
        else
        {
            AfxAssertObjects(1, &sh, afxFcc_ENV);
            shReady = FALSE;

            afxManager* mgr = AfxGetShellClass();
            AfxAssertClass(mgr, afxFcc_ENV);

            AfxAssert(TheShell == sh);

            if (_AfxDestructObjects(mgr, 1, (void**)&TheShell))
                AfxThrowError();

            AfxAssert(TheShell != sh); // Attention! Dtor moves the object pointer to expose the object base.
            AfxZero(TheShell, sizeof(afxObjectBase));
        }
    }
    default: break;
    }
    return err;
}
