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

#define _AFX_SIM_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/sim/anim/awxAnimus.h"

typedef struct
{
    afxSimulation sim;
    afxReal currClock;
    afxReal localDur;
} _afxPupParadigm;

// PUPPET INTERLINK

_AFXINL void AfxAccumulateAnimusInterlinkLoopTransform(awxAnimusInterlink *intk, afxReal secsElapsed, afxReal *totalWeight, afxReal translation[3], afxReal rotation[3], afxBool inverse)
{
    if (intk->callbacks->AccumulateLoopTransform)
        intk->callbacks->AccumulateLoopTransform(intk, secsElapsed, totalWeight, translation, rotation, inverse);
}

_AFXINL awxBody _AwxAnimusInterlinkGetBody(awxAnimusInterlink *intk)
{
    awxBody bod = AfxGetLinker(&intk->bod);
    return bod;
}

_AFXINL awxAnimus _AwxAnimusInterlinkGetAnimus(awxAnimusInterlink *intk)
{
    awxAnimus amus = AfxGetLinker(&intk->amus);
    return amus;
}

_AFXINL void _AwxAnimusInterlinkUnlinkBody(awxAnimusInterlink *intk)
{
    AfxPopLinkage(&intk->amus);
    AfxPopLinkage(&intk->bod);
}

_AFXINL void _AwxAnimusInterlinkLinkBody(awxAnimusInterlink *intk, awxAnimus amus, awxBody bod, afxBool active)
{
    AfxPushLinkage(&intk->amus, &amus->puppets);

    if (active)
        AfxPushLinkage(&intk->bod, &bod->motors);
    else
        AfxPushLinkageAtEnd(&intk->bod, &bod->motors);
}

_AFXINL void _AwxAnimusInterlinkRelinkBodySide(awxAnimusInterlink *intk, afxBool active)
{
    awxBody bod = _AwxAnimusInterlinkGetBody(intk);
    AfxPopLinkage(&intk->bod);

    if (active)
        AfxPushLinkage(&intk->bod, &bod->motors);
    else
        AfxPushLinkageAtEnd(&intk->bod, &bod->motors);
}

_AFXINL afxBool _AfxMotorHasEffect(awxAnimus amus)
{
    return (amus->flags & awxMotorFlags_ACTIVE) && amus->currentWeight != 0.0;
}

_AFXINL void _AfxMotorSortBindings(awxAnimus amus)
{
    afxBool active = _AfxMotorHasEffect(amus);

    awxAnimusInterlink *intk;
    AfxChainForEveryLinkage(&amus->puppets, awxAnimusInterlink, amus, intk)
    {
        _AwxAnimusInterlinkRelinkBodySide(intk, active);
    }
}

_AFXINL afxBool _AfxAnimusFreeIfUnused(awxAnimus amus);

_AFXINL void _AwxAnimusInterlinkFree(awxAnimusInterlink *intk)
{
    if (intk)
    {
        _AwxAnimusInterlinkUnlinkBody(intk);

        if (intk->callbacks->CleanupBindingState)
            intk->callbacks->CleanupBindingState(intk);

        awxAnimus amus = _AwxAnimusInterlinkGetAnimus(intk);
        _AfxAnimusFreeIfUnused(amus);
    }
    AfxDeallocate(NIL, intk);
}

_AFXINL awxAnimusInterlink* _AwxAnimusInterlinkCreate(afxSimulation sim, awxAnimusInterlinkCallbacks const *callbacks, awxAnimus amus, awxBody bod, afxBool active, void *InitData)
{
    awxAnimusInterlink* intk = AfxAllocate(NIL, 1, sizeof(*intk), 0, AfxHint());

    if (intk)
    {
        intk->callbacks = callbacks;
        intk->reservedPointer = 0;
        _AwxAnimusInterlinkLinkBody(intk, amus, bod, active);

        if (callbacks->InitializeBindingState)
            callbacks->InitializeBindingState(intk, InitData);
    }
    return intk;
}

_AFXINL void _FreeMotorRing(afxChain *ch)
{
    awxAnimusInterlink *intk;
    AfxChainForEveryLinkage(ch, awxAnimusInterlink, amus, intk)
    {
        _AwxAnimusInterlinkFree(intk);
    }
}

_AFXINL void _FreeBodyRing(afxChain *ch)
{
    awxAnimusInterlink *intk;
    AfxChainForEveryLinkage(ch, awxAnimusInterlink, bod, intk)
    {
        _AwxAnimusInterlinkFree(intk);
    }
}



// PUPPET



_AFX afxReal AwxGetAnimusCompletionClock(awxAnimus amus)
{
    return amus->killClock;
}

_AFX afxBool AwxGetAnimusCompletionCheckFlag(awxAnimus amus)
{
    return (amus->flags & awxMotorFlags_KILL_ONCE_COMPLETE) == awxMotorFlags_KILL_ONCE_COMPLETE;
}

_AFX void AwxSetAnimusCompletionCheckFlag(awxAnimus amus, afxBool checkComplete)
{
    if (checkComplete)
        amus->flags |= awxMotorFlags_KILL_ONCE_COMPLETE;
    else
        amus->flags &= (~(awxMotorFlags_KILL_ONCE_COMPLETE));
}

_AFX void AwxSetAnimusClockOnly(awxAnimus amus, afxReal time)
{
    amus->currentClock = time;
}

_AFX afxBool AfxMotorIsComplete(awxAnimus amus)
{
    return (amus->flags & awxMotorFlags_KILL_ONCE_COMPLETE) && amus->currentClock >= amus->killClock;
}

_AFX afxBool AwxFreeMotorIfComplete(awxAnimus amus)
{
    // Should be compatible with char FreeControlIfComplete(control *Control)

    if (!amus || !AfxMotorIsComplete(amus))
        return FALSE;

    while (0 < AfxReleaseObjects(1, (void*) { amus }));
    return TRUE;
}

_AFX afxReal AwxGetAnimusWeight(awxAnimus amus)
{
    return amus->currentWeight;
}

_AFX void AwxSetAnimusWeight(awxAnimus amus, afxReal weight)
{
    afxReal currWeight = amus->currentWeight;
    amus->currentWeight = weight;

    if (currWeight == 0.0)
    {
        if (0.0 == weight)
            return;

    LABEL_5:
        _AfxMotorSortBindings(amus);
        return;
    }

    if (weight == 0.0)
        goto LABEL_5;
}

//_AFX granny_track_mask const* AwxGetAnimusTrackGroupModelMask(awxAnimus amus, granny_model_instance const*  ModelInstance);
//_AFX granny_track_mask const* AwxGetAnimusTrackGroupTrackMask(awxAnimus amus, granny_animation const*  Animation, granny_int32x TrackGroupIndex);

_AFX afxBool AfxMotorIsActive(awxAnimus amus)
{
    return (amus->flags & awxMotorFlags_ACTIVE) == awxMotorFlags_ACTIVE;
}

_AFX void AwxSetAnimusActive(awxAnimus amus, afxBool active)
{
    if (((amus->flags & awxMotorFlags_ACTIVE) == awxMotorFlags_ACTIVE) != active)
    {
        if (active)
            amus->flags |= awxMotorFlags_ACTIVE;
        else
            amus->flags &= (~(awxMotorFlags_ACTIVE));

        _AfxMotorSortBindings(amus);
    }
}
_AFXINL void _UnpackEaseCurve(afxNat value, afxV4d result)
{
    result[0] = (afxReal)(afxNat8)value * 0.0039215689;
    result[1] = (afxReal)((afxNat16)value >> 8) * 0.0039215689;
    result[2] = (afxReal)((value >> 16) & 0xFF) * 0.0039215689;
    result[3] = (afxReal)(value >> 24) * 0.0039215689;
}

_AFXINL afxReal _ComputeEaseCurve(afxReal *Bezier, afxReal StartClock, afxReal CurrentClock, afxReal EndClock)
{
    afxReal v5; // st7

    afxReal v4 = EndClock - StartClock;

    if (v4 == 0.0)
    {
        if (CurrentClock < (double)StartClock)
            goto LABEL_3;
    LABEL_7:
        v5 = 1.0;
        return ((1.0 - v5) * Bezier[1] + v5 * Bezier[2]) * (1.0 - v5) * v5 * 3.0
            + v5 * Bezier[3] * v5 * v5
            + (1.0 - v5) * Bezier[0] * (1.0 - v5) * (1.0 - v5);
    }
    v5 = (CurrentClock - StartClock) / v4;
    if (v5 >= 0.0)
    {
        if (v5 <= 1.0)
            return ((1.0 - v5) * Bezier[1] + v5 * Bezier[2]) * (1.0 - v5) * v5 * 3.0
            + v5 * Bezier[3] * v5 * v5
            + (1.0 - v5) * Bezier[0] * (1.0 - v5) * (1.0 - v5);
        goto LABEL_7;
    }
LABEL_3:
    v5 = 0.0;
    return ((1.0 - v5) * Bezier[1] + v5 * Bezier[2]) * (1.0 - v5) * v5 * 3.0
        + v5 * Bezier[3] * v5 * v5
        + (1.0 - v5) * Bezier[0] * (1.0 - v5) * (1.0 - v5);
}

_AFX afxReal AwxGetAnimusEaseCurveMultiplier(awxAnimus amus)
{
    afxV4d easeInValues;
    _UnpackEaseCurve(amus->easeInValues, easeInValues);
    afxV4d easeOutValues;
    _UnpackEaseCurve(amus->easeOutValues, easeOutValues);

    afxReal result = 1.0;

    if (amus->easeInStartClock >= amus->easeOutStartClock && (amus->flags & awxMotorFlags_EASE_OUT) && amus->flags & awxMotorFlags_EASE_IN)
    {
        if (amus->currentClock > amus->easeOutEndClock)
        {
            if (amus->currentClock > amus->easeInStartClock)
            {
                if (amus->currentClock <= amus->easeInEndClock)
                    result = _ComputeEaseCurve(easeInValues, amus->easeInStartClock, amus->currentClock, amus->easeInEndClock);
            }
            else
            {
                afxReal v3 = amus->currentClock - amus->easeOutEndClock;
                afxReal v4 = amus->easeInStartClock - amus->easeOutEndClock;
                result = (1.0 - v3 / v4) * easeOutValues[3] + easeInValues[0] * (v3 / v4);
            }
        }
        else
        {
            result = _ComputeEaseCurve(easeOutValues, amus->easeOutStartClock, amus->currentClock, amus->easeOutEndClock);
        }
    }
    else
    {
        if ((amus->flags & awxMotorFlags_EASE_OUT) && amus->currentClock >= amus->easeOutStartClock)
            result = _ComputeEaseCurve(easeOutValues, amus->easeOutStartClock, amus->currentClock, amus->easeOutEndClock);

        if ((amus->flags & awxMotorFlags_EASE_IN) && amus->currentClock <= amus->easeInEndClock)
            result = _ComputeEaseCurve(easeInValues, amus->easeInStartClock, amus->currentClock, amus->easeInEndClock);
    }
    return result;
}

_AFX afxReal AwxGetAnimusEffectiveWeight(awxAnimus amus)
{
    return AfxMotorIsActive(amus) ? (AwxGetAnimusEaseCurveMultiplier(amus) * amus->currentWeight) : 0.0;
}

_AFX afxReal AwxAnimusEaseIn(awxAnimus amus, afxReal Duration, afxBool fromCurrent)
{
    afxReal currentClock = AwxGetAnimusClock(amus);
    afxReal multiplier = fromCurrent ? AwxGetAnimusEaseCurveMultiplier(amus) : 0.0;    
    AwxSetAnimusEaseIn(amus, TRUE);
    AwxSetAnimusEaseOut(amus, FALSE);
    afxReal fromCurrenta = currentClock + Duration;
    AwxSetAnimusEaseInCurve(amus, currentClock, fromCurrenta, multiplier, multiplier, 1.0, 1.0);
    return fromCurrenta;
}

_AFX afxReal AwxAnimusEaseOut(awxAnimus amus, afxReal duration)
{
    afxReal currentClock = AwxGetAnimusClock(amus);
    afxReal multiplier = AwxGetAnimusEaseCurveMultiplier(amus);
    AwxSetAnimusEaseIn(amus, FALSE);
    AwxSetAnimusEaseOut(amus, TRUE);
    afxReal durationa = currentClock + duration;
    AwxSetAnimusEaseOutCurve(amus, currentClock, durationa, multiplier, multiplier, 0.0, 0.0);
    return durationa;
}

_AFX void AwxRecenterAnimusClocks(awxAnimus amus, afxReal currClock)
{
    amus->currentClock = currClock + amus->currentClock;
    amus->killClock = currClock + amus->killClock;
    amus->easeInStartClock = currClock + amus->easeInStartClock;
    amus->easeInEndClock = currClock + amus->easeInEndClock;
    amus->easeOutStartClock = currClock + amus->easeOutStartClock;
    amus->easeOutEndClock = currClock + amus->easeOutEndClock;
}

_AFX void AwxForceAnimusClampedLooping(awxAnimus amus, afxBool clamp)
{
    if (clamp)
        amus->flags |= awxMotorFlags_FORCE_CLAMPLED_LOOPS;
    else
        amus->flags &= (~(awxMotorFlags_FORCE_CLAMPLED_LOOPS));
}

_AFX void AwxGetAnimusLoopState(awxAnimus amus, afxBool *underflowLoop, afxBool *overflowLoop)
{
    AwxGetAnimusRawLocalClock(amus);

    if (amus->flags & awxMotorFlags_FORCE_CLAMPLED_LOOPS)
    {
        *overflowLoop = 0;
        *underflowLoop = 0;
    }
    else if (amus->loopCnt)
    {
        *underflowLoop = amus->loopIdx > 0;
        *overflowLoop = amus->loopIdx < amus->loopCnt - 1;
    }
    else
    {
        *overflowLoop = 1;
        *underflowLoop = 1;
    }
}

_AFX void _AfxAnimusModulusLocalClock(awxAnimus amus)
{
    afxReal v1 = amus->localClock / amus->localDur;
    afxInt v2 = amus->loopIdx;
    afxInt v3 = v2 + (afxInt)v1;

    if (amus->localClock < 0.0)
        --v3;

    afxInt v4 = amus->loopCnt;

    if (v4)
    {
        if (v3 >= 0)
        {
            if (v3 >= v4)
                v3 = v4 - 1;
        }
        else
        {
            v3 = 0;
        }
    }

    afxInt v5 = v3 - v2;

    if (v5)
    {
        amus->loopIdx = v5 + v2;
        amus->localClock = amus->localClock - (afxReal)v5 * amus->localDur;
    }
}

_AFX void AwxSetAnimusTargetState(awxAnimus amus, afxReal currentGlobalTime, afxReal targetGlobalTime, afxReal localTime, afxInt loopIndex)
{
    afxReal t = (targetGlobalTime - currentGlobalTime) * amus->speed;
    amus->dTLocalClockPending = 0.0;
    amus->currentClock = currentGlobalTime;
    amus->loopIdx = loopIndex;
    amus->localClock = localTime - t;
    _AfxAnimusModulusLocalClock(amus);
}

_AFX void AwxSetAnimusEaseIn(awxAnimus amus, afxBool easeIn)
{
    if (easeIn)
        amus->flags |= awxMotorFlags_EASE_IN;
    else
        amus->flags &= (~(awxMotorFlags_EASE_IN));
}

_AFX void AwxSetAnimusEaseOut(awxAnimus amus, afxBool easeOut)
{
    if (easeOut)
        amus->flags |= awxMotorFlags_EASE_OUT;
    else
        amus->flags &= (~(awxMotorFlags_EASE_OUT));
}

_AFX void _PackEaseCurve(afxNat *result, afxReal Af, afxReal Bf, afxReal Cf, afxReal Df)
{
    *result = ((afxInt)(Af * 255.0)) | ((((afxInt)(Bf * 255.0)) | ((((afxInt)(Cf * 255.0)) | ((afxInt)((afxInt)(Df * 255.0)) << 8)) << 8)) << 8);
}

_AFX void AwxSetAnimusEaseInCurve(awxAnimus amus, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    amus->easeInStartClock = StartSeconds;
    amus->easeInEndClock = EndSeconds;
    _PackEaseCurve(&amus->easeInValues, StartValue, StartTangent, EndTangent, EndValue);
}

_AFX void AwxSetAnimusEaseOutCurve(awxAnimus amus, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    amus->easeOutStartClock = StartSeconds;
    amus->easeOutEndClock = EndSeconds;
    _PackEaseCurve(&amus->easeOutValues, StartValue, StartTangent, EndTangent, EndValue);
}

_AFX afxReal AwxGetAnimusClampedLocalClock(awxAnimus amus)
{
    afxReal result = AwxGetAnimusRawLocalClock(amus);

    if (result < 0.0)
        return 0.0;

    if (result > amus->localDur)
        result = amus->localDur;

    return result;
}

_AFX afxReal AwxGetAnimusLocalDuration(awxAnimus amus)
{
    return amus->localDur;
}

_AFX afxReal AwxGetAnimusRawLocalClock(awxAnimus amus)
{
    unsigned __int8 v3 = 0; // c0
    unsigned __int8 v4 = 0; // c2

    if (amus->dTLocalClockPending != 0.0)
    {
        afxReal v1 = amus->speed * amus->dTLocalClockPending + amus->localClock;
        amus->localClock = v1;

        if (v3 | v4) // wtf?
        {
            if (amus->loopCnt && amus->loopIdx <= 0)
                goto LABEL_10;
        }
        else
        {
            if (v1 < amus->localDur)
                goto LABEL_10;

            if (amus->loopCnt)
            {
                if (amus->loopIdx >= amus->loopCnt - 1)
                    goto LABEL_10;
            }
        }
        _AfxAnimusModulusLocalClock(amus);
    LABEL_10:
        amus->dTLocalClockPending = 0.0;
    }
    return amus->localClock;
}

_AFX void AwxSetAnimusRawLocalClock(awxAnimus amus, afxReal LocalClock)
{
    amus->dTLocalClockPending = 0.0;
    amus->localClock = LocalClock;
}

_AFX void** AwxGetAnimusUserDataArray(awxAnimus amus)
{
    return amus->userData;
}

_AFX afxReal AwxGetAnimusDuration(awxAnimus amus)
{
    afxReal result = 1.9140881e38;

    if (amus->loopCnt)
    {
        if (amus->speed > 0.0)
            return amus->localDur / amus->speed * amus->loopCnt;

        if (amus->speed < 0.0)
            result = -(amus->localDur / amus->speed) * amus->loopCnt;
    }
    return result;
}

_AFX afxReal AwxGetAnimusDurationLeft(awxAnimus amus)
{
    afxReal rawLocalClock = AwxGetAnimusRawLocalClock(amus);
    afxInt loopCnt = amus->loopCnt;
    
    if (!loopCnt)
        return 1.9140881e38;

    if (amus->speed > 0.0)
    {
        afxInt loopIdx = amus->loopIdx;

        if (loopIdx < loopCnt)
            return (afxReal)(loopCnt - loopIdx - 1) * (1.0 / amus->speed * amus->localDur) + (amus->localDur - rawLocalClock) * (1.0 / amus->speed);

        return 0.0;
    }

    if (amus->speed >= 0.0)
        return 1.9140881e38;

    if (amus->loopIdx < 0)
        return 0.0;

    return -(1.0 / amus->speed * amus->localDur) * (double)amus->loopIdx - 1.0 / amus->speed * rawLocalClock;
}

_AFX afxInt AwxGetAnimusIteration(awxAnimus amus)
{
    AwxGetAnimusRawLocalClock(amus);
    return amus->loopIdx;
}

_AFX void AwxSetAnimusIteration(awxAnimus amus, afxInt loopIdx)
{
    amus->loopIdx = loopIdx;
}

_AFX afxInt AwxGetAnimusLoopCount(awxAnimus amus)
{
    return amus->loopCnt;
}

_AFX void AwxSetAnimusLoopCount(awxAnimus amus, afxInt loopCnt)
{
    amus->loopCnt = loopCnt;
}

_AFX afxReal AwxGetAnimusSpeed(awxAnimus amus)
{
    return amus->speed;
}

_AFX void AwxSetAnimusSpeed(awxAnimus amus, afxReal speed)
{
    amus->speed = speed;
}

_AFX afxReal AwxGetAnimusClock(awxAnimus amus)
{
    return amus->currentClock;
}

_AFX void AwxSetAnimusClock(awxAnimus amus, afxReal time)
{
    afxReal dt = time - amus->currentClock;
    amus->currentClock = time;
    amus->dTLocalClockPending = dt + amus->dTLocalClockPending;
}

_AFXINL afxBool _AfxAnimusIsUnused(awxAnimus amus)
{
    return AfxChainIsEmpty(&amus->puppets);
}

_AFXINL afxBool _AfxAnimusFreeIfUnused(awxAnimus amus)
{
    if (!amus || !(amus->flags & awxMotorFlags_KILL_ONCE_UNUSED) || !_AfxAnimusIsUnused(amus))
        return FALSE;

    while (0 < AfxReleaseObjects(1, (void*) { amus }));
    return TRUE;
}

_AFX void AwxFreeAnimusOnceUnused(awxAnimus amus)
{
    amus->flags |= awxMotorFlags_KILL_ONCE_UNUSED;
    _AfxAnimusFreeIfUnused(amus);
}

_AFX void AfxCompleteAnimusAt(awxAnimus amus, afxReal atSecs)
{
    amus->flags |= awxMotorFlags_KILL_ONCE_COMPLETE;
    amus->killClock = atSecs;
}

_AFX afxError _AfxAmusDtor(awxAnimus amus)
{
    AfxEntry("amus=%p", amus);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &amus, afxFcc_AMUS);

    amus->flags &= 0xFFFFFFF8;
    _FreeMotorRing(&amus->puppets);

    return err;
}

_AFX afxError _AfxAmusCtor(awxAnimus amus, afxCookie const *cookie)
{
    AfxEntry("amus=%p", amus);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &amus, afxFcc_AMUS);

    afxReal currClock = *(afxReal*)cookie->udd[0];
    afxReal localDur = *(afxReal*)cookie->udd[1];

    AfxTakeChain(&amus->puppets, amus);
    amus->flags = awxMotorFlags_ACTIVE;
    amus->currentClock = currClock;
    amus->dTLocalClockPending = 0.0;
    amus->localClock = 0.0;
    amus->speed = 1.0;
    amus->localDur = localDur;
    amus->loopIdx = 0;
    amus->loopCnt = 1;
    amus->killClock = 0;
    amus->currentWeight = 1;
    amus->easeInStartClock = 0;
    amus->easeInEndClock = 0;
    amus->easeInValues = (afxNat)-65536;
    amus->easeOutStartClock = 0;
    amus->easeOutEndClock = 0;
    amus->easeOutValues = (afxNat)0xFFFF;

    return err;
}

_AFX afxClassConfig _AfxMotClsConfig =
{
    .fcc = afxFcc_AMUS,
    .name = "Animus",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(awxAnimus)),
    .mmu = NIL,
    .ctor = (void*)_AfxAmusCtor,
    .dtor = (void*)_AfxAmusDtor
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireAnimuses(afxSimulation sim, afxNat cnt, awxAnimus amus[], afxReal currClock, afxReal localDur)
{
    AfxEntry("sim=%p,currClock=%f,localDur=%f", sim, currClock, localDur);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    
    if (AfxAcquireObjects(AwxGetAnimusClass(sim), cnt, (afxObject*)amus, (void const*[]) { &currClock, &localDur }))
        AfxThrowError();

    return err;
}

_AFX void AfxRecenterAllAnimusClocks(afxSimulation sim, afxReal dCurrentClock)
{
    afxClass *cls = AwxGetAnimusClass(sim);
    afxNat cnt = AfxCountInstances(cls);

    for (afxNat i = 0; i < cnt; i++)
    {
        awxAnimus amus;

        if (AfxEnumerateInstances(cls, i, 1, (void*)&amus))
        {
            AwxRecenterAnimusClocks(amus, dCurrentClock);
        }
    }
}
