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
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                      (c 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_BODY_CONTROL_H
#define AFX_BODY_CONTROL_H

// Terminology: Puppet is a movable model of a person or animal that is used in entertainment and is typically moved either by strings controlled from above or by a hand inside it.

// There are two primary objects involved in the run-time animation layer : the afxBody and the afxAnimator.
// Together, they represent the two ends of the animation process.
// The afxBody represents the state of a single model that may have some number of animations affecting it, 
// whereas the afxAnimator represents the state of an animation that may be affecting some number of models.
// When your engine creates a new animating entity in the game world, you create a afxBody to accompany it.
// When you want to play an animation on such an instance, you create a afxAnimator.

#include "afxBody.h"

AFX_DEFINE_HANDLE(afxAnimator);

AFX_DECLARE_STRUCT(afxControlledAnimation);
AFX_DECLARE_STRUCT(afxControlledPose);
AFX_DECLARE_STRUCT(afxPose);
AFX_DECLARE_STRUCT(afxWorldPose);

AFX_DECLARE_STRUCT(afxAnimatorInterlinkCallbacks);

AFX_DEFINE_STRUCT(afxAnimatorInterlink)
{
    afxLinkage anir;
    afxLinkage bod;
    afxAnimatorInterlinkCallbacks const *callbacks;
    unsigned int derived[4];
    void *reservedPointer;
};

AFX_DEFINE_STRUCT(afxAnimatorInterlinkCallbacks)
{
    afxControlledAnimation*(*GetControlledAnimation)(afxAnimatorInterlink*);
    afxControlledPose*(*GetControlledPose)(afxAnimatorInterlink*);
    afxBool(*InitializeBindingState)(afxAnimatorInterlink*, void*);
    void(*AccumulateBindingState)(afxAnimatorInterlink*, afxInt, afxInt, afxPose*, afxReal, afxInt const*);
    void(*BuildBindingDirect)(afxAnimatorInterlink*, afxInt, afxReal const*, afxWorldPose*, afxReal);
    void(*AccumulateLoopTransform)(afxAnimatorInterlink*, afxReal, afxReal*, afxReal*, afxReal*, afxBool);
    void(*CleanupBindingState)(afxAnimatorInterlink*);
};

typedef enum afxAnimatorFlags
{
    afxAnimatorFlags_ACTIVE = AFX_BIT_OFFSET(0),
    afxAnimatorFlags_KILL_ONCE_COMPLETE = AFX_BIT_OFFSET(1),
    afxAnimatorFlags_KILL_ONCE_UNUSED = AFX_BIT_OFFSET(2),
    afxAnimatorFlags_EASE_IN = AFX_BIT_OFFSET(3),
    afxAnimatorFlags_EASE_OUT = AFX_BIT_OFFSET(4),
    afxAnimatorFlags_FORCE_CLAMPLED_LOOPS = AFX_BIT_OFFSET(5)
} afxAnimatorFlags;

AFX_OBJECT(afxAnimator)
{
    afxChain puppets;
    afxNat flags;
    afxReal currentClock;
    afxReal dTLocalClockPending;
    afxReal localClock;
    afxReal speed;
    afxReal localDur;
    afxInt loopIdx;
    afxInt loopCnt;
    afxReal killClock;
    afxReal currentWeight;
    afxReal easeInStartClock;
    afxReal easeInEndClock;
    afxNat easeInValues;
    afxReal easeOutStartClock;
    afxReal easeOutEndClock;
    afxNat easeOutValues;
    void* userData[4];
};


AFX afxError        AfxAcquireAnimators(afxSimulation sim, afxNat cnt, afxAnimator bodc[], afxReal currClock, afxReal localDur);


// The afxAnimator handle you get back when you play an animation is the handle you use to manipulate the animation during playback.
// You can perform a number of timing and weighting operations through the afxAnimator, but before we get to that, 
// it's important to note that as long as you hold a control handle, Qwadro considers its corresponding animation to be active.
// Thus, for every afxAnimator that you create through calls like PlayControlledAnimation, 
// you must have a corresponding call to free the control. Here are some of the ways you can do that:

//AFX void        AfxAnimatorFree(afxAnimator anir); // // Free the control (and stop the animation) immediately
AFX void        AfxFreeAnimatorOnceUnused(afxAnimator anir); // Free the control once all the afxBody objects it affects have been freed
AFX void        AfxCompleteAnimatorAt(afxAnimator anir, afxReal atSecs); // Free the control X seconds

// Each afxAnimator keeps track of what time it thinks it is.
// This is an absolute time(see Basic Model Setup and Animation: A Brief Discussion of Time), and can be directly queried or modified:

AFX afxReal     AfxGetAnimatorClock(afxAnimator anir); // // Find out what time the control thinks it is
AFX void        AfxSetAnimatorClock(afxAnimator anir, afxReal time); // Tell it to advance by X second

// Modifying a afxAnimator's clock will cause all its animations to be advanced appropriately. 
// However, they will not necessarily advance by the amount of time you advanced the clock. 
// The speed of the control acts as a time multiplier on the control's animations.

// The default speed setting is 1.0f, which keeps animations advancing in lock-step with the control's clock. 
// A fractional setting makes the animations play back slower than normal, and a setting of greater than 1.0f makes them play back faster than normal. 
// A negative setting makes the animations play backwards. Here are some examples: 

AFX afxReal     AfxGetAnimatorSpeed(afxAnimator anir); // Find out how fast the control's animations are playing
AFX void        AfxSetAnimatorSpeed(afxAnimator anir, afxReal speed); // Have the control play its animations X times fast. (0.5 = half as fast as real time, 10 = ten times faster than real time, 0 = Stop all regardless of the clock, -1.0 = play backwards)

// Animations can be looped, either indefinitely or for a specific number of loops. The loop count is queried and modified like this: 

AFX afxInt      AfxGetAnimatorLoopCount(afxAnimator anir); // Find out how many loops this control is set to do (by default, it will be 1)
AFX void        AfxSetAnimatorLoopCount(afxAnimator anir, afxInt loopCnt); // Make the control do X loops. (0 = loop indefinitely)

// A loop count of 0 indicates the special case of infinite looping.Animations played by a control with a loop count of 0 will play forever.

// In addition to setting the loop count, it is also possible to find out or modify how many loops the control has done. 
// This is done by querying or modifying the loop index:

AFX afxInt      AfxGetAnimatorIteration(afxAnimator anir); // Find out what loop the control is on
AFX void        AfxSetAnimatorIteration(afxAnimator anir, afxInt loopIdx); // Make the control think it's done X loops already

// Since the afxAnimator object gives you a lot of control over how its animations are played, 
// it can be difficult for you to determine how long an animation might take to play back given a certain control state. 
// To make this easy, Qwadro supplies two helper functions that do the calculations for you:

AFX afxReal     AfxGetAnimatorDuration(afxAnimator anir); // Find out how long this control plays for, total
AFX afxReal     AfxGetAnimatorDurationLeft(afxAnimator anir); // Find out how long this control will continue to play for, accounting for however much time has already elapsed

// Often times, you will want to pair a afxBody or a afxAnimator in Qwadro with some application-specific structure of your own.
// This does not pose a problem for most Qwadro APIs, since you will always have your application specific structure handy.
// But, for the model and control iteration APIs, you have no way of recovering this pairing for the elements you iterate over.

// To alleviate this problem, Qwadro allows you to attach user-specific pointers to both the afxBody and the afxAnimator objects.
// With each structure, you get an array of GrannyMaximumUserData void pointers that you can read and write at will.
// You retrieve the array pointers like this:

AFX void**      AfxGetAnimatorUserDataArray(afxAnimator anir);

// Although a afxAnimator has a clock set by AfxSetAnimatorClock, it also maintains a second internal clock that is animation-relative. 
// This is called the "control local clock", and it is synchronized such that 0 coincides with the beginning of the animation, 
// and the maximum meaningful value (equal to the duration of the animation) coincides with the end of the animation. 

AFX afxReal     AfxGetAnimatorLocalDuration(afxAnimator anir); // Find out the duration of the local clock
AFX afxReal     AfxGetAnimatorRawLocalClock(afxAnimator anir); // Find out the animation-relative time for the control
AFX void        AfxSetAnimatorRawLocalClock(afxAnimator anir, afxReal localClock); // Force the animation position back to X of the animation (0 = the beginning)

// Because AfxGetAnimatorRawLocalClock can return a value less than zero or greater than the value of GetLocalDuration, 
// if you need a time guaranteed to be within the legal range for the animation, you need to ask for the clamped local clock specifically:

AFX afxReal     AfxGetAnimatorClampedLocalClock(afxAnimator anir);

// The result of AfxGetAnimatorClampedLocalClock will be equal to AfxGetAnimatorRawLocalClock if the local clock is within the valid range, 
// otherwise the clamped version will be clamped to 0 or AfxGetAnimatorLocalDuration. This is the value that is used during sampling, 
// so if the animation has under or over-flowed, it will simply stick on the first or last frame of the animation.

// Sometimes, you may not want to set the current state of a afxAnimator, you may want to set its state at some other time in the future(or past).
// To do this, call SetTargetState with the current time, the time you want to set values for, and then the local clock and loop index that you want the control to have at that time.
// So if you want a afxAnimator to be half a second through the third loop of its animation(loop index = 2) at a time three seconds in the future:

AFX void        AfxSetAnimatorTargetState(afxAnimator anir, afxReal currGlobalTime, afxReal targetGlobalTime, afxReal localTime, afxInt loopIdx);

// This function uses the existing speed and looping information of the control, so make sure you set those up first.
// Also note that this call just sets the current local clock and loop index, it does nothing clever in the future, so obviously if you later change the speed of the control, 
// the above target will not necessarily be hit.

// EaseIn and EaseOut are the most convenient ways to set the ease curves for a control, but sometimes you need a more direct method.
// First, you always have the option of enabling or disabling either curve:

AFX void        AfxSetAnimatorEaseIn(afxAnimator anir, afxBool easeIn);
AFX void        AfxSetAnimatorEaseOut(afxAnimator anir, afxBool easeOut);

// You can also explicitly set the start and end times, beginning and ending weights, and bezier tangents for both curves:

AFX void        AfxSetAnimatorEaseInCurve(afxAnimator anir, afxReal startSeconds, afxReal endSeconds, afxReal startValue, afxReal startTangent, afxReal endTangent, afxReal endValue); // Ease the control in over the next two seconds
AFX void        AfxSetAnimatorEaseOutCurve(afxAnimator anir, afxReal startSeconds, afxReal endSeconds, afxReal startValue, afxReal startTangent, afxReal endTangent, afxReal endValue); // In ten seconds, ease the control out over two seconds

// Note that the four bezier control parameters are the starting weight, the starting weight tangent, the ending weight tangent, and the ending weight, respectively. 
// They are interpreted as the four control points of a one-dimensional cubic bezier curve. Note that, because it is a bezier curve (and not a b-spline), 
// it is guarenteed to always start and end precisely at the first and last values you pass, regardless of what the two middle values (tangents) are. 
// The tangents control the shape of the curve only, and do not effect the start or end weight. 

// Every afxAnimator is always in a particular looping state, based on whether or not it is on its first loop, last loop, a middle loop, or if it's simply not looping. 
// These conditions can be broken down into two boolean values: whether or not the afxAnimator will loop if it underflows (if it needs to look at an animation value earlier in time), 
// and whether or not the afxAnimator will loop if it overflows (if it needs to look at an animation value later in time). 
// The animation sampling code needs this information so it can determine whether to wrap or clamp the b-spline information should it read beyond either end of the animation data. 
// While it is rare for an application to need this information as well, you can always get it like this: 

AFX void        AfxGetAnimatorLoopState(afxAnimator anir, afxBool *underflowLoop, afxBool *overflowLoop); // Figure out what looping state the control is in

// Very occasionally(see warning below!), you want to have a single afxAnimator controling an animation playing on multiple objects. 
// One example is playing a marching animation on a group of soldiers - sometimes it is more convenient to have a single afxAnimator for all of them than to manipulate a list of controls, one per soldier.

// You could do this by having an animation with multiple track groups, and binding each to the object, but if you have 100 soldiers, that is 100 track groups to add to your animation, 
// which seems tedious and memory-hungry. You really only want to have the animation of a single soldier.

// You can do this by creating controlled animations using BeginControlledAnimation and EndControlledAnimation as usual. Normally, each pair would create a new control. 
// But you can tell it not to do so by inserting the UseExistingControlForAnimation call between the two. 
// What this says is that you want it to tag the animation onto an existing afxAnimator rather than create a new one.

#if  0
granny_control TheControl = NULL;
granny_real32 StartTime = (the start time);
granny_animation TheAnimation = (the animation to play);
granny_int32x TrackGroup = (the track group in the animation);
granny_model_instance *ModelInstances[100];
for (int i = 0; i < 100; i++)
{
    granny_controlled_animation_builder Builder =
        GrannyBeginControlledAnimation(StartTime, TheAnimation);
    GrannySetTrackGroupTarget(Builder, TrackGroup, ModelInstances[i]);
    if (i > 0)
    {
        GrannyUseExistingControlForAnimation(Builder, TheControl);
    }
    TheControl = GrannyEndControlledAnimation(Builder);
}
#endif

// Be very very careful using this call. A afxAnimator has only a single duration, and large parts of Qwadro are set up to assume it is the correct one! For example, 
// be very careful if trying to bind multiple different animations to a single control. It is possible, and it does work, but only if they are exactly the same length. 
// You may get very strange behaviour and possibly also crashes(because the clamped local clock will not actually be in the valid range for one of more of the playing animations).
// This facility is very open to abuse, so be careful.

// It is also worth pointing out that doing this is usually not what you want anyway. There is no significant speed or memory penalty for creating a afxAnimator for each model. 
// The afxAnimator structure is allocated quickly, freed quickly, processed quickly, and is not very big. Typically, most of the time in Qwadro is spent sampling the animations into the local buffers, 
// and this still needs to be done for each model, even if they all use a single afxAnimator.
// The disadvantage with using a single afxAnimator is that all the models will play exactly the same animation at exactly the same rate, whereas in practice, even in a group of very well-trained soldiers, 
// you still want slight variations in time and speed of the animations. The only reason to do this is if you have extreme demands(tens of thousands of models playing the same animation), 
// or have other reasons that you must have a single afxAnimator for all the instances.

//AFX afxAnimator   AfxAnimatorCreate(afxReal currentClock, afxReal localDuration);
AFX afxReal     AfxGetAnimatorCompletionClock(afxAnimator anir);
AFX afxBool     AfxGetAnimatorCompletionCheckFlag(afxAnimator anir);
AFX void        AfxSetAnimatorCompletionCheckFlag(afxAnimator anir, afxBool checkComplete);
AFX void        AfxSetAnimatorClockOnly(afxAnimator anir, afxReal time);
AFX afxBool     AfxAnimatorIsComplete(afxAnimator anir);
AFX afxBool     AfxFreeAnimatorIfComplete(afxAnimator anir);
AFX afxReal     AfxAnimatorGetWeight(afxAnimator anir);
AFX void        AfxAnimatorSetWeight(afxAnimator anir, afxReal weight);
//AFX granny_track_mask const* AfxAnimatorGetTrackGroupModelMask(afxAnimator anir, granny_model_instance const*  ModelInstance);
//AFX granny_track_mask const* AfxAnimatorGetTrackGroupTrackMask(afxAnimator anir, granny_animation const*  Animation, granny_int32x TrackGroupIndex);
AFX afxBool     AfxAnimatorIsActive(afxAnimator anir);
AFX void        AfxSetAnimatorActive(afxAnimator anir, afxBool active);
AFX afxReal     AfxGetAnimatorEaseCurveMultiplier(afxAnimator anir);
AFX afxReal     AfxGetAnimatorEffectiveWeight(afxAnimator anir);
AFX afxReal     AfxAnimatorEaseIn(afxAnimator anir, afxReal duration, afxBool fromCurrent);
AFX afxReal     AfxAnimatorEaseOut(afxAnimator anir, afxReal duration);

AFX void        AfxRecenterAnimatorClocks(afxAnimator anir, afxReal currClock);
AFX void        AfxForceAnimatorClampedLooping(afxAnimator anir, afxBool clamp);



// To iterate over the controls that affect a model, you use a loop like this:

//for (granny_model_control_binding *Binding = GrannyModelControlsBegin(ModelInstance); Binding != GrannyModelControlsEnd(ModelInstance); Binding = GrannyModelControlsNext(Binding))
//{
//    granny_control *Control = GrannyGetControlFromBinding(Binding);
//}

/*
AFX granny_model_control_binding * GrannyModelControlsBegin(afxBody Model);
AFX granny_model_control_binding * GrannyModelControlsNext(granny_model_control_binding * Binding);
AFX granny_model_control_binding * GrannyModelControlsEnd(afxBody Model);
AFX granny_model_control_binding * GrannyControlModelsBegin(afxAnimator anir);
AFX granny_model_control_binding * GrannyControlModelsNext(granny_model_control_binding * Binding);
AFX granny_model_control_binding * GrannyControlModelsEnd(afxAnimator anir);
AFX afxBody GrannyGetModelInstanceFromBinding(granny_model_control_binding * Binding);
AFX afxAnimator GrannyGetControlFromBinding(granny_model_control_binding * Binding);
*/


AFXINL afxBody _AfxAnimatorInterlinkGetBody(afxAnimatorInterlink *intk);
AFXINL afxAnimator _AfxAnimatorInterlinkGetAnimator(afxAnimatorInterlink *intk);
AFXINL void AfxAccumulateAnimatorInterlinkLoopTransform(afxAnimatorInterlink *intk, afxReal secsElapsed, afxReal *totalWeight, afxReal translation[3], afxReal rotation[3], afxBool inverse);
AFXINL afxBool _AfxAnimatorHasEffect(afxAnimator anir);
AFXINL void _FreeBodyRing(afxChain *ch);

#endif//AFX_BODY_CONTROL_H