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

#define _AFX_CORE_C
#define _AFX_SERVICE_C
#include "../src/afx/dev/afxDevCoreBase.h"

_AFX afxError AfxDoService(afxService svc, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &svc, afxFcc_SVC);

    return err;
}

_AFX afxResult JobWorkerThread(afxThread thr, afxEvent* ev)
{
    // Sleep until signaled the first time
    //AfxWaitForSemaphore(g_semaphore);

    //if (g_fnJobWorker)
    {
        //while (workerState.keepAlive)
        {
            afxJobType jobType;
            {
                afxInt64 endSpinTime = 0;

                //while (!g_jobTypeQueue.Dequeue(jobType))
                {
                    // if the dequeue failed, just try again in a light spin. There may be multiple enqueues happening,
                    // and we were woken up by a later enqueue, and another enqueue is not yet done its writing.
                    if (endSpinTime == 0)
                    {
                        //PerformanceCounter(&endSpinTime);
                        //endSpinTime += (afxInt64)(g_fFreqRatio * 0.01); // only spin for about 10us
                    }

                    // wait a bit or then yield to another thread
                    afxInt64 currentTime = 0;
                    //PerformanceCounter(&currentTime);

                    if (currentTime < endSpinTime)
                    {
                        //AkThreadYield();
                    }
                }
            }

            //g_fnJobWorker(jobType, workerState.uExecutionTimeUSec);

            // Go back to sleep until signaled for new work
            //AfxWaitForSemaphore(g_semaphore);
        }
    }
    //AfxExitThread(0);
}

_AFX void RequestJobWorker(afxWorkerFn in_fnJobWorker, afxJobType jobType, afxNat workerCnt, void* in_pClientData)
{
    //g_fnJobWorker = in_fnJobWorker;

    for (int i = 0; i < (int)workerCnt; i++)
    {
        //while (!AfxPushQueue2(&fifo, jobType))
        {
            // if the job could not be enqueued, then yield the thread so that someone else can clear up the queue, and try again
            AfxYield();
        }
    }
    //AfxReleaseSemaphore(g_semaphore, workerCnt);
}

_AFX afxError _AfxSvcDtor(afxService svc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &svc, afxFcc_SVC);

    if (svc->workers)
    {
        // Mark all of the threads as inactive
        for (afxNat i = 0; i < svc->workerThrCnt; ++i)
            svc->workers[i].keepAlive = FALSE;

        // signal all of the them to wake-up to process said inactivity
        //AfxReleaseSemaphore(g_semaphore, svc->workerThrCnt);

        // and then wait for them to gracefully close
        for (afxNat i = 0; i < svc->workerThrCnt; ++i)
        {
            afxResult exitCode;
            AfxWaitForThreadExit(svc->workers[i].thr, &exitCode);
            AfxReleaseObjects(1, (void*)&svc->workers[i].thr);
        }

        AfxDeallocate(svc->workers);
        svc->workers = NIL;
    }
    AfxCleanUpInterlockedQueue(&svc->jobTypeQue);
    svc->running = FALSE;
    return err;
}

_AFX afxError _AfxSvcCtor(afxService svc, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &svc, afxFcc_SVC);

    afxServiceConfig const *config = ((afxServiceConfig const *)cookie->udd[1]) + cookie->no;
    afxNat workerThrCnt = 1;
    afxNat execTimeUsec = 0;

    // Entries in the queue must be a power of 2
    afxNat uNumEntries = 2;
    while (uNumEntries < workerThrCnt * AFX_NUM_JOB_TYPES)
    {
        uNumEntries <<= 1;
    }
    
    if (AfxSetUpInterlockedQueue(&svc->jobTypeQue, sizeof(afxJobType), uNumEntries)) AfxThrowError();
    else
    {
        if (!(svc->workers = AfxAllocate(workerThrCnt, sizeof(svc->workers[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            AfxZero2(workerThrCnt, sizeof(svc->workers[0]), svc->workers);
            
            svc->execTimeUsec = execTimeUsec;
            svc->workerThrCnt = workerThrCnt;

            for (afxNat workerIdx = 0; workerIdx < workerThrCnt; workerIdx++)
            {
                svc->workers[workerIdx].execTimeUsec = execTimeUsec;
                svc->workers[workerIdx].keepAlive = TRUE;
                svc->workers[workerIdx].workerIdx = workerIdx;

                afxThreadConfig thrCfg = { 0 };
                thrCfg.procCb = JobWorkerThread;

                if (AfxAcquireThread(AfxHere(), &thrCfg, &svc->workers[workerIdx].thr))
                {
                    AfxThrowError();
                }
            }
            svc->running = TRUE;
        }
    }
    return err;
}

_AFX afxClassConfig const _AfxSvcMgrCfg =
{
    .fcc = afxFcc_SVC,
    .name = "Service",
    .desc = "Autonomous Service",
    .fixedSiz = sizeof(AFX_OBJECT(afxService)),
    .ctor = (void*)_AfxSvcCtor,
    .dtor = (void*)_AfxSvcDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireServices(afxNat cnt, afxServiceConfig const config[], afxService services[])
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetServiceClass();
    AfxAssertClass(cls, afxFcc_SVC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)services, (void const*[]) { NIL, (void*)config }))
        AfxThrowError();

    AfxAssertObjects(cnt, services, afxFcc_SVC);

    return err;
}
