/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   U S E R   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_IMPL___SHELL_H
#define AUX_IMPL___SHELL_H

#include "../../impl/afxExecImplKit.h"
#include "qwadro/inc/ux/afxUxDefs.h"
#include "qwadro/inc/math/avxViewport.h"
#include "qwadro/inc/ux/afxShell.h"
//#include "qwadro/../../dep_/vgl1/vgl1.h"

typedef struct afxDesktop afxDesktop;

AFX_DEFINE_STRUCT(afxDesktop) // DWM
{
    afxReal64           wpOverHp; // physical w/h
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxReal             refreshRate;
    afxWarp              res; // Screen resolution. Absolute extent available.
    afxSurface       dout;
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

    afxClassConfig      wndClsCfg;
    afxClassConfig      xssClsCfg;

};
#endif//_AUX_SHELL_C

#ifndef _AUX_UX_C
AFX_DECLARE_STRUCT(_auxSesImpl);
#else
AFX_DEFINE_STRUCT(_auxSesImpl)
{
    afxUnit64(*pumpCb)(afxSession,afxFlags, afxUnit64);
    afxBool(*hasClipboardCb)(afxSession, afxFlags);
    afxUnit(*getClipboardCb)(afxSession, afxString*);
    afxError(*setClipboardCb)(afxSession, afxString const*);
    afxBool(*getCurs)(afxSession, afxRect*,afxWindow,afxRect*,afxRect*);
    afxError(*fseCb)(afxSession,afxWindow,afxBool);
    afxError(*focusCb)(afxSession, afxWindow, afxFlags);
    afxError(*promote)(afxSession, afxWindow);
    afxError(*grabCursorCb)(afxSession, afxWindow, afxBool);
};
#endif

#ifdef _AUX_SESSION_C
#ifdef _AUX_SESSION_IMPL
AFX_OBJECT(_auxSession)
#else
AFX_OBJECT(afxSession)
#endif
{
    _auxSesImpl const*  pimpl;
    void*               idd;

    afxChain            classes;
    afxClass            termCls;
    afxClass            wndCls;
    afxClass            xssCls;

    afxWindow           focusedWnd;
    afxWindow           cursCapturedOn;
    afxRect             cursRect;

    struct
    {
        afxUnit         hidNo;

        // keyboard
        afxUnit         fnKeyCnt;
        afxUnit         keyCnt;
        afxUnit8        keyState[2][afxKey_TOTAL];

        // mouse
        afxUnit         buttonCnt;
        afxUnit         sampleRate;
        afxBool         mbState[2][AFX_MB_TOTAL]; // [ last, prev ][i]
        afxReal         wheelDelta[2]; // [ last, prev ]
        afxV2d          motion[2]; // [ last, prev ]
        afxV2d          motionAcc[2]; // motion acceleration

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
    afxUnit             seatCnt;

    void*               vm;
    afxUnit             scriptedEvCnt;
    afxString const*    scriptedEvNames;
    afxStringBase       scriptedEvNameDb;
    afxPool             scriptedEvHandlers;

    //afxUnit              dwmCnt;
    afxDesktop          dwm;

    afxString4096       clipb;

    afxUnit             vduIdx;
    afxDrawSystem       dsys;
    afxUnit             sdevId;
    afxUnit             soutIdx;
    afxMixSystem     msys;
    afxSink      aso;

};
#endif//_AUX_SESSION_C

#ifndef _AUX_UX_C
AFX_DECLARE_STRUCT(_auxWndDdi);
#else
AFX_DEFINE_STRUCT(_auxWndDdi)
{
    afxError(*redrawCb)(afxWindow, afxRect const*);
    afxError(*chIconCb)(afxWindow, avxRaster);
    afxError(*adjustCb)(afxWindow, afxRect const*, afxRect const*);
    void(*focus)(afxWindow);
    afxBool(*hasFocus)(afxWindow);
    afxUnit(*titleCb)(afxWindow);
};
#endif

#ifdef _AUX_WINDOW_C
#ifdef _AUX_WINDOW_IMPL
AFX_OBJECT(_auxWindow)
#else
AFX_OBJECT(afxWindow)
#endif
{
    _auxWndDdi const*pimpl;

    afxDesktop*     dwm;
    afxChain        classes;
    afxClass        widCls;

    afxSurface   dout;
    afxSurface   frameDout;
    afxRect         frameRc;
    afxRect         frameRcMin;
    afxRect         frameRcMax;
    afxRect         areaRc; // aka client area
    afxBool         alwaysOnTop;

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

    avxRaster       icon;
    afxString2048   title;

    afxBool         active;
    afxBool         focused;
    afxBool         decorated;
    afxBool         floating;
    afxBool         resizable;
    afxBool         fullscreen;

    afxClock        startClock;
    afxClock        lastClock;

    afxV2d          cursPos;
    afxV2d          cursMove;
    afxV2d          cursPosNdc;
    afxV2d          cursMoveNdc;
    afxBool         cursConfined; // confined in window
    afxRect         cursConfinRect;
    afxBool         cursHidden; // visually disabled in window surface
    
    afxBool         redrawFrameRequested;
    afxBool         redrawSurfaceRequested;

    afxWidget       hoveredWidg;
    afxV2d          hoveredPoint;
    afxWidget       focusedWidg;
    afxWidget       grabbedWidg;
    afxV2d          grabPoint;

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
    afxUnit             port;
    afxHidFlag          flags;
    void*               idd;
};
#endif//_AUX_HID_C

#ifdef _AUX_KEYBOARD_C
#ifndef _AFX_CONTEXT_C
#   error "Require afxDevLink implementation"
#endif
AFX_OBJECT(afxKeyboard)
{
    AFX_OBJ(afxDevLink) ctx;
    afxUnit             port;
    afxHidFlag          flags;
    void*               idd;

    // keyboard
    afxUnit             fnKeyCnt;
    afxUnit             keyCnt;
    afxUnit8            lastKeyState[afxKey_TOTAL];
    afxUnit8            prevKeyState[afxKey_TOTAL];
};
#endif//_AUX_KEYBOARD_C

#ifdef _AUX_MOUSE_C
#ifndef _AFX_CONTEXT_C
#   error "Require afxDevLink implementation"
#endif
AFX_OBJECT(afxMouse)
{
    AFX_OBJ(afxDevLink) ctx;
    afxUnit             port;
    afxHidFlag          flags;
    void*               idd;

    // mouse
    afxUnit             buttonCnt;
    afxUnit             sampleRate;
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
#   error "Require afxDevLink implementation"
#endif
AFX_OBJECT(afxGamepad)
{
    AFX_OBJ(afxDevLink) ctx;
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
    afxLink parent;
    afxChain children;

    afxTransform local;
    afxM4d iw;

    afxResult(*updateCb)(afxWidget wid, afxReal dt);
    afxError(*renderCb)(afxWidget wid, afxDrawContext dctx);
};
#endif//_AUX_WIDGET_C
#endif//_AUX_UX_C

AUX afxClassConfig const _AuxCtrlStdImplementation;
AUX afxClassConfig const _AuxKbdStdImplementation;
AUX afxClassConfig const _AuxMseStdImplementation;
AUX afxClassConfig const _AUX_SES_CLASS_CONFIG;
AUX _auxSesImpl const _AUX_SES_IMPL;

AUX afxClassConfig const _AUX_WND_CLASS_CONFIG;
AUX _auxWndDdi const _AUX_WND_IMPL;

AUX afxClassConfig const _AUX_WID_CLASS_CONFIG;

AUX afxClass const* AfxGetMouseClass(afxShell ssh);
AUX afxClass const* AfxGetKeyboardClass(afxShell ssh);
AUX afxClass const* AfxGetControllerClass(afxShell ssh);
AUX afxClass const* _AuxGetSessionClass(afxModule icd);

AUX afxClass const* _AuxWndGetWidClass(afxWindow wnd);
AUX afxClass const* _AuxSesGetWndClass(afxSession ses);
AUX afxClass const* _AuxSesGetXssClass(afxSession ses);

#endif//AUX_IMPL___SHELL_H
