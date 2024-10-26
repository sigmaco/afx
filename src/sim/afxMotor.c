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
#define _AMX_MOTOR_C
#include "../dev/AmxImplKit.h"

_AMXINL void** AfxGetMotorUdd(afxMotor moto)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    return moto->userData;
}

_AMXINL void AfxSetMotorClockOnly(afxReal time, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);

        moto->timing.currClock = time;
    }
}

_AMXINL afxBool AfxMotorTerminationIsScheduled(afxMotor moto)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    return (moto->flags & afxMotorFlag_KILL_ONCE_COMPLETE) == afxMotorFlag_KILL_ONCE_COMPLETE;
}

_AMXINL afxBool AfxMotorIsActive(afxMotor moto)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    return (moto->flags & afxMotorFlag_ACTIVE) == afxMotorFlag_ACTIVE;
}

_AMXINL afxBool AfxMotorHasTerminated(afxMotor moto)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    return (moto->flags & afxMotorFlag_KILL_ONCE_COMPLETE) && moto->timing.currClock >= moto->timing.killClock;
}

_AMXINL afxBool AfxMotorHasEffect(afxMotor moto)
{
    return (moto->flags & afxMotorFlag_ACTIVE) && moto->currWeight != 0.0;
}

_AMXINL void AfxEnableMotorTerminationCheck(afxBool checkComplete, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);

        if (checkComplete)
            moto->flags |= afxMotorFlag_KILL_ONCE_COMPLETE;
        else
            moto->flags &= (~(afxMotorFlag_KILL_ONCE_COMPLETE));
    }
}

_AMXINL void AfxEnableMotorIterationClamping(afxBool clamp, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);

        if (clamp)
            moto->flags |= afxMotorFlag_FORCE_CLAMPLED_LOOPS;
        else
            moto->flags &= (~(afxMotorFlag_FORCE_CLAMPLED_LOOPS));
    }
}

_AMXINL void AfxEnableMotorEaseIn(afxBool easeIn, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);

        if (easeIn)
            moto->flags |= afxMotorFlag_EASE_IN;
        else
            moto->flags &= (~(afxMotorFlag_EASE_IN));
    }
}

_AMXINL void AfxEnableMotorEaseOut(afxBool easeOut, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);

        if (easeOut)
            moto->flags |= afxMotorFlag_EASE_OUT;
        else
            moto->flags &= (~(afxMotorFlag_EASE_OUT));
    }
}

_AMXINL void AfxActivateMotor(afxBool active, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);

        if (((moto->flags & afxMotorFlag_ACTIVE) == afxMotorFlag_ACTIVE) != active)
        {
            if (active)
                moto->flags |= afxMotorFlag_ACTIVE;
            else
                moto->flags &= (~(afxMotorFlag_ACTIVE));

            // we do not resort
        }
    }
}

_AMXINL void AfxGetMotorTiming(afxMotor moto, afxMotorTiming* timing)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    AFX_ASSERT(timing);
    *timing = moto->timing;
}

_AMXINL void AfxRebaseMotorClocks(afxReal currClock, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        moto->timing.currClock += currClock;
        moto->timing.killClock += currClock;
        moto->timing.easeInStartClock += currClock;
        moto->timing.easeInEndClock += currClock;
        moto->timing.easeOutStartClock += currClock;
        moto->timing.easeOutEndClock += currClock;
    }
}

_AMXINL void AfxResetMotorLocalClock(afxReal localClock, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        moto->dtLocalClockPending = 0.0;
        moto->localClock = localClock;
    }
}

_AMXINL void AfxSelectMotorIteration(afxInt currIterIdx, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        moto->currIterIdx = currIterIdx;
    }
}

_AMXINL void AfxResetMotorIterations(afxInt iterCnt, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        moto->iterCnt = iterCnt;
    }
}

_AMXINL void AfxSetMotorSpeed(afxReal speed, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        moto->speed = speed;
    }
}

_AMXINL void AfxSetMotorWeight(afxReal weight, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        //afxReal currWeight = moto->currWeight;
        moto->currWeight = weight;
        // we do not resort
    }
}

_AMXINL void AfxUpdateMotorClock(afxReal time, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        afxReal dt = time - moto->timing.currClock;
        moto->timing.currClock = time;
        moto->dtLocalClockPending += dt;
    }
}

_AMXINL afxBool AfxMotorIsUnused(afxMotor moto)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    return 0 == AfxGetRefCount(moto); //AfxChainIsEmpty(&moto->puppets);
}

_AMXINL void AfxScheduleMotorTermination(afxReal atSecs, afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, motors, afxFcc_MOTO);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMotor moto = motors[i];
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        moto->flags |= afxMotorFlag_KILL_ONCE_COMPLETE;
        moto->timing.killClock = atSecs;
    }
}

_AMXINL void AfxQueryMotorState(afxMotor moto, afxMotorState* state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);

    afxReal localClockRaw, localClockClamped;
    localClockRaw = AfxQueryMotorLocalClock(moto, &localClockClamped);
    afxReal speed = moto->speed;
    afxInt iterCnt = state->iterCnt;
    afxInt currIterIdx = moto->currIterIdx;
    afxReal localDur = moto->localDur;

    AFX_ASSERT(state);
    state->active = (moto->flags & afxMotorFlag_ACTIVE) == afxMotorFlag_ACTIVE;
    state->speed = speed;
    state->iterCnt = iterCnt;
    state->currIterIdx = currIterIdx;
    state->currWeight = moto->currWeight;
    state->effectiveWeight = state->active ? AfxGetMotorEaseCurveMultiplier(moto) * moto->currWeight : 0.f;
    state->localClockRaw = localClockRaw;
    state->localClockClamped = localClockClamped;
    state->localDur = localDur;

    // determine iteration state
    if (moto->flags & afxMotorFlag_FORCE_CLAMPLED_LOOPS)
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

            if (moto->currIterIdx < 0)
                state->durLeft = 0.0;
            else
                state->durLeft = -(1.0 / speed * localDur) * (afxReal)currIterIdx - 1.0 / speed * localClockRaw;
        }
    }
}

_AMXINL afxReal AfxMotorEaseIn(afxMotor moto, afxReal Duration, afxBool fromCurrent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    afxReal currClock = moto->timing.currClock;
    afxReal multiplier = fromCurrent ? AfxGetMotorEaseCurveMultiplier(moto) : 0.0;    
    AfxEnableMotorEaseIn(TRUE, 1, &moto);
    AfxEnableMotorEaseOut(FALSE, 1, &moto);
    afxReal fromCurrenta = currClock + Duration;
    AfxSetMotorEaseInCurve(moto, currClock, fromCurrenta, multiplier, multiplier, 1.0, 1.0);
    return fromCurrenta;
}

_AMXINL afxReal AfxMotorEaseOut(afxMotor moto, afxReal duration)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    afxReal currClock = moto->timing.currClock;
    afxReal multiplier = AfxGetMotorEaseCurveMultiplier(moto);
    AfxEnableMotorEaseIn(FALSE, 1, &moto);
    AfxEnableMotorEaseOut(TRUE, 1, &moto);
    afxReal durationa = currClock + duration;
    AfxSetMotorEaseOutCurve(moto, currClock, durationa, multiplier, multiplier, 0.0, 0.0);
    return durationa;
}

// Modulus local clock and dependents

_AMXINL void _AfxMotorModulusLocalClock(afxMotor moto)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    afxReal t = moto->localClock / moto->localDur;
    afxInt currIterIdx = moto->currIterIdx;
    afxInt i = currIterIdx + (afxInt)t;

    if (moto->localClock < 0.0)
        --i;

    afxInt iterCnt = moto->iterCnt;

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
        moto->currIterIdx = j + currIterIdx;
        moto->localClock = moto->localClock - (afxReal64)j * moto->localDur;
    }
}

_AMXINL void AfxSetMotorTargetState(afxMotor moto, afxReal currGlobalTime, afxReal targetGlobalTime, afxReal localTime, afxInt loopIndex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    afxReal t = (targetGlobalTime - currGlobalTime) * moto->speed;
    moto->dtLocalClockPending = 0.0;
    moto->timing.currClock = currGlobalTime;
    moto->currIterIdx = loopIndex;
    moto->localClock = localTime - t;
    _AfxMotorModulusLocalClock(moto);
}

_AMXINL afxReal AfxQueryMotorLocalClock(afxMotor moto, afxReal* clamped)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    afxInt currIterIdx = moto->currIterIdx;
    afxInt iterCnt = moto->iterCnt;

    if (moto->dtLocalClockPending != 0.0)
    {
        // process pending local clock

        while (1)
        {
            afxReal t = moto->speed * moto->dtLocalClockPending + moto->localClock;
            moto->localClock = t;

            if (1) // [!]
            {
                if (iterCnt && (currIterIdx <= 0))
                    break; // avoid modulus
            }
            else
            {
                if (t < moto->localDur)
                    break; // avoid modulus

                if (iterCnt)
                {
                    if (currIterIdx >= (iterCnt - 1))
                        break; // avoid modulus
                }
            }
            _AfxMotorModulusLocalClock(moto);
            break;
        }
        moto->dtLocalClockPending = 0.0; // reset pending local clock
    }
    
    afxReal localClock = moto->localClock;

    if (clamped)
    {
        if (localClock < 0.0)
            *clamped = 0.0;
        else if (localClock > moto->localDur)
            *clamped = moto->localDur;
        else
            *clamped = localClock;
    }
    return localClock;
}

_AMXINL void _PackEaseCurve(afxUnit *result, afxReal Af, afxReal Bf, afxReal Cf, afxReal Df)
{
    *result = ((afxInt)(Af * 255.0)) | ((((afxInt)(Bf * 255.0)) | ((((afxInt)(Cf * 255.0)) | ((afxInt)((afxInt)(Df * 255.0)) << 8)) << 8)) << 8);
}

_AMXINL void AfxSetMotorEaseInCurve(afxMotor moto, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    moto->timing.easeInStartClock = StartSeconds;
    moto->timing.easeInEndClock = EndSeconds;
    _PackEaseCurve(&moto->easeInValues, StartValue, StartTangent, EndTangent, EndValue);
}

_AMXINL void AfxSetMotorEaseOutCurve(afxMotor moto, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    moto->timing.easeOutStartClock = StartSeconds;
    moto->timing.easeOutEndClock = EndSeconds;
    _PackEaseCurve(&moto->easeOutValues, StartValue, StartTangent, EndTangent, EndValue);
}

_AMXINL afxReal _ComputeEaseCurve(afxReal *Bezier, afxReal StartClock, afxReal CurrentClock, afxReal EndClock)
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

_AMXINL void _UnpackEaseCurve(afxUnit32 value, afxV4d result)
{
    result[0] = (afxReal)(afxUnit8)value * 0.0039215689;
    result[1] = (afxReal)((afxUnit16)value >> 8) * 0.0039215689;
    result[2] = (afxReal)((value >> 16) & 0xFF) * 0.0039215689;
    result[3] = (afxReal)(value >> 24) * 0.0039215689;
}

_AMXINL afxReal AfxGetMotorEaseCurveMultiplier(afxMotor moto)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    afxV4d easeInValues;
    _UnpackEaseCurve(moto->easeInValues, easeInValues);
    afxV4d easeOutValues;
    _UnpackEaseCurve(moto->easeOutValues, easeOutValues);

    afxReal result = 1.0;

    if (moto->timing.easeInStartClock >= moto->timing.easeOutStartClock && (moto->flags & afxMotorFlag_EASE_OUT) && moto->flags & afxMotorFlag_EASE_IN)
    {
        if (moto->timing.currClock > moto->timing.easeOutEndClock)
        {
            if (moto->timing.currClock > moto->timing.easeInStartClock)
            {
                if (moto->timing.currClock <= moto->timing.easeInEndClock)
                    result = _ComputeEaseCurve(easeInValues, moto->timing.easeInStartClock, moto->timing.currClock, moto->timing.easeInEndClock);
            }
            else
            {
                afxReal a = (moto->timing.currClock - moto->timing.easeOutEndClock) / (moto->timing.easeInStartClock - moto->timing.easeOutEndClock);
                result = (1.0 - a) * easeOutValues[3] + easeInValues[0] * (a);
            }
        }
        else
        {
            result = _ComputeEaseCurve(easeOutValues, moto->timing.easeOutStartClock, moto->timing.currClock, moto->timing.easeOutEndClock);
        }
    }
    else
    {
        if ((moto->flags & afxMotorFlag_EASE_OUT) && moto->timing.currClock >= moto->timing.easeOutStartClock)
            result = _ComputeEaseCurve(easeOutValues, moto->timing.easeOutStartClock, moto->timing.currClock, moto->timing.easeOutEndClock);

        if ((moto->flags & afxMotorFlag_EASE_IN) && moto->timing.currClock <= moto->timing.easeInEndClock)
            result = _ComputeEaseCurve(easeInValues, moto->timing.easeInStartClock, moto->timing.currClock, moto->timing.easeInEndClock);
    }
    return result;
}

_AMX afxError _AmxMotoDtorCb(afxMotor moto)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);

    moto->flags &= 0xFFFFFFF8;
    
    return err;
}

_AMX afxError _AmxMotoCtorCb(afxMotor moto, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &moto, afxFcc_MOTO);

    afxSimulation sim = args[0];
    afxReal currClock = *(afxReal*)args[1];
    afxReal localDur = *(afxReal*)args[2];
    afxUnit iterCnt = *(afxUnit*)args[3];

    moto->flags = afxMotorFlag_ACTIVE;
    moto->timing.currClock = currClock;
    moto->dtLocalClockPending = 0.0;
    moto->localClock = 0.0;
    moto->speed = 1.0;
    moto->localDur = localDur;
    moto->currIterIdx = 0;
    moto->iterCnt = iterCnt;
    moto->timing.killClock = 0;
    moto->currWeight = 1;
    moto->timing.easeInStartClock = 0;
    moto->timing.easeInEndClock = 0;
    moto->easeInValues = (afxUnit)-65536;
    moto->timing.easeOutStartClock = 0;
    moto->timing.easeOutEndClock = 0;
    moto->easeOutValues = (afxUnit)0xFFFF;

    return err;
}

_AMX afxClassConfig const _AmxMotoMgrCfg =
{
    .fcc = afxFcc_MOTO,
    .name = "Motor",
    .desc = "Locomotion Motor",
    .fixedSiz = sizeof(AFX_OBJECT(afxMotor)),
    .ctor = (void*)_AmxMotoCtorCb,
    .dtor = (void*)_AmxMotoDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquireMotors(afxSimulation sim, afxReal currClock, afxReal localDur, afxUnit iterCnt, afxUnit cnt, afxMotor moto[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxClass *cls = AkxGetMotorClass(sim);
    AfxAssertClass(cls, afxFcc_MOTO);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)moto, (void const*[]) { sim, &currClock, &localDur, &iterCnt }))
        AfxThrowError();

    return err;
}

_AMX afxUnit AfxRecenterAllMotorClocks(afxSimulation sim, afxReal dCurrentClock)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);    
    afxUnit i = 0;
    afxMotor moto;
    afxClass *cls = AkxGetMotorClass(sim);
    AfxAssertClass(cls, afxFcc_MOTO);    
    while (!AfxEnumerateClassInstances(cls, i++, 1, (afxObject*)&moto))
    {
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        AfxRebaseMotorClocks(dCurrentClock, 1, &moto);
    }
    return i;
}

_AMXINL afxBool AfxReleaseTerminatedMotors(afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(motors);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    afxMotor moto;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((moto = motors[i]))
        {
            AfxAssertObjects(1, &moto, afxFcc_MOTO);

            if (AfxMotorHasTerminated(moto))
                if (AfxReleaseObjects(1, (void*[]) { moto }))
                    ++rslt;
        }
    }
    return rslt;
}

_AMXINL afxBool AfxReleaseUnusedMotors(afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(motors);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    afxMotor moto;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((moto = motors[i]))
        {
            AfxAssertObjects(1, &moto, afxFcc_MOTO);

            if ((moto->flags & afxMotorFlag_KILL_ONCE_UNUSED) && AfxMotorIsUnused(moto))
                if (AfxReleaseObjects(1, &moto))
                    ++rslt;
        }
    }
    return rslt;
}

_AMXINL afxBool AfxReleaseOnceUnusedMotors(afxUnit cnt, afxMotor motors[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(motors);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    afxMotor moto;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((moto = motors[i]))
        {
            AfxAssertObjects(1, &moto, afxFcc_MOTO);
            moto->flags |= afxMotorFlag_KILL_ONCE_UNUSED;
        }
    }
    return AfxReleaseUnusedMotors(cnt, motors);
}
