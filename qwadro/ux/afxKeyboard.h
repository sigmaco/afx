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

// This software is part of Advanced Multimedia UX Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience on Qwadro                                       //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_KEYBOARD_H
#define AUX_KEYBOARD_H

#include "qwadro/ux/afxHid.h"
#include "qwadro/math/afxVector.h"

typedef enum afxKey
// layout de HID
{
    // NIL
    afxKey_A = 0x04,
    afxKey_B,
    afxKey_C,
    afxKey_D,
    afxKey_E,
    afxKey_F,
    afxKey_G,
    afxKey_H,
    afxKey_I,
    afxKey_J,
    afxKey_K,
    afxKey_L,
    afxKey_M,
    afxKey_N,
    afxKey_O,
    afxKey_P,
    afxKey_Q,
    afxKey_R,
    afxKey_S,
    afxKey_T,
    afxKey_U,
    afxKey_V,
    afxKey_W,
    afxKey_X,
    afxKey_Y,
    afxKey_Z, // 29/0x1D

    afxKey_C1 = 0x1E, // !
    afxKey_C2, // @
    afxKey_C3, // #
    afxKey_C4, // $
    afxKey_C5, // %
    afxKey_C6, // ¨
    afxKey_C7, // &
    afxKey_C8, // *
    afxKey_C9, // (
    afxKey_C0, // )

    afxKey_RET = 0x28, // carriage return (aka enter)
    afxKey_ESC, // Escape
    afxKey_BACK, // Backspace
    afxKey_TAB = 0x2B,
    afxKey_SPACE = 0x2C, // Spacebar
    afxKey_MINUS,
    afxKey_EQUAL,
    afxKey_LBRACE, // L Brace
    afxKey_RBRACE, // R Brace
    afxKey_BACKSLASH = 0x31, // backslash --- "\"
    afxKey_TILDE = 0x32, // non-US # and ~
    afxKey_SEMICOL = 0x33, // semicolon --- ; and :
    afxKey_QUOTE, // apostrophe --- ' and "
    afxKey_GRAVE, // grave --- ` and ~
    afxKey_COMMA, // comma --- , and <
    afxKey_PERIOD, // dot --- . and >
    afxKey_SLASH, // forward slash --- /
    afxKey_CLOCK = 0x39, // caps lock

    afxKey_F1 = 0x3A,
    afxKey_F2,
    afxKey_F3,
    afxKey_F4,
    afxKey_F5,
    afxKey_F6,
    afxKey_F7,
    afxKey_F8,
    afxKey_F9,
    afxKey_F10,
    afxKey_F11,
    afxKey_F12,

    afxKey_PRINT = 0x46, // print/system request
    afxKey_SLOCK, // scroll lock
    afxKey_PAUSE, // pause/break

    afxKey_INSERT, // insert
    afxKey_HOME, // home
    afxKey_PAGEUP, // page up
    afxKey_DELETE, // delete
    afxKey_END, // end
    afxKey_PAGEDN, // page down

    afxKey_RIGHT = 0x4F, // arrow right
    afxKey_LEFT, // arrow left
    afxKey_DOWN, // arrow down
    afxKey_UP, // arrow up

    afxKey_NLOCK = 0x53, // numpad lock
    afxKey_NDIV, // numpad /
    afxKey_NMUL, // numpad *
    afxKey_NSUB, // numpad -
    afxKey_NADD, // numpad +
    afxKey_NENT, // numpad ENTER

    afxKey_N1 = 0x59, // numpad END
    afxKey_N2, // numpad DOWN
    afxKey_N3, // numpad PGDN
    afxKey_N4, // numpad LEFT
    afxKey_N5,
    afxKey_N6, // numpad RIGHT
    afxKey_N7, // numpad HOME
    afxKey_N8, // numpad UP
    afxKey_N9, // numpad PGUP
    afxKey_N0, // numpad INS
    afxKey_NSEP, // sperator (dot or comma) and numpad DEL

    afxKey_BACKSLASH_ALT = 0x64, // non-US \ and |
    afxKey_APP = 0x65, // context/application

    afxKey_F13 = 0x68,
    afxKey_F14,
    afxKey_F15,
    afxKey_F16,
    afxKey_F17,
    afxKey_F18,
    afxKey_F19,
    afxKey_F20,
    afxKey_F21,
    afxKey_F22,
    afxKey_F23,
    afxKey_F24,

    afxKey_EXEC = 0x74, // execute
    afxKey_HELP, // help
    afxKey_MENU, // menu
    afxKey_SELECT, // select
    afxKey_STOP, // stop
    afxKey_AGAIN, // again
    afxKey_UNDO, // undo
    afxKey_CUT, // cut
    afxKey_COPY, // copy
    afxKey_PASTE, // paste
    afxKey_FIND, // find
    afxKey_MUTE, // mute
    afxKey_VOLUP, // volume up
    afxKey_VOLDN, // volume down

    afxKey_SEP2, // numpad separator alt (inverse of SEP) --- dot or comma

    afxKey_LCTRL = 0xE0, // left control
    afxKey_LSHIFT, // left shift
    afxKey_LALT, // left alt
    afxKey_LMETA, // left meta (Windows key on Win32)
    afxKey_RCTRL, // right control
    afxKey_RSHIFT, // right shift
    afxKey_RALT, // right alt
    afxKey_RMETA, // right meta

    afxKey_MM_PLAY = 0xE8, // play/pause
    afxKey_MM_STOPCD,
    afxKey_MM_PREV,
    afxKey_MM_NEXT,
    afxKey_MM_EJECT,
    afxKey_MM_VOLUP,
    afxKey_MM_VOLDOWN,
    afxKey_MM_MUTE,
    afxKey_MM_WEB,
    afxKey_MM_BWD,
    afxKey_MM_FWD,
    afxKey_MM_STOP,
    afxKey_MM_FIND,
    afxKey_MM_SCROLLUP,
    afxKey_MM_SCROLLDN,
    afxKey_MM_EDIT,
    afxKey_MM_SLEEP,
    afxKey_MM_COFFEE,
    afxKey_MM_REFRESH,
    afxKey_MM_CALC,

    afxKey_TOTAL = 0x100
} afxKey;

typedef enum afxMouseButton
{
    AFX_LMB,
    AFX_RMB,
    AFX_MMB,
    AFX_XMB1,
    AFX_XMB2,

    AFX_MB_TOTAL
} afxMouseButton;

// KEYBOARD AND KEYPAD

AUX afxReal         AfxGetKeyPressure(afxUnit seat, afxKey code);
AUX afxReal         AfxGetCombinedKeyPressure(afxUnit seat, afxKey lhs, afxKey rhs);

AUX afxBool         AfxIsKeyPressed(afxUnit seat, afxKey code);
AUX afxBool         AfxWasKeyPressed(afxUnit seat, afxKey code);
AUX afxBool         AfxWasKeyReleased(afxUnit seat, afxKey code);

AUX afxBool         AfxWereKeysPressed(afxUnit seat, afxUnit cnt, afxKey const codes[]);
AUX afxBool         AfxWereKeysReleased(afxUnit seat, afxUnit cnt, afxKey const codes[]);

AUX afxError        AfxEmulatePressedKeys(afxUnit seat, afxUnit cnt, afxKey const key[], afxUnit8 const pressure[]);
AUX afxError        AfxReleaseAllKeys(afxUnit seat);


AUX afxError        AfxReleaseAllKeys2(afxMask seats);
AUX afxMask         AfxWereKeysReleased2(afxMask seats, afxUnit cnt, afxKey const codes[]);
AUX afxMask         AfxWasKeyReleased2(afxMask seats, afxKey code);
AUX afxMask         AfxWereKeysPressed2(afxMask seats, afxUnit cnt, afxKey const codes[]);
AUX afxMask         AfxWasKeyPressed2(afxMask seats, afxKey code);
AUX afxMask         AfxIsKeyPressed2(afxMask seats, afxKey code);

// MOUSE AND POINTER

AUX afxBool         AfxIsMousePressed(afxUnit seat, afxMouseButton mb);
AUX afxBool         AfxWasMousePressed(afxUnit seat, afxMouseButton mb);
AUX afxBool         AfxWasMouseReleased(afxUnit seat, afxMouseButton mb);

AUX afxMask         AfxIsMousePressed2(afxMask seats, afxMouseButton mb);
AUX afxMask         AfxWasMousePressed2(afxMask seats, afxMouseButton mb);
AUX afxMask         AfxWasMouseReleased2(afxMask seats, afxMouseButton mb);

AUX void            AfxGetMouseMotion(afxUnit seat, afxV2d motion, afxV2d vel, afxV2d accel);
AUX afxBool         AfxTestMouseMotionX(afxUnit seat, afxInt tolerance);
AUX afxBool         AfxTestMouseMotionZ(afxUnit seat, afxInt tolerance);

AUX afxReal         AfxGetMouseWheelDelta(afxUnit seat);

AUX afxError        AfxEmulateMouseMotion(afxUnit seat, afxV2d const motion);
AUX afxError        AfxEmulateMouseWheelAction(afxUnit seat, afxReal delta);
AUX afxError        AfxEmulateMouseButtonActions(afxUnit seat, afxUnit cnt, afxMouseButton const butt[], afxBool const pressed[]);
AUX afxError        AfxReleaseMouseButtons(afxUnit seat);

#endif//AUX_KEYBOARD_H
