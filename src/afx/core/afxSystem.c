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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
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

                // Start the subsystems

                afxBool ssysDisabled = FALSE;

                if (!(AfxIniGetBool(&ini, &AfxString("SoundSystem"), &AfxString("bDisabled"), &ssysDisabled) && ssysDisabled))
                {
                    AfxDbgLogf(6, NIL, "Establishing the Qwadro Sound System...");

                    afxUri uri;
                    AfxMakeUri(&uri, "e2sound.dll", 0);
                    afxModule e2sound;

                    if (AfxLoadModule(&uri, NIL, &e2sound)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &e2sound, afxFcc_MDLE);
                        afxError(*entryPoint)(afxModule mdle, afxNat reqCode, void* udd) = AfxFindModuleSymbol(e2sound, "AfxEntryPoint");

                        if (!entryPoint || entryPoint(e2sound, afxFcc_SYS, (void const*[]) { sys, &ini, config }))
                            AfxThrowError();

                        if (err)
                            AfxReleaseObjects(1, &e2sound);

                        sys->e2sound = e2sound;
                    }
                }

                if (!err)
                {
                    afxBool dsysDisabled = FALSE;

                    if (!(AfxIniGetBool(&ini, &AfxString("DrawSystem"), &AfxString("bDisabled"), &dsysDisabled) && dsysDisabled))
                    {
                        AfxDbgLogf(6, NIL, "Establishing the Qwadro Draw System...");

                        afxUri uri;
                        AfxMakeUri(&uri, "e2draw.dll", 0);
                        afxModule e2draw;

                        if (AfxLoadModule(&uri, NIL, &e2draw)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &e2draw, afxFcc_MDLE);
                            afxError(*entryPoint)(afxModule mdle, afxNat reqCode, void* udd) = AfxFindModuleSymbol(e2draw, "AfxEntryPoint");

                            if (!entryPoint || entryPoint(e2draw, afxFcc_SYS, (void const*[]) { sys, &ini, config }))
                                AfxThrowError();

                            if (err)
                                AfxReleaseObjects(1, &e2draw);
                        }
                        sys->e2draw = e2draw;
                    }
                }

                if (!err)
                {
                    afxError err = AFX_ERR_NONE;
                    AfxAssertObjects(1, &sys, afxFcc_SYS);
                    AfxDbgLogf(6, NIL, "Establishing up the Environment...");
                    
                    afxUri uri;
                    AfxMakeUri(&uri, "e2ux", 0);
                    afxModule e2uxMdle;

                    if (AfxLoadModule(&uri, NIL, &e2uxMdle)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &e2uxMdle, afxFcc_MDLE);
                        afxError(*entryPoint)(afxModule mdle, afxNat reqCode, void* udd) = AfxFindModuleSymbol(e2uxMdle, "AfxEntryPoint");

                        if (!entryPoint || entryPoint(e2uxMdle, afxFcc_SYS, (void const*[]) { sys, &ini, NIL } ))
                            AfxThrowError();

                        if (err)
                            AfxReleaseObjects(1, &e2uxMdle);
                    }
                    sys->e2ux = e2uxMdle;
                }

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
                    if (_AfxDestructObjects(mgr, 1, (void*[]) { sys }))
                        AfxThrowError();

                    if (sys->e2ux)
                    {
                        AfxAssertObjects(1, &sys->e2ux, afxFcc_MDLE);
                        afxError(*entryPoint)(afxModule mdle, afxNat reqCode, void* udd) = AfxFindModuleSymbol(sys->e2ux, "AfxEntryPoint");

                        if (!entryPoint || entryPoint(sys->e2ux, afxFcc_SYS, NIL))
                            AfxThrowError();

                        AfxReleaseObjects(1, &sys->e2ux);
                    }

                    if (sys->e2draw)
                    {
                        AfxAssertObjects(1, &sys->e2draw, afxFcc_MDLE);
                        afxError(*entryPoint)(afxModule mdle, afxNat reqCode, void* udd) = AfxFindModuleSymbol(sys->e2draw, "AfxEntryPoint");

                        if (!entryPoint || entryPoint(sys->e2draw, afxFcc_SYS, NIL))
                            AfxThrowError();

                        AfxReleaseObjects(1, &sys->e2draw);
                    }

                    if (sys->e2sound)
                    {
                        AfxAssertObjects(1, &sys->e2sound, afxFcc_MDLE);
                        afxError(*entryPoint)(afxModule mdle, afxNat reqCode, void* udd) = AfxFindModuleSymbol(sys->e2sound, "AfxEntryPoint");

                        if (!entryPoint || entryPoint(sys->e2sound, afxFcc_SYS, NIL))
                            AfxThrowError();

                        AfxReleaseObjects(1, &sys->e2sound);
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
            AfxRequestShutdown(exitCode);
            AfxDoSystemExecution(0);
        } while (AfxSystemIsExecuting());

        sysReady = FALSE;
        AfxCleanUpChainedManagers(&orphanClassChain);

        if (sys->e2ux)
        {
            AfxAssertObjects(1, &sys->e2ux, afxFcc_MDLE);
            afxError(*entryPoint)(afxModule mdle, afxNat reqCode, void* udd) = AfxFindModuleSymbol(sys->e2ux, "AfxEntryPoint");

            if (!entryPoint || entryPoint(sys->e2ux, afxFcc_SYS, NIL))
                AfxThrowError();

            AfxReleaseObjects(1, &sys->e2ux);
        }

        if (sys->e2draw)
        {
            AfxAssertObjects(1, &sys->e2draw, afxFcc_MDLE);
            afxError(*entryPoint)(afxModule mdle, afxNat reqCode, void* udd) = AfxFindModuleSymbol(sys->e2draw, "AfxEntryPoint");

            if (!entryPoint || entryPoint(sys->e2draw, afxFcc_SYS, NIL))
                AfxThrowError();

            AfxReleaseObjects(1, &sys->e2draw);
        }

        if (sys->e2sound)
        {
            AfxAssertObjects(1, &sys->e2sound, afxFcc_MDLE);
            afxError(*entryPoint)(afxModule mdle, afxNat reqCode, void* udd) = AfxFindModuleSymbol(sys->e2sound, "AfxEntryPoint");

            if (!entryPoint || entryPoint(sys->e2sound, afxFcc_SYS, NIL))
                AfxThrowError();

            AfxReleaseObjects(1, &sys->e2sound);
        }

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
