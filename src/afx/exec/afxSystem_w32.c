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
_AFX afxByte theSysData[AFX_ALIGN(sizeof(afxObjectBase), 16) + AFX_ALIGN(sizeof(((afxSystem)0)[0]), 16)] = { 0 };
_AFX afxSystem TheSystem = (void*)&theSysData;
AFX_STATIC_ASSERT(sizeof(TheSystem[0]) > sizeof(void*), "");
AFX_STATIC_ASSERT(sizeof(theSysData) >= (sizeof(afxObjectBase) + sizeof(TheSystem[0])), "");
#else
_AFX afxSystem TheSystem = NIL;
#endif
extern afxReal64 const renderWareUnitsPerMeter;
extern afxThread _primeThr;
extern afxNat32 _primeTid;

extern afxClassConfig const _AfxMmuMgrCfg;
extern afxClassConfig const _AfxMdleMgrCfg;
extern afxClassConfig const _AfxThrMgrCfg;
extern afxClassConfig const _AfxIosMgrCfg;
extern afxClassConfig const _AfxCdcMgrCfg;
//extern afxClassConfig const _AfxFileMgrCfg;
extern afxClassConfig const _AfxArcMgrCfg;
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
    while (PeekMessageA(&(msg), NIL, 0, 0, PM_REMOVE/* | PM_NOYIELD*/))
    {
        ++msgCnt;

        if (msg.message == WM_QUIT) // PostQuitMessage()
        {
            AfxRequestShutdown(0);
        }
        else
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
                    AfxMakeString(&cur, "\n$ ", 0);
                    afxString2048 buf;
                    AfxMakeString2048(&buf, NIL);
                    AfxPrompt(&cur, &buf.str);
                    AfxPrint(0, buf.buf);
                    AfxPrint(0, "\n");
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

#if 0
_AFX afxBool DoDevService(afxDevice dev, afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    AfxAssertObjects(1, &thr, afxFcc_THR);
    dev->procCb(dev, thr);
    return 0; // continue
}
#endif

extern afxBool _AfxThrExecuteCb(afxThread thr, void* udd);

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

            afxNat tid = AfxGetTid();
            
            if (tid == _primeTid)
            {
                if (sys->avx.ioctl)
                    sys->avx.ioctl(sys, sys->avx.e2drawDll, 0, &timeout);

                if (sys->asx.ioctl)
                    sys->asx.ioctl(sys, sys->asx.e2soundDll, 0, &timeout);

                if (sys->aux.ioctl)
                    sys->aux.ioctl(sys, sys->aux.e2uxDll, 0, &timeout);

                //AfxInvokeDevices(NIL, 0, AFX_N32_MAX, (void*)DoDevService, _primeThr);
                //PrimeThreadProc(sys->primeThr, );
            }
            else
            {
                afxThread curr;
                AfxGetThread(&curr);
                _AfxThrExecuteCb(curr, &timeout);
                //AfxInvokeThreads(0, AFX_N32_MAX, _AfxThrExecuteCb, &timeout);
            }
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
            AfxFormatUri(&urib.uri, "//./%c/", 'a' + i);
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
#ifdef AFX_ISA_X86_64
#   ifdef AFX_OS_WIN
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
#   elif defined(AFX_OS_LNX)
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
#elif defined(AFX_ISA_X86_32)
#   ifdef AFX_OS_WIN
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
#   elif defined(AFX_OS_LNX)
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
    AfxSetUpChain(classes, sys);
    AfxRegisterClass(&sys->strbMgr, NIL, classes, &_AfxStrbMgrCfg);

    // setting the process working directory and the Qwadro/system working directory...
    afxUri uri;
    afxChar buf[2048];
    GetModuleFileNameA(NIL, buf, sizeof(buf));
    AfxStrrchr(buf, '\\')[1] = '\0';
    AfxMakeUri(&uri, buf, 0);
    AfxMakeUri2048(&sys->pwd, &uri);
    AfxCanonicalizePath(&sys->pwd.uri, TRUE);
    GetModuleFileNameA(GetModuleHandleA("e2coree"), buf, sizeof(buf));    
    AfxStrrchr(buf, '\\')[1] = '\0';
    AfxStrcat(buf, "..\\");
    AfxMakeUri(&uri, buf, 0);
    AfxMakeUri2048(&sys->qwd, &uri);
    AfxCanonicalizePath(&sys->qwd.uri, TRUE);
    SetCurrentDirectoryA(AfxGetUriData(&sys->pwd.uri, 0));

#ifdef AFX_OS_WIN
    SYSTEM_INFO si;
    GetSystemInfo(&si);
#endif
#if !0
    AfxSetUpIni(ini);
    AfxMakeUri(&uri, "system.ini", 0);

    if (AfxIniLoadFromFile(ini, &uri))
    {
        afxString2048 tbs;
        AfxMakeString2048(&tbs, AfxGetUriString(&sys->pwd.uri));
        AfxConcatenateStringL(&tbs.str, "system.ini", 0);

        AfxMakeUriFromString(&uri, &tbs.str.str);
        AfxIniLoadFromFile(ini, &uri);
    }
    
    afxString issSystem = AFX_STRING("System");

    if (!AfxIniGetNat(ini, &issSystem, &AfxString("nIoBufferSize"), &sys->ioBufSiz) || !sys->ioBufSiz)
    {
        sys->ioBufSiz = BUFSIZ;
    }

    if (!AfxIniGetReal(ini, &issSystem, &AfxString("fUnitsPerMeter"), &sys->unitsPerMeter) || !sys->unitsPerMeter)
    {
        sys->unitsPerMeter = renderWareUnitsPerMeter;
    }

#endif
    if (!AfxIniGetNat(ini, &issSystem, &AfxString("nHardwareThreadingCapacity"), &sys->hwConcurrencyCap) || (0 == sys->hwConcurrencyCap))
    {
        sys->hwConcurrencyCap =
#ifdef AFX_OS_WIN
            si.dwNumberOfProcessors;
#else
            1;
#endif
    }

    if (!AfxIniGetNat(ini, &issSystem, &AfxString("nMemoryPageSize"), &sys->memPageSize) || (0 == sys->memPageSize))
    {
        sys->memPageSize =
#ifdef AFX_OS_WIN
            si.dwPageSize;
#else
            4096;
#endif
    }

    if (!AfxIniGetNat(ini, &issSystem, &AfxString("nAllocationGranularity"), &sys->allocGranularity) || (0 == sys->allocGranularity))
    {
        sys->allocGranularity =
#ifdef AFX_OS_WIN
            si.dwAllocationGranularity;
#else
            4096;
#endif
    }


    _primeTid = AfxGetTid();
    AfxAssert(_primeTid);
    sys->ioBufSiz = BUFSIZ;
    sys->unitsPerMeter = renderWareUnitsPerMeter;


    afxSystemConfig defCfg;
    AfxChooseSystemConfiguration(&defCfg);

    sys->assertHook = defCfg.assertHook;
    sys->profilerPopTimer = defCfg.profilerPopTimer;
    sys->profilerPushTimer = defCfg.profilerPushTimer;
    sys->profilerPostMarker = defCfg.profilerPostMarker;

    sys->ptrSiz = sizeof(void*);
    sys->bigEndian = FALSE;

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


    afxClassConfig clsCfg;

    clsCfg = _AfxThrMgrCfg;
    clsCfg.unitsPerPage = sys->hwConcurrencyCap;
    AfxRegisterClass(&sys->thrMgr, NIL, classes, &clsCfg); // require txu

    AfxRegisterClass(&sys->mmuMgr, NIL, classes, &_AfxMmuMgrCfg);

    AfxRegisterClass(&sys->svcMgr, NIL, classes, &_AfxSvcMgrCfg);
    //AfxRegisterClass(&sys->fileMgr, NIL, classes, &_AfxFileMgrConfig);
    AfxRegisterClass(&sys->cdcMgr, NIL, classes, &_AfxCdcMgrCfg);


    AfxRegisterClass(&sys->iosMgr, NIL, classes, &_AfxIosMgrCfg);
    AfxRegisterClass(&sys->archMgr, NIL, classes, &_AfxArcMgrCfg); // require iob
    AfxRegisterClass(&sys->fsysMgr, NIL, classes, &_AfxFsysMgrCfg); // require iob, arch


    AfxRegisterClass(&sys->exeMgr, NIL, classes, &_AfxMdleMgrCfg);
    AfxRegisterClass(&sys->devMgr, NIL, classes, &_AfxDevMgrCfg); // require mdle

    afxThreadConfig thrCfg = { 0 };
    thrCfg.tid = AfxGetTid();
    thrCfg.procCb = PrimeThreadProc;

    void *p = AfxAllocate(1, 16, 0, AfxHere());
    AfxDeallocate(p);
    p = AfxAllocate(1, 16, 0, AfxHere());
    AfxDeallocate(p);
    p = AfxAllocate(1, 16, 0, AfxHere());
    AfxDeallocate(p);
    p = AfxAllocate(1, 16, 0, AfxHere());
    AfxDeallocate(p);
    p = AfxAllocate(1, 16, 0, AfxHere());
    AfxDeallocate(p);
    p = AfxAllocate(1, 16, 0, AfxHere());
    AfxDeallocate(p);
    p = AfxAllocate(1, 16, 0, AfxHere());
    AfxDeallocate(p);
    p = AfxAllocate(1, 16, 0, AfxHere());
    AfxDeallocate(p);
    p = AfxAllocate(1, 16, 0, AfxHere());
    AfxDeallocate(p);
    p = AfxAllocate(1, 16, 0, AfxHere());
    void *p0 = AfxAllocate(1, 1600, 0, AfxHere());
    void *p1 = AfxAllocate(1, 1006, 0, AfxHere());
    void *p2 = AfxAllocate(1, 106, 0, AfxHere());
    void *p3 = AfxAllocate(1, 16000, 0, AfxHere());
    void *p4 = AfxAllocate(1, 1600, 0, AfxHere());
    void *p5 = AfxAllocate(1, 16000, 0, AfxHere());
    void *p6 = AfxAllocate(1, 16000, 0, AfxHere());
    void *p7 = AfxAllocate(1, 16000, 0, AfxHere());
    void *p8 = AfxAllocate(1, 16000, 0, AfxHere());
    void *p9 = AfxAllocate(1, 10600, 0, AfxHere());
    AfxDeallocate(p);
    AfxDeallocate(p1);
    AfxDeallocate(p2);
    AfxDeallocate(p3);
    AfxDeallocate(p4);
    AfxDeallocate(p5);
    AfxDeallocate(p6);
    AfxDeallocate(p7);
    AfxDeallocate(p8);
    AfxDeallocate(p9);

    if (AfxAcquireThread(AfxHere(), &thrCfg, &_primeThr)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &_primeThr, afxFcc_THR);
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
    AfxReleaseObjects(1, &_primeThr);

    // objects will be released at class drop.
    AfxCleanUpChainedClasses(&sys->classes);

    AfxDeallocateUri(&sys->pwd.uri);

    //AfxDeallocateArena(&sys->ioArena);
    //AfxReleaseObject(&(sys->memD->obj));

    //_AfxFreeExternalAllocations();
    
    _AfxDeinitMmu(NIL);

    AfxZero(sys, sizeof(sys[0]));

    return err;
}

_AFX void AfxChooseSystemConfiguration(afxSystemConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    AfxZero(cfg, sizeof(*cfg));

    cfg->assertHook = AfxStdAssertHookCallback;
    
    cfg->root = NIL;
    cfg->profilerPopTimer = NIL;
    cfg->profilerPushTimer = NIL;
    cfg->profilerPostMarker = NIL;

    // load ini here

    //if (platform)
    {
#ifdef AFX_OS_WIN
        AfxZero(&cfg->platform, sizeof(cfg->platform));
        cfg->platform.hInst = GetModuleHandle(NULL);
        cfg->platform.hWnd = GetActiveWindow();
#endif
    }
}
