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

_AFX afxKey const vkDereferenceMap[afxKey_TOTAL];

afxKey const vkDereferenceMap[afxKey_TOTAL] =
{
    /*      0               1               2               3               4               5               6               7               8               9               A               B               C               D               E               F*/
    /* 0 */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           afxKey_TAB,    0x00,           0x00,           0x00,           afxKey_RET,    0x00,           0x00,
    /* 1 */ 0x00,           0x00,           0x00,           afxKey_PAUSE,  0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           afxKey_ESC,    0x00,           0x00,           0x00,           0x00,
    /* 2 */ afxKey_SPACE,  0x00,           0x00,           0x00,           0x00,           afxKey_LEFT,   afxKey_UP,     afxKey_RIGHT,  afxKey_DOWN,   0x00,           0x00,           0x00,           afxKey_PRINT,  afxKey_INSERT, afxKey_DELETE, 0x00,
    /* 3 */ afxKey_0,      afxKey_1,      afxKey_2,      afxKey_3,      afxKey_4,      afxKey_5,      afxKey_6,      afxKey_7,      afxKey_8,      afxKey_9,      0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* 4 */ 0x00,           afxKey_A,      afxKey_B,      afxKey_C,      afxKey_D,      afxKey_E,      afxKey_F,      afxKey_G,      afxKey_H,      afxKey_I,      afxKey_J,      afxKey_K,      afxKey_L,      afxKey_M,      afxKey_N,      afxKey_O,
    /* 5 */ afxKey_P,      afxKey_Q,      afxKey_R,      afxKey_S,      afxKey_T,      afxKey_U,      afxKey_V,      afxKey_W,      afxKey_X,      afxKey_Y,      afxKey_Z,      afxKey_LMETA,  afxKey_RMETA,  afxKey_APP,    0x00,           0x00,
    /* 6 */ afxKey_NUM0,   afxKey_NUM1,   afxKey_NUM2,   afxKey_NUM3,   afxKey_NUM4,   afxKey_NUM5,   afxKey_NUM6,   afxKey_NUM7,   afxKey_NUM8,   afxKey_NUM9,   0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* 7 */ afxKey_F1,     afxKey_F2,     afxKey_F3,     afxKey_F4,     afxKey_F5,     afxKey_F6,     afxKey_F7,     afxKey_F8,     afxKey_F9,     afxKey_F10,    afxKey_F11,    afxKey_F12,    0x00,           0x00,           0x00,           0x00,
    /* 8 */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* 9 */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* A */ afxKey_LSHIFT, afxKey_RSHIFT, afxKey_LCTRL,  afxKey_RCTRL,  afxKey_LALT,   afxKey_RALT,   0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* B */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* C */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* D */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* E */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* F */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00
};

_AFX afxKey const Scan1MakeToQwadroDereferenceMap[afxKey_TOTAL] =
{
    /*      0       1       2       3       4       5       6       7       8       9       A       B       C       D       E       F*/
    /* 0 */ 0x00,   afxKey_ESC,   afxKey_1,   afxKey_2,   afxKey_3,   afxKey_4,   afxKey_5,   afxKey_6,   afxKey_7,   afxKey_8,   afxKey_9,   afxKey_0,   afxKey_MINUS,   afxKey_EQUAL,   afxKey_BACK,   afxKey_TAB,
    /* 1 */ afxKey_Q,   afxKey_W,   afxKey_E,   afxKey_R,   afxKey_T,   afxKey_Y,   afxKey_U,   afxKey_I,   afxKey_O,   afxKey_P,   afxKey_LBRACE,   afxKey_RBRACE,   afxKey_RET,   0x00,   afxKey_A,   afxKey_S,
    /* 2 */ afxKey_D,   afxKey_F,   afxKey_G,   afxKey_H,   afxKey_J,   afxKey_K,   afxKey_L,   afxKey_SEMICOL,   afxKey_QUOTE,   afxKey_TILDE,   afxKey_LSHIFT,   afxKey_BACKSLASH,   afxKey_Z,   afxKey_X,   afxKey_C,   afxKey_V,
    /* 3 */ afxKey_B,   afxKey_N,   afxKey_M,   afxKey_COMMA,   afxKey_PERIOD,   afxKey_SLASH,   0x00,   0x00,   0x00,   afxKey_SPACE,   afxKey_CAPSLOCK,   afxKey_F1,   afxKey_F2,   afxKey_F3,   afxKey_F4,   afxKey_F5,
    /* 4 */ afxKey_F6,   afxKey_F7,   afxKey_F8,   afxKey_F9,   afxKey_F10,   afxKey_PAUSE,   afxKey_SCROLLOCK,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    /* 5 */ 0x00,   0x00,   0x00,   0x00,   afxKey_PRINT,   0x00,   0x00,   afxKey_F11,   afxKey_F12,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
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
            AfxLogAdvertence("HID %p connected.", msg->lParam);
        }
        else if (msg->wParam == GIDC_REMOVAL)
        {
            AfxLogAdvertence("HID %p disconnected.", msg->lParam);
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
                //afxKey key = vkDereferenceMap[rid->data.keyboard.VKey];
                afxNat8 pressure = (RI_KEY_BREAK == (rid->data.keyboard.Flags & RI_KEY_BREAK)) ? 0x00 : 0xFF; //!!(rid->data.keyboard.Message == WM_KEYDOWN || rid->data.keyboard.Message == WM_SYSKEYDOWN);
                AfxEmulatePressedKeys(0, 1, &key2, &pressure);
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

_AFX afxClassConfig const _AfxHidMgrCfg =
{
    .fcc = afxFcc_HID,
    .name = "HID",
    .desc = "Human Input Handling",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxHid)),
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxNat AfxInvokeHids(afxNat first, afxNat cnt, afxBool(*f)(afxHid, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetHidManager();
    AfxAssertClass(cls, afxFcc_HID);
    return cnt ? AfxInvokeObjects(cls, first, cnt, (void*)f, udd) : 0;
}

_AFX afxNat AfxEnumerateHids(afxNat first, afxNat cnt, afxHid hids[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(hids);
    afxManager* cls = AfxGetHidManager();
    AfxAssertClass(cls, afxFcc_HID);
    return cnt ? AfxEnumerateObjects(cls, first, cnt, (afxObject*)hids) : 0;
}

_AFX afxNat AfxCountHids(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetHidManager();
    AfxAssertClass(cls, afxFcc_HID);
    return AfxCountObjects(cls);
}
