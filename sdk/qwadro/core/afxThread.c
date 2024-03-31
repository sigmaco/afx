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

#define _AFX_CORE_C
#define _AFX_THREAD_C
#include "qwadro/core/afxSystem.h"

_AFX afxResult AfxWaitForThread(afxThread thr, afxResult* exitCode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    if (!thr->started || thr->finished)
    {
        // The thread associated with this Thread object has finished execution (i.e. when it returns from run()). This function will return true if the thread has finished. It also returns true if the thread has not been started yet.
        AfxAssert(!thr->running);
        return TRUE;
    }
    else
    {
        while (thr->running)
        {
            AfxYieldThreading();
        }
        return TRUE;
    }
  AfxLogEcho("Joined. (thr)%p", thr);
  return 0;
}

_AFX afxBool AfxGetThreadExitCode(afxThread thr, afxInt* exitCode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    // Retrieves the context of the specified thread.

    AfxAssert(exitCode);
    *exitCode = thr->exitCode;
    return AfxThreadIsFinished(thr);
}

_AFX void AfxRequestThreadInterruption(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    //void requestInterruption()
    //Request the interruption of the thread.That request is advisory and it is up to code running on the thread to decide if and how it should act upon such request. 
    // This function does not stop any event loop running on the thread and does not terminate it in any way.

    if (!thr->running || thr->finished || thr->isInFinish)
    {
        // nothing to do
    }
    else
    {
        thr->interruptionRequested = TRUE;
    }
}

_AFX void* AfxGetThreadUdd(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return thr->udd;
}

_AFX afxBool AfxThreadIsRunning(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return thr->running && !thr->isInFinish;
}

_AFX afxBool AfxThreadIsFinished(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return thr->finished || thr->isInFinish;
}

_AFX afxNat AfxSuspendThread(afxThread thr)
{
    /*
        If the function succeeds, execution of the specified thread is suspended and the thread's suspend count is incremented. 
        Suspending a thread causes the thread to stop executing user-mode (application) code.

        This function is primarily designed for use by debuggers. 
        It is not intended to be used for thread synchronization. 
        Calling SuspendThread on a thread that owns a synchronization object, such as a mutex or critical section, 
        can lead to a deadlock if the calling thread tries to obtain a synchronization object owned by a suspended thread. 
        To avoid this situation, a thread within an application that is not a debugger should signal the other thread to suspend itself. 
        The target thread must be designed to watch for this signal and respond appropriately.

        Each thread has a suspend count (with a maximum value of MAXIMUM_SUSPEND_COUNT). 
        If the suspend count is greater than zero, the thread is suspended; otherwise, the thread is not suspended and is eligible for execution. 
        Calling SuspendThread causes the target thread's suspend count to be incremented. 
        Attempting to increment past the maximum suspend count causes an error without incrementing the count.

        The ResumeThread function decrements the suspend count of a suspended thread.
    */
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    afxNat cnt = thr->suspendCnt;

    ++thr->suspendCnt;

    return cnt;
}

_AFX afxNat AfxResumeThread(afxThread thr)
{
    /*
        The ResumeThread function checks the suspend count of the subject thread. 
        If the suspend count is zero, the thread is not currently suspended. 
        Otherwise, the subject thread's suspend count is decremented. 
        If the resulting value is zero, then the execution of the subject thread is resumed.

        If the return value is zero, the specified thread was not suspended. 
        If the return value is 1, the specified thread was suspended but was restarted. 
        If the return value is greater than 1, the specified thread is still suspended.
    */
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    afxNat cnt = thr->suspendCnt;

    if (cnt)
        --thr->suspendCnt;

    return cnt;
}

#if 0
_AFX int __stdcall startCbOnThread(afxThread thr)
{
    thr->tid = GetCurrentThreadId();
    thr->osHandle = (afxSize)GetCurrentThread();

    _AfxThrRun(thr);

    return thr->exitCode;
}
#endif

_AFX void AfxRunThreads(afxNat cnt, afxThread threads[])
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxThread thr = threads[i];
        AfxAssertObjects(1, &thr, afxFcc_THR);

        // Begins execution of the thread by calling run(). The operating system will schedule the thread according to the priority parameter. If the thread is already running, this function does nothing.
        // The effect of the priority parameter is dependent on the operating system's scheduling policy. In particular, the priority will be ignored on systems that do not support thread priorities (such as on Linux, see the sched_setscheduler documentation for more details).

        if (!thr->running)
        {
            //thr->started = TRUE;

            AfxGetClock(&thr->startClock);
            thr->lastClock = thr->startClock;
            //thr->currClock = thr->startClock;
            thr->iterCntSwapClock = thr->lastClock;
            thr->iterNo = 0;
            //thr->currTime = 0;
            //thr->deltaTime = 0;

            thr->running = TRUE;
            thr->finished = FALSE;
            thr->exited = FALSE;
            thr->exitCode = 0;
            thr->interruptionRequested = FALSE;

            //AfxExecuteThread(thr); // opportunistic attempt to run immediately, if not blocked by affinity or other factor.
        }
    }
}

_AFX void AfxRunThread(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    AfxRunThreads(1, &thr);
}

_AFX afxError _AfxThrDtor(afxThread thr)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &thr, afxFcc_THR);

    // Destroys the Thread.
    // Note that deleting a Thread object will not stop the execution of the thread it manages.Deleting a running Thread(i.e.isFinished() returns false) will result in a program crash.Wait for the finished() signal before deleting the QThread.
    // Since 6.3, it is allowed to delete a Thread instance created by a call to Thread::create() even if the corresponding thread is still running.In such a case, framework will post an interruption request to that thread(via requestInterruption()); will ask the thread's event loop (if any) to quit (via quit()); and will block until the thread has finished.

    while (thr->running)
    {
        AfxRequestThreadInterruption(thr);
        AfxDoSystemExecution(0);
        //AfxYieldThreading();
        //afxResult exitCode = 0;
        //AfxWaitForThread(thr, &exitCode);
    }

#if 0
    AfxAssert(AfxQueueIsEmpty(&thr->events));

    if (AfxDeallocateQueue(&thr->events))
        AfxThrowError();
#endif

    _AfxReleaseTxus(1, &thr);
    AfxAssert(AfxChainIsEmpty(&thr->txus));

    return err;
}

_AFX afxError _AfxThrCtor(afxThread thr, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    afxThreadConfig const *config = ((afxThreadConfig const *)cookie->udd[0]) + cookie->no;
    
    if (AfxSetUpSlock(&thr->txuSlock)) AfxThrowError();
    else
    {
        //thr->affineProcUnitIdx = spec ? spec->affineProcUnitIdx : AFX_INVALID_INDEX; // if not equal to AFX_INVALID_INDEX, this thread can be ran by any system processor unit, else case, will only be ran by the unit specified by this index.
        thr->startClock = (afxClock) { 0 };
        //thr->currClock = (afxClock) { 0 };
        thr->lastClock = (afxClock) { 0 };
        thr->iterCntSwapClock = (afxClock) { 0 };
        thr->iterNo = 0;
        //thr->currTime = 0;
        //thr->deltaTime = 0;
        thr->proc = config ? config->proc : NIL;
        thr->started = FALSE;
        thr->running = FALSE;
        thr->finished = FALSE;
        thr->exited = FALSE;
        thr->isInFinish = FALSE;
        thr->interruptionRequested = FALSE;
        thr->exitCode = 0;
        thr->udd = config ? config->udd : NIL;

        thr->suspendCnt = 0;
        //thr->affineThrUnitIdx = AFX_INVALID_INDEX;

        thr->baseTxu = config->baseTxu;
        thr->txuCnt = AfxMax(1, config->txuCnt);
        AfxSetUpChain(&thr->txus, thr);

        //AfxSetThreadAffinity(thr, spec ? spec->affinityMask : NIL);

        // The new thread is not started -- it must be started by an explicit call to start().
        // This allows you to connect to its signals, move Objects to the thread, choose the new thread's priority and so on.
        // The function f will be called in the new thread.

#if 0
        if (AfxAllocateQueue(&thr->events, sizeof(afxEvent), 32)) AfxThrowError();
        else
        {

        }

        if (err)
            AfxDeallocateQueue(&thr->events);
#endif
        if (err)
            AfxCleanUpSlock(&thr->txuSlock);
    }
    return err;
}

_AFX afxClassConfig const _AfxThrClsConfig =
{
    .fcc = afxFcc_THR,
    .name = "Thread",
    .desc = "Threading",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxThread)),
    .mmu = NIL,
    .ctor = (void*)_AfxThrCtor,
    .dtor = (void*)_AfxThrDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireThreads(afxThreadConfig const* cfg, afxHint const hint, afxNat cnt, afxThread threads[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(threads);
    AfxAssert(hint);
    AfxAssert(cfg);
    AfxAssert(cnt);

    AfxLogEcho("Acquiring %u threads...  [%u, %u] \"%s:%i\"", cnt, cfg->baseTxu, cfg->txuCnt, AfxFindPathTarget((char const *const)hint[0]), (int)hint[1]);

    // Creates a new Thread object that will execute the function f with the arguments args.
    // The new thread is not started -- it must be started by an explicit call to start(). This allows you to connect to its signals, move Objects to the thread, choose the new thread's priority and so on. The function f will be called in the new thread.

    afxManager* cls = AfxGetThreadClass();
    AfxAssertClass(cls, afxFcc_THR);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)threads, (void const*[]) { (void*)cfg, hint })) AfxThrowError();
    else
    {
        if (_AfxAcquireTxus(cnt, threads))
        {
            AfxThrowError();
            AfxReleaseObjects(cnt, (void**)threads);
        }
    }
    return err;
}

_AFX afxNat AfxEnumerateThreads(afxNat first, afxNat cnt, afxThread threads[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(threads);
    AfxAssert(cnt);
    afxManager* cls = AfxGetThreadClass();
    AfxAssertClass(cls, afxFcc_THR);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)threads);
}

_AFX afxNat AfxInvokeThreads(afxNat first, afxNat cnt, afxBool(*f)(afxThread, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(cnt);
    afxManager* cls = AfxGetThreadClass();
    AfxAssertClass(cls, afxFcc_THR);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxCountThreads(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetThreadClass();
    AfxAssertClass(cls, afxFcc_THR);
    return AfxCountObjects(cls);
}
