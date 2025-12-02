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

// This software is part of Advanced User Experiences Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_ENVIRONMENT_DDK_H
#define AUX_ENVIRONMENT_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxShell.h"
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
    afxUnit             seatCnt;
    afxSeatConfig const*seats;

    afxString           username;

    afxUnit             ddevId;
    afxUnit             vduIdx;
    afxDrawSystem       dsys;
    afxUnit             sdevId;
    afxUnit             soutIdx;
    afxMixSystem      msys;

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
    afxBool(*hasClipboardCb)(afxEnvironment, afxUnit slot, afxFlags);
    afxUnit(*getClipboardCb)(afxEnvironment, afxUnit slot, afxFlags flags, afxString*);
    afxError(*setClipboardCb)(afxEnvironment, afxUnit slot, afxFlags flags, afxString const*);
    afxBool(*getCurs)(afxEnvironment, afxRect*,afxWindow,afxRect*,afxRect*);
    afxError(*fseCb)(afxEnvironment,afxWindow,afxBool);
    afxError(*focusCb)(afxEnvironment, afxWindow, afxFlags);
    afxError(*drawBgCb)(afxEnvironment, afxDrawContext, afxFlags);
    afxError(*promote)(afxEnvironment, afxWindow);
    afxError(*grabCursorCb)(afxEnvironment, afxWindow, afxBool);
};
#endif

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
        afxV2d          motionVel[2]; // Ex.: the cursor is moving 500 px / sec to the right.
        afxV2d          motionAcc[2]; // The speed is increasing rapidly; the user is flicking the mouse.
        afxReal         lastMotionTime; // ms

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

AUX afxError _AfxEnvFocusWindowCb(afxEnvironment env, afxWindow wnd, afxFlags flags);
AUX afxError _AfxEnvSetClipboardContentCb(afxEnvironment env, afxUnit slot, afxFlags flags, afxString const* buf);
AUX afxUnit _AfxEnvGetClipboardContentCb(afxEnvironment env, afxUnit slot, afxFlags flags, afxString* buf);
AUX afxBool _AfxEnvHasClipboardContentCb(afxEnvironment env, afxUnit slot, afxFlags flags);
AUX afxUnit64 _AfxEnvPollInputCb(afxEnvironment env, afxFlags flags, afxUnit64 timeout);

AUX afxEnvironment gActiveEnv;

#endif//AUX_ENVIRONMENT_DDK_H
