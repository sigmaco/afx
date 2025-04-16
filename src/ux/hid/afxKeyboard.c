/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_CORE_C
#define _AFX_DEVICE_C

#define _AUX_UX_C
#define _AUX_HID_C
#define _AFX_CONTEXT_C
#define _AUX_KEYBOARD_C
#define _AUX_CONTROLLER_C
#define _AUX_SESSION_C
#include "../impl/auxImplementation.h"

_AUX afxError AfxEmulatePressedKeys(afxUnit seat, afxUnit cnt, afxKey const key[], afxUnit8 const pressure[], afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        AFX_ASSERT(cnt);
        AFX_ASSERT(key);
        AFX_ASSERT(pressure);

        for (afxUnit i = 0; i < cnt; i++)
        {
            ses->seats[seat].keyState[1][key[i]] = ses->seats[seat].keyState[0][key[i]];
            ses->seats[seat].keyState[0][key[i]] = pressure[i];

            auxEvent ev = { 0 };
            ev.ev.id = afxEvent_UX;
            ev.ev.siz = sizeof(ev);
            ev.id = auxEventId_KEY;
            ev.seat = seat;
            ev.wnd = wnd;
            AfxEmitEvent(wnd, (void*)&ev.ev);
        }
    }
    return err;
}

_AUX afxError AfxReleaseAllKeys(afxUnit seat, afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        afxUnit cnt = ses->seats[seat].keyCnt;
        AFX_ASSERT(cnt);

        for (afxUnit i = 0; i < cnt; i++)
        {
            if (ses->seats[seat].keyState[0][i])
            {
                ses->seats[seat].keyState[1][i] = ses->seats[seat].keyState[0][i];
                ses->seats[seat].keyState[0][i] = 0x00;

                auxEvent ev = { 0 };
                ev.ev.id = afxEvent_UX;
                ev.ev.siz = sizeof(ev);
                ev.id = auxEventId_KEY;
                ev.seat = seat;
                ev.wnd = wnd;
                AfxEmitEvent(wnd, (void*)&ev.ev);
            }
        }
    }
    return err;
}

_AUX afxError AfxReleaseAllKeys2(afxMask seats, afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BIT(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        afxUnit cnt = ses->seats[seatIdx].keyCnt;
        AFX_ASSERT(cnt);

        for (afxUnit i = 0; i < cnt; i++)
        {
            if (ses->seats[seatIdx].keyState[0][i])
            {
                ses->seats[seatIdx].keyState[1][i] = ses->seats[seatIdx].keyState[0][i];
                ses->seats[seatIdx].keyState[0][i] = 0x00;

                auxEvent ev = { 0 };
                ev.ev.id = afxEvent_UX;
                ev.ev.siz = sizeof(ev);
                ev.id = auxEventId_KEY;
                ev.seat = seatIdx;
                ev.wnd = wnd;
                AfxEmitEvent(wnd, (void*)&ev.ev);
            }
        }
    }
    return err;
}

_AUX afxBool AfxWereKeysReleased(afxUnit seat, afxUnit cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        afxBool cnt2 = 0;

        for (afxUnit i = 0; i < cnt; i++)
            if (ses->seats[seat].keyState[1][codes[i]] && !(ses->seats[seat].keyState[0][codes[i]]))
                cnt2++;

        rslt = cnt2;
    }
    return rslt;
}

_AUX afxMask AfxWereKeysReleased2(afxMask seats, afxUnit cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BIT(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        afxBool cnt2 = 0;

        for (afxUnit i = 0; i < cnt; i++)
            if (ses->seats[seatIdx].keyState[1][codes[i]] &&
                !(ses->seats[seatIdx].keyState[0][codes[i]]))
                cnt2++;

        if (cnt2 == cnt)
        {
            rslt |= AFX_BIT(seatIdx);
        }
    }
    return rslt;
}

_AUX afxBool AfxWasKeyReleased(afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        rslt = (ses->seats[seat].keyState[1][code] && !(ses->seats[seat].keyState[0][code]));
    }
    return rslt;
}

_AUX afxMask AfxWasKeyReleased2(afxMask seats, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BIT(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].keyState[1][code] &&
            !(ses->seats[seatIdx].keyState[0][code]))
        {
            rslt |= AFX_BIT(seatIdx);
        }
    }
    return rslt;
}

_AUX afxBool AfxWereKeysPressed(afxUnit seat, afxUnit cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        afxBool cnt2 = 0;

        for (afxUnit i = 0; i < cnt; i++)
            if (ses->seats[seat].keyState[0][codes[i]] && !(ses->seats[seat].keyState[1][codes[i]]))
                cnt2++;

        rslt = cnt2;
    }
    return rslt;
}

_AUX afxMask AfxWereKeysPressed2(afxMask seats, afxUnit cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BIT(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        afxBool cnt2 = 0;

        for (afxUnit i = 0; i < cnt; i++)
            if (ses->seats[seatIdx].keyState[0][codes[i]] &&
                (!(ses->seats[seatIdx].keyState[1][codes[i]])))
                cnt2++;

        if (cnt2 == cnt)
        {
            rslt |= AFX_BIT(seatIdx);
        }
    }
    return rslt;
}

_AUX afxBool AfxWasKeyPressed(afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        rslt = (ses->seats[seat].keyState[0][code] && !(ses->seats[seat].keyState[1][code]));
    }
    return rslt;
}

_AUX afxMask AfxWasKeyPressed2(afxMask seats, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BIT(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].keyState[0][code] &&
            !(ses->seats[seatIdx].keyState[1][code]))
        {
            rslt |= AFX_BIT(seatIdx);
        }
    }
    return rslt;
}

_AUX afxBool AfxIsKeyPressed(afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        rslt = !!ses->seats[seat].keyState[0][code];
    }
    return rslt;
}

_AUX afxMask AfxIsKeyPressed2(afxMask seats, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BIT(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (!!ses->seats[seatIdx].keyState[0][code])
        {
            rslt |= AFX_BIT(seatIdx);
        }
    }
    return rslt;
}

_AUX afxReal AfxGetCombinedKeyPressure(afxUnit seat, afxKey lhs, afxKey rhs)
{
    afxError err = AFX_ERR_NONE;
    afxReal pressure = 0.0;

    afxSession ses;
    if (!AfxGetSession(&ses)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        pressure = -AfxRealFromByte(ses->seats[seat].keyState[0][lhs]);
        pressure += AfxRealFromByte(ses->seats[seat].keyState[0][rhs]);
    }
    return pressure;
}

_AUX afxReal AfxGetKeyPressure(afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxReal pressure = 0.0;

    afxSession ses;
    if (!AfxGetSession(&ses)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        pressure += AfxRealFromByte(ses->seats[seat].keyState[0][code]);
    }
    return pressure;
}

_AUX afxResult AfxCountPressedKeys(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxResult c = 0;

    afxSession ses;
    if (!AfxGetSession(&ses)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        afxResult cc = c;

        for (; cc < afxKey_TOTAL; ++cc)
        {
            c += !!ses->seats[seat].keyState[0][cc];
        }
    }
    return (c);
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

    afxSession ses = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    afxUnit const *ports = args[1];
    afxUnit seat = (afxUnit)ports[invokeNo];

    kbd->flags = NIL;

    // keyboard
    AfxZero(kbd->lastKeyState, sizeof(kbd->lastKeyState));
    AfxZero(kbd->prevKeyState, sizeof(kbd->prevKeyState));
    kbd->keyCnt = afxKey_TOTAL;
    kbd->fnKeyCnt = afxKey_F12 - afxKey_F1;

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

#if 0
_AUX afxError AfxAcquireControllers(afxUnit cnt, afxUnit const seat[], afxController controllers[])
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetControllerClass();
    AFX_ASSERT_CLASS(cls, afxFcc_CTRL);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxController tmp;
        
        if ((tmp = AfxGetController(seat ? seat[i] : 0)))
        {
            if (AfxReacquireObjects(1, (afxObject*)&tmp)) AfxThrowError();
            else
            {
                controllers[i] = tmp;
            }
        }
        else
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&controllers[i], (void const*[]) { seat ? (void*)&seat[i] : 0 }))
                AfxThrowError();
        }

        if (err)
        {
            AfxDisposeObjects(i, (afxObject*)controllers);
            break;
        }
    }
    return err;
}

_AUX afxController AfxGetController(afxSession ses, afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxUnit i = 0;
    afxController ctrl;
    while (AfxEnumerateControllers(i, 1, &ctrl))
    {
        AFX_ASSERT_OBJECTS(afxFcc_CTRL, 1, &ctrl)

        if (ctrl->hid.seat == seat)
            return ctrl;

        ++i;
    }
    return NIL;
}
#endif
