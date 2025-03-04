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

#define _AFX_SIM_C
#define _ASX_MOTOR_C
#include "impl/asxImplementation.h"

_ASXINL void* AfxGetCapstanUdd(afxCapstan caps, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

    if (4 >= idx)
        return NIL;

    return caps->udd[idx];
}

_ASXINL void AfxSetCapstanClockOnly(afxReal time, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        caps->timing.currClock = time;
    }
}

_ASXINL afxBool AfxCapstanTerminationIsScheduled(afxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    return (caps->flags & afxCapstanFlag_KILL_ONCE_COMPLETE) == afxCapstanFlag_KILL_ONCE_COMPLETE;
}

_ASXINL afxBool AfxCapstanIsActive(afxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    return (caps->flags & afxCapstanFlag_ACTIVE) == afxCapstanFlag_ACTIVE;
}

_ASXINL afxBool AfxCapstanHasTerminated(afxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    return (caps->flags & afxCapstanFlag_KILL_ONCE_COMPLETE) && caps->timing.currClock >= caps->timing.killClock;
}

_ASXINL afxBool AfxCapstanHasEffect(afxCapstan caps)
{
    return (caps->flags & afxCapstanFlag_ACTIVE) && caps->currWeight != 0.0;
}

_ASXINL void AfxEnableCapstanTerminationCheck(afxBool checkComplete, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        if (checkComplete)
            caps->flags |= afxCapstanFlag_KILL_ONCE_COMPLETE;
        else
            caps->flags &= (~(afxCapstanFlag_KILL_ONCE_COMPLETE));
    }
}

_ASXINL void AfxEnableCapstanIterationClamping(afxBool clamp, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        if (clamp)
            caps->flags |= afxCapstanFlag_FORCE_CLAMPLED_LOOPS;
        else
            caps->flags &= (~(afxCapstanFlag_FORCE_CLAMPLED_LOOPS));
    }
}

_ASXINL void AfxEnableCapstanEaseIn(afxBool easeIn, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        if (easeIn)
            caps->flags |= afxCapstanFlag_EASE_IN;
        else
            caps->flags &= (~(afxCapstanFlag_EASE_IN));
    }
}

_ASXINL void AfxEnableCapstanEaseOut(afxBool easeOut, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        if (easeOut)
            caps->flags |= afxCapstanFlag_EASE_OUT;
        else
            caps->flags &= (~(afxCapstanFlag_EASE_OUT));
    }
}

_ASXINL void AfxActivateCapstan(afxBool active, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        if (((caps->flags & afxCapstanFlag_ACTIVE) == afxCapstanFlag_ACTIVE) != active)
        {
            if (active)
                caps->flags |= afxCapstanFlag_ACTIVE;
            else
                caps->flags &= (~(afxCapstanFlag_ACTIVE));

            // we do not resort
        }
    }
}

_ASXINL void AfxGetCapstanTiming(afxCapstan caps, afxCapstanTiming* timing)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    AFX_ASSERT(timing);
    *timing = caps->timing;
}

_ASXINL void AfxRebaseCapstanClocks(afxReal currClock, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->timing.currClock += currClock;
        caps->timing.killClock += currClock;
        caps->timing.easeInStartClock += currClock;
        caps->timing.easeInEndClock += currClock;
        caps->timing.easeOutStartClock += currClock;
        caps->timing.easeOutEndClock += currClock;
    }
}

_ASXINL void AfxResetCapstanLocalClock(afxReal localClock, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->dtLocalClockPending = 0.0;
        caps->localClock = localClock;
    }
}

_ASXINL void AfxSelectCapstanIteration(afxInt currIterIdx, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->currIterIdx = currIterIdx;
    }
}

_ASXINL void AfxResetCapstanIterations(afxInt iterCnt, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->iterCnt = iterCnt;
    }
}

_ASXINL void AfxSetCapstanSpeed(afxReal speed, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->speed = speed;
    }
}

_ASXINL void AfxSetCapstanWeight(afxReal weight, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        //afxReal currWeight = caps->currWeight;
        caps->currWeight = weight;
        // we do not resort
    }
}

_ASXINL void AfxUpdateCapstanClock(afxReal time, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        afxReal dt = time - caps->timing.currClock;
        caps->timing.currClock = time;
        caps->dtLocalClockPending += dt;
    }
}

_ASXINL afxBool AfxCapstanIsUnused(afxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    return 0 == AfxGetRefCount(caps); //AfxIsChainEmpty(&caps->puppets);
}

_ASXINL void AfxScheduleCapstanTermination(afxReal atSecs, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->flags |= afxCapstanFlag_KILL_ONCE_COMPLETE;
        caps->timing.killClock = atSecs;
    }
}

_ASXINL void AfxQueryCapstanState(afxCapstan caps, afxCapstanState* state)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

    afxReal localClockRaw, localClockClamped;
    localClockRaw = AfxQueryCapstanLocalClock(caps, &localClockClamped);
    afxReal speed = caps->speed;
    afxInt iterCnt = state->iterCnt;
    afxInt currIterIdx = caps->currIterIdx;
    afxReal localDur = caps->localDur;

    AFX_ASSERT(state);
    state->active = (caps->flags & afxCapstanFlag_ACTIVE) == afxCapstanFlag_ACTIVE;
    state->speed = speed;
    state->iterCnt = iterCnt;
    state->currIterIdx = currIterIdx;
    state->currWeight = caps->currWeight;
    state->effectiveWeight = state->active ? AfxGetCapstanEaseCurveMultiplier(caps) * caps->currWeight : 0.f;
    state->localClockRaw = localClockRaw;
    state->localClockClamped = localClockClamped;
    state->localDur = localDur;

    // determine iteration state
    if (caps->flags & afxCapstanFlag_FORCE_CLAMPLED_LOOPS)
    {
        state->overflow = FALSE;
        state->underflow = FALSE;
    }
    else if (iterCnt)
    {
        state->underflow = currIterIdx > 0;
        state->overflow = currIterIdx < iterCnt - 1;
    }
    else
    {
        state->overflow = TRUE;
        state->underflow = TRUE;
    }

    // determine duration
    
    state->dur = 1.9140881e38;
    state->durLeft = 1.9140881e38;

    if (iterCnt)
    {
        if (speed > 0.0)
        {
            state->dur = (localDur / speed) * iterCnt;

            if (currIterIdx < iterCnt)
                state->durLeft = (afxReal)(iterCnt - currIterIdx - 1) * (1.0 / speed * localDur) + (localDur - localClockRaw) * (1.0 / speed);
            else
                state->durLeft = 0.0;
        }
        else if (speed < 0.0)
        {
            state->dur = -(localDur / speed) * iterCnt;

            if (caps->currIterIdx < 0)
                state->durLeft = 0.0;
            else
                state->durLeft = -(1.0 / speed * localDur) * (afxReal)currIterIdx - 1.0 / speed * localClockRaw;
        }
    }
}

_ASXINL afxReal AfxCapstanEaseIn(afxCapstan caps, afxReal Duration, afxBool fromCurrent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    afxReal currClock = caps->timing.currClock;
    afxReal multiplier = fromCurrent ? AfxGetCapstanEaseCurveMultiplier(caps) : 0.0;    
    AfxEnableCapstanEaseIn(TRUE, 1, &caps);
    AfxEnableCapstanEaseOut(FALSE, 1, &caps);
    afxReal fromCurrenta = currClock + Duration;
    AfxSetCapstanEaseInCurve(caps, currClock, fromCurrenta, multiplier, multiplier, 1.0, 1.0);
    return fromCurrenta;
}

_ASXINL afxReal AfxCapstanEaseOut(afxCapstan caps, afxReal duration)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    afxReal currClock = caps->timing.currClock;
    afxReal multiplier = AfxGetCapstanEaseCurveMultiplier(caps);
    AfxEnableCapstanEaseIn(FALSE, 1, &caps);
    AfxEnableCapstanEaseOut(TRUE, 1, &caps);
    afxReal durationa = currClock + duration;
    AfxSetCapstanEaseOutCurve(caps, currClock, durationa, multiplier, multiplier, 0.0, 0.0);
    return durationa;
}

// Modulus local clock and dependents

_ASXINL void _AfxCapstanModulusLocalClock(afxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    afxReal t = caps->localClock / caps->localDur;
    afxInt currIterIdx = caps->currIterIdx;
    afxInt i = currIterIdx + (afxInt)t;

    if (caps->localClock < 0.0)
        --i;

    afxInt iterCnt = caps->iterCnt;

    if (iterCnt)
    {
        if (i >= 0)
        {
            if (i >= iterCnt)
                i = iterCnt - 1;
        }
        else i = 0;
    }

    afxInt j = i - currIterIdx;

    if (j)
    {
        caps->currIterIdx = j + currIterIdx;
        caps->localClock = caps->localClock - (afxReal64)j * caps->localDur;
    }
}

_ASXINL void AfxSetCapstanTargetState(afxCapstan caps, afxReal currGlobalTime, afxReal targetGlobalTime, afxReal localTime, afxInt loopIndex)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    afxReal t = (targetGlobalTime - currGlobalTime) * caps->speed;
    caps->dtLocalClockPending = 0.0;
    caps->timing.currClock = currGlobalTime;
    caps->currIterIdx = loopIndex;
    caps->localClock = localTime - t;
    _AfxCapstanModulusLocalClock(caps);
}

_ASXINL afxReal AfxQueryCapstanLocalClock(afxCapstan caps, afxReal* clamped)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    afxInt currIterIdx = caps->currIterIdx;
    afxInt iterCnt = caps->iterCnt;

    if (caps->dtLocalClockPending != 0.0)
    {
        // process pending local clock

        while (1)
        {
            afxReal t = caps->speed * caps->dtLocalClockPending + caps->localClock;
            caps->localClock = t;

            if (1) // [!]
            {
                if (iterCnt && (currIterIdx <= 0))
                    break; // avoid modulus
            }
            else
            {
                if (t < caps->localDur)
                    break; // avoid modulus

                if (iterCnt)
                {
                    if (currIterIdx >= (iterCnt - 1))
                        break; // avoid modulus
                }
            }
            _AfxCapstanModulusLocalClock(caps);
            break;
        }
        caps->dtLocalClockPending = 0.0; // reset pending local clock
    }
    
    afxReal localClock = caps->localClock;

    if (clamped)
    {
        if (localClock < 0.0)
            *clamped = 0.0;
        else if (localClock > caps->localDur)
            *clamped = caps->localDur;
        else
            *clamped = localClock;
    }
    return localClock;
}

_ASXINL void _PackEaseCurve(afxUnit *result, afxReal Af, afxReal Bf, afxReal Cf, afxReal Df)
{
    *result = ((afxInt)(Af * 255.0)) | ((((afxInt)(Bf * 255.0)) | ((((afxInt)(Cf * 255.0)) | ((afxInt)((afxInt)(Df * 255.0)) << 8)) << 8)) << 8);
}

_ASXINL void AfxSetCapstanEaseInCurve(afxCapstan caps, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    caps->timing.easeInStartClock = StartSeconds;
    caps->timing.easeInEndClock = EndSeconds;
    _PackEaseCurve(&caps->easeInValues, StartValue, StartTangent, EndTangent, EndValue);
}

_ASXINL void AfxSetCapstanEaseOutCurve(afxCapstan caps, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    caps->timing.easeOutStartClock = StartSeconds;
    caps->timing.easeOutEndClock = EndSeconds;
    _PackEaseCurve(&caps->easeOutValues, StartValue, StartTangent, EndTangent, EndValue);
}

_ASXINL afxReal _ComputeEaseCurve(afxReal *Bezier, afxReal StartClock, afxReal CurrentClock, afxReal EndClock)
{
    afxReal v5; // st7

    afxReal v4 = EndClock - StartClock;

    if (v4 == 0.0)
    {
        if (CurrentClock < (double)StartClock)
            goto LABEL_3;
    LABEL_7:
        v5 = 1.0;
        return ((1.0 - v5) * Bezier[1] + v5 * Bezier[2]) * (1.0 - v5) * v5 * 3.0 + v5 * Bezier[3] * v5 * v5 + (1.0 - v5) * Bezier[0] * (1.0 - v5) * (1.0 - v5);
    }

    v5 = (CurrentClock - StartClock) / v4;

    if (v5 >= 0.0)
    {
        if (v5 <= 1.0)
            return ((1.0 - v5) * Bezier[1] + v5 * Bezier[2]) * (1.0 - v5) * v5 * 3.0 + v5 * Bezier[3] * v5 * v5 + (1.0 - v5) * Bezier[0] * (1.0 - v5) * (1.0 - v5);

        goto LABEL_7;
    }

LABEL_3:
    v5 = 0.0;
    return ((1.0 - v5) * Bezier[1] + v5 * Bezier[2]) * (1.0 - v5) * v5 * 3.0 + v5 * Bezier[3] * v5 * v5 + (1.0 - v5) * Bezier[0] * (1.0 - v5) * (1.0 - v5);
}

_ASXINL void _UnpackEaseCurve(afxUnit32 value, afxV4d result)
{
    result[0] = (afxReal)(afxUnit8)value * 0.0039215689;
    result[1] = (afxReal)((afxUnit16)value >> 8) * 0.0039215689;
    result[2] = (afxReal)((value >> 16) & 0xFF) * 0.0039215689;
    result[3] = (afxReal)(value >> 24) * 0.0039215689;
}

_ASXINL afxReal AfxGetCapstanEaseCurveMultiplier(afxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    afxV4d easeInValues;
    _UnpackEaseCurve(caps->easeInValues, easeInValues);
    afxV4d easeOutValues;
    _UnpackEaseCurve(caps->easeOutValues, easeOutValues);

    afxReal result = 1.0;

    if (caps->timing.easeInStartClock >= caps->timing.easeOutStartClock && (caps->flags & afxCapstanFlag_EASE_OUT) && caps->flags & afxCapstanFlag_EASE_IN)
    {
        if (caps->timing.currClock > caps->timing.easeOutEndClock)
        {
            if (caps->timing.currClock > caps->timing.easeInStartClock)
            {
                if (caps->timing.currClock <= caps->timing.easeInEndClock)
                    result = _ComputeEaseCurve(easeInValues, caps->timing.easeInStartClock, caps->timing.currClock, caps->timing.easeInEndClock);
            }
            else
            {
                afxReal a = (caps->timing.currClock - caps->timing.easeOutEndClock) / (caps->timing.easeInStartClock - caps->timing.easeOutEndClock);
                result = (1.0 - a) * easeOutValues[3] + easeInValues[0] * (a);
            }
        }
        else
        {
            result = _ComputeEaseCurve(easeOutValues, caps->timing.easeOutStartClock, caps->timing.currClock, caps->timing.easeOutEndClock);
        }
    }
    else
    {
        if ((caps->flags & afxCapstanFlag_EASE_OUT) && caps->timing.currClock >= caps->timing.easeOutStartClock)
            result = _ComputeEaseCurve(easeOutValues, caps->timing.easeOutStartClock, caps->timing.currClock, caps->timing.easeOutEndClock);

        if ((caps->flags & afxCapstanFlag_EASE_IN) && caps->timing.currClock <= caps->timing.easeInEndClock)
            result = _ComputeEaseCurve(easeInValues, caps->timing.easeInStartClock, caps->timing.currClock, caps->timing.easeInEndClock);
    }
    return result;
}

_ASX afxError _AsxMotoDtorCb(afxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

    caps->flags &= 0xFFFFFFF8;
    
    return err;
}

_ASX afxError _AsxMotoCtorCb(afxCapstan caps, void** args, afxUnit invokeNo)
{
    (void)invokeNo;

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxCapstanConfig const* cfg = AFX_CAST(afxCapstanConfig const*, args[1]);

    caps->flags = afxCapstanFlag_ACTIVE;
    caps->timing.currClock = cfg->currClock;
    caps->dtLocalClockPending = 0.0;
    caps->localClock = 0.0;
    caps->speed = 1.0;
    caps->localDur = cfg->localDur;
    caps->currIterIdx = 0;
    caps->iterCnt = cfg->iterCnt;
    caps->timing.killClock = 0;
    caps->currWeight = 1;
    caps->timing.easeInStartClock = 0;
    caps->timing.easeInEndClock = 0;
    caps->easeInValues = (afxUnit)-65536;
    caps->timing.easeOutStartClock = 0;
    caps->timing.easeOutEndClock = 0;
    caps->easeOutValues = (afxUnit)0xFFFF;

    return err;
}

_ASX afxClassConfig const _ASX_MOTO_CLASS_CONFIG =
{
    .fcc = afxFcc_MOTO,
    .name = "Capstan",
    .desc = "Locomotion Capstan",
    .fixedSiz = sizeof(AFX_OBJECT(afxCapstan)),
    .ctor = (void*)_AsxMotoCtorCb,
    .dtor = (void*)_AsxMotoDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxBool AfxReleaseTerminatedCapstans(afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(capstans);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    afxCapstan caps;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((caps = capstans[i]))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

            if (AfxCapstanHasTerminated(caps))
                if (AfxDisposeObjects(1, (void*[]) { caps }))
                    ++rslt;
        }
    }
    return rslt;
}

_ASX afxBool AfxReleaseUnusedCapstans(afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(capstans);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    afxCapstan caps;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((caps = capstans[i]))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

            if ((caps->flags & afxCapstanFlag_KILL_ONCE_UNUSED) && AfxCapstanIsUnused(caps))
                if (AfxDisposeObjects(1, &caps))
                    ++rslt;
        }
    }
    return rslt;
}

_ASX afxBool AfxReleaseOnceUnusedCapstans(afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(capstans);
    AFX_ASSERT(cnt);

    //afxBool rslt = 0;
    afxCapstan caps;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((caps = capstans[i]))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
            caps->flags |= afxCapstanFlag_KILL_ONCE_UNUSED;
        }
    }
    return AfxReleaseUnusedCapstans(cnt, capstans);
}

_ASX afxError AfxAcquireCapstans(afxSimulation sim, afxCapstanConfig const* cfg, afxUnit cnt, afxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(cfg);

    afxClass *cls = (afxClass *)_AsxGetCapstanClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MOTO);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)capstans, (void const*[]) { sim, cfg }))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    return err;
}

_ASX afxUnit AfxRecenterAllCapstanClocks(afxSimulation sim, afxReal dCurrentClock)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);    
    afxUnit i = 0;
    afxCapstan caps;
    afxClass *cls = (afxClass *)_AsxGetCapstanClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MOTO);    
    while (!AfxEnumerateObjects(cls, i++, 1, (afxObject*)&caps))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        AfxRebaseCapstanClocks(dCurrentClock, 1, &caps);
    }
    return i;
}
