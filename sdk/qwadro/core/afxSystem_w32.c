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

_AFX afxBool sysReady = FALSE;
_AFX afxByte theSysData[AFX_ALIGN(sizeof(afxObjectBase), 16) + AFX_ALIGN(sizeof(AFX_OBJECT(afxSystem)), 16)] = { 0 };
_AFX afxSystem TheSystem = (void*)&theSysData;
static_assert(sizeof(theSysData) >= (sizeof(afxObjectBase) + sizeof(TheSystem[0])), "");

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

_AFX afxBool DoDevService(afxDevice dev, afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    AfxAssertObjects(1, &thr, afxFcc_THR);
    dev->proc(dev, thr);
    return 0; // continue
}

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
                if (sys->ssysctl)
                    sys->ssysctl(sys, 0);

                if (sys->dsysctl)
                    sys->dsysctl(sys, 0);

                AfxInvokeDevices(NIL, 0, AFX_N32_MAX, (void*)DoDevService, _primeThr);
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
    return err;
}

_AFX afxError _AfxSysCtor(afxSystem sys, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    afxManifest* ini = cookie->udd[0];
    afxSystemConfig const *cfg = cookie->udd[1];

    AfxZero(sys, sizeof(sys[0]));

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


#ifdef AFX_OS_WIN32
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
#ifdef AFX_OS_WIN32
            si.dwNumberOfProcessors;
#else
            1;
#endif
    }

    if (0 == sys->memPageSize)
    {
        sys->memPageSize =
#ifdef AFX_OS_WIN32
            si.dwPageSize;
#else
            4096;
#endif
    }

    if (0 == sys->allocGranularity)
    {
        sys->allocGranularity =
#ifdef AFX_OS_WIN32
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
    afxPlatformConfig defPlatCfg;
    AfxChooseSystemConfiguration(&defCfg, &defPlatCfg);

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

    afxChain* mgrChn = &sys->mgrChn;
    AfxSetUpChain(mgrChn, sys);
    afxClassConfig clsCfg;

    clsCfg = _AfxThrMgrCfg;
    clsCfg.unitsPerPage = sys->hwConcurrencyCap;
    AfxEstablishManager(&sys->thrMgr, NIL, mgrChn, &clsCfg); // require txu

    AfxEstablishManager(&sys->mmuMgr, NIL, mgrChn, &_AfxMmuMgrCfg);

    AfxEstablishManager(&sys->svcMgr, NIL, mgrChn, &_AfxSvcMgrCfg);
    //AfxEstablishManager(&sys->fileMgr, NIL, classes, &_AfxFileMgrConfig);
    AfxEstablishManager(&sys->cdcMgr, NIL, mgrChn, &_AfxCdcMgrCfg);
    AfxEstablishManager(&sys->strbMgr, NIL, mgrChn, &_AfxStrbMgrCfg);


    AfxEstablishManager(&sys->iosMgr, NIL, mgrChn, &_AfxIosMgrCfg);
    AfxEstablishManager(&sys->archMgr, NIL, mgrChn, &_AfxArcMgrCfg); // require iob
    AfxEstablishManager(&sys->fsysMgr, NIL, mgrChn, &_AfxFsysMgrCfg); // require iob, arch


    AfxEstablishManager(&sys->exeMgr, NIL, mgrChn, &_AfxMdleMgrCfg);
    AfxEstablishManager(&sys->devMgr, NIL, mgrChn, &_AfxDevMgrCfg); // require mdle

    afxThreadConfig thrCfg = { 0 };
    thrCfg.tid = AfxGetTid();
    thrCfg.procCb = PrimeThreadProc;

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
            AfxFormatUri(&urib.uri, "system/e2coree.dll");
            afxModule e2coree;

            if (AfxLoadModule(&urib.uri, NIL, &e2coree)) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &e2coree, afxFcc_MDLE);
                sys->e2coree = e2coree;

                if (err)
                    AfxReleaseObjects(1, (void*[]) { e2coree });
            }
        }
    }

    if (err)
        AfxCleanUpChainedManagers(&sys->mgrChn);

    return err;
}

_AFX afxError _AfxSysDtor(afxSystem sys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    // objects will be released at class drop.
    AfxCleanUpChainedManagers(&sys->mgrChn);

    AfxDeallocateUri(&sys->pwd.uri);

    //AfxDeallocateArena(&sys->ioArena);
    //AfxReleaseObject(&(sys->memD->obj));

    //_AfxFreeExternalAllocations();

    AfxZero(sys, sizeof(sys[0]));

    return err;
}

_AFX void AfxChooseSystemConfiguration(afxSystemConfig* cfg, afxPlatformConfig* platform)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    AfxZero(cfg, sizeof(*cfg));

    cfg->assertHook = AfxStdAssertHookCallback;
    
    cfg->root = NIL;
    cfg->profilerPopTimer = NIL;
    cfg->profilerPushTimer = NIL;
    cfg->profilerPostMarker = NIL;

    if (platform)
    {
#ifdef AFX_OS_WIN32
        AfxZero(platform, sizeof(*platform));
        platform->instance = GetModuleHandle(NULL);
        platform->hWnd = GetActiveWindow();        
#endif
    }
}
