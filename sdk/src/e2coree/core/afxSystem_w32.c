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
#define _AFX_MODULE_C
#define _AFX_ICD_C
#define _AFX_FILE_C
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
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sound/afxSoundSystem.h"

extern afxSystem theSys;
extern afxClassConfig const _AfxSysClsConfig;

extern afxNat mainThreadId;
extern afxResult _AfxDoSystemThreading(afxSystem sys, afxTime timeout);
extern afxError _AfxSysLoadIcd(afxSystem sys, afxUri const* file, afxIcd *icd);

extern afxClassConfig const _AfxMmuClsConfig;
extern afxClassConfig const _AfxMdleClsConfig;
extern afxClassConfig const _AfxThrClsConfig;
extern afxClassConfig const _AfxTxuClsConfig;
extern afxClassConfig const _AfxIosClsConfig;
extern afxClassConfig const _AfxCdcClsConfig;
extern afxClassConfig const _AfxFileClsConfig;
extern afxClassConfig const _AfxArcClsConfig;
extern afxClassConfig const _AfxAppClsConfig;
extern afxClassConfig const _AfxKbdClsConfig;
extern afxClassConfig const _AfxMseClsConfig;
extern afxClassConfig const _AfxSimClsConfig;
extern afxClassConfig const _AfxSvcClsConfig;
extern afxClassConfig const _AfxIcdClsConfig;
extern afxClassConfig const _AfxDevClsConfig;
extern afxClassConfig const _AfxStoClsConfig;
extern afxClassConfig const _AfxHidClsConfig;
extern afxClassConfig const _AfxStrcClsConfig;

extern afxError _AfxDsysCtor(afxDrawSystem dsys, afxSystem sys, afxDrawSystemConfig const* config);
extern afxError _AfxDsysDtor(afxDrawSystem dsys);
extern afxError _AfxSsysCtor(afxSoundSystem ssys, afxSystem sys, afxSoundSystemConfig const* config);
extern afxError _AfxSsysDtor(afxSoundSystem ssys);

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
                    AfxAssist("User input requested support.");
                    AfxAssist("Get help at https://sigmaco.org/");
                    system("start https://sigmaco.org");
                }
                else if (AfxKeyWasPressed(0, AFX_KEY_F4))
                {
                    //AfxRequestThreadInterruption(thr);
                    AfxAdvertise("User input requested application break.");
                    AfxRequestSystemShutdown(0);
                }
                else if (AfxKeyWasPressed(0, AFX_KEY_F5))
                {
                    AfxAdvertise("User input requested application reboot.");
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

    if (!(sys = AfxGetSystem())) AfxThrowError();
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
    AfxLogMessageFormatted(0xFF000000, "\n %s:%i\n\t     %s", file, line, exp);
}

_AFX afxError _AfxSysScanForIcds(afxSystem sys, afxUri const* fileMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    HANDLE fh;
    WIN32_FIND_DATAA wfd;
    afxFixedUri2048 pathBuf;
    AfxMakeFixedUri2048(&pathBuf, NIL);
    AfxResolveUri(afxFileFlag_RX, fileMask, &pathBuf.uri);
    if ((fh = FindFirstFileA(AfxGetUriData(&pathBuf.uri, 0), &(wfd))))
    {
        do
        {
            afxUri uri, file;
            AfxMakeUri(&uri, wfd.cFileName, 0);
            AfxGetUriObject(&file, &uri);
            afxIcd icd;

            if (!(icd = AfxFindIcd(&file)))
            {
                _AfxSysLoadIcd(sys, &file, &icd);
            }
        } while (FindNextFileA(fh, &wfd));
        FindClose(fh);
    }
    return err;
}

_AFX afxError _AfxSysMountDefaultFileStorages(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;

    afxUri tmpUriMaps[8];
    AfxMakeUri(&tmpUriMaps[0], ".", 0);
    AfxMakeUri(&tmpUriMaps[1], "system", 0);
    AfxMakeUri(&tmpUriMaps[2], "system", 0);
    AfxMakeUri(&tmpUriMaps[3], "code", 0);
    AfxMakeUri(&tmpUriMaps[4], "sound", 0);
    AfxMakeUri(&tmpUriMaps[5], "data", 0);
    AfxMakeUri(&tmpUriMaps[6], "art", 0);
    AfxMakeUri(&tmpUriMaps[7], "tmp", 0);

    afxString tmpHostPaths[8];
#ifdef AFX_PLATFORM_WIN
    AfxMakeString(&tmpHostPaths[0], "", 0);
    AfxMakeString(&tmpHostPaths[1], "system", 0);
#   ifdef AFX_PLATFORM_W64
    AfxMakeString(&tmpHostPaths[2], "system\\w64", 0);
#   elif defined(AFX_PLATFORM_W32)
    AfxMakeString(&tmpHostPaths[2], "system\\w32", 0);
#   endif
    AfxMakeString(&tmpHostPaths[3], "code", 0);
    AfxMakeString(&tmpHostPaths[4], "sound", 0);
    AfxMakeString(&tmpHostPaths[5], "data", 0);
    AfxMakeString(&tmpHostPaths[6], "art", 0);
    AfxMakeString(&tmpHostPaths[7], "tmp", 0);
#else
    AfxMakeString(&tmpHostPaths[0], "", 0);
    AfxMakeString(&tmpHostPaths[1], "system", 0);
    AfxMakeString(&tmpHostPaths[2], "system/unix", 0);
    AfxMakeString(&tmpHostPaths[3], "code", 0);
    AfxMakeString(&tmpHostPaths[4], "sound", 0);
    AfxMakeString(&tmpHostPaths[5], "data", 0);
    AfxMakeString(&tmpHostPaths[6], "art", 0);
    AfxMakeString(&tmpHostPaths[7], "tmp", 0);
#endif

    afxStoragePointSpecification mnptSpecs[] =
    {
        { &tmpUriMaps[0], &tmpHostPaths[0], afxIoFlag_RX, FALSE },
        { &tmpUriMaps[1], &tmpHostPaths[1], afxIoFlag_RX, FALSE },
        { &tmpUriMaps[2], &tmpHostPaths[2], afxIoFlag_RX, FALSE },
        { &tmpUriMaps[3], &tmpHostPaths[3], afxIoFlag_RX, FALSE },
        { &tmpUriMaps[4], &tmpHostPaths[4], afxIoFlag_RX, FALSE },
        { &tmpUriMaps[5], &tmpHostPaths[5], afxIoFlag_RX, FALSE },
        { &tmpUriMaps[6], &tmpHostPaths[6], afxIoFlag_RX, FALSE },
        { &tmpUriMaps[7], &tmpHostPaths[7], afxIoFlag_RWX, FALSE },
    };

    afxNat mpCnt = AFX_COUNTOF(mnptSpecs);
    AfxAssert(AFX_COUNTOF(sys->defStops) >= mpCnt);

    if (AfxMountStoragePoints(mpCnt, mnptSpecs, sys->defStops)) AfxThrowError();
    else
    {

    }

    return err;
}

_AFX afxError _AfxSysCtor(afxSystem sys, afxCookie const* cookie)
{
    AfxEntry("sys=%p", sys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    afxSystemConfig const *cfg = ((afxSystemConfig const *)cookie->udd[0]) + cookie->no;

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
    sys->unitsToMeter = defCfg.unitsToMeter;
    sys->ioBufSiz = defCfg.ioBufSiz;
    sys->hwConcurrencyCap = defCfg.maxHwThreads;
    sys->memPageSize = defCfg.memPageSiz;
    sys->allocGranularity = defCfg.allocGranularity;
    sys->profilerPopTimer = defCfg.profilerPopTimer;
    sys->profilerPushTimer = defCfg.profilerPushTimer;
    sys->profilerPostMarker = defCfg.profilerPostMarker;

    sys->ptrSiz = sizeof(void*);
    sys->nonLe = FALSE;

    afxNat handlePageSiz = defCfg.handlePageSiz;

    if (cfg)
    {
        if (cfg->memPageSiz)
            sys->memPageSize = cfg->memPageSiz;

        if (cfg->allocGranularity)
            sys->allocGranularity = cfg->allocGranularity;

        if (cfg->maxHwThreads)
            sys->hwConcurrencyCap = cfg->maxHwThreads;

        if (cfg->assertHook)
            sys->assertHook = cfg->assertHook;

        if (cfg->unitsToMeter)
            sys->unitsToMeter = cfg->unitsToMeter;

        if (cfg->ioBufSiz)
            sys->ioBufSiz = cfg->ioBufSiz;

        if (cfg->handlePageSiz)
            handlePageSiz = cfg->handlePageSiz;

        if (cfg->profilerPopTimer)
            sys->profilerPopTimer = cfg->profilerPopTimer;

        if (cfg->profilerPushTimer)
            sys->profilerPushTimer = cfg->profilerPushTimer;

        if (cfg->profilerPostMarker)
            sys->profilerPostMarker = cfg->profilerPostMarker;
    }

    afxChain* classes = &sys->classes;
    AfxTakeChain(classes, &sys);
    afxClassConfig clsCfg;

    sys->dsys = NIL;
    sys->ssys = NIL;

    sys->hwConcurrencyCap = 1; // DBG

    clsCfg = _AfxTxuClsConfig;
    clsCfg.unitsPerPage = sys->hwConcurrencyCap;
    clsCfg.maxCnt = sys->hwConcurrencyCap;
    AfxMountClass(&sys->txus, NIL, classes, &clsCfg);

    theSys = sys; // evil trick to get this working at bootstrap

    afxTxu txu[32];
    AfxAcquireObjects(&sys->txus, sys->hwConcurrencyCap, (void**)txu, (void const*[]) { sys });

    if (!err)
    {
        afxMmu mmu = NIL;
        afxClassConfig clsCfg;

        AfxMountClass(&sys->mmus, NIL, classes, &_AfxMmuClsConfig);
        AfxMountClass(&sys->threads, NIL, classes, &_AfxThrClsConfig);
        clsCfg = _AfxDevClsConfig;
        clsCfg.mmu = mmu;
        AfxMountClass(&sys->devices, NIL, classes, &clsCfg);
        AfxMountClass(&sys->services, NIL, classes, &_AfxSvcClsConfig);
        AfxMountClass(&sys->files, NIL, classes, &_AfxFileClsConfig);
        AfxMountClass(&sys->streams, NIL, classes, &_AfxIosClsConfig);
        AfxMountClass(&sys->codecs, NIL, classes, &_AfxCdcClsConfig);
        AfxMountClass(&sys->archives, AfxGetFileClass(), classes, &_AfxArcClsConfig);
        AfxMountClass(&sys->modules, NIL, classes, &_AfxMdleClsConfig);
        AfxMountClass(&sys->icds, AfxGetModuleClass(), classes, &_AfxIcdClsConfig);
        clsCfg = _AfxStoClsConfig;
        clsCfg.mmu = mmu;
        AfxMountClass(&sys->storages, NIL, classes, &clsCfg);
        AfxMountClass(&sys->applications, NIL, classes, &_AfxAppClsConfig);
        AfxMountClass(&sys->hids, AfxGetDeviceClass(), classes, &_AfxHidClsConfig);
        AfxMountClass(&sys->keyboards, AfxGetHidClass(), classes, &_AfxKbdClsConfig);
        AfxMountClass(&sys->mouses, AfxGetHidClass(), classes, &_AfxMseClsConfig);
        AfxMountClass(&sys->strcats, NIL, classes, &_AfxStrcClsConfig);
        AfxMountClass(&sys->simulations, NIL, classes, &_AfxSimClsConfig);

        AfxAdvertise("Memory page size: %d", sys->memPageSize);
        AfxAdvertise("Logical processor count: %d", sys->hwConcurrencyCap);

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
                afxUri uri;
                afxChar buf[2048], buf2[2048];
                GetModuleFileNameA(NIL, buf, sizeof(buf));
                PathRemoveFileSpecA(buf);
                AfxStrcat(buf, "\\");
                AfxMakeUri(&uri, buf, 0);
                AfxCloneUri(&sys->pwd, &uri);
                GetModuleFileNameA(GetModuleHandleA("e2coree.dll"), buf, sizeof(buf));
                PathRemoveFileSpecA(buf);
                AfxStrcat(buf, "\\..\\..\\");
                PathCanonicalizeA(buf2, buf);
                AfxMakeUri(&uri, buf2, 0);
                AfxCloneUri(&sys->qwd, &uri);
                SetCurrentDirectoryA(AfxGetUriData(&sys->qwd, 0));

                if (_AfxSysMountDefaultFileStorages(sys)) AfxThrowError();
                else
                {
                    afxFixedUri128 uri2;
                    AfxMakeFixedUri128(&uri2, NIL);
                    AfxFormatUri(&uri2.uri, "system/e2coree.dll");

                    if (AfxMountLoadableModules(1, &uri2.uri, &sys->e2coree)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &sys->e2coree, afxFcc_MDLE);

                        afxNat mainKbdPort = { 0 };

                        if (AfxAcquireKeyboards(1, &mainKbdPort, &sys->stdKbd)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &sys->stdKbd, afxFcc_KBD);

                            if (cfg->sound)
                            {
                                if (!(sys->ssys = AfxAllocate(sys->mmu, 1, sizeof(*sys->ssys), NIL, AfxHint()))) AfxThrowError();
                                else
                                {
                                    if (_AfxSsysCtor(sys->ssys, sys, cfg->sound))
                                        AfxThrowError();

                                    if (err)
                                        AfxDeallocate(sys->mmu, sys->ssys);
                                }
                            }

                            if (!err)
                            {
                                if (cfg->draw)
                                {
                                    if (!(sys->dsys = AfxAllocate(sys->mmu, 1, sizeof(*sys->dsys), NIL, AfxHint()))) AfxThrowError();
                                    else
                                    {
                                        if (_AfxDsysCtor(sys->dsys, sys, cfg->draw))
                                            AfxThrowError();

                                        if (err)
                                            AfxDeallocate(sys->mmu, sys->dsys);
                                    }
                                }

                                if (!err)
                                {
                                    afxUri uri;
                                    AfxMakeUri(&uri, "system/*.icd", 0);
                                    _AfxSysScanForIcds(sys, &uri);
                                }

                                sys->operating = TRUE;
                                sys->interruptionRequested = FALSE;
                            }

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
                AfxDeallocateUri(&sys->pwd);

                AfxReleaseObjects(1, (void*[]) { sys->mmu });
            }
        }
    }

    if (err)
        theSys = NIL; // undo our hack in failure cause

    return err;
}

_AFX afxError _AfxSysDtor(afxSystem sys)
{
    AfxEntry("sys=%p", sys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    for (afxNat i = 0; i < sys->hwConcurrencyCap * 2; i++)
        AfxYieldThreading();

    _AfxShutdownOrphanClasses();

    if (sys->dsys)
    {
        _AfxDsysDtor(sys->dsys);
        AfxDeallocate(NIL, sys->dsys);
    }

    if (sys->ssys)
    {
        //_AfxSsysDtor(sys->ssys);
        AfxDeallocate(NIL, sys->ssys);
    }

    // will be released at class drop.
    //AfxReleaseObject(&(sys->stdKbd->hid.obj));
    //AfxReleaseObject(&(sys->e2coree->obj));
    //AfxReleaseObject(&(sys->mmu->obj));

    AfxDeallocateUri(&sys->pwd);

    _AfxUninstallChainedClasses(&sys->classes);
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
    AfxZero(1, sizeof(*cfg), cfg);

#ifdef AFX_PLATFORM_W32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    cfg->maxHwThreads = si.dwNumberOfProcessors;
    cfg->memPageSiz = si.dwPageSize;
    cfg->allocGranularity = si.dwAllocationGranularity;
#else
    cfg->maxHwThreads = 1;
    cfg->memPageSiz = 4096;
    cfg->allocGranularity = 4096;
#endif
    cfg->genrlArenaSpace = cfg->memPageSiz;

    cfg->assertHook = AfxStdAssertHookCallback;
    cfg->unitsToMeter = 1;
    cfg->ioBufSiz = BUFSIZ;
    cfg->ioArenaSpace = cfg->memPageSiz;

    cfg->handlePageSiz = (cfg->memPageSiz * AFX_BYTE_SIZE) / 32; // ((cfg->memPageSiz / 4) * 32) / 32;

    cfg->root = NIL;
    cfg->profilerPopTimer = NIL;
    cfg->profilerPushTimer = NIL;
    cfg->profilerPostMarker = NIL;

    if (platform)
    {
#ifdef AFX_PLATFORM_W32
        AfxZero(1, sizeof(*platform), platform);
        platform->instance = GetModuleHandle(NULL);
        platform->hWnd = GetActiveWindow();
#endif
    }
}
