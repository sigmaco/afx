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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1

#include <sys/stat.h>
#include <stdio.h>

#if (defined(_WIN32) || defined(_WIN64))
#   include <Windows.h>
#endif

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_HID_C
#include "qwadro/core/afxSystem.h"
#include "qwadro/io/afxHid.h"

_AFX afxKey const vkDereferenceMap[AFX_KEY_TOTAL];

afxKey const vkDereferenceMap[AFX_KEY_TOTAL] =
{
    /*      0               1               2               3               4               5               6               7               8               9               A               B               C               D               E               F*/
    /* 0 */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           AFX_KEY_TAB,    0x00,           0x00,           0x00,           AFX_KEY_RET,    0x00,           0x00,
    /* 1 */ 0x00,           0x00,           0x00,           AFX_KEY_PAUSE,  0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           AFX_KEY_ESC,    0x00,           0x00,           0x00,           0x00,
    /* 2 */ AFX_KEY_SPACE,  0x00,           0x00,           0x00,           0x00,           AFX_KEY_LEFT,   AFX_KEY_UP,     AFX_KEY_RIGHT,  AFX_KEY_DOWN,   0x00,           0x00,           0x00,           AFX_KEY_PRINT,  AFX_KEY_INSERT, AFX_KEY_DELETE, 0x00,
    /* 3 */ AFX_KEY_0,      AFX_KEY_1,      AFX_KEY_2,      AFX_KEY_3,      AFX_KEY_4,      AFX_KEY_5,      AFX_KEY_6,      AFX_KEY_7,      AFX_KEY_8,      AFX_KEY_9,      0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* 4 */ 0x00,           AFX_KEY_A,      AFX_KEY_B,      AFX_KEY_C,      AFX_KEY_D,      AFX_KEY_E,      AFX_KEY_F,      AFX_KEY_G,      AFX_KEY_H,      AFX_KEY_I,      AFX_KEY_J,      AFX_KEY_K,      AFX_KEY_L,      AFX_KEY_M,      AFX_KEY_N,      AFX_KEY_O,
    /* 5 */ AFX_KEY_P,      AFX_KEY_Q,      AFX_KEY_R,      AFX_KEY_S,      AFX_KEY_T,      AFX_KEY_U,      AFX_KEY_V,      AFX_KEY_W,      AFX_KEY_X,      AFX_KEY_Y,      AFX_KEY_Z,      AFX_KEY_LMETA,  AFX_KEY_RMETA,  AFX_KEY_APP,    0x00,           0x00,
    /* 6 */ AFX_KEY_NUM0,   AFX_KEY_NUM1,   AFX_KEY_NUM2,   AFX_KEY_NUM3,   AFX_KEY_NUM4,   AFX_KEY_NUM5,   AFX_KEY_NUM6,   AFX_KEY_NUM7,   AFX_KEY_NUM8,   AFX_KEY_NUM9,   0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* 7 */ AFX_KEY_F1,     AFX_KEY_F2,     AFX_KEY_F3,     AFX_KEY_F4,     AFX_KEY_F5,     AFX_KEY_F6,     AFX_KEY_F7,     AFX_KEY_F8,     AFX_KEY_F9,     AFX_KEY_F10,    AFX_KEY_F11,    AFX_KEY_F12,    0x00,           0x00,           0x00,           0x00,
    /* 8 */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* 9 */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* A */ AFX_KEY_LSHIFT, AFX_KEY_RSHIFT, AFX_KEY_LCTRL,  AFX_KEY_RCTRL,  AFX_KEY_LALT,   AFX_KEY_RALT,   0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* B */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* C */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* D */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* E */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* F */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00
};

_AFX afxKey const Scan1MakeToQwadroDereferenceMap[AFX_KEY_TOTAL] =
{
    /*      0       1       2       3       4       5       6       7       8       9       A       B       C       D       E       F*/
    /* 0 */ 0x00,   AFX_KEY_ESC,   AFX_KEY_1,   AFX_KEY_2,   AFX_KEY_3,   AFX_KEY_4,   AFX_KEY_5,   AFX_KEY_6,   AFX_KEY_7,   AFX_KEY_8,   AFX_KEY_9,   AFX_KEY_0,   AFX_KEY_MINUS,   AFX_KEY_EQUAL,   AFX_KEY_BACK,   AFX_KEY_TAB,
    /* 1 */ AFX_KEY_Q,   AFX_KEY_W,   AFX_KEY_E,   AFX_KEY_R,   AFX_KEY_T,   AFX_KEY_Y,   AFX_KEY_U,   AFX_KEY_I,   AFX_KEY_O,   AFX_KEY_P,   AFX_KEY_LBRACE,   AFX_KEY_RBRACE,   AFX_KEY_RET,   0x00,   AFX_KEY_A,   AFX_KEY_S,
    /* 2 */ AFX_KEY_D,   AFX_KEY_F,   AFX_KEY_G,   AFX_KEY_H,   AFX_KEY_J,   AFX_KEY_K,   AFX_KEY_L,   AFX_KEY_SEMICOL,   AFX_KEY_QUOTE,   AFX_KEY_TILDE,   AFX_KEY_LSHIFT,   AFX_KEY_BACKSLASH,   AFX_KEY_Z,   AFX_KEY_X,   AFX_KEY_C,   AFX_KEY_V,
    /* 3 */ AFX_KEY_B,   AFX_KEY_N,   AFX_KEY_M,   AFX_KEY_COMMA,   AFX_KEY_PERIOD,   AFX_KEY_SLASH,   0x00,   0x00,   0x00,   AFX_KEY_SPACE,   AFX_KEY_CAPSLOCK,   AFX_KEY_F1,   AFX_KEY_F2,   AFX_KEY_F3,   AFX_KEY_F4,   AFX_KEY_F5,
    /* 4 */ AFX_KEY_F6,   AFX_KEY_F7,   AFX_KEY_F8,   AFX_KEY_F9,   AFX_KEY_F10,   AFX_KEY_PAUSE,   AFX_KEY_SCROLLOCK,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* 5 */ 0x00,   0x00,   0x00,   0x00,   AFX_KEY_PRINT,   0x00,   0x00,   AFX_KEY_F11,   AFX_KEY_F12,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* 6 */ 0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* 7 */ 0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* 8 */ 0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* 9 */ 0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* A */ 0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* B */ 0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* C */ 0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* D */ 0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* E */ 0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* F */ 0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00
};

_AFX afxResult _AfxProcessSystemInputMessageWin32(MSG* msg)
{
    afxError err = AFX_ERR_NONE;
    static afxByte bytes[4096] = { 0 };
    //afxNat32 foreOrBackground = GET_RAWINPUT_CODE_WPARAM(msg->wParam);
    UINT bufSiz;
    afxResult rslt = 0;

    if (msg->message == WM_INPUT_DEVICE_CHANGE)
    {
        if (msg->wParam == GIDC_ARRIVAL)
        {
            AfxAdvertise("HID %p connected.", msg->lParam);
        }
        else if (msg->wParam == GIDC_REMOVAL)
        {
            AfxAdvertise("HID %p disconnected.", msg->lParam);
        }
        else
        {
            AfxThrowError();
        }
    }
    else if (msg->message == WM_INPUT)
    {
        bufSiz = sizeof(bytes);
        GetRawInputData((HRAWINPUT)msg->lParam, RID_INPUT, NIL, &(bufSiz), sizeof(RAWINPUTHEADER));
        AfxAssert(bufSiz <= sizeof(bytes));

        if ((bufSiz != GetRawInputData((HRAWINPUT)msg->lParam, RID_INPUT, bytes, &(bufSiz), sizeof(RAWINPUTHEADER)))) AfxThrowError();
        else
        {
            RAWINPUT* rid = (RAWINPUT*)bytes;

            if (rid->header.dwType == RIM_TYPEMOUSE)
            {
                afxNat butChangeCnt = 0;
                afxMouseButton buttons[AFX_MB_TOTAL];
                afxBool pressed[AFX_MB_TOTAL];

                if (rid->data.mouse.usButtonFlags & (RI_MOUSE_LEFT_BUTTON_DOWN | RI_MOUSE_LEFT_BUTTON_UP))
                {
                    buttons[butChangeCnt] = AFX_LMB;
                    pressed[butChangeCnt] = (RI_MOUSE_LEFT_BUTTON_DOWN == (rid->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN));
                    butChangeCnt++;
                }

                if (rid->data.mouse.usButtonFlags & (RI_MOUSE_RIGHT_BUTTON_DOWN | RI_MOUSE_RIGHT_BUTTON_UP))
                {
                    buttons[butChangeCnt] = AFX_RMB;
                    pressed[butChangeCnt] = (RI_MOUSE_RIGHT_BUTTON_DOWN == (rid->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN));
                    butChangeCnt++;
                }

                if (rid->data.mouse.usButtonFlags & (RI_MOUSE_MIDDLE_BUTTON_DOWN | RI_MOUSE_MIDDLE_BUTTON_UP))
                {
                    buttons[butChangeCnt] = AFX_MMB;
                    pressed[butChangeCnt] = (RI_MOUSE_MIDDLE_BUTTON_DOWN == (rid->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN));
                    butChangeCnt++;
                }

                if (rid->data.mouse.usButtonFlags & (RI_MOUSE_BUTTON_4_DOWN | RI_MOUSE_BUTTON_4_UP))
                {
                    buttons[butChangeCnt] = AFX_XMB1;
                    pressed[butChangeCnt] = (RI_MOUSE_BUTTON_4_DOWN == (rid->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN));
                    butChangeCnt++;
                }

                if (rid->data.mouse.usButtonFlags & (RI_MOUSE_BUTTON_5_DOWN | RI_MOUSE_BUTTON_5_UP))
                {
                    buttons[butChangeCnt] = AFX_XMB2;
                    pressed[butChangeCnt] = (RI_MOUSE_BUTTON_5_DOWN == (rid->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN));
                    butChangeCnt++;
                }

                if (butChangeCnt)
                    AfxEmulateMouseButtonActions(0, butChangeCnt, buttons, pressed);

                afxReal motion[2] = { (afxReal)rid->data.mouse.lLastX, (afxReal)rid->data.mouse.lLastY };
                AfxEmulateMouseMotion(0, motion);

                if (RI_MOUSE_WHEEL == (rid->data.mouse.usButtonFlags & RI_MOUSE_WHEEL))
                {
                    afxReal wheel = (afxInt16)rid->data.mouse.usButtonData;
                    AfxEmulateMouseWheelAction(0, wheel);
                }
            }
            else if (rid->header.dwType == RIM_TYPEKEYBOARD)
            {
                afxKey key2 = Scan1MakeToQwadroDereferenceMap[rid->data.keyboard.MakeCode];
                afxKey key = vkDereferenceMap[rid->data.keyboard.VKey];
                afxBool pressed = !(RI_KEY_BREAK == (rid->data.keyboard.Flags & RI_KEY_BREAK)); //!!(rid->data.keyboard.Message == WM_KEYDOWN || rid->data.keyboard.Message == WM_SYSKEYDOWN);
                AfxEmulateKeysAction(0, 1, &key2, &pressed);
            }
        }
    }
    return rslt;
}

_AFXINL afxNat AfxGetHidPort(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    return hid->port;
}

_AFXINL afxResult AfxTestHidFlags(afxHid hid, afxHidFlag flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(hid, afxFcc_HID);
    AfxAssert(flags);
    return (hid->flags & flags);
}

_AFX afxClassConfig const _AfxHidClsConfig =
{
    .fcc = afxFcc_HID,
    .name = "Human Input Device",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxHid)),
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxNat AfxCurateHids(afxNat first, afxNat cnt, afxBool(*f)(afxHid, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetHidClass();
    AfxAssertClass(cls, afxFcc_HID);
    return cnt ? AfxCurateInstances(cls, first, cnt, (void*)f, udd) : 0;
}

_AFX afxNat AfxEnumerateHids(afxNat first, afxNat cnt, afxHid hid[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(hid);
    afxClass* cls = AfxGetHidClass();
    AfxAssertClass(cls, afxFcc_HID);
    return cnt ? AfxEnumerateInstances(cls, first, cnt, (afxObject*)hid) : 0;
}

_AFX afxNat AfxCountHids(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetHidClass();
    AfxAssertClass(cls, afxFcc_HID);
    return AfxCountInstances(cls);
}
