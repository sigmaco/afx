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

#include "../dev/afxExecImplKit.h"
#include "qwadro/inc/ux/afxUxDefs.h"
#include "qwadro/inc/math/afxViewport.h"
#include "qwadro/inc/ux/afxShell.h"

typedef struct afxDesktop afxDesktop;

AFX_DEFINE_STRUCT(afxDesktop) // DWM
{
    afxReal64           wpOverHp; // physical w/h
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxReal             refreshRate;
    afxWhd              res; // Screen resolution. Absolute extent available.
    afxDrawOutput       dout;
};

AFX_DEFINE_STRUCT(afxShellInfo)
{
    afxDeviceInfo           dev;
    afxClassConfig const*   kbdClsCfg;
    afxClassConfig const*   mseClsCfg;
    afxClassConfig const*   padClsCfg;
    afxClassConfig const*   sesClsCfg;
    afxClassConfig const*   widClsCfg;
    afxClassConfig const*   wndClsCfg;
    afxClassConfig const*   xssClsCfg;
};


#ifdef _AUX_UX_C
#ifdef _AUX_SHELL_C
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(afxShell)
{
    AFX_OBJ(afxDevice)  dev;
    afxClass            kbdCls;
    afxClass            mseCls;
    afxClass            padCls;
    afxClass            sesCls;

    afxClassConfig      widClsCfg;
    afxClassConfig      wndClsCfg;
    afxClassConfig      xssClsCfg;

    afxWindow           focusedWnd;
    afxWindow           curCapturedOn;
    afxHid              stdHid;

    afxTime             (*pumpCb)(afxShell);

    void*               vm;
    afxUnit              scriptedEvCnt;
    afxString const*    scriptedEvNames;
    afxStringBase       scriptedEvNameDb;
    afxPool             scriptedEvHandlers;

    //afxUnit              dwmCnt;
    afxDesktop          dwm;

    void*               idd;
};
#endif//_AUX_SHELL_C

#ifdef _AUX_SESSION_C
#ifdef _AUX_SESSION_IMPL
AFX_OBJECT(_auxSession)
#else
AFX_OBJECT(afxSession)
#endif
{
    afxChain            classes;
    afxClass            termCls;
    afxClass            widCls;
    afxClass            wndCls;
    afxClass            xssCls;

    afxWindow           focusedWnd;
    afxWindow           curCapturedOn;
    afxHid              stdHid;

    struct
    {
        afxUnit          hidNo;

        // keyboard
        afxUnit          fnKeyCnt;
        afxUnit          keyCnt;
        afxUnit8         keyState[2][afxKey_TOTAL];

        // mouse
        afxUnit          buttonCnt;
        afxUnit          sampleRate;
        afxBool         mbState[2][AFX_MB_TOTAL]; // [ last, prev ][i]
        afxReal         wheelDelta[2]; // [ last, prev ]
        afxV2d          motion[2]; // [ last, prev ]

        // gamepad
        afxBool         fn[2][4]; // function keys
        afxBool         lsb[2][4]; // left-side buttons [ <, >, v, ^ ]
        afxBool         rsb[2][4]; // right-side buttons [ W, X, Y, Z]
        afxBool         shoulder[2][2]; // [ left, right ] --- L1, R1
        afxReal         throttle[2][2]; // [ left, right ] --- L2, R2
        afxV2d          analogSticks[2][2]; // [ left, right ]

        afxReal         loFreqVib; // The low-frequency rumble motor (usually the left one).
        afxReal         hiFreqVib; // The high-frequency rumble motor (usually the right one).

    }                   seats[2];
    afxUnit              seatCnt;

    void*               vm;
    afxUnit              scriptedEvCnt;
    afxString const*    scriptedEvNames;
    afxStringBase       scriptedEvNameDb;
    afxPool             scriptedEvHandlers;

    //afxUnit              dwmCnt;
    afxDesktop          dwm;

    void*               idd;
};
#endif//_AUX_SESSION_C

#ifdef _AUX_WINDOW_C
#ifdef _AUX_WINDOW_IMPL
AFX_OBJECT(_auxWindow)
#else
AFX_OBJECT(afxWindow)
#endif
{
    afxDesktop*     dwm;
    afxChain        classes;

    afxDrawOutput   dout;
    afxDrawOutput   frameDout;
    afxRect         frameRect;
    afxRect         surfaceRect; // aka client area

    struct// auxFrame
    {
        afxRect     margin; // used to make room for drop shadow
        afxRect     border; // used to place resize control on frame.
        afxRect     padding; // used to align content area

        //afxInt    left, top, right, bottom;
    };
    afxInt          framePos;

    afxUnit          marginL;
    afxUnit          marginT;
    afxUnit          marginR;
    afxUnit          marginB;

    afxString2048   caption;

    afxBool         active;
    afxBool         focused;
    afxBool         decorated;
    afxBool         floating;
    afxBool         resizable;
    afxBool         fullscreen;

    afxClock        startClock;
    afxClock        lastClock;

    afxV2d          cursorPos;
    afxV2d          cursorMove;
    afxV2d          cursorPosNdc;
    afxV2d          cursorMoveNdc;

    afxBool         cursorDisabled; // disabled on surface
    afxBool         cursorConfined; // confined on surface

    afxBool         redrawFrameRequested;
    afxBool         redrawSurfaceRequested;

    afxWidget       hoveredWidg;
    afxV2d          hoveredPoint;
    afxWidget       focusedWidg;
    afxWidget       grabbedWidg;
    afxV2d          grabPoint;

    afxBool         (*moveCb)(afxWindow, afxUnit const[2]);
    afxError        (*redrawCb)(afxWindow, afxRect const*);
    afxError        (*chIconCb)(afxWindow, afxRaster);
    afxError        (*adjustCb)(afxWindow, afxRect const*, afxRect const*);

    void*           udd;
};
#endif//_AUX_WINDOW_C

AFX_DEFINE_STRUCT(afxHidInfo)
{
    afxDeviceInfo   dev;
    afxHidType      type;
    afxHidCaps      caps;
    afxHidLimits    limits;
};

#ifdef _AUX_HID_C
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(afxHid)
{
    AFX_OBJ(afxDevice)  dev;
    afxUnit              port;
    afxHidFlag          flags;
    void*               idd;
};
#endif//_AUX_HID_C

#ifdef _AUX_KEYBOARD_C
#ifndef _AFX_CONTEXT_C
#   error "Require afxContext implementation"
#endif
AFX_OBJECT(afxKeyboard)
{
    AFX_OBJ(afxContext) ctx;
    afxUnit              port;
    afxHidFlag          flags;
    void*               idd;

    // keyboard
    afxUnit              fnKeyCnt;
    afxUnit              keyCnt;
    afxUnit8             lastKeyState[afxKey_TOTAL];
    afxUnit8             prevKeyState[afxKey_TOTAL];
};
#endif//_AUX_KEYBOARD_C

#ifdef _AUX_MOUSE_C
#ifndef _AFX_CONTEXT_C
#   error "Require afxContext implementation"
#endif
AFX_OBJECT(afxMouse)
{
    AFX_OBJ(afxContext) ctx;
    afxUnit              port;
    afxHidFlag          flags;
    void*               idd;

    // mouse
    afxUnit              buttonCnt;
    afxUnit              sampleRate;
    afxBool             lastMbState[AFX_MB_TOTAL];
    afxBool             prevMbState[AFX_MB_TOTAL];
    afxReal             lastWheelDelta;
    afxReal             prevWheelDelta;
    afxV2d              lastMotion;
    afxV2d              prevMotion;
};
#endif//_AUX_MOUSE_C

#ifdef _AUX_GAMEPAD_C
#ifndef _AFX_CONTEXT_C
#   error "Require afxContext implementation"
#endif
AFX_OBJECT(afxGamepad)
{
    AFX_OBJ(afxContext) ctx;
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

#ifdef _AUX_WIDGET_C
#ifdef _AUX_WIDGET_IMPL
AFX_OBJECT(_auxWidget)
#else
AFX_OBJECT(afxWidget)
#endif
{
    afxLinkage                      parent;
    afxChain                        children;

    afxTransform                    local;
    afxM4d                          iw;

    afxResult(*update)(afxWidget wid, afxReal dt);

    afxWidgetImplementation const   *impl;
    afxWidgetImplementationData     *data;

    // common
    afxV2d                          anchor; // -1 ... 0 ... 1, left-center-right, top-middle-bottom

    afxString*                      text; // 2048
    afxString*                      hint; // 2048
    afxByte                         extra[512];
    struct
    {
        afxRaster                   bgTexture;
        afxColor                    color, bgColor;

        afxUnit                      visibility;
        afxUnit                      cursor; // 'arrow', 'pointer' for links

        // padding, marging
        // border length, border radius

        afxReal                     lineHeight;
        afxReal                     letterSpacing;
        afxUnit                      wordBreak;

        struct
        {
            afxString*              face; // 32 // "sans"
            afxReal                 size; // 18.0 (px)
            afxUnit                  style; // normal
            afxUnit                  weight; // 400
        }                           font;

    };
};
#endif//_AUX_WIDGET_C
#endif//_AUX_UX_C

AUX afxClassConfig const _AuxCtrlStdImplementation;
AUX afxClassConfig const _AuxKbdStdImplementation;
AUX afxClassConfig const _AuxMseStdImplementation;
AUX afxClassConfig const _AuxHidStdImplementation;
AUX afxClassConfig const _AuxWndStdImplementation;
AUX afxClassConfig const _AuxSesStdImplementation;
AUX afxClassConfig const _AuxWidStdImplementation;

AUX afxError _AuxRegisterShells(afxModule icd, afxUnit cnt, afxShellInfo const infos[], afxShell shells[]);
AUX afxError _AuxRegisterHids(afxModule icd, afxUnit cnt, afxHidInfo const infos[], afxHid hids[]);

#endif//AUX_BASE_OBJECTS_H
