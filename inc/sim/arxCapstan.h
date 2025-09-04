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

// There are two primary objects involved in the run-time animation layer : the arxBody and the arxCapstan.
// Together, they represent the two ends of the animation process.
// The arxBody represents the state of a single model that may have some number of animations affecting it, 
// whereas the arxCapstan represents the state of an animation that may be affecting some number of models.
// When your engine creates a new animating entity in the game world, you create a arxBody to accompany it.
// When you want to play an animation on such an instance, you create a arxCapstan.

// Essential part of our machinima.

#ifndef ARX_MOTOR_H
#define ARX_MOTOR_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/render/arxPose.h"

AFX_DEFINE_STRUCT(arxCapstanTiming)
{
    afxReal         currClock;
    afxReal         killClock;
    afxReal         easeInStartClock;
    afxReal         easeInEndClock;
    afxReal         easeOutStartClock;
    afxReal         easeOutEndClock;
};

AFX_DEFINE_STRUCT(arxCapstanState)
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

AFX_DEFINE_STRUCT(arxCapstanConfig)
{
    afxReal currClock;
    afxReal localDur;
    afxUnit iterCnt;
};

ARXINL void         ArxQueryCapstanState
/// Query the current state of a motor execution.
(
    arxCapstan        caps,
    arxCapstanState*  state
);

ARX afxReal         ArxQueryCapstanLocalClock
/// Query the raw local clock and, optionally, compute a clamped value of it valid to be used to sample animation.
(
    arxCapstan        caps,
    afxReal*        clamped // [0-localDur]
);

ARX void            ArxGetCapstanTiming
/// Get internal clocks for a motor.
(
    arxCapstan        caps,
    arxCapstanTiming* timing
);

ARX afxBool         ArxCapstanIsUnused(arxCapstan caps);

// TERMINATION

ARX afxBool         ArxCapstanHasTerminated(arxCapstan caps);

ARX afxBool         ArxCapstanTerminationIsScheduled(arxCapstan caps);

ARX afxBool         ArxCapstanIsActive(arxCapstan caps);

ARX afxBool         ArxCapstanHasEffect(arxCapstan caps);

ARX void*           ArxGetCapstanUdd(arxCapstan caps, afxUnit idx);

ARX void            ArxSetCapstanTargetState(arxCapstan caps, afxReal currGlobalTime, afxReal targetGlobalTime, afxReal localTime, afxInt currIterIdx);

ARX void            ArxSetCapstanEaseInCurve(arxCapstan caps, afxReal startSeconds, afxReal endSeconds, afxReal startValue, afxReal startTangent, afxReal endTangent, afxReal endValue); // Ease the control in over the next two seconds
ARX void            ArxSetCapstanEaseOutCurve(arxCapstan caps, afxReal startSeconds, afxReal endSeconds, afxReal startValue, afxReal startTangent, afxReal endTangent, afxReal endValue); // In ten seconds, ease the control out over two seconds

ARX afxReal         ArxCapstanEaseIn(arxCapstan caps, afxReal duration, afxBool fromCurrent);
ARX afxReal         ArxCapstanEaseOut(arxCapstan caps, afxReal duration);

ARX afxReal         ArxGetCapstanEaseCurveMultiplier(arxCapstan caps);

////////////////////////////////////////////////////////////////////////////////

/// Resets the raw local clock of a motor to the time passed in.

ARX void            ArxResetCapstanLocalClock(afxReal localClock, afxUnit cnt, arxCapstan capstans[]);

// Devido a limita��o imposto pelo tipo afxReal32, os rel�gios de uma arxCapstan n�o podem operar por mais que um dia e meio sem problemas relacionados a perda de precis�o.
// � necess�rio recentralizar os rel�gios em algum momento ap�s tal per�odo de opera��o.

/// Rebase internal clocks with the passed into one.

ARX void            ArxRebaseCapstanClocks(afxReal currClock, afxUnit cnt, arxCapstan capstans[]);

/// Update the current motor clock to a new one. // new absolute time

ARX void            ArxStepCapstans(afxReal /* NOT delta */ time, afxUnit cnt, arxCapstan capstans[]);

// deveria ser suspend, j� que o motor j� come�a ativo

ARX void            ArxActivateCapstan(afxBool active, afxUnit cnt, arxCapstan capstans[]);

ARX void            ArxSetCapstanWeight(afxReal weight, afxUnit cnt, arxCapstan capstans[]);

/// Sets the speed multiplier. 0.f = stop it, -1.f play it backwards, 0.5 half the playback, 2.0 doubles the playback, etc.

ARX void            ArxSetCapstanSpeed(afxReal speed, afxUnit cnt, arxCapstan capstans[]);

ARX void            ArxSetCapstanClockOnly(afxReal time, afxUnit cnt, arxCapstan capstans[]);

/// Resets the total of iterations allowed for the capstans. 0 means infinite.

ARX void            ArxResetCapstanIterations(afxInt iterCnt, afxUnit cnt, arxCapstan capstans[]);

/// Selects the current iteration index.

ARX void            ArxSelectCapstanIteration(afxInt currIterIdx, afxUnit cnt, arxCapstan capstans[]);

ARX void            ArxEnableCapstanIterationClamping(afxBool clamp, afxUnit cnt, arxCapstan capstans[]);
ARX void            ArxEnableCapstanTerminationCheck(afxBool checkComplete, afxUnit cnt, arxCapstan capstans[]);
ARX void            ArxEnableCapstanEaseIn(afxBool easeIn, afxUnit cnt, arxCapstan capstans[]);
ARX void            ArxEnableCapstanEaseOut(afxBool easeOut, afxUnit cnt, arxCapstan capstans[]);

/// Schedules the self-termination when capstans reaches the specified seconds.

ARX void            ArxScheduleCapstanTermination(afxReal atSecs, afxUnit cnt, arxCapstan capstans[]);

ARX afxBool         ArxReleaseUnusedCapstans(afxUnit cnt, arxCapstan capstans[]);
ARX afxBool         ArxReleaseOnceUnusedCapstans(afxUnit cnt, arxCapstan capstans[]);
ARX afxBool         ArxReleaseTerminatedCapstans(afxUnit cnt, arxCapstan capstans[]);

ARX afxError        ArxAcquireCapstans(afxSimulation sim, arxCapstanConfig const* cfg, afxUnit cnt, arxCapstan capstans[]);

#endif//ARX_MOTOR_H
