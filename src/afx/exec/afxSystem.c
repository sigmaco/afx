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
#include "../src/afx/dev/afxDevCoreBase.h"

_AFX afxChar const _rwBuildDateTime[];
static afxChar const  _rwBuildDateTime[] = "\nCore built at " __DATE__ " " __TIME__ "\n";
static afxChar const _Ebenezer[] = "Ebenezer";

extern afxSystem TheSystem;
extern afxBool sysReady;
extern afxBool usysReady = FALSE;
extern afxBool ssysReady = FALSE;

_AFX afxReal64 const renderWareUnitsPerMeter = 1.f;
extern afxChain orphanClassChain;

_AFX afxError _AfxSysCtor(afxSystem sys, afxCookie const* cookie);
_AFX afxError _AfxSysDtor(afxSystem sys);

_AFX afxClass* _AfxGetSysMgr(void)
{
    //afxError err = AFX_ERR_NONE;
    static afxClass sysCls = { 0 };
    static afxClassConfig const sysClsCfg =
    {
        .fcc = afxFcc_SYS,
        .name = "System",
        .desc = "Basic I/O System",
        .maxCnt = 1,
        //.fixedSiz = sizeof(AFX_OBJECT(afxSystem)),
        .ctor = (void*)_AfxSysCtor,
        .dtor = (void*)_AfxSysDtor
    };
    
    if (sysCls.fcc != afxFcc_CLS)
    {
        AfxRegisterClass(&sysCls, NIL, NIL, &sysClsCfg);
    }
    return &sysCls;
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
    return &sys->classes;
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

_AFX afxClass* AfxGetStorageClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->fsysMgr;
    AfxAssertClass(cls, afxFcc_FSYS);
    return cls;
}

_AFX afxClass* AfxGetServiceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->svcMgr;
    AfxAssertClass(cls, afxFcc_SVC);
    return cls;
}

_AFX afxClass* AfxGetThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->thrMgr;
    AfxAssertClass(cls, afxFcc_THR);
    return cls;
}

_AFX afxClass* AfxGetDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->devMgr;
    AfxAssertClass(cls, afxFcc_DEV);
    return cls;
}

_AFX afxClass* AfxGetFileClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->fileMgr;
    AfxAssertClass(cls, afxFcc_FILE);
    return cls;
}

_AFX afxClass* AfxGetStringBaseClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->strbMgr;
    AfxAssertClass(cls, afxFcc_STRB);
    return cls;
}

_AFX afxClass* AfxGetArchiveClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->archMgr;
    AfxAssertClass(cls, afxFcc_ARC);
    return cls;
}

_AFX afxClass* AfxGetMmuClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->mmuMgr;
    AfxAssertClass(cls, afxFcc_MMU);
    return cls;
}

_AFX afxClass* AfxGetStreamClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->iosMgr;
    AfxAssertClass(cls, afxFcc_IOB);
    return cls;
}

_AFX afxClass* AfxGetModuleClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->exeMgr;
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

    if (sys->avx.dsys)
    {
        sys->avx.ioctl(sys, sys->avx.e2drawDll, 4, NIL);
    }

    if (sys->avx.e2drawDll)
    {
        AfxAssertObjects(1, &sys->avx.e2drawDll, afxFcc_MDLE);
        AfxReleaseObjects(1, &sys->avx.e2drawDll);
    }

    AfxZero(&sys->avx, sizeof(sys->avx));
    return err;
}

_AFX afxError _AfxDoAsxShutdown(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    ssysReady = FALSE;

    if (sys->asx.ssys)
    {
        sys->asx.ioctl(sys, sys->asx.e2soundDll, 4, NIL);
    }

    if (sys->asx.e2soundDll)
    {
        AfxAssertObjects(1, &sys->asx.e2soundDll, afxFcc_MDLE);
        AfxReleaseObjects(1, &sys->asx.e2soundDll);
    }
    AfxZero(&sys->asx, sizeof(sys->asx));
    return err;
}

_AFX afxError _AfxDoAuxShutdown(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    
    usysReady = FALSE;

    if (sys->aux.usys)
    {
        sys->aux.ioctl(sys, sys->aux.e2uxDll, 4, NIL);
    }

    if (sys->aux.e2uxDll)
    {
        AfxAssertObjects(1, &sys->aux.e2uxDll, afxFcc_MDLE);
        AfxReleaseObjects(1, &sys->aux.e2uxDll);
    }
    AfxZero(&sys->aux, sizeof(sys->aux));
    return err;
}

_AFX afxError _AfxDoAvxStartUp(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    AfxZero(&sys->avx, sizeof(sys->avx));
    afxBool dsysDisabled = FALSE;

    if (!(AfxIniGetBool(ini, &AfxString("DrawSystem"), &AfxString("bDisabled"), &dsysDisabled) && dsysDisabled))
    {
        AfxDbgLogf(6, NIL, "Doing the AVX start up...");

        afxUri uri;
        AfxMakeUri(&uri, "e2draw", 0);
        afxModule e2drawDll = NIL;

        if (AfxLoadModule(&uri, NIL, &e2drawDll)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &e2drawDll, afxFcc_MDLE);
            sys->avx.e2drawDll = e2drawDll;

            if (!(sys->avx.ioctl = AfxFindModuleSymbol(e2drawDll, "AfxSystemIoctl"))) AfxThrowError();
            else
            {
                if (sys->avx.ioctl(sys, e2drawDll, 3, (void*[]) { &ini, &sys->classes })) AfxThrowError();
                else
                {
                    AfxAssert(sys->avx.dsys);
                }
            }

            if (err)
            {
                AfxReleaseObjects(1, &sys->avx.e2drawDll);
                sys->avx.e2drawDll = NIL;
            }
        }
    }
    return err;
}

_AFX afxError _AfxDoAsxStartUp(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    AfxZero(&sys->asx, sizeof(sys->asx));
    afxBool ssysDisabled = FALSE;

    if (!(AfxIniGetBool(ini, &AfxString("SoundSystem"), &AfxString("bDisabled"), &ssysDisabled) && ssysDisabled))
    {
        AfxDbgLogf(6, NIL, "Doing the ASX start up...");

        afxUri uri;
        AfxMakeUri(&uri, "e2sound", 0);
        afxModule e2soundDll = NIL;

        if (AfxLoadModule(&uri, NIL, &e2soundDll)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &e2soundDll, afxFcc_MDLE);
            sys->asx.e2soundDll = e2soundDll;

            if (!(sys->asx.ioctl = AfxFindModuleSymbol(e2soundDll, "AfxSystemIoctl"))) AfxThrowError();
            else
            {
                if (sys->asx.ioctl(sys, e2soundDll, 3, (void*[]) { &ini, &sys->classes })) AfxThrowError();
                else
                {
                    AfxAssert(sys->asx.ssys);
                }
            }

            if (err)
            {
                AfxReleaseObjects(1, &sys->asx.e2soundDll);
                sys->asx.e2soundDll = NIL;
            }
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
        sys->aux.e2uxDll = e2uxDll;

        if (!(sys->aux.ioctl = AfxFindModuleSymbol(e2uxDll, "AfxSystemIoctl"))) AfxThrowError();
        else
        {
            if (sys->aux.ioctl(sys, e2uxDll, 3, (void*[]) { &ini, &sys->classes })) AfxThrowError();
            else
            {
                AfxAssert(sys->aux.usys);
            }
        }

        if (err)
        {
            AfxReleaseObjects(1, &sys->aux.e2uxDll);
            sys->aux.e2uxDll = NIL;
        }
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
            //TheSystem = malloc(sizeof(afxObjectBase) + sizeof(TheSystem[0]));

            afxClass* cls = _AfxGetSysMgr();
            AfxAssertClass(cls, afxFcc_SYS);

            if (_AfxConstructClassInstances(cls, 1, (void**)&TheSystem, (void*[]) { &ini, (void*)config })) AfxThrowError();
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

#if 0
                // Test path

                afxUri2048 uri, uri2;
                AfxMakeUri2048(&uri2, NIL);
                AfxMakeUri2048(&uri, NIL);
                
                //AfxCanonicalizePath(&uri.uri, TRUE);

                AfxFormatUri(&uri.uri, "art://./actor/");
                AfxResolveUri(afxFileFlag_R, &uri.uri, &uri2.uri);
                AfxFormatUri(&uri.uri, "//./art/actor/");
                AfxResolveUri(afxFileFlag_R, &uri.uri, &uri2.uri);
                AfxFormatUri(&uri.uri, "system://./e2draw.dll");
                AfxResolveUri(afxFileFlag_R, &uri.uri, &uri2.uri);
                AfxFormatUri(&uri.uri, "//./system/e2draw.dll");                
                AfxResolveUri(afxFileFlag_R, &uri.uri, &uri2.uri);
#endif

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

                    if (_AfxDestructClassInstances(cls, 1, (void*[]) { sys }))
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
#if 0
        do
        {
            AfxRequestShutdown(exitCode);
            AfxDoSystemExecution(0);
        } while (AfxSystemIsExecuting());
#endif
        sysReady = FALSE;
        AfxCleanUpChainedClasses(&orphanClassChain);

        if (_AfxDoAuxShutdown(sys))
            AfxThrowError();

        if (_AfxDoAsxShutdown(sys))
            AfxThrowError();

        if (_AfxDoAvxShutdown(sys))
            AfxThrowError();

        afxClass* cls = _AfxGetSysMgr();
        AfxAssertClass(cls, afxFcc_SYS);

        AfxAssert(TheSystem == sys);

        if (_AfxDestructClassInstances(cls, 1, (void**)&TheSystem))
            AfxThrowError();

        AfxAssert(TheSystem != sys); // Attention! Dtor moves the object pointer to expose the object hdr.
        AfxZero(TheSystem, sizeof(afxObjectBase));
        AfxDeregisterClass(cls);
    }
#ifndef _AFX_DISABLE_DEBUGGER
    _AfxDbgDetach();
#endif
    AfxReleaseTerminal();
}
