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

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <hidusage.h>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_HID_C
#define _AFX_KEYBOARD_C
#include "qwadro/core/afxSystem.h"

_AFX afxKey const kbdLayoutUs[AFX_KEY_TOTAL];
_AFX afxKey const kbdLayoutBr[AFX_KEY_TOTAL];

afxKey const kbdLayoutUs[AFX_KEY_TOTAL] =
{
    0x00, 0x29, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2D, 0x2E, 0x2A, 0x2B,
    0x14, 0x1A, 0x08, 0x15, 0x17, 0x1C, 0x18, 0x0C, 0x12, 0x13, 0x2F, 0x30, 0x9E, 0xE0, 0x04, 0x16,
    0x07, 0x09, 0x0A, 0x0B, 0x0D, 0x0E, 0x0F, 0x33, 0x34, 0x35, 0xE1, 0x31, 0x1D, 0x1B, 0x06, 0x19,
    0x05, 0x11, 0x10, 0x36, 0x37, 0x38, 0xE5, 0x00, 0xE2, 0x00, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E,
    0x3F, 0x40, 0x41, 0x42, 0x43, 0x48, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE4, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0xE6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4A, 0x52, 0x4B, 0x00, 0x50, 0x00, 0x4F, 0x00, 0x4D,
    0x51, 0x4E, 0x49, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE3, 0xE7, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

afxKey const kbdLayoutBr[AFX_KEY_TOTAL] =
{
    0x00, 0x29, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2D, 0x2E, 0x2A, 0x2B,
    0x14, 0x1A, 0x08, 0x15, 0x17, 0x1C, 0x18, 0x0C, 0x12, 0x13, 0x2F, 0x30, 0x9E, 0xE0, 0x04, 0x16,
    0x07, 0x09, 0x0A, 0x0B, 0x0D, 0x0E, 0x0F, 0x33, 0x34, 0x35, 0xE1, 0x31, 0x1D, 0x1B, 0x06, 0x19,
    0x05, 0x11, 0x10, 0x36, 0x37, 0x38, 0xE5, 0x00, 0xE2, 0x00, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E,
    0x3F, 0x40, 0x41, 0x42, 0x43, 0x48, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE4, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0xE6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4A, 0x52, 0x4B, 0x00, 0x50, 0x00, 0x4F, 0x00, 0x4D,
    0x51, 0x4E, 0x49, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE3, 0xE7, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

_AFX afxError AfxEmulatePressedKeys(afxNat port, afxNat cnt, afxKey const key[], afxNat8 const pressure[])
{
    afxError err = AFX_ERR_NONE;
    afxKeyboard kbd;

    if (AfxGetKeyboard(port, &kbd))
    {
        AfxAssertObjects(1, &kbd, afxFcc_KBD);
        AfxAssert(cnt);
        AfxAssert(key);
        AfxAssert(pressure);

        for (afxNat i = 0; i < cnt; i++)
        {
            afxEventId evtype = (pressure[i]) ? AFX_EVENT_KBD_PRESSED : AFX_EVENT_KBD_RELEASED;

            kbd->prevState[key[i]] = kbd->currState[key[i]];
            kbd->currState[key[i]] = pressure[i];

            afxEvent ev;
            AfxEventDeploy(&ev, evtype, (void*)kbd, (void*)&key[i]);
            AfxObjectEmitEvent(kbd, &ev);
        }
    }
    return err;
}

_AFX afxBool AfxKeysWereReleased(afxNat port, afxNat cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxKeyboard kbd;

    if (AfxGetKeyboard(port, &kbd))
    {
        AfxAssertObjects(1, &kbd, afxFcc_KBD);

        afxBool cnt2 = 0;

        for (afxNat i = 0; i < cnt; i++)
            if (kbd->prevState[codes[i]] && !(kbd->currState[codes[i]]))
                cnt2++;

        rslt = cnt2;
    }
    return rslt;
}

_AFX afxBool AfxKeyWasReleased(afxNat port, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxKeyboard kbd;

    if (AfxGetKeyboard(port, &kbd))
    {
        AfxAssertObjects(1, &kbd, afxFcc_KBD);

        rslt = (kbd->prevState[code] && !(kbd->currState[code]));
    }
    return rslt;
}

_AFX afxBool AfxKeysWerePressed(afxNat port, afxNat cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxKeyboard kbd;

    if (AfxGetKeyboard(port, &kbd))
    {
        AfxAssertObjects(1, &kbd, afxFcc_KBD);

        afxBool cnt2 = 0;

        for (afxNat i = 0; i < cnt; i++)
            if (kbd->currState[codes[i]] && !(kbd->prevState[codes[i]]))
                cnt2++;

        rslt = cnt2;
    }
    return rslt;
}

_AFX afxBool AfxKeyWasPressed(afxNat port, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxKeyboard kbd;

    if (AfxGetKeyboard(port, &kbd))
    {
        AfxAssertObjects(1, &kbd, afxFcc_KBD);

        rslt = (kbd->currState[code] && !(kbd->prevState[code]));
    }
    return rslt;
}

_AFX afxBool AfxKeyIsPressed(afxNat port, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxKeyboard kbd;

    if (AfxGetKeyboard(port, &kbd))
    {
        AfxAssertObjects(1, &kbd, afxFcc_KBD);
        rslt = !!kbd->currState[code];
    }
    return rslt;
}

_AFX afxReal AfxGetCombinedKeyPressure(afxNat port, afxKey lhs, afxKey rhs)
{
    afxError err = AFX_ERR_NONE;
    afxReal pressure = 0.0;
    afxKeyboard kbd;

    if (AfxGetKeyboard(port, &kbd))
    {
        AfxAssertObjects(1, &kbd, afxFcc_KBD);
        pressure = -AfxRealFromByte(kbd->currState[lhs]);
        pressure += AfxRealFromByte(kbd->currState[rhs]);
    }
    return pressure;
}

_AFX afxReal AfxGetKeyPressure(afxNat port, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxReal pressure = 0.0;
    afxKeyboard kbd;

    if (AfxGetKeyboard(port, &kbd))
    {
        AfxAssertObjects(1, &kbd, afxFcc_KBD);

        pressure += AfxRealFromByte(kbd->currState[code]);
    }
    return pressure;
}

_AFX afxResult AfxCountPressedKeys(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxResult c = 0;
    afxKeyboard kbd;

    if (AfxGetKeyboard(port, &kbd))
    {
        AfxAssertObjects(1, &kbd, afxFcc_KBD);

        afxResult cc = c;

        for (; cc < AFX_KEY_TOTAL; ++cc)
        {
            c += !!kbd->currState[cc];
        }
    }
    return (c);
}

_AFX afxError _AfxKbdDtor(afxKeyboard kbd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &kbd, afxFcc_KBD);
    return err;
}

_AFX afxResult _AfxKbdCtor(afxKeyboard kbd, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &kbd, afxFcc_KBD);

    afxNat const *ports = cookie->udd[0];
    afxNat port = (afxNat)ports[cookie->no];

    kbd->hid.port = port;
    kbd->hid.flags = NIL;

    AfxZero(kbd->currState, sizeof(kbd->currState));
    AfxZero(kbd->prevState, sizeof(kbd->prevState));
    kbd->keyCnt = AFX_KEY_TOTAL;
    kbd->fnKeyCnt = AFX_KEY_F12 - AFX_KEY_F1;

    static afxBool hasRidKbdBeenRegistered = FALSE;

    if (hasRidKbdBeenRegistered == FALSE)
    {
        RAWINPUTDEVICE rid;
        rid.usUsagePage = HID_USAGE_PAGE_GENERIC; // ((USAGE)0x01)
        rid.usUsage = HID_USAGE_GENERIC_KEYBOARD; // ((USAGE)0x06)
        rid.dwFlags = /*RIDEV_NOLEGACY | RIDEV_NOHOTKEYS |*/ RIDEV_DEVNOTIFY/* | RIDEV_APPKEYS*/; // ignores legacy keyboard messages
        rid.hwndTarget = NIL;

        if (!(hasRidKbdBeenRegistered = RegisterRawInputDevices(&(rid), sizeof(rid) / sizeof(rid), sizeof(rid)))) AfxThrowError();
        else
        {

        }
    }
    return err;
}

_AFX afxClassConfig const _AfxKbdClsConfig =
{
    .fcc = afxFcc_KBD,
    .name = "Keyboard",
    .desc = "HID/Keyboard Handling",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxKeyboard)),
    .mmu = NIL,
    .ctor = (void*)_AfxKbdCtor,
    .dtor = (void*)_AfxKbdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireKeyboard(afxNat port, afxKeyboard* kbd)
{
    afxError err = AFX_ERR_NONE;
    afxKeyboard kbd2 = NIL;

    if (AfxGetKeyboard(port, &kbd2))
    {
        AfxLogEcho("Reacquiring keyboard at port %u", port);

        if (AfxReacquireObjects(1, (void*[]) { kbd2 }))
            AfxThrowError();
    }
    else
    {
        AfxLogEcho("Acquiring keyboard at port %u", port);

        afxManager* cls = AfxGetKeyboardClass();
        AfxAssertClass(cls, afxFcc_KBD);

        if (AfxAcquireObjects(cls, 1, (afxObject*)&kbd2, (void const*[]) { &port }))
            AfxThrowError();
    }

    AfxAssert(kbd);
    *kbd = kbd2;
    return err;
}

_AFX afxNat AfxInvokeKeyboards(afxNat first, afxNat cnt, afxBool(*f)(afxKeyboard, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetKeyboardClass();
    AfxAssertClass(cls, afxFcc_KBD);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateKeyboards(afxNat first, afxNat cnt, afxKeyboard keyboards[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(keyboards);
    afxManager* cls = AfxGetKeyboardClass();
    AfxAssertClass(cls, afxFcc_KBD);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)keyboards);
}

_AFX afxNat AfxCountKeyboards(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetKeyboardClass();
    AfxAssertClass(cls, afxFcc_KBD);
    return AfxCountObjects(cls);
}

_AFX afxBool AfxGetKeyboard(afxNat port, afxKeyboard* kbd)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;
    afxNat i = 0;
    afxKeyboard kbd2;
    while (AfxEnumerateKeyboards(i, 1, &kbd2))
    {
        AfxAssertObjects(1, &kbd2, afxFcc_KBD);

        if (kbd2->hid.port == port)
        {
            found = TRUE;
            break;
        }
        ++i;
    }
    AfxAssert(kbd);
    *kbd = found ? kbd2 : NIL;
    return found;
}
