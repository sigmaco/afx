/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

#define _AFX_CORE_C
#define _AFX_DEVICE_C

#define _AUX_UX_C
#define _AUX_HID_C
#define _AFX_CONTEXT_C
#define _AUX_KEYBOARD_C
#define _AUX_MOUSE_C
#define _AUX_CONTROLLER_C
#define _AUX_ENVIRONMENT_C
#include "../ux/impl/auxImplementation.h"

_AUX afxError AfxEmulatePressedKeys(afxUnit seat, afxUnit cnt, afxKey const key[], afxUnit8 const pressure[])
{
    afxError err = AFX_ERR_NONE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        AFX_ASSERT(cnt);
        AFX_ASSERT(key);
        AFX_ASSERT(pressure);

        for (afxUnit i = 0; i < cnt; i++)
        {
            env->seats[seat].keyState[1][key[i]] = env->seats[seat].keyState[0][key[i]];
            env->seats[seat].keyState[0][key[i]] = pressure[i];

            auxEvent ev = { 0 };
            ev.ev.id = afxEvent_UX;
            ev.ev.siz = sizeof(ev);
            ev.id = auxEventId_KEY;
            ev.seat = seat;
            AfxEmitEvent(env, (void*)&ev.ev);
        }
    }
    return err;
}

_AUX afxError AfxReleaseAllKeys(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        afxUnit cnt = env->seats[seat].keyCnt;
        AFX_ASSERT(cnt);

        for (afxUnit i = 0; i < cnt; i++)
        {
            if (env->seats[seat].keyState[0][i])
            {
                env->seats[seat].keyState[1][i] = env->seats[seat].keyState[0][i];
                env->seats[seat].keyState[0][i] = 0x00;

                auxEvent ev = { 0 };
                ev.ev.id = afxEvent_UX;
                ev.ev.siz = sizeof(ev);
                ev.id = auxEventId_KEY;
                ev.seat = seat;
                AfxEmitEvent(env, (void*)&ev.ev);
            }
        }
    }
    return err;
}

_AUX afxError AfxReleaseAllKeys2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    for (afxUnit seatIdx = 0, seatCnt = env->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        afxUnit cnt = env->seats[seatIdx].keyCnt;
        AFX_ASSERT(cnt);

        for (afxUnit i = 0; i < cnt; i++)
        {
            if (env->seats[seatIdx].keyState[0][i])
            {
                env->seats[seatIdx].keyState[1][i] = env->seats[seatIdx].keyState[0][i];
                env->seats[seatIdx].keyState[0][i] = 0x00;

                auxEvent ev = { 0 };
                ev.ev.id = afxEvent_UX;
                ev.ev.siz = sizeof(ev);
                ev.id = auxEventId_KEY;
                ev.seat = seatIdx;
                AfxEmitEvent(env, (void*)&ev.ev);
            }
        }
    }
    return err;
}

_AUX afxBool AfxWereKeysReleased(afxUnit seat, afxUnit cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        afxBool cnt2 = 0;

        for (afxUnit i = 0; i < cnt; i++)
            if (env->seats[seat].keyState[1][codes[i]] && !(env->seats[seat].keyState[0][codes[i]]))
                cnt2++;

        rslt = cnt2;
    }
    return rslt;
}

_AUX afxMask AfxWereKeysReleased2(afxMask seats, afxUnit cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    for (afxUnit seatIdx = 0, seatCnt = env->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        afxBool cnt2 = 0;

        for (afxUnit i = 0; i < cnt; i++)
            if (env->seats[seatIdx].keyState[1][codes[i]] &&
                !(env->seats[seatIdx].keyState[0][codes[i]]))
                cnt2++;

        if (cnt2 == cnt)
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxBool AfxWasKeyReleased(afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        rslt = (env->seats[seat].keyState[1][code] && !(env->seats[seat].keyState[0][code]));
    }
    return rslt;
}

_AUX afxMask AfxWasKeyReleased2(afxMask seats, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    for (afxUnit seatIdx = 0, seatCnt = env->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (env->seats[seatIdx].keyState[1][code] &&
            !(env->seats[seatIdx].keyState[0][code]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxBool AfxWereKeysPressed(afxUnit seat, afxUnit cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        afxBool cnt2 = 0;

        for (afxUnit i = 0; i < cnt; i++)
            if (env->seats[seat].keyState[0][codes[i]] && !(env->seats[seat].keyState[1][codes[i]]))
                cnt2++;

        rslt = cnt2;
    }
    return rslt;
}

_AUX afxMask AfxWereKeysPressed2(afxMask seats, afxUnit cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    for (afxUnit seatIdx = 0, seatCnt = env->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        afxBool cnt2 = 0;

        for (afxUnit i = 0; i < cnt; i++)
            if (env->seats[seatIdx].keyState[0][codes[i]] &&
                (!(env->seats[seatIdx].keyState[1][codes[i]])))
                cnt2++;

        if (cnt2 == cnt)
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxBool AfxWasKeyPressed(afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        rslt = (env->seats[seat].keyState[0][code] && !(env->seats[seat].keyState[1][code]));
    }
    return rslt;
}

_AUX afxMask AfxWasKeyPressed2(afxMask seats, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    for (afxUnit seatIdx = 0, seatCnt = env->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (env->seats[seatIdx].keyState[0][code] &&
            !(env->seats[seatIdx].keyState[1][code]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxBool AfxIsKeyPressed(afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        rslt = !!env->seats[seat].keyState[0][code];
    }
    return rslt;
}

_AUX afxMask AfxIsKeyPressed2(afxMask seats, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    for (afxUnit seatIdx = 0, seatCnt = env->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (!!env->seats[seatIdx].keyState[0][code])
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxReal AfxGetCombinedKeyPressure(afxUnit seat, afxKey lhs, afxKey rhs)
{
    afxError err = AFX_ERR_NONE;
    afxReal pressure = 0.0;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        pressure = -AFX_UN8_TO_R32(env->seats[seat].keyState[0][lhs]);
        pressure += AFX_UN8_TO_R32(env->seats[seat].keyState[0][rhs]);
    }
    return pressure;
}

_AUX afxReal AfxGetKeyPressure(afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxReal pressure = 0.0;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        pressure += AFX_UN8_TO_R32(env->seats[seat].keyState[0][code]);
    }
    return pressure;
}

_AUX afxResult AfxCountPressedKeys(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxResult c = 0;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        afxResult cc = c;

        for (; cc < afxKey_TOTAL; ++cc)
        {
            c += !!env->seats[seat].keyState[0][cc];
        }
    }
    return (c);
}

_AUX void AfxGetMouseMotion(afxUnit seat, afxV2d motion, afxV2d vel, afxV2d accel)
{
    afxError err = AFX_ERR_NONE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        motion[0] = env->seats[seat].motion[0][0];
        motion[1] = env->seats[seat].motion[0][1];

        if (vel)
        {
            vel[0] = env->seats[seat].motionVel[0][0];
            vel[1] = env->seats[seat].motionVel[0][1];
        }

        if (accel)
        {
            accel[0] = env->seats[seat].motionAcc[0][0];
            accel[1] = env->seats[seat].motionAcc[0][1];
        }
    }
}

_AUX afxReal AfxGetMouseWheelDelta(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return 0;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
        return env->seats[seat].wheelDelta[0];

    return 0;
}

_AUX afxBool AfxIsMousePressed(afxUnit seat, afxMouseButton mb)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
        if (mb < AFX_MB_TOTAL)
            rslt = (env->seats[seat].mbState[0][mb]);

    return rslt;
}

_AUX afxBool AfxWasMousePressed(afxUnit seat, afxMouseButton mb)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
        if (mb < AFX_MB_TOTAL)
            rslt = (env->seats[seat].mbState[0][mb] && !(env->seats[seat].mbState[1][mb]));

    return rslt;
}

_AUX afxBool AfxWasMouseReleased(afxUnit seat, afxMouseButton mb)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
        if (mb < AFX_MB_TOTAL)
            rslt = (env->seats[seat].mbState[1][mb] && !(env->seats[seat].mbState[0][mb]));

    return rslt;
}

_AUX afxMask AfxIsMousePressed2(afxMask seats, afxMouseButton mb)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    for (afxUnit seatIdx = 0, seatCnt = env->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (mb < AFX_MB_TOTAL)
        {
            if (env->seats[seatIdx].mbState[0][mb])
            {
                rslt |= AFX_BITMASK(seatIdx);
            }
        }
    }
    return rslt;
}

_AUX afxMask AfxWasMousePressed2(afxMask seats, afxMouseButton mb)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    for (afxUnit seatIdx = 0, seatCnt = env->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (mb < AFX_MB_TOTAL)
        {
            if (env->seats[seatIdx].mbState[0][mb] &&
                !(env->seats[seatIdx].mbState[1][mb]))
            {
                rslt |= AFX_BITMASK(seatIdx);
            }
        }
    }
    return rslt;
}

_AUX afxMask AfxWasMouseReleased2(afxMask seats, afxMouseButton mb)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    for (afxUnit seatIdx = 0, seatCnt = env->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (mb < AFX_MB_TOTAL)
        {
            if (env->seats[seatIdx].mbState[1][mb] &&
                !(env->seats[seatIdx].mbState[0][mb]))
            {
                rslt |= AFX_BITMASK(seatIdx);
            }
        }
    }
    return rslt;
}

_AUX afxBool AfxTestMouseMotionX(afxUnit seat, afxInt tolerance)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
        rslt = (((afxReal)AFX_ABS(env->seats[seat].motion[0][0] - env->seats[seat].motion[1][0])) >= (afxReal)tolerance);

    return rslt;
}

_AUX afxBool AfxTestMouseMotionZ(afxUnit seat, afxInt tolerance)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
        rslt = (((afxReal)AFX_ABS(env->seats[seat].motion[0][1] - env->seats[seat].motion[1][1])) >= (afxReal)tolerance);

    return rslt;
}

_AUX afxError AfxEmulateMouseMotion(afxUnit seat, afxV2d const motion)
{
    afxError err = AFX_ERR_NONE;

    // TODO: Drop afxWindow parameter and call session's event handler, which should pass input event for windows.

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        afxReal t = (afxReal)AfxGetTimer();
        afxReal dt = t - (afxReal)env->seats[seat].lastMotionTime;
        env->seats[seat].lastMotionTime = t;

        // Velocity (what most motion compensation uses).
        // velocity = (pos_now - pos_prev) / dt

        afxReal dx = motion[0] - env->seats[seat].motion[0][0];
        afxReal dy = motion[1] - env->seats[seat].motion[0][1];
        afxReal vx = 0, vy = 0;

        if (dt > 0)
        {
            vx = (afxReal)dx * 1000.0f / (afxReal)dt; // pixels per second
            vy = (afxReal)dy * 1000.0f / (afxReal)dt;
        }

        // Acceleration (less commonly needed).
        // acceleration = (velocity_now - velocity_prev) / dt

        env->seats[seat].motionAcc[1][0] = env->seats[seat].motionAcc[0][0];
        env->seats[seat].motionAcc[1][1] = env->seats[seat].motionAcc[0][1];
        env->seats[seat].motionAcc[0][0] = vx - env->seats[seat].motionVel[0][0];
        env->seats[seat].motionAcc[0][1] = vy - env->seats[seat].motionVel[0][1];

        env->seats[seat].motionVel[1][0] = env->seats[seat].motionVel[0][0];
        env->seats[seat].motionVel[1][1] = env->seats[seat].motionVel[0][1];
        env->seats[seat].motionVel[0][0] = vx;
        env->seats[seat].motionVel[0][1] = vy;

        env->seats[seat].motion[1][0] = env->seats[seat].motion[0][0];
        env->seats[seat].motion[1][1] = env->seats[seat].motion[0][1];
        env->seats[seat].motion[0][0] = motion[0];
        env->seats[seat].motion[0][1] = motion[1];

        // Let's say our UI rendering lags by 1 frame. You want to render the cursor slightly ahead of where it was last frame, 
        // based on how fast it was moving.
        // predicted_pos = current_pos + velocity * frame_delay_seconds;
        // That's velocity-based compensation; no acceleration needed.
        // If we wanted to simulate momentum (e.g., scroll continues after mouse release), then acceleration would help.

        // Velocity = what we want for compensating UI delay.
        // Acceleration = useful for gestures, momentum, or input prediction beyond 1 frame.
        // So unless we’re building something with inertia, momentum, or physics, we likely only need velocity.

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.ev.siz = sizeof(ev);
        ev.id = auxEventId_AXIS;
        ev.seat = seat;
        AfxEmitEvent(env, (void*)&ev);
    }
    return err;
}

_AUX afxError AfxEmulateMouseWheelAction(afxUnit seat, afxReal delta)
{
    afxError err = AFX_ERR_NONE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        env->seats[seat].wheelDelta[1] = env->seats[seat].wheelDelta[0];
        env->seats[seat].wheelDelta[0] = delta;

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.ev.siz = sizeof(ev);
        ev.id = auxEventId_WHEEL;
        ev.seat = seat;
        AfxEmitEvent(env, (void*)&ev);
    }
    return err;
}

_AUX afxError AfxEmulateMouseButtonActions(afxUnit seat, afxUnit cnt, afxMouseButton const butt[], afxBool const pressed[])
{
    afxError err = AFX_ERR_NONE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        AFX_ASSERT(cnt);
        AFX_ASSERT(butt);
        AFX_ASSERT(pressed);

        for (afxUnit i = 0; i < cnt; i++)
        {
            auxEventId evtype;

            switch (butt[i])
            {
            case AFX_LMB:
            {
                evtype = auxEventId_LMB;
                break;
            }
            case AFX_RMB:
            {
                evtype = auxEventId_RMB;
                break;
            }
            case AFX_MMB:
            {
                evtype = auxEventId_MMB;
                break;
            }
            case AFX_XMB1:
            {
                evtype = auxEventId_XMB1;
                break;
            }
            case AFX_XMB2:
            {
                evtype = auxEventId_XMB2;
                break;
            }
            default:
            {
                AfxThrowError();
                evtype = NIL;
                break;
            }
            }

            env->seats[seat].mbState[1][butt[i]] = env->seats[seat].mbState[0][butt[i]];
            env->seats[seat].mbState[0][butt[i]] = !!pressed[i];

            if (evtype)
            {
                auxEvent ev = { 0 };
                ev.ev.id = afxEvent_UX;
                ev.ev.siz = sizeof(ev);
                ev.id = evtype;
                ev.seat = seat;
                AfxEmitEvent(env, (void*)&ev);
            }
        }
    }
    return err;
}

_AUX afxError AfxReleaseMouseButtons(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (seat < env->seatCnt)
    {
        afxUnit cnt = env->seats[seat].buttonCnt;
        AFX_ASSERT(cnt);

        for (afxUnit i = 0; i < cnt; i++)
        {
            auxEventId evtype = auxEventId_LMB + i;

            env->seats[seat].mbState[1][i] = env->seats[seat].mbState[0][i];
            env->seats[seat].mbState[0][i] = FALSE;

            auxEvent ev = { 0 };
            ev.ev.id = afxEvent_UX;
            ev.ev.siz = sizeof(ev);
            ev.id = evtype;
            ev.seat = seat;
            AfxEmitEvent(env, (void*)&ev);
        }
    }
    return err;
}

_AUX afxError _AuxKbdDtorCb(afxKeyboard kbd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_KBD, 1, &kbd);



    return err;
}

_AUX afxResult _AuxKbdCtorCb(afxKeyboard kbd, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_KBD, 1, &kbd);

    afxEnvironment env = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    afxUnit const *ports = args[1];
    afxUnit seat = (afxUnit)ports[invokeNo];

    kbd->flags = NIL;

    // keyboard
    AfxZero(kbd->lastKeyState, sizeof(kbd->lastKeyState));
    AfxZero(kbd->prevKeyState, sizeof(kbd->prevKeyState));
    kbd->keyCnt = afxKey_TOTAL;
    kbd->fnKeyCnt = afxKey_F12 - afxKey_F1;

    // mouse
    AfxZero(kbd->lastMbState, sizeof(kbd->lastMbState));
    AfxZero(kbd->prevMbState, sizeof(kbd->prevMbState));
    kbd->buttonCnt = AFX_MB_TOTAL;
    kbd->lastMotion[0] = 0;
    kbd->lastMotion[1] = 0;
    kbd->lastWheelDelta = 0;
    kbd->prevWheelDelta = 0;
    kbd->sampleRate = 1;

    return err;
}

_AUX afxClassConfig const _AuxKbdStdImplementation =
{
    .fcc = afxFcc_KBD,
    .name = "Keyboard",
    .desc = "HID/Keyboard Interface",
    .fixedSiz = sizeof(AFX_OBJ(afxKeyboard)),
    .ctor = (void*)_AuxKbdCtorCb,
    .dtor = (void*)_AuxKbdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

