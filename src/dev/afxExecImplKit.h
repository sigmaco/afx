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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_EXEC_IMPL_KIT_H
#define AFX_EXEC_IMPL_KIT_H

#include "qwadro/inc/exec/afxSystem.h"

#ifdef _AFX_CONTEXT_C
AFX_OBJECT(afxContext)
{
    afxChain            classes;
    afxClass            semaphores;
    afxClass            fences;
    afxError            (*waitFenc)(afxBool, afxUnit64, afxUnit, const afxFence[]);
    afxError            (*resetFenc)(afxUnit, afxFence const[]);
};
#endif//_AFX_CONTEXT_C

#ifdef _AFX_DEVICE_C
AFX_OBJECT(afxDevice)
{
    afxUri8             urn; // "//./dev/"
    afxDeviceType       type;
    afxDeviceStatus     status;
    afxAcceleration     acceleration;
    afxBool             serving;

    afxUnit32            vendorId;
    afxUnit32            vendorDevId;
    afxUnit32            driverVer;
    afxUnit32            apiVer;

    afxLinkage          icd;
    afxString128        devName;

    afxChain            classes;

    afxUnit              manifestPagNo;

    afxError            (*procCb)(afxDevice,afxThread,void*);
    afxResult           (*ioctlCb)(afxDevice,afxUnit,va_list);
};
#endif//_AFX_DEVICE_C

#ifdef _AFX_FENCE_C
#ifdef _AFX_FENCE_IMPL
AFX_OBJECT(_afxFence)
#else
AFX_OBJECT(afxFence)
#endif
{
    afxAtom32       signaled;
};
#endif//_AFX_FENCE_C

#ifdef _AFX_MODULE_C
AFX_OBJECT(afxModule)
{
    afxModuleType       type;
    afxUri128           path;
    void*               osHandle;
    afxBool             hasBeenLoaded;
    afxBool             demangle;

    afxManifest         etc;
    afxFlags            flags;
    afxChain            devices; // if it is a ICD, this chain interlinks each device provided by this driver.
    afxBool             attached;
   
    afxError(*icdHookFn)(afxModule, afxUri const*);

    afxString           description;
    afxUnit              verMajor, verMinor, verPatch, verBuild;
    afxString           product;
    afxString           vendor;
    afxString           legal;
};
#endif//_AFX_MODULE_C

#if 0
#ifdef _AFX_DRIVER_C
AFX_OBJECT(afxDriver)
{
    AFX_OBJ(afxModule)  mdle;
    afxManifest         etc;
    afxChain            devices;
    afxFlags            flags;

    afxError            (*icdHookFn)(afxDriver, afxUri const*);

    afxString           description;
    afxString           version;
    afxString           product;
    afxString           vendor;
    afxString           legal;
};
#endif//_AFX_DRIVER_C
#endif

#ifdef _AFX_SEMAPHORE_C
#ifdef _AFX_SEMAPHORE_IMPL
AFX_OBJECT(_afxSemaphore)
#else
AFX_OBJECT(afxSemaphore)
#endif
{
    afxUnit64            value;
};
#endif//_AFX_SEMAPHORE_C

#ifdef _AFX_SERVICE_C
AFX_OBJECT(afxService)
{
    afxBool         running;
    struct WorkerState
    {
        afxThread       thr;
        afxUnit          execTimeUsec;
        afxUnit          workerIdx;
        volatile afxBool keepAlive;
    }*              workers;
    //AkSemaphore g_semaphore;
    afxWorkerFn     workerFn;
    afxUnit          execTimeUsec; // Maximum amount of time allotted for one execution of a worker, in microseconds. Defaults to 0 (no timeout).
    afxUnit          workerThrCnt;  // How many threads to allocate for processing jobs.
    void*           workerThrProps; // Array of thread settings, should have uNumWorkerThreads elements. If null, will take default thread settings
    afxInterlockedQueue         jobTypeQue;


    afxReqWorkerFn  reqJobWorkerFn; /// Function called by the job manager when a new worker needs to be requested. When null, all jobs will be executed on the same thread that calls DoService().
    afxUnit          maxActiveWorkers; /// The maximum number of concurrent workers that will be requested. Must be >= 1 for each job type.
    afxUnit          memSlabCnt; /// Number of memory slabs to pre-allocate for job manager memory. At least one slab per worker thread should be pre-allocated. Default is 1.
    afxUnit          memSlabSiz; /// Size of each memory slab used for job manager memory. Must be a power of two. Default is 8K.
    void*           udd; /// Arbitrary data that will be passed back to the client when calling afxReqWorkerFn.
};
#endif//_AFX_SERVICE_C

#ifdef _AFX_THREAD_C
AFX_OBJECT(afxThread)
{
    afxUnit              unitIdx;
    afxUnit32            tid;
    void*               osHandle;
    //afxThreadProc       procCb;
    void*               udd[4];

    //afxUnit          affineProcUnitIdx; // if not equal to AFX_INVALID_INDEX, this thread can be ran by any system processor unit, else case, will only be ran by the unit specified by this index.
    //afxUnit          affineThrUnitIdx; // if set bit set, only such processor will can run this thread.
    afxClock            startClock;
    afxClock            lastClock;
    afxClock            execCntSwapClock;
    afxUnit              lastExecCnt;
    afxUnit              execNo;
    afxBool             started;
    afxBool             exited;
    afxBool             running;
    afxUnit              suspendCnt;
    afxBool             isInFinish;
    afxBool             finished;
    afxBool             interruptionRequested;
    afxInt              exitCode;

    afxCondition        statusCnd;
    afxMutex            statusCndMtx;

    afxSlock            evSlock;
    afxArena            evArena;
    afxQueue            events;
    afxInterlockedQueue events2;

    afxChar const*      _file_;
    afxSize             _line_;
    afxChar const*      _func_;
};
#endif//_AFX_THREAD_C

#ifdef _AFX_SYSTEM_C
AFX_OBJECT(afxSystem)
{
    afxBool                 isInBootUp;
    afxBool                 isInShutdown;
    afxBool                 operating;
    afxBool                 interruptionRequested;
    afxInt                  exitCode;

    afxChain                classes;
    afxClass                mmuCls;
    afxClass                strbCls;
    afxClass                mdleCls;
    afxClass                thrCls;
    afxClass                svcCls;
    afxClass                devCls;
    afxClass                cdcCls;
    afxClass                iosCls;
    afxClass                fsysCls;
    afxClass                xexuCls;

    afxUnit                  ptrSiz;
    afxBool                 bigEndian;
    afxUnit                  memPageSiz; // The page size and the granularity of page protection and commitment.
    afxUnit                  allocGranularity;
    afxUnit                  hwThreadingCap; // # of logical proc units (hardware threads)
    afxReal                 unitsPerMeter; // the number of units in a meter.
    afxSize                 maxMemUsage;
    afxUnit                  ioBufSiz;
    afxIoBridge             primeExu;
    afxUnit32                primeTid;
    afxThread               primeThr;

    afxAssertHook           assertHook; // external assertion handling function (optional)
    afxReallocatorFn        reallocatorFn;
    afxProfilerPushTimerFn  profilerPushTimer; // external (optional) function for tracking performance of the system that is called when a timer starts. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPopTimerFn   profilerPopTimer; // external (optional) function for tracking performance of the system that is called when a timer stops. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPostMarkerFn profilerPostMarker; // external (optional) function for tracking significant events in the system, to act as a marker or bookmark. (only called in Debug and Profile binaries; this is not called in Release)

    afxUri2048              pwd; // process working dir (usually abs/path/to/qwadro/system/$(host)/)
    afxUri2048              qwd; // root dir for Qwadro forked from pwd (usually qwadro/system/$(host)/../../)

    afxStorage              defStops[9]; // [ ., system/$(host)d, system/$(host), system, code, sound, data, art, tmp ]

    afxModule               e2coree;

    // avx
    struct
    {
        afxBool             disabled;
        afxModule           e2drawDll;
        afxClass            ddevCls;
        afxError            (*ioctl)(afxSystem, afxModule, afxUnit, void*);
    }                       avx;
    // asx
    struct
    {
        afxBool             disabled;
        afxModule           e2soundDll;
        afxClass            sdevCls;
        afxError            (*ioctl)(afxSystem, afxModule, afxUnit, void*);
    }                       asx;
    // amx
    struct
    {
        afxBool             disabled;
        afxModule           e2simDll;
        afxClass            mdevCls;
        afxError            (*ioctl)(afxSystem, afxModule, afxUnit, void*);
    }                       amx;
    // aux (shell)
    struct
    {
        afxBool             disabled;
        afxModule           e2mmuxDll;
        afxClass            hidCls;
        afxClass            sshCls;
        afxError            (*ioctl)(afxSystem, afxModule, afxUnit, void*);
    }                       aux;

    afxModule               e2sim;

    struct
    {
        struct
        {
            afxFcc          type;
            afxChain        resources;
        }                   supplyChain[1];
    }                       resourcing;
};
#endif//_AFX_SYSTEM_C

AFX afxClassConfig const _AfxDevBaseImplementation;

#endif//AFX_EXEC_IMPL_KIT_H
