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

#ifndef AFX_TXU_H
#define AFX_TXU_H

#include "qwadro/core/afxClock.h"
#include "qwadro/core/afxMutex.h"
#include "qwadro/core/afxSlock.h"
#include "qwadro/core/afxThread.h"
#include "qwadro/core/afxCondition.h"
#include "qwadro/mem/afxArray.h"

#ifdef _AFX_CORE_C
#ifdef _AFX_TXU_C
AFX_OBJECT(afxTxu)
{
    afxNat          unitIdx;
    afxNat          tid;
    void*           osHandle;
    afxChain        threads;
    afxThread       activeThr;

    afxClock        startClock;
    afxClock        lastClock;
    //afxClock        currClock;
    //afxReal64       currTime;
    //afxReal64       deltaTime;
    afxNat          iterNo;
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
#endif//_AFX_TXU_C
#endif//_AFX_CORE_C

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
AFX void        AfxYieldThreading(void);

// functions affection the currrent caller thread.

AFX void        AfxGetThreadingUnit(afxNat *txuIdx);
AFX void        AfxGetThreadingId(afxNat32 *tid);

AFX void        AfxGetThread(afxThread* thr);
AFX void        AfxGetThreadCounter(afxNat* iterNo, afxNat* lastFreq);
AFX void        AfxQueryThreadTime(afxReal64* ct, afxReal64* dt);
AFX void        AfxGetThreadClock(afxClock* curr, afxClock* last);

AFX afxError    _AfxAcquireTxus(afxNat cnt, afxThread threads[]);
AFX afxError    _AfxReleaseTxus(afxNat cnt, afxThread threads[]);

#endif//AFX_TXU_H
