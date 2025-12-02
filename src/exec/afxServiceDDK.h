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

#ifndef AFX_SERVICE_DDK_H
#define AFX_SERVICE_DDK_H

#include "qwadro/exec/afxSystem.h"
#include "../exec/afxThreadingDDK.h"

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


    afxReqWorkerFn  reqJobWorkerFn; // Function called by the job manager when a new worker needs to be requested. When null, all jobs will be executed on the same thread that calls DoService().
    afxUnit          maxActiveWorkers; // The maximum number of concurrent workers that will be requested. Must be >= 1 for each job type.
    afxUnit          memSlabCnt; // Number of memory slabs to pre-allocate for job manager memory. At least one slab per worker thread should be pre-allocated. Default is 1.
    afxUnit          memSlabSiz; // Size of each memory slab used for job manager memory. Must be a power of two. Default is 8K.
    void*           udd; // Arbitrary data that will be passed back to the client when calling afxReqWorkerFn.
};
#endif//_AFX_SERVICE_C

AFX afxClassConfig const _AFX_SVC_CLASS_CONFIG;

#endif//AFX_SERVICE_DDK_H
