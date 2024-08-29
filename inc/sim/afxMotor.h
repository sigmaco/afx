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

// Terminology: Motor is an animating force or underlying purpose; intention;
// Terminology: Puppet is a movable model of a person or animal that is used in entertainment and is typically moved either by strings controlled from above or by a hand inside it.

// There are two primary objects involved in the run-time animation layer : the afxBody and the afxMotor.
// Together, they represent the two ends of the animation process.
// The afxBody represents the state of a single model that may have some number of animations affecting it, 
// whereas the afxMotor represents the state of an animation that may be affecting some number of models.
// When your engine creates a new animating entity in the game world, you create a afxBody to accompany it.
// When you want to play an animation on such an instance, you create a afxMotor.

#ifndef AMX_MOTOR_H
#define AMX_MOTOR_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/cad/afxPose.h"

AFX_DEFINE_STRUCT(afxMotorTiming)
{
    afxReal         currClock;
    afxReal         killClock;
    afxReal         easeInStartClock;
    afxReal         easeInEndClock;
    afxReal         easeOutStartClock;
    afxReal         easeOutEndClock;
};

AFX_DEFINE_STRUCT(afxMotorState)
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

AMXINL void         AkxQueryMotorState
/// Query the current state of a motor execution.
(
    afxMotor        moto,
    afxMotorState*  state
);

AMX afxReal         AkxQueryMotorLocalClock
/// Query the raw local clock and, optionally, compute a clamped value of it valid to be used to sample animation.
(
    afxMotor        moto,
    afxReal*        clamped // [0-localDur]
);

AMX void            AfxGetMotorTiming
/// Get internal clocks for a motor.
(
    afxMotor        moto,
    afxMotorTiming* timing
);

AMX afxBool         AfxMotorIsUnused(afxMotor moto);

// TERMINATION

AMX afxBool         AfxMotorHasTerminated(afxMotor moto);

AMX afxBool         AkxMotorTerminationIsScheduled(afxMotor moto);

AMX afxBool         AfxMotorIsActive(afxMotor moto);

AMX afxBool         AfxMotorHasEffect(afxMotor moto);

AMX void**          AkxGetMotorUdd(afxMotor moto);

AMX void            AkxSetMotorTargetState(afxMotor moto, afxReal currGlobalTime, afxReal targetGlobalTime, afxReal localTime, afxInt currIterIdx);

AMX void            AkxSetMotorEaseInCurve(afxMotor moto, afxReal startSeconds, afxReal endSeconds, afxReal startValue, afxReal startTangent, afxReal endTangent, afxReal endValue); // Ease the control in over the next two seconds
AMX void            AkxSetMotorEaseOutCurve(afxMotor moto, afxReal startSeconds, afxReal endSeconds, afxReal startValue, afxReal startTangent, afxReal endTangent, afxReal endValue); // In ten seconds, ease the control out over two seconds

AMX afxReal         AkxMotorEaseIn(afxMotor moto, afxReal duration, afxBool fromCurrent);
AMX afxReal         AkxMotorEaseOut(afxMotor moto, afxReal duration);

AMX afxReal         AkxGetMotorEaseCurveMultiplier(afxMotor moto);

////////////////////////////////////////////////////////////////////////////////

AMX afxError        AfxAcquireMotors(afxSimulation sim, afxReal currClock, afxReal localDur, afxNat iterCnt, afxNat cnt, afxMotor moto[]);

AMX afxBool         AkxReleaseUnusedMotors(afxNat cnt, afxMotor motors[]);
AMX afxBool         AkxReleaseOnceUnusedMotors(afxNat cnt, afxMotor motors[]);
AMX afxBool         AkxReleaseTerminatedMotors(afxNat cnt, afxMotor motors[]);

/// Resets the raw local clock of a motor to the time passed in.

AMX void            AkxResetMotorLocalClock(afxReal localClock, afxNat cnt, afxMotor motors[]);

// Devido a limitação imposto pelo tipo afxReal32, os relógios de uma afxMotor não podem operar por mais que um dia e meio sem problemas relacionados a perda de precisão.
// É necessário recentralizar os relógios em algum momento após tal período de operação.

/// Rebase internal clocks with the passed into one.

AMX void            AkxRebaseMotorClocks(afxReal currClock, afxNat cnt, afxMotor motors[]);

/// Update the current motor clock to a new one. /// new absolute time

AMX void            AkxUpdateMotorClock(afxReal time, afxNat cnt, afxMotor motors[]);

// deveria ser suspend, já que o motor já começa ativo

AMX void            AkxActivateMotor(afxBool active, afxNat cnt, afxMotor motors[]);

AMX void            AkxSetMotorWeight(afxReal weight, afxNat cnt, afxMotor motors[]);

/// Sets the speed multiplier. 0.f = stop it, -1.f play it backwards, 0.5 half the playback, 2.0 doubles the playback, etc.

AMX void            AkxSetMotorSpeed(afxReal speed, afxNat cnt, afxMotor motors[]);

AMX void            AkxSetMotorClockOnly(afxReal time, afxNat cnt, afxMotor motors[]);

/// Resets the total of iterations allowed for the motors. 0 means infinite.

AMX void            AkxResetMotorIterations(afxInt iterCnt, afxNat cnt, afxMotor motors[]);

/// Selects the current iteration index.

AMX void            AkxSelectMotorIteration(afxInt currIterIdx, afxNat cnt, afxMotor motors[]);

AMX void            AkxEnableMotorIterationClamping(afxBool clamp, afxNat cnt, afxMotor motors[]);
AMX void            AkxEnableMotorTerminationCheck(afxBool checkComplete, afxNat cnt, afxMotor motors[]);
AMX void            AkxEnableMotorEaseIn(afxBool easeIn, afxNat cnt, afxMotor motors[]);
AMX void            AkxEnableMotorEaseOut(afxBool easeOut, afxNat cnt, afxMotor motors[]);

/// Schedules the self-termination when motors reaches the specified seconds.

AMX void            AfxScheduleMotorTermination(afxReal atSecs, afxNat cnt, afxMotor motors[]);

#endif//AMX_MOTOR_H
