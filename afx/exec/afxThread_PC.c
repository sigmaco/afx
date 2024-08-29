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

#include "../dep/tinycthread.h"

#define _AFX_CORE_C
#define _AFX_THREAD_C
#include "../dev/afxDevCoreBase.h"

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
    //return GetCurrentThreadId();
    return _currTid ? _currTid : (_currTid = GetCurrentThreadId());
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

_AFX afxBool AfxIsPrimeThread(void)
{
#if 0
    afxThread curr, prime;
    return (AfxGetThread(&curr) && AfxGetPrimeThread(&prime) && (curr == prime));
#else
    return (AfxGetTid() == AfxGetPrimeTid());
#endif
}

_AFXINL void _AfxGetThreadFrequency(afxThread thr, afxNat* execNo, afxNat* lastFreq)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    AfxAssert(execNo);
    *execNo = thr->execNo;
    AfxAssert(lastFreq);
    *lastFreq = thr->lastExecCnt;
}

_AFX void AfxGetThreadFrequency(afxNat* execNo, afxNat* lastFreq)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lastFreq);
    AfxAssert(execNo);
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
    *ct = ct2;
    AfxAssert(dt);
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

_AFXINL afxBool _AfxShouldThreadBeInterrupted(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    afxBool rslt = FALSE;

    // Return true if the task running on this thread should be stopped. 
    // An interruption can be requested by RequestThreadInterruption().

    if (!(rslt = !!(thr->interruptionRequested)))
        rslt = (thr->running && (!thr->finished) && (thr->isInFinish));

    return rslt;
}

_AFX afxBool AfxShouldThreadBeInterrupted(void)
{
    afxError err = AFX_ERR_NONE;
    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return _AfxShouldThreadBeInterrupted(thr);
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetThreadExitCode(afxThread thr, afxInt* exitCode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    // Retrieves the context of the specified thread.

    AfxAssert(exitCode);
    *exitCode = thr->exitCode;
    return AfxIsThreadFinished(thr);
}

_AFX afxResult AfxWaitForThreadExit(afxThread thr, afxResult* exitCode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    afxResult rslt = FALSE;
    // The thread associated with this Thread object has finished execution (i.e. when it returns from run()). 
    // This function will return true if the thread has finished. 
    // It also returns true if the thread has not been started yet.
    
    afxThread curr;
    AfxGetThread(&curr);
    //AfxAssertObjects(1, &curr, afxFcc_THR);

    if (curr == thr) rslt = TRUE;
    else
    {
        while (AfxIsThreadRunning(thr))
        {
            AfxLockMutex(&thr->statusCndMtx);

            while (AfxIsThreadRunning(thr))
                AfxWaitCondition(&thr->statusCnd, &thr->statusCndMtx);

            AfxUnlockMutex(&thr->statusCndMtx);
        }
        rslt = TRUE;
        AfxAssert(exitCode);
        *exitCode = thr->exitCode;
    }
    return rslt;
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

_AFX void** AfxGetThreadUdd(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return thr->udd;
}

_AFX afxBool AfxIsThreadRunning(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return thr->running && !thr->isInFinish;
}

_AFX afxBool AfxIsThreadFinished(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return thr->finished || thr->isInFinish;
}

_AFX afxBool AfxPostEvent(afxThread thr, void(*event)(void* udd), void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return PostThreadMessageA(thr->tid, WM_USER, (WPARAM)event, (LPARAM)udd);
}

_AFX int _AfxExecTxuCb(void** v)
{
    afxError err = AFX_ERR_NONE;
    
    afxThread thr = v[0];
    AfxAssertObjects(1, &thr, afxFcc_THR);
    afxInt(*proc)(void* arg) = v[1];
    void* arg = v[2];

    thr->tid = AfxGetTid();
    //thr->osHandle = thrd_current();
    //_currTid = thr->tid;
    _currThr = thr;

    AfxAssert(!AfxIsPrimeThread());
    AfxSignalCondition2(&thr->statusCnd);

    AfxLockMutex(&thr->statusCndMtx);

    while (!thr->osHandle) // waiting for acquisitor to set the valid OS-side handle.
        AfxWaitCondition(&thr->statusCnd, &thr->statusCndMtx);

    AfxUnlockMutex(&thr->statusCndMtx);

    AfxDbgLogf(1, NIL, "Starting %u on thread execution unit #%u...", thr->tid, AfxGetObjectId(thr));

    // START ANY STATE

    _AfxInitMmu(thr);

#ifdef AFX_OS_WIN
    // When your thread starts running, it should post a message to itself first to create a message queue, then set a signal to indicate that it is ready to receive messages, then finally enter its message loop.
    PostThreadMessageA(thr->tid, WM_USER, NIL, NIL);
#endif

    // START THE WORK

    //afxInt(*f)(afxThread thr, void* udd);

    if (proc)
    {
        thr->exitCode = proc(arg);
    }
    else
    {
        for (;;)
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

            //AfxAssert(thr->procCb);

            if (thr->running && !thr->started && !thr->finished/* && AfxSystemIsExecuting()*/)
            {
                afxEvent ev2 = { 0 };
                ev2.siz = sizeof(ev2);
                ev2.id = afxThreadEvent_RUN;
             
                AfxSignalCondition2(&thr->statusCnd);
                thr->started = TRUE;

                AfxEmitEvent(thr, &ev2);
            }

            MSG msg;
            afxResult msgCnt = 0;
            while (PeekMessageA(&msg, NIL, 0, 0, PM_REMOVE | PM_NOYIELD))
            {
                ++msgCnt;

                if (msg.message == WM_USER)
                {
                    void(*event)(void* udd) = (void*)msg.wParam;

                    if (event)
                        event((void*)msg.lParam);
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }

            afxEvent* ev;
            while ((ev = AfxPeekQueue(&thr->events)))
            {
                AfxEmitEvent(thr, ev);
                AfxPopQueue(&thr->events);
            }

            if (_AfxShouldThreadBeInterrupted(thr))
            {
                afxEvent ev2 = { 0 };
                ev2.siz = sizeof(ev2);
                ev2.id = afxThreadEvent_QUIT;
                AfxEmitEvent(thr, &ev2);
            }

            if (AfxIsThreadFinished(thr) || thr->exited)
            {
                AfxDbgLogf(1, NIL, "Stopping %u on thread execution unit #%u...", thr->tid, AfxGetObjectId(thr));
                break;
            }
            AfxYield();
        }
    }

    _AfxDeinitMmu(thr);
    AfxSignalCondition2(&thr->statusCnd);
    thr->tid = 0;
    AfxSignalCondition2(&thr->statusCnd);
    return thr->exitCode;
}

_AFX afxError AfxRunThread(afxThread thr, afxInt(*proc)(void* arg), void* arg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    // Begins execution of the thread by calling run(). 
    // The operating system will schedule the thread according to the priority parameter. 
    // If the thread is already running, this function does nothing.
    // The effect of the priority parameter is dependent on the operating system's scheduling policy. 
    // In particular, the priority will be ignored on systems that do not support thread priorities (such as on Linux, see the sched_setscheduler documentation for more details).

    if (!AfxIsThreadRunning(thr))
    {
        //thr->started = TRUE;

        AfxGetClock(&thr->startClock);
        AfxCopyClock(&thr->lastClock, &thr->startClock);
        AfxCopyClock(&thr->execCntSwapClock, &thr->lastClock);
        thr->execNo = 0;

        thr->running = TRUE;
        thr->finished = FALSE;
        thr->exited = FALSE;
        thr->exitCode = 0;
        thr->interruptionRequested = FALSE;

        if (thr->tid == 0)
        {
            AfxAssert(!thr->osHandle);
            
            if (thrd_success != thrd_create((thrd_t)&thr->osHandle, (void*)_AfxExecTxuCb, (void*[]) { thr, proc, arg })) AfxThrowError();
            else
            {
                // we have to set the real OS handle from outside the thread proc.
                AfxAssert(thr->osHandle);
                AfxSignalCondition(&thr->statusCnd);
                AfxLockMutex(&thr->statusCndMtx);

                while (!thr->tid) // we have to wait for the thread register its own TID.
                    AfxWaitCondition(&thr->statusCnd, &thr->statusCndMtx);

                AfxUnlockMutex(&thr->statusCndMtx);
                AfxAssert(thr->tid);

                AfxAssert(GetThreadId(thr->osHandle) == thr->tid);
                AfxAssert(thr->osHandle == thr->osHandle);
            }
        }
    }
    return err;
}

_AFX afxError _AfxThrDtorCb(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    // Destroys the Thread.
    // Note that deleting a Thread object will not stop the execution of the thread it manages.Deleting a running Thread(i.e.isFinished() returns false) will result in a program crash.Wait for the finished() signal before deleting the QThread.
    // Since 6.3, it is allowed to delete a Thread instance created by a call to Thread::create() even if the corresponding thread is still running.In such a case, framework will post an interruption request to that thread(via requestInterruption()); will ask the thread's event loop (if any) to quit (via quit()); and will block until the thread has finished.
    
    afxResult exitCode;    
    do AfxRequestThreadInterruption(thr);
    while (!AfxWaitForThreadExit(thr, &exitCode));

    AfxAssert(AfxIsQueueEmpty(&thr->events));

    if (AfxCleanUpQueue(&thr->events))
        AfxThrowError();

    if (thr->osHandle)
    {
        AfxAssert(thr->tid);
        //AfxAssert(!thr->procCb);
        CloseHandle(thr->osHandle);
    }

    AfxCleanUpCondition(&thr->statusCnd);
    AfxCleanUpMutex(&thr->statusCndMtx);

    return err;
}

_AFX afxError _AfxThrCtorCb(afxThread thr, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    //AfxZero(thr, sizeof(thr[0]));

    afxSystem sys = args[0];
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxSize const* hint = args[1];
    afxThreadConfig const *cfg = ((afxThreadConfig const *)(args[2]));// +invokeNo; --- not specific
    AfxAssert(cfg);
    
    //thr->affineProcUnitIdx = spec ? spec->affineProcUnitIdx : AFX_INVALID_INDEX; // if not equal to AFX_INVALID_INDEX, this thread can be ran by any system processor unit, else case, will only be ran by the unit specified by this index.
    AfxResetClock(&thr->startClock);
    AfxResetClock(&thr->lastClock);
    AfxResetClock(&thr->execCntSwapClock);
    thr->execNo = 0;
    //thr->procCb = cfg->procCb;
    thr->started = FALSE;
    thr->running = FALSE;
    thr->finished = FALSE;
    thr->exited = FALSE;
    thr->isInFinish = FALSE;
    thr->interruptionRequested = FALSE;
    thr->exitCode = 0;

    AfxSetUpMutex(&thr->statusCndMtx, AFX_MTX_PLAIN);
    AfxSetUpCondition(&thr->statusCnd);

    if (cfg)
    {
        thr->udd[0] = cfg->udd[0];
        thr->udd[1] = cfg->udd[1];
        thr->udd[2] = cfg->udd[2];
        thr->udd[3] = cfg->udd[3];
    }
    else
    {
        thr->udd[0] = NIL;
        thr->udd[1] = NIL;
        thr->udd[2] = NIL;
        thr->udd[3] = NIL;
    }

    thr->_file_ = _AfxDbgTrimFilename((void*)hint[0]);
    thr->_line_ = hint[1];
    thr->_func_ = (void*)hint[2];

    thr->suspendCnt = 0;
    //thr->affineThrUnitIdx = AFX_INVALID_INDEX;

    //AfxSetThreadAffinity(thr, spec ? spec->affinityMask : NIL);

    // The new thread is not started -- it must be started by an explicit call to start().
    // This allows you to connect to its signals, move Objects to the thread, choose the new thread's priority and so on.
    // The function f will be called in the new thread.

    if (AfxSetUpQueue(&thr->events, sizeof(afxEvent), AfxMax(AFX_THR_MIN_EVENT_CAP, cfg->minEventCap))) AfxThrowError();
    else
    {
        //AfxDeploySlock(&thr->evSlock);
        //AfxDeployArena(NIL, &thr->evArena, NIL, AfxHere());

        //AfxDeployInterlockedQueue(&thr->events2, sizeof(afxPostedEvent), AFX_THR_MIN_EVENT_CAP);

        thr->tid = 0;

        if (cfg->tid != 0)
        {
            afxThread curr;

            if (AfxSystemIsExecuting() && AfxFindThread(cfg->tid, &curr)) AfxThrowError();
            else
            {
                void*osHandle2;

                if (!(osHandle2 = (void*)OpenThread(THREAD_QUERY_INFORMATION, NIL, cfg->tid))) AfxThrowError();
                else
                {
                    thr->osHandle = osHandle2;
                    thr->tid = cfg->tid;

                    if (thr->tid == AfxGetPrimeTid()) // is prime thread
                    {
                        _currTid = AfxGetTid();
                        _currThr = thr;
                        AfxAssert(AfxIsPrimeThread());
                        AfxRunThread(thr, NIL, NIL);
                    }
                }
            }
        }
    }

    if (err)
        AfxCleanUpQueue(&thr->events);

    return err;
}

_AFX afxClassConfig const _AfxThrMgrCfg =
{
    .fcc = afxFcc_THR,
    .name = "Thread",
    .desc = "Thread Execution Unit",
    .fixedSiz = sizeof(AFX_OBJECT(afxThread)),
    .ctor = (void*)_AfxThrCtorCb,
    .dtor = (void*)_AfxThrDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxNat AfxInvokeThreads(afxNat first, afxNat cnt, afxBool(*f)(afxThread, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(cnt);
    afxClass* cls = AfxGetThreadClass();
    AfxAssertClass(cls, afxFcc_THR);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateThreads(afxNat first, afxNat cnt, afxThread threads[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(threads);
    AfxAssert(cnt);
    afxClass* cls = AfxGetThreadClass();
    AfxAssertClass(cls, afxFcc_THR);
    afxNat rslt = AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)threads);
    AfxAssertObjects(rslt, threads, afxFcc_THR);
    return rslt;
}

_AFX afxError AfxAcquireThreads(afxHere const hint, afxThreadConfig const* cfg, afxNat cnt, afxThread threads[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(threads);
    AfxAssert(hint);
    AfxAssert(cfg);

    AfxLogEcho("Acquiring thread...  %u \"%s:%i\"", cfg->tid, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1]);

    // Creates a new Thread object that will execute the function f with the arguments args.
    // The new thread is not started -- it must be started by an explicit call to start(). This allows you to connect to its signals, move Objects to the thread, choose the new thread's priority and so on. The function f will be called in the new thread.

    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass* cls = AfxGetThreadClass();
    AfxAssertClass(cls, afxFcc_THR);
    
    if (AfxAcquireObjects(cls, cnt, (afxObject*)threads, (void const*[]) { sys, hint, (void*)cfg, })) AfxThrowError();
    else
    {
        AfxAssertObjects(cnt, threads, afxFcc_THR);        
    }
    return err;
}
