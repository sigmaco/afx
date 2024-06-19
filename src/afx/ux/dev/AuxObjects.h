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
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_BASE_OBJECTS_H
#define AUX_BASE_OBJECTS_H

#include "../src/afx/dev/afxDevCoreBase.h"
#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/math/avxViewport.h"
#include "qwadro/ux/afxShell.h"

typedef struct afxDesktop afxDesktop;

AFX_DEFINE_STRUCT(afxDesktop) // DWM
{
    afxReal64           wpOverHp; // physical w/h
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxReal             refreshRate;
    afxWhd              res; // Screen resolution. Absolute extent available.
    afxDrawOutput       dout;
};

#ifdef _AUX_SHELL_C
#ifdef _AUX_SHELL_IMPL
AFX_OBJECT(_auxShell)
#else
AFX_OBJECT(afxShell)
#endif
{
    afxBool             ready;
    afxClass            wndCls;
    afxWindow           focusedWnd;
    afxWindow           curCapturedOn;
    afxClass            hidCls;
    afxClass            ctrlCls;
    afxClass            xssCls;
    afxHid              stdHid;

    void*               vm;
    afxNat              scriptedEvCnt;
    afxString const*    scriptedEvNames;
    afxStringBase       scriptedEvNameDb;
    afxPool             scriptedEvHandlers;

    //afxNat              dwmCnt;
    afxDesktop          dwm;
};
#endif//_AUX_SHELL_C

#ifdef _AUX_WINDOW_C
#ifdef _AUX_WINDOW_IMPL
AFX_OBJECT(_auxWindow)
#else
AFX_OBJECT(afxWindow)
#endif
{
    afxDesktop*     dwm;
    afxChain        classes;
    afxClass        widCls;

    afxDrawContext  dctx;
    afxDrawOutput   dout;
    afxDrawOutput   frameDout;
    afxRect         frameRect;
    afxRect         surfaceRect; // aka client area

    struct auxFrame
    {
        afxRect     margin; // used to make room for drop shadow
        afxRect     border; // used to place resize control on frame.
        afxRect     padding; // used to align content area

        //afxInt    left, top, right, bottom;
    };
    afxInt          framePos;

    afxNat          marginL;
    afxNat          marginT;
    afxNat          marginR;
    afxNat          marginB;

    afxString2048   caption;

    afxBool         active;
    afxBool         focused;
    afxBool         decorated;
    afxBool         floating;
    afxBool         fullscreen;

    afxClock        startClock;
    afxClock        lastClock;

    afxV2d          cursorPos;
    afxV2d          cursorMove;
    afxV2d          cursorPosNdc;
    afxV2d          cursorMoveNdc;

    afxBool         cursorDisabled;
    afxBool         cursorConfined;

    afxWidget       hoveredWidg;
    afxV2d          hoveredPoint;
    afxWidget       focusedWidg;
    afxWidget       grabbedWidg;
    afxV2d          grabPoint;

    void*           udd;
};
#endif//_AUX_WINDOW_C

#ifdef _AUX_HID_C
AFX_OBJECT(afxHid)
{
    AFX_OBJECT(afxDevice)   dev;
    afxNat                  port;
    afxHidFlag              flags;

    // keyboard
    afxNat                  fnKeyCnt;
    afxNat                  keyCnt;
    afxNat8                 lastKeyState[afxKey_TOTAL];
    afxNat8                 prevKeyState[afxKey_TOTAL];

    // mouse
    afxNat                  buttonCnt;
    afxNat                  sampleRate;
    afxBool                 lastMbState[AFX_MB_TOTAL];
    afxBool                 prevMbState[AFX_MB_TOTAL];
    afxReal                 lastWheelDelta;
    afxReal                 prevWheelDelta;
    afxV2d                  lastMotion;
    afxV2d                  prevMotion;
};
#endif//_AUX_HID_C

#ifdef _AUX_GAMEPAD_C
AFX_OBJECT(afxGamepad)
{
    AFX_OBJECT(afxHid)  hid;
    afxBool             fn[4];
    afxBool             lhb[4];
    afxBool             rhb[4];
    afxReal             trigger[2];
    afxV2d              stickAxis[2];

    afxReal             loFreqVib; // The low-frequency rumble motor (usually the left one).
    afxReal             hiFreqVib; // The high-frequency rumble motor (usually the right one).
    // The two motors should not be the same, as they create different vibration effects.
};
#endif//_AUX_GAMEPAD_C

#ifdef _AUX_CONTROLLER_C
AFX_OBJECT(afxController)
{
    AFX_OBJECT(afxHid)  hid;
    struct
    {
        afxBool         esc, f1, f2, f3, f4;
        afxBool         apos, a1, a2, a3, a4, a5;
        afxBool         tab, q, w, e, r, t;
        afxBool         caps, a, s, d, f, g;
        afxBool         shift, z, x, c, v, b;
        afxBool         ctrl, alt, spc;
        afxBool         lmb, rmb, mmb;
    }                   state[2];

    afxBool             menuOpen;
    afxBool             abstractMenuOpen;


};
#endif//_AUX_CONTROLLER_C



#endif//AUX_BASE_OBJECTS_H
