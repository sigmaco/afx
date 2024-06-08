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
extern afxBool usysReady = FALSE;
extern afxBool dsysReady = FALSE;
extern afxBool ssysReady = FALSE;

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

_AFX afxBool AfxGetDrawSystem(afxDrawSystem* system)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(system);
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxTryAssertObjects(1, &sys->dsys, afxFcc_DSYS);
    *system = sys->dsys;
    return dsysReady;
}

_AFX afxBool AfxGetSoundSystem(afxSoundSystem* system)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(system);
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxTryAssertObjects(1, &sys->ssys, afxFcc_SSYS);
    *system = sys->ssys;
    return ssysReady;
}

_AFX afxBool AfxGetShell(afxShell* shell)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(shell);
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxTryAssertObjects(1, &sys->usys, afxFcc_USYS);
    *shell = sys->usys;
    return usysReady;
}

_AFX afxChain* _AfxGetSystemClassChain(void)
{
    afxSystem sys;
    AfxGetSystem(&sys);
    return &sys->mgrChn;
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

_AFX afxNat AfxGetThreadingCapacity(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->hwConcurrencyCap;
}

_AFX afxManager* AfxGetShellClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->usysMgr;
    AfxAssertClass(cls, afxFcc_USYS);
    return cls;
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

_AFX afxManager* AfxGetDrawSystemClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->dsysMgr;
    AfxAssertClass(cls, afxFcc_DSYS);
    return cls;
}

_AFX afxManager* AfxGetSoundSystemClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManager *cls = &sys->ssysMgr;
    AfxAssertClass(cls, afxFcc_SSYS);
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

_AFX void AfxRequestShutdown(afxInt exitCode)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if (!AfxGetSystem(&sys)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sys, afxFcc_SYS);
        //_AfxInterruptionAllThreads(exitCode);
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
    AfxAssert(receiver);
    AfxAssert(ev);

    afxNat32 tid = AfxGetTid();
    
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
    return 0;
}

_AFX afxError _AfxDoAvxShutdown(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    dsysReady = FALSE;

    if (sys->dsys)
    {
        afxManager* mgr = AfxGetDrawSystemClass();
        AfxAssertClass(mgr, afxFcc_DSYS);

        AfxAssert(sys->dsysObj != sys->dsys); // Attention! Dtor moves the object pointer to expose the object base.

        if (_AfxDestructObjects(mgr, 1, (afxObject*)&sys->dsys)) // compound will avoid pointer movement
            AfxThrowError();

        AfxAssert(sys->dsysObj == sys->dsys); // Attention! Dtor moves the object pointer to expose the object base.
        sys->dsys = NIL;
        AfxZero(sys->dsysObj, sizeof(afxObjectBase));

        AfxAssert(!AfxCountObjects(mgr));
        AfxAbolishManager(mgr);
    }

    if (sys->e2drawDll)
    {
        AfxAssertObjects(1, &sys->e2drawDll, afxFcc_MDLE);
        AfxReleaseObjects(1, &sys->e2drawDll);
    }
    return err;
}

_AFX afxError _AfxDoAsxShutdown(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    ssysReady = FALSE;

    if (sys->ssys)
    {
        afxManager* mgr = AfxGetSoundSystemClass();
        AfxAssertClass(mgr, afxFcc_SSYS);

        AfxAssert(sys->ssysObj != sys->ssys); // Attention! Dtor moves the object pointer to expose the object base.

        if (_AfxDestructObjects(mgr, 1, (afxObject*)&sys->ssys)) // compound will avoid pointer movement
            AfxThrowError();

        AfxAssert(sys->ssysObj == sys->ssys); // Attention! Dtor moves the object pointer to expose the object base.
        sys->ssys = NIL;
        AfxZero(sys->ssysObj, sizeof(afxObjectBase));

        AfxAssert(!AfxCountObjects(mgr));
        AfxAbolishManager(mgr);
    }

    if (sys->e2soundDll)
    {
        AfxAssertObjects(1, &sys->e2soundDll, afxFcc_MDLE);
        AfxReleaseObjects(1, &sys->e2soundDll);
    }
    return err;
}

_AFX afxError _AfxDoAuxShutdown(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    
    usysReady = FALSE;

    if (sys->usys)
    {
        afxManager* mgr = AfxGetShellClass();
        AfxAssertClass(mgr, afxFcc_USYS);

        AfxAssert(sys->usysObj != sys->usys); // Attention! Dtor moves the object pointer to expose the object base.

        if (_AfxDestructObjects(mgr, 1, (afxObject*)&sys->usys)) // compound will avoid pointer movement
            AfxThrowError();

        AfxAssert(sys->usysObj == sys->usys); // Attention! Dtor moves the object pointer to expose the object base.
        sys->usys = NIL;
        AfxZero(sys->usysObj, sizeof(afxObjectBase));

        AfxAssert(!AfxCountObjects(mgr));
        AfxAbolishManager(mgr);
    }

    if (sys->e2uxDll)
    {
        AfxAssertObjects(1, &sys->e2uxDll, afxFcc_MDLE);
        AfxReleaseObjects(1, &sys->e2uxDll);
    }
    return err;
}

_AFX afxError _AfxDoAvxStartUp(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxDbgLogf(6, NIL, "Doing the AVX start up...");

    afxBool dsysDisabled = FALSE;

    if (!(AfxIniGetBool(ini, &AfxString("DrawSystem"), &AfxString("bDisabled"), &dsysDisabled) && dsysDisabled))
    {
        afxUri uri;
        AfxMakeUri(&uri, "e2draw", 0);
        afxModule e2drawDll = NIL;

        if (AfxLoadModule(&uri, NIL, &e2drawDll)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &e2drawDll, afxFcc_MDLE);
            sys->e2drawDll = e2drawDll;

            if (!(sys->avxctl = AfxFindModuleSymbol(e2drawDll, "AfxSystemIoctl"))) AfxThrowError();
            else
            {
                void* udd[2] = { 0 };

                if (sys->avxctl(sys, e2drawDll, 2, udd)) AfxThrowError();
                else
                {
                    afxClassConfig dsysMgrCfg = *(afxClassConfig*)udd[0];
                    afxObject dsysObj = udd[1];

                    AfxEstablishManager(&sys->dsysMgr, NIL, _AfxGetSystemClassChain(), &dsysMgrCfg);
                    afxManager* mgr = AfxGetDrawSystemClass();
                    AfxAssertClass(mgr, afxFcc_DSYS);

                    AfxZero(dsysObj, sizeof(afxObjectBase));
                    sys->dsysObj = dsysObj;
                    sys->dsys = (void*)dsysObj;

                    if (_AfxConstructObjects(mgr, 1, (void**)&sys->dsys, (void*[]) { sys, (void*)ini })) AfxThrowError();
                    else
                    {
                        afxDrawSystem dsys = sys->dsys;
                        AfxAssert(dsysObj != dsys); // Attention! Ctor moves the object pointer to hide out the object base.
                        AfxAssertObjects(1, &dsys, afxFcc_DSYS);
                        sys->dsys = dsys;
                        dsysReady = TRUE;
                    }
                }
            }

            if (err)
                AfxReleaseObjects(1, &sys->e2drawDll);
        }
    }
    return err;
}

_AFX afxError _AfxDoAsxStartUp(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxDbgLogf(6, NIL, "Doing the ASX start up...");

    afxBool ssysDisabled = FALSE;

    if (!(AfxIniGetBool(ini, &AfxString("SoundSystem"), &AfxString("bDisabled"), &ssysDisabled) && ssysDisabled))
    {
        afxUri uri;
        AfxMakeUri(&uri, "e2sound", 0);
        afxModule e2soundDll = NIL;

        if (AfxLoadModule(&uri, NIL, &e2soundDll)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &e2soundDll, afxFcc_MDLE);
            sys->e2soundDll = e2soundDll;

            if (!(sys->asxctl = AfxFindModuleSymbol(e2soundDll, "AfxSystemIoctl"))) AfxThrowError();
            else
            {
                void* udd[2] = { 0 };

                if (sys->asxctl(sys, e2soundDll, 2, udd)) AfxThrowError();
                else
                {
                    afxClassConfig ssysMgrCfg = *(afxClassConfig*)udd[0];
                    afxObject ssysObj = udd[1];

                    AfxEstablishManager(&sys->ssysMgr, NIL, _AfxGetSystemClassChain(), &ssysMgrCfg);
                    afxManager* mgr = AfxGetSoundSystemClass();
                    AfxAssertClass(mgr, afxFcc_SSYS);

                    AfxZero(ssysObj, sizeof(afxObjectBase));
                    sys->ssysObj = ssysObj;
                    sys->ssys = (void*)ssysObj;

                    if (_AfxConstructObjects(mgr, 1, (void**)&sys->ssys, (void*[]) { sys, (void*)ini })) AfxThrowError();
                    else
                    {
                        afxSoundSystem ssys = sys->ssys;
                        AfxAssert(ssysObj != ssys); // Attention! Ctor moves the object pointer to hide out the object base.
                        AfxAssertObjects(1, &ssys, afxFcc_SSYS);
                        sys->ssys = ssys;
                        ssysReady = TRUE;
                    }
                }
            }

            if (err)
                AfxReleaseObjects(1, &sys->e2soundDll);
        }
    }
    return err;
}

_AFX afxError _AfxDoAuxStartUp(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxDbgLogf(6, NIL, "Doing the AUX start up...");

    afxUri uri;
    AfxMakeUri(&uri, "e2ux", 0);
    afxModule e2uxDll = NIL;

    if (AfxLoadModule(&uri, NIL, &e2uxDll)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &e2uxDll, afxFcc_MDLE);
        sys->e2uxDll = e2uxDll;

        if (!(sys->auxctl = AfxFindModuleSymbol(e2uxDll, "AfxSystemIoctl"))) AfxThrowError();
        else
        {
            void* udd[2] = { 0 };

            if (sys->auxctl(sys, e2uxDll, 2, udd)) AfxThrowError();
            else
            {
                afxClassConfig usysMgrCfg = *(afxClassConfig*)udd[0];
                afxObject usysObj = udd[1];

                AfxEstablishManager(&sys->usysMgr, NIL, _AfxGetSystemClassChain(), &usysMgrCfg);
                afxManager* mgr = AfxGetShellClass();
                AfxAssertClass(mgr, afxFcc_USYS);
                
                AfxZero(usysObj, sizeof(afxObjectBase));
                sys->usysObj = usysObj;
                sys->usys = (void*)usysObj;

                if (_AfxConstructObjects(mgr, 1, (void**)&sys->usys, (void*[]) { sys, (void*)ini })) AfxThrowError();
                else
                {
                    afxShell usys = sys->usys;
                    AfxAssert(usysObj != usys); // Attention! Ctor moves the object pointer to hide out the object base.
                    AfxAssertObjects(1, &usys, afxFcc_USYS);
                    sys->usys = usys;
                    usysReady = TRUE;
                }
            }
        }

        if (err)
            AfxReleaseObjects(1, &sys->e2uxDll);
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
            //AfxSetUpIni(&ini);
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

                if (!err && _AfxDoAvxStartUp(sys, &ini))
                    AfxThrowError();

                if (!err && _AfxDoAsxStartUp(sys, &ini))
                    AfxThrowError();

                if (!err && _AfxDoAuxStartUp(sys, &ini))
                    AfxThrowError();

                if (!err)
                {

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
                }

                if (err)
                {
                    if (_AfxDoAuxShutdown(sys))
                        AfxThrowError();

                    if (_AfxDoAsxShutdown(sys))
                        AfxThrowError();

                    if (_AfxDoAvxShutdown(sys))
                        AfxThrowError();

                    if (_AfxDestructObjects(mgr, 1, (void*[]) { sys }))
                        AfxThrowError();
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
            AfxRequestShutdown(exitCode);
            AfxDoSystemExecution(0);
        } while (AfxSystemIsExecuting());

        sysReady = FALSE;
        AfxCleanUpChainedManagers(&orphanClassChain);

        if (_AfxDoAuxShutdown(sys))
            AfxThrowError();

        if (_AfxDoAsxShutdown(sys))
            AfxThrowError();

        if (_AfxDoAvxShutdown(sys))
            AfxThrowError();

        afxManager* mgr = _AfxGetSysMgr();
        AfxAssertClass(mgr, afxFcc_SYS);

        AfxAssert(TheSystem == sys);

        if (_AfxDestructObjects(mgr, 1, (void**)&TheSystem))
            AfxThrowError();

        AfxAssert(TheSystem != sys); // Attention! Dtor moves the object pointer to expose the object hdr.
        AfxZero(TheSystem, sizeof(afxObjectBase));
    }
#ifndef _AFX_DISABLE_DEBUGGER
    _AfxDbgDetach();
#endif
    AfxReleaseTerminal();
}
