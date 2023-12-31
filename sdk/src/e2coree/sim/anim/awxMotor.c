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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_SIM_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/sim/anim/awxMotor.h"

typedef struct
{
    afxSimulation sim;
    afxReal currClock;
    afxReal localDur;
} _afxPupParadigm;

// PUPPET INTERLINK

_AFXINL void AfxAccumulateMotorInterlinkLoopTransform(awxMotorInterlink *intk, afxReal secsElapsed, afxReal *totalWeight, afxReal translation[3], afxReal rotation[3], afxBool inverse)
{
    if (intk->callbacks->AccumulateLoopTransform)
        intk->callbacks->AccumulateLoopTransform(intk, secsElapsed, totalWeight, translation, rotation, inverse);
}

_AFXINL awxBody _AwxMotorInterlinkGetBody(awxMotorInterlink *intk)
{
    awxBody bod = AfxGetLinker(&intk->bod);
    return bod;
}

_AFXINL awxMotor _AwxMotorInterlinkGetMotor(awxMotorInterlink *intk)
{
    awxMotor mot = AfxGetLinker(&intk->mot);
    return mot;
}

_AFXINL void _AwxMotorInterlinkUnlinkBody(awxMotorInterlink *intk)
{
    AfxPopLinkage(&intk->mot);
    AfxPopLinkage(&intk->bod);
}

_AFXINL void _AwxMotorInterlinkLinkBody(awxMotorInterlink *intk, awxMotor mot, awxBody bod, afxBool active)
{
    AfxPushLinkage(&intk->mot, &mot->puppets);

    if (active)
        AfxPushLinkage(&intk->bod, &bod->motors);
    else
        AfxPushLinkageAtEnd(&intk->bod, &bod->motors);
}

_AFXINL void _AwxMotorInterlinkRelinkBodySide(awxMotorInterlink *intk, afxBool active)
{
    awxBody bod = _AwxMotorInterlinkGetBody(intk);
    AfxPopLinkage(&intk->bod);

    if (active)
        AfxPushLinkage(&intk->bod, &bod->motors);
    else
        AfxPushLinkageAtEnd(&intk->bod, &bod->motors);
}

_AFXINL afxBool _AfxMotorHasEffect(awxMotor mot)
{
    return (mot->flags & awxMotorFlags_ACTIVE) && mot->currentWeight != 0.0;
}

_AFXINL void _AfxMotorSortBindings(awxMotor mot)
{
    afxBool active = _AfxMotorHasEffect(mot);

    awxMotorInterlink *intk;
    AfxChainForEveryLinkage(&mot->puppets, awxMotorInterlink, mot, intk)
    {
        _AwxMotorInterlinkRelinkBodySide(intk, active);
    }
}

_AFXINL afxBool _AfxMotorFreeIfUnused(awxMotor mot);

_AFXINL void _AwxMotorInterlinkFree(awxMotorInterlink *intk)
{
    if (intk)
    {
        _AwxMotorInterlinkUnlinkBody(intk);

        if (intk->callbacks->CleanupBindingState)
            intk->callbacks->CleanupBindingState(intk);

        awxMotor mot = _AwxMotorInterlinkGetMotor(intk);
        _AfxMotorFreeIfUnused(mot);
    }
    AfxDeallocate(NIL, intk);
}

_AFXINL awxMotorInterlink* _AwxMotorInterlinkCreate(afxSimulation sim, awxMotorInterlinkCallbacks const *callbacks, awxMotor mot, awxBody bod, afxBool active, void *InitData)
{
    awxMotorInterlink* intk = AfxAllocate(NIL, sizeof(*intk), 1, 0, AfxHint());

    if (intk)
    {
        intk->callbacks = callbacks;
        intk->reservedPointer = 0;
        _AwxMotorInterlinkLinkBody(intk, mot, bod, active);

        if (callbacks->InitializeBindingState)
            callbacks->InitializeBindingState(intk, InitData);
    }
    return intk;
}

_AFXINL void _FreeMotorRing(afxChain *ch)
{
    awxMotorInterlink *intk;
    AfxChainForEveryLinkage(ch, awxMotorInterlink, mot, intk)
    {
        _AwxMotorInterlinkFree(intk);
    }
}

_AFXINL void _FreeBodyRing(afxChain *ch)
{
    awxMotorInterlink *intk;
    AfxChainForEveryLinkage(ch, awxMotorInterlink, bod, intk)
    {
        _AwxMotorInterlinkFree(intk);
    }
}



// PUPPET



_AFX afxReal AwxGetMotorCompletionClock(awxMotor mot)
{
    return mot->killClock;
}

_AFX afxBool AwxGetMotorCompletionCheckFlag(awxMotor mot)
{
    return (mot->flags & awxMotorFlags_KILL_ONCE_COMPLETE) == awxMotorFlags_KILL_ONCE_COMPLETE;
}

_AFX void AwxSetMotorCompletionCheckFlag(awxMotor mot, afxBool checkComplete)
{
    if (checkComplete)
        mot->flags |= awxMotorFlags_KILL_ONCE_COMPLETE;
    else
        mot->flags &= (~(awxMotorFlags_KILL_ONCE_COMPLETE));
}

_AFX void AwxSetMotorClockOnly(awxMotor mot, afxReal time)
{
    mot->currentClock = time;
}

_AFX afxBool AfxMotorIsComplete(awxMotor mot)
{
    return (mot->flags & awxMotorFlags_KILL_ONCE_COMPLETE) && mot->currentClock >= mot->killClock;
}

_AFX afxBool AwxFreeMotorIfComplete(awxMotor mot)
{
    // Should be compatible with char FreeControlIfComplete(control *Control)

    if (!mot || !AfxMotorIsComplete(mot))
        return FALSE;

    while (0 < AfxReleaseObjects(1, (void*) { mot }));
    return TRUE;
}

_AFX afxReal AwxGetMotorWeight(awxMotor mot)
{
    return mot->currentWeight;
}

_AFX void AwxSetMotorWeight(awxMotor mot, afxReal weight)
{
    afxReal currWeight = mot->currentWeight;
    mot->currentWeight = weight;

    if (currWeight == 0.0)
    {
        if (0.0 == weight)
            return;

    LABEL_5:
        _AfxMotorSortBindings(mot);
        return;
    }

    if (weight == 0.0)
        goto LABEL_5;
}

//_AFX granny_track_mask const* AwxGetMotorTrackGroupModelMask(awxMotor mot, granny_model_instance const*  ModelInstance);
//_AFX granny_track_mask const* AwxGetMotorTrackGroupTrackMask(awxMotor mot, granny_animation const*  Animation, granny_int32x TrackGroupIndex);

_AFX afxBool AfxMotorIsActive(awxMotor mot)
{
    return (mot->flags & awxMotorFlags_ACTIVE) == awxMotorFlags_ACTIVE;
}

_AFX void AwxSetMotorActive(awxMotor mot, afxBool active)
{
    if (((mot->flags & awxMotorFlags_ACTIVE) == awxMotorFlags_ACTIVE) != active)
    {
        if (active)
            mot->flags |= awxMotorFlags_ACTIVE;
        else
            mot->flags &= (~(awxMotorFlags_ACTIVE));

        _AfxMotorSortBindings(mot);
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

_AFX afxReal AwxGetMotorEaseCurveMultiplier(awxMotor mot)
{
    afxV4d easeInValues;
    _UnpackEaseCurve(mot->easeInValues, easeInValues);
    afxV4d easeOutValues;
    _UnpackEaseCurve(mot->easeOutValues, easeOutValues);

    afxReal result = 1.0;

    if (mot->easeInStartClock >= mot->easeOutStartClock && (mot->flags & awxMotorFlags_EASE_OUT) && mot->flags & awxMotorFlags_EASE_IN)
    {
        if (mot->currentClock > mot->easeOutEndClock)
        {
            if (mot->currentClock > mot->easeInStartClock)
            {
                if (mot->currentClock <= mot->easeInEndClock)
                    result = _ComputeEaseCurve(easeInValues, mot->easeInStartClock, mot->currentClock, mot->easeInEndClock);
            }
            else
            {
                afxReal v3 = mot->currentClock - mot->easeOutEndClock;
                afxReal v4 = mot->easeInStartClock - mot->easeOutEndClock;
                result = (1.0 - v3 / v4) * easeOutValues[3] + easeInValues[0] * (v3 / v4);
            }
        }
        else
        {
            result = _ComputeEaseCurve(easeOutValues, mot->easeOutStartClock, mot->currentClock, mot->easeOutEndClock);
        }
    }
    else
    {
        if ((mot->flags & awxMotorFlags_EASE_OUT) && mot->currentClock >= mot->easeOutStartClock)
            result = _ComputeEaseCurve(easeOutValues, mot->easeOutStartClock, mot->currentClock, mot->easeOutEndClock);

        if ((mot->flags & awxMotorFlags_EASE_IN) && mot->currentClock <= mot->easeInEndClock)
            result = _ComputeEaseCurve(easeInValues, mot->easeInStartClock, mot->currentClock, mot->easeInEndClock);
    }
    return result;
}

_AFX afxReal AwxGetMotorEffectiveWeight(awxMotor mot)
{
    return AfxMotorIsActive(mot) ? (AwxGetMotorEaseCurveMultiplier(mot) * mot->currentWeight) : 0.0;
}

_AFX afxReal AwxMotorEaseIn(awxMotor mot, afxReal Duration, afxBool fromCurrent)
{
    afxReal currentClock = AwxGetMotorClock(mot);
    afxReal multiplier = fromCurrent ? AwxGetMotorEaseCurveMultiplier(mot) : 0.0;    
    AwxSetMotorEaseIn(mot, TRUE);
    AwxSetMotorEaseOut(mot, FALSE);
    afxReal fromCurrenta = currentClock + Duration;
    AwxSetMotorEaseInCurve(mot, currentClock, fromCurrenta, multiplier, multiplier, 1.0, 1.0);
    return fromCurrenta;
}

_AFX afxReal AwxMotorEaseOut(awxMotor mot, afxReal duration)
{
    afxReal currentClock = AwxGetMotorClock(mot);
    afxReal multiplier = AwxGetMotorEaseCurveMultiplier(mot);
    AwxSetMotorEaseIn(mot, FALSE);
    AwxSetMotorEaseOut(mot, TRUE);
    afxReal durationa = currentClock + duration;
    AwxSetMotorEaseOutCurve(mot, currentClock, durationa, multiplier, multiplier, 0.0, 0.0);
    return durationa;
}

_AFX void AwxRecenterMotorClocks(awxMotor mot, afxReal currClock)
{
    mot->currentClock = currClock + mot->currentClock;
    mot->killClock = currClock + mot->killClock;
    mot->easeInStartClock = currClock + mot->easeInStartClock;
    mot->easeInEndClock = currClock + mot->easeInEndClock;
    mot->easeOutStartClock = currClock + mot->easeOutStartClock;
    mot->easeOutEndClock = currClock + mot->easeOutEndClock;
}

_AFX void AwxForceMotorClampedLooping(awxMotor mot, afxBool clamp)
{
    if (clamp)
        mot->flags |= awxMotorFlags_FORCE_CLAMPLED_LOOPS;
    else
        mot->flags &= (~(awxMotorFlags_FORCE_CLAMPLED_LOOPS));
}

_AFX void AwxGetMotorLoopState(awxMotor mot, afxBool *underflowLoop, afxBool *overflowLoop)
{
    AwxGetMotorRawLocalClock(mot);

    if (mot->flags & awxMotorFlags_FORCE_CLAMPLED_LOOPS)
    {
        *overflowLoop = 0;
        *underflowLoop = 0;
    }
    else if (mot->loopCnt)
    {
        *underflowLoop = mot->loopIdx > 0;
        *overflowLoop = mot->loopIdx < mot->loopCnt - 1;
    }
    else
    {
        *overflowLoop = 1;
        *underflowLoop = 1;
    }
}

_AFX void _AfxMotorModulusLocalClock(awxMotor mot)
{
    afxReal v1 = mot->localClock / mot->localDur;
    afxInt v2 = mot->loopIdx;
    afxInt v3 = v2 + (afxInt)v1;

    if (mot->localClock < 0.0)
        --v3;

    afxInt v4 = mot->loopCnt;

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
        mot->loopIdx = v5 + v2;
        mot->localClock = mot->localClock - (afxReal)v5 * mot->localDur;
    }
}

_AFX void AwxSetMotorTargetState(awxMotor mot, afxReal currentGlobalTime, afxReal targetGlobalTime, afxReal localTime, afxInt loopIndex)
{
    afxReal t = (targetGlobalTime - currentGlobalTime) * mot->speed;
    mot->dTLocalClockPending = 0.0;
    mot->currentClock = currentGlobalTime;
    mot->loopIdx = loopIndex;
    mot->localClock = localTime - t;
    _AfxMotorModulusLocalClock(mot);
}

_AFX void AwxSetMotorEaseIn(awxMotor mot, afxBool easeIn)
{
    if (easeIn)
        mot->flags |= awxMotorFlags_EASE_IN;
    else
        mot->flags &= (~(awxMotorFlags_EASE_IN));
}

_AFX void AwxSetMotorEaseOut(awxMotor mot, afxBool easeOut)
{
    if (easeOut)
        mot->flags |= awxMotorFlags_EASE_OUT;
    else
        mot->flags &= (~(awxMotorFlags_EASE_OUT));
}

_AFX void _PackEaseCurve(afxNat *result, afxReal Af, afxReal Bf, afxReal Cf, afxReal Df)
{
    *result = ((afxInt)(Af * 255.0)) | ((((afxInt)(Bf * 255.0)) | ((((afxInt)(Cf * 255.0)) | ((afxInt)((afxInt)(Df * 255.0)) << 8)) << 8)) << 8);
}

_AFX void AwxSetMotorEaseInCurve(awxMotor mot, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    mot->easeInStartClock = StartSeconds;
    mot->easeInEndClock = EndSeconds;
    _PackEaseCurve(&mot->easeInValues, StartValue, StartTangent, EndTangent, EndValue);
}

_AFX void AwxSetMotorEaseOutCurve(awxMotor mot, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    mot->easeOutStartClock = StartSeconds;
    mot->easeOutEndClock = EndSeconds;
    _PackEaseCurve(&mot->easeOutValues, StartValue, StartTangent, EndTangent, EndValue);
}

_AFX afxReal AwxGetMotorClampedLocalClock(awxMotor mot)
{
    afxReal result = AwxGetMotorRawLocalClock(mot);

    if (result < 0.0)
        return 0.0;

    if (result > mot->localDur)
        result = mot->localDur;

    return result;
}

_AFX afxReal AwxGetMotorLocalDuration(awxMotor mot)
{
    return mot->localDur;
}

_AFX afxReal AwxGetMotorRawLocalClock(awxMotor mot)
{
    unsigned __int8 v3 = 0; // c0
    unsigned __int8 v4 = 0; // c2

    if (mot->dTLocalClockPending != 0.0)
    {
        afxReal v1 = mot->speed * mot->dTLocalClockPending + mot->localClock;
        mot->localClock = v1;

        if (v3 | v4) // wtf?
        {
            if (mot->loopCnt && mot->loopIdx <= 0)
                goto LABEL_10;
        }
        else
        {
            if (v1 < mot->localDur)
                goto LABEL_10;

            if (mot->loopCnt)
            {
                if (mot->loopIdx >= mot->loopCnt - 1)
                    goto LABEL_10;
            }
        }
        _AfxMotorModulusLocalClock(mot);
    LABEL_10:
        mot->dTLocalClockPending = 0.0;
    }
    return mot->localClock;
}

_AFX void AwxSetMotorRawLocalClock(awxMotor mot, afxReal LocalClock)
{
    mot->dTLocalClockPending = 0.0;
    mot->localClock = LocalClock;
}

_AFX void** AwxGetMotorUserDataArray(awxMotor mot)
{
    return mot->userData;
}

_AFX afxReal AwxGetMotorDuration(awxMotor mot)
{
    afxReal result = 1.9140881e38;

    if (mot->loopCnt)
    {
        if (mot->speed > 0.0)
            return mot->localDur / mot->speed * mot->loopCnt;

        if (mot->speed < 0.0)
            result = -(mot->localDur / mot->speed) * mot->loopCnt;
    }
    return result;
}

_AFX afxReal AwxGetMotorDurationLeft(awxMotor mot)
{
    afxReal rawLocalClock = AwxGetMotorRawLocalClock(mot);
    afxInt loopCnt = mot->loopCnt;
    
    if (!loopCnt)
        return 1.9140881e38;

    if (mot->speed > 0.0)
    {
        afxInt loopIdx = mot->loopIdx;

        if (loopIdx < loopCnt)
            return (afxReal)(loopCnt - loopIdx - 1) * (1.0 / mot->speed * mot->localDur) + (mot->localDur - rawLocalClock) * (1.0 / mot->speed);

        return 0.0;
    }

    if (mot->speed >= 0.0)
        return 1.9140881e38;

    if (mot->loopIdx < 0)
        return 0.0;

    return -(1.0 / mot->speed * mot->localDur) * (double)mot->loopIdx - 1.0 / mot->speed * rawLocalClock;
}

_AFX afxInt AwxGetMotorIteration(awxMotor mot)
{
    AwxGetMotorRawLocalClock(mot);
    return mot->loopIdx;
}

_AFX void AwxSetMotorIteration(awxMotor mot, afxInt loopIdx)
{
    mot->loopIdx = loopIdx;
}

_AFX afxInt AwxGetMotorLoopCount(awxMotor mot)
{
    return mot->loopCnt;
}

_AFX void AwxSetMotorLoopCount(awxMotor mot, afxInt loopCnt)
{
    mot->loopCnt = loopCnt;
}

_AFX afxReal AwxGetMotorSpeed(awxMotor mot)
{
    return mot->speed;
}

_AFX void AwxSetMotorSpeed(awxMotor mot, afxReal speed)
{
    mot->speed = speed;
}

_AFX afxReal AwxGetMotorClock(awxMotor mot)
{
    return mot->currentClock;
}

_AFX void AwxSetMotorClock(awxMotor mot, afxReal time)
{
    afxReal dt = time - mot->currentClock;
    mot->currentClock = time;
    mot->dTLocalClockPending = dt + mot->dTLocalClockPending;
}

_AFXINL afxBool _AfxMotorIsUnused(awxMotor mot)
{
    return AfxChainIsEmpty(&mot->puppets);
}

_AFXINL afxBool _AfxMotorFreeIfUnused(awxMotor mot)
{
    if (!mot || !(mot->flags & awxMotorFlags_KILL_ONCE_UNUSED) || !_AfxMotorIsUnused(mot))
        return FALSE;

    while (0 < AfxReleaseObjects(1, (void*) { mot }));
    return TRUE;
}

_AFX void AwxFreeMotorOnceUnused(awxMotor mot)
{
    mot->flags |= awxMotorFlags_KILL_ONCE_UNUSED;
    _AfxMotorFreeIfUnused(mot);
}

_AFX void AfxCompleteMotorAt(awxMotor mot, afxReal atSecs)
{
    mot->flags |= awxMotorFlags_KILL_ONCE_COMPLETE;
    mot->killClock = atSecs;
}

_AFX afxError _AfxMotDtor(awxMotor mot)
{
    AfxEntry("mot=%p", mot);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOTO);

    mot->flags &= 0xFFFFFFF8;
    _FreeMotorRing(&mot->puppets);

    return err;
}

_AFX afxError _AfxMotCtor(awxMotor mot, afxCookie const *cookie)
{
    AfxEntry("mot=%p", mot);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOTO);

    afxReal currClock = *(afxReal*)cookie->udd[0];
    afxReal localDur = *(afxReal*)cookie->udd[1];

    AfxTakeChain(&mot->puppets, mot);
    mot->flags = awxMotorFlags_ACTIVE;
    mot->currentClock = currClock;
    mot->dTLocalClockPending = 0.0;
    mot->localClock = 0.0;
    mot->speed = 1.0;
    mot->localDur = localDur;
    mot->loopIdx = 0;
    mot->loopCnt = 1;
    mot->killClock = 0;
    mot->currentWeight = 1;
    mot->easeInStartClock = 0;
    mot->easeInEndClock = 0;
    mot->easeInValues = (afxNat)-65536;
    mot->easeOutStartClock = 0;
    mot->easeOutEndClock = 0;
    mot->easeOutValues = (afxNat)0xFFFF;

    return err;
}

_AFX afxClassConfig _AfxMotClsConfig =
{
    .fcc = afxFcc_MOTO,
    .name = "Motor",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(awxMotor)),
    .mmu = NIL,
    .ctor = (void*)_AfxMotCtor,
    .dtor = (void*)_AfxMotDtor
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireMotors(afxSimulation sim, afxNat cnt, awxMotor mot[], afxReal currClock, afxReal localDur)
{
    AfxEntry("sim=%p,currClock=%f,localDur=%f", sim, currClock, localDur);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    
    if (AfxAcquireObjects(AwxGetMotorClass(sim), cnt, (afxObject*)mot, (void const*[]) { &currClock, &localDur }))
        AfxThrowError();

    return err;
}

_AFX void AfxRecenterAllMotorClocks(afxSimulation sim, afxReal dCurrentClock)
{
    afxClass *cls = AwxGetMotorClass(sim);
    afxNat cnt = AfxCountInstances(cls);

    for (afxNat i = 0; i < cnt; i++)
    {
        awxMotor mot;

        if (AfxEnumerateInstances(cls, i, 1, (void*)&mot))
        {
            AwxRecenterMotorClocks(mot, dCurrentClock);
        }
    }
}
