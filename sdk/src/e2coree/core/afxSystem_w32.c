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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
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
#define _AFX_ARCHIVE_C
#define _AFX_APPLICATION_C
#define _AFX_MMU_C
#define _AFX_DEVICE_C
#define _AFX_DRAW_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_TXU_C
#define _AFX_SOUND_C
#define _AFX_SOUND_SYSTEM_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sound/afxSoundSystem.h"


_AFX afxClass _sysClass;
afxClass _sysClass;
_AFX afxBool _sysClassInited = FALSE;
_AFX afxChar const _rwBuildDateTime[];
static afxChar const  _rwBuildDateTime[] = "\nCore built at " __DATE__ " " __TIME__ "\n";
static afxChar const _Ebenezer[] = "Ebenezer";

_AFX afxSystem theSys;
afxSystem theSys = NIL;
//_AFX struct _afxSysD theSys;
_AFX afxClassConfig _AfxSysClsConfig;

_AFX afxNat mainThreadId;
extern afxResult _AfxDoSystemThreading(afxSystem sys, afxTime timeout);
extern afxError _AfxSysLoadIcd(afxSystem sys, afxUri const* file, afxIcd *icd);

extern afxClassConfig _AfxMmuClsConfig;
extern afxClassConfig _AfxMdleClsConfig;
extern afxClassConfig _AfxThrClsConfig;
extern afxClassConfig _AfxTxuClsConfig;
extern afxClassConfig _AfxIosClsConfig;
extern afxClassConfig _AfxFileClsConfig;
extern afxClassConfig _AfxArcClsConfig;
extern afxClassConfig const _AfxAppClsConfig;
extern afxClassConfig _AfxKbdClsConfig;
extern afxClassConfig _AfxMseClsConfig;
extern afxClassConfig const _AfxSimClsConfig;
extern afxClassConfig const _AfxSvcClsConfig;
extern afxClassConfig const _AfxIcdClsConfig;
extern afxClassConfig const _AfxDevClsConfig;
extern afxClassConfig const _AfxStoClsConfig;
extern afxClassConfig const _AfxHidClsConfig;

extern afxError _AfxDsysCtor(afxDrawSystem dsys, afxSystem sys, afxDrawSystemConfig const* config);
extern afxError _AfxDsysDtor(afxDrawSystem dsys);
extern afxError _AfxSsysCtor(afxSoundSystem ssys, afxSystem sys, afxSoundSystemConfig const* config);
extern afxError _AfxSsysDtor(afxSoundSystem ssys);

AFX_DEFINE_STRUCT(afxStoragePoint)
{
    afxFcc          fcc;
    afxLinkage      io;

    afxUri         name; // name of exchange point // AFX_FS_SYM_LEN // can't have control chars. Actually works like a variable without $().
    afxUri         path; // path of exchange point
    afxIoFlags      flags;
    afxArchive      arc;
};

#if 0
AFX afxBool _AfxGetSysD(afxSystem sys, struct _afxSysD**sysD)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxBool rslt = FALSE;

    if (theSys->sysObj == sys)
    {
        AfxAssert(sysD);
        *sysD = &theSys;
        rslt = TRUE;
    }
    return rslt;
}
#endif

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

AFX afxError _AfxMountSystemThreadingUnit(afxSystem sys);

// READ ONLY METHODS //////////////////////////////////////////////////////////

_AFX afxSystem AfxGetSystem(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = theSys;
    AfxTryAssertObjects(1, &sys, afxFcc_SYS);
    return sys;
}

_AFXINL afxDrawSystem _AfxGetDsysData(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxDrawSystem dsys = sys->dsys;
    AfxAssertType(dsys, afxFcc_DSYS);
    return dsys;
}

_AFXINL afxSoundSystem _AfxGetSsysData(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxSoundSystem ssys = sys->ssys;
    AfxAssertType(ssys, afxFcc_SSYS);
    return ssys;
}

_AFX afxNat AfxGetIoBufferSize(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->ioBufSiz;
}

_AFX afxArena* AfxGetIoArena(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxArena *aren = &sys->ioArena;
    AfxAssertType(aren, afxFcc_AREN);
    return aren;
}

_AFX afxMmu AfxGetIoContext(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxMmu mmu = sys->mmu;
    AfxTryAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AFX afxMmu AfxGetSystemContext(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxMmu mmu = sys->mmu;
    AfxTryAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AFX afxNat AfxGetMemoryPageSize(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->memPageSize;
}

_AFX afxClass* AfxGetStorageClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->storages;
    AfxAssertClass(cls, afxFcc_STO);
    return cls;
}

_AFX afxNat AfxGetThreadingCapacity(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    return sys->hwConcurrencyCap;
}

_AFX afxClass* AfxGetSimulationClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->simulations;
    AfxAssertClass(cls, afxFcc_SIM);
    return cls;
}

_AFX afxClass* AfxGetServiceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->services;
    AfxAssertClass(cls, afxFcc_SVC);
    return cls;
}

_AFX afxClass* AfxGetThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->threads;
    AfxAssertClass(cls, afxFcc_THR);
    return cls;
}

_AFX afxClass* AfxGetTxuClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->txus;
    AfxAssertClass(cls, afxFcc_TXU);
    return cls;
}

_AFX afxClass* AfxGetDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->devices;
    AfxAssertClass(cls, afxFcc_DEV);
    return cls;
}

_AFX afxClass* AfxGetFileClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->files;
    AfxAssertClass(cls, afxFcc_FILE);
    return cls;
}

_AFX afxClass* AfxGetArchiveClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->archives;
    AfxAssertClass(cls, afxFcc_ARC);
    return cls;
}

_AFX afxClass* AfxGetMmuClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->mmus;
    AfxAssertClass(cls, afxFcc_MMU);
    return cls;
}

_AFX afxClass* AfxGetStreamClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->streams;
    AfxAssertClass(cls, afxFcc_IOS);
    return cls;
}

_AFX afxClass* AfxGetHidClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->hids;
    AfxAssertClass(cls, afxFcc_HID);
    return cls;
}

_AFX afxClass* AfxGetKeyboardClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->keyboards;
    AfxAssertClass(cls, afxFcc_KBD);
    return cls;
}

_AFX afxClass* AfxGetMouseClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->mouses;
    AfxAssertClass(cls, afxFcc_MSE);
    return cls;
}

_AFX afxClass* AfxGetModuleClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->modules;
    AfxAssertClass(cls, afxFcc_MDLE);
    return cls;
}

_AFX afxClass* AfxGetIcdClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->icds;
    AfxAssertClass(cls, afxFcc_ICD);
    return cls;
}

_AFX afxClass* AfxGetApplicationClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &sys->applications;
    AfxAssertClass(cls, afxFcc_APP);
    return cls;
}

_AFX afxUri const* AfxGetSystemRootPath(afxUri *copy)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxUri const* rootDir = &sys->rootDir;
    return copy ? AfxCopyUri(copy, rootDir), copy : rootDir;
}

_AFX afxString const* AfxGetSystemRootPathString(afxBufferedString *copy)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxString const* rootDir = AfxGetUriString(&sys->rootDir);
    return copy ? AfxCopyString(copy, rootDir), &copy->str : rootDir;
}

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
    AfxCurateThreads(0, AFX_N32_MAX, _AfxThrQuitAndExecuteCb, &exitCode);
}

_AFX void AfxRequestSystemShutdown(afxInt exitCode)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if (!(sys = AfxGetSystem())) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sys, afxFcc_SYS);
        _AfxInterruptionAllThreads(exitCode);
        sys->exitCode = exitCode;
        sys->isInShutdown = TRUE;
    }
}

_AFX afxBool AfxSystemIsExecuting(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if ((sys = AfxGetSystem()))
    {
        AfxAssertObjects(1, &sys, afxFcc_SYS);
        return sys->operating && !sys->isInShutdown;
    }
    return FALSE;
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

_AFXINL afxBool _AfxNotifyObject(afxHandle *receiver, afxEvent *ev)
{
    // Sends event to receiver: receiver->event(event). 
    // Returns the value that is returned from the receiver's event handler. 
    // Note that this function is called for all events sent to any object in any thread.

    afxError err = AFX_ERR_NONE;    
    AfxAssertType(receiver, afxFcc_OBJ);
    AfxAssertType(ev, afxFcc_EVNT);
    afxBool rslt = AfxObjectEmitEvent(receiver, ev);

    return rslt;
}

_AFX afxBool AfxEmitEvents(afxNat cnt, afxHandle *receiver[], afxEvent ev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(receiver);
    AfxAssert(ev);
    afxBool res = 0;
    
    for (afxNat i = 0; i < cnt; i++)
    {
        afxHandle *obj = receiver[i];
        AfxAssertType(obj, afxFcc_OBJ);
        afxEvent *e = &ev[i];
        AfxAssertType(e, afxFcc_EVNT);
        _AfxNotifyObject(obj, e);
    }
    return res;
}

_AFX afxBool AfxReemitEvent(afxNat cnt, afxHandle *receiver[], afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(receiver);
    AfxAssertType(ev, afxFcc_EVNT);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxHandle *obj = receiver[i];
        AfxAssertType(obj, afxFcc_OBJ);
        _AfxNotifyObject(obj, ev);
    }
    return err;
}

_AFX afxBool AfxEmitEvent(afxHandle *receiver, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(receiver, afxFcc_OBJ);
    AfxAssertType(ev, afxFcc_EVNT);
    return _AfxNotifyObject(receiver, ev);
}

_AFX afxError AfxPostEvents(afxNat cnt, afxHandle *receiver[], afxEvent ev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(receiver);
    AfxAssertType(ev, afxFcc_EVNT);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxHandle *obj = receiver[i];
        AfxAssertType(obj, afxFcc_OBJ);
        _AfxNotifyObject(obj, ev);
        AfxThrowError();
        // Not implemented yet.
    }
    return err;
}

_AFX afxError AfxPostEvent(afxHandle *receiver, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(receiver, afxFcc_OBJ);
    AfxAssertType(ev, afxFcc_EVNT);
    
    if (AfxPostEvents(1, &receiver, ev))
        AfxThrowError();

    return err;
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
    AfxResolveUri(AFX_FILE_FLAG_RX, fileMask, &pathBuf.uri);
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
        AfxMountClass(&sys->simulations, NIL, classes, &_AfxSimClsConfig);

        afxChar buf[2048];
        GetModuleFileNameA(NULL, buf, sizeof(buf));
        PathRemoveFileSpecA(buf);
        AfxConcatRawString(buf, "/");

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
                afxUri rootDir;
                AfxMakeUri(&rootDir, buf, 0);
                AfxCloneUri(&sys->rootDir, &rootDir);
                SetCurrentDirectoryA(AfxGetUriData(&rootDir, 0));

                afxUri tmpUriMaps[6];
                AfxMakeUri(&tmpUriMaps[0], ".", 0);
                AfxMakeUri(&tmpUriMaps[1], "system", 0);
                AfxMakeUri(&tmpUriMaps[2], "sound", 0);
                AfxMakeUri(&tmpUriMaps[3], "data", 0);
                AfxMakeUri(&tmpUriMaps[4], "art", 0);
                AfxMakeUri(&tmpUriMaps[5], "tmp", 0);

                afxUri tmpHostPaths[6];
                AfxMakeUri(&tmpHostPaths[0], "..", 0);
#ifdef AFX_PLATFORM_X64
                AfxMakeUri(&tmpHostPaths[1], "../system64", 0);
#elif defined(AFX_PLATFORM_X86)
                AfxMakeUri(&tmpHostPaths[1], "../system32", 0);
#else
                AfxMakeUri(&tmpHostPaths[1], "../system", 0);
#endif
                AfxMakeUri(&tmpHostPaths[2], "../sound", 0);
                AfxMakeUri(&tmpHostPaths[3], "../data", 0);
                AfxMakeUri(&tmpHostPaths[4], "../art", 0);
                AfxMakeUri(&tmpHostPaths[5], "../tmp", 0);

                afxStoragePointSpecification mnptSpecs[] =
                {
                    { &tmpUriMaps[0], &tmpHostPaths[0], afxIoFlag_RX, FALSE },
                    { &tmpUriMaps[1], &tmpHostPaths[1], afxIoFlag_RX, FALSE },
                    { &tmpUriMaps[2], &tmpHostPaths[2], afxIoFlag_RX, FALSE },
                    { &tmpUriMaps[3], &tmpHostPaths[3], afxIoFlag_RX, FALSE },
                    { &tmpUriMaps[4], &tmpHostPaths[4], afxIoFlag_RX, FALSE },
                    { &tmpUriMaps[5], &tmpHostPaths[5], afxIoFlag_RWX, FALSE },
                };

                afxNat mpCnt = AFX_COUNTOF(mnptSpecs);
                AfxAssert(AFX_COUNTOF(sys->defStops) >= mpCnt);

                if (AfxMountStoragePoints(mpCnt, mnptSpecs, sys->defStops)) AfxThrowError();
                else
                {
                    afxFixedUri128 uri2;
                    AfxMakeFixedUri128(&uri2, NIL);
                    AfxFormatUri(&uri2.uri, "system/e2coree.dll");

                    if (AfxAcquireModules(1, &uri2.uri, &sys->e2coree)) AfxThrowError();
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
                                if (!(sys->ssys = AfxAllocate(sys->mmu, sizeof(*sys->ssys), 1, NIL, AfxHint()))) AfxThrowError();
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
                                    if (!(sys->dsys = AfxAllocate(sys->mmu, sizeof(*sys->dsys), 1, NIL, AfxHint()))) AfxThrowError();
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
                AfxDeallocateUri(&sys->rootDir);

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

    AfxDeallocateUri(&sys->rootDir);

    _AfxUninstallChainedClasses(&sys->classes);
    //AfxDeallocateArena(&sys->ioArena);
    //AfxReleaseObject(&(sys->memD->obj));

    //_AfxFreeExternalAllocations();
    theSys = NIL;
    return err;
}

_AFX afxClassConfig _AfxSysClsConfig =
{
    .fcc = afxFcc_SYS,
    .name = "Basic I/O System",
    .maxCnt = 1,
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSystem)),
    .mmu = NIL,
    .ctor = (void*)_AfxSysCtor,
    .dtor = (void*)_AfxSysDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX void AfxShutdownSystem(afxInt exitCode)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if ((sys = AfxGetSystem()))
    {
        do { AfxRequestSystemShutdown(exitCode); } while (AfxSystemIsExecuting());
        while (!AfxReleaseObjects(1, (void*[]) { sys }));
        AfxDismountClass(&_sysClass);
    };
#ifndef _AFX_DISABLE_DEBUGGER
    AfxDetachDebugger();
#endif
}

_AFX afxError AfxBootUpSystem(afxSystemConfig const *config)
{
    afxError err = AFX_ERR_NONE;

#ifndef _AFX_DISABLE_DEBUGGER
    AfxAttachDebugger(NIL);
#endif
    afxSystem sys;

    if ((sys = AfxGetSystem())) AfxThrowError();
    else
    {
        AfxMountClass(&_sysClass, NIL, NIL, &_AfxSysClsConfig);

        if (AfxAcquireObjects(&_sysClass, 1, (afxObject*)&sys, (void const*[]) { (void*)config })) AfxThrowError();
        else
        {
            AfxAssert(sys == AfxGetSystem());
        }
    }
    return err;
}

_AFX void AfxChooseSystemConfiguration(afxSystemConfig* cfg, afxPlatformConfig* platform)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    AfxZero(1, sizeof(*cfg), cfg);

#ifdef AFX_PLATFORM_WIN32
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
#ifdef AFX_PLATFORM_WIN32
        AfxZero(1, sizeof(*platform), platform);
        platform->instance = GetModuleHandle(NULL);
        platform->hWnd = GetActiveWindow();
#endif
    }
}
