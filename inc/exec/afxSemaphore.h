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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// Semaphores are a synchronization primitive that can be used to insert a dependency between queue operations or between a queue operation and the host.
/// Binary semaphores have two states - signaled and unsignaled. 
/// Timeline semaphores have a strictly increasing 64 - bit unsigned integer payload and are signaled with respect to a particular reference value.
/// A semaphore can be signaled after execution of a queue operation is completed, and a queue operation can wait for a semaphore to become signaled before it begins execution.
/// A timeline semaphore can additionally be signaled from the host with the AfxSignalSemaphore command and waited on from the host with the AfxWaitSemaphores command.

#ifndef AFX_SEMAPHORE_H
#define AFX_SEMAPHORE_H

#include "qwadro/inc/base/afxChain.h"

/// Return the draw context witch this semaphore belongs to.
AFX void*               AfxGetSemaphoreContext(afxSemaphore sem);

AFX afxError            AfxSignalSemaphore
/// Signal a timeline semaphore on the host
(
    afxSemaphore        sem, /// the semaphore to signal.
    afxNat64            value /// the value to signal.
);

AFX afxError            AfxGetSemaphoreValue
/// Query the current state of a timeline semaphore.
(
    afxSemaphore        sem, /// the semaphore to query.
    afxNat64*           value /// a 64-bit integer var in which the current counter value of the semaphore is returned.
);

////////////////////////////////////////////////////////////////////////////////

AFX afxError            AfxAcquireSemaphores
/// Create a new queue semaphore object.
(
    void*               ctx, /// the logical device that creates the semaphore.
    afxNat              cnt,
    afxSemaphore        semaphores[] /// a array of handles in which the resulting semaphore objects are returned.
);

AFX afxError            AfxWaitForSemaphores
/// Wait for one or more timeline semaphores to reach particular counter values.
(
    afxNat64            timeout, /// the timeout period in units of nanoseconds.
    afxBool             waitAll, /// the condition is that all semaphores must reach the specified value, else at least one semaphore has reached.
    afxNat              cnt, /// the number of semaphores to wait on.
    afxSemaphore const  semaphores[], /// an array of cnt semaphores handles.
    afxNat64 const      values[] /// an array of timeline values
);

#endif//AFX_SEMAPHORE_H
