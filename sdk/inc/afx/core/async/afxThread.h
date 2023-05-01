/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_THREAD_H
#define AFX_THREAD_H

#include "afx/core/afxObject.h"
#include "afx/core/mem/afxQueue.h"
#include "afx/core/time/afxTime.h"
#include "afx/core/mem/afxMemory.h"

// The afxThread class provides a platform-independent way to manage threads.

// A afxThread object manages one thread of control within the program. afxThreads begin executing in run(). By default, run() starts the event loop by calling exec() and runs a Qt event loop inside the thread.

AFX_DEFINE_HANDLE(afxThread);

AFX_OBJECT(afxThread)
{
    afxObject       obj;
#ifdef _AFX_THREAD_C
    afxNat32        tid;
    afxSize         osHandle;
    afxHint         hint;
    void            (*start)(afxThread thr, void *udd);
    void            *udd;
    afxInt          exitCode;
    afxBool         started;
    afxBool         running;
    afxBool         finnished;
    afxBool         interruptionRequested;
    afxQueue        events;
    void            (*exec)(afxThread thr);
    afxMemory       mem;
#endif
};

AFX void*       AfxThreadGetSystem(afxThread thr);
AFX void*       AfxThreadGetUdd(afxThread thr);

AFX afxNat32    AfxThreadGetId(afxThread thr);
AFX void        AfxThreadStart(afxThread thr);
AFX void        AfxThreadRequestInterruption(afxThread thr);
AFX afxError    AfxThreadPostEvent(afxThread thr, afxEvent const *ev);
AFX afxBool     AfxThreadAreEqual(afxThread thr, afxThread other);

// functions affection the currrent caller thread.

AFX afxNat32    AfxGetTid(void);
AFX void        AfxYield(void);
AFX void        AfxExit(afxResult exitCode);
AFX afxResult   AfxSleep(afxTimeSpec const* dur, afxTimeSpec *remaining);
AFX afxResult   AfxWaitForThread(afxThread thr, afxResult *exitCode);

#endif//AFX_THREAD_H