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
#include "qwadro/core/afxSystem.h"

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
"      ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::      \n"
"     :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:     \n"
"     +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+     \n"
"     +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+     \n"
"     +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+     \n"
"     #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#     \n"
"      ###### ###  ###   ###   ###     ### #########  ###    ###  ########      \n"
"                                                                               \n"
"              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              \n"
"                                                                               \n"
"                               Public Test Build                               \n"
"                   (c) 2017 SIGMA, Engineering In Technology                   \n"
"                                www.sigmaco.org                                \n"
"                                                                               \n"
);

_AFX afxManager _sysClass;
afxManager _sysClass;
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
    .name = "System",
    .desc = "Basic I/O System",
    .maxCnt = 1,
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSystem)),
    .mmu = NIL,
    .ctor = (void*)_AfxSysCtor,
    .dtor = (void*)_AfxSysDtor
};

_AFX afxBool AfxGetSystem(afxSystem* sys)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys2 = theSys;
    AfxTryAssertObjects(1, &sys2, afxFcc_SYS);
    AfxAssert(sys);
    *sys = sys2;
    return !!sys2;
}

_AFX afxChain* _AfxGetSystemClassChain(void)
{
    afxSystem sys;
    AfxGetSystem(&sys);
    return &sys->classes;
}

_AFX afxBool AfxGetDrawSystem(afxDrawSystem* dsys)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxDrawSystem dsys2 = sys->dsys;
    AfxTryAssertObjects(1, &dsys2, afxFcc_DSYS);
    AfxAssert(dsys);
    *dsys = dsys2;
    return !!dsys2;
}

_AFX afxBool AfxGetSoundSystem(afxSoundSystem* ssys)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxSoundSystem ssys2 = sys->ssys;
    AfxTryAssertObjects(1, &ssys2, afxFcc_SSYS);
    AfxAssert(ssys);
    *ssys = ssys2;
    return !!ssys2;
}

_AFX afxBool AfxGetEnvironment(afxEnvironment* env)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssert(env);
    *env = sys->env;
    return !!sys->env;
}

_AFX afxNat AfxGetIoBufferSize(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->ioBufSiz;
}

_AFX afxArena* AfxGetIoArena(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxArena *aren = &sys->ioArena;
    AfxAssertType(aren, afxFcc_AREN);
    return aren;
}

_AFX afxMmu AfxGetIoContext(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxMmu mmu = sys->mmu;
    AfxTryAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AFX afxMmu AfxGetSystemContext(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxMmu mmu = sys->mmu;
    AfxTryAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AFX afxNat AfxGetMemoryPageSize(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->memPageSize;
}

_AFX afxManager* AfxGetStorageClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->fsysCls;
    AfxAssertClass(cls, afxFcc_FSYS);
    return cls;
}

_AFX afxNat AfxGetThreadingCapacity(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->hwConcurrencyCap;
}

_AFX afxManager* AfxGetServiceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->svcCls;
    AfxAssertClass(cls, afxFcc_SVC);
    return cls;
}

_AFX afxManager* AfxGetThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->thrCls;
    AfxAssertClass(cls, afxFcc_THR);
    return cls;
}

_AFX afxManager* AfxGetTxuClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->txuCls;
    AfxAssertClass(cls, afxFcc_TXU);
    return cls;
}

_AFX afxManager* AfxGetDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->devCls;
    AfxAssertClass(cls, afxFcc_DEV);
    return cls;
}

_AFX afxManager* AfxGetEnvironmentClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->envCls;
    AfxAssertClass(cls, afxFcc_ENV);
    return cls;
}

_AFX afxManager* AfxGetFileClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->fileCls;
    AfxAssertClass(cls, afxFcc_FILE);
    return cls;
}

_AFX afxManager* AfxGetStringBaseClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->strbCls;
    AfxAssertClass(cls, afxFcc_STRB);
    return cls;
}

_AFX afxManager* AfxGetArchiveClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->archCls;
    AfxAssertClass(cls, afxFcc_ARC);
    return cls;
}

_AFX afxManager* AfxGetMmuClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->mmuCls;
    AfxAssertClass(cls, afxFcc_MMU);
    return cls;
}

_AFX afxManager* AfxGetStreamClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->iosCls;
    AfxAssertClass(cls, afxFcc_IOB);
    return cls;
}

_AFX afxManager* AfxGetHidClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->hidCls;
    AfxAssertClass(cls, afxFcc_HID);
    return cls;
}

_AFX afxManager* AfxGetKeyboardClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->kbdCls;
    AfxAssertClass(cls, afxFcc_KBD);
    return cls;
}

_AFX afxManager* AfxGetMouseClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->mseCls;
    AfxAssertClass(cls, afxFcc_MSE);
    return cls;
}

_AFX afxManager* AfxGetControllerClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->ctrlCls;
    AfxAssertClass(cls, afxFcc_CTRL);
    return cls;
}

_AFX afxManager* AfxGetModuleClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->exeCls;
    AfxAssertClass(cls, afxFcc_MDLE);
    return cls;
}

_AFX afxUri const* AfxGetSystemDirectory(afxUri *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxUri const* qwd = &sys->qwd.uri;
    return dst ? AfxCopyUri(dst, qwd), dst : qwd;
}

_AFX afxString const* AfxGetSystemDirectoryString(afxRestring *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxString const* qwd = AfxGetUriString(&sys->qwd.uri);
    return dst ? AfxCopyString(dst, qwd), &dst->str : qwd;
}

_AFX afxUri const* AfxGetPwd(afxUri *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxUri const* pwd = &sys->pwd.uri;
    return dst ? AfxCopyUri(dst, pwd), dst : pwd;
}

_AFX afxString const* AfxGetPwdString(afxRestring *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
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

    if (!AfxGetSystem(&sys)) AfxThrowError();
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

    if (AfxGetSystem(&sys))
    {
        AfxAssertObjects(1, &sys, afxFcc_SYS);
        return sys->operating && !sys->isInShutdown;
    }
    return FALSE;
}

_AFXINL afxBool _AfxNotifyObject(afxObject receiver, afxEvent *ev)
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

_AFX afxBool AfxEmitEvents(afxNat cnt, afxObject receiver[], afxEvent ev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(receiver);
    AfxAssert(ev);
    afxBool res = 0;
    
    for (afxNat i = 0; i < cnt; i++)
    {
        afxObject obj = receiver[i];
        AfxAssertType(obj, afxFcc_OBJ);
        afxEvent *e = &ev[i];
        AfxAssertType(e, afxFcc_EVNT);
        _AfxNotifyObject(obj, e);
    }
    return res;
}

_AFX afxBool AfxReemitEvent(afxNat cnt, afxObject receiver[], afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(receiver);
    AfxAssertType(ev, afxFcc_EVNT);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxObject obj = receiver[i];
        AfxAssertType(obj, afxFcc_OBJ);
        _AfxNotifyObject(obj, ev);
    }
    return err;
}

_AFX afxBool AfxEmitEvent(afxObject receiver, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(receiver, afxFcc_OBJ);
    AfxAssertType(ev, afxFcc_EVNT);
    return _AfxNotifyObject(receiver, ev);
}

_AFX afxError AfxPostEvents(afxNat cnt, afxObject receiver[], afxEvent ev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(receiver);
    AfxAssertType(ev, afxFcc_EVNT);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxObject obj = receiver[i];
        AfxAssertType(obj, afxFcc_OBJ);
        _AfxNotifyObject(obj, ev);
        AfxThrowError();
        // Not implemented yet.
    }
    return err;
}

_AFX afxError AfxPostEvent(afxObject receiver, afxEvent *ev)
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

    if (AfxGetSystem(&sys))
    {
        AfxDbgLogf(6, NIL, "Shutting down Qwadro Execution Ecosystem...\n");

        do { AfxRequestSystemShutdown(exitCode); } while (AfxSystemIsExecuting());
        while (!AfxReleaseObjects(1, (void*[]) { sys }));
        AfxCleanUpManager(&_sysClass);
    };
#ifndef _AFX_DISABLE_DEBUGGER
    _AfxDbgDetach();
#endif
    AfxReleaseTerminal();
}

_AFX afxError AfxDoSystemBootUp(afxSystemConfig const *config)
{
    afxError err = AFX_ERR_NONE;

    if (!AfxAcquireTerminal()) AfxThrowError();
    else
    {
#ifndef _AFX_DISABLE_DEBUGGER
        _AfxDbgAttach(NIL);
#endif
        afxSystem sys;

        if (AfxGetSystem(&sys)) AfxThrowError();
        else
        {
            AfxDbgLogf(6, NIL, "Booting up Qwadro Execution Ecosystem...\n");

            AfxSetUpManager(&_sysClass, NIL, NIL, &_AfxSysClsConfig);

            afxManifest ini;
            AfxSetUpIni(&ini);
            // platform-dependent ctor will load the correct ini file.

            if (AfxAcquireObjects(&_sysClass, 1, (afxObject*)&sys, (void const*[]) { &ini, config })) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &sys, afxFcc_SYS);
                
                afxUri uri;
                afxBool ssysEnabled = FALSE;

                if (AfxIniGetBool(&ini, &AfxString("SoundSystem"), &AfxString("bEnabled"), &ssysEnabled) && ssysEnabled)
                {
                    AfxDbgLogf(6, NIL, "Setting up the Qwadro Sound System...\n");

                    AfxMakeUri(&uri, "e2sound.dll", 0);

                    if (AfxLoadModule(&uri, NIL, &sys->e2sound)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &sys->e2sound, afxFcc_MDLE);

                        afxManager*(*_AfxGetSsysClass)(void) = AfxFindModuleSymbol(sys->e2sound, "_AfxGetSsysClass");

                        if (!_AfxGetSsysClass) AfxThrowError();
                        else
                        {
                            afxManager* cls = _AfxGetSsysClass();
                            AfxAssertClass(cls, afxFcc_SSYS);

                            if (AfxAcquireObjects(cls, 1, (afxObject*)&sys->ssys, (void const*[]) { sys, &ini, (void*)config ? config->sound : NIL })) AfxThrowError();
                            else
                            {
                                AfxAssertObjects(1, &sys->ssys, afxFcc_SSYS);
                                afxSoundSystem ssys;
                                AfxGetSoundSystem(&ssys);
                                AfxAssert(sys->ssys == ssys);
                            }

                            if (err)
                                AfxCleanUpManager(cls);
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
                        AfxDbgLogf(6, NIL, "Setting up the Qwadro Draw System...\n");

                        AfxMakeUri(&uri, "e2draw.dll", 0);

                        if (AfxLoadModule(&uri, NIL, &sys->e2draw)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &sys->e2draw, afxFcc_MDLE);

                            afxManager*(*_AfxGetDsysClass)(void) = AfxFindModuleSymbol(sys->e2draw, "_AfxGetDsysClass");

                            if (!_AfxGetDsysClass) AfxThrowError();
                            else
                            {
                                afxManager* cls = _AfxGetDsysClass();
                                AfxAssertClass(cls, afxFcc_DSYS);

                                if (AfxAcquireObjects(cls, 1, (afxObject*)&sys->dsys, (void const*[]) { sys, &ini, (void*)config ? config->draw : NIL })) AfxThrowError();
                                else
                                {
                                    AfxAssertObjects(1, &sys->dsys, afxFcc_DSYS);
                                    afxDrawSystem dsys;
                                    AfxGetDrawSystem(&dsys);
                                    AfxAssert(sys->dsys == dsys);
                                }

                                if (err)
                                    AfxCleanUpManager(cls);
                            }

                            if (err)
                                AfxReleaseObjects(1, (void*[]) { sys->e2draw });
                        }
                    }

                    if (!err)
                    {
                        afxUri uri;

                        afxUri point, location;
                        AfxMakeUri(&point, "code", 0);
                        AfxMakeUri(&location, "system", 0);
                        AfxMountStorageUnit(&point, &location, afxFileFlag_RWX);

                        if (!(sys->env = AfxAcquireEnvironment()))
                            AfxThrowError();

                        AfxMakeUri(&uri, "system/qwadro.xss", 0);

                        afxString s;
                        AfxMakeString(&s, "qwadro", 0);
                        AfxLoadScript(&s, &uri);
#if 0
                        if (sys->e2sound)
                        {

                            AfxMakeString(&s, "sound", 0);
                            AfxMakeUri(&uri, "system/sound.xs", 0);
                            AfxLoadScript(&s, &uri);
                        }

                        if (sys->e2draw)
                        {
                            AfxMakeString(&s, "draw", 0);
                            AfxMakeUri(&uri, "system/draw.xs", 0);
                            AfxLoadScript(&s, &uri);
                        }
#endif

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
    }
    return err;
}
