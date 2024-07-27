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
#include "../src/afx/dev/afxDevCoreBase.h"
#include "dev/AuxOverWin32.h"

_AUX afxKey const vkDereferenceMap[afxKey_TOTAL];
_AUX afxKey const kbdLayoutUss[afxKey_TOTAL];
_AUX afxKey const kbdLayoutAbnt2[afxKey_TOTAL];

afxKey const kbdLayoutUss[afxKey_TOTAL] =
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

afxKey const kbdLayoutAbnt2[afxKey_TOTAL] =
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

_AUX afxKey const _win32VkToQwadro[256] =
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
            ev.udd[0] = (void*)key[i];
            AfxNotifyObject(hid, &ev);
        }
    }
    return err;
}

_AUX afxBool AfxWereKeysReleased(afxNat port, afxNat cnt, afxKey const codes[])
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

_AUX afxBool AfxWasKeyReleased(afxNat port, afxKey code)
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

_AUX afxBool AfxWereKeysPressed(afxNat port, afxNat cnt, afxKey const codes[])
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

_AUX afxBool AfxWasKeyPressed(afxNat port, afxKey code)
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

_AUX afxBool AfxIsKeyPressed(afxNat port, afxKey code)
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

_AUX afxBool AfxIsLmbPressed(afxNat port)
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

_AUX afxBool AfxIsRmbPressed(afxNat port)
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

_AUX afxBool AfxIsMmbPressed(afxNat port)
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

_AUX afxBool AfxIsXmb1Pressed(afxNat port)
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

_AUX afxBool AfxIsXmb2Pressed(afxNat port)
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

_AUX afxBool AfxWasLmbPressed(afxNat port)
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

_AUX afxBool AfxWasRmbPressed(afxNat port)
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

_AUX afxBool AfxWasMmbPressed(afxNat port)
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

_AUX afxBool AfxWasXmb1Pressed(afxNat port)
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

_AUX afxBool AfxWasXmb2Pressed(afxNat port)
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

_AUX afxBool AfxWasLmbReleased(afxNat port)
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

_AUX afxBool AfxWasRmbReleased(afxNat port)
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

_AUX afxBool AfxWasMmbReleased(afxNat port)
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

_AUX afxBool AfxWasXmb1Released(afxNat port)
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

_AUX afxBool AfxWasXmb2Released(afxNat port)
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

_AUX afxError _AuxHidDtorCb(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &hid, afxFcc_HID);
    return err;
}

_AUX afxResult _AuxHidCtorCb(afxHid hid, afxCookie const *cookie)
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

_AUX afxClassConfig const _AuxHidMgrCfg =
{
    .fcc = afxFcc_HID,
    .name = "HID",
    .desc = "Human Input Device Handling",
    .fixedSiz = sizeof(AFX_OBJECT(afxHid)),
    .ctor = (void*)_AuxHidCtorCb,
    .dtor = (void*)_AuxHidDtorCb
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

        afxClass* cls = AfxGetHidClass();
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
    afxClass* cls = AfxGetHidClass();
    AfxAssertClass(cls, afxFcc_HID);
    return cnt ? AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd) : 0;
}

_AUX afxNat AfxEnumerateHids(afxNat first, afxNat cnt, afxHid hids[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(hids);
    afxClass* cls = AfxGetHidClass();
    AfxAssertClass(cls, afxFcc_HID);
    return cnt ? AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)hids) : 0;
}
