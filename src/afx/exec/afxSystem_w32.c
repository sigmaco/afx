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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1

#include <sys/stat.h>
#include <stdio.h>

#if (defined(_WIN32) || defined(_WIN64))
#   include <Windows.h>
#   include <Shlwapi.h>
//#   include <combaseapi.h>
#   include <process.h>
#   include <sys/timeb.h>
#else
#   include <unistd.h>
#endif

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#include "../src/afx/dev/afxDevCoreBase.h"

AFX_STATIC_ASSERT(sizeof(AFX_OBJECT(afxSystem)) > sizeof(void*), "");
_AFX afxBool sysReady = FALSE;
#if !0
_AFX afxByte theSysData[AFX_ALIGNED_SIZEOF(sizeof(afxObjectBase), 16) + AFX_ALIGNED_SIZEOF(sizeof(((afxSystem)0)[0]), 16)] = { 0 };
_AFX afxSystem TheSystem = (void*)&theSysData;
AFX_STATIC_ASSERT(sizeof(TheSystem[0]) > sizeof(void*), "");
AFX_STATIC_ASSERT(sizeof(theSysData) >= (sizeof(afxObjectBase) + sizeof(TheSystem[0])), "");
#else
_AFX afxSystem TheSystem = NIL;
#endif
extern afxReal64 const renderwareUnitsPerMeter;

extern afxClassConfig const _AfxMmuMgrCfg;
extern afxClassConfig const _AfxMdleMgrCfg;
extern afxClassConfig const _AfxThrMgrCfg;
extern afxClassConfig const _AfxCdcMgrCfg;
extern afxClassConfig const _AfxSimMgrCfg;
extern afxClassConfig const _AfxSvcMgrCfg;
extern afxClassConfig const _AfxDevMgrCfg;
extern afxClassConfig const _AfxFsysMgrCfg;
extern afxClassConfig const _AfxStrbMgrCfg;
extern afxClassConfig const _AfxShMgrCfg;

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

    SHORT escSt = 0, lastEscSt = 0;

    MSG msg;
    afxResult msgCnt = 0;
    while (PeekMessageA(&msg, NIL, 0, 0, PM_REMOVE | PM_NOYIELD))
    {
        ++msgCnt;

        if (msg.message == WM_QUIT) // PostQuitMessage()
        {
            AfxRequestShutdown(0);
        }
        else if (msg.message == WM_USER)
        {
            void(*event)(void* udd) = (void*)msg.wParam;

            if (event)
                event((void*)msg.lParam);
        }
        else
        {
            if (AfxIsPrimeThread())
            {
                lastEscSt = escSt;
                escSt = (GetKeyState(VK_ESCAPE) & 0x8000);

                if (GetKeyState(VK_F1))
                {
                    AfxLogAssistence("User input requested support.");
                    AfxLogAssistence("Get help at https://sigmaco.org/");
                    system("start https://sigmaco.org");
                }
                else if (GetKeyState(VK_F4))
                {
                    //AfxRequestThreadInterruption(thr);
                    AfxLogAdvertence("User input requested application break.");
                    AfxRequestShutdown(0);
                }
                else if (GetKeyState(VK_F5))
                {
                    AfxLogAdvertence("User input requested application reboot.");
                    //_AfxInterruptionAllApplications();
                    //opcode = AFX_OPCODE_REBOOT;
                }
                else if (lastEscSt && !(escSt))
                {
                    {
                        afxString cur;
                        AfxMakeString(&cur, 0, "\n$ ", 0);
                        afxString2048 buf;
                        AfxMakeString2048(&buf, NIL);
                        AfxPrompt(&cur, &buf.str);
                        AfxPrint(0, buf.buf);
                        AfxPrint(0, "\n");
                    }
                }
            }

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }
    return msgCnt;
}

_AFX afxResult PrimeThreadProc(afxThread thr, afxEvent* ev)
{
    return 0;
}

extern afxBool _AfxThrExecuteCb(afxThread thr, void* udd);

_AFX afxTime AfxDoThreading(afxTime timeout)
{
    afxError err = AFX_ERR_NONE;

    afxTime first, last, dt;
    AfxGetTime(&first);

    do
    {
        SHORT escSt = 0, lastEscSt = 0;

        MSG msg;
        afxResult msgCnt = 0;
        while (PeekMessageA(&msg, NIL, 0, 0, PM_REMOVE | PM_NOYIELD))
        {
            ++msgCnt;

            if (msg.message == WM_QUIT) // PostQuitMessage()
            {
                if (AfxIsPrimeThread())
                    AfxRequestShutdown(0);
            }
            else if (msg.message == WM_USER)
            {
                void(*event)(void* udd) = (void*)msg.wParam;

                if (event)
                    event((void*)msg.lParam);
            }
            else
            {
                if (AfxIsPrimeThread())
                {
                    afxSystem sys;
                    AfxGetSystem(&sys);
                    AfxAssertObjects(1, &sys, afxFcc_SYS);

                    if (sys->aux.ioctl)
                        sys->aux.ioctl(sys, sys->aux.e2uxDll, 0, &timeout);

                    if (sys->asx.ioctl)
                        sys->asx.ioctl(sys, sys->asx.e2soundDll, 0, &timeout);

                    if (sys->avx.ioctl)
                        sys->avx.ioctl(sys, sys->avx.e2drawDll, 0, &timeout);

                    lastEscSt = escSt;
                    escSt = (GetKeyState(VK_ESCAPE) & 0x8000);

                    if (GetKeyState(VK_F1))
                    {
                        AfxLogAssistence("User input requested support.");
                        AfxLogAssistence("Get help at https://sigmaco.org/");
                        system("start https://sigmaco.org");
                    }
                    else if (GetKeyState(VK_F4))
                    {
                        //AfxRequestThreadInterruption(thr);
                        AfxLogAdvertence("User input requested application break.");
                        AfxRequestShutdown(0);
                    }
                    else if (GetKeyState(VK_F5))
                    {
                        AfxLogAdvertence("User input requested application reboot.");
                        //_AfxInterruptionAllApplications();
                        //opcode = AFX_OPCODE_REBOOT;
                    }
                    else if (lastEscSt && !(escSt))
                    {
                        {
                            afxString cur;
                            AfxMakeString(&cur, 0, "\n$ ", 0);
                            afxString2048 buf;
                            AfxMakeString2048(&buf, NIL);
                            AfxPrompt(&cur, &buf.str);
                            AfxPrint(0, buf.buf);
                            AfxPrint(0, "\n");
                        }
                    }
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }
        break; // <------
    } while (timeout > (dt = (AfxGetTime(&last) - first)));
    return dt;
}

_AFX void AfxStdAssertHookCallback(char const* exp, char const* file, int line)
{
    assert(exp);
    assert(file);
    assert(line);
    AfxDbgLogf(8, NIL, "\n %s:%i\n\t     %s", file, line, exp);
}

_AFX afxError MountHostVolumes()
{
    afxError err = AFX_ERR_NONE;

    afxUri32 urib, urib2;
    AfxMakeUri32(&urib, NIL);
    AfxMakeUri32(&urib2, NIL);

    char buf[256];
    struct stat st;

    for (int i = 0; i < 26; i++)
    {
        sprintf(buf, "%c:\\", 'A' + i);

        if (0 == stat(buf, &st))
        {
            AfxFormatUri(&urib.uri, "%c", 'a' + i);
            AfxFormatUri(&urib2.uri, "%c:\\", 'A' + i);
            
            afxFileFlags ioFlags = NIL;

            if ((st.st_mode & _S_IREAD) == _S_IREAD)
                ioFlags |= afxFileFlag_R;

            if ((st.st_mode & _S_IWRITE) == _S_IWRITE)
                ioFlags |= afxFileFlag_W;

            if ((st.st_mode & _S_IEXEC) == _S_IEXEC)
                ioFlags |= afxFileFlag_X;

            AfxMountStorageUnit(&urib.uri, &urib2.uri, ioFlags);
        }
    }

    int a = 0;
}

_AFX afxError _AfxSysMountDefaultFileStorages(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    
    afxUri point, location;
    AfxMakeUri(&point, 0, "z", 0);
    AfxMakeUri(&location, 0, "", 0);

    if (AfxMountStorageUnit(&point, &location, afxFileFlag_RX)) AfxThrowError();
    else
    {
        AfxMakeUri(&point, 0, "z", 0);
        AfxMakeUri(&location, 0, "system", 0);

        if (AfxMountStorageUnit(&point, &location, afxFileFlag_RX)) AfxThrowError();
        else
        {
#ifdef AFX_ISA_X86_64
#   ifdef AFX_OS_WIN
            AfxMakeUri(&location, 0, "system64", 0);

            if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, 0, "system64d", 0);

                if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
#   elif defined(AFX_OS_LNX)
            AfxMakeUri(&location, 0, "system64x", 0);

            if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, 0, "system64xd", 0);

                if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
#   endif
#elif defined(AFX_ISA_X86_32)
#   ifdef AFX_OS_WIN
            AfxMakeUri(&location, 0, "system32", 0);

            if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, 0, "system32d", 0);

                if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
#   elif defined(AFX_OS_LNX)
            AfxMakeUri(&location, 0, "systemx", 0);

            if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, 0, "system32xd", 0);

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
    MountHostVolumes();
    return err;
}

_AFX afxError _AfxSysCtor(afxSystem sys, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    afxManifest* ini = cookie->udd[0];
    afxSystemConfig const *cfg = cookie->udd[1];

    //AfxZero(sys, sizeof(sys[0]));

    _AfxInitMmu(NIL);

    afxChain* classes = &sys->classes;
    AfxDeployChain(classes, sys);
    AfxRegisterClass(&sys->strbMgr, NIL, classes, &_AfxStrbMgrCfg);

    // setting the process working directory and the Qwadro/system working directory...
    afxUri uri;
    afxChar buf[2048];
    GetModuleFileNameA(NIL, buf, sizeof(buf));
    AfxStrrchr(buf, '\\')[1] = '\0';
    AfxMakeUri(&uri, 0, buf, 0);
    AfxMakeUri2048(&sys->pwd, &uri);
    AfxCanonicalizePath(&sys->pwd.uri, TRUE);
    GetModuleFileNameA(GetModuleHandleA("e2coree"), buf, sizeof(buf));    
    AfxStrrchr(buf, '\\')[1] = '\0';
    AfxStrcat(buf, "..\\");
    AfxMakeUri(&uri, 0, buf, 0);
    AfxMakeUri2048(&sys->qwd, &uri);
    AfxCanonicalizePath(&sys->qwd.uri, TRUE);
    SetCurrentDirectoryA(AfxGetUriData(&sys->pwd.uri, 0));

    sys->primeTid = AfxGetTid();
    AfxAssert(sys->primeTid);

    sys->maxMemUsage = cfg->maxMemUsage;
    sys->memPageSiz = cfg->memPageSiz;
    sys->allocGranularity = cfg->allocGranularity;
    sys->ioBufSiz = cfg->ioBufSiz;
    sys->unitsPerMeter = cfg->unitsToMeter;

    sys->assertHook = cfg->assertHook;
    sys->profilerPopTimer = cfg->profilerPopTimer;
    sys->profilerPushTimer = cfg->profilerPushTimer;
    sys->profilerPostMarker = cfg->profilerPostMarker;

    sys->ptrSiz = sizeof(void*);
    sys->bigEndian = FALSE;

    //sys->genrlArenaSpace = cfg->genrlArenaSpace;
    //sys->ioArenaSpace = cfg->ioArenaSpace;
    sys->hwThreadingCap = cfg->hwThreadingCap;
    sys->reallocatorFn = cfg->reallocatorFn;



    afxClassConfig clsCfg;

    clsCfg = _AfxThrMgrCfg;
    clsCfg.unitsPerPage = sys->hwThreadingCap;
    AfxRegisterClass(&sys->thrMgr, NIL, classes, &clsCfg); // require txu

    AfxRegisterClass(&sys->mmuMgr, NIL, classes, &_AfxMmuMgrCfg);

    AfxRegisterClass(&sys->svcMgr, NIL, classes, &_AfxSvcMgrCfg);
    //AfxRegisterClass(&sys->fileMgr, NIL, classes, &_AfxFileMgrConfig);
    AfxRegisterClass(&sys->cdcMgr, NIL, classes, &_AfxCdcMgrCfg);

    AfxRegisterClass(&sys->fsysMgr, NIL, classes, &_AfxFsysMgrCfg); // require iob, arch


    AfxRegisterClass(&sys->exeMgr, NIL, classes, &_AfxMdleMgrCfg);
    AfxRegisterClass(&sys->devMgr, NIL, classes, &_AfxDevMgrCfg); // require mdle

    afxThreadConfig thrCfg = { 0 };
    thrCfg.tid = (sys->primeTid = AfxGetTid());
    thrCfg.procCb = PrimeThreadProc;

    if (AfxAcquireThread(AfxHere(), &thrCfg, &sys->primeThr)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sys->primeThr, afxFcc_THR);
        //sys->primeThr = primeThr;

        if (_AfxSysMountDefaultFileStorages(sys)) AfxThrowError();
        else
        {
            afxUri128 urib;
            AfxMakeUri128(&urib, NIL);
            AfxFormatUri(&urib.uri, "e2coree");
            afxModule e2coree;

            if (AfxLoadModule(&urib.uri, NIL, &e2coree)) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &e2coree, afxFcc_MDLE);
                sys->e2coree = e2coree;

                if (err)
                    AfxReleaseObjects(1, &sys->e2coree);
            }
        }
    }

    if (err)
        AfxCleanUpChainedClasses(&sys->classes);

    return err;
}

_AFX afxError _AfxSysDtor(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    AfxReleaseObjects(1, &sys->e2coree);
    AfxReleaseObjects(1, &sys->primeThr);

    // objects will be released at class drop.
    AfxCleanUpChainedClasses(&sys->classes);

    //AfxDeallocateUri(&sys->pwd.uri);

    //AfxDismantleArena(&sys->ioArena);
    //AfxReleaseObject(&(sys->memD->obj));

    //_AfxFreeExternalAllocations();
    
    _AfxDeinitMmu(NIL);

    AfxZero(sys, sizeof(sys[0]));

    return err;
}

_AFX void AfxConfigureSystem(afxSystemConfig* config)
{
    afxError err = AFX_ERR_NONE;
    afxSystemConfig cfg = { 0 };

    afxUri uri;
    afxManifest ini;
    AfxSetUpIni(&ini);
    AfxMakeUri(&uri, 0, "system.ini", 0);

    if (AfxIniLoadFromFile(&ini, &uri))
    {
        afxString2048 tbs;
        AfxMakeString2048(&tbs, TheSystem ? AfxGetUriString(&TheSystem->pwd.uri) : NIL);
        AfxCatenateStringL(&tbs.str, "system.ini", 0);
        
        AfxWrapUriString(&uri, &tbs.str);
        AfxIniLoadFromFile(&ini, &uri);
    }

#ifdef AFX_OS_WIN
    SYSTEM_INFO si;
    GetSystemInfo(&si);
#endif

    afxString sSystem = AFX_STRING("System");

    if (!AfxIniGetNat(&ini, &sSystem, &AfxString("nIoBufferSize"), &cfg.ioBufSiz) || !cfg.ioBufSiz)
    {
        cfg.ioBufSiz = BUFSIZ;
    }

    if (!AfxIniGetReal(&ini, &sSystem, &AfxString("fUnitsPerMeter"), &cfg.unitsToMeter) || !cfg.unitsToMeter)
    {
        cfg.unitsToMeter = renderwareUnitsPerMeter;
    }

    if (!AfxIniGetNat(&ini, &sSystem, &AfxString("nHwThreadingCapacity"), &cfg.hwThreadingCap) || !cfg.hwThreadingCap)
    {
        cfg.hwThreadingCap =
#ifdef AFX_OS_WIN
            si.dwNumberOfProcessors;
#else
            1;
#endif
    }

    if (!AfxIniGetNat(&ini, &sSystem, &AfxString("nMemoryPageSize"), &cfg.memPageSiz) || !cfg.memPageSiz)
    {
        cfg.memPageSiz =
#ifdef AFX_OS_WIN
            si.dwPageSize;
#else
            4096;
#endif
    }

    if (!AfxIniGetNat(&ini, &sSystem, &AfxString("nAllocationGranularity"), &cfg.allocGranularity) || !cfg.allocGranularity)
    {
        cfg.allocGranularity =
#ifdef AFX_OS_WIN
            si.dwAllocationGranularity;
#else
            4096;
#endif
    }

    cfg.assertHook = AfxStdAssertHookCallback;
    
    //cfg.root = NIL;
    cfg.profilerPopTimer = NIL;
    cfg.profilerPushTimer = NIL;
    cfg.profilerPostMarker = NIL;

    //if (platform)
    {
#ifdef AFX_OS_WIN
        AfxZero(&cfg.platform, sizeof(cfg.platform));
        cfg.platform.hInst = GetModuleHandle(NULL);
        cfg.platform.hWnd = GetActiveWindow();
#endif
    }

    if (!(AfxIniGetBool(&ini, &AfxString("DrawSystem"), &AfxString("bDisabled"), &cfg.avxDisabled)))
    {
        cfg.avxDisabled = FALSE;
    }

    if (!(AfxIniGetBool(&ini, &AfxString("SoundSystem"), &AfxString("bDisabled"), &cfg.asxDisabled)))
    {
        cfg.asxDisabled = FALSE;
    }

    if (!(AfxIniGetBool(&ini, &AfxString("Shell"), &AfxString("bDisabled"), &cfg.auxDisabled)))
    {
        cfg.auxDisabled = FALSE;
    }

    AfxCleanUpIni(&ini);

    *config = cfg;
}
