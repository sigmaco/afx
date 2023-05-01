/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_KEYBOARD_H
#define AFX_KEYBOARD_H

#include "afx/core/io/afxHid.h"

typedef enum // layout de HID
{
    // NIL
    AFX_KEY_A = 4,
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
    AFX_KEY_Z,
    AFX_KEY_1,
    AFX_KEY_2,
    AFX_KEY_3,
    AFX_KEY_4,
    AFX_KEY_5,
    AFX_KEY_6,
    AFX_KEY_7,
    AFX_KEY_8,
    AFX_KEY_9,
    AFX_KEY_0,
    AFX_KEY_RET = 0x28, // (Carriage) Return/Enter
    AFX_KEY_ESC, // Escape
    AFX_KEY_BACK, // Backspace
    AFX_KEY_TAB,
    AFX_KEY_SPC, // Spacebar
    AFX_KEY_MINUS,
    AFX_KEY_EQUAL,
    AFX_KEY_LBRCKT, // L Bracket
    AFX_KEY_RBRCKT, // R Bracket
    AFX_KEY_BCKSLSH, // Backslash
    AFX_KEY_SEMICOL = 51, // Semicolon
    AFX_KEY_QUOTE,
    AFX_KEY_GRAVE,
    AFX_KEY_COMMA,
    AFX_KEY_PERIOD,
    AFX_KEY_SLASH,
    AFX_KEY_CAPSLCK,
    AFX_KEY_F1,
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
    AFX_KEY_PRINT,
    AFX_KEY_SCRLCK,
    AFX_KEY_PAUSE,
    AFX_KEY_INS,
    AFX_KEY_HOME,
    AFX_KEY_PGUP,
    AFX_KEY_DEL,
    AFX_KEY_END,
    AFX_KEY_PGDN,
    AFX_KEY_RIGHT,
    AFX_KEY_LEFT,
    AFX_KEY_DOWN,
    AFX_KEY_UP,
    AFX_KEY_NUMLCK,
    AFX_KEY_DIV,
    AFX_KEY_MUL,
    AFX_KEY_SUB,
    AFX_KEY_ADD,
    AFX_KEY_ENT,
    AFX_KEY_NUM1, // END
    AFX_KEY_NUM2, // DOWN
    AFX_KEY_NUM3, // PGDN
    AFX_KEY_NUM4, // LEFT
    AFX_KEY_NUM5,
    AFX_KEY_NUM6, // RIGHT
    AFX_KEY_NUM7, // HOME
    AFX_KEY_NUM8, // UP
    AFX_KEY_NUM9, // PGUP
    AFX_KEY_NUM0, // INS
    AFX_KEY_DOT, // DEL
    AFX_KEY_BCKSLSH_ALT, // non-US \ and |
    AFX_KEY_APP,
    AFX_KEY_LCTRL = 224,
    AFX_KEY_LSHFT,
    AFX_KEY_LALT,
    AFX_KEY_LGUI,
    AFX_KEY_RCTRL,
    AFX_KEY_RSHFT,
    AFX_KEY_RALT,
    AFX_KEY_RGUI,

    AFX_KEY_TOTAL = 256
} afxKey;

AFX_DEFINE_HANDLE(afxKeyboard);


AFX_OBJECT(afxKeyboard)
{
    AFX_OBJECT(afxHid)  hid;
#ifdef _AFX_KEYBOARD_C
    afxNat              fnKeyCnt;
    afxNat              keyCnt;
    afxBool             currState[AFX_KEY_TOTAL];
    afxBool             prevState[AFX_KEY_TOTAL];
#endif
};

AFX void*       AfxKeyboardGetSystem(afxKeyboard kbd);

AFX afxBool     AfxKeyboardArePressed(afxKeyboard kbd, afxNat cnt, afxKey codes[]);
AFX afxResult   AfxKeyboardEmulateActions(afxKeyboard kbd, afxNat cnt, afxKey const key[], afxBool const pressed[]);
AFX afxBool     AfxKeyboardIsPressed(afxKeyboard kbd, afxKey code);
AFX afxBool     AfxKeyboardWasPressed(afxKeyboard kbd, afxKey code);
AFX afxBool     AfxKeyboardWasReleased(afxKeyboard kbd, afxKey code);
AFX afxBool     AfxKeyboardWerePressed(afxKeyboard kbd, afxNat cnt, afxKey codes[]);
AFX afxBool     AfxKeyboardWereReleased(afxKeyboard kbd, afxNat cnt, afxKey codes[]);

#endif//AFX_KEYBOARD_H