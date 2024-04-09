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

#ifndef AFX_KEYBOARD_H
#define AFX_KEYBOARD_H

#include "qwadro/ux/afxHid.h"

typedef enum // layout de HID
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
    
    afxKey_1 = 0x1E, // !
    afxKey_2, // @
    afxKey_3, // #
    afxKey_4, // $
    afxKey_5, // %
    afxKey_6, // ¨
    afxKey_7, // &
    afxKey_8, // *
    afxKey_9, // (
    afxKey_0, // )

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
    afxKey_CAPSLOCK = 0x39,

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
    afxKey_SCROLLOCK, // scroll lock
    afxKey_PAUSE, // pause/break
    afxKey_INSERT, // insert
    afxKey_HOME, // home
    afxKey_PAGEUP, // page up
    afxKey_DELETE, // delete
    afxKey_END, // end
    afxKey_PAGEDOWN, // page down
    
    afxKey_RIGHT = 0x4F, // arrow right
    afxKey_LEFT, // arrow left
    afxKey_DOWN, // arrow down
    afxKey_UP, // arrow up

    afxKey_NUMLOCK = 0x53, // numpad lock
    afxKey_DIV, // numpad /
    afxKey_MUL, // numpad *
    afxKey_SUB, // numpad -
    afxKey_ADD, // numpad +
    afxKey_ENT, // numpad ENTER

    afxKey_NUM1 = 0x59, // numpad END
    afxKey_NUM2, // numpad DOWN
    afxKey_NUM3, // numpad PGDN
    afxKey_NUM4, // numpad LEFT
    afxKey_NUM5,
    afxKey_NUM6, // numpad RIGHT
    afxKey_NUM7, // numpad HOME
    afxKey_NUM8, // numpad UP
    afxKey_NUM9, // numpad PGUP
    afxKey_NUM0, // numpad INS
    afxKey_SEP, // sperator (dot or comma) and numpad DEL
    
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
    afxKey_VOLDOWN, // volume down

    afxKey_SEP2, // numpad separator alt (inverse of SEP) --- dot or comma

    afxKey_LCTRL = 0xE0, // left control
    afxKey_LSHIFT, // left shift
    afxKey_LALT, // left alt
    afxKey_LMETA, // left meta (Windows key on Win32)
    afxKey_RCTRL, // right control
    afxKey_RSHIFT, // right shift
    afxKey_RALT, // right alt
    afxKey_RMETA, // right meta
    
    afxKey_MEDIA_PLAY = 0xE8, // play/pause
    afxKey_MEDIA_STOPCD,
    afxKey_MEDIA_PREV,
    afxKey_MEDIA_NEXT,
    afxKey_MEDIA_EJECTCD,
    afxKey_MEDIA_VOLUP,
    afxKey_MEDIA_VOLDOWN,
    afxKey_MEDIA_MUTE,
    afxKey_MEDIA_WEB,
    afxKey_MEDIA_BWD,
    afxKey_MEDIA_4WD,
    afxKey_MEDIA_STOP,
    afxKey_MEDIA_FIND,
    afxKey_MEDIA_SCROLLUP,
    afxKey_MEDIA_SCROLLDN,
    afxKey_MEDIA_EDIT,
    afxKey_MEDIA_SLEEP,
    afxKey_MEDIA_COFFEE,
    afxKey_MEDIA_REFRESH,
    afxKey_MEDIA_CALC,

    afxKey_TOTAL = 0x100
} afxKey;

#ifdef _AFX_CORE_C
#ifdef _AFX_KEYBOARD_C
AFX_OBJECT(afxKeyboard)
{
    AFX_OBJECT(afxHid)  hid;
    afxNat              fnKeyCnt;
    afxNat              keyCnt;
    afxNat8             currState[afxKey_TOTAL];
    afxNat8             prevState[afxKey_TOTAL];
};
#endif//_AFX_KEYBOARD_C
#endif//_AFX_CORE_C

AFX afxReal         AfxGetKeyPressure(afxNat port, afxKey code);
AFX afxReal         AfxGetCombinedKeyPressure(afxNat port, afxKey lhs, afxKey rhs);

AFX afxBool         AfxKeyIsPressed(afxNat port, afxKey code);
AFX afxBool         AfxKeyWasPressed(afxNat port, afxKey code);
AFX afxBool         AfxKeyWasReleased(afxNat port, afxKey code);

AFX afxBool         AfxKeysWerePressed(afxNat port, afxNat cnt, afxKey const codes[]);
AFX afxBool         AfxKeysWereReleased(afxNat port, afxNat cnt, afxKey const codes[]);

AFX afxError        AfxEmulatePressedKeys(afxNat port, afxNat cnt, afxKey const key[], afxNat8 const pressure[]);

////////////////////////////////////////////////////////////////////////////////

AFX afxBool         AfxGetKeyboard(afxNat port, afxKeyboard* keyboard);

AFX afxError        AfxAcquireKeyboard(afxNat port, afxKeyboard* keyboard);

#endif//AFX_KEYBOARD_H
