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
#include "../dev/afxExecImplKit.h"

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

_AFX afxClass const* AfxGetIoBridgeClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass const* cls = &sys->exuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_EXU);
    return cls;
}

_AFX afxClass const* AfxGetStreamClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass const* cls = &sys->iosCls;
    AFX_ASSERT_CLASS(cls, afxFcc_IOB);
    return cls;
}

_AFX afxClass* AfxGetStorageClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->fsysCls;
    AFX_ASSERT_CLASS(cls, afxFcc_FSYS);
    return cls;
}

_AFX afxClass* AfxGetServiceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->svcCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SVC);
    return cls;
}

_AFX afxClass* AfxGetThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->thrCls;
    AFX_ASSERT_CLASS(cls, afxFcc_THR);
    return cls;
}

_AFX afxClass* AfxGetStringBaseClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->strbCls;
    AFX_ASSERT_CLASS(cls, afxFcc_STRB);
    return cls;
}

_AFX afxClass* AfxGetMmuClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->mmuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MMU);
    return cls;
}

_AFX afxClass* AfxGetModuleClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass *cls = &sys->mdleCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDLE);
    return cls;
}

_AFX afxClass const* AfxGetDeviceClass(void)
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
    return dst ? AfxCopyString(dst, qwd), dst : qwd;
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
    return dst ? AfxCopyString(dst, pwd), dst : pwd;
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

    ssysReady = FALSE;
#if 0
    if (sys->asx.ssys)
    {
        sys->asx.ioctl(sys, sys->asx.e2soundDll, 4, NIL);
    }
#endif
    if (sys->asx.e2soundDll)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &sys->asx.e2soundDll);
        AfxDisposeObjects(1, &sys->asx.e2soundDll);
    }
    //AfxZero(&sys->asx, sizeof(sys->asx));

    msysReady = FALSE;
#if 0
    if (sys->amx.msys)
    {
        sys->amx.ioctl(sys, sys->amx.e2simDll, 4, NIL);
}
#endif
    if (sys->amx.e2simDll)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &sys->amx.e2simDll);
        AfxDisposeObjects(1, &sys->amx.e2simDll);
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

    if (AfxFindInitializationSection(&ini, &AfxString("Qwadro.Icd"), &icdPagIdx) &&
        AfxFindInitializationRecord(&ini, icdPagIdx, &AfxString("DllFile"), &recIdx) &&
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

    if (AfxFindInitializationSection(&ini, &AfxString("Qwadro.Icd"), &icdPagIdx) &&
        AfxFindInitializationRecord(&ini, icdPagIdx, &AfxString("DllFile"), &recIdx) &&
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
    AfxFormatUri(&dllFile.uri, "%.*s/%.*s.dll", AfxPushString(&basePath.str), AfxPushString(&fname.str));

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
            AfxFormatUri(&dllFile.uri, "%.*s/%.*s.dll", AfxPushString(&fpath.uri.str), AfxPushString(&fnameOnly.str));

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

_AFX afxError _AfxDoSubsystemStartUp(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    if (!(sys->avx.disabled))
    {
        //AfxDbgLogf(6, NIL, "Doing the AVX start up...");

        AfxDeployChain(&sys->avx.icdChain, sys);

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2draw", 0);
        afxModule e2drawDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2drawDll)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &e2drawDll);
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
                    AFX_ASSERT(sys->avx.dsys);
                }
            }
#endif
            if (err)
            {
                AfxDisposeObjects(1, &sys->avx.e2drawDll);
                sys->avx.e2drawDll = NIL;
            }
        }
    }

    if (!(sys->amx.disabled))
    {
        //AfxDbgLogf(6, NIL, "Doing the AMX start up...");

        AfxDeployChain(&sys->amx.icdChain, sys);

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2combo", 0);
        afxModule e2simDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2simDll)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &e2simDll);
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
                    AFX_ASSERT(sys->amx.msys);
                }
            }
#endif
            if (err)
            {
                AfxDisposeObjects(1, &sys->amx.e2simDll);
                sys->amx.e2simDll = NIL;
            }
        }
    }

    if (!(sys->asx.disabled))
    {
        //AfxDbgLogf(6, NIL, "Doing the ASX start up...");
        
        AfxDeployChain(&sys->asx.icdChain, sys);

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2sound", 0);
        afxModule e2soundDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2soundDll)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &e2soundDll);
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
                    AFX_ASSERT(sys->asx.ssys);
                }
            }
#endif
            if (err)
            {
                AfxDisposeObjects(1, &sys->asx.e2soundDll);
                sys->asx.e2soundDll = NIL;
            }
        }
    }

    if (!(sys->aux.disabled))
    {
        //AfxDbgLogf(6, NIL, "Doing the AUX start up...");

        AfxDeployChain(&sys->aux.icdChain, sys);

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2mmux", 0);
        afxModule e2mmuxDll = NIL;

        if (AfxLoadModule(&uri, AFX_BIT(8), &e2mmuxDll)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &e2mmuxDll);
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
                    AFX_ASSERT(sys->aux.usys);
                }
            }
#endif
            if (err)
            {
                AfxDisposeObjects(1, &sys->aux.e2mmuxDll);
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
            AfxDbgLogf(6, NIL, "Booting...\n");

            afxManifest ini;
            AfxDeployManifest(&ini);
            // platform-dependent ctor will load the correct ini file.

            AFX_ASSERT(TheSystem == sys);
            AfxZero(TheSystem, sizeof(afxObjectBase));
            //TheSystem = malloc(sizeof(afxObjectBase) + sizeof(TheSystem[0]));

            afxClass* cls = _AfxGetSysMgr();
            AFX_ASSERT_CLASS(cls, afxFcc_SYS);

            if (_AfxConstructObjects(cls, 1, (void**)&TheSystem, (void*[]) { &ini, (void*)config })) AfxThrowError();
            else
            {
                AFX_ASSERT(TheSystem != sys); // Attention! Ctor moves the object pointer to hide out the object base.
                sys = TheSystem;
                AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

                AfxLogY("Memory page Size: %d", sys->memPageSiz);
                AfxLogY("HW threading: %d", sys->hwThreadingCap);

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

                    if (_AfxDestructObjects(cls, 1, (void*[]) { sys }))
                        AfxThrowError();
                }
            }

            AfxDismantleManifest(&ini);
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
        AfxDeregisterChainedClasses(&orphanClassChain);

        if (_AfxDoSubsystemShutdown(sys))
            AfxThrowError();

        afxClass* cls = _AfxGetSysMgr();
        AFX_ASSERT_CLASS(cls, afxFcc_SYS);

        AFX_ASSERT(TheSystem == sys);

        if (_AfxDestructObjects(cls, 1, (void**)&TheSystem))
            AfxThrowError();

        AFX_ASSERT(TheSystem != sys); // Attention! Dtor moves the object pointer to expose the object hdr.
        AfxZero(TheSystem, sizeof(afxObjectBase));
        AfxDismountClass(cls);
    }
#ifndef _AFX_DISABLE_DEBUGGER
    _AfxDbgDetach();
#endif
    AfxReleaseConsole();
}
