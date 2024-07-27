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

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience on Qwadro                                       //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_HID_H
#define AUX_HID_H

#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/exec/afxDevice.h"
#include "qwadro/math/afxVector.h"

// A HID may be a haptic device.

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

typedef enum afxHidFlag
{
    AFX_HID_FLAG_DUMMY
} afxHidFlag;

AUX afxNat      AfxGetHidPort(afxHid hid);
AUX afxResult   AfxTestHidFlags(afxHid hid, afxHidFlag flags);

// KEYBOARD ////////////////////////////////////////////////////////////////////

AUX afxReal     AfxGetKeyPressure(afxNat port, afxKey code);
AUX afxReal     AfxGetCombinedKeyPressure(afxNat port, afxKey lhs, afxKey rhs);

AUX afxBool     AfxIsKeyPressed(afxNat port, afxKey code);
AUX afxBool     AfxWasKeyPressed(afxNat port, afxKey code);
AUX afxBool     AfxWasKeyReleased(afxNat port, afxKey code);

AUX afxBool     AfxWereKeysPressed(afxNat port, afxNat cnt, afxKey const codes[]);
AUX afxBool     AfxWereKeysReleased(afxNat port, afxNat cnt, afxKey const codes[]);

AUX afxError    AfxEmulatePressedKeys(afxNat port, afxNat cnt, afxKey const key[], afxNat8 const pressure[], afxObject ovy, afxObject wid);

// MOUSE ///////////////////////////////////////////////////////////////////////

AUX afxBool     AfxIsLmbPressed(afxNat port);
AUX afxBool     AfxWasLmbPressed(afxNat port);
AUX afxBool     AfxWasLmbReleased(afxNat port);

AUX afxBool     AfxIsRmbPressed(afxNat port);
AUX afxBool     AfxWasRmbPressed(afxNat port);
AUX afxBool     AfxWasRmbReleased(afxNat port);

AUX afxBool     AfxIsMmbPressed(afxNat port);
AUX afxBool     AfxWasMmbPressed(afxNat port);
AUX afxBool     AfxWasMmbReleased(afxNat port);

AUX afxBool     AfxIsXmb1Pressed(afxNat port);
AUX afxBool     AfxWasXmb1Pressed(afxNat port);
AUX afxBool     AfxWasXmb1Released(afxNat port);

AUX afxBool     AfxIsXmb2Pressed(afxNat port);
AUX afxBool     AfxWasXmb2Pressed(afxNat port);
AUX afxBool     AfxWasXmb2Released(afxNat port);

AUX void        AfxGetMouseMotion(afxNat port, afxReal motion[2]);
AUX afxReal     AfxGetMouseWheelDelta(afxNat port);

AUX afxBool     AfxMouseHasHorizontalChanged(afxNat port, afxInt tolerance);
AUX afxBool     AfxMouseHasVerticalChanged(afxNat port, afxInt tolerance);

AUX afxError    AfxEmulateMouseMotion(afxNat port, afxReal const motion[2]);
AUX afxError    AfxEmulateMouseWheelAction(afxNat port, afxReal delta);
AUX afxError    AfxEmulateMouseButtonActions(afxNat port, afxNat cnt, afxMouseButton const butt[], afxBool const pressed[]);

////////////////////////////////////////////////////////////////////////////////

AUX afxBool     AfxGetHid(afxNat port, afxHid* hid);

AUX afxError    AfxAcquireHid(afxNat port, afxHid* hid);

#endif//AUX_HID_H
