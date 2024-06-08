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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <dwmapi.h>
#include <shellapi.h>
#pragma comment(lib,"dwmapi.lib")

#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_WINDOW_C
#define _AUX_WINDOW_IMPL
#include "qwadro/ux/afxShell.h"

_AUX afxKey const win32VkToQwadro[] =
{
    NIL,
    NIL, // VK_LBUTTON	0x01	Left mouse button
    NIL, // VK_RBUTTON	0x02	Right mouse button
    NIL, // VK_CANCEL	0x03	Control - break processing
    NIL, // VK_MBUTTON	0x04	Middle mouse button
    NIL, // VK_XBUTTON1	0x05	X1 mouse button
    NIL, // VK_XBUTTON2	0x06	X2 mouse button
    NIL, // - 0x07	Reserved
    afxKey_BACK, // VK_BACK	0x08	BACKSPACE key
    afxKey_TAB, // VK_TAB	0x09	TAB key
    NIL, // - 0x0A	Reserved
    NIL, // - 0x0B	Reserved
    NIL, // VK_CLEAR	0x0C	CLEAR key
    afxKey_RET, // VK_RETURN	0x0D	ENTER key
    NIL, //- 0x0E	Unassigned
    NIL, //- 0x0F	Unassigned
    NIL, // VK_SHIFT	0x10	SHIFT key
    NIL, // VK_CONTROL	0x11	CTRL key
    NIL, // VK_MENU	0x12	ALT key
    afxKey_PAUSE, // VK_PAUSE	0x13	PAUSE key
    afxKey_CLOCK, // VK_CAPITAL	0x14	CAPS LOCK key
    NIL, // VK_KANA	0x15	IME Kana mode // VK_HANGUL	0x15	IME Hangul mode
    NIL, // VK_IME_ON	0x16	IME On
    NIL, // VK_JUNJA	0x17	IME Junja mode
    NIL, // VK_FINAL	0x18	IME final mode
    NIL, // VK_HANJA	0x19	IME Hanja mode // VK_KANJI	0x19	IME Kanji mode
    NIL, // VK_IME_OFF	0x1A	IME Off
    afxKey_ESC, // VK_ESCAPE	0x1B	ESC key
    NIL, // VK_CONVERT	0x1C	IME convert
    NIL, // VK_NONCONVERT	0x1D	IME nonconvert
    NIL, // VK_ACCEPT	0x1E	IME accept
    NIL, // VK_MODECHANGE	0x1F	IME mode change request
    afxKey_SPACE, // VK_SPACE	0x20	SPACEBAR
    NIL, // VK_PRIOR	0x21	PAGE UP key
    NIL, // VK_NEXT	0x22	PAGE DOWN key
    afxKey_END, // VK_END	0x23	END key
    afxKey_HOME, // VK_HOME	0x24	HOME key
    afxKey_LEFT, // VK_LEFT	0x25	LEFT ARROW key
    afxKey_UP, // VK_UP	0x26	UP ARROW key
    afxKey_RIGHT, // VK_RIGHT	0x27	RIGHT ARROW key
    afxKey_DOWN, // VK_DOWN	0x28	DOWN ARROW key
    afxKey_SELECT, // VK_SELECT	0x29	SELECT key
    NIL, // VK_PRINT	0x2A	PRINT key
    afxKey_EXEC, // VK_EXECUTE	0x2B	EXECUTE key
    afxKey_PRINT, // VK_SNAPSHOT	0x2C	PRINT SCREEN key
    afxKey_INSERT, // VK_INSERT	0x2D	INS key
    afxKey_DELETE, // VK_DELETE	0x2E	DEL key
    afxKey_HELP, // VK_HELP	0x2F	HELP key

    afxKey_C0, // 0x30	0 key
    afxKey_C1, // 0x31	1 key
    afxKey_C2, // 0x32	2 key
    afxKey_C3, // 0x33	3 key
    afxKey_C4, // 0x34	4 key
    afxKey_C5, // 0x35	5 key
    afxKey_C6, // 0x36	6 key
    afxKey_C7, // 0x37	7 key
    afxKey_C8, // 0x38	8 key
    afxKey_C9, // 0x39	9 key

    NIL, // 0x3A	Undefined
    NIL, // 0x3B	Undefined
    NIL, // 0x3C	Undefined
    NIL, // 0x3D	Undefined
    NIL, // 0x3E	Undefined
    NIL, // 0x3F	Undefined
    NIL, // 0x40	Undefined

    afxKey_A, // 0x41	A key
    afxKey_B, // 0x42	B key
    afxKey_C, // 0x43	C key
    afxKey_D, // 0x44	D key
    afxKey_E, // 0x45	E key
    afxKey_F, // 0x46	F key
    afxKey_G, // 0x47	G key
    afxKey_H, // 0x48	H key
    afxKey_I, // 0x49	I key
    afxKey_J, // 0x4A	J key
    afxKey_K, // 0x4B	K key
    afxKey_L, // 0x4C	L key
    afxKey_M, // 0x4D	M key
    afxKey_N, // 0x4E	N key
    afxKey_O, // 0x4F	O key
    afxKey_P, // 0x50	P key
    afxKey_Q, // 0x51	Q key
    afxKey_R, // 0x52	R key
    afxKey_S, // 0x53	S key
    afxKey_T, // 0x54	T key
    afxKey_U, // 0x55	U key
    afxKey_V, // 0x56	V key
    afxKey_W, // 0x57	W key
    afxKey_X, // 0x58	X key
    afxKey_Y, // 0x59	Y key
    afxKey_Z, // 0x5A	Z key

    afxKey_LMETA, // VK_LWIN	0x5B	Left Windows key
    afxKey_RMETA, // VK_RWIN	0x5C	Right Windows key
    afxKey_APP, // VK_APPS	0x5D	Applications key
    NIL, // - 0x5E	Reserved
    afxKey_MM_SLEEP, // VK_SLEEP	0x5F	Computer Sleep key

    afxKey_N0, // VK_NUMPAD0	0x60	Numeric keypad 0 key
    afxKey_N1, // VK_NUMPAD1	0x61	Numeric keypad 1 key
    afxKey_N2, // VK_NUMPAD2	0x62	Numeric keypad 2 key
    afxKey_N3, // VK_NUMPAD3	0x63	Numeric keypad 3 key
    afxKey_N4, // VK_NUMPAD4	0x64	Numeric keypad 4 key
    afxKey_N5, // VK_NUMPAD5	0x65	Numeric keypad 5 key
    afxKey_N6, // VK_NUMPAD6	0x66	Numeric keypad 6 key
    afxKey_N7, // VK_NUMPAD7	0x67	Numeric keypad 7 key
    afxKey_N8, // VK_NUMPAD8	0x68	Numeric keypad 8 key
    afxKey_N9, // VK_NUMPAD9	0x69	Numeric keypad 9 key

    afxKey_NMUL, // VK_MULTIPLY	0x6A	Multiply key
    afxKey_NADD, // VK_ADD	0x6B	Add key
    afxKey_NSEP, // VK_SEPARATOR	0x6C	Separator key
    afxKey_NSUB, // VK_SUBTRACT	0x6D	Subtract key
    afxKey_SEP2, // VK_DECIMAL	0x6E	Decimal key
    afxKey_NDIV, // VK_DIVIDE	0x6F	Divide key

    afxKey_F1, // VK_F1	0x70	F1 key
    afxKey_F2, // VK_F2	0x71	F2 key
    afxKey_F3, // VK_F3	0x72	F3 key
    afxKey_F4, // VK_F4	0x73	F4 key
    afxKey_F5, // VK_F5	0x74	F5 key
    afxKey_F6, // VK_F6	0x75	F6 key
    afxKey_F7, // VK_F7	0x76	F7 key
    afxKey_F8, // VK_F8	0x77	F8 key
    afxKey_F9, // VK_F9	0x78	F9 key
    afxKey_F10, // VK_F10	0x79	F10 key
    afxKey_F11, // VK_F11	0x7A	F11 key
    afxKey_F12, // VK_F12	0x7B	F12 key
    afxKey_F13, // VK_F13	0x7C	F13 key
    afxKey_F14, // VK_F14	0x7D	F14 key
    afxKey_F15, // VK_F15	0x7E	F15 key
    afxKey_F16, // VK_F16	0x7F	F16 key
    afxKey_F17, // VK_F17	0x80	F17 key
    afxKey_F18, // VK_F18	0x81	F18 key
    afxKey_F19, // VK_F19	0x82	F19 key
    afxKey_F20, // VK_F20	0x83	F20 key
    afxKey_F21, // VK_F21	0x84	F21 key
    afxKey_F22, // VK_F22	0x85	F22 key
    afxKey_F23, // VK_F23	0x86	F23 key
    afxKey_F24, // VK_F24	0x87	F24 key

    NIL, // - 0x88	Reserved
    NIL, // - 0x89	Reserved
    NIL, // - 0x8A	Reserved
    NIL, // - 0x8B	Reserved
    NIL, // - 0x8C	Reserved
    NIL, // - 0x8D	Reserved
    NIL, // - 0x8E	Reserved
    NIL, // - 0x8F	Reserved
    afxKey_NLOCK, // VK_NUMLOCK	0x90	NUM LOCK key
    afxKey_SLOCK, // VK_SCROLL	0x91	SCROLL LOCK key
    NIL, // - 0x92	OEM specific
    NIL, // - 0x93	OEM specific
    NIL, // - 0x94	OEM specific
    NIL, // - 0x95	OEM specific
    NIL, // - 0x96	OEM specific
    NIL, // - 0x97	Unassigned
    NIL, // - 0x98	Unassigned
    NIL, // - 0x99	Unassigned
    NIL, // - 0x9A	Unassigned
    NIL, // - 0x9B	Unassigned
    NIL, // - 0x9C	Unassigned
    NIL, // - 0x9D	Unassigned
    NIL, // - 0x9E	Unassigned
    NIL, // - 0x9F	Unassigned

    afxKey_LSHIFT, // VK_LSHIFT	0xA0	Left SHIFT key
    afxKey_RSHIFT, // VK_RSHIFT	0xA1	Right SHIFT key
    afxKey_LCTRL, // VK_LCONTROL	0xA2	Left CONTROL key
    afxKey_RCTRL, // VK_RCONTROL	0xA3	Right CONTROL key
    afxKey_LALT, // VK_LMENU	0xA4	Left ALT key
    afxKey_RALT, // VK_RMENU	0xA5	Right ALT key

    afxKey_MM_BWD, // VK_BROWSER_BACK	0xA6	Browser Back key
    afxKey_MM_FWD, // VK_BROWSER_FORWARD	0xA7	Browser Forward key
    afxKey_MM_REFRESH, // VK_BROWSER_REFRESH	0xA8	Browser Refresh key
    NIL, // VK_BROWSER_STOP	0xA9	Browser Stop key
    NIL, // VK_BROWSER_SEARCH	0xAA	Browser Search key
    NIL, // VK_BROWSER_FAVORITES	0xAB	Browser Favorites key
    NIL, // VK_BROWSER_HOME	0xAC	Browser Start and Home key
    afxKey_MUTE, // VK_VOLUME_MUTE	0xAD	Volume Mute key
    afxKey_VOLDN, // VK_VOLUME_DOWN	0xAE	Volume Down key
    afxKey_VOLUP, // VK_VOLUME_UP	0xAF	Volume Up key
    afxKey_MM_NEXT, // VK_MEDIA_NEXT_TRACK	0xB0	Next Track key
    afxKey_MM_PREV, // VK_MEDIA_PREV_TRACK	0xB1	Previous Track key
    afxKey_MM_STOP, // VK_MEDIA_STOP	0xB2	Stop Media key
    afxKey_MM_PLAY, // VK_MEDIA_PLAY_PAUSE	0xB3	Play / Pause Media key
    NIL, // VK_LAUNCH_MAIL	0xB4	Start Mail key
    NIL, // VK_LAUNCH_MEDIA_SELECT	0xB5	Select Media key
    NIL, // VK_LAUNCH_APP1	0xB6	Start Application 1 key
    NIL, // VK_LAUNCH_APP2	0xB7	Start Application 2 key
    NIL, // - 0xB8	Reserved
    NIL, // - 0xB9	Reserved
    NIL, // VK_OEM_1	0xBA	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the; : key
    NIL, // VK_OEM_PLUS	0xBB	For any country / region, the + key
    NIL, // VK_OEM_COMMA	0xBC	For any country / region, the , key
    NIL, // VK_OEM_MINUS	0xBD	For any country / region, the - key
    NIL, // VK_OEM_PERIOD	0xBE	For any country / region, the.key
    NIL, // VK_OEM_2	0xBF	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the / ? key
    NIL, // VK_OEM_3	0xC0	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the `~ key
    NIL, // - 0xC1	Reserved
    NIL, // - 0xC2	Reserved
    NIL, // - 0xC3	Reserved
    NIL, // - 0xC4	Reserved
    NIL, // - 0xC5	Reserved
    NIL, // - 0xC6	Reserved
    NIL, // - 0xC7	Reserved
    NIL, // - 0xC8	Reserved
    NIL, // - 0xC9	Reserved
    NIL, // - 0xCA	Reserved
    NIL, // - 0xCB	Reserved
    NIL, // - 0xCC	Reserved
    NIL, // - 0xCD	Reserved
    NIL, // - 0xCE	Reserved
    NIL, // - 0xCF	Reserved
    NIL, // - 0xD0	Reserved
    NIL, // - 0xD1	Reserved
    NIL, // - 0xD2	Reserved
    NIL, // - 0xD3	Reserved
    NIL, // - 0xD4	Reserved
    NIL, // - 0xD5	Reserved
    NIL, // - 0xD6	Reserved
    NIL, // - 0xD7	Reserved
    NIL, // - 0xD8	Reserved
    NIL, // - 0xD9	Reserved
    NIL, // - 0xDA	Reserved
    NIL, // VK_OEM_4	0xDB	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the[{ key
    NIL, // VK_OEM_5	0xDC	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the \\ | key
    NIL, // VK_OEM_6	0xDD	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the]} key
    NIL, // VK_OEM_7	0xDE	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the '" key
    NIL, // VK_OEM_8	0xDF	Used for miscellaneous characters; it can vary by keyboard.
    NIL, // - 0xE0	Reserved
    NIL, // - 0xE1	OEM specific
    NIL, // VK_OEM_102	0xE2	The <> keys on the US standard keyboard, or the \\ | key on the non - US 102 - key keyboard
    NIL, // - 0xE3 - E4	OEM specific
    NIL, // VK_PROCESSKEY	0xE5	IME PROCESS key
    NIL, // - 0xE6	OEM specific
    NIL, // VK_PACKET	0xE7	Used to pass Unicode characters as if they were keystrokes.The VK_PACKET key is the low word of a 32 - bit Virtual Key value used for non - keyboard input methods.For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
    NIL, // - 0xE8	Unassigned
    NIL, // - 0xE9 - F5	OEM specific
    NIL, // VK_ATTN	0xF6	Attn key
    NIL, // VK_CRSEL	0xF7	CrSel key
    NIL, // VK_EXSEL	0xF8	ExSel key
    NIL, // VK_EREOF	0xF9	Erase EOF key
    NIL, // VK_PLAY	0xFA	Play key
    NIL, // VK_ZOOM	0xFB	Zoom key
    NIL, // VK_NONAME	0xFC	Reserved
    NIL, // VK_PA1	0xFD	PA1 key
    NIL, // VK_OEM_CLEAR	0xFE	Clear key
};

extern afxKey const Scan1MakeToQwadroDereferenceMap[afxKey_TOTAL];

extern afxClassConfig const _AuxWidMgrCfg;

_AUX void CalcWindowValuesW32(HWND window, afxInt* out_extra_width, afxInt32* out_extra_height)
{
    RECT r, c;
    GetWindowRect(window, &r);
    GetClientRect(window, &c);
    *out_extra_width = (r.right - r.left) - (c.right - c.left);
    *out_extra_height = (r.bottom - r.top) - (c.bottom - c.top);
}

_AUX void CalcWindowMarginsW32(HWND window, afxNat* left, afxNat* top, afxNat* right, afxNat* bottom)
{
    RECT r, c;
    GetWindowRect(window, &r);
    GetClientRect(window, &c);
    *left = c.left - r.left;
    *top = c.top - r.top;
    *right = r.right - c.right;
    *bottom = r.bottom - c.bottom;
}

_AUX HICON _AuxCreateWin32Icon(afxTarga const* tga, afxNat xHotspot, afxNat yHotspot, afxBool icon)
// Creates an RGBA icon or cursor
{
    afxError err = NIL;
    HICON handle = NIL;

    BITMAPV5HEADER bi = { 0 };
    bi.bV5Size = sizeof(bi);
    bi.bV5Width = tga->whd[0];
    bi.bV5Height = -tga->whd[1];
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5AlphaMask = 0xff000000; // ARGB
    bi.bV5RedMask   = 0x00ff0000;
    bi.bV5GreenMask = 0x0000ff00;
    bi.bV5BlueMask  = 0x000000ff;

    afxByte* dst = NIL;
    HDC dc = GetDC(NULL);
    HBITMAP color = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&dst, NULL, (DWORD)0);
    ReleaseDC(NULL, dc);

    if (!color) AfxThrowError();
    else
    {
        HBITMAP mask;

        if (!(mask = CreateBitmap(tga->whd[0], tga->whd[1], 1, 1, NULL))) AfxThrowError();
        else
        {
            afxByte const* src = tga->data.src;

            for (afxNat i = 0; i < tga->whd[0] * tga->whd[1]; i++)
            {
                dst[0] = src[2];
                dst[1] = src[1];
                dst[2] = src[0];
                dst[3] = src[3];
                dst += 4;
                src += 4;
            }

            ICONINFO ii = { 0 };
            ii.fIcon = icon;
            ii.xHotspot = xHotspot;
            ii.yHotspot = yHotspot;
            ii.hbmMask = mask;
            ii.hbmColor = color;

            if (!(handle = CreateIconIndirect(&ii)))
                AfxThrowError();

            DeleteObject(mask);
        }
        DeleteObject(color);
    }
    return handle;
}

_AUX LRESULT WINAPI _AuxWndHndlngPrcW32Callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    afxError err = AFX_ERR_NONE;
    afxWindow wnd = (afxWindow)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (wnd)
    {
        AfxAssertObjects(1, &wnd, afxFcc_WND);
        static afxByte buf[4096] = { 0 };
        afxNat bufSiz = sizeof(buf);

        afxNat32 tid = AfxGetTid();
        afxNat32 doutTid = AfxGetObjectTid(wnd);
        AfxAssert(doutTid == tid);
        
        AfxAssert(wnd->w32.hWnd == hWnd);
#if 0
        HDC dc = wnd->w32.hDc;

        if (dc)
        {
            SwapBuffers(dc); // deadlocks all
        }
#endif

        switch (message)
        {
        case WM_INPUT_DEVICE_CHANGE:
        {
            HANDLE hDevice = (void*)lParam;
            AfxAssert(hDevice);
            RID_DEVICE_INFO info = { 0 };
            info.cbSize = sizeof(info);
            UINT cbSize = info.cbSize;
            GetRawInputDeviceInfoA(hDevice, RIDI_DEVICEINFO, &info, &cbSize);

            if (wParam == GIDC_ARRIVAL)
            {
                AfxLogAdvertence("HID %p connected.", hDevice);
            }
            else if (wParam == GIDC_REMOVAL)
            {
                AfxLogAdvertence("HID %p disconnected.", hDevice);
            }
            else
            {
                AfxThrowError();
            }
        }
        case WM_INPUT:
        {
            afxBool foregroundInput = (GET_RAWINPUT_CODE_WPARAM(wParam) == RIM_INPUT); // Only handle foreground events.
            
            //if (foregroundInput) // Only handle foreground events.
            {
                HRAWINPUT hRawInput = (void*)lParam;
                GetRawInputData(hRawInput, RID_INPUT, NIL, &(bufSiz), sizeof(RAWINPUTHEADER));
                
                if (bufSiz > 0) // Ignore empty packets.
                {
                    if ((bufSiz != GetRawInputData(hRawInput, RID_INPUT, buf, &(bufSiz), sizeof(RAWINPUTHEADER)))) AfxThrowError();
                    else
                    {
                        RAWINPUT* rid = (RAWINPUT*)buf;

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
                            {
                                AfxEmulateMouseButtonActions(0, butChangeCnt, buttons, pressed);

                                for (afxNat i = 0; i < butChangeCnt; i++)
                                {
                                    afxUxEventId evtype;

                                    switch (buttons[i])
                                    {
                                    case AFX_LMB:
                                    {
                                        evtype = afxUxEventId_LMB;
                                        break;
                                    }
                                    case AFX_RMB:
                                    {
                                        evtype = afxUxEventId_RMB;
                                        break;
                                    }
                                    case AFX_MMB:
                                    {
                                        evtype = afxUxEventId_MMB;
                                        break;
                                    }
                                    case AFX_XMB1:
                                    {
                                        evtype = afxUxEventId_XMB1;
                                        break;
                                    }
                                    case AFX_XMB2:
                                    {
                                        evtype = afxUxEventId_XMB2;
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
                                        afxUxEvent ev = { 0 };
                                        ev.id = evtype;
                                        ev.wnd = wnd;
                                        ev.hidPortIdx = 0;
                                        //AfxNotifyObject(wnd, (void*)&ev);
                                        AfxEmitEvent(wnd, (void*)&ev);
                                    }
                                }
                            }

                            afxReal motion[2] = { (afxReal64)rid->data.mouse.lLastX, (afxReal64)rid->data.mouse.lLastY };
                            AfxEmulateMouseMotion(0, motion);

                            afxUxEvent ev = { 0 };
                            ev.id = afxUxEventId_AXIS;
                            ev.wnd = wnd;
                            ev.hidPortIdx = 0;
                            //AfxNotifyObject(wnd, (void*)&ev);
                            AfxEmitEvent(wnd, (void*)&ev);

                            if (RI_MOUSE_WHEEL == (usButtonFlags & RI_MOUSE_WHEEL))
                            {
                                afxReal wheel = (afxInt16)rid->data.mouse.usButtonData;
                                AfxEmulateMouseWheelAction(0, wheel);

                                afxUxEvent ev = { 0 };
                                ev.id = afxUxEventId_WHEEL;
                                ev.wnd = wnd;
                                ev.hidPortIdx = 0;
                                //AfxNotifyObject(wnd, (void*)&ev);
                                AfxEmitEvent(wnd, (void*)&ev);
                            }
                        }
                        else if (rid->header.dwType == RIM_TYPEKEYBOARD)
                        {
                            afxKey key2 = win32VkToQwadro[rid->data.keyboard.VKey];
                            //afxKey key = vkDereferenceMap[rid->data.keyboard.VKey];
                            afxNat8 pressure = (RI_KEY_BREAK == (rid->data.keyboard.Flags & RI_KEY_BREAK)) ? 0x00 : 0xFF; //!!(rid->data.keyboard.Message == WM_KEYDOWN || rid->data.keyboard.Message == WM_SYSKEYDOWN);
                            AfxEmulatePressedKeys(0, 1, &key2, &pressure, wnd, NIL);

                            afxUxEvent ev = { 0 };
                            ev.id = afxUxEventId_KEY;
                            ev.hidPortIdx = 0;
                            ev.wnd = wnd;
                            ev.udd = key2;
                            //AfxNotifyObject(wnd, (void*)&ev);
                            AfxEmitEvent(wnd, (void*)&ev);
                        }
                    }
                }
            }
            break;
        }
        case WM_SYSCOMMAND: // Intercept System Commands
        {
            switch (wParam & 0xfff0)
            {
            case SC_SCREENSAVE: // screensaver trying to start
            case SC_MONITORPOWER: // monitor trying to enter powersave mode
            {
                if (!wnd->fullscreen) break;
                else
                {
                    // We are running in full screen mode, so disallow screen saver and screen blanking
                    return 0;
                }
            }case SC_KEYMENU: // user trying to access window menu (using ALT key)
            {
                //if (!wnd->w32.keymenu)
                  //  return 0;

                break;
            }
            }
            break;
        }
        case WM_CLOSE: // Did We Receive A Close Message?
        {
            // Sent as a signal that a window or an application should terminate.
            // wParam = This parameter is not used.
            // lParam = This parameter is not used.
            // If an application processes this message, it should return zero.

            //PostQuitMessage(0); // Send A Quit Message
            ShowWindow(hWnd, SW_MINIMIZE);
            return 0; // handled. Also prevent process at end of this function
        }
        case WM_ACTIVATE: // Watch For Window Activate Message
        {
            if (!HIWORD(wParam)) // Check Minimization State
            {
                wnd->active = TRUE; // Program Is Active
            }
            else
            {
                wnd->active = FALSE; // Program Is No Longer Active
            }
            return 0; // Return To The Message Loop
        }
        case WM_SHOWWINDOW:
        {
            //UpdateWindow(dout->idd->wnd);
            break;
        }
        case WM_SIZE:
        {
            //if (doutD->resizable)
            {
                /*
                    WM_SIZE
                        Sent to a window after its size has changed.

                    wParam --- The type of resizing requested. This parameter canv be one of the following values.
                        SIZE_MAXHIDE = 4 --- Message is sent to mem pop-up windows when some other window is maximized.
                        SIZE_MAXIMIZED = 2 --- The window has been maximized.
                        SIZE_MAXSHOW = 3 --- Message is sent to mem pop-up windows when some other window has been restored to its former size.
                        SIZE_MINIMIZED = 1 --- The window has been minimized.
                        SIZE_RESTORED = 0 --- The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.

                    lParam
                        The low-order word of lParam specifies the new width of the client area.
                        The high-order word of lParam specifies the new height of the client area.

                */

                afxWhd whdNew = { LOWORD(lParam), HIWORD(lParam), 1 }; // já é client area, filha da puta.

                afxRect cr = { 0 };
                cr.w = LOWORD(lParam);
                cr.h = HIWORD(lParam);

                if (cr.w * cr.h) // don't set to zero
                {
                    AfxAdjustWindowSurface(wnd, &cr);
                }
            }
            //AfxDrawOutputProcess(dout);
            break;
        }
        case WM_DISPLAYCHANGE: // The WM_DISPLAYCHANGE message is sent to all windows when the display resolution has changed.
        {
            HDC dc;
            afxDrawOutput dout = wnd->dout;
            AfxGetDrawOutputIdd(dout, 0, &dc);
            afxWhd const resolution = { GetDeviceCaps(dc, HORZRES), GetDeviceCaps(dc, VERTRES), GetDeviceCaps(dc, PLANES) };
            afxReal64 physAspRatio = AfxFindPhysicalAspectRatio(GetDeviceCaps(dc, HORZSIZE), GetDeviceCaps(dc, VERTSIZE));
            afxNat refreshRate = GetDeviceCaps(dc, VREFRESH);
            AfxResetDrawOutputResolution(dout, resolution, refreshRate, physAspRatio);

            afxDesktop* dwm = wnd->dwm;
            dwm->wpOverHp = physAspRatio;
            dwm->refreshRate = refreshRate;
            dwm->res[0] = resolution[0];
            dwm->res[1] = resolution[1];
            dwm->res[2] = resolution[2];
            dwm->wrOverHr = (afxReal64)dwm->res[0] / (afxReal64)dwm->res[1];
            
            break;
        }
        case WM_STYLECHANGED: // Sent to a window after the SetWindowLong function has changed one or more of the window's styles.
        {
            // Sent to a window after the SetWindowLong function has changed one or more of the window's styles. A window receives this message through its WindowProc function.
            // wParam = Indicates whether the window's styles or extended window styles have changed. This parameter can be one or more of the following values.
            
            afxNat mleft, mtop, mright, mbottom;
            CalcWindowMarginsW32(wnd->w32.hWnd, &mleft, &mtop, &mright, &mbottom);
            wnd->marginL = mleft;
            wnd->marginT = mtop;
            wnd->marginR = mright;
            wnd->marginB = mbottom;

            break;
        }
        case WM_MOUSEMOVE:
        {
            // TODO build a kind of handler for draw input to deal with it.

            POINTS points = MAKEPOINTS(lParam);

            afxV2d curr = { AfxScalar(points.x), AfxScalar(points.y) };

            AfxSubV2d(wnd->cursorMove, wnd->cursorPos, curr);
            AfxCopyV2d(wnd->cursorPos, curr);

            afxV2d screen = { AfxScalar(wnd->frameRect.w), AfxScalar(wnd->frameRect.h) };

            AfxNdcV2d(wnd->cursorPosNdc, wnd->cursorPos, screen);
            AfxNdcV2d(wnd->cursorMoveNdc, wnd->cursorMove, screen);

            //data2->breake = TRUE;
            break;
        }
#if 0
        case WM_DROPFILES: // Sent when the user drops a file on the window of an application that has registered itself as a recipient of dropped files.
        {
            // TODO build a kind of handler for draw input to deal with it.

            afxFileDrop fdrop;
            AFX_ZERO(&fdrop);

            POINT ppt;
            HDROP hDrop = (HDROP)wParam;
            DragQueryPoint(hDrop, &ppt);

            fdrop.x = ppt.x;
            fdrop.y = ppt.y;
            afxNat cnt = DragQueryFileA(hDrop, 0xFFFFFFFF, NIL, NIL);

            AfxAllocateArray(&fdrop.files, sizeof(afxChar const*), 0, AfxHere());

            afxChar* name = NIL;
            afxNat len = 0;
            afxNat i;

            afxDrawContext dctx;
            AfxGetDrawOutputContext(dout, &dctx);
            AfxAssertType(dctxD, afxFcc_DCTX);
            afxMmu mmu = AfxGetDrawContextMmu(dctx);
            AfxAssertObjects(1, &mmu, afxFcc_MMU);

            for (i = 0; i < cnt; i++)
            {
                len = DragQueryFileA(hDrop, i, NIL, 0);

                if (!(name = AfxAllocate(mmu, len + 1, 1, 0, AfxHere()))) AfxLogError("");
                else
                {
                    DragQueryFileA(hDrop, i, name, len + 1);
                    afxNat arrIdx;
                    void *arrel = AfxInsertArrayUnit(&fdrop.files, &arrIdx);
                    AfxCopy2(1, sizeof(name), name, arrel);
                }
            }

            //afxEvent ev;
            //AfxEventDeploy(&ev, AFX_EVENT_DOUT_DRAGNDROP, &doutD->obj, &fdrop);
            //AfxNotifyObject(&doutD->obj, &ev);

            for (i = 0; i < AfxCountArrayElements(&fdrop.files); i++)
            {
                AfxLogEcho("%s", *(afxChar const**)AfxGetArrayUnit(&fdrop.files, i));
            }

            for (i = 0; i < AfxCountArrayElements(&fdrop.files); i++)
            {
                AfxDeallocate(mmu, *(afxChar**)AfxGetArrayUnit(&fdrop.files, i));
            }

            AfxDeallocateArray(&fdrop.files);

            DragFinish(hDrop);
            //data2->breake = TRUE;
            //return 0;
            break;
        }
#endif
        case WM_KILLFOCUS: // Sent to a window immediately before it loses the keyboard focus.
        {
            if (wnd->cursorDisabled)
                0;//AfxEnableCursor(wnd);
            else if (wnd->cursorConfined)
                AfxLiberateCursor(wnd);

            wnd->focused = FALSE;
            break;
        }
        case WM_SETFOCUS: // Sent to a window after it has gained the keyboard focus.
        {
            wnd->focused = TRUE;

            if (!AfxCursorIsOnSurface(wnd)) break; // Don't handle frame interaction; just handle cursor in surface.
            else
            {
                if (wnd->cursorDisabled)
                    0;//AfxDisableCursor(wnd);
                else if (wnd->cursorConfined)
                    AfxConfineCursor(wnd);

                return 0;
            }
        }
        case WM_PAINT:
        {
            ValidateRect(hWnd, NULL);
            //SetWindowTextA(wnd->wnd, AfxGetStringData(&wnd->caption.str, 0));
            return 0; // An application returns zero if it processes this message.
        }
        case WM_ERASEBKGND:
        {
            // Flicker is usually caused by interference via WM_ERASEBKGND. 
            // If you haven't already, intercept WM_ERASEBKGND and do nothing in the regions where you are displaying OpenGL content.            
            ValidateRect(hWnd, NULL);
            return 1; // An application should return nonzero if it erases the background; otherwise, it should return zero.
        }
        default: break;
        }
    }
    return DefWindowProcA(hWnd, message, wParam, lParam);
}

_AUX BOOL CALLBACK FindShellWorkerWindowW32(HWND hwnd, LPARAM lParam)
{
    HWND* found = (HWND*)lParam;

    if (FindWindowExA(hwnd, NULL, "SHELLDLL_DefView", NULL))
        *found = FindWindowExA(NULL, hwnd, "WorkerW", NULL);

    return TRUE;
}

_AUX HWND FindShellBackgroundWindowW32(void)
{
    HWND hwnd = 0;
#ifdef AFX_OS_WIN10
    // Windows 10 Method

    SendMessageTimeoutA(FindWindowA("ProgMan", NULL), 0x052C, 0, 0, SMTO_NORMAL, 1000, NULL);
    hwnd = 0;
    EnumWindows(FindShellWorkerWindowW32, (LPARAM)&(hwnd));
#else
    // Windows 7 Method
    HWND p = FindWindowA("ProgMan", NULL);
    HWND s = FindWindowExA(p, NULL, "SHELLDLL_DefView", NULL);
    hwnd = FindWindowExA(s, NULL, "SysListView32", NULL);
#endif
    return hwnd;
}

_AUX afxError AfxChangeWindowIcon(afxWindow wnd, afxTarga* tga)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    HICON hIcon = NIL;

    if (tga)
    {
        if (!(hIcon = _AuxCreateWin32Icon(tga, 0, 0, TRUE))) AfxThrowError();
        else
        {
            HWND hWnd = wnd->w32.hWnd;
            //Change both icons to the same icon handle.
            SendMessageA(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessageA(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            //This will ensure that the application icon gets changed too.
            SendMessageA(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessageA(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        }
    }

    if (!err)
    {
        if (wnd->w32.hIcon)
            DestroyIcon(wnd->w32.hIcon);

        wnd->w32.hIcon = hIcon;
    }
    return err;
}

_AUX afxError AfxLoadWindowIcon(afxWindow wnd, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(uri);

    AfxThrowError();

    return err;
}

_AUX afxBool DoutNotifyOvy(afxWindow wnd, afxNat bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    wnd->w32.lastBufIdx = bufIdx;
    wnd->w32.swap = 1;
    return 1;
}

_AUX afxError AfxRedrawWindow(afxWindow wnd, afxRect const* surr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    
    HDC dc = wnd->w32.hSurfaceDc;
    
    if (dc)
    {
        if (wnd->w32.swap)
        {
            wnd->w32.swap = FALSE;
            //SwapBuffers(dc);
            DwmFlush();
        }
    }
    return err;
}

_AUX afxManager* AfxGetWidgetClass(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxManager *cls = &wnd->widMgr;
    AfxAssertClass(cls, afxFcc_WID);
    return cls;
}

_AUX afxBool AfxGetSurfaceDrawOutput(afxWindow wnd, afxDrawOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxDrawOutput dout = wnd->dout;
    AfxTryAssertObjects(1, &dout, afxFcc_DOUT);
    *output = dout;
    return !!dout;
}

_AUX afxBool AfxGetWindowDrawOutput(afxWindow wnd, afxDrawOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxDrawOutput dout = wnd->frameDout;
    AfxTryAssertObjects(1, &dout, afxFcc_DOUT);
    *output = dout;
    return !!dout;
}

_AUX void AfxStepWindow(afxWindow wnd, afxReal64* ct, afxReal64* dt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxClock currClock;
    AfxGetClock(&currClock);
    AfxAssert(ct);
    *ct = AfxGetSecondsElapsed(&wnd->startClock, &currClock);
    AfxAssert(dt);
    *dt = AfxGetSecondsElapsed(&wnd->lastClock, &currClock);
    wnd->lastClock = currClock;
}

_AUX afxNat AfxFormatWindowCaption(afxWindow wnd, afxChar const* fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(fmt);
    va_list va;
    va_start(va, fmt);
    afxNat len = AfxFormatStringArg(&wnd->caption.str, fmt, va);
    SetWindowTextA(wnd->w32.hWnd, AfxGetStringData(&wnd->caption.str.str, 0));
    va_end(va);
    return len;
}

_AUX void AfxGetWindowRect(afxWindow wnd, afxRect* frame)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(frame);
    *frame = wnd->frameRect;
}

_AUX void AfxGetSurfaceRect(afxWindow wnd, afxRect* surface)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(surface);
    *surface = wnd->surfaceRect;
}

_AUX afxBool AfxTraceScreenToSurface(afxWindow wnd, afxNat const screenPos[2], afxNat surfPos[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(screenPos);
    POINT point = { .x = screenPos[0],.y = screenPos[1] };
    afxBool rslt = !!ScreenToClient(wnd->w32.hWnd, &point);
    AfxAssert(surfPos);
    surfPos[0] = point.x;
    surfPos[1] = point.y;
    return rslt;
}

_AUX afxBool AfxTraceSurfaceToScreen(afxWindow wnd, afxNat const surfPos[2], afxNat screenPos[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(surfPos);
    POINT point = { .x = surfPos[0], .y = surfPos[1] };
    afxBool rslt = !!ClientToScreen(wnd->w32.hWnd, &point);
    AfxAssert(screenPos);
    screenPos[0] = point.x;
    screenPos[1] = point.y;
    return rslt;
}

_AUX afxBool AfxMoveWindow(afxWindow wnd, afxNat const pos[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    return !!SetWindowPos(wnd->w32.hWnd, NIL, pos[0], pos[1], 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}

_AUX afxError AfxAdjustWindow(afxWindow wnd, afxRect const* frame)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    afxDesktop* dwm = wnd->dwm;
    AfxAssert3(frame, frame->w, frame->h);

    afxRect rc2;
    rc2.x = AfxMinu(frame->x, dwm->res[0] - 1);
    rc2.y = AfxMinu(frame->y, dwm->res[1] - 1);
    rc2.w = AfxMax(1, AfxMin(frame->w, dwm->res[0]));
    rc2.h = AfxMax(1, AfxMin(frame->h, dwm->res[1]));

    if ((wnd->frameRect.x != rc2.x) ||
        (wnd->frameRect.y != rc2.y) ||
        (wnd->frameRect.w != rc2.w) ||
        (wnd->frameRect.h != rc2.h))
    {
        afxInt32 extraWndWidth, extraWndHeight;
        CalcWindowValuesW32(wnd->w32.hWnd, &extraWndWidth, &extraWndHeight);

        AfxAssert2(rc2.w, rc2.h);
        wnd->frameRect = rc2;

        wnd->surfaceRect.x = wnd->marginL;
        wnd->surfaceRect.y = wnd->marginT;
        wnd->surfaceRect.w = AfxMin(AfxMax(1, rc2.w - wnd->marginR - wnd->marginL), wnd->frameRect.w);
        wnd->surfaceRect.h = AfxMin(AfxMax(1, rc2.h - wnd->marginB - wnd->marginT), wnd->frameRect.h);

        if (!SetWindowPos(wnd->w32.hWnd, NULL, 0, 0, wnd->frameRect.w, wnd->frameRect.h, SWP_NOZORDER))
            AfxThrowError();

        afxWhd whd;
        afxDrawOutput dout = wnd->dout;
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        AfxGetDrawOutputExtent(dout, whd);
        whd[0] = wnd->surfaceRect.w;
        whd[1] = wnd->surfaceRect.h;

        if (AfxAdjustDrawOutput(dout, whd))
            AfxThrowError();
    }
    return err;
}

_AUX afxError AfxAdjustWindowFromNdc(afxWindow wnd, afxV2d const offset, afxV2d const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(extent);

    afxDesktop* dwm = wnd->dwm;
    afxRect rc = { 0 };

    if (offset)
    {
        rc.x = (afxNat)AfxUnndcf(offset[0], dwm->res[0] - 1);
        rc.y = (afxNat)AfxUnndcf(offset[0], dwm->res[1] - 1);
    }

    AfxAssert(extent);
    rc.w = (afxNat)AfxUnndcf(extent[0], dwm->res[0]);
    rc.h = (afxNat)AfxUnndcf(extent[1], dwm->res[1]);

    if (AfxAdjustWindow(wnd, &rc))
        AfxThrowError();

    return err;
}

_AUX afxError AfxAdjustWindowSurface(afxWindow wnd, afxRect const* area)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    
    AfxAssert(area);
    AfxAssert2(wnd->frameRect.w > (afxNat)area->x, wnd->frameRect.h > (afxNat)area->y);
    //AfxAssert4(area->w, wnd->frameRect.w > (afxNat)area->w, area->h, wnd->frameRect.h > (afxNat)area->h);

    afxRect rc2;    
    rc2.x = AfxMinu(area->x, wnd->frameRect.w - 1);
    rc2.y = AfxMinu(area->y, wnd->frameRect.h - 1);
    rc2.w = AfxMax(1, area->w/*AfxMin(area->w, wnd->frameRect.w)*/);
    rc2.h = AfxMax(1, area->h/*AfxMin(area->h, wnd->frameRect.h)*/);

    if ((wnd->surfaceRect.x != rc2.x) ||
        (wnd->surfaceRect.y != rc2.y) ||
        (wnd->surfaceRect.w != rc2.w) ||
        (wnd->surfaceRect.h != rc2.h))
    {
        afxInt32 extraWndWidth, extraWndHeight;
        CalcWindowValuesW32(wnd->w32.hWnd, &extraWndWidth, &extraWndHeight);
        
        AfxAssert2(rc2.w, rc2.h);
        wnd->frameRect.w = rc2.w + extraWndWidth;
        wnd->frameRect.h = rc2.h + extraWndHeight;
        wnd->surfaceRect = rc2;

        if (!SetWindowPos(wnd->w32.hWnd, NULL, 0, 0, wnd->frameRect.w, wnd->frameRect.h, SWP_NOMOVE | SWP_NOZORDER))
            AfxThrowError();

        afxDrawOutput dout = wnd->dout;

        if (dout)
        {
            AfxAssertObjects(1, &dout, afxFcc_DOUT);

            afxWhd whd;
            AfxGetDrawOutputExtent(dout, whd);
            whd[0] = wnd->surfaceRect.w;
            whd[1] = wnd->surfaceRect.h;

            if (AfxAdjustDrawOutput(dout, whd))
                AfxThrowError();
        }
    }
    return err;
}

_AUX afxError AfxAdjustWindowSurfaceFromNdc(afxWindow wnd, afxV2d const offset, afxV2d const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    afxDesktop* dwm = wnd->dwm;
    afxRect rc = { 0 };

    if (offset)
    {
        rc.x = (afxNat)AfxUnndcf(offset[0], wnd->frameRect.w - 1);
        rc.y = (afxNat)AfxUnndcf(offset[0], wnd->frameRect.h - 1);
    }

    AfxAssert(extent);
    rc.w = (afxNat)AfxUnndcf(extent[0], dwm->res[0]);
    rc.h = (afxNat)AfxUnndcf(extent[1], dwm->res[1]);
        
    if (AfxAdjustWindowSurface(wnd, &rc))
        AfxThrowError();

    return err;
}

_AUX afxBool _AuxWndStdEventCb(afxWindow wnd, afxUxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    switch (ev->id)
    {
    case afxUxEventId_KEY:
    {
        if (AfxKeyWasPressed(0, afxKey_PRINT))
        {
            if (wnd->dout)
            {
                afxUri2048 uri;
                AfxMakeUri2048(&uri, NIL);
                AfxFormatUri(&uri.uri, "tmp/ss-wnd%u-%u.tga", AfxGetObjectId((void*)wnd), AfxGetTimer());
                AfxPrintDrawOutputBuffer(wnd->dout, 0, &uri.uri);
            }
        }   
        break;
    }
    }
    return TRUE;
}

_AUX afxError _AuxWndCtorCb(afxWindow wnd, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    afxShell usys = cookie->udd[0];
    afxDesktop* dwm = cookie->udd[1];
    afxDrawContext dctx = cookie->udd[2];
    afxRect const* frameRect = cookie->udd[3];

    AfxAssert(dwm);
    wnd->dwm = dwm;

    wnd->active = FALSE;
    wnd->focused = FALSE;
    AfxZeroV2d(wnd->cursorPos);
    AfxZeroV2d(wnd->cursorMove);
    AfxZeroV2d(wnd->cursorPosNdc);
    AfxZeroV2d(wnd->cursorMoveNdc);

    AfxGetClock(&wnd->startClock);
    wnd->lastClock = wnd->startClock;

    AfxZeroV2d(wnd->grabPoint);
    AfxZeroV2d(wnd->hoveredPoint);

    wnd->grabbedWidg = NIL;
    wnd->hoveredWidg = NIL;
    wnd->focusedWidg = NIL;

    AfxSetUpChain(&wnd->mgrChn, (void*)wnd);

    AfxEstablishManager(&wnd->widMgr, NIL, &wnd->mgrChn, &_AuxWidMgrCfg);

    afxString s;
    AfxMakeString(&s, usys->w32.wndClss.lpszClassName, 0);
    AfxMakeString2048(&wnd->caption, &s);

    afxBool fullscreen = FALSE;
    afxBool decorated = TRUE;
    afxBool resizable = TRUE;
    afxBool floating = FALSE;

    DWORD dwExStyle = WS_EX_APPWINDOW; // Window Extended Style
    DWORD dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS; // Window Style

    if (fullscreen)
    {
        dwExStyle |= WS_EX_TOPMOST;
        dwStyle |= WS_POPUP;
        //ShowCursor(FALSE);
    }
    else
    {
        if (floating)
            dwExStyle |= WS_EX_TOPMOST;

        dwStyle |= WS_SYSMENU | WS_MINIMIZEBOX;

        if (!decorated) dwStyle |= WS_POPUP;
        else
        {
            dwStyle |= WS_CAPTION;

            if (resizable)
                dwStyle |= WS_MAXIMIZEBOX | WS_THICKFRAME;
        }
    }

    HWND hWnd = CreateWindowExA(dwExStyle, usys->w32.wndClss.lpszClassName, usys->w32.wndClss.lpszClassName, dwStyle, 0, 0, 1, 1, NIL, NIL, usys->w32.wndClss.hInstance, NIL);

    if (!hWnd) AfxThrowError();
    else
    {
        wnd->w32.hIcon = NIL;
        wnd->w32.hFrameDc = NIL;
        wnd->w32.hSurfaceDc = NIL;
        wnd->w32.hWnd = hWnd;
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)wnd);

        DragAcceptFiles(hWnd, TRUE);
            
        afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxNat ddevId = AfxGetObjectId(ddev);
            
        afxDrawOutput dout;
        afxDrawOutputConfig doutCfg = { 0 };
        doutCfg.pixelFmtDs[0] = afxPixelFormat_D24;
        //doutCfg.pixelFmtDs[0] = afxPixelFormat_D24S8;
        //doutCfg.pixelFmtDs[1] = afxPixelFormat_D24S8;
        doutCfg.bufCnt = 3;
        doutCfg.endpointNotifyObj = wnd;
        doutCfg.endpointNotifyFn = (void*)DoutNotifyOvy;
        doutCfg.w32.hInst = usys->w32.wndClss.hInstance;
        doutCfg.w32.hWnd = hWnd;
        doutCfg.udd[0] = &wnd->w32.hSurfaceDc;

        if (AfxAcquireDrawOutput(ddevId, &doutCfg, &dout)) AfxThrowError();
        else
        {
            wnd->dout = dout;
            wnd->dctx = dctx;
            AfxReconnectDrawOutput(dout, dctx);

            if (frameRect)
                AfxAdjustWindow(wnd, frameRect);
            else
                AfxAdjustWindowFromNdc(wnd, NIL, AfxV2d(0.5, 0.5));
#if 0
            if (presentAlpha)
            {
                DWM_BLURBEHIND bb = { 0 };
                bb.dwFlags = DWM_BB_ENABLE;
                bb.fEnable = TRUE;
                DwmEnableBlurBehindWindow(wnd, &(bb)); // não functiona no chad Windows XP
            }
#endif

            ShowWindow(hWnd, SHOW_OPENWINDOW);
        }

        if (err)
            DestroyWindow(wnd->w32.hWnd);
    }
    return err;
}

_AUX afxError _AuxWndDtorCb(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    AfxCleanUpChainedManagers(&wnd->mgrChn);

    AfxReleaseObjects(1, &wnd->dout);

    AfxChangeWindowIcon(wnd, NIL); // detach any custom icon
    AfxAssert(!wnd->w32.hIcon);

    DragAcceptFiles(wnd->w32.hWnd, FALSE);
    DestroyWindow(wnd->w32.hWnd);
    
    return err;
}

_AUX afxClassConfig const _AuxWndMgrCfg =
{
    .fcc = afxFcc_WND,
    .name = "Window",
    .desc = "Desktop Window",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxWindow)),
    .ctor = (void*)_AuxWndCtorCb,
    .dtor = (void*)_AuxWndDtorCb,
    .event = (void*)_AuxWndStdEventCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireWindow(afxNat dwmId, afxDrawContext dctx, afxRect const* rc, afxWindow* window)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    
    afxShell usys;
    AfxGetShell(&usys);
    AfxAssertObjects(1, &usys, afxFcc_USYS);
    AfxAssertRange(usys->dwmCnt, dwmId, 1);
    afxDesktop* dwm = &usys->dwms[dwmId];

    afxManager* cls = AfxGetWindowManager();
    AfxAssertClass(cls, afxFcc_WND);
    afxWindow wnd;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&wnd, (void const*[]) { usys, dwm, dctx, rc })) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &wnd, afxFcc_WND);
        AfxAssert(window);
        *window = wnd;
    }
    return err;
}
