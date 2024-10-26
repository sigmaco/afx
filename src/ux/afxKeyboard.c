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

#define _AFX_CORE_C
#define _AFX_DEVICE_C

#define _AUX_UX_C
#define _AUX_HID_C
#define _AFX_CONTEXT_C
#define _AUX_KEYBOARD_C
#define _AUX_CONTROLLER_C
#define _AUX_SESSION_C
#include "../dev/AuxImplKit.h"

_AUX afxError AfxEmulatePressedKeys(afxSession ses, afxUnit seat, afxUnit cnt, afxKey const key[], afxUnit8 const pressure[], afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

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

_AUX afxError AfxReleaseAllKeys(afxSession ses, afxUnit seat, afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

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

_AUX afxBool AfxWereKeysReleased(afxSession ses, afxUnit seat, afxUnit cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

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

_AUX afxBool AfxWasKeyReleased(afxSession ses, afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (seat < ses->seatCnt)
    {
        rslt = (ses->seats[seat].keyState[1][code] && !(ses->seats[seat].keyState[0][code]));
    }
    return rslt;
}

_AUX afxBool AfxWereKeysPressed(afxSession ses, afxUnit seat, afxUnit cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

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

_AUX afxBool AfxWasKeyPressed(afxSession ses, afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (seat < ses->seatCnt)
    {
        rslt = (ses->seats[seat].keyState[0][code] && !(ses->seats[seat].keyState[1][code]));
    }
    return rslt;
}

_AUX afxBool AfxIsKeyPressed(afxSession ses, afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (seat < ses->seatCnt)
    {
        rslt = !!ses->seats[seat].keyState[0][code];
    }
    return rslt;
}

_AUX afxReal AfxGetCombinedKeyPressure(afxSession ses, afxUnit seat, afxKey lhs, afxKey rhs)
{
    afxError err = AFX_ERR_NONE;
    afxReal pressure = 0.0;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (seat < ses->seatCnt)
    {
        pressure = -AfxRealFromByte(ses->seats[seat].keyState[0][lhs]);
        pressure += AfxRealFromByte(ses->seats[seat].keyState[0][rhs]);
    }
    return pressure;
}

_AUX afxReal AfxGetKeyPressure(afxSession ses, afxUnit seat, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxReal pressure = 0.0;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (seat < ses->seatCnt)
    {
        pressure += AfxRealFromByte(ses->seats[seat].keyState[0][code]);
    }
    return pressure;
}

_AUX afxResult AfxCountPressedKeys(afxSession ses, afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxResult c = 0;
    AfxAssertObjects(1, &ses, afxFcc_SES);

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
    AfxAssertObjects(1, &kbd, afxFcc_KBD);



    return err;
}

_AUX afxResult _AuxKbdCtorCb(afxKeyboard kbd, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &kbd, afxFcc_KBD);

    afxUnit const *ports = args[0];
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
    AfxAssertClass(cls, afxFcc_CTRL);

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
            AfxReleaseObjects(i, (afxObject*)controllers);
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
        AfxAssertObjects(1, &ctrl, afxFcc_CTRL);

        if (ctrl->hid.seat == seat)
            return ctrl;

        ++i;
    }
    return NIL;
}
#endif
