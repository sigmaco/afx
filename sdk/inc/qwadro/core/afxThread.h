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

#ifndef AFX_THREAD_H
#define AFX_THREAD_H

#include "qwadro/core/afxClock.h"
#include "qwadro/core/afxSlock.h"

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

AFX_DEFINE_STRUCT(afxThreadConfig)
{
    afxError            (*proc)(afxThread thr, void *udd, afxInt opcode);
    void*               udd;
    afxNat              baseTxu;
    afxNat              txuCnt;
    afxThreadPurpose    purpose;
};

#ifdef _AFX_CORE_C
#ifdef _AFX_THREAD_C
AFX_DEFINE_STRUCT(afxThreadLinkage)
{
    afxLinkage          thr;
    afxLinkage          txu;
};

AFX_OBJECT(afxThread)
{
    //afxLinkage      procUnit;
    afxSlock            txuSlock;
    //afxNat          affineProcUnitIdx; // if not equal to AFX_INVALID_INDEX, this thread can be ran by any system processor unit, else case, will only be ran by the unit specified by this index.
    //afxNat          affineThrUnitIdx; // if set bit set, only such processor will can run this thread.
    afxClock            startClock;
    afxClock            lastClock;
    //afxClock            currClock;
    //afxReal64           currTime;
    //afxReal64           deltaTime;
    afxNat              iterNo;
    afxNat              lastIterCnt;
    afxClock            iterCntSwapClock;
    afxError            (*proc)(afxThread thr, void *udd, afxThreadOpcode opcode);
    afxBool             started;
    afxBool             exited;
    afxBool             running;
    afxNat              suspendCnt;
    afxBool             isInFinish;
    afxBool             finished;
    afxBool             interruptionRequested;
    afxInt              exitCode;

    // our way of doing CPU affinity actually
    afxNat              baseTxu;
    afxNat              txuCnt;
    
    afxChain            txus;

    void*               udd;
};
#endif//_AFX_THREAD_C
#endif//_AFX_CORE_C

AFX void*       AfxGetThreadUdd(afxThread thr);

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

// functions affection the currrent caller thread.

AFX void        AfxRunThreads(afxNat cnt, afxThread threads[]);

AFX afxError    AfxAcquireThreads(afxThreadConfig const* cfg, afxHint const hint, afxNat cnt, afxThread threads[]);

#endif//AFX_THREAD_H
