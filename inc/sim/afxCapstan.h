#pragma once
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

// Servo
// Governor
// Regulator

// Terminology: Motive is an animating force or underlying purpose; intention;
// Terminology: Puppet is a movable model of a person or animal that is used in entertainment and is typically moved either by strings controlled from above or by a hand inside it.

// There are two primary objects involved in the run-time animation layer : the afxBody and the afxCapstan.
// Together, they represent the two ends of the animation process.
// The afxBody represents the state of a single model that may have some number of animations affecting it, 
// whereas the afxCapstan represents the state of an animation that may be affecting some number of models.
// When your engine creates a new animating entity in the game world, you create a afxBody to accompany it.
// When you want to play an animation on such an instance, you create a afxCapstan.

// Essential part of our machinima.

#ifndef ASX_MOTOR_H
#define ASX_MOTOR_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/sim/body/afxPose.h"

AFX_DEFINE_STRUCT(afxCapstanTiming)
{
    afxReal         currClock;
    afxReal         killClock;
    afxReal         easeInStartClock;
    afxReal         easeInEndClock;
    afxReal         easeOutStartClock;
    afxReal         easeOutEndClock;
};

AFX_DEFINE_STRUCT(afxCapstanState)
{
    afxBool         active;
    afxReal         speed;
    afxInt          iterCnt;
    afxInt          currIterIdx;
    afxReal         currWeight;
    afxReal         effectiveWeight;
    afxBool         underflow; // Figure out what looping state the control is in
    afxBool         overflow; // Figure out what looping state the control is in
    afxReal         localClockRaw;
    afxReal         localClockClamped;
    afxReal         dur; // Find out how long this control plays for, total
    afxReal         durLeft; // Find out how long this control will continue to play for, accounting for however much time has already elapsed
    afxReal         localDur;
};

AFX_DEFINE_STRUCT(afxCapstanConfig)
{
    afxReal currClock;
    afxReal localDur;
    afxUnit iterCnt;
};

ASXINL void         AfxQueryCapstanState
/// Query the current state of a motor execution.
(
    afxCapstan        caps,
    afxCapstanState*  state
);

ASX afxReal         AfxQueryCapstanLocalClock
/// Query the raw local clock and, optionally, compute a clamped value of it valid to be used to sample animation.
(
    afxCapstan        caps,
    afxReal*        clamped // [0-localDur]
);

ASX void            AfxGetCapstanTiming
/// Get internal clocks for a motor.
(
    afxCapstan        caps,
    afxCapstanTiming* timing
);

ASX afxBool         AfxCapstanIsUnused(afxCapstan caps);

// TERMINATION

ASX afxBool         AfxCapstanHasTerminated(afxCapstan caps);

ASX afxBool         AfxCapstanTerminationIsScheduled(afxCapstan caps);

ASX afxBool         AfxCapstanIsActive(afxCapstan caps);

ASX afxBool         AfxCapstanHasEffect(afxCapstan caps);

ASX void*           AfxGetCapstanUdd(afxCapstan caps, afxUnit idx);

ASX void            AfxSetCapstanTargetState(afxCapstan caps, afxReal currGlobalTime, afxReal targetGlobalTime, afxReal localTime, afxInt currIterIdx);

ASX void            AfxSetCapstanEaseInCurve(afxCapstan caps, afxReal startSeconds, afxReal endSeconds, afxReal startValue, afxReal startTangent, afxReal endTangent, afxReal endValue); // Ease the control in over the next two seconds
ASX void            AfxSetCapstanEaseOutCurve(afxCapstan caps, afxReal startSeconds, afxReal endSeconds, afxReal startValue, afxReal startTangent, afxReal endTangent, afxReal endValue); // In ten seconds, ease the control out over two seconds

ASX afxReal         AfxCapstanEaseIn(afxCapstan caps, afxReal duration, afxBool fromCurrent);
ASX afxReal         AfxCapstanEaseOut(afxCapstan caps, afxReal duration);

ASX afxReal         AfxGetCapstanEaseCurveMultiplier(afxCapstan caps);

////////////////////////////////////////////////////////////////////////////////

/// Resets the raw local clock of a motor to the time passed in.

ASX void            AfxResetCapstanLocalClock(afxReal localClock, afxUnit cnt, afxCapstan capstans[]);

// Devido a limitação imposto pelo tipo afxReal32, os relógios de uma afxCapstan não podem operar por mais que um dia e meio sem problemas relacionados a perda de precisão.
// É necessário recentralizar os relógios em algum momento após tal período de operação.

/// Rebase internal clocks with the passed into one.

ASX void            AfxRebaseCapstanClocks(afxReal currClock, afxUnit cnt, afxCapstan capstans[]);

/// Update the current motor clock to a new one. /// new absolute time

ASX void            AfxUpdateCapstanClock(afxReal time, afxUnit cnt, afxCapstan capstans[]);

// deveria ser suspend, já que o motor já começa ativo

ASX void            AfxActivateCapstan(afxBool active, afxUnit cnt, afxCapstan capstans[]);

ASX void            AfxSetCapstanWeight(afxReal weight, afxUnit cnt, afxCapstan capstans[]);

/// Sets the speed multiplier. 0.f = stop it, -1.f play it backwards, 0.5 half the playback, 2.0 doubles the playback, etc.

ASX void            AfxSetCapstanSpeed(afxReal speed, afxUnit cnt, afxCapstan capstans[]);

ASX void            AfxSetCapstanClockOnly(afxReal time, afxUnit cnt, afxCapstan capstans[]);

/// Resets the total of iterations allowed for the capstans. 0 means infinite.

ASX void            AfxResetCapstanIterations(afxInt iterCnt, afxUnit cnt, afxCapstan capstans[]);

/// Selects the current iteration index.

ASX void            AfxSelectCapstanIteration(afxInt currIterIdx, afxUnit cnt, afxCapstan capstans[]);

ASX void            AfxEnableCapstanIterationClamping(afxBool clamp, afxUnit cnt, afxCapstan capstans[]);
ASX void            AfxEnableCapstanTerminationCheck(afxBool checkComplete, afxUnit cnt, afxCapstan capstans[]);
ASX void            AfxEnableCapstanEaseIn(afxBool easeIn, afxUnit cnt, afxCapstan capstans[]);
ASX void            AfxEnableCapstanEaseOut(afxBool easeOut, afxUnit cnt, afxCapstan capstans[]);

/// Schedules the self-termination when capstans reaches the specified seconds.

ASX void            AfxScheduleCapstanTermination(afxReal atSecs, afxUnit cnt, afxCapstan capstans[]);

ASX afxBool         AfxReleaseUnusedCapstans(afxUnit cnt, afxCapstan capstans[]);
ASX afxBool         AfxReleaseOnceUnusedCapstans(afxUnit cnt, afxCapstan capstans[]);
ASX afxBool         AfxReleaseTerminatedCapstans(afxUnit cnt, afxCapstan capstans[]);

ASX afxError        AfxAcquireCapstans(afxSimulation sim, afxCapstanConfig const* cfg, afxUnit cnt, afxCapstan capstans[]);

#endif//ASX_MOTOR_H
