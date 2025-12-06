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

// This software is part of Advanced User Experiences Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_ENVIRONMENT_DDK_H
#define AUX_ENVIRONMENT_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxEnvironment.h"
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

AFX_DEFINE_STRUCT(_auxEnvAcq)
{
    void*               udd;
    afxString           tag;
    afxString           name;
    afxUnit             seatCnt;
    afxSeatConfig const*seats;

    afxUnit             ddevId;
    afxUnit             vduIdx;
    afxDrawSystem       dsys;
    afxUnit             sdevId;
    afxUnit             soutIdx;
    afxMixSystem        msys;

    afxClassConfig const* wndClsCfg;
    afxClassConfig const* fntClsCfg;
    afxClassConfig const* themClsCfg;
    afxClassConfig const* xssClsCfg;
};

AFX_DECLARE_STRUCT(_auxIddEnv);

#ifndef _AUX_UX_C
AFX_DECLARE_STRUCT(_auxDdiEnv);
#else
AFX_DEFINE_STRUCT(_auxDdiEnv)
{
    afxUnit64(*pumpCb)(afxEnvironment,afxFlags, afxUnit64);
    afxBool(*hasClipboardCb)(afxEnvironment, afxUnit seat, afxUnit slot, afxFlags);
    afxUnit(*getClipboardCb)(afxEnvironment, afxUnit seat, afxUnit slot, afxFlags flags, afxString*);
    afxError(*setClipboardCb)(afxEnvironment, afxUnit seat, afxUnit slot, afxFlags flags, afxString const*);
    afxBool(*getCurs)(afxEnvironment, afxUnit seat, afxRect*,afxWindow,afxRect*,afxRect*);
    afxError(*fseCb)(afxEnvironment,afxWindow,afxBool);
    afxError(*focusCb)(afxEnvironment, afxUnit seat, afxWindow, afxFlags);
    afxError(*drawBgCb)(afxEnvironment, afxDrawContext, afxFlags);
    afxError(*promote)(afxEnvironment, afxWindow);
    afxError(*grabCursorCb)(afxEnvironment, afxWindow, afxBool);
};
#endif

#define _AUX_SEAT_KEY_SWAPS 2
#define _AUX_SEAT_MB_SWAPS 2
#define _AUX_SEAT_GB_SWAPS 2
#define _AUX_SEAT_GB_FN 4
#define _AUX_SEAT_GB_LSB 4
#define _AUX_SEAT_GB_RSB 4
#define _AUX_SEAT_GB_SHOLDERS 2
#define _AUX_SEAT_GB_THROTTLES 2
#define _AUX_SEAT_GB_ANCHORS 2
#define _AUX_SEAT_GB_STICKS 2

AFX_DEFINE_STRUCT(_auxSeatData)
{
    afxString4096   clipb;

    afxUnit         hidNo;

    // keyboard
    afxUnit         fnKeyCnt;
    afxUnit         keyCnt;
    afxUnit8        keyState[_AUX_SEAT_KEY_SWAPS][afxKey_TOTAL];

    // mouse
    afxUnit         buttonCnt;
    afxUnit         sampleRate;
    afxBool8        mbState[_AUX_SEAT_MB_SWAPS][AFX_MB_TOTAL]; // [ last, prev ][i]
    afxReal         wheelDelta[_AUX_SEAT_MB_SWAPS]; // [ last, prev ]
    afxV2d          motion[_AUX_SEAT_MB_SWAPS]; // [ last, prev ]
    afxV2d          motionVel[_AUX_SEAT_MB_SWAPS]; // Ex.: the cursor is moving 500 px / sec to the right.
    afxV2d          motionAcc[_AUX_SEAT_MB_SWAPS]; // The speed is increasing rapidly; the user is flicking the mouse.
    afxReal         lastMotionTime; // ms

    // gamepad
    afxBool8        fn[_AUX_SEAT_GB_SWAPS][_AUX_SEAT_GB_FN]; // function keys
    afxBool8        lsb[_AUX_SEAT_GB_SWAPS][_AUX_SEAT_GB_LSB]; // left-side buttons [ <, >, v, ^ ]
    afxBool8        rsb[_AUX_SEAT_GB_SWAPS][_AUX_SEAT_GB_RSB]; // right-side buttons [ W, X, Y, Z]
    afxBool8        shoulder[_AUX_SEAT_GB_SWAPS][_AUX_SEAT_GB_SHOLDERS]; // [ left, right ] --- LS/L1, RS/R1
    afxReal         throttle[_AUX_SEAT_GB_SWAPS][_AUX_SEAT_GB_THROTTLES]; // [ left, right ] --- LT/L2, RT/R2
    afxReal         anchor[_AUX_SEAT_GB_SWAPS][_AUX_SEAT_GB_ANCHORS]; // [ left, right ] --- LA/L3, RA/R3
    afxV2d          analogSticks[_AUX_SEAT_GB_SWAPS][_AUX_SEAT_GB_STICKS]; // [ left, right ] --- L <O>, R <O>.

    afxReal         loFreqVib; // The low-frequency rumble motor (usually the left one).
    afxReal         hiFreqVib; // The high-frequency rumble motor (usually the right one).

};

#ifdef _AUX_ENVIRONMENT_C
#ifdef _AUX_ENVIRONMENT_IMPL
AFX_OBJECT(_auxEnvironment)
#else
AFX_OBJECT(afxEnvironment)
#endif
{
    _auxIddEnv*         idd;
    _auxDdiEnv const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxString512        name;

    afxChain            classes;
    afxClass            termCls;
    afxClass            wndCls;
    afxClass            fntCls;
    afxClass            themCls;
    afxClass            xssCls;

    afxWindow           focusedWnd;
    afxBool             cursHidden;
    afxWindow           cursCapturedOn;
    afxRect             cursRect;
    afxV2d              cursVel;
    afxV2d              cursAcc;
    afxReal             lastCursUpdTime;

    afxUnit             seatCnt;
    _auxSeatData        seats[AFX_MAX_USERS_PER_ENVIRONMENT];

    void*               vm;
    afxUnit             scriptedEvCnt;
    afxString const*    scriptedEvNames;
    afxStringBase       scriptedEvNameDb;
    afxPool             scriptedEvHandlers;

    //afxUnit              dwmCnt;
    afxDesktop          dwm;

    afxUnit             vduIdx;
    afxDrawSystem       dsys;
    afxUnit             sdevId;
    afxUnit             soutIdx;
    afxMixSystem        msys;
    afxSink             aso;

};
#endif//_AUX_ENVIRONMENT_C

AUX afxClassConfig const _AUX_ENV_CLASS_CONFIG;

AUX _auxDdiEnv const _AUX_DDI_ENV;

AUX afxClass const* _AuxEnvGetWndClass(afxEnvironment env);
AUX afxClass const* _AuxEnvGetFntClass(afxEnvironment env);
AUX afxClass const* _AuxEnvGetThemClass(afxEnvironment env);
AUX afxClass const* _AuxEnvGetXssClass(afxEnvironment env);

AUX afxError _AfxEnvFocusWindowCb(afxEnvironment env, afxUnit seat, afxWindow wnd, afxFlags flags);
AUX afxError _AfxEnvSetClipboardContentCb(afxEnvironment env, afxUnit seat, afxUnit slot, afxFlags flags, afxString const* buf);
AUX afxUnit _AfxEnvGetClipboardContentCb(afxEnvironment env, afxUnit seat, afxUnit slot, afxFlags flags, afxString* buf);
AUX afxBool _AfxEnvHasClipboardContentCb(afxEnvironment env, afxUnit seat, afxUnit slot, afxFlags flags);
AUX afxUnit64 _AfxEnvPollInputCb(afxEnvironment env, afxFlags flags, afxUnit64 timeout);

AUX afxEnvironment gActiveEnv;

#endif//AUX_ENVIRONMENT_DDK_H
