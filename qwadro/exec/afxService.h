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

// The autonomous service interface for Qwadro.

#ifndef AFX_SERVICE_H
#define AFX_SERVICE_H

#include "qwadro/io/afxStream.h"
#include "qwadro/mem/afxInterlockedQueue.h"

typedef enum afxJobType
{
    afxJobType_
} afxJobType;

typedef void(*afxWorkerFn) (afxJobType typ, afxUnit execTimeUsec);

/// Callback function prototype definition used for handling requests from service for new workers to perform work.
typedef void(*afxReqWorkerFn)(afxWorkerFn workerFn, afxJobType typ, afxUnit workerCnt, void* udd);

AFX_DEFINE_STRUCT(afxServiceConfig)
{
    afxReqWorkerFn  reqJobWorkerFn; // Function called by the job manager when a new worker needs to be requested. When null, all jobs will be executed on the same thread that calls DoService().
    afxUnit         maxActiveWorkers; // The maximum number of concurrent workers that will be requested. Must be >= 1 for each job type.
    afxUnit         memSlabCnt; // Number of memory slabs to pre-allocate for job manager memory. At least one slab per worker thread should be pre-allocated. Default is 1.
    afxUnit         memSlabSiz; // Size of each memory slab used for job manager memory. Must be a power of two. Default is 8K.
    void*           udd; // Arbitrary data that will be passed back to the client when calling afxReqWorkerFn.
};

AFX afxError        AfxDoService(afxService svc, afxTime timeout);

////////////////////////////////////////////////////////////////////////////////

AFX afxError        AfxAcquireServices(afxUnit cnt, afxServiceConfig const config[], afxService services[]);

#endif//AFX_SERVICE_H
