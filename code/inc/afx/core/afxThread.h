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

#ifndef AFX_THREAD_H
#define AFX_THREAD_H

#include "afx/core/afxInstance.h"
#include "afx/core/afxQueue.h"
#include "afx/core/afxTime.h"
#include "afx/core/afxContext.h"
#include "afx/core/afxClock.h"
#include "afx/core/afxSlock.h"

// The afxThread class provides a platform-independent way to manage threads.

// A afxThread object manages one thread of control within the program. afxThreads begin executing in run(). By default, run() starts the event loop by calling exec() and runs a Qt event loop inside the thread.

typedef enum afxThreadEvent
{
    AFX_EVENT_THR_EXIT,
    AFX_EVENT_THR_QUIT,
    AFX_EVENT_THR_START,
    AFX_EVENT_THR_TERMINATE,
} afxThreadEvent;

typedef enum afxThreadOpcode
{
    //AFX_THR_OPCODE_NOP = 0,
    AFX_THR_OPCODE_RUN = 1,
    AFX_THR_OPCODE_QUIT,
} afxThreadOpcode;

AFX_DEFINE_STRUCT(afxThreadConfig)
{
    afxError        (*proc)(afxThread thr, void *udd, afxInt opcode);
    void            *udd;
};

#ifdef _AFX_CORE_C
#ifdef _AFX_PROCESSOR_C
AFX_DEFINE_STRUCT(afxProcessor)
{
    afxNat          unitIdx;
    afxNat          tid;
    void*           osHandle;
    //afxChain            threads;
    afxThread       activeThr;

    afxClock        startClock;
    afxClock        lastClock;
    afxClock        currClock;
    afxReal64       currTime;
    afxReal64       deltaTime;
    afxNat          iterCnt;
    afxNat          lastIterCnt;
    afxClock        iterCntSwapClock;

#ifdef _WIN32
    // system clock
    LARGE_INTEGER   QPC_InitialVal;
    unsigned int    LastTickCount;
    __int64         LastMicroseconds;
    __int64         CurrentAdjustment;
    LARGE_INTEGER   QPC_Frequency;
#endif
};
#endif//_AFX_PROCESSOR_C
#endif//_CORE_C

#ifdef _AFX_CORE_C
#ifdef _AFX_THREAD_C
AFX_OBJECT(afxThread)
{
    //afxLinkage      procUnit;
    afxSlock        procSlock;
    //afxNat          affineProcUnitIdx; // if not equal to AFX_INVALID_INDEX, this thread can be ran by any system processor unit, else case, will only be ran by the unit specified by this index.
    afxNat          affineProcUnitIdx; // if set bit set, only such processor will can run this thread.
    afxClock        startClock;
    afxClock        lastClock;
    afxClock        currClock;
    afxReal64       currTime;
    afxReal64       deltaTime;
    afxNat          iterCnt;
    afxNat          lastIterCnt;
    afxClock        iterCntSwapClock;
    afxError        (*proc)(afxThread thr, void *udd, afxThreadOpcode opcode);
    afxBool         started;
    afxBool         exited;
    afxBool         running;
    afxNat          suspendCnt;
    afxBool         isInFinish;
    afxBool         finished;
    afxBool         interruptionRequested;
    afxInt          exitCode;
    void            *udd;
};
#endif//_AFX_THREAD_C
#endif//_AFX_CORE_C

AFX afxNat      AfxEnumerateThreads(afxNat first, afxNat cnt, afxThread thr[]);
AFX afxNat      AfxCurateThreads(afxNat first, afxNat cnt, afxBool(*f)(afxThread, void*), void *udd);

AFX afxError    AfxAcquireThreads(afxNat cnt, afxThread thr[], afxThreadConfig const config[], afxHint const hint);

AFX void*       AfxGetThreadUdd(afxThread thr);

AFX void        AfxSetThreadAffinity(afxThread thr, afxNat procUnitIdx);

#if 0
AFX afxNat32    AfxGetThreadId(afxThread thr);
#endif

AFX void        AfxRunThread(afxThread thr);

/// Request the interruption of the thread. 
/// That request is advisory and it is up to code running on the thread to decide if and how it should act upon such request. 
/// This function does not stop any event loop running on the thread and does not terminate it in any way.

AFX void        AfxRequestThreadInterruption(afxThread thr);

AFX afxError    AfxPostThreadEvent(afxThread thr, afxEvent const *ev);


AFX afxBool     AfxThreadAreEqual(afxThread thr, afxThread other);

AFX afxNat      AfxResumeThread(afxThread thr);
AFX afxNat      AfxSuspendThread(afxThread thr);

/// Returns true if the thread is running; otherwise returns false.
AFX afxBool     AfxThreadIsRunning(afxThread thr);

/// Returns true if the thread is finished; otherwise returns false.
AFX afxBool     AfxThreadIsFinished(afxThread thr);

/// Return true if the task running on this thread should be stopped. An interruption can be requested by RequestThreadInterruption().
AFX afxBool     AfxShouldInterruptExecution(void);

/// Tells the thread's event loop to exit with a return code.
/// After calling this function, the thread leaves the event loop and returns from the call to event loop exec(). The event loop exec() function returns returnCode.
/// By convention, a returnCode of 0 means success, any non - zero value indicates an error.
/// Note that unlike the C library function of the same name, this function does return to the caller – it is event processing that stops.
/// No event loops will be started anymore in this thread until thread exec() has been called again.If the eventloop in thread exec() is not running then the next call to thread exec() will also return immediately.

AFX void        AfxExitExecution(afxInt code);

AFX afxBool     AfxGetThreadExitCode(afxThread thr, afxInt *exitCode);

/// Tells the thread's event loop to exit with return code 0 (success). Equivalent to calling AfxExitThread(0).
/// This function does nothing if the thread does not have an event loop.

AFX void        AfxQuitExecution(void);

/// Blocks the thread until the thread associated with this afxThread object has finished execution (i.e. when it returns from run()). 
/// This function will return true if the thread has finished. It also returns true if the thread has not been started yet.

AFX afxResult   AfxWaitForThread(afxThread thr, afxResult *exitCode);

/// Yields execution of the current thread to another runnable thread, if any. Note that the operating system decides to which thread to switch.

AFX void        AfxYieldThreading(void);

// functions affection the currrent caller thread.

AFX void        AfxGetThread(afxThread *thr);
AFX void        AfxGetThreadingUnit(afxNat *unitIdx);
AFX void        AfxGetThreadingId(afxNat32 *tid);

AFX void        AfxGetExecutionCounter(afxNat *currIter, afxNat *lastFreq);
AFX void        AfxGetExecutionTime(afxReal64 *curr, afxReal64 *delta);
AFX void        AfxGetExecutionClock(afxClock *curr, afxClock *last);

AFX void        AfxEndProcessor(afxResult exitCode);

AFX afxResult   AfxSleepProcessor(afxTimeSpec const* dur, afxTimeSpec *remaining);


//AFX afxBool     _AfxGetThrD(afxThread thr, struct _afxThrD **thrD, struct _afxSysD* sysD);

#endif//AFX_THREAD_H
