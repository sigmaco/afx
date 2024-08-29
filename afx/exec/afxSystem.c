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
#include "../dev/afxDevCoreBase.h"

_AFX afxChar const _rwBuildDateTime[];
static afxChar const  _rwBuildDateTime[] = "\nCore built at " __DATE__ " " __TIME__ "\n";
static afxChar const _Ebenezer[] = "Ebenezer";

extern afxSystem TheSystem;
extern afxBool sysReady;
extern afxBool usysReady = FALSE;
extern afxBool ssysReady = FALSE;
extern afxBool msysReady = FALSE;

_AFX afxReal64 const renderwareUnitsPerMeter = 1.f;
extern afxChain orphanClassChain;

_AFX afxError _AfxSysCtor(afxSystem sys, void** args, afxNat invokeNo);
_AFX afxError _AfxSysDtor(afxSystem sys);
extern afxError _AfxLoadModule(afxUri const* uri, afxFlags flags, void* udd, afxModule* mdle);

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

_AFX afxNat32 AfxGetPrimeTid(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->primeTid;
}

_AFX afxBool AfxGetPrimeThread(afxThread* thread)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxThread thr = sys->primeThr;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    *thread = thr;
    return !!thr;
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
    if (AfxGetSystem(&sys))
    {
        AfxAssertObjects(1, &sys, afxFcc_SYS);
    }
    return sys ? sys->memPageSiz : 0;
}

_AFX afxNat AfxGetThreadingCapacity(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->hwThreadingCap;
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

_AFX afxClass const* AfxGetDriverClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &sys->icdCls;
    AfxAssertClass(cls, afxFcc_ICD);
    return cls;
}

_AFX afxClass const* AfxGetDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &sys->devCls;
    AfxAssertClass(cls, afxFcc_DEV);
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

_AFX afxString const* AfxGetSystemDirectoryString(afxString *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxString const* qwd = AfxGetUriString(&sys->qwd.uri);
    return dst ? AfxCopyString(dst, qwd), dst : qwd;
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

_AFX afxString const* AfxGetPwdString(afxString *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxString const* pwd = AfxGetUriString(&sys->pwd.uri);
    return dst ? AfxCopyString(dst, pwd), dst : pwd;
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

_AFX afxError _AfxDoSubsystemShutdown(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    usysReady = FALSE;
#if 0
    if (sys->aux.usys)
    {
        sys->aux.ioctl(sys, sys->aux.e2mmuxDll, 4, NIL);
    }
#endif
    if (sys->aux.e2mmuxDll)
    {
        AfxAssertObjects(1, &sys->aux.e2mmuxDll, afxFcc_MDLE);
        AfxReleaseObjects(1, &sys->aux.e2mmuxDll);
    }
    AfxZero(&sys->aux, sizeof(sys->aux));

    ssysReady = FALSE;
#if 0
    if (sys->asx.ssys)
    {
        sys->asx.ioctl(sys, sys->asx.e2soundDll, 4, NIL);
    }
#endif
    if (sys->asx.e2soundDll)
    {
        AfxAssertObjects(1, &sys->asx.e2soundDll, afxFcc_MDLE);
        AfxReleaseObjects(1, &sys->asx.e2soundDll);
    }
    AfxZero(&sys->asx, sizeof(sys->asx));

    msysReady = FALSE;
#if 0
    if (sys->amx.msys)
    {
        sys->amx.ioctl(sys, sys->amx.e2simDll, 4, NIL);
}
#endif
    if (sys->amx.e2simDll)
    {
        AfxAssertObjects(1, &sys->amx.e2simDll, afxFcc_MDLE);
        AfxReleaseObjects(1, &sys->amx.e2simDll);
    }
    AfxZero(&sys->amx, sizeof(sys->amx));
#if 0
    if (sys->avx.dsys)
    {
        sys->avx.ioctl(sys, sys->avx.e2drawDll, 4, NIL);
    }
#endif
    if (sys->avx.e2drawDll)
    {
        AfxAssertObjects(1, &sys->avx.e2drawDll, afxFcc_MDLE);
        AfxReleaseObjects(1, &sys->avx.e2drawDll);
    }

    AfxZero(&sys->avx, sizeof(sys->avx));
    return err;
}

_AFX afxBool _AfxParseDdevManifestCb(afxUri const* manifestUri, void* udd)
{
    afxError err = AFX_ERR_NONE;

    afxManifest ini;
    AfxSetUpIni(&ini);
    afxUri manifestFile;
    AfxClipUriFile(&manifestFile, manifestUri);
    AfxIniLoadFromFile(&ini, &manifestFile);

    afxDeviceType devType = afxDeviceType_DRAW;

    afxNat icdPagIdx, listPagIdx, listCnt, devPagIdx, recIdx;
    afxString devPag, rec, s;
    afxUri dllFile;

    if (AfxFindManifestPage(&ini, &AfxString("Qwadro.Icd"), &icdPagIdx) &&
        AfxFindManifestRecord(&ini, icdPagIdx, &AfxString("DllFile"), &recIdx) &&
        AfxGetManifestUri(&ini, icdPagIdx, recIdx, &dllFile))
    {
        afxDriver icd;

        if (_AfxLoadDriver(&dllFile, NIL, &icd))
        {

        }
    }

    AfxCleanUpIni(&ini);
    return TRUE;
}

_AFX afxError _AfxScanIcdManifests(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxUri fileMask;
    AfxMakeUri(&fileMask, 0, "//./z/*.inf", 0);
    AfxFindFiles(&fileMask, _AfxParseDdevManifestCb, sys);
}

_AFX afxError _AfxDoSubsystemStartUp(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    if (!(sys->avx.disabled))
    {
        AfxDbgLogf(6, NIL, "Doing the AVX start up...");

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2vgi", 0);
        afxModule e2drawDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2drawDll)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &e2drawDll, afxFcc_MDLE);
            sys->avx.e2drawDll = e2drawDll;

            afxError(*scmHookFn)(afxModule, afxManifest const*) = AfxFindModuleSymbol(e2drawDll, "afxScmHook");

            if (!scmHookFn) AfxThrowError();
            else
            {
                if (scmHookFn(e2drawDll, ini)) AfxThrowError();
                else
                {

                }
            }
#if 0
            if (!(sys->avx.ioctl = AfxFindModuleSymbol(e2drawDll, "AfxSystemIoctl"))) AfxThrowError();
            else
            {
                if (sys->avx.ioctl(sys, e2drawDll, 3, (void*[]) { &ini, &sys->classes })) AfxThrowError();
                else
                {
                    AfxAssert(sys->avx.dsys);
                }
            }
#endif
            if (err)
            {
                AfxReleaseObjects(1, &sys->avx.e2drawDll);
                sys->avx.e2drawDll = NIL;
            }
        }
    }

    if (!(sys->amx.disabled))
    {
        AfxDbgLogf(6, NIL, "Doing the AMX start up...");

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2pragma", 0);
        afxModule e2simDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2simDll)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &e2simDll, afxFcc_MDLE);
            sys->amx.e2simDll = e2simDll;

            afxError(*scmHookFn)(afxModule, afxManifest const*) = AfxFindModuleSymbol(e2simDll, "afxScmHook");

            if (!scmHookFn) AfxThrowError();
            else
            {
                if (scmHookFn(e2simDll, ini)) AfxThrowError();
                else
                {

                }
            }
#if 0
            if (!(sys->amx.ioctl = AfxFindModuleSymbol(e2soundDll, "AfxSystemIoctl"))) AfxThrowError();
            else
            {
                if (sys->amx.ioctl(sys, e2simDll, 3, (void*[]) { &ini, &sys->classes })) AfxThrowError();
                else
                {
                    AfxAssert(sys->amx.msys);
                }
            }
#endif
            if (err)
            {
                AfxReleaseObjects(1, &sys->amx.e2simDll);
                sys->amx.e2simDll = NIL;
            }
        }
    }

    if (!(sys->asx.disabled))
    {
        AfxDbgLogf(6, NIL, "Doing the ASX start up...");

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2sound", 0);
        afxModule e2soundDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2soundDll)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &e2soundDll, afxFcc_MDLE);
            sys->asx.e2soundDll = e2soundDll;

            afxError(*scmHookFn)(afxModule, afxManifest const*) = AfxFindModuleSymbol(e2soundDll, "afxScmHook");

            if (!scmHookFn) AfxThrowError();
            else
            {
                if (scmHookFn(e2soundDll, ini)) AfxThrowError();
                else
                {

                }
            }
#if 0
            if (!(sys->asx.ioctl = AfxFindModuleSymbol(e2soundDll, "AfxSystemIoctl"))) AfxThrowError();
            else
            {
                if (sys->asx.ioctl(sys, e2soundDll, 3, (void*[]) { &ini, &sys->classes })) AfxThrowError();
                else
                {
                    AfxAssert(sys->asx.ssys);
                }
            }
#endif
            if (err)
            {
                AfxReleaseObjects(1, &sys->asx.e2soundDll);
                sys->asx.e2soundDll = NIL;
            }
        }
    }

    if (!(sys->aux.disabled))
    {
        AfxDbgLogf(6, NIL, "Doing the AUX start up...");

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2mmux", 0);
        afxModule e2mmuxDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2mmuxDll)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &e2mmuxDll, afxFcc_MDLE);
            sys->aux.e2mmuxDll = e2mmuxDll;

            afxError(*scmHookFn)(afxModule, afxManifest const*) = AfxFindModuleSymbol(e2mmuxDll, "afxScmHook");

            if (!scmHookFn) AfxThrowError();
            else
            {
                if (scmHookFn(e2mmuxDll, ini)) AfxThrowError();
                else
                {

                }
            }
#if 0
            if (!(sys->aux.ioctl = AfxFindModuleSymbol(e2mmuxDll, "AfxSystemIoctl"))) AfxThrowError();
            else
            {
                if (sys->aux.ioctl(sys, e2mmuxDll, 3, (void*[]) { &ini, &sys->classes })) AfxThrowError();
                else
                {
                    AfxAssert(sys->aux.usys);
                }
            }
#endif
            if (err)
            {
                AfxReleaseObjects(1, &sys->aux.e2mmuxDll);
                sys->aux.e2mmuxDll = NIL;
            }
        }
    }

    _AfxScanIcdManifests(sys, ini);

    return err;
}

_AFX afxError AfxDoSystemBootUp(afxSystemConfig const *config)
{
    afxError err = AFX_ERR_NONE;

    if (!AfxReacquireConsole()) AfxThrowError();
    else
    {
#ifndef _AFX_DISABLE_DEBUGGER
        _AfxDbgAttach(NIL);
#endif
        afxSystem sys;

        if (AfxGetSystem(&sys)) AfxThrowError();
        else
        {
            AfxDbgLogf(6, NIL, "Booting up...\n");

            afxManifest ini;
            AfxSetUpIni(&ini);
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

                AfxLogY("Memory Page Size: %d", sys->memPageSiz);
                AfxLogY("HW Threading Capacity: %d", sys->hwThreadingCap);

                if (!err && _AfxDoSubsystemStartUp(sys, &ini))
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
                        AfxMakeUri(&uri, 0, "system/sound.xs", 0);
                        AfxLoadScript(&s, &uri);
                    }

                    if (sys->e2draw)
                    {
                        AfxMakeString(&s, "draw", 0);
                        AfxMakeUri(&uri, 0, "system/draw.xs", 0);
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
                    if (_AfxDoSubsystemShutdown(sys))
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
        AfxDbgLogf(6, NIL, "Shutting down...\n");
#if 0
        do
        {
            AfxRequestShutdown(exitCode);
            AfxDoSystemThreading(0);
        } while (AfxSystemIsExecuting());
#endif
        sysReady = FALSE;
        AfxCleanUpChainedClasses(&orphanClassChain);

        if (_AfxDoSubsystemShutdown(sys))
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
    AfxReleaseConsole();
}
