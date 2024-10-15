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
#include "../dev/afxExecImplKit.h"

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
extern afxClassConfig const _AfxIcdClsCfg;
extern afxClassConfig const _AfxThrMgrCfg;
extern afxClassConfig const _AfxCdcMgrCfg;
extern afxClassConfig const _AfxSimMgrCfg;
extern afxClassConfig const _AfxSvcMgrCfg;
extern afxClassConfig const _AfxIosClsCfg;
extern afxClassConfig const _AfxFsysMgrCfg;
extern afxClassConfig const _AfxStrbMgrCfg;
extern afxClassConfig const _AfxShMgrCfg;
extern afxClassConfig const _AfxXexuStdImplementation;

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

_AFX afxResult PrimeThreadProc(afxThread thr, afxEvent* ev)
{
    return 0;
}

#if 0
_AFX afxBool SshProc(afxShell ssh, MSG const* msg)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ssh, afxFcc_SSH);

    AfxCallDevice((afxDevice)ssh, 3, msg);

    return TRUE;
}

_AFX afxTime AfxDoSystemThreading(afxTime timeout)
{
    afxError err = AFX_ERR_NONE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);

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
                if (!sys->aux.disabled)
                {
                    if (sys->aux.ioctl)
                        sys->aux.ioctl(sys, sys->aux.e2mmuxDll, 0, &timeout);

                    AfxInvokeClassInstances(&sys->aux.sshCls, 0, 0, (void*)SshProc, &msg);
                }

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
#if 0
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
#endif
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }
        break; // <------
    } while (timeout > (dt = (AfxGetTime(&last) - first)));
    return dt;
}
#endif

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

    afxUri32 urib2;
    AfxMakeUri32(&urib2, NIL);

    char buf[256];
    struct stat st;

    for (int i = 0; i < 26; i++)
    {
        sprintf(buf, "%c:\\", 'A' + i);

        if (0 == stat(buf, &st))
        {
            AfxFormatUri(&urib2.uri, "%c:\\", 'A' + i);
            
            afxFileFlags ioFlags = NIL;

            if ((st.st_mode & _S_IREAD) == _S_IREAD)
                ioFlags |= afxFileFlag_R;

            if ((st.st_mode & _S_IWRITE) == _S_IWRITE)
                ioFlags |= afxFileFlag_W;

            if ((st.st_mode & _S_IEXEC) == _S_IEXEC)
                ioFlags |= afxFileFlag_X;

            AfxMountStorageUnit('a' + i, &urib2.uri, ioFlags);
        }
    }

    int a = 0;
    return err;
}

_AFX afxError _AfxSysMountDefaultFileStorages(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    
    afxUri point, location;
    AfxMakeUri(&location, 0, "", 0);

    if (AfxMountStorageUnit('z', &location, afxFileFlag_RX)) AfxThrowError();
    else
    {
        AfxMakeUri(&location, 0, "system", 0);

        if (AfxMountStorageUnit('z', &location, afxFileFlag_RX)) AfxThrowError();
        else
        {
#ifdef AFX_ISA_X86_64
#   ifdef AFX_OS_WIN
            AfxMakeUri(&location, 0, "system64", 0);

            if (AfxMountStorageUnit('z', &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, 0, "system64d", 0);

                if (AfxMountStorageUnit('z', &location, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
            AfxMakeUri(&location, 0, "C:\\Windows\\System32", 0);

            if (AfxMountStorageUnit('c', &location, afxFileFlag_RX))
                AfxThrowError();
#   elif defined(AFX_OS_LNX)
            AfxMakeUri(&location, 0, "system64x", 0);

            if (AfxMountStorageUnit('z', &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, 0, "system64xd", 0);

                if (AfxMountStorageUnit('z', &location, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
#   endif
#elif defined(AFX_ISA_X86_32)
#   ifdef AFX_OS_WIN
            AfxMakeUri(&location, 0, "system32", 0);

            if (AfxMountStorageUnit('z', &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, 0, "system32d", 0);

                if (AfxMountStorageUnit('z', &location, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif

            AfxMakeUri(&location, 0, "C:\\Windows\\SysWow64", 0);

            if (AfxMountStorageUnit('c', &location, afxFileFlag_RX))
                AfxThrowError();
#   elif defined(AFX_OS_LNX)
            AfxMakeUri(&location, 0, "systemx", 0);

            if (AfxMountStorageUnit('z', &location, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location, 0, "system32xd", 0);

                if (AfxMountStorageUnit('z', &location, afxFileFlag_RWX))
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
        AfxMakeUri(&location, "code", 0);

        if (AfxMountStorageUnit('c', &location, afxFileFlag_RX)) AfxThrowError();
        else
        {
            AfxMakeUri(&location, "sound", 0);

            if (AfxMountStorageUnit('s', &location, afxFileFlag_RX)) AfxThrowError();
            else
            {
                AfxMakeUri(&location, "data", 0);

                if (AfxMountStorageUnit('d', &location, afxFileFlag_RX)) AfxThrowError();
                else
                {
                    AfxMakeUri(&location, "art", 0);

                    if (AfxMountStorageUnit('a', &location, afxFileFlag_RX)) AfxThrowError();
                    else
                    {
                        AfxMakeUri(&location, "tmp", 0);

                        if (AfxMountStorageUnit('t', &location, afxFileFlag_RWX))
                            AfxThrowError();
                    }
                }
            }
        }
    }
#endif
    //MountHostVolumes();
    return err;
}

_AFX afxError _AfxSysCtor(afxSystem sys, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    afxManifest* ini = args[0];
    afxSystemConfig const *cfg = args[1];

    //AfxZero(sys, sizeof(sys[0]));

    _AfxInitMmu(NIL);

    afxChain* classes = &sys->classes;
    AfxDeployChain(classes, sys);
    AfxRegisterClass(&sys->strbCls, NIL, classes, &_AfxStrbMgrCfg);

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
    AfxRegisterClass(&sys->thrCls, NIL, classes, &clsCfg); // require txu

    AfxRegisterClass(&sys->mmuCls, NIL, classes, &_AfxMmuMgrCfg);

    AfxRegisterClass(&sys->svcCls, NIL, classes, &_AfxSvcMgrCfg);
    //AfxRegisterClass(&sys->fileMgr, NIL, classes, &_AfxFileMgrConfig);
    AfxRegisterClass(&sys->cdcCls, NIL, classes, &_AfxCdcMgrCfg);

    AfxRegisterClass(&sys->iosCls, NIL, classes, &_AfxIosClsCfg);

    AfxRegisterClass(&sys->fsysCls, NIL, classes, &_AfxFsysMgrCfg); // require iob, arch

    AfxRegisterClass(&sys->mdleCls, NIL, classes, &_AfxMdleMgrCfg);
    //AfxRegisterClass(&sys->icdCls, &sys->exeMgr, classes, &_AfxIcdClsCfg);
    
    clsCfg = _AfxDevBaseImplementation;
    clsCfg.ctor = NIL; // must be called by specialized device constructor. It is needed to perform nested specialized device creation.
    AfxRegisterClass(&sys->devCls, NIL, classes, &clsCfg);

    clsCfg = _AfxXexuStdImplementation;
    AfxRegisterClass(&sys->xexuCls, NIL, classes, &clsCfg);

    afxThreadConfig thrCfg = { 0 };
    thrCfg.tid = (sys->primeTid = AfxGetTid());
    
    if (AfxAcquireThreads(AfxHere(), &thrCfg, 1, &sys->primeThr)) AfxThrowError();
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

            if (AfxLoadModule(&urib.uri, AFX_BIT(8), &e2coree)) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &e2coree, afxFcc_MDLE);
                sys->e2coree = e2coree;

                if (AfxAcquireIoBridge(&cfg->mainIoBridge, &sys->primeExu))
                    AfxThrowError();

                if (err)
                    AfxReleaseObjects(1, &sys->e2coree);
            }
        }
    }

    if (err)
        AfxDeregisterChainedClasses(&sys->classes);

    return err;
}

_AFX afxError _AfxSysDtor(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    AfxReleaseObjects(1, &sys->e2coree);
    AfxReleaseObjects(1, &sys->primeThr);

    // objects will be released at class drop.
    AfxDeregisterChainedClasses(&sys->classes);

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
    AfxDeployManifest(&ini);
    AfxMakeUri(&uri, 0, "system.ini", 0);

    if (AfxLoadInitializationFile(&ini, &uri))
    {
        afxString2048 tbs;
        AfxMakeString2048(&tbs, TheSystem ? AfxGetUriString(&TheSystem->pwd.uri) : NIL);
        AfxCatenateStringL(&tbs.str, "system.ini", 0);
        
        AfxWrapUriString(&uri, &tbs.str);
        AfxLoadInitializationFile(&ini, &uri);
    }

#ifdef AFX_OS_WIN
    SYSTEM_INFO si;
    GetSystemInfo(&si);
#endif

    afxString sSystem = AFX_STRING("System");

    if (!AfxGetInitializationNat(&ini, &sSystem, &AfxString("nIoBufferSize"), &cfg.ioBufSiz) || !cfg.ioBufSiz)
    {
        cfg.ioBufSiz = BUFSIZ;
    }

    if (!AfxGetInitializationReal(&ini, &sSystem, &AfxString("fUnitsPerMeter"), &cfg.unitsToMeter) || !cfg.unitsToMeter)
    {
        cfg.unitsToMeter = renderwareUnitsPerMeter;
    }

    if (!AfxGetInitializationNat(&ini, &sSystem, &AfxString("nHwThreadingCapacity"), &cfg.hwThreadingCap) || !cfg.hwThreadingCap)
    {
        cfg.hwThreadingCap =
#ifdef AFX_OS_WIN
            si.dwNumberOfProcessors;
#else
            1;
#endif
    }

    if (!AfxGetInitializationNat(&ini, &sSystem, &AfxString("nMemoryPageSize"), &cfg.memPageSiz) || !cfg.memPageSiz)
    {
        cfg.memPageSiz =
#ifdef AFX_OS_WIN
            si.dwPageSize;
#else
            4096;
#endif
    }

    if (!AfxGetInitializationNat(&ini, &sSystem, &AfxString("nAllocationGranularity"), &cfg.allocGranularity) || !cfg.allocGranularity)
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

    if (!(AfxGetInitializationBool(&ini, &AfxString("DrawSystem"), &AfxString("bDisabled"), &cfg.avxDisabled)))
    {
        cfg.avxDisabled = FALSE;
    }

    if (!(AfxGetInitializationBool(&ini, &AfxString("SoundSystem"), &AfxString("bDisabled"), &cfg.asxDisabled)))
    {
        cfg.asxDisabled = FALSE;
    }

    if (!(AfxGetInitializationBool(&ini, &AfxString("Shell"), &AfxString("bDisabled"), &cfg.auxDisabled)))
    {
        cfg.auxDisabled = FALSE;
    }

    AfxDismantleManifest(&ini);

    *config = cfg;
}
