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

#ifndef AFX_MOTOR_H
#define AFX_MOTOR_H

// Terminology: Motor is an animating force or underlying purpose; intention;
// Terminology: Puppet is a movable model of a person or animal that is used in entertainment and is typically moved either by strings controlled from above or by a hand inside it.

// There are two primary objects involved in the run-time animation layer : the afxBody and the akxMotor.
// Together, they represent the two ends of the animation process.
// The afxBody represents the state of a single model that may have some number of animations affecting it, 
// whereas the akxMotor represents the state of an animation that may be affecting some number of models.
// When your engine creates a new animating entity in the game world, you create a afxBody to accompany it.
// When you want to play an animation on such an instance, you create a akxMotor.

#include "qwadro/space/akxPose.h"

AFX_DECLARE_STRUCT(akxControlledAnimation);
AFX_DECLARE_STRUCT(akxControlledPose);

AFX_DECLARE_STRUCT(akxMotorInterlinkCallbacks);

typedef struct controlled_pose
{
    akxPose Pose; // 0
    const akxTrackMask *ModelMask; // 4
} controlled_pose;

typedef struct controlled_animation
{
    struct animation_binding *Binding; // 0
    accumulation_mode AccumulationMode; // 4
    const akxTrackMask *TrackMask; // 8
    const akxTrackMask *ModelMask; // 12
} controlled_animation;

AFX_DEFINE_STRUCT(akxMotive)
{
    akxMotor            moto;
    afxLinkage          bod;
    akxMotorInterlinkCallbacks const *callbacks;
    union
    {
        controlled_animation ca;
        controlled_pose      cp;
    };
    void*               reservedPtr;
};

AFX_DEFINE_STRUCT(akxMotorInterlinkCallbacks)
{
    struct controlled_animation*(*GetControlledAnimation)(akxMotive*);
    struct controlled_pose*(*GetControlledPose)(akxMotive*);
    afxBool(*InitializeBindingState)(akxMotive*, void*);
    void(*AccumulateBindingState)(akxMotive*, afxNat, afxNat, akxPose*, afxReal, afxNat const*);
    void(*BuildBindingDirect)(akxMotive*, afxNat, afxReal const*, akxPoseBuffer*, afxReal);
    void(*AccumulateLoopTransform)(akxMotive*, afxReal, afxReal*, afxReal*, afxReal*, afxBool);
    void(*CleanupBindingState)(akxMotive*);
};

typedef enum akxMotorFlags
{
    akxMotorFlags_ACTIVE = AFX_BIT(0),
    akxMotorFlags_KILL_ONCE_COMPLETE = AFX_BIT(1),
    akxMotorFlags_KILL_ONCE_UNUSED = AFX_BIT(2),
    akxMotorFlags_EASE_IN = AFX_BIT(3),
    akxMotorFlags_EASE_OUT = AFX_BIT(4),
    akxMotorFlags_FORCE_CLAMPLED_LOOPS = AFX_BIT(5)
} akxMotorFlags;

AFX_OBJECT(akxMotor)
{
    akxMotorFlags   flags;
    afxReal         currClock;
    afxReal         dtLocalClockPending;
    afxReal         localClock;
    afxReal         speed;
    afxReal         localDur;
    afxInt          currIterIdx;
    afxInt          iterCnt;
    afxReal         killClock;
    afxReal         currWeight;
    afxReal         easeInStartClock;
    afxReal         easeInEndClock;
    afxNat          easeInValues;
    afxReal         easeOutStartClock;
    afxReal         easeOutEndClock;
    afxNat          easeOutValues;
    void*           userData[4];
};

// The akxMotor handle you get back when you play an animation is the handle you use to manipulate the animation during playback.
// You can perform a number of timing and weighting operations through the akxMotor, but before we get to that, 
// it's important to note that as long as you hold a control handle, Qwadro considers its corresponding animation to be active.
// Thus, for every akxMotor that you create through calls like PlayControlledAnimation, 
// you must have a corresponding call to free the control. Here are some of the ways you can do that:

AKX afxBool     AfxMotorIsUnused(akxMotor moto);

AKX void        AfxScheduleMotorTermination(akxMotor moto, afxReal atSecs); // Free the control X seconds

// Each akxMotor keeps track of what time it thinks it is.
// This is an absolute time, and can be directly queried or modified:

AKX afxReal     AkxGetMotorClock(akxMotor moto); // // Find out what time the control thinks it is
AKX void        AkxSetMotorClock(akxMotor moto, afxReal time); // Tell it to advance by X second

// Modifying a akxMotor's clock will cause all its animations to be advanced appropriately. 
// However, they will not necessarily advance by the amount of time you advanced the clock. 
// The speed of the control acts as a time multiplier on the control's animations.

// The default speed setting is 1.0f, which keeps animations advancing in lock-step with the control's clock. 
// A fractional setting makes the animations play back slower than normal, and a setting of greater than 1.0f makes them play back faster than normal. 
// A negative setting makes the animations play backwards. Here are some examples: 

AKX afxReal     AkxGetMotorSpeed(akxMotor moto); // Find out how fast the control's animations are playing
AKX void        AkxSetMotorSpeed(akxMotor moto, afxReal speed); // Have the control play its animations X times fast. (0.5 = half as fast as real time, 10 = ten times faster than real time, 0 = Stop all regardless of the clock, -1.0 = play backwards)

// Animations can be looped, either indefinitely or for a specific number of loops. The loop count is queried and modified like this: 

AKX afxInt      AkxCountMotorIterations(akxMotor moto); // Find out how many loops this control is set to do (by default, it will be 1)
AKX void        AkxAdjustMotorIterations(akxMotor moto, afxInt iterCnt); // Make the control do X loops. (0 = loop indefinitely)

// A loop count of 0 indicates the special case of infinite looping.Animations played by a control with a loop count of 0 will play forever.

// In addition to setting the loop count, it is also possible to find out or modify how many loops the control has done. 
// This is done by querying or modifying the loop index:

AKX afxInt      AkxQueryMotorIteration(akxMotor moto); // Find out what loop the control is on
AKX void        AkxSelectMotorIteration(akxMotor moto, afxInt currIterIdx); // Make the control think it's done X loops already

// Desde o objecto akxMotor dar lhe muito controle sobre como suas animações são reproduzidas, 
// isto pode ser difícil para você em determinar quão longo uma animação deveria tomar para reproduzir dado um certo estado de controle.
// Para facilitar isto, Qwadro suple duas funções ajudantes que fazem as calculações para você:

AKX afxReal     AkxDetermineMotorDuration(akxMotor moto); // Find out how long this control plays for, total
AKX afxReal     AkxDetermineMotorRemainingDuration(akxMotor moto); // Find out how long this control will continue to play for, accounting for however much time has already elapsed

// Por vezes, um author vai querer parear um afxBody ou uma afxMotor no Qwadro com some application-specific structure of your own.
// This does not pose a problem for most Qwadro APIs, since you will always have your application specific structure handy.
// But, for the model and control iteration APIs, you have no way of recovering this pairing for the elements you iterate over.

// To alleviate this problem, Qwadro allows you to attach user-specific pointers to both the afxBody and the akxMotor objects.
// With each structure, you get an array of void pointers that you can read and write at will.
// You retrieve the array pointers like this:

AKX void**      AkxGetMotorUdd(akxMotor moto);

// Although a akxMotor has a clock set by AkxSetMotorClock, it also maintains a second internal clock that is animation-relative. 
// This is called the "control local clock", and it is synchronized such that 0 coincides with the beginning of the animation, 
// and the maximum meaningful value (equal to the duration of the animation) coincides with the end of the animation. 

AKX afxReal     AkxGetMotorLocalDuration(akxMotor moto); // Find out the duration of the local clock
AKX afxReal     AkxQueryMotorRawLocalClock(akxMotor moto); // Find out the animation-relative time for the control
AKX void        AkxAdjustMotorRawLocalClock(akxMotor moto, afxReal localClock); // Force the animation position back to X of the animation (0 = the beginning)

// Because AkxQueryMotorRawLocalClock can return a value less than zero or greater than the value of GetLocalDuration, 
// if you need a time guaranteed to be within the legal range for the animation, you need to ask for the clamped local clock specifically:

AKX afxReal     AkxGetMotorClampedLocalClock(akxMotor moto);

// The result of AkxGetMotorClampedLocalClock will be equal to AkxQueryMotorRawLocalClock if the local clock is within the valid range, 
// otherwise the clamped version will be clamped to 0 or AkxGetMotorLocalDuration. This is the value that is used during sampling, 
// so if the animation has under or over-flowed, it will simply stick on the first or last frame of the animation.

// Sometimes, you may not want to set the current state of a akxMotor, you may want to set its state at some other time in the future(or past).
// To do this, call SetTargetState with the current time, the time you want to set values for, and then the local clock and loop index that you want the control to have at that time.
// So if you want a akxMotor to be half a second through the third loop of its animation(loop index = 2) at a time three seconds in the future:

AKX void        AkxSetMotorTargetState(akxMotor moto, afxReal currGlobalTime, afxReal targetGlobalTime, afxReal localTime, afxInt currIterIdx);

// This function uses the existing speed and looping information of the control, so make sure you set those up first.
// Also note that this call just sets the current local clock and loop index, it does nothing clever in the future, so obviously if you later change the speed of the control, 
// the above target will not necessarily be hit.

// EaseIn and EaseOut are the most convenient ways to set the ease curves for a control, but sometimes you need a more direct method.
// First, you always have the option of enabling or disabling either curve:

AKX void        AkxEnableMotorEaseIn(akxMotor moto, afxBool easeIn);
AKX void        AkxEnableMotorEaseOut(akxMotor moto, afxBool easeOut);

// You can also explicitly set the start and end times, beginning and ending weights, and bezier tangents for both curves:

AKX void        AkxSetMotorEaseInCurve(akxMotor moto, afxReal startSeconds, afxReal endSeconds, afxReal startValue, afxReal startTangent, afxReal endTangent, afxReal endValue); // Ease the control in over the next two seconds
AKX void        AkxSetMotorEaseOutCurve(akxMotor moto, afxReal startSeconds, afxReal endSeconds, afxReal startValue, afxReal startTangent, afxReal endTangent, afxReal endValue); // In ten seconds, ease the control out over two seconds

// Note that the four bezier control parameters are the starting weight, the starting weight tangent, the ending weight tangent, and the ending weight, respectively. 
// They are interpreted as the four control points of a one-dimensional cubic bezier curve. Note that, because it is a bezier curve (and not a b-spline), 
// it is guarenteed to always start and end precisely at the first and last values you pass, regardless of what the two middle values (tangents) are. 
// The tangents control the shape of the curve only, and do not effect the start or end weight. 

// Every akxMotor is always in a particular looping state, based on whether or not it is on its first loop, last loop, a middle loop, or if it's simply not looping. 
// These conditions can be broken down into two boolean values: whether or not the akxMotor will loop if it underflows (if it needs to look at an animation value earlier in time), 
// and whether or not the akxMotor will loop if it overflows (if it needs to look at an animation value later in time). 
// The animation sampling code needs this information so it can determine whether to wrap or clamp the b-spline information should it read beyond either end of the animation data. 
// While it is rare for an application to need this information as well, you can always get it like this: 

AKX void        AkxQueryMotorIterationState(akxMotor moto, afxBool *underflowLoop, afxBool *overflowLoop); // Figure out what looping state the control is in

// Very occasionally(see warning below!), you want to have a single akxMotor controling an animation playing on multiple objects. 
// One example is playing a marching animation on a group of soldiers - sometimes it is more convenient to have a single akxMotor for all of them than to manipulate a list of controls, one per soldier.

// You could do this by having an animation with multiple track groups, and binding each to the object, but if you have 100 soldiers, that is 100 track groups to add to your animation, 
// which seems tedious and memory-hungry. You really only want to have the animation of a single soldier.



// Be very very careful using this call. A akxMotor has only a single duration, and large parts of Qwadro are set up to assume it is the correct one! For example, 
// be very careful if trying to bind multiple different animations to a single control. It is possible, and it does work, but only if they are exactly the same length. 
// You may get very strange behaviour and possibly also crashes(because the clamped local clock will not actually be in the valid range for one of more of the playing animations).
// This facility is very open to abuse, so be careful.

// It is also worth pointing out that doing this is usually not what you want anyway. There is no significant speed or memory penalty for creating a akxMotor for each model. 
// The akxMotor structure is allocated quickly, freed quickly, processed quickly, and is not very big. Typically, most of the time in Qwadro is spent sampling the animations into the local buffers, 
// and this still needs to be done for each model, even if they all use a single akxMotor.
// The disadvantage with using a single akxMotor is that all the models will play exactly the same animation at exactly the same rate, whereas in practice, even in a group of very well-trained soldiers, 
// you still want slight variations in time and speed of the animations. The only reason to do this is if you have extreme demands(tens of thousands of models playing the same animation), 
// or have other reasons that you must have a single akxMotor for all the instances.

//AKX akxMotor   AfxMotorCreate(afxReal currentClock, afxReal localDuration);
AKX afxReal     AkxGetMotorTerminationClock(akxMotor moto);
AKX afxBool     AkxMotorTerminationIsScheduled(akxMotor moto);
AKX void        AkxEnableMotorTerminationCheck(akxMotor moto, afxBool checkComplete);

AKX void        AkxSetMotorClockOnly(akxMotor moto, afxReal time);

AKX afxBool     AfxMotorHasTerminated(akxMotor moto);

AKX afxReal     AkxGetMotorWeight(akxMotor moto);
AKX void        AkxSetMotorWeight(akxMotor moto, afxReal weight);

AKX afxBool     AfxMotorIsActive(akxMotor moto);
AKX void        AkxActivateMotor(akxMotor moto, afxBool active); // deveria ser suspend, já que o motor já começa ativo

AKX afxReal     AkxGetMotorEaseCurveMultiplier(akxMotor moto);
AKX afxReal     AkxGetMotorEffectiveWeight(akxMotor moto);

AKX afxReal     AkxMotorEaseIn(akxMotor moto, afxReal duration, afxBool fromCurrent);
AKX afxReal     AkxMotorEaseOut(akxMotor moto, afxReal duration);

AKX void        AkxRecenterMotorClocks(akxMotor moto, afxReal currClock);
AKX void        AkxEnableMotorIterationClamping(akxMotor moto, afxBool clamp);

AKX afxBool     AfxMotorHasEffect(akxMotor moto);

////////////////////////////////////////////////////////////////////////////////

AKX afxError    AfxAcquireMotors(afxSimulation sim, afxReal currClock, afxReal localDur, afxNat cnt, akxMotor moto[]);

AKX afxBool     AkxReleaseTerminatedMotors(afxNat cnt, akxMotor motors[]);

AKX afxBool     AkxReleaseUnusedMotors(afxNat cnt, akxMotor motors[]);

AKX afxBool     AkxReleaseOnceUnusedMotors(afxNat cnt, akxMotor motors[]);

#endif//AFX_MOTOR_H
