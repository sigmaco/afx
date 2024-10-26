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

/// Fences are a synchronization primitive that can be used to insert a dependency from a queue to the host.
/// Fences have two states - signaled and unsignaled. 
/// A fence can be signaled as part of the execution of a queue submission command. 
/// Fences can be unsignaled on the host with AfxResetFences.
/// Fences can be waited on by the host with the AfxWaitForFences command, and the current state can be queried with AfxGetFenceStatus.

/// The internal data of a fence may include a reference to any resources and pending work associated with signal or unsignal operations performed on that fence object, collectively referred to as the fence’s payload.
/// Mechanisms to import and export that internal data to and from fences are provided below. 
/// These mechanisms indirectly enable applications to share fence state between two or more fences and other synchronization primitives across process and API boundaries.

#ifndef AFX_FENCE_H
#define AFX_FENCE_H

#include "qwadro/inc/base/afxChain.h"

/// Return the device witch this fence belongs to.
AFX afxDevice       AfxGetFenceDevice(afxFence fenc);

/// Return the device context witch this fence belongs to.
AFX afxContext      AfxGetFenceContext(afxFence fenc);

/// Return the status of this fence.
AFX afxBool         AfxFenceIsSignaled(afxFence fenc);

////////////////////////////////////////////////////////////////////////////////

AFX afxError        AfxAcquireFences
/// Create a new fence object.
(
    afxContext ctx, /// the logical device that creates the fence.
    afxBool         signaled, /// fence object is created in the signaled state.
    afxUnit          cnt,
    afxFence        fences[] /// a array of handles in which the resulting fence objects are returned.
);

AFX afxError        AfxWaitForFences
/// Wait for one or more fences to become signaled.
(
    afxBool         waitAll, /// the condition is that all fences must be signaled, else at least one fence is signaled.
    afxUnit64        timeout, /// the timeout period in units of nanoseconds.
    afxUnit          cnt, /// the number of fences to wait on.
    afxFence const  fences[] /// an array of cnt fence handles.    
);

AFX afxError        AfxResetFences
/// Resets one or more fence objects.
(
    afxUnit          cnt, /// the number of fences to reset.
    afxFence const  fences[] /// an array of fence handles to reset.
);

#endif//AFX_FENCE_H
