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

_AFX afxChar const _rwBuildDateTime[];
static afxChar const  _rwBuildDateTime[] = "\nCore built at " __DATE__ " " __TIME__ "\n";
static afxChar const _Ebenezer[] = "Ebenezer";

extern afxSystem TheSystem;
extern afxBool sysReady;

_AFX afxReal64 const renderWareUnitsPerMeter = 1.f;
extern afxChain orphanClassChain;

_AFX afxError _AfxSysCtor(afxSystem sys, afxCookie const* cookie);
_AFX afxError _AfxSysDtor(afxSystem sys);

_AFX afxManager* _AfxGetSysMgr(void)
{
    //afxError err = AFX_ERR_NONE;
    static afxManager sysMgr = { 0 };
    static afxBool sysMgrReady = FALSE;
    static afxClassConfig const sysMgrCfg =
    {
        .fcc = afxFcc_SYS,
        .name = "System",
        .desc = "Basic I/O System",
        .maxCnt = 1,
        //.unitsPerPage = 1,
        //.size = sizeof(AFX_OBJECT(afxSystem)),
        .ctor = (void*)_AfxSysCtor,
        .dtor = (void*)_AfxSysDtor
    };
    
    if (sysMgr.fcc != afxFcc_CLS)
    {
        AfxEstablishManager(&sysMgr, NIL, NIL, &sysMgrCfg);
        sysMgrReady = TRUE;
    }
    return &sysMgr;
}

_AFX afxBool AfxGetSystem(afxSystem* system)
{
    afxError err = AFX_ERR_NONE;
    //AfxTryAssertObjects(1, &TheSystem, afxFcc_SYS);
    AfxAssert(system);
    *system = TheSystem;
    return sysReady;
}

_AFX afxChain* _AfxGetSystemClassChain(void)
{
    afxSystem sys;
    AfxGetSystem(&sys);
    return &sys->mgrChn;
}

_AFX afxBool AfxGetShell(afxShell* shell)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssert(shell);
    *shell = sys->env;
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
    afxManager *cls = &sys->fsysMgr;
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
    afxManager *cls = &sys->svcMgr;
    AfxAssertClass(cls, afxFcc_SVC);
    return cls;
}

_AFX afxManager* AfxGetThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->thrMgr;
    AfxAssertClass(cls, afxFcc_THR);
    return cls;
}

_AFX afxManager* AfxGetDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->devMgr;
    AfxAssertClass(cls, afxFcc_DEV);
    return cls;
}

_AFX afxManager* AfxGetEnvironmentClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->envMgr;
    AfxAssertClass(cls, afxFcc_ENV);
    return cls;
}

_AFX afxManager* AfxGetFileClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->fileMgr;
    AfxAssertClass(cls, afxFcc_FILE);
    return cls;
}

_AFX afxManager* AfxGetStringBaseClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->strbMgr;
    AfxAssertClass(cls, afxFcc_STRB);
    return cls;
}

_AFX afxManager* AfxGetArchiveClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->archMgr;
    AfxAssertClass(cls, afxFcc_ARC);
    return cls;
}

_AFX afxManager* AfxGetMmuClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->mmuMgr;
    AfxAssertClass(cls, afxFcc_MMU);
    return cls;
}

_AFX afxManager* AfxGetStreamClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->iosMgr;
    AfxAssertClass(cls, afxFcc_IOB);
    return cls;
}

_AFX afxManager* AfxGetHidClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->hidMgr;
    AfxAssertClass(cls, afxFcc_HID);
    return cls;
}

_AFX afxManager* AfxGetKeyboardClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->kbdMgr;
    AfxAssertClass(cls, afxFcc_KBD);
    return cls;
}

_AFX afxManager* AfxGetMouseClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->mseMgr;
    AfxAssertClass(cls, afxFcc_MSE);
    return cls;
}

_AFX afxManager* AfxGetControllerClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->ctrlMgr;
    AfxAssertClass(cls, afxFcc_CTRL);
    return cls;
}

_AFX afxManager* AfxGetModuleClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->exeMgr;
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

_AFX afxBool AfxEmitEvent(afxObject receiver, afxEvent* ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(receiver, afxFcc_OBJ);
    AfxAssertType(ev, afxFcc_EVNT);

    afxNat32 tid;
    AfxGetTid(&tid);
    
    if (tid == AfxGetObjectTid(receiver))
    {
        // Sends event event directly to receiver receiver, using the notify() function. Returns the value that was returned from the event handler.

        AfxNotifyObject(receiver, ev);
    }
    else
    {
        // Adds the event event, with the object receiver as the receiver of the event, to an event queue and returns immediately.

        AfxThrowError();
    }
}

_AFX afxError _SysCleanUpEnv(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxShell env;
    AfxGetShell(&env);

    if (env)
    {
        AfxAssertObjects(1, &env, afxFcc_ENV);

        if (!AfxReleaseObjects(1, (void*[]) { env }))
            AfxThrowError();

        sys->env = NIL;

        afxUri point, location;
        AfxMakeUri(&point, "code", 0);
        AfxMakeUri(&location, "system", 0);
        AfxDismountStorageUnit(&point, &location, afxFileFlag_RWX);
    }

    if (sys->stdKbd)
    {
        AfxReleaseObjects(1, (void*[]) { sys->stdKbd });
        sys->stdKbd = NIL;
    }

    return err;
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
            AfxDbgLogf(6, NIL, "Booting up the Qwadro Execution Ecosystem...\n");

            afxManifest ini;
            AfxSetUpIni(&ini);
            // platform-dependent ctor will load the correct ini file.

            AfxAssert(TheSystem == sys);
            AfxZero(TheSystem, sizeof(afxObjectBase));

            afxManager* mgr = _AfxGetSysMgr();
            AfxAssertClass(mgr, afxFcc_SYS);

            if (_AfxConstructObjects(mgr, 1, (void**)&TheSystem, (void*[]) { &ini, (void*)config })) AfxThrowError();
            else
            {
                AfxAssert(TheSystem != sys); // Attention! Ctor moves the object pointer to hide out the object base.
                sys = TheSystem;
                AfxAssertObjects(1, &sys, afxFcc_SYS);

                AfxLogY("Memory page size: %d", sys->memPageSize);
                AfxLogY("Logical processor count: %d", sys->hwConcurrencyCap);

                // Start the subsystems

                afxBool ssysDisabled = FALSE;

                if (!(AfxIniGetBool(&ini, &AfxString("SoundSystem"), &AfxString("bDisabled"), &ssysDisabled) && ssysDisabled))
                {
                    AfxDbgLogf(6, NIL, "Setting up the Qwadro Sound System...");

                    afxUri uri;
                    AfxMakeUri(&uri, "e2sound.dll", 0);
                    afxModule e2sound;

                    if (AfxLoadModule(&uri, NIL, &e2sound)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &e2sound, afxFcc_MDLE);

                        afxResult(*ssysctl)(afxSystem, afxInt, ...) = AfxFindModuleSymbol(e2sound, "_AaxSsysctl");

                        if (!ssysctl) AfxThrowError();
                        else
                        {
                            afxSoundSystem ssys;

                            if (ssysctl(sys, 1, ini, config ? config->sound : NIL)) AfxThrowError();
                            else
                            {
                                sys->ssysctl = ssysctl;
                                sys->e2sound = e2sound;
                            }
                        }

                        if (err)
                            AfxReleaseObjects(1, (void*[]) { e2sound });
                    }
                }

                if (!err)
                {
                    afxBool dsysDisabled = FALSE;

                    if (!(AfxIniGetBool(&ini, &AfxString("DrawSystem"), &AfxString("bDisabled"), &dsysDisabled) && dsysDisabled))
                    {
                        AfxDbgLogf(6, NIL, "Setting up the Qwadro Draw System...");

                        afxUri uri;
                        AfxMakeUri(&uri, "e2draw.dll", 0);
                        afxModule e2draw;

                        if (AfxLoadModule(&uri, NIL, &e2draw)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &e2draw, afxFcc_MDLE);

                            afxResult(*dsysctl)(afxSystem, afxInt, ...) = AfxFindModuleSymbol(e2draw, "_AvxDsysctl");

                            if (!dsysctl) AfxThrowError();
                            else
                            {
                                afxDrawSystem dsys;

                                if (dsysctl(sys, 1, ini, config ? config->draw : NIL)) AfxThrowError();
                                else
                                {
                                    sys->dsysctl = dsysctl;
                                    sys->e2draw = e2draw;
                                }
                            }

                            if (err)
                                AfxReleaseObjects(1, (void*[]) { e2draw });
                        }
                    }
                }

                if (!err)
                {
                    afxError err = AFX_ERR_NONE;
                    AfxAssertObjects(1, &sys, afxFcc_SYS);
                    AfxDbgLogf(6, NIL, "Setting up the Environment...");

                    afxKeyboard kbd;

                    if (AfxAcquireKeyboard(0, &kbd)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &kbd, afxFcc_KBD);
                        sys->stdKbd = kbd;
                        afxMouse mse;

                        if (AfxAcquireMouse(0, &mse)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &mse, afxFcc_MSE);
                            sys->stdMse = mse;

                            afxUri point, location;
                            AfxMakeUri(&point, "code", 0);
                            AfxMakeUri(&location, "system", 0);

                            if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX)) AfxThrowError();
                            else
                            {
                                if (AfxAcquireShell(&sys->env)) AfxThrowError();
                                else
                                {
                                    afxUri uri;
                                    AfxMakeUri(&uri, "system/qwadro.xss", 0);

                                    afxString s;
                                    AfxMakeString(&s, "qwadro", 0);
                                    AfxLoadScript(&s, &uri);
                                }

                                if (err)
                                    AfxDismountStorageUnit(&point, &location, afxFileFlag_RWX);
                            }

                            if (err)
                                AfxReleaseObjects(1, (void*[]) { mse });
                        }

                        if (err)
                        {
                            AfxReleaseObjects(1, (void*[]) { kbd });
                        }
                    }
                }

                if (!err)
                {
                    afxShell env;
                    AfxGetShell(&env);
                    AfxAssert(sys->env == env);

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

                    if (!err)
                    {

                        sys->operating = TRUE;
                        sys->interruptionRequested = FALSE;
                        sysReady = TRUE;
                    }

                    if (err)
                        _SysCleanUpEnv(sys);
                }

                if (err)
                {
                    if (_AfxDestructObjects(mgr, 1, (void*[]) { sys }))
                        AfxThrowError();

                    if (sys->dsysctl)
                    {
                        if (sys->dsysctl(sys, 0))
                            AfxThrowError();

                        AfxAssertObjects(1, &sys->e2draw, afxFcc_MDLE);
                        AfxReleaseObjects(1, (void*[]) { sys->e2draw });
                    }

                    if (sys->ssysctl)
                    {
                        if (sys->ssysctl(sys, 0))
                            AfxThrowError();

                        AfxAssertObjects(1, &sys->e2sound, afxFcc_MDLE);
                        AfxReleaseObjects(1, (void*[]) { sys->e2sound });
                    }
                }
            }

            AfxCleanUpIni(&ini);
        }
    }
    return err;
}

_AFX void AfxDoSystemShutdown(afxInt exitCode)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if (AfxGetSystem(&sys))
    {
        AfxDbgLogf(6, NIL, "Shutting down the Qwadro Execution Ecosystem...\n");

        do
        {
            AfxRequestSystemShutdown(exitCode);
            AfxDoSystemExecution(0);
        } while (AfxSystemIsExecuting());

        sysReady = FALSE;
        AfxCleanUpChainedManagers(&orphanClassChain);

        if (sys->dsysctl)
        {
            if (sys->dsysctl(sys, 2))
                AfxThrowError();

            AfxAssertObjects(1, &sys->e2draw, afxFcc_MDLE);
            AfxReleaseObjects(1, (void*[]) { sys->e2draw });
        }

        if (sys->ssysctl)
        {
            if (sys->ssysctl(sys, 2))
                AfxThrowError();

            AfxAssertObjects(1, &sys->e2sound, afxFcc_MDLE);
            AfxReleaseObjects(1, (void*[]) { sys->e2sound });
        }

        afxManager* mgr = _AfxGetSysMgr();
        AfxAssertClass(mgr, afxFcc_SYS);

        AfxAssert(TheSystem == sys);

        if (_AfxDestructObjects(mgr, 1, (void**)&TheSystem))
            AfxThrowError();

        AfxAssert(TheSystem != sys); // Attention! Dtor moves the object pointer to expose the object base.
        AfxZero(TheSystem, sizeof(afxObjectBase));
    }
#ifndef _AFX_DISABLE_DEBUGGER
    _AfxDbgDetach();
#endif
    AfxReleaseTerminal();
}
