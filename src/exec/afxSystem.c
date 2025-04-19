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
#include "../impl/afxExecImplKit.h"

_AFX afxChar const _rwBuildDateTime[];
static afxChar const  _rwBuildDateTime[] = "\nCore built at " __DATE__ " " __TIME__ "\n";

extern afxSystem TheSystem;
extern afxBool sysReady;
_AFX afxBool usysReady = FALSE;
_AFX afxBool ssysReady = FALSE;
_AFX afxBool msysReady = FALSE;

_AFX afxReal64 const renderwareUnitsPerMeter = 1.f;
extern afxChain orphanClassChain;

_AFX afxError _AfxSysCtor(afxSystem sys, void** args, afxUnit invokeNo);
_AFX afxError _AfxSysDtor(afxSystem sys);
extern afxError _AfxAttachDriver(afxModule mdle, afxUri const* manifest, void* udd);

extern afxError avxScmHook(afxModule mdle, afxManifest const* ini);
extern afxError amxScmHook(afxModule mdle, afxManifest const* ini);
extern afxError auxScmHook(afxModule mdle, afxManifest const* ini);

_AFX afxClass* _AfxGetSysMgr(void)
{
    //afxError err = AFX_ERR_NONE;
    static afxClass sysCls = { 0 };
    static afxClassConfig const sysClsCfg =
    {
        .fcc = afxFcc_SYS,
        .name = "System",
        .desc = "Base I/O System",
        .maxCnt = 1,
        //.fixedSiz = sizeof(AFX_OBJECT(afxSystem)),
        .fixedSiz = 0,
        .ctor = (void*)_AfxSysCtor,
        .dtor = (void*)_AfxSysDtor
    };
    
    if (sysCls.fcc != afxFcc_CLS)
    {
        AfxMountClass(&sysCls, NIL, NIL, &sysClsCfg);
    }
    return &sysCls;
}

_AFX afxBool AfxGetSystem(afxSystem* system)
{
    afxError err = AFX_ERR_NONE;
    //AfxTryAssertObjects(1, &TheSystem, afxFcc_SYS);
    AFX_ASSERT(system);
    *system = TheSystem;
    return sysReady;
}

_AFX afxUnit32 AfxGetPrimeTid(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    return sys->primeTid;
}

_AFX afxBool AfxGetPrimeThread(afxThread* thread)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxThread thr = sys->primeThr;
    AFX_ASSERT_OBJECTS(afxFcc_THR, 1, &thr);
    *thread = thr;
    return !!thr;
}

_AFX afxChain* _AfxGetSystemClassChain(void)
{
    afxSystem sys;
    AfxGetSystem(&sys);
    return &sys->classes;
}

_AFX afxUnit AfxGetIoBufferSize(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    return sys->ioBufSiz;
}

_AFX afxUnit AfxGetMemoryPageSize(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    if (AfxGetSystem(&sys))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    }
    return sys ? sys->memPageSiz : 0;
}

_AFX afxUnit AfxGetThreadingCapacity(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    return sys->hwThreadingCap;
}

_AFX afxClass const* _AfxGetIoBridgeClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass const* cls = &sys->exuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_EXU);
    return cls;
}

_AFX afxClass const* _AfxGetStreamClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass const* cls = &sys->iosCls;
    AFX_ASSERT_CLASS(cls, afxFcc_IOB);
    return cls;
}

_AFX afxClass* _AfxGetStorageClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->fsysCls;
    AFX_ASSERT_CLASS(cls, afxFcc_FSYS);
    return cls;
}

_AFX afxClass* _AfxGetServiceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->svcCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SVC);
    return cls;
}

_AFX afxClass* _AfxGetThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->thrCls;
    AFX_ASSERT_CLASS(cls, afxFcc_THR);
    return cls;
}

_AFX afxClass* _AfxGetThreadPoolClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->thrpCls;
    AFX_ASSERT_CLASS(cls, afxFcc_THRP);
    return cls;
}

_AFX afxClass* _AfxGetStringBaseClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->strbCls;
    AFX_ASSERT_CLASS(cls, afxFcc_STRB);
    return cls;
}

_AFX afxClass* _AfxGetMmuClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->mmuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MMU);
    return cls;
}

_AFX afxClass* _AfxGetModuleClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->mdleCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDLE);
    return cls;
}

_AFX afxClass const* _AfxGetDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass const* cls = &sys->devCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DEV);
    return cls;
}

_AFX afxUri const* AfxGetSystemDirectory(afxUri *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxUri const* qwd = &sys->qwd.uri;
    return dst ? AfxCopyUri(dst, qwd), dst : qwd;
}

_AFX afxString const* AfxGetSystemDirectoryString(afxString *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxString const* qwd = AfxGetUriString(&sys->qwd.uri);
    return dst ? AfxCopyString(dst, 0, qwd, 0), dst : qwd;
}

_AFX afxUri const* AfxGetPwd(afxUri *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxUri const* pwd = &sys->pwd.uri;
    return dst ? AfxCopyUri(dst, pwd), dst : pwd;
}

_AFX afxString const* AfxGetPwdString(afxString *dst)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxString const* pwd = AfxGetUriString(&sys->pwd.uri);
    return dst ? AfxCopyString(dst, 0, pwd, 0), dst : pwd;
}

_AFX void AfxRequestShutdown(afxInt exitCode)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if (!AfxGetSystem(&sys)) AfxThrowError();
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
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
        AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
        return sys->operating && !sys->isInShutdown;
    }
    return FALSE;
}

_AFX afxBool AfxEmitEvent(afxObject receiver, afxEvent* ev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(receiver);
    AFX_ASSERT(ev);

    afxUnit32 tid = AfxGetTid();
    
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
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    usysReady = FALSE;
#if 0
    if (sys->aux.usys)
    {
        sys->aux.ioctl(sys, sys->aux.e2mmuxDll, 4, NIL);
    }
#endif
    if (sys->aux.e2mmuxDll)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &sys->aux.e2mmuxDll);
        AfxDisposeObjects(1, &sys->aux.e2mmuxDll);
    }
    //AfxZero(&sys->aux, sizeof(sys->aux));

    msysReady = FALSE;
#if 0
    if (sys->asx.ssys)
    {
        sys->asx.ioctl(sys, sys->asx.e2soundDll, 4, NIL);
    }
#endif
    if (sys->amx.e2mixDll)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &sys->amx.e2mixDll);
        AfxDisposeObjects(1, &sys->amx.e2mixDll);
    }
    //AfxZero(&sys->asx, sizeof(sys->asx));

    ssysReady = FALSE;
#if 0
    if (sys->amx.msys)
    {
        sys->amx.ioctl(sys, sys->amx.e2simDll, 4, NIL);
}
#endif
    if (sys->asx.e2simDll)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &sys->asx.e2simDll);
        AfxDisposeObjects(1, &sys->asx.e2simDll);
    }
    //AfxZero(&sys->amx, sizeof(sys->amx));
#if 0
    if (sys->avx.dsys)
    {
        sys->avx.ioctl(sys, sys->avx.e2drawDll, 4, NIL);
    }
#endif
    if (sys->avx.e2drawDll)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &sys->avx.e2drawDll);
        AfxDisposeObjects(1, &sys->avx.e2drawDll);
    }

    //AfxZero(&sys->avx, sizeof(sys->avx));
    return err;
}

_AFX afxBool _AfxParseDdevManifestCb(afxUri const* manifestUri, void* udd)
{
    afxError err = AFX_ERR_NONE;

    afxManifest ini;
    AfxDeployManifest(&ini);
    afxUri manifestFile;
    AfxClipUriFile(&manifestFile, manifestUri);
    AfxLoadInitializationFile(&ini, &manifestFile);

    afxDeviceType devType = afxDeviceType_DRAW;

    afxUnit icdPagIdx, listPagIdx, listCnt, devPagIdx, recIdx;
    afxString devPag, rec, s;
    afxUri dllFile;

    if (AfxFindInitializationSection(&ini, &AFX_STRING("Qwadro.Icd"), &icdPagIdx) &&
        AfxFindInitializationRecord(&ini, icdPagIdx, &AFX_STRING("DllFile"), &recIdx) &&
        AfxGetManifestUri(&ini, icdPagIdx, recIdx, &dllFile))
    {
        afxDriver icd;

        //if (_AfxLoadDriver(&dllFile, NIL, &icd))
        {

        }
    }

    AfxDismantleManifest(&ini);
    return TRUE;
}

_AFX afxBool _AfxParseDriverManifestCb(afxSystem sys, afxUri const* manifestUri)
{
    afxError err = AFX_ERR_NONE;

    afxManifest ini;
    AfxDeployManifest(&ini);
    afxUri manifestFile;
    AfxClipUriFile(&manifestFile, manifestUri);
    AfxLoadInitializationFile(&ini, &manifestFile);

    afxDeviceType devType = afxDeviceType_DRAW;

    afxUnit icdPagIdx, listPagIdx, listCnt, devPagIdx, recIdx;
    afxString devPag, rec, s;
    afxUri dllFile;

    if (AfxFindInitializationSection(&ini, &AFX_STRING("Qwadro.Icd"), &icdPagIdx) &&
        AfxFindInitializationRecord(&ini, icdPagIdx, &AFX_STRING("DllFile"), &recIdx) &&
        AfxGetManifestUri(&ini, icdPagIdx, recIdx, &dllFile))
    {
        afxDriver icd;

        //if (_AfxLoadDriver(&dllFile, NIL, &icd))
        {

        }
    }

    AfxDismantleManifest(&ini);
    return TRUE;
}

_AFX afxBool _AfxLoadAndAttachIcd(void* udd, afxUnit diskId, afxUnit endpointIdx, afxUri const* path, afxUri const* osPath)
{
    afxError err = AFX_ERR_NONE;
    afxBool next = TRUE;

    afxManifest ini;
    afxUri manifestFile;
    AfxDeployManifest(&ini);
    AfxClipUriPath(&manifestFile, osPath);
    AfxLoadInitializationFile(&ini, osPath);

    afxUri basePath, fname, fext;
    AfxExcerptPathSegments(path, &basePath, &basePath, &fname, &fext);

    afxUri2048 dllFile;
    AfxMakeUri2048(&dllFile, NIL);
    AfxFormatUri(&dllFile.uri, "%.*s/%.*s.dll", AfxPushString(&basePath.s), AfxPushString(&fname.s));

    afxModule icd;
    if (AfxLoadModule(&dllFile.uri, NIL, &icd))
        AfxThrowError();

    AfxDismantleManifest(&ini);

    return next;
}

_AFX afxError _AfxScanIcdManifests(afxSystem sys, afxManifest const* sysIni)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxUri fileMask;
    AfxMakeUri(&fileMask, 0, "//./z/*.inf", 0);
    AfxFindFiles(&fileMask, afxFileFlag_R, (void*)_AfxLoadAndAttachIcd, sys);
#if 0
    afxFile file;

    if (!AfxOpen(&fileMask, afxFileFlag_R, &file))
    {
        do
        {
            afxUri2048 fpath;
            AfxMakeUri2048(&fpath, NIL);
            AfxGetResolvedFileUri(file, &fpath.uri);

            afxManifest ini;
            afxUri manifestFile;
            AfxDeployManifest(&ini);
            AfxClipUriPath(&manifestFile, &fpath.uri);
            AfxLoadInitializationFile(&ini, &manifestFile);

            AfxGetFileUri(file, &fpath.uri);

            afxUri fnameOnly;
            AfxClipUriTarget(&fnameOnly, &fpath.uri);
            
            afxUri2048 dllFile;
            AfxMakeUri2048(&dllFile, NIL);
            AfxFormatUri(&dllFile.uri, "%.*s/%.*s.dll", AfxPushString(&fpath.uri.s), AfxPushString(&fnameOnly.s));

            afxModule icd;

            if (AfxLoadModule(&dllFile.uri, NIL, &icd)) AfxThrowError();
            else
            {
                afxError(*icdHookFn)(afxModule, afxUri const*) = AfxFindModuleSymbol(icd, "afxIcdHook");

                if (icdHookFn)
                {
                    if (_AfxAttachDriver(icd, &ini, NIL))
                    {

                    }
                }
                else
                {
                    afxError(*asiHookFn)(afxModule, afxUri const*) = AfxFindModuleSymbol(icd, "afxAsiHook");

                    if (!asiHookFn) AfxThrowError();
                    else
                    {

                    }
                }
            }

            AfxDismantleManifest(&ini);
        }
        while (AfxFindNextFile(file));
        AfxDisposeObjects(1, &file);
    }
#endif
    //AfxForEachUriResolution(afxFileFlag_R, &fileMask, (void*)_AfxParseDriverManifestCb, sys);
    int a = 0;
}

_AFX afxError AfxBootstrapSystem(afxSystemConfig const *config)
{
    afxError err = AFX_ERR_NONE;

    if (!AfxReacquireConsole())
    {
        AfxThrowError();
        return err;
    }

    afxSystem sys;
    if (AfxGetSystem(&sys))
    {
        AfxThrowError();
        return err;
    }


#ifndef _AFX_DISABLE_DEBUGGER
    _AfxDbgAttach(NIL);
#endif
    AfxReportf(6, NIL, "Booting...");

    afxManifest ini;
    AfxDeployManifest(&ini);
    // platform-dependent ctor will load the correct ini file.

    afxClass* cls = _AfxGetSysMgr();
    AFX_ASSERT_CLASS(cls, afxFcc_SYS);

    AFX_ASSERT(TheSystem == sys);
    AfxZero(TheSystem, sizeof(afxObjectBase));
    if (_AfxConstructObjects(cls, 1, (void**)&TheSystem, (void*[]) { &ini, (void*)config }))
        AfxThrowError();
    else
    {
        AFX_ASSERT(TheSystem != sys); // Attention! Ctor moves the object pointer to hide out the object base.
        sys = TheSystem;
        AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

        AfxReportY("Memory page Size: %d", sys->memPageSiz);
        AfxReportY("HW threading: %d", sys->hwThreadingCap);
    }

    // Loading modulations
    AfxDeployChain(&sys->avx.icdChain, sys);
    AfxDeployChain(&sys->amx.icdChain, sys);
    AfxDeployChain(&sys->aux.icdChain, sys);
    AfxDeployChain(&sys->asx.icdChain, sys);

    avxScmHook(sys->e2coree, &ini);
    amxScmHook(sys->e2coree, &ini);
    auxScmHook(sys->e2coree, &ini);

#if 0
    if (!err)
    {
        // Loading AVX

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2targa", 0);
        afxModule e2drawDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2drawDll)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &e2drawDll);
            sys->avx.e2drawDll = e2drawDll;
        }
    }

    if (!err)
    {
        // Loding AMX

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2mixva", 0);
        afxModule e2mixDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2mixDll)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &e2mixDll);
            sys->amx.e2mixDll = e2mixDll;
        }
    }

    if (!err)
    {
        // Loading AUX

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2mmux", 0);
        afxModule e2mmuxDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2mmuxDll)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &e2mmuxDll);
            sys->aux.e2mmuxDll = e2mmuxDll;

            if (err)
            {
                AfxDisposeObjects(1, &sys->aux.e2mmuxDll);
                sys->aux.e2mmuxDll = NIL;
            }
        }
    }
#endif

    if (!err)
    {
        // Loading ASX

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2synerg", 0);
        afxModule e2simDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2simDll))
        {
            AfxThrowError();
            err = 0;
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &e2simDll);
            sys->asx.e2simDll = e2simDll;
        }
    }

    _AfxScanIcdManifests(sys, &ini);

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

        if (_AfxDestructObjects(cls, 1, (void*[]) { sys }))
            AfxThrowError();
    }

    AfxDismantleManifest(&ini);
    return err;
}

_AFX void AfxDoSystemShutdown(afxInt exitCode)
{
    afxError err = AFX_ERR_NONE;

    afxSystem sys;
    if (AfxGetSystem(&sys))
    {
        AfxReportf(6, NIL, "Shutting down...\n");
#if 0
        do
        {
            AfxRequestShutdown(exitCode);
            AfxDoSystemThreading(0);
        } while (AfxSystemIsExecuting());
#endif
        sysReady = FALSE;
        AfxDeregisterChainedClasses(&orphanClassChain);

        if (_AfxDoSubsystemShutdown(sys))
            AfxThrowError();

        afxClass* cls = _AfxGetSysMgr();
        AFX_ASSERT_CLASS(cls, afxFcc_SYS);

        AFX_ASSERT(TheSystem == sys);
        if (_AfxDestructObjects(cls, 1, (void**)&TheSystem))
            AfxThrowError();

        AfxDismountClass(cls);

        AFX_ASSERT(TheSystem != sys); // Attention! Dtor moves the object pointer to expose the object hdr.
        AfxZero(TheSystem, sizeof(afxObjectBase));
    }
#ifndef _AFX_DISABLE_DEBUGGER
    _AfxDbgDetach();
#endif
    AfxReleaseConsole();
}
