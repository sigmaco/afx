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

#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN 1
#   include <Windows.h>
#   include <hidusage.h>
#endif

#define _AFX_UX_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_HID_C
#include "qwadro/ux/afxShell.h"

_AUX afxKey const vkDereferenceMap[afxKey_TOTAL];
_AUX afxKey const kbdLayoutUs[afxKey_TOTAL];
_AUX afxKey const kbdLayoutBr[afxKey_TOTAL];

afxKey const kbdLayoutUs[afxKey_TOTAL] =
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

afxKey const kbdLayoutBr[afxKey_TOTAL] =
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

afxKey const vkDereferenceMap[afxKey_TOTAL] =
{
    /*      0               1               2               3               4               5               6               7               8               9               A               B               C               D               E               F*/
    /* 0 */ 0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           afxKey_TAB,    0x00,           0x00,           0x00,           afxKey_RET,    0x00,           0x00,
    /* 1 */ 0x00,           0x00,           0x00,           afxKey_PAUSE,  0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           0x00,           afxKey_ESC,    0x00,           0x00,           0x00,           0x00,
    /* 2 */ afxKey_SPACE,  0x00,           0x00,           0x00,           0x00,           afxKey_LEFT,   afxKey_UP,     afxKey_RIGHT,  afxKey_DOWN,   0x00,           0x00,           0x00,           afxKey_PRINT,  afxKey_INSERT, afxKey_DELETE, 0x00,
    /* 3 */ afxKey_C0,      afxKey_C1,      afxKey_C2,      afxKey_C3,      afxKey_C4,      afxKey_C5,      afxKey_C6,      afxKey_C7,      afxKey_C8,      afxKey_C9,      0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
    /* 4 */ 0x00,           afxKey_A,      afxKey_B,      afxKey_C,      afxKey_D,      afxKey_E,      afxKey_F,      afxKey_G,      afxKey_H,      afxKey_I,      afxKey_J,      afxKey_K,      afxKey_L,      afxKey_M,      afxKey_N,      afxKey_O,
    /* 5 */ afxKey_P,      afxKey_Q,      afxKey_R,      afxKey_S,      afxKey_T,      afxKey_U,      afxKey_V,      afxKey_W,      afxKey_X,      afxKey_Y,      afxKey_Z,      afxKey_LMETA,  afxKey_RMETA,  afxKey_APP,    0x00,           0x00,
    /* 6 */ afxKey_N0,   afxKey_N1,   afxKey_N2,   afxKey_N3,   afxKey_N4,   afxKey_N5,   afxKey_N6,   afxKey_N7,   afxKey_N8,   afxKey_N9,   0x00,           0x00,           0x00,           0x00,           0x00,           0x00,
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

_AUX afxKey const Scan1MakeToQwadroDereferenceMap[afxKey_TOTAL] =
{
    /*      0       1       2       3       4       5       6       7       8       9       A       B       C       D       E       F*/
    /* 0 */ 0x00,   afxKey_ESC,   afxKey_C1,   afxKey_C2,   afxKey_C3,   afxKey_C4,   afxKey_C5,   afxKey_C6,   afxKey_C7,   afxKey_C8,   afxKey_C9,   afxKey_C0,   afxKey_MINUS,   afxKey_EQUAL,   afxKey_BACK,   afxKey_TAB,
    /* 1 */ afxKey_Q,   afxKey_W,   afxKey_E,   afxKey_R,   afxKey_T,   afxKey_Y,   afxKey_U,   afxKey_I,   afxKey_O,   afxKey_P,   afxKey_LBRACE,   afxKey_RBRACE,   afxKey_RET,   0x00,   afxKey_A,   afxKey_S,
    /* 2 */ afxKey_D,   afxKey_F,   afxKey_G,   afxKey_H,   afxKey_J,   afxKey_K,   afxKey_L,   afxKey_SEMICOL,   afxKey_QUOTE,   afxKey_TILDE,   afxKey_LSHIFT,   afxKey_BACKSLASH,   afxKey_Z,   afxKey_X,   afxKey_C,   afxKey_V,
    /* 3 */ afxKey_B,   afxKey_N,   afxKey_M,   afxKey_COMMA,   afxKey_PERIOD,   afxKey_SLASH,   0x00,   0x00,   0x00,   afxKey_SPACE,   afxKey_CLOCK,   afxKey_F1,   afxKey_F2,   afxKey_F3,   afxKey_F4,   afxKey_F5,
    /* 4 */ afxKey_F6,   afxKey_F7,   afxKey_F8,   afxKey_F9,   afxKey_F10,   afxKey_PAUSE,   afxKey_SLOCK,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
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

_AUX afxResult _AfxProcessSystemInputMessageWin32(MSG* msg)
{
    afxError err = AFX_ERR_NONE;
    static afxByte bytes[4096] = { 0 };
    //afxNat32 foreOrBackground = GET_RAWINPUT_CODE_WPARAM(msg->wParam);
    UINT bufSiz;
    afxResult rslt = 0;

    if (msg->message == WM_INPUT_DEVICE_CHANGE)
    {
        HANDLE hDevice = (void*)msg->lParam;
        AfxAssert(hDevice);
        RID_DEVICE_INFO info = { 0 };
        info.cbSize = sizeof(info);
        UINT cbSize = info.cbSize;
        GetRawInputDeviceInfo(hDevice, RIDI_DEVICEINFO, &info, &cbSize);

        if (msg->wParam == GIDC_ARRIVAL)
        {
            AfxLogAdvertence("HID %p connected.", hDevice);
        }
        else if (msg->wParam == GIDC_REMOVAL)
        {
            AfxLogAdvertence("HID %p disconnected.", hDevice);
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
                USHORT usButtonFlags = rid->data.mouse.usButtonFlags;

                if (usButtonFlags & (RI_MOUSE_LEFT_BUTTON_DOWN | RI_MOUSE_LEFT_BUTTON_UP))
                {
                    buttons[butChangeCnt] = AFX_LMB;
                    pressed[butChangeCnt] = (RI_MOUSE_LEFT_BUTTON_DOWN == (usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN));
                    butChangeCnt++;
                }

                if (usButtonFlags & (RI_MOUSE_RIGHT_BUTTON_DOWN | RI_MOUSE_RIGHT_BUTTON_UP))
                {
                    buttons[butChangeCnt] = AFX_RMB;
                    pressed[butChangeCnt] = (RI_MOUSE_RIGHT_BUTTON_DOWN == (usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN));
                    butChangeCnt++;
                }

                if (usButtonFlags & (RI_MOUSE_MIDDLE_BUTTON_DOWN | RI_MOUSE_MIDDLE_BUTTON_UP))
                {
                    buttons[butChangeCnt] = AFX_MMB;
                    pressed[butChangeCnt] = (RI_MOUSE_MIDDLE_BUTTON_DOWN == (usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN));
                    butChangeCnt++;
                }

                if (usButtonFlags & ((RI_MOUSE_BUTTON_4_DOWN | RI_MOUSE_BUTTON_4_UP) | (RI_MOUSE_BUTTON_5_DOWN | RI_MOUSE_BUTTON_5_UP)))
                {
                    if (usButtonFlags & (RI_MOUSE_BUTTON_4_DOWN | RI_MOUSE_BUTTON_4_UP))
                    {
                        buttons[butChangeCnt] = AFX_XMB1;
                        pressed[butChangeCnt] = (RI_MOUSE_BUTTON_4_DOWN == (usButtonFlags & RI_MOUSE_BUTTON_4_DOWN));
                        butChangeCnt++;
                    }

                    if (usButtonFlags & (RI_MOUSE_BUTTON_5_DOWN | RI_MOUSE_BUTTON_5_UP))
                    {
                        buttons[butChangeCnt] = AFX_XMB2;
                        pressed[butChangeCnt] = (RI_MOUSE_BUTTON_5_DOWN == (usButtonFlags & RI_MOUSE_BUTTON_5_DOWN));
                        butChangeCnt++;
                    }
                }

                if (butChangeCnt)
                    AfxEmulateMouseButtonActions(0, butChangeCnt, buttons, pressed);

                afxReal motion[2] = { (afxReal64)rid->data.mouse.lLastX, (afxReal64)rid->data.mouse.lLastY };
                AfxEmulateMouseMotion(0, motion);

                if (RI_MOUSE_WHEEL == (usButtonFlags & RI_MOUSE_WHEEL))
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
                AfxEmulatePressedKeys(0, 1, &key2, &pressure, NIL, NIL);
            }
        }
    }
    return rslt;
}

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxEmulatePressedKeys(afxNat port, afxNat cnt, afxKey const key[], afxNat8 const pressure[], afxObject ovy, afxObject wid)
{
    afxError err = AFX_ERR_NONE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);
        AfxAssert(cnt);
        AfxAssert(key);
        AfxAssert(pressure);

        for (afxNat i = 0; i < cnt; i++)
        {
            afxEventId evtype = (pressure[i]) ? AFX_EVENT_KBD_PRESSED : AFX_EVENT_KBD_RELEASED;

            hid->prevKeyState[key[i]] = hid->lastKeyState[key[i]];
            hid->lastKeyState[key[i]] = pressure[i];

            afxEvent ev = { 0 };
            ev.id = evtype;
            ev.udd[0] = key[i];
            AfxNotifyObject(hid, &ev);
        }
    }
    return err;
}

_AUX afxBool AfxKeysWereReleased(afxNat port, afxNat cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        afxBool cnt2 = 0;

        for (afxNat i = 0; i < cnt; i++)
            if (hid->prevKeyState[codes[i]] && !(hid->lastKeyState[codes[i]]))
                cnt2++;

        rslt = cnt2;
    }
    return rslt;
}

_AUX afxBool AfxKeyWasReleased(afxNat port, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->prevKeyState[code] && !(hid->lastKeyState[code]));
    }
    return rslt;
}

_AUX afxBool AfxKeysWerePressed(afxNat port, afxNat cnt, afxKey const codes[])
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        afxBool cnt2 = 0;

        for (afxNat i = 0; i < cnt; i++)
            if (hid->lastKeyState[codes[i]] && !(hid->prevKeyState[codes[i]]))
                cnt2++;

        rslt = cnt2;
    }
    return rslt;
}

_AUX afxBool AfxKeyWasPressed(afxNat port, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->lastKeyState[code] && !(hid->prevKeyState[code]));
    }
    return rslt;
}

_AUX afxBool AfxKeyIsPressed(afxNat port, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);
        rslt = !!hid->lastKeyState[code];
    }
    return rslt;
}

_AUX afxReal AfxGetCombinedKeyPressure(afxNat port, afxKey lhs, afxKey rhs)
{
    afxError err = AFX_ERR_NONE;
    afxReal pressure = 0.0;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);
        pressure = -AfxRealFromByte(hid->lastKeyState[lhs]);
        pressure += AfxRealFromByte(hid->lastKeyState[rhs]);
    }
    return pressure;
}

_AUX afxReal AfxGetKeyPressure(afxNat port, afxKey code)
{
    afxError err = AFX_ERR_NONE;
    afxReal pressure = 0.0;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        pressure += AfxRealFromByte(hid->lastKeyState[code]);
    }
    return pressure;
}

_AUX afxResult AfxCountPressedKeys(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxResult c = 0;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        afxResult cc = c;

        for (; cc < afxKey_TOTAL; ++cc)
        {
            c += !!hid->lastKeyState[cc];
        }
    }
    return (c);
}

////////////////////////////////////////////////////////////////////////////////

_AUX void AfxGetMouseMotion(afxNat port, afxReal motion[2])
{
    afxError err = AFX_ERR_NONE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        motion[0] = hid->lastMotion[0];
        motion[1] = hid->lastMotion[1];
    }
}

_AUX afxReal AfxGetMouseWheelDelta(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        return hid->lastWheelDelta;
    }
    return 0;
}

_AUX afxBool AfxLmbIsPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->lastMbState[AFX_LMB]);
    }
    return rslt;
}

_AUX afxBool AfxRmbIsPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);
        rslt = (hid->lastMbState[AFX_RMB]);
    }
    return rslt;
}

_AUX afxBool AfxMmbIsPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->lastMbState[AFX_MMB]);
    }
    return rslt;
}

_AUX afxBool AfxXmb1IsPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->lastMbState[AFX_XMB1]);
    }
    return rslt;
}

_AUX afxBool AfxXmb2IsPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->lastMbState[AFX_XMB2]);
    }
    return rslt;
}

_AUX afxBool AfxLmbWasPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->lastMbState[AFX_LMB] && !(hid->prevMbState[AFX_LMB]));
    }
    return rslt;
}

_AUX afxBool AfxRmbWasPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->lastMbState[AFX_RMB] && !(hid->prevMbState[AFX_RMB]));
    }
    return rslt;
}

_AUX afxBool AfxMmbWasPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->lastMbState[AFX_MMB] && !(hid->prevMbState[AFX_MMB]));
    }
    return rslt;
}

_AUX afxBool AfxXmb1WasPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->lastMbState[AFX_XMB1] && !(hid->prevMbState[AFX_XMB1]));
    }
    return rslt;
}

_AUX afxBool AfxXmb2WasPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->lastMbState[AFX_XMB2] && !(hid->prevMbState[AFX_XMB2]));
    }
    return rslt;
}

_AUX afxBool AfxLmbWasReleased(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->prevMbState[AFX_LMB] && !(hid->lastMbState[AFX_LMB]));
    }
    return rslt;
}

_AUX afxBool AfxRmbWasReleased(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->prevMbState[AFX_RMB] && !(hid->lastMbState[AFX_RMB]));
    }
    return rslt;
}

_AUX afxBool AfxMmbWasReleased(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->prevMbState[AFX_MMB] && !(hid->lastMbState[AFX_MMB]));
    }
    return rslt;
}

_AUX afxBool AfxXmb1WasReleased(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->prevMbState[AFX_XMB1] && !(hid->lastMbState[AFX_XMB1]));
    }
    return rslt;
}

_AUX afxBool AfxXmb2WasReleased(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (hid->prevMbState[AFX_XMB2] && !(hid->lastMbState[AFX_XMB2]));
    }
    return rslt;
}

_AUX afxBool AfxMouseHasHorizontalChanged(afxNat port, afxInt tolerance)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (((afxReal)AfxAbs(hid->lastMotion[0] - hid->prevMotion[0])) >= (afxReal)tolerance);
    }
    return rslt;
}

_AUX afxBool AfxMouseHasVerticalChanged(afxNat port, afxInt tolerance)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        rslt = (((afxReal)AfxAbs(hid->lastMotion[1] - hid->prevMotion[1])) >= (afxReal)tolerance);
    }
    return rslt;
}

_AUX afxError AfxEmulateMouseMotion(afxNat port, afxReal const motion[2])
{
    afxError err = AFX_ERR_NONE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        hid->prevMotion[0] = hid->lastMotion[0];
        hid->prevMotion[1] = hid->lastMotion[1];
        hid->lastMotion[0] = motion[0];
        hid->lastMotion[1] = motion[1];

        afxEvent ev = { 0 };
        ev.id = AFX_EVENT_MSE_AXIS_UPDATED;
        ev.udd[0] = hid->lastMotion;
        AfxNotifyObject(hid, &ev);
    }
    return err;
}

_AUX afxError AfxEmulateMouseWheelAction(afxNat port, afxReal delta)
{
    afxError err = AFX_ERR_NONE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);

        hid->prevWheelDelta = hid->lastWheelDelta;
        hid->lastWheelDelta = delta;

        afxEvent ev;
        ev.id = AFX_EVENT_MSE_WHEEL_UPDATED;
        ev.udd[0] = &hid->lastWheelDelta;
        AfxNotifyObject(hid, &ev);
    }
    return err;
}

_AUX afxError AfxEmulateMouseButtonActions(afxNat port, afxNat cnt, afxMouseButton const butt[], afxBool const pressed[])
{
    afxError err = AFX_ERR_NONE;
    afxHid hid;

    if (AfxGetHid(port, &hid))
    {
        AfxAssertObjects(1, &hid, afxFcc_HID);
        AfxAssert(cnt);
        AfxAssert(butt);
        AfxAssert(pressed);

        for (afxNat i = 0; i < cnt; i++)
        {
            afxEventId evtype;

            switch (butt[i])
            {
            case AFX_LMB:
            {
                evtype = (pressed[i]) ? AFX_EVENT_MSE_LMB_PRESSED : AFX_EVENT_MSE_LMB_RELEASED;
                break;
            }
            case AFX_RMB:
            {
                evtype = (pressed[i]) ? AFX_EVENT_MSE_RMB_PRESSED : AFX_EVENT_MSE_RMB_RELEASED;
                break;
            }
            case AFX_MMB:
            {
                evtype = (pressed[i]) ? AFX_EVENT_MSE_MMB_PRESSED : AFX_EVENT_MSE_MMB_RELEASED;
                break;
            }
            case AFX_XMB1:
            {
                evtype = (pressed[i]) ? AFX_EVENT_MSE_XMB1_PRESSED : AFX_EVENT_MSE_XMB1_RELEASED;
                break;
            }
            case AFX_XMB2:
            {
                evtype = (pressed[i]) ? AFX_EVENT_MSE_XMB2_PRESSED : AFX_EVENT_MSE_XMB2_RELEASED;
                break;
            }
            default:
            {
                AfxThrowError();
                evtype = NIL;
                break;
            }
            }

            if (evtype)
            {
                hid->prevMbState[butt[i]] = hid->lastMbState[butt[i]];
                hid->lastMbState[butt[i]] = !!pressed[i];

                afxEvent ev = { 0 };
                ev.id = evtype;
                ev.udd[0] = (void*)&butt[i];
                AfxNotifyObject(hid, &ev);
            }
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AUXINL afxNat AfxGetHidPort(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    return hid->port;
}

_AUXINL afxResult AfxTestHidFlags(afxHid hid, afxHidFlag flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(hid, afxFcc_HID);
    AfxAssert(flags);
    return (hid->flags & flags);
}

_AUX afxError _AfxHidDtor(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &hid, afxFcc_HID);
    return err;
}

_AUX afxResult _AfxHidCtor(afxHid hid, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &hid, afxFcc_HID);

    afxNat const *ports = cookie->udd[0];
    afxNat port = (afxNat)ports[cookie->no];

    hid->port = port;
    hid->flags = NIL;

    // keyboard
    AfxZero(hid->lastKeyState, sizeof(hid->lastKeyState));
    AfxZero(hid->prevKeyState, sizeof(hid->prevKeyState));
    hid->keyCnt = afxKey_TOTAL;
    hid->fnKeyCnt = afxKey_F12 - afxKey_F1;

    // mouse
    AfxZero2(1, sizeof(hid->lastMbState), hid->lastMbState);
    AfxZero2(1, sizeof(hid->prevMbState), hid->prevMbState);
    hid->buttonCnt = AFX_MB_TOTAL;
    hid->lastMotion[0] = 0;
    hid->lastMotion[1] = 0;
    hid->lastWheelDelta = 0;
    hid->prevWheelDelta = 0;
    hid->sampleRate = 1;

    static afxBool hasRidBeenRegistered = FALSE;

    if (hasRidBeenRegistered == FALSE)
    {
        static RAWINPUTDEVICE const rid[] =
        {
            {
                .usUsagePage = HID_USAGE_PAGE_GENERIC, // ((USAGE)0x01)
                .usUsage = HID_USAGE_GENERIC_KEYBOARD, // ((USAGE)0x06)
                .dwFlags = /*RIDEV_NOLEGACY | RIDEV_NOHOTKEYS |*/ RIDEV_DEVNOTIFY/* | RIDEV_APPKEYS*/, // ignores legacy keyboard messages
                .hwndTarget = NIL
            },
            {
                .usUsagePage = HID_USAGE_PAGE_GENERIC, // ((USAGE)0x01)
                .usUsage = HID_USAGE_GENERIC_MOUSE, // ((USAGE) 0x02)
                .dwFlags = /*RIDEV_NOLEGACY |*/ RIDEV_DEVNOTIFY, // ignores legacy mouse messages
                .hwndTarget = NIL
            },
            {
                .usUsagePage = HID_USAGE_PAGE_GENERIC, // ((USAGE)0x01)
                .usUsage = HID_USAGE_GENERIC_GAMEPAD, // ((USAGE) 0x05)
                .dwFlags = RIDEV_DEVNOTIFY,
                .hwndTarget = NIL
            }
        };

        if (!(hasRidBeenRegistered = RegisterRawInputDevices(rid, AFX_COUNTOF(rid), sizeof(rid[0]))))
            AfxThrowError();
    }
    return err;
}

_AUX afxClassConfig const _AfxHidMgrCfg =
{
    .fcc = afxFcc_HID,
    .name = "HID",
    .desc = "Human Input Device Handling",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxHid)),
    .ctor = (void*)_AfxHidCtor,
    .dtor = (void*)_AfxHidDtor
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireHid(afxNat port, afxHid* hid)
{
    afxError err = AFX_ERR_NONE;
    afxHid hid2 = NIL;

    if (AfxGetHid(port, &hid2))
    {
        AfxLogEcho("Reacquiring HID at port %u", port);

        if (AfxReacquireObjects(1, (void*[]) { hid2 }))
            AfxThrowError();
    }
    else
    {
        AfxLogEcho("Acquiring HID at port %u", port);

        afxManager* cls = AfxGetHidManager();
        AfxAssertClass(cls, afxFcc_HID);

        if (AfxAcquireObjects(cls, 1, (afxObject*)&hid2, (void const*[]) { &port }))
            AfxThrowError();
    }

    AfxAssert(hid);
    *hid = hid2;
    return err;
}

_AUX afxBool AfxGetHid(afxNat port, afxHid* hid)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;
    afxNat i = 0;
    afxHid hid2;
    while (AfxEnumerateHids(i, 1, &hid2))
    {
        AfxAssertObjects(1, &hid2, afxFcc_HID);

        if (hid2->port == port)
        {
            found = TRUE;
            break;
        }
        ++i;
    }
    AfxAssert(hid);
    *hid = found ? hid2 : NIL;
    return found;
}

_AUX afxNat AfxInvokeHids(afxNat first, afxNat cnt, afxBool(*f)(afxHid, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetHidManager();
    AfxAssertClass(cls, afxFcc_HID);
    return cnt ? AfxInvokeObjects(cls, first, cnt, (void*)f, udd) : 0;
}

_AUX afxNat AfxEnumerateHids(afxNat first, afxNat cnt, afxHid hids[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(hids);
    afxManager* cls = AfxGetHidManager();
    AfxAssertClass(cls, afxFcc_HID);
    return cnt ? AfxEnumerateObjects(cls, first, cnt, (afxObject*)hids) : 0;
}

_AUX afxNat AfxCountHids(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetHidManager();
    AfxAssertClass(cls, afxFcc_HID);
    return AfxCountObjects(cls);
}
