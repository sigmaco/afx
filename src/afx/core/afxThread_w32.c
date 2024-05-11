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

#include "tinycthread.h"

#define _AFX_CORE_C
#define _AFX_THREAD_C
#include "qwadro/core/afxSystem.h"

_AFX afxNat32 _primeTid = NIL;
_AFX afxThread _primeThr = NIL;
AFX_THREAD_LOCAL static afxNat32 _currTid = 0;
AFX_THREAD_LOCAL static afxThread _currThr = NIL;

_AFX afxNat32 AfxGetTid(void)
{
#if defined(_WIN32)
    afxError err = NIL;
    AfxAssert(!_currTid || (_currTid == GetCurrentThreadId()));
#else
#error "";
#endif
    //return _currTid;
    return GetCurrentThreadId();
}

_AFX void AfxYield(void)
{
    thrd_yield();
}

struct _thrFindThreadParam
{
    afxNat32 tid;
    afxThread thr;
};

_AFXINL afxBool _ThrFindThreadCb(afxThread thr, void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    struct _thrFindThreadParam* param = udd;

    if (param->tid == thr->tid)
    {
        param->thr = thr;
        return TRUE; // interrupt
    }
    return FALSE; // continue
}

_AFX afxBool AfxFindThread(afxNat32 tid, afxThread* thread)
{
    afxError err = AFX_ERR_NONE;
    struct _thrFindThreadParam param = { .thr = NIL,.tid = tid };

    if (tid)
    {
        AfxInvokeThreads(0, AFX_N32_MAX, _ThrFindThreadCb, &param);
        AfxTryAssertObjects(1, &param.thr, afxFcc_THR);
    }

    AfxAssert(thread);
    *thread = param.thr ? param.thr : NIL;
    return !!param.thr;
}

_AFX afxBool AfxGetThread(afxThread* thread)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(thread);
#if 0
    *thread = NIL;
    afxNat32 tid = AfxGetTid();
    return AfxFindThread(tid, thread);
#else
    *thread = _currThr;
    return !!_currThr;
#endif
}

_AFX afxBool AfxGetPrimeThread(afxThread* thread)
{
    afxError err = AFX_ERR_NONE;
    afxThread thr = _primeThr;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    *thread = thr;
    return !!thr;
}

_AFX afxBool AfxIsPrimeThread(void)
{
#if 0
    afxThread curr, prime;
    return (AfxGetThread(&curr) && AfxGetPrimeThread(&prime) && (curr == prime));
#else
    return (_currTid == _primeTid);
#endif
}

_AFXINL void _AfxGetThreadFrequency(afxThread thr, afxNat* execNo, afxNat* lastFreq)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    AfxAssert(execNo);
    AfxAssert(lastFreq);
    *execNo = thr->execNo;
    *lastFreq = thr->lastExecCnt;
}

_AFX void AfxGetThreadFrequency(afxNat* execNo, afxNat* lastFreq)
{
    afxError err = AFX_ERR_NONE;
    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);
    _AfxGetThreadFrequency(thr, execNo, lastFreq);
}

_AFXINL void _AfxGetThreadClock(afxThread thr, afxClock* curr, afxClock* last)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    afxClock currClock;
    AfxGetClock(&currClock);
    AfxAssert(curr);
    AfxAssert(last);
    *curr = currClock;
    *last = thr->lastClock;
    thr->lastClock = currClock;
}

_AFX void AfxGetThreadClock(afxClock* curr, afxClock* last)
{
    afxError err = AFX_ERR_NONE;
    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);
    _AfxGetThreadClock(thr, curr, last);
}

_AFXINL void _AfxGetThreadTime(afxThread thr, afxReal64* ct, afxReal64* dt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    afxClock currClock;
    AfxGetClock(&currClock);
    afxReal64 ct2 = AfxGetSecondsElapsed(&thr->startClock, &currClock);
    afxReal64 dt2 = AfxGetSecondsElapsed(&thr->lastClock, &currClock);
    thr->lastClock = currClock;
    AfxAssert(ct);
    AfxAssert(dt);
    *ct = ct2;
    *dt = dt2;
}
_AFX void AfxGetThreadTime(afxReal64* ct, afxReal64* dt)
{
    afxError err = AFX_ERR_NONE;
    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);
    _AfxGetThreadTime(thr, ct, dt);
}

_AFXINL void _AfxExitThread(afxThread thr, afxInt code)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    // Tells the thread's event loop to exit with a return code.
    // After calling this function, the thread leaves the event loop and returns from the call to exec(). The exec() function returns returnCode.
    // By convention, a returnCode of 0 means success, any non - zero value indicates an error.
    // Note that unlike the C library function of the same name, this function does return to the caller -- it is event processing that stops.
    // No EventLoops will be started anymore in this thread until exec() has been called again.If the eventloop in exec() is not running then the next call to exec() will also return immediately.

    thr->exitCode = code;
    thr->exited = TRUE;
    thr->running = FALSE;
}

_AFX void AfxExitThread(afxInt code)
{
    afxError err = AFX_ERR_NONE;
    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);
    _AfxExitThread(thr, code);
}

_AFXINL void _AfxQuitThread(afxThread thr)
{
    // Tells the thread's event loop to exit with return code 0 (success). Equivalent to calling exit(0).
    // This function does nothing if the thread does not have an event loop.

    _AfxExitThread(thr, 0);
}

_AFX void AfxQuitThread(void)
{
    afxError err = AFX_ERR_NONE;
    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);
    _AfxQuitThread(thr);
}

_AFXINL afxBool _AfxThreadShouldBeInterrupted(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    afxBool rslt = FALSE;

    // Return true if the task running on this thread should be stopped. An interruption can be requested by RequestThreadInterruption().

    if (!(rslt = !!(thr->interruptionRequested)))
        rslt = (thr->running && (!thr->finished) && (thr->isInFinish));

    return rslt;
}

_AFX afxBool AfxThreadShouldBeInterrupted(void)
{
    afxError err = AFX_ERR_NONE;
    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return _AfxThreadShouldBeInterrupted(thr);
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxResult AfxWaitForThread(afxThread thr, afxResult* exitCode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    // The thread associated with this Thread object has finished execution (i.e. when it returns from run()). This function will return true if the thread has finished. It also returns true if the thread has not been started yet.

    if (!thr->started || thr->finished)
    {
        AfxAssert(!thr->running);
        return TRUE;
    }
    else
    {
        while (thr->running)
        {
            AfxYield();
        }
        return TRUE;
    }
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

_AFX afxBool _AfxThrExecuteCb(afxThread thr, void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    //AfxDoSystemExecution(0);

    afxThread curr;
    AfxGetThread(&curr);
    AfxAssert(thr == curr);

    if (thr == curr)
    {
        if (thr->running)
        {
            afxClock currClock, lastClock;
            AfxGetThreadClock(&currClock, &lastClock);

            if (1.0 > AfxGetSecondsElapsed(&thr->execCntSwapClock, &currClock)) ++thr->execNo;
            else
            {
                thr->execCntSwapClock = currClock;
                thr->lastExecCnt = thr->execNo;
                thr->execNo = 0;
            }

            AfxAssert(thr->procCb);

            if (thr->running && !thr->started && !thr->finished && AfxSystemIsExecuting())
            {
                afxEvent ev2 = { 0 };
                ev2.id = afxThreadEvent_RUN;
                thr->procCb(thr, &ev2);
                thr->started = TRUE;
            }

            afxEvent* ev;

            if ((ev = AfxPullNextQueueUnit(&thr->events)))
            {
                thr->procCb(thr, ev);
                AfxPopNextQueue(&thr->events);
            }
            else
            {
                afxEvent ev2 = { 0 };
                ev2.id = NIL;
                thr->procCb(thr, &ev2);
            }

            if (thr->interruptionRequested)
            {
                afxEvent ev2 = { 0 };
                ev2.id = afxThreadEvent_QUIT;
                thr->procCb(thr, &ev2);
            }
        }
    }
    return FALSE; // dont interrupt curation
}

_AFX afxBool AfxPostEvent(afxThread thr, void(*event)(void* udd), void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return PostThreadMessageA(thr->tid, WM_USER, (WPARAM)event, (LPARAM)udd);
}

_AFX int startCbOnSysProcUnit(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    thr->tid = AfxGetTid();
    thr->osHandle = thrd_current();
    _currTid = thr->tid;
    _currThr = thr;
    AfxAssert(!AfxIsPrimeThread());
    
    _AfxInitMmu(thr);

#ifdef AFX_OS_WIN
    // When your thread starts running, it should post a message to itself first to create a message queue, then set a signal to indicate that it is ready to receive messages, then finally enter its message loop.
    PostThreadMessageA(thr->tid, WM_USER, NIL, NIL);
#endif

    AfxYield();

    AfxDbgLogf(1, NIL, "Starting Thread Execution Unit %u", thr->tid);

    afxBool again = TRUE;

    do
    {
        MSG msg;
        afxResult msgCnt = 0;
        while (PeekMessageA(&(msg), NIL, 0, 0, PM_REMOVE/* | PM_NOYIELD*/))
        {
            ++msgCnt;

            if (msg.message == WM_QUIT) // PostQuitMessage()
            {
                int a = 1;
            }
            else if (msg.message == WM_USER)
            {
                void(*event)(void* udd) = (void*)msg.wParam;

                if (event)
                    event((void*)msg.lParam);
            }
            else
            {
                if (msg.message == WM_INPUT || msg.message == WM_INPUT_DEVICE_CHANGE)
                {
                    int a = 1;
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }

        if (!!(thr->interruptionRequested))
            again = FALSE;

        _AfxThrExecuteCb(thr, 0);

        if (!again)
            AfxExitThread(0);

        AfxYield();
    } while (again);

    AfxDbgLogf(1, NIL, "Stopping Thread Execution Unit %u", thr->tid);

    _AfxDeinitMmu(thr);

    thr->osHandle = NIL;
    thr->tid = 0;

    return thr->exitCode;
}

_AFX void AfxRunThread(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    // Begins execution of the thread by calling run(). The operating system will schedule the thread according to the priority parameter. If the thread is already running, this function does nothing.
    // The effect of the priority parameter is dependent on the operating system's scheduling policy. In particular, the priority will be ignored on systems that do not support thread priorities (such as on Linux, see the sched_setscheduler documentation for more details).

    if (thr->running)
    {
        _AfxThrExecuteCb(thr, 0);
    }
    else
    {
        //thr->started = TRUE;

        AfxGetClock(&thr->startClock);
        thr->lastClock = thr->startClock;
        thr->execCntSwapClock = thr->lastClock;
        thr->execNo = 0;

        thr->running = TRUE;
        thr->finished = FALSE;
        thr->exited = FALSE;
        thr->exitCode = 0;
        thr->interruptionRequested = FALSE;

        if (thr->tid == 0)
        {
            AfxAssert(!thr->osHandle);
            thrd_t thrd = NIL;

            if (thrd_success != thrd_create(&thrd, (void*)startCbOnSysProcUnit, thr)) AfxThrowError();
            else
            {
                do
                {
                    AfxYield();
                } while (!thr->tid);

                AfxAssert(thr->tid);
                AfxAssert(thr->osHandle);
            }
        }
    }
}

_AFX afxError _AfxThrDtor(afxThread thr)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &thr, afxFcc_THR);

    // Destroys the Thread.
    // Note that deleting a Thread object will not stop the execution of the thread it manages.Deleting a running Thread(i.e.isFinished() returns false) will result in a program crash.Wait for the finished() signal before deleting the QThread.
    // Since 6.3, it is allowed to delete a Thread instance created by a call to Thread::create() even if the corresponding thread is still running.In such a case, framework will post an interruption request to that thread(via requestInterruption()); will ask the thread's event loop (if any) to quit (via quit()); and will block until the thread has finished.
    
    afxResult exitCode;
    AfxRequestThreadInterruption(thr);
    AfxWaitForThread(thr, &exitCode);

    AfxAssert(AfxQueueIsEmpty(&thr->events));

    if (AfxDeallocateQueue(&thr->events))
        AfxThrowError();

    if (thr->osHandle)
    {
        AfxAssert(thr->tid);
        AfxAssert(!thr->procCb);
        CloseHandle(thr->osHandle);
    }

    return err;
}

_AFX afxError _AfxThrCtor(afxThread thr, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    AfxZero(thr, sizeof(thr[0]));

    afxSize const* hint = cookie->udd[1];
    afxThreadConfig const *cfg = ((afxThreadConfig const *)cookie->udd[2]) + cookie->no;
    AfxAssert(cfg);
    
    //thr->affineProcUnitIdx = spec ? spec->affineProcUnitIdx : AFX_INVALID_INDEX; // if not equal to AFX_INVALID_INDEX, this thread can be ran by any system processor unit, else case, will only be ran by the unit specified by this index.
    thr->startClock = (afxClock) { 0 };
    thr->lastClock = (afxClock) { 0 };
    thr->execCntSwapClock = (afxClock) { 0 };
    thr->execNo = 0;
    thr->procCb = cfg ? cfg->procCb : NIL;
    thr->started = FALSE;
    thr->running = FALSE;
    thr->finished = FALSE;
    thr->exited = FALSE;
    thr->isInFinish = FALSE;
    thr->interruptionRequested = FALSE;
    thr->exitCode = 0;
    thr->udd = cfg ? cfg->udd : NIL;

    thr->_file_ = AfxFindPathTarget((void*)hint[0]);
    thr->_line_ = hint[1];
    thr->_func_ = (void*)hint[2];

    thr->suspendCnt = 0;
    //thr->affineThrUnitIdx = AFX_INVALID_INDEX;

    //AfxSetThreadAffinity(thr, spec ? spec->affinityMask : NIL);

    // The new thread is not started -- it must be started by an explicit call to start().
    // This allows you to connect to its signals, move Objects to the thread, choose the new thread's priority and so on.
    // The function f will be called in the new thread.

    if (AfxAllocateQueue(&thr->events, sizeof(afxEvent), AfxMax(AFX_THR_MIN_EVENT_CAP, cfg->minEventCap))) AfxThrowError();
    else
    {
        //AfxSetUpSlock(&thr->evSlock);
        //AfxAllocateArena(NIL, &thr->evArena, NIL, AfxHere());

        //AfxSetUpFifo(&thr->events2, sizeof(afxPostedEvent), AFX_THR_MIN_EVENT_CAP);

        thr->tid = 0;

        if (cfg->tid != 0)
        {
            afxThread curr;

            if (AfxSystemIsExecuting() && AfxFindThread(cfg->tid, &curr)) AfxThrowError();
            else
            {
                if (!(thr->osHandle = (void*)OpenThread(THREAD_QUERY_INFORMATION, NIL, cfg->tid))) AfxThrowError();
                else
                {
                    thr->tid = cfg->tid;
                    thr->procCb = NIL;

                    if (thr->tid == _primeTid) // is prime thread
                    {
                        _currTid = _primeTid;
                        _currThr = thr;
                        AfxAssert(AfxIsPrimeThread());
                    }
                }
            }
        }
    }

    if (err)
        AfxDeallocateQueue(&thr->events);

    return err;
}

_AFX afxClassConfig const _AfxThrMgrCfg =
{
    .fcc = afxFcc_THR,
    .name = "Thread",
    .desc = "Thread Execution Unit",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxThread)),
    .mmu = NIL,
    .ctor = (void*)_AfxThrCtor,
    .dtor = (void*)_AfxThrDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireThread(afxHere const hint, afxThreadConfig const* cfg, afxThread* thread)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(thread);
    AfxAssert(hint);
    AfxAssert(cfg);

    AfxLogEcho("Acquiring thread...  %u \"%s:%i\"", cfg->tid, AfxFindPathTarget((char const *const)hint[0]), (int)hint[1]);

    // Creates a new Thread object that will execute the function f with the arguments args.
    // The new thread is not started -- it must be started by an explicit call to start(). This allows you to connect to its signals, move Objects to the thread, choose the new thread's priority and so on. The function f will be called in the new thread.

    afxManager* cls = AfxGetThreadClass();
    AfxAssertClass(cls, afxFcc_THR);
    afxThread thr;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&thr, (void const*[]) { NIL, hint, (void*)cfg, })) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &thr, afxFcc_THR);
        AfxAssert(thread);
        *thread = thr;
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
    afxNat rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)threads);
    AfxAssertObjects(rslt, threads, afxFcc_THR);
    return rslt;
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
