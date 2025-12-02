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

#ifndef AUX_HID_DDK_H
#define AUX_HID_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxShell.h"
//#include "qwadro/../../dep_/vgl1/vgl1.h"

AFX_DEFINE_STRUCT(afxHidInfo)
{
    afxDeviceInfo   dev;
    afxHidType      type;
    afxHidCaps      caps;
    afxHidLimits    limits;
};

AFX_DECLARE_STRUCT(_auxDdiHid);
AFX_DECLARE_STRUCT(_auxIddHid);

#ifdef _AUX_HID_C
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(afxHid)
{
    AFX_OBJ(afxDevice)  dev;
    _auxIddHid*         idd;
    _auxDdiHid const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxUnit             port;
    afxHidFlag          flags;
};
#endif//_AUX_HID_C

AFX_DECLARE_STRUCT(_auxDdiKbd);
AFX_DECLARE_STRUCT(_auxIddKbd);

#ifdef _AUX_KEYBOARD_C
#ifndef _AFX_CONTEXT_C
#   error "Require afxDevLink implementation"
#endif
AFX_OBJECT(afxKeyboard)
{
    AFX_OBJ(afxDevLink) ctx;
    _auxIddKbd*         idd;
    _auxDdiKbd const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxUnit             port;
    afxHidFlag          flags;

    // keyboard
    afxUnit             fnKeyCnt;
    afxUnit             keyCnt;
    afxUnit8            lastKeyState[afxKey_TOTAL];
    afxUnit8            prevKeyState[afxKey_TOTAL];

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
#endif//_AUX_KEYBOARD_C

AFX_DECLARE_STRUCT(_auxIddPad);
AFX_DECLARE_STRUCT(_auxDdiPad);

#ifdef _AUX_GAMEPAD_C
#ifndef _AFX_CONTEXT_C
#   error "Require afxDevLink implementation"
#endif
AFX_OBJECT(afxGamepad)
{
    AFX_OBJ(afxDevLink) ctx;
    _auxIddHid*         idd;
    _auxDdiHid const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

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

AUX afxClassConfig const _AuxCtrlStdImplementation;
AUX afxClassConfig const _AuxKbdStdImplementation;
AUX afxClassConfig const _AuxMseStdImplementation;

AUX afxClass const* AfxGetMouseClass(afxShell ssh);
AUX afxClass const* AfxGetKeyboardClass(afxShell ssh);
AUX afxClass const* AfxGetControllerClass(afxShell ssh);

AUX afxClassConfig const _AUX_HID_CLASS_CONFIG;

#endif//AUX_HID_DDK_H
