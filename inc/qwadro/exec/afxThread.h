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

/// The afxThread class provides a platform-independent way to manage threads.

/// A afxThread object manages one thread of control within the program.
/// afxThreads begin executing in run().
/// By default, run() starts the event loop by calling exec() and runs a event loop inside the thread.

#ifndef AFX_THREAD_H
#define AFX_THREAD_H

#include "qwadro/exec/afxClock.h"
#include "qwadro/exec/afxCondition.h"
#include "qwadro/exec/afxMutex.h"
#include "qwadro/mem/afxQueue.h"
#include "qwadro/base/afxEvent.h"
#include "qwadro/exec/afxAtomic.h"

#define AFX_THR_MIN_EVENT_CAP 32

typedef enum afxThreadPurpose
{
    afxThreadPurpose_SYSTEM = 1,
    afxThreadPurpose_SOUND,
    afxThreadPurpose_DRAW,
    afxThreadPurpose_ASIO,
    afxThreadPurpose_COMM,
    afxThreadPurpose_SIM,
    afxThreadPurpose_HID
} afxThreadPurpose;

typedef afxResult(*afxThreadProc)(afxThread thr, afxEvent* ev);

AFX_DEFINE_STRUCT(afxThreadConfig)
{
    afxNat              minEventCap;
    afxThreadPurpose    purpose;
    afxThreadProc       procCb;
    afxNat              tid;
    void*               udd[4];
};

////////////////////////////////////////////////////////////////////////////////

/// functions affection the currrent caller thread.

AFX afxBool     AfxIsPrimeThread(void);

AFX afxNat32    AfxGetTid(void);

AFX afxBool     AfxFindThread(afxNat32 tid, afxThread* thread);

AFX afxBool     AfxGetThread(afxThread* thread);

/// Return true if the task running on this thread should be stopped. An interruption can be requested by RequestThreadInterruption().
AFX afxBool     AfxThreadShouldBeInterrupted(void);

/// Tells the thread's event loop to exit with a return code.
/// After calling this function, the thread leaves the event loop and returns from the call to event loop exec(). The event loop exec() function returns returnCode.
/// By convention, a returnCode of 0 means success, any non - zero value indicates an error.
/// Note that unlike the C library function of the same name, this function does return to the caller – it is event processing that stops.
/// No event loops will be started anymore in this thread until thread exec() has been called again.If the eventloop in thread exec() is not running then the next call to thread exec() will also return immediately.

AFX void        AfxExitThread(afxInt code);

/// Tells the thread's event loop to exit with return code 0 (success). Equivalent to calling AfxExitThread(0).
/// This function does nothing if the thread does not have an event loop.

AFX void        AfxQuitThread(void);

/// Yields execution of the current thread to another runnable thread, if any. Note that the operating system decides to which thread to switch.
AFX void        AfxYield(void);

AFX void        AfxGetThreadFrequency(afxNat* iterNo, afxNat* lastFreq);
AFX void        AfxGetThreadTime(afxReal64* ct, afxReal64* dt);
AFX void        AfxGetThreadClock(afxClock* curr, afxClock* last);

////////////////////////////////////////////////////////////////////////////////

AFX void**      AfxGetThreadUdd(afxThread thr);

/// Begins execution of the thread by calling run(). The operating system will schedule the thread according to the priority parameter. If the thread is already running, this function does nothing.
/// The effect of the priority parameter is dependent on the operating system's scheduling policy. In particular, the priority will be ignored on systems that do not support thread priorities (such as on Linux, see the sched_setscheduler documentation for more details).

AFX void        AfxRunThread(afxThread thr);

/// Request the interruption of the thread. 
/// That request is advisory and it is up to code running on the thread to decide if and how it should act upon such request. 
/// This function does not stop any event loop running on the thread and does not terminate it in any way.

AFX void        AfxRequestThreadInterruption(afxThread thr);

//AFX afxError    AfxPostThreadEvent(afxThread thr, afxEvent const *ev);


AFX afxNat      AfxResumeThread(afxThread thr);
AFX afxNat      AfxSuspendThread(afxThread thr);

/// Returns true if the thread is running; otherwise returns false.
AFX afxBool     AfxThreadIsRunning(afxThread thr);

/// Returns true if the thread is finished; otherwise returns false.
AFX afxBool     AfxThreadIsFinished(afxThread thr);

AFX afxBool     AfxGetThreadExitCode(afxThread thr, afxInt *exitCode);

/// Blocks the thread until the thread associated with this afxThread object has finished execution (i.e. when it returns from run()). 
/// This function will return true if the thread has finished. It also returns true if the thread has not been started yet.

AFX afxResult   AfxWaitForThread(afxThread thr, afxResult *exitCode);

////////////////////////////////////////////////////////////////////////////////

AFX afxError    AfxAcquireThread(afxHere const hint, afxThreadConfig const* cfg, afxThread* thread);

AFX afxBool     AfxGetPrimeThread(afxThread* thread);

#endif//AFX_THREAD_H
