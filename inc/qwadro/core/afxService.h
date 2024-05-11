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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// The autonomous service interface for Qwadro.

#ifndef AFX_SERVICE_H
#define AFX_SERVICE_H

#include "qwadro/io/afxStream.h"
#include "qwadro/mem/afxFifo.h"

typedef enum afxJobType
{
    afxJobType_
} afxJobType;

typedef void(*afxWorkerFn) (afxJobType typ, afxNat execTimeUsec);

/// Callback function prototype definition used for handling requests from service for new workers to perform work.
typedef void(*afxReqWorkerFn)(afxWorkerFn workerFn, afxJobType typ, afxNat workerCnt, void* udd);

AFX_DEFINE_STRUCT(afxServiceConfig)
{
    afxReqWorkerFn  reqJobWorkerFn; /// Function called by the job manager when a new worker needs to be requested. When null, all jobs will be executed on the same thread that calls DoService().
    afxNat          maxActiveWorkers; /// The maximum number of concurrent workers that will be requested. Must be >= 1 for each job type.
    afxNat          memSlabCnt; /// Number of memory slabs to pre-allocate for job manager memory. At least one slab per worker thread should be pre-allocated. Default is 1.
    afxNat          memSlabSiz; /// Size of each memory slab used for job manager memory. Must be a power of two. Default is 8K.
    void*           udd; /// Arbitrary data that will be passed back to the client when calling afxReqWorkerFn.
};

#ifdef _AFX_CORE_C
#ifdef _AFX_SERVICE_C
AFX_OBJECT(afxService)
{
    afxBool         running;
    struct WorkerState
    {
        afxThread       thr;
        afxNat          execTimeUsec;
        afxNat          workerIdx;
        volatile afxBool keepAlive;
    }*              workers;
    //AkSemaphore g_semaphore;
    afxWorkerFn     workerFn;
    afxNat          execTimeUsec; // Maximum amount of time allotted for one execution of a worker, in microseconds. Defaults to 0 (no timeout).
    afxNat          workerThrCnt;  // How many threads to allocate for processing jobs.
    void*           workerThrProps; // Array of thread settings, should have uNumWorkerThreads elements. If null, will take default thread settings
    afxFifo         jobTypeQue;


    afxReqWorkerFn  reqJobWorkerFn; /// Function called by the job manager when a new worker needs to be requested. When null, all jobs will be executed on the same thread that calls DoService().
    afxNat          maxActiveWorkers; /// The maximum number of concurrent workers that will be requested. Must be >= 1 for each job type.
    afxNat          memSlabCnt; /// Number of memory slabs to pre-allocate for job manager memory. At least one slab per worker thread should be pre-allocated. Default is 1.
    afxNat          memSlabSiz; /// Size of each memory slab used for job manager memory. Must be a power of two. Default is 8K.
    void*           udd; /// Arbitrary data that will be passed back to the client when calling afxReqWorkerFn.
};
#endif//_AFX_SERVICE_C
#endif//_AFX_CORE_C

AFX afxError        AfxDoService(afxService svc, afxTime timeout);

////////////////////////////////////////////////////////////////////////////////

AFX afxError        AfxAcquireServices(afxNat cnt, afxServiceConfig const config[], afxService services[]);

#endif//AFX_SERVICE_H
