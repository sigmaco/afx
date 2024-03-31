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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1

#include <sys/stat.h>
#include <stdio.h>

#if (defined(_WIN32) || defined(_WIN64))
#   include <Windows.h>
#   include <Shlwapi.h>
#   include <combaseapi.h>
#   include <process.h>
#   include <sys/timeb.h>
#else
#   include <unistd.h>
#endif

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_THREAD_C
#define _AFX_HID_C
#define _AFX_MOUSE_C
#define _AFX_KEYBOARD_C
#define _AFX_EXECUTABLE_C
#define _AFX_ICD_C
#define _AFX_FILE_C
#define _AFX_FILE_SYSTEM_C
#define _AFX_STREAM_C
#define _AFX_CODEC_C
#define _AFX_ARCHIVE_C
#define _AFX_APPLICATION_C
#define _AFX_MMU_C
#define _AFX_DEVICE_C
#define _AFX_DRAW_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_TXU_C
#define _AFX_SOUND_C
#define _AFX_SOUND_SYSTEM_C
#define _AFX_STRING_CATALOG_C
#define _AFX_CONTROLLER_C
#include "qwadro/core/afxSystem.h"

_AFXEXPORT afxReal const defRenderWareUnitsPerMeter = 1.f;

extern afxSystem theSys;
extern afxClassConfig const _AfxSysClsConfig;

extern afxNat mainThreadId;
extern afxResult _AfxDoSystemThreading(afxSystem sys, afxTime timeout);

extern afxClassConfig const _AfxMmuClsConfig;
extern afxClassConfig const _AfxMdleClsConfig;
extern afxClassConfig const _AfxThrClsConfig;
extern afxClassConfig const _AfxTxuClsConfig;
extern afxClassConfig const _AfxIosClsConfig;
extern afxClassConfig const _AfxCdcClsConfig;
//extern afxClassConfig const _AfxFileClsConfig;
extern afxClassConfig const _AfxArcClsConfig;
extern afxClassConfig const _AfxAppClsConfig;
extern afxClassConfig const _AfxKbdClsConfig;
extern afxClassConfig const _AfxMseClsConfig;
extern afxClassConfig const _AfxSimClsConfig;
extern afxClassConfig const _AfxSvcClsConfig;
extern afxClassConfig const _AfxIcdClsConfig;
extern afxClassConfig const _AfxDevClsConfig;
extern afxClassConfig const _AfxFsysClsConfig;
extern afxClassConfig const _AfxHidClsConfig;
extern afxClassConfig const _AfxCtrlClsConfig;
extern afxClassConfig const _AfxStrbClsConfig;
extern afxClassConfig const _AfxEnvClsConfig;

_AFX void Start_us_count(afxNat64* out_count)
{
    QueryPerformanceCounter((LARGE_INTEGER*)out_count);
}

_AFX afxNat32 Delta_us_count(afxNat64* last_count)
{
    static afxNat64 frequency = 1000;
    static afxInt32 got_frequency = 0;

    if (!got_frequency)
    {
        got_frequency = 1;
        QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    }
    afxNat64 start = *last_count;
    QueryPerformanceCounter((LARGE_INTEGER*)last_count);
    return((afxNat32)(((*last_count - start) * (afxNat64)1000000) / frequency));
}

#define Start_timer() { afxNat64 __timer; Start_us_count( &__timer );
#define End_and_start_next_timer( count ) count += Delta_us_count( &__timer );
#define End_timer( count ) End_and_start_next_timer( count ) }

// READ ONLY METHODS //////////////////////////////////////////////////////////

AFX afxResult _AfxProcessSystemInputMessageWin32(MSG* msg); // from afxHid.c

_AFX afxResult _AfxProcessHostPlatformEvents(void)
{
    afxError err = AFX_ERR_NONE;

    MSG msg;
    //afxEvent ev;
    afxResult msgCnt = 0;
    
    while (PeekMessage(&(msg), NIL, 0, 0, PM_REMOVE/* | PM_NOYIELD*/))
    {
        ++msgCnt;

        if (msg.message == WM_QUIT) // PostQuitMessage()
        {
            AfxRequestSystemShutdown(0);
        }
        else
        {
            if (msg.message == WM_INPUT || msg.message == WM_INPUT_DEVICE_CHANGE)
            {
                _AfxProcessSystemInputMessageWin32(&msg);

                if (AfxKeyWasPressed(0, AFX_KEY_F1))
                {
                    AfxLogAssistence("User input requested support.");
                    AfxLogAssistence("Get help at https://sigmaco.org/");
                    system("start https://sigmaco.org");
                }
                else if (AfxKeyWasPressed(0, AFX_KEY_F4))
                {
                    //AfxRequestThreadInterruption(thr);
                    AfxLogAdvertence("User input requested application break.");
                    AfxRequestSystemShutdown(0);
                }
                else if (AfxKeyWasPressed(0, AFX_KEY_F5))
                {
                    AfxLogAdvertence("User input requested application reboot.");
                    //_AfxInterruptionAllApplications();
                    //opcode = AFX_OPCODE_REBOOT;
                }
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msgCnt;
}

_AFX afxResult AfxDoSystemExecution(afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    afxResult code = 0;
    afxSystem sys;
    (void)timeout;

    if (!AfxGetSystem(&sys)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sys, afxFcc_SYS);

        if (AfxSystemIsExecuting())
        {
            afxResult msgCnt = _AfxProcessHostPlatformEvents();
            _AfxDoSystemThreading(sys, timeout);
        }

        if (sys->isInShutdown)
            code = sys->exitCode;
    }
    return code;
}

_AFX void AfxStdAssertHookCallback(char const* exp, char const* file, int line)
{
    assert(exp);
    assert(file);
    assert(line);
    AfxDbgLogf(8, NIL, "\n %s:%i\n\t     %s", file, line, exp);
}

_AFX afxError _AfxSysMountDefaultFileStorages(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;

    afxUri point, location;
    AfxMakeUri(&point, ".", 0);
    AfxMakeUri(&location, "", 0);

    if (AfxMountStorageUnit(&point, &location, afxFileFlag_RX)) AfxThrowError();
    else
    {
        AfxMakeUri(&point, "system", 0);
        AfxMakeUri(&location, "system", 0);

        if (AfxMountStorageUnit(&point, &location, afxFileFlag_RX)) AfxThrowError();
        else
        {
#ifdef AFX_PLATFORM_64
#   ifdef AFX_PLATFORM_WIN
            AfxMakeUri(&location, "system64", 0);

            if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, "system64d", 0);

                if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
#   elif defined(AFX_PLATFORM_LINUX)
            AfxMakeUri(&location, "system64x", 0);

            if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, "system64xd", 0);

                if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
#   endif
#elif defined(AFX_PLATFORM_32)
#   ifdef AFX_PLATFORM_WIN
            AfxMakeUri(&location, "system32", 0);

            if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, "system32d", 0);

                if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
#   elif defined(AFX_PLATFORM_LINUX)
            AfxMakeUri(&location, "systemx", 0);

            if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, "system32xd", 0);

                if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
#   endif
#endif
        }
    }
#if 0
    if (!err)
    {
        AfxMakeUri(&point, "code", 0);
        AfxMakeUri(&location, "code", 0);

        if (AfxMountStorageUnit(&point, &location, afxFileFlag_RX)) AfxThrowError();
        else
        {
            AfxMakeUri(&point, "sound", 0);
            AfxMakeUri(&location, "sound", 0);

            if (AfxMountStorageUnit(&point, &location, afxFileFlag_RX)) AfxThrowError();
            else
            {
                AfxMakeUri(&point, "data", 0);
                AfxMakeUri(&location, "data", 0);

                if (AfxMountStorageUnit(&point, &location, afxFileFlag_RX)) AfxThrowError();
                else
                {
                    AfxMakeUri(&point, "art", 0);
                    AfxMakeUri(&location, "art", 0);

                    if (AfxMountStorageUnit(&point, &location, afxFileFlag_RX)) AfxThrowError();
                    else
                    {
                        AfxMakeUri(&point, "tmp", 0);
                        AfxMakeUri(&location, "tmp", 0);

                        if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX))
                            AfxThrowError();
                    }
                }
            }
        }
    }
#endif
    return err;
}

_AFX afxError _AfxSysCtor(afxSystem sys, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    AfxZero2(1, sizeof(AFX_OBJECT(afxSystem)), sys);

    afxManifest* ini = cookie->udd[0];
    afxSystemConfig const *cfg = cookie->udd[1];

    // setting the process working directory and the Qwadro/system working directory...
    afxUri uri;
    afxChar buf[2048];
    GetModuleFileNameA(NIL, buf, sizeof(buf));
    AfxStrrchr(buf, '\\')[1] = '\0';
    AfxMakeUri(&uri, buf, 0);
    AfxMakeUri2048(&sys->pwd, &uri);
    AfxCanonicalizePath(&sys->pwd.uri, TRUE);
    GetModuleFileNameA(GetModuleHandleA("e2coree.dll"), buf, sizeof(buf));
    AfxStrrchr(buf, '\\')[1] = '\0';
    AfxStrcat(buf, "..\\");
    AfxMakeUri(&uri, buf, 0);
    AfxMakeUri2048(&sys->qwd, &uri);
    AfxCanonicalizePath(&sys->qwd.uri, TRUE);
    SetCurrentDirectoryA(AfxGetUriData(&sys->pwd.uri, 0));


#ifdef AFX_PLATFORM_W32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
#endif

    AfxMakeUri(&uri, "system.ini", 0);

    if (AfxIniLoadFromFile(ini, &uri))
    {
        afxString2048 tbs;
        AfxMakeString2048(&tbs, AfxGetUriString(&sys->pwd.uri));
        AfxConcatenateStringL(&tbs.str, "system.ini", 0);

        AfxUriFromString(&uri, &tbs.str.str);
        AfxIniLoadFromFile(ini, &uri);
    }
    
    afxString issSystem = AFX_STRING("System");
    AfxIniGetNat(ini, &issSystem, &AfxString("nHardwareThreadingCapacity"), &sys->hwConcurrencyCap);
    AfxIniGetNat(ini, &issSystem, &AfxString("nMemoryPageSize"), &sys->memPageSize);
    AfxIniGetNat(ini, &issSystem, &AfxString("nAllocationGranularity"), &sys->allocGranularity);
    AfxIniGetNat(ini, &issSystem, &AfxString("nIoBufferSize"), &sys->ioBufSiz);
    AfxIniGetReal(ini, &issSystem, &AfxString("fUnitsPerMeter"), &sys->unitsPerMeter);
    
    if (0 == sys->hwConcurrencyCap)
    {
        sys->hwConcurrencyCap =
#ifdef AFX_PLATFORM_W32
            si.dwNumberOfProcessors;
#else
            1;
#endif
    }

    if (0 == sys->memPageSize)
    {
        sys->memPageSize =
#ifdef AFX_PLATFORM_W32
            si.dwPageSize;
#else
            4096;
#endif
    }

    if (0 == sys->allocGranularity)
    {
        sys->allocGranularity =
#ifdef AFX_PLATFORM_W32
            si.dwAllocationGranularity;
#else
            4096;
#endif
    }

    if (0 == sys->ioBufSiz)
    {
        sys->ioBufSiz = BUFSIZ;
    }


    if (0.f == sys->unitsPerMeter)
    {
        sys->unitsPerMeter = defRenderWareUnitsPerMeter;
    }

    mainThreadId;
    AfxGetThreadingId(&mainThreadId);
    
    sys->isInShutdown = FALSE;
    sys->isInBootUp = FALSE;
    sys->operating = FALSE;
    sys->interruptionRequested = FALSE;

    afxSystemConfig defCfg;
    afxPlatformConfig defPlatCfg;
    AfxChooseSystemConfiguration(&defCfg, &defPlatCfg);

    sys->assertHook = defCfg.assertHook;
    sys->profilerPopTimer = defCfg.profilerPopTimer;
    sys->profilerPushTimer = defCfg.profilerPushTimer;
    sys->profilerPostMarker = defCfg.profilerPostMarker;

    sys->ptrSiz = sizeof(void*);
    sys->nonLe = FALSE;

    if (cfg)
    {
        if (cfg->assertHook)
            sys->assertHook = cfg->assertHook;

        if (cfg->profilerPopTimer)
            sys->profilerPopTimer = cfg->profilerPopTimer;

        if (cfg->profilerPushTimer)
            sys->profilerPushTimer = cfg->profilerPushTimer;

        if (cfg->profilerPostMarker)
            sys->profilerPostMarker = cfg->profilerPostMarker;
    }

    afxChain* classes = &sys->classes;
    AfxSetUpChain(classes, &sys);
    afxClassConfig clsCfg;

    sys->dsys = NIL;
    sys->ssys = NIL;

    sys->hwConcurrencyCap = 1; // DBG

    clsCfg = _AfxTxuClsConfig;
    clsCfg.unitsPerPage = sys->hwConcurrencyCap;
    clsCfg.maxCnt = sys->hwConcurrencyCap;
    AfxSetUpManager(&sys->txuCls, NIL, classes, &clsCfg);

    theSys = sys; // evil trick to get this working at bootstrap

    afxTxu txu[32];
    AfxAcquireObjects(&sys->txuCls, sys->hwConcurrencyCap, (void**)txu, (void const*[]) { sys });

    if (!err)
    {
        afxMmu mmu = NIL;
        afxClassConfig clsCfg;

        AfxSetUpManager(&sys->mmuCls, NIL, classes, &_AfxMmuClsConfig);
        
        AfxSetUpManager(&sys->thrCls, NIL, classes, &_AfxThrClsConfig); // require txu

        AfxSetUpManager(&sys->svcCls, NIL, classes, &_AfxSvcClsConfig);
        //AfxSetUpManager(&sys->fileCls, NIL, classes, &_AfxFileClsConfig);
        AfxSetUpManager(&sys->cdcCls, NIL, classes, &_AfxCdcClsConfig);
        AfxSetUpManager(&sys->strbCls, NIL, classes, &_AfxStrbClsConfig);
        

        AfxSetUpManager(&sys->iosCls, NIL, classes, &_AfxIosClsConfig);
        AfxSetUpManager(&sys->archCls, NIL, classes, &_AfxArcClsConfig); // require iob
        AfxSetUpManager(&sys->fsysCls, NIL, classes, &_AfxFsysClsConfig); // require iob, arch


        AfxSetUpManager(&sys->exeCls, NIL, classes, &_AfxMdleClsConfig);
        AfxSetUpManager(&sys->devCls, NIL, classes, &_AfxDevClsConfig); // require mdle


        AfxSetUpManager(&sys->hidCls, /*AfxGetDeviceClass()*/NIL, classes, &_AfxHidClsConfig);
        AfxSetUpManager(&sys->kbdCls, AfxGetHidClass(), classes, &_AfxKbdClsConfig); // require hid
        AfxSetUpManager(&sys->mseCls, AfxGetHidClass(), classes, &_AfxMseClsConfig); // require hid
        AfxSetUpManager(&sys->ctrlCls, AfxGetHidClass(), classes, &_AfxCtrlClsConfig); // require hid
        
        AfxSetUpManager(&sys->envCls, NIL, classes, &_AfxEnvClsConfig); // require iob

        AfxLogY("Memory page size: %d", sys->memPageSize);
        AfxLogY("Logical processor count: %d", sys->hwConcurrencyCap);

        afxAllocationStrategy as = { 0 };
        as.size = sizeof(afxByte);
        as.align = AFX_SIMD_ALIGN;
        as.cap = 0;
        as.duration = AFX_ALL_DUR_TRANSIENT;
        as.resizable = TRUE;
        as.stock = sys->memPageSize * 8192; // 32MB

        if (AfxAcquireMmus(1, AfxHint(), &as, &sys->mmu)) AfxThrowError();
        else
        {
            if (AfxAllocateArena(sys->mmu, &sys->ioArena, NIL, AfxHint())) AfxThrowError();
            else
            {
                if (_AfxSysMountDefaultFileStorages(sys)) AfxThrowError();
                else
                {
                    afxUri128 uri2;
                    AfxMakeUri128(&uri2, NIL);
                    AfxFormatUri(&uri2.uri, "system/e2coree.dll");

                    if (AfxLoadModule(&uri2.uri, NIL, &sys->e2coree)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &sys->e2coree, afxFcc_MDLE);

                        if (AfxAcquireKeyboard(0, &sys->stdKbd)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &sys->stdKbd, afxFcc_KBD);

                            if (err)
                                AfxReleaseObjects(1, (void*[]) { sys->stdKbd });

                        }
                    }
                }

                if (err)
                    AfxReleaseObjects(1, (void*[]) { sys->e2coree });
            }

            if (err)
            {
                AfxDeallocateUri(&sys->pwd.uri);

                AfxReleaseObjects(1, (void*[]) { sys->mmu });
            }
        }

        if (err)
            _AfxUninstallChainedClasses(&sys->classes);
    }

    if (err)
        theSys = NIL; // undo our hack in failure cause

    return err;
}

_AFX afxError _AfxSysDtor(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    for (afxNat i = 0; i < sys->hwConcurrencyCap * 2; i++)
        AfxYieldThreading();

    _AfxShutdownOrphanClasses();

    // objects will be released at class drop.
    _AfxUninstallChainedClasses(&sys->classes);

    AfxDeallocateUri(&sys->pwd.uri);

    //AfxDeallocateArena(&sys->ioArena);
    //AfxReleaseObject(&(sys->memD->obj));

    //_AfxFreeExternalAllocations();
    theSys = NIL;
    return err;
}

_AFX void AfxChooseSystemConfiguration(afxSystemConfig* cfg, afxPlatformConfig* platform)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    AfxZero2(1, sizeof(*cfg), cfg);

    cfg->assertHook = AfxStdAssertHookCallback;
    
    cfg->root = NIL;
    cfg->profilerPopTimer = NIL;
    cfg->profilerPushTimer = NIL;
    cfg->profilerPostMarker = NIL;

    if (platform)
    {
#ifdef AFX_PLATFORM_W32
        AfxZero2(1, sizeof(*platform), platform);
        platform->instance = GetModuleHandle(NULL);
        platform->hWnd = GetActiveWindow();        
#endif
    }
}
