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

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO DRAWING DEVICE SYNCHRONIZATION FENCE                              //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

/// Fences are a synchronization primitive that can be used to insert a dependency from a queue to the host.
/// Fences have two states - signaled and unsignaled. 
/// A fence can be signaled as part of the execution of a queue submission command. 
/// Fences can be unsignaled on the host with AvxResetFences.
/// Fences can be waited on by the host with the AvxWaitForFences command, and the current state can be queried with AfxGetFenceStatus.

/// The internal data of a fence may include a reference to any resources and pending work associated with signal or unsignal operations performed on that fence object, collectively referred to as the fence’s payload.
/// Mechanisms to import and export that internal data to and from fences are provided below. 
/// These mechanisms indirectly enable applications to share fence state between two or more fences and other synchronization primitives across process and API boundaries.

#ifndef AVX_FENCE_H
#define AVX_FENCE_H

#include "qwadro/draw/afxDrawDefs.h"

typedef enum avxFenceFlag
{
    // The fence interacts with the CPU; that's, the host can wait on or reset it.
    // If not present, it is a device-only fence.
    avxFenceFlag_CPU        = AFX_BITMASK(0),
    // The fence is a timeline semaphore.
    // If not present, it is a binary semaphore.
    avxFenceFlag_TIMELINE   = AFX_BITMASK(1),
    // The fence has been imported from other API.
    avxFenceFlag_EXTERNAL   = AFX_BITMASK(2)
} avxFenceFlags;

AFX_DEFINE_STRUCT(avxFenceInfo)
{
    avxFenceFlags   flags;
    // The initial value for a timeline semaphore.
    // For binary, any value makes the object be created in the signaled state.
    afxUnit64       initialVal;
    afxString       tag;
    void*           udd;

    afxFlags        exFlags;
    afxString       exType;
    void*           exHandle;
    afxString       exName;
};

AVX afxError        AvxAcquireFences
// Create a new fence object.
(
    // The draw system that creates the fence.
    afxDrawSystem   dsys,
    // The number of fences to be acquired.
    afxUnit         cnt,
    // The information prescribing the fences to be acquired.
    avxFenceInfo const info[],
    // An array of handles in which the resulting fence objects are returned.
    avxFence        fences[]
);

AVX afxError        AvxWaitForFences
// Wait for one or more fences to become signaled.
(
    afxDrawSystem   dsys,
    // The timeout period in units of nanoseconds.
    afxUnit64       timeout,
    // The condition is that all fences must be signaled, else at least one fence is signaled.
    afxBool         waitAll,
    // The number of fences to wait on.
    afxUnit         cnt,
    // An array of cnt fence handles.    
    avxFence const  fences[],
    // An array of @cnt timeline values.
    // Optinal. Only for avxFenceFlag_TIMELINE.
    afxUnit64 const values[]
);

AVX afxError        AvxResetFences
// Resets one or more fence objects.
(
    afxDrawSystem   dsys,
    // The number of fences to reset.
    afxUnit         cnt,
    // An array of fence handles to reset.
    avxFence const  fences[]
);

////////////////////////////////////////////////////////////////////////////////

// Return the draw system witch this fence belongs to.
AVX afxDrawSystem   AvxGetFenceHost(avxFence fenc);

// Return the status of this fence.
AVX afxBool         AvxCheckForFenceSignaled(avxFence fenc);

AVX afxError        AvxGetTimelineFenceCounter(avxFence fenc, afxUnit64* value);

AVX afxError        AvxSignalTimelineFence(avxFence fenc, afxUnit64 value);

#endif//AVX_FENCE_H
