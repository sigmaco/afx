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

#include <sys/stat.h>
#include <stdio.h>

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sound/afxSoundSystem.h"

_AFX afxClass _sysClass;
afxClass _sysClass;
_AFX afxBool _sysClassInited = FALSE;
_AFX afxChar const _rwBuildDateTime[];
static afxChar const  _rwBuildDateTime[] = "\nCore built at " __DATE__ " " __TIME__ "\n";
static afxChar const _Ebenezer[] = "Ebenezer";

_AFX afxSystem theSys;
afxSystem theSys = NIL;
afxNat mainThreadId;

_AFX afxError _AfxSysCtor(afxSystem sys, afxCookie const* cookie);
_AFX afxError _AfxSysDtor(afxSystem sys);

_AFX afxClassConfig const _AfxSysClsConfig =
{
    .fcc = afxFcc_SYS,
    .name = "Qwadro Execution Ecosystem",
    .maxCnt = 1,
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSystem)),
    .mmu = NIL,
    .ctor = (void*)_AfxSysCtor,
    .dtor = (void*)_AfxSysDtor
};

_AFX afxSystem AfxGetSystem(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = theSys;
    AfxTryAssertObjects(1, &sys, afxFcc_SYS);
    return sys;
}

_AFXINL afxDrawSystem _AfxGetDsysData(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxDrawSystem dsys = sys->dsys;
    AfxAssertType(dsys, afxFcc_DSYS);
    return dsys;
}

_AFXINL afxSoundSystem _AfxGetSsysData(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxSoundSystem ssys = sys->ssys;
    AfxAssertType(ssys, afxFcc_SSYS);
    return ssys;
}

_AFX afxNat AfxGetIoBufferSize(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->ioBufSiz;
}

_AFX afxArena* AfxGetIoArena(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxArena *aren = &sys->ioArena;
    AfxAssertType(aren, afxFcc_AREN);
    return aren;
}

_AFX afxMmu AfxGetIoContext(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxMmu mmu = sys->mmu;
    AfxTryAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AFX afxMmu AfxGetSystemContext(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxMmu mmu = sys->mmu;
    AfxTryAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AFX afxNat AfxGetMemoryPageSize(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->memPageSize;
}

_AFX afxClass* AfxGetStorageClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->storages;
    AfxAssertClass(cls, afxFcc_STO);
    return cls;
}

_AFX afxNat AfxGetThreadingCapacity(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->hwConcurrencyCap;
}

_AFX afxClass* AfxGetSimulationClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->simulations;
    AfxAssertClass(cls, afxFcc_SIM);
    return cls;
}

_AFX afxClass* AfxGetServiceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->services;
    AfxAssertClass(cls, afxFcc_SVC);
    return cls;
}

_AFX afxClass* AfxGetThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->threads;
    AfxAssertClass(cls, afxFcc_THR);
    return cls;
}

_AFX afxClass* AfxGetTxuClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->txus;
    AfxAssertClass(cls, afxFcc_TXU);
    return cls;
}

_AFX afxClass* AfxGetDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->devices;
    AfxAssertClass(cls, afxFcc_DEV);
    return cls;
}

_AFX afxClass* AfxGetFileClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->files;
    AfxAssertClass(cls, afxFcc_FILE);
    return cls;
}

_AFX afxClass* AfxGetStringCatalogClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->strcats;
    AfxAssertClass(cls, afxFcc_STRC);
    return cls;
}

_AFX afxClass* AfxGetArchiveClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->archives;
    AfxAssertClass(cls, afxFcc_ARC);
    return cls;
}

_AFX afxClass* AfxGetMmuClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->mmus;
    AfxAssertClass(cls, afxFcc_MMU);
    return cls;
}

_AFX afxClass* AfxGetStreamClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->streams;
    AfxAssertClass(cls, afxFcc_IOS);
    return cls;
}

_AFX afxClass* AfxGetHidClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->hids;
    AfxAssertClass(cls, afxFcc_HID);
    return cls;
}

_AFX afxClass* AfxGetKeyboardClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->keyboards;
    AfxAssertClass(cls, afxFcc_KBD);
    return cls;
}

_AFX afxClass* AfxGetMouseClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->mouses;
    AfxAssertClass(cls, afxFcc_MSE);
    return cls;
}

_AFX afxClass* AfxGetModuleClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->modules;
    AfxAssertClass(cls, afxFcc_MDLE);
    return cls;
}

_AFX afxClass* AfxGetIcdClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->icds;
    AfxAssertClass(cls, afxFcc_ICD);
    return cls;
}

_AFX afxClass* AfxGetApplicationClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->applications;
    AfxAssertClass(cls, afxFcc_APP);
    return cls;
}

_AFX afxUri const* AfxGetSystemDirectory(afxUri *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxUri const* qwd = &sys->qwd;
    return dst ? AfxCopyUri(dst, qwd), dst : qwd;
}

_AFX afxString const* AfxGetSystemDirectoryString(afxBufferedString *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxString const* qwd = AfxGetUriString(&sys->qwd);
    return dst ? AfxCopyString(dst, qwd), &dst->str : qwd;
}

_AFX afxUri const* AfxGetPwd(afxUri *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxUri const* pwd = &sys->pwd;
    return dst ? AfxCopyUri(dst, pwd), dst : pwd;
}

_AFX afxString const* AfxGetPwdString(afxBufferedString *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxString const* pwd = AfxGetUriString(&sys->pwd);
    return dst ? AfxCopyString(dst, pwd), &dst->str : pwd;
}

_AFX afxBool _AfxThrQuitAndExecuteCb(afxThread thr, void *udd)
{
    afxError err = AFX_ERR_NONE;
    (void)udd;
    //afxInt exitCode = *(afxInt*)udd;

    AfxRequestThreadInterruption(thr);

    return FALSE; // dont interrupt curation
}

_AFX void _AfxInterruptionAllThreads(afxInt exitCode)
{
    AfxInvokeThreads(0, AFX_N32_MAX, _AfxThrQuitAndExecuteCb, &exitCode);
}

_AFX void AfxRequestSystemShutdown(afxInt exitCode)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if (!(sys = AfxGetSystem())) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sys, afxFcc_SYS);
        _AfxInterruptionAllThreads(exitCode);
        sys->exitCode = exitCode;
        sys->isInShutdown = TRUE;
    }
}

_AFX afxBool AfxSystemIsExecuting(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if ((sys = AfxGetSystem()))
    {
        AfxAssertObjects(1, &sys, afxFcc_SYS);
        return sys->operating && !sys->isInShutdown;
    }
    return FALSE;
}

_AFXINL afxBool _AfxNotifyObject(afxHandle *receiver, afxEvent *ev)
{
    // Sends event to receiver: receiver->event(event). 
    // Returns the value that is returned from the receiver's event handler. 
    // Note that this function is called for all events sent to any object in any thread.

    afxError err = AFX_ERR_NONE;    
    AfxAssertType(receiver, afxFcc_OBJ);
    AfxAssertType(ev, afxFcc_EVNT);
    afxBool rslt = AfxObjectEmitEvent(receiver, ev);

    return rslt;
}

_AFX afxBool AfxEmitEvents(afxNat cnt, afxHandle *receiver[], afxEvent ev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(receiver);
    AfxAssert(ev);
    afxBool res = 0;
    
    for (afxNat i = 0; i < cnt; i++)
    {
        afxHandle *obj = receiver[i];
        AfxAssertType(obj, afxFcc_OBJ);
        afxEvent *e = &ev[i];
        AfxAssertType(e, afxFcc_EVNT);
        _AfxNotifyObject(obj, e);
    }
    return res;
}

_AFX afxBool AfxReemitEvent(afxNat cnt, afxHandle *receiver[], afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(receiver);
    AfxAssertType(ev, afxFcc_EVNT);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxHandle *obj = receiver[i];
        AfxAssertType(obj, afxFcc_OBJ);
        _AfxNotifyObject(obj, ev);
    }
    return err;
}

_AFX afxBool AfxEmitEvent(afxHandle *receiver, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(receiver, afxFcc_OBJ);
    AfxAssertType(ev, afxFcc_EVNT);
    return _AfxNotifyObject(receiver, ev);
}

_AFX afxError AfxPostEvents(afxNat cnt, afxHandle *receiver[], afxEvent ev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(receiver);
    AfxAssertType(ev, afxFcc_EVNT);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxHandle *obj = receiver[i];
        AfxAssertType(obj, afxFcc_OBJ);
        _AfxNotifyObject(obj, ev);
        AfxThrowError();
        // Not implemented yet.
    }
    return err;
}

_AFX afxError AfxPostEvent(afxHandle *receiver, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(receiver, afxFcc_OBJ);
    AfxAssertType(ev, afxFcc_EVNT);
    
    if (AfxPostEvents(1, &receiver, ev))
        AfxThrowError();

    return err;
}

_AFX void AfxShutdownSystem(afxInt exitCode)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if ((sys = AfxGetSystem()))
    {
        do { AfxRequestSystemShutdown(exitCode); } while (AfxSystemIsExecuting());
        while (!AfxReleaseObjects(1, (void*[]) { sys }));
        AfxDismountClass(&_sysClass);
    };
#ifndef _AFX_DISABLE_DEBUGGER
    AfxDetachDebugger();
#endif
}

_AFX afxError AfxBootUpSystem(afxSystemConfig const *config)
{
    afxError err = AFX_ERR_NONE;

#ifndef _AFX_DISABLE_DEBUGGER
    AfxAttachDebugger(NIL);
#endif
    afxSystem sys;

    if ((sys = AfxGetSystem())) AfxThrowError();
    else
    {
        AfxMountClass(&_sysClass, NIL, NIL, &_AfxSysClsConfig);

        if (AfxAcquireObjects(&_sysClass, 1, (afxObject*)&sys, (void const*[]) { (void*)config })) AfxThrowError();
        else
        {
            AfxAssert(sys == AfxGetSystem());
        }
    }
    return err;
}
