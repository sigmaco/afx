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
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_KEYBOARD_H
#define AFX_KEYBOARD_H

#include "afx/core/afxDebug.h"

typedef enum // layout de HID
{
    // NIL
    AFX_KEY_A = 0x04,
    AFX_KEY_B,
    AFX_KEY_C,
    AFX_KEY_D,
    AFX_KEY_E,
    AFX_KEY_F,
    AFX_KEY_G,
    AFX_KEY_H,
    AFX_KEY_I,
    AFX_KEY_J,
    AFX_KEY_K,
    AFX_KEY_L,
    AFX_KEY_M,
    AFX_KEY_N,
    AFX_KEY_O,
    AFX_KEY_P,
    AFX_KEY_Q,
    AFX_KEY_R,
    AFX_KEY_S,
    AFX_KEY_T,
    AFX_KEY_U,
    AFX_KEY_V,
    AFX_KEY_W,
    AFX_KEY_X,
    AFX_KEY_Y,
    AFX_KEY_Z, // 29/0x1D
    
    AFX_KEY_1 = 0x1E, // !
    AFX_KEY_2, // @
    AFX_KEY_3, // #
    AFX_KEY_4, // $
    AFX_KEY_5, // %
    AFX_KEY_6, // �
    AFX_KEY_7, // &
    AFX_KEY_8, // *
    AFX_KEY_9, // (
    AFX_KEY_0, // )

    AFX_KEY_RET = 0x28, // carriage return (aka enter)
    AFX_KEY_ESC, // Escape
    AFX_KEY_BACK, // Backspace
    AFX_KEY_TAB = 0x2B,
    AFX_KEY_SPACE = 0x2C, // Spacebar
    AFX_KEY_MINUS,
    AFX_KEY_EQUAL,
    AFX_KEY_LBRACE, // L Brace
    AFX_KEY_RBRACE, // R Brace
    AFX_KEY_BACKSLASH = 0x31, // backslash --- "\"
    AFX_KEY_TILDE = 0x32, // non-US # and ~
    AFX_KEY_SEMICOL = 0x33, // semicolon --- ; and :
    AFX_KEY_QUOTE, // apostrophe --- ' and "
    AFX_KEY_GRAVE, // grave --- ` and ~
    AFX_KEY_COMMA, // comma --- , and <
    AFX_KEY_PERIOD, // dot --- . and >
    AFX_KEY_SLASH, // forward slash --- /
    AFX_KEY_CAPSLOCK = 0x39,

    AFX_KEY_F1 = 0x3A,
    AFX_KEY_F2,
    AFX_KEY_F3,
    AFX_KEY_F4,
    AFX_KEY_F5,
    AFX_KEY_F6,
    AFX_KEY_F7,
    AFX_KEY_F8,
    AFX_KEY_F9,
    AFX_KEY_F10,
    AFX_KEY_F11,
    AFX_KEY_F12,

    AFX_KEY_PRINT = 0x46, // print/system request
    AFX_KEY_SCROLLOCK, // scroll lock
    AFX_KEY_PAUSE, // pause/break
    AFX_KEY_INSERT, // insert
    AFX_KEY_HOME, // home
    AFX_KEY_PAGEUP, // page up
    AFX_KEY_DELETE, // delete
    AFX_KEY_END, // end
    AFX_KEY_PAGEDOWN, // page down
    
    AFX_KEY_RIGHT = 0x4F, // arrow right
    AFX_KEY_LEFT, // arrow left
    AFX_KEY_DOWN, // arrow down
    AFX_KEY_UP, // arrow up

    AFX_KEY_NUMLOCK = 0x53, // numpad lock
    AFX_KEY_DIV, // numpad /
    AFX_KEY_MUL, // numpad *
    AFX_KEY_SUB, // numpad -
    AFX_KEY_ADD, // numpad +
    AFX_KEY_ENT, // numpad ENTER

    AFX_KEY_NUM1 = 0x59, // numpad END
    AFX_KEY_NUM2, // numpad DOWN
    AFX_KEY_NUM3, // numpad PGDN
    AFX_KEY_NUM4, // numpad LEFT
    AFX_KEY_NUM5,
    AFX_KEY_NUM6, // numpad RIGHT
    AFX_KEY_NUM7, // numpad HOME
    AFX_KEY_NUM8, // numpad UP
    AFX_KEY_NUM9, // numpad PGUP
    AFX_KEY_NUM0, // numpad INS
    AFX_KEY_SEP, // sperator (dot or comma) and numpad DEL
    
    AFX_KEY_BACKSLASH_ALT = 0x64, // non-US \ and |
    AFX_KEY_APP = 0x65, // context/application
    
    AFX_KEY_F13 = 0x68,
    AFX_KEY_F14,
    AFX_KEY_F15,
    AFX_KEY_F16,
    AFX_KEY_F17,
    AFX_KEY_F18,
    AFX_KEY_F19,
    AFX_KEY_F20,
    AFX_KEY_F21,
    AFX_KEY_F22,
    AFX_KEY_F23,
    AFX_KEY_F24,

    AFX_KEY_EXEC = 0x74, // execute
    AFX_KEY_HELP, // help
    AFX_KEY_MENU, // menu
    AFX_KEY_SELECT, // select
    AFX_KEY_STOP, // stop
    AFX_KEY_AGAIN, // again
    AFX_KEY_UNDO, // undo
    AFX_KEY_CUT, // cut
    AFX_KEY_COPY, // copy
    AFX_KEY_PASTE, // paste
    AFX_KEY_FIND, // find
    AFX_KEY_MUTE, // mute
    AFX_KEY_VOLUP, // volume up
    AFX_KEY_VOLDOWN, // volume down

    AFX_KEY_SEP2, // numpad separator alt (inverse of SEP) --- dot or comma

    AFX_KEY_LCTRL = 0xE0, // left control
    AFX_KEY_LSHIFT, // left shift
    AFX_KEY_LALT, // left alt
    AFX_KEY_LMETA, // left meta (Windows key on Win32)
    AFX_KEY_RCTRL, // right control
    AFX_KEY_RSHIFT, // right shift
    AFX_KEY_RALT, // right alt
    AFX_KEY_RMETA, // right meta
    
    AFX_KEY_MEDIA_PLAY = 0xE8, // play/pause
    AFX_KEY_MEDIA_STOPCD,
    AFX_KEY_MEDIA_PREV,
    AFX_KEY_MEDIA_NEXT,
    AFX_KEY_MEDIA_EJECTCD,
    AFX_KEY_MEDIA_VOLUP,
    AFX_KEY_MEDIA_VOLDOWN,
    AFX_KEY_MEDIA_MUTE,
    AFX_KEY_MEDIA_WEB,
    AFX_KEY_MEDIA_BWD,
    AFX_KEY_MEDIA_4WD,
    AFX_KEY_MEDIA_STOP,
    AFX_KEY_MEDIA_FIND,
    AFX_KEY_MEDIA_SCROLLUP,
    AFX_KEY_MEDIA_SCROLLDN,
    AFX_KEY_MEDIA_EDIT,
    AFX_KEY_MEDIA_SLEEP,
    AFX_KEY_MEDIA_COFFEE,
    AFX_KEY_MEDIA_REFRESH,
    AFX_KEY_MEDIA_CALC,

    AFX_KEY_TOTAL = 0x100
} afxKey;

#ifdef _AFX_KEYBOARD_C
AFX_OBJECT(afxKeyboard)
{
    afxNat              port;
    afxFlags            flags;
    afxNat              fnKeyCnt;
    afxNat              keyCnt;
    afxBool             currState[AFX_KEY_TOTAL];
    afxBool             prevState[AFX_KEY_TOTAL];
};
#endif//_AFX_KEYBOARD_C

AFX afxBool     AfxGetKeyboard(afxNat port, afxKeyboard* kbd);

AFX afxError    AfxAcquireKeyboards(afxNat cnt, afxNat const port[], afxKeyboard kbd[]);

AFX afxBool     AfxKeyIsPressed(afxNat port, afxKey code);
AFX afxBool     AfxKeyWasPressed(afxNat port, afxKey code);
AFX afxBool     AfxKeyWasReleased(afxNat port, afxKey code);

AFX afxBool     AfxKeysArePressed(afxNat port, afxNat cnt, afxKey const codes[]);
AFX afxBool     AfxKeysWerePressed(afxNat port, afxNat cnt, afxKey const codes[]);
AFX afxBool     AfxKeysWereReleased(afxNat port, afxNat cnt, afxKey const codes[]);

AFX afxError    AfxEmulateKeysAction(afxNat port, afxNat cnt, afxKey const key[], afxBool const pressed[]);

#endif//AFX_KEYBOARD_H