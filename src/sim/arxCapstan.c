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
#define _ARX_MOTOR_C
#include "../../sim/impl/asxImplementation.h"

_ARXINL void* ArxGetCapstanUdd(arxCapstan caps, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

    if (4 >= idx)
        return NIL;

    return caps->udd[idx];
}

_ARXINL void ArxSetCapstanClockOnly(afxReal time, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        caps->timing.currClock = time;
    }
}

_ARXINL afxBool ArxCapstanTerminationIsScheduled(arxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    return (caps->flags & arxCapstanFlag_KILL_ONCE_COMPLETE) == arxCapstanFlag_KILL_ONCE_COMPLETE;
}

_ARXINL afxBool ArxCapstanIsActive(arxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    return (caps->flags & arxCapstanFlag_ACTIVE) == arxCapstanFlag_ACTIVE;
}

_ARXINL afxBool ArxCapstanHasTerminated(arxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    return (caps->flags & arxCapstanFlag_KILL_ONCE_COMPLETE) && caps->timing.currClock >= caps->timing.killClock;
}

_ARXINL afxBool ArxCapstanHasEffect(arxCapstan caps)
{
    return (caps->flags & arxCapstanFlag_ACTIVE) && caps->currWeight != 0.0;
}

_ARXINL void ArxEnableCapstanTerminationCheck(afxBool checkComplete, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        if (checkComplete)
            caps->flags |= arxCapstanFlag_KILL_ONCE_COMPLETE;
        else
            caps->flags &= (~(arxCapstanFlag_KILL_ONCE_COMPLETE));
    }
}

_ARXINL void ArxEnableCapstanIterationClamping(afxBool clamp, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        if (clamp)
            caps->flags |= arxCapstanFlag_FORCE_CLAMPLED_LOOPS;
        else
            caps->flags &= (~(arxCapstanFlag_FORCE_CLAMPLED_LOOPS));
    }
}

_ARXINL void ArxEnableCapstanEaseIn(afxBool easeIn, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        if (easeIn)
            caps->flags |= arxCapstanFlag_EASE_IN;
        else
            caps->flags &= (~(arxCapstanFlag_EASE_IN));
    }
}

_ARXINL void ArxEnableCapstanEaseOut(afxBool easeOut, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        if (easeOut)
            caps->flags |= arxCapstanFlag_EASE_OUT;
        else
            caps->flags &= (~(arxCapstanFlag_EASE_OUT));
    }
}

_ARXINL void ArxActivateCapstan(afxBool active, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

        if (((caps->flags & arxCapstanFlag_ACTIVE) == arxCapstanFlag_ACTIVE) != active)
        {
            if (active)
                caps->flags |= arxCapstanFlag_ACTIVE;
            else
                caps->flags &= (~(arxCapstanFlag_ACTIVE));

            // we do not resort
        }
    }
}

_ARXINL void ArxGetCapstanTiming(arxCapstan caps, arxCapstanTiming* timing)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    AFX_ASSERT(timing);
    *timing = caps->timing;
}

_ARXINL void ArxRebaseCapstanClocks(afxReal currClock, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->timing.currClock += currClock;
        caps->timing.killClock += currClock;
        caps->timing.easeInStartClock += currClock;
        caps->timing.easeInEndClock += currClock;
        caps->timing.easeOutStartClock += currClock;
        caps->timing.easeOutEndClock += currClock;
    }
}

_ARXINL void ArxResetCapstanLocalClock(afxReal localClock, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->dtLocalClockPending = 0.0;
        caps->localClock = localClock;
    }
}

_ARXINL void ArxSelectCapstanIteration(afxInt currIterIdx, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->currIterIdx = currIterIdx;
    }
}

_ARXINL void ArxResetCapstanIterations(afxInt iterCnt, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->iterCnt = iterCnt;
    }
}

_ARXINL void ArxSetCapstanSpeed(afxReal speed, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->speed = speed;
    }
}

_ARXINL void ArxSetCapstanWeight(afxReal weight, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        //afxReal currWeight = caps->currWeight;
        caps->currWeight = weight;
        // we do not resort
    }
}

_ARXINL void ArxStepCapstans(afxReal time, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        afxReal dt = time - caps->timing.currClock;
        caps->timing.currClock = time;
        caps->dtLocalClockPending += dt;
    }
}

_ARXINL afxBool ArxCapstanIsUnused(arxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    return 0 == AfxGetRefCount(caps); //AfxIsChainEmpty(&caps->puppets);
}

_ARXINL void ArxScheduleCapstanTermination(afxReal atSecs, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxCapstan caps = capstans[i];
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        caps->flags |= arxCapstanFlag_KILL_ONCE_COMPLETE;
        caps->timing.killClock = atSecs;
    }
}

_ARXINL void ArxQueryCapstanState(arxCapstan caps, arxCapstanState* state)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

    afxReal localClockRaw, localClockClamped;
    localClockRaw = ArxQueryCapstanLocalClock(caps, &localClockClamped);
    afxReal speed = caps->speed;
    afxInt iterCnt = state->iterCnt;
    afxInt currIterIdx = caps->currIterIdx;
    afxReal localDur = caps->localDur;

    AFX_ASSERT(state);
    state->active = (caps->flags & arxCapstanFlag_ACTIVE) == arxCapstanFlag_ACTIVE;
    state->speed = speed;
    state->iterCnt = iterCnt;
    state->currIterIdx = currIterIdx;
    state->currWeight = caps->currWeight;
    state->effectiveWeight = state->active ? ArxGetCapstanEaseCurveMultiplier(caps) * caps->currWeight : 0.f;
    state->localClockRaw = localClockRaw;
    state->localClockClamped = localClockClamped;
    state->localDur = localDur;

    // determine iteration state
    if (caps->flags & arxCapstanFlag_FORCE_CLAMPLED_LOOPS)
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

_ARXINL afxReal ArxCapstanEaseIn(arxCapstan caps, afxReal Duration, afxBool fromCurrent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    afxReal currClock = caps->timing.currClock;
    afxReal multiplier = fromCurrent ? ArxGetCapstanEaseCurveMultiplier(caps) : 0.0;    
    ArxEnableCapstanEaseIn(TRUE, 1, &caps);
    ArxEnableCapstanEaseOut(FALSE, 1, &caps);
    afxReal fromCurrenta = currClock + Duration;
    ArxSetCapstanEaseInCurve(caps, currClock, fromCurrenta, multiplier, multiplier, 1.0, 1.0);
    return fromCurrenta;
}

_ARXINL afxReal ArxCapstanEaseOut(arxCapstan caps, afxReal duration)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    afxReal currClock = caps->timing.currClock;
    afxReal multiplier = ArxGetCapstanEaseCurveMultiplier(caps);
    ArxEnableCapstanEaseIn(FALSE, 1, &caps);
    ArxEnableCapstanEaseOut(TRUE, 1, &caps);
    afxReal durationa = currClock + duration;
    ArxSetCapstanEaseOutCurve(caps, currClock, durationa, multiplier, multiplier, 0.0, 0.0);
    return durationa;
}

// Modulus local clock and dependents

_ARXINL void _AfxCapstanModulusLocalClock(arxCapstan caps)
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

_ARXINL void ArxSetCapstanTargetState(arxCapstan caps, afxReal currGlobalTime, afxReal targetGlobalTime, afxReal localTime, afxInt loopIndex)
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

_ARXINL afxReal ArxQueryCapstanLocalClock(arxCapstan caps, afxReal* clamped)
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

_ARXINL void _PackEaseCurve(afxUnit *result, afxReal Af, afxReal Bf, afxReal Cf, afxReal Df)
{
    *result = ((afxInt)(Af * 255.0)) | ((((afxInt)(Bf * 255.0)) | ((((afxInt)(Cf * 255.0)) | ((afxInt)((afxInt)(Df * 255.0)) << 8)) << 8)) << 8);
}

_ARXINL void ArxSetCapstanEaseInCurve(arxCapstan caps, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    caps->timing.easeInStartClock = StartSeconds;
    caps->timing.easeInEndClock = EndSeconds;
    _PackEaseCurve(&caps->easeInValues, StartValue, StartTangent, EndTangent, EndValue);
}

_ARXINL void ArxSetCapstanEaseOutCurve(arxCapstan caps, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    caps->timing.easeOutStartClock = StartSeconds;
    caps->timing.easeOutEndClock = EndSeconds;
    _PackEaseCurve(&caps->easeOutValues, StartValue, StartTangent, EndTangent, EndValue);
}

_ARXINL afxReal _ComputeEaseCurve(afxReal *Bezier, afxReal StartClock, afxReal CurrentClock, afxReal EndClock)
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

_ARXINL void _UnpackEaseCurve(afxUnit32 value, afxV4d result)
{
    result[0] = (afxReal)(afxUnit8)value * 0.0039215689;
    result[1] = (afxReal)((afxUnit16)value >> 8) * 0.0039215689;
    result[2] = (afxReal)((value >> 16) & 0xFF) * 0.0039215689;
    result[3] = (afxReal)(value >> 24) * 0.0039215689;
}

_ARXINL afxReal ArxGetCapstanEaseCurveMultiplier(arxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
    afxV4d easeInValues;
    _UnpackEaseCurve(caps->easeInValues, easeInValues);
    afxV4d easeOutValues;
    _UnpackEaseCurve(caps->easeOutValues, easeOutValues);

    afxReal result = 1.0;

    if (caps->timing.easeInStartClock >= caps->timing.easeOutStartClock && (caps->flags & arxCapstanFlag_EASE_OUT) && caps->flags & arxCapstanFlag_EASE_IN)
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
        if ((caps->flags & arxCapstanFlag_EASE_OUT) && caps->timing.currClock >= caps->timing.easeOutStartClock)
            result = _ComputeEaseCurve(easeOutValues, caps->timing.easeOutStartClock, caps->timing.currClock, caps->timing.easeOutEndClock);

        if ((caps->flags & arxCapstanFlag_EASE_IN) && caps->timing.currClock <= caps->timing.easeInEndClock)
            result = _ComputeEaseCurve(easeInValues, caps->timing.easeInStartClock, caps->timing.currClock, caps->timing.easeInEndClock);
    }
    return result;
}

_ARX afxError _ArxMotoDtorCb(arxCapstan caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

    caps->flags &= 0xFFFFFFF8;
    
    return err;
}

_ARX afxError _ArxMotoCtorCb(arxCapstan caps, void** args, afxUnit invokeNo)
{
    (void)invokeNo;

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    arxCapstanConfig const* cfg = AFX_CAST(arxCapstanConfig const*, args[1]);

    caps->flags = arxCapstanFlag_ACTIVE;
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

_ARX afxClassConfig const _ARX_MOTO_CLASS_CONFIG =
{
    .fcc = afxFcc_MOTO,
    .name = "Capstan",
    .desc = "Locomotion Capstan",
    .fixedSiz = sizeof(AFX_OBJECT(arxCapstan)),
    .ctor = (void*)_ArxMotoCtorCb,
    .dtor = (void*)_ArxMotoDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxBool ArxReleaseTerminatedCapstans(afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(capstans);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    arxCapstan caps;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((caps = capstans[i]))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

            if (ArxCapstanHasTerminated(caps))
                if (AfxDisposeObjects(1, (void*[]) { caps }))
                    ++rslt;
        }
    }
    return rslt;
}

_ARX afxBool ArxReleaseUnusedCapstans(afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(capstans);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    arxCapstan caps;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((caps = capstans[i]))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);

            if ((caps->flags & arxCapstanFlag_KILL_ONCE_UNUSED) && ArxCapstanIsUnused(caps))
                if (AfxDisposeObjects(1, &caps))
                    ++rslt;
        }
    }
    return rslt;
}

_ARX afxBool ArxReleaseOnceUnusedCapstans(afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(capstans);
    AFX_ASSERT(cnt);

    //afxBool rslt = 0;
    arxCapstan caps;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((caps = capstans[i]))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
            caps->flags |= arxCapstanFlag_KILL_ONCE_UNUSED;
        }
    }
    return ArxReleaseUnusedCapstans(cnt, capstans);
}

_ARX afxError ArxAcquireCapstans(afxSimulation sim, arxCapstanConfig const* cfg, afxUnit cnt, arxCapstan capstans[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(cfg);

    afxClass *cls = (afxClass *)_ArxGetCapstanClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MOTO);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)capstans, (void const*[]) { sim, cfg }))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, cnt, capstans);

    return err;
}

_ARX afxUnit AfxRecenterAllCapstanClocks(afxSimulation sim, afxReal dCurrentClock)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxUnit i = 0;
    arxCapstan caps;
    afxClass *cls = (afxClass *)_ArxGetCapstanClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MOTO);    
    while (!AfxEnumerateObjects(cls, i++, 1, (afxObject*)&caps))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &caps);
        ArxRebaseCapstanClocks(dCurrentClock, 1, &caps);
    }
    return i;
}
