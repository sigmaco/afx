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

_AFX afxString const sigmaSignature = AFX_STRING(
"\n       ::::::::    :::::::::::    ::::::::    ::::     ::::       :::          "
"\n      :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:        "
"\n      +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+       "
"\n      +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:      "
"\n             +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+      "
"\n      #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#      "
"\n       ########    ###########    ########    ###       ###   ###     ###      "
"\n                                                                               "
"\n              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              "
"\n                                                                               "
"\n                               Public Test Build                               "
"\n                   (c) 2017 SIGMA, Engineering In Technology                   "
"\n                                www.sigmaco.org                                "
"\n                                                                               "
);

_AFX afxString const qwadroSignature = AFX_STRING(
"\n      ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::      "
"\n     :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:     "
"\n     +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+     "
"\n     +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+     "
"\n     +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+     "
"\n     #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#     "
"\n      ###### ###  ###   ###   ###     ### #########  ###    ###  ########      "
"\n                                                                               "
"\n              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              "
"\n                                                                               "
"\n                               Public Test Build                               "
"\n                   (c) 2017 SIGMA, Engineering In Technology                   "
"\n                                www.sigmaco.org                                "
"\n                                                                               "
);

_AFX afxClass _sysClass;
afxClass _sysClass;
_AFX afxBool _sysClassInited = FALSE;
_AFX afxChar const _rwBuildDateTime[];
static afxChar const  _rwBuildDateTime[] = "\nCore built at " __DATE__ " " __TIME__ "\n";
static afxChar const _Ebenezer[] = "Ebenezer";

_AFX afxSystem theSys;
afxSystem theSys = NIL;
afxNat mainThreadId;

extern afxError _AfxSysScanForIcds(afxSystem sys, afxUri const* fileMask);

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

_AFXINL afxDrawSystem AfxGetDrawSystem(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxDrawSystem dsys = sys->dsys;
    AfxTryAssertObjects(1, &dsys, afxFcc_DSYS);
    return dsys;
}

_AFXINL afxSoundSystem AfxGetSoundSystem(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxSoundSystem ssys = sys->ssys;
    AfxTryAssertObjects(1, &ssys, afxFcc_SSYS);
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

_AFX afxClass* AfxGetFileSystemClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->fsysCls;
    AfxAssertClass(cls, afxFcc_FSYS);
    return cls;
}

_AFX afxNat AfxGetThreadingCapacity(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->hwConcurrencyCap;
}

_AFX afxClass* AfxGetServiceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->svcCls;
    AfxAssertClass(cls, afxFcc_SVC);
    return cls;
}

_AFX afxClass* AfxGetThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->thrCls;
    AfxAssertClass(cls, afxFcc_THR);
    return cls;
}

_AFX afxClass* AfxGetThreadingUnitClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->txuCls;
    AfxAssertClass(cls, afxFcc_TXU);
    return cls;
}

_AFX afxClass* AfxGetDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->devCls;
    AfxAssertClass(cls, afxFcc_DEV);
    return cls;
}

_AFX afxClass* AfxGetFileClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->fileCls;
    AfxAssertClass(cls, afxFcc_FILE);
    return cls;
}

_AFX afxClass* AfxGetStringCatalogClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->strcCls;
    AfxAssertClass(cls, afxFcc_STRC);
    return cls;
}

_AFX afxClass* AfxGetArchiveClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->archCls;
    AfxAssertClass(cls, afxFcc_ARC);
    return cls;
}

_AFX afxClass* AfxGetMmuClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->mmuCls;
    AfxAssertClass(cls, afxFcc_MMU);
    return cls;
}

_AFX afxClass* AfxGetStreamClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->iosCls;
    AfxAssertClass(cls, afxFcc_IOS);
    return cls;
}

_AFX afxClass* AfxGetHidClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->hidCls;
    AfxAssertClass(cls, afxFcc_HID);
    return cls;
}

_AFX afxClass* AfxGetKeyboardClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->kbdCls;
    AfxAssertClass(cls, afxFcc_KBD);
    return cls;
}

_AFX afxClass* AfxGetMouseClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->mseCls;
    AfxAssertClass(cls, afxFcc_MSE);
    return cls;
}

_AFX afxClass* AfxGetControllerClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->ctrlCls;
    AfxAssertClass(cls, afxFcc_CTRL);
    return cls;
}

_AFX afxClass* AfxGetExecutableClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->exeCls;
    AfxAssertClass(cls, afxFcc_EXE);
    return cls;
}

_AFX afxClass* AfxGetIcdClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->icdCls;
    AfxAssertClass(cls, afxFcc_ICD);
    return cls;
}

_AFX afxUri const* AfxGetSystemDirectory(afxUri *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxUri const* qwd = &sys->qwd.uri;
    return dst ? AfxCopyUri(dst, qwd), dst : qwd;
}

_AFX afxString const* AfxGetSystemDirectoryString(afxRestring *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxString const* qwd = AfxGetUriString(&sys->qwd.uri);
    return dst ? AfxCopyString(dst, qwd), &dst->str : qwd;
}

_AFX afxUri const* AfxGetPwd(afxUri *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxUri const* pwd = &sys->pwd.uri;
    return dst ? AfxCopyUri(dst, pwd), dst : pwd;
}

_AFX afxString const* AfxGetPwdString(afxRestring *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxString const* pwd = AfxGetUriString(&sys->pwd.uri);
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

_AFX void AfxDoSystemShutdown(afxInt exitCode)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if ((sys = AfxGetSystem()))
    {
        AfxAdvertise("Shutting down Qwadro...");

        do { AfxRequestSystemShutdown(exitCode); } while (AfxSystemIsExecuting());
        while (!AfxReleaseObjects(1, (void*[]) { sys }));
        AfxDismountClass(&_sysClass);
    };
#ifndef _AFX_DISABLE_DEBUGGER
    AfxDetachDebugger();
#endif
}

_AFX afxError AfxDoSystemBootUp(afxSystemConfig const *config)
{
    afxError err = AFX_ERR_NONE;

#ifndef _AFX_DISABLE_DEBUGGER
    AfxAttachDebugger(NIL);
#endif
    afxSystem sys;

    if ((sys = AfxGetSystem())) AfxThrowError();
    else
    {
        AfxAdvertise("Booting up Qwadro...");

        AfxMountClass(&_sysClass, NIL, NIL, &_AfxSysClsConfig);

        afxIni ini;
        AfxSetUpIni(&ini);
        // platform-dependent ctor will load the correct ini file.

        if (AfxAcquireObjects(&_sysClass, 1, (afxObject*)&sys, (void const*[]) { &ini, config })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &sys, afxFcc_SYS);
            AfxAssert(sys == AfxGetSystem());

            afxUri uri;
            afxBool ssysEnabled = FALSE;

            if (AfxIniGetBool(&ini, &AfxString("SoundSystem"), &AfxString("bEnabled"), &ssysEnabled) && ssysEnabled)
            {
                AfxMakeUri(&uri, "e2sound.dll", 0);

                if (AfxLoadExecutables(1, &uri, &sys->e2sound)) AfxThrowError();
                else
                {
                    AfxAssertObjects(1, &sys->e2sound, afxFcc_EXE);
                    
                    afxClass*(*_AfxGetSsysClass)(void) = AfxFindExecutableSymbol(sys->e2sound, "_AfxGetSsysClass");

                    if (!_AfxGetSsysClass) AfxThrowError();
                    else
                    {
                        afxClass* cls = _AfxGetSsysClass();
                        AfxAssertClass(cls, afxFcc_SSYS);

                        if (AfxAcquireObjects(cls, 1, (afxObject*)&sys->ssys, (void const*[]) { sys, &ini, (void*)config->sound })) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &sys->ssys, afxFcc_SSYS);
                            AfxAssert(sys->ssys == AfxGetSoundSystem());
                        }

                        if (err)
                            AfxDismountClass(cls);
                    }

                    if (err)
                        AfxReleaseObjects(1, (void*[]) { sys->e2sound });
                }
            }

            if (!err)
            {
                afxBool dsysEnabled = FALSE;

                if (AfxIniGetBool(&ini, &AfxString("DrawSystem"), &AfxString("bEnabled"), &dsysEnabled) && dsysEnabled)
                {
                    AfxMakeUri(&uri, "e2draw.dll", 0);

                    if (AfxLoadExecutables(1, &uri, &sys->e2draw)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &sys->e2draw, afxFcc_EXE);

                        afxClass*(*_AfxGetDsysClass)(void) = AfxFindExecutableSymbol(sys->e2draw, "_AfxGetDsysClass");

                        if (!_AfxGetDsysClass) AfxThrowError();
                        else
                        {
                            afxClass* cls = _AfxGetDsysClass();
                            AfxAssertClass(cls, afxFcc_DSYS);

                            if (AfxAcquireObjects(cls, 1, (afxObject*)&sys->dsys, (void const*[]) { sys, &ini, (void*)config->draw })) AfxThrowError();
                            else
                            {
                                AfxAssertObjects(1, &sys->dsys, afxFcc_DSYS);
                                AfxAssert(sys->dsys == AfxGetDrawSystem());
                            }

                            if (err)
                                AfxDismountClass(cls);
                        }

                        if (err)
                            AfxReleaseObjects(1, (void*[]) { sys->e2draw });
                    }
                }

                if (!err)
                {
                    afxUri uri;
                    AfxMakeUri(&uri, "system/*.inf", 0);
                    _AfxSysScanForIcds(sys, &uri);

                    sys->operating = TRUE;
                    sys->interruptionRequested = FALSE;
                }

                if (err)
                {
                    if (sys->ssys)
                    {
                        AfxReleaseObjects(1, (void*[]) { sys->dsys });
                    }

                    if (sys->e2draw)
                        AfxReleaseObjects(1, (void*[]) { sys->e2draw });

                }
            }

            if (err)
            {
                if (sys->ssys)
                {
                    AfxReleaseObjects(1, (void*[]) { sys->ssys });
                }

                if (sys->e2sound)
                    AfxReleaseObjects(1, (void*[]) { sys->e2sound });

                AfxReleaseObjects(1, (void*[]) { sys });
            }
        }

        AfxCleanUpIni(&ini);
    }
    return err;
}
