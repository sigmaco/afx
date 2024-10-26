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

#include "qwadro/inc/ux/afxUxDefs.h"
#include "qwadro/inc/exec/afxDevice.h"
#include "qwadro/inc/math/afxVector.h"

// A HID may be a haptic device.

typedef enum afxHidFlag
{
    AFX_HID_FLAG_DUMMY
} afxHidFlag;

typedef enum afxHidType
{
    afxHidType_KEYBOARD, // keyboard or keypad
    afxHidType_MOUSE, // pointer or mouse
    afxHidType_GAMEPAD, // game-specific device
    afxHidType_TOUCHPAD,
} afxHidType;

AFX_DEFINE_STRUCT(afxHidCaps)
{
    afxBool8 mouseCapture;
    afxBool8 mouseHasHorizWheel;
    afxBool8 hasXmbs;
    afxBool8 vibration;
    afxBool8 stereoVibration; // term used by SIGMA to refer to dual motor controllers.
    afxBool8 wireless;
    afxBool8 hasMmi; // Example: has audio headset attached to a gamepad.
    afxBool8 wheel; // has bidirectional wheel. Ex.: the mouse's vertical scroll.
    afxBool8 axes; // has logical axes. It is not a analog stick. Mouses offer at least one.
    afxBool8 analogStick; // has analog sticks.
    afxBool8 throttle; // has accelerator keys.

    afxBool8 triggerButtons; // has side-mounted button. Ex.: R1.
    afxBool8 triggerThrottle; // has side-mounted throttle. Ex.: R2.
    afxBool8 triggerAnalogStick; // has side-mounted analog stick.

    afxBool8 touchArea; // touch points returns touch area information.
    afxBool8 touchPressure; // pressure information is available.
    afxBool8 touchVelocity; // velocity information is available.
    afxBool8 touchPos; // touch points returns position information.
    afxBool8 touchPosNormalized; // normalized position is available.
    afxBool8 mouseEmulation; // the device synthesizes mouse events.
    afxBool8 hasScroll; // has a scroll capability.
    afxBool8 pixelPreciseScroll; // (usually a touchpad) scrolls with pixel precision.
    afxBool8 hover; // has a hover capability.
    afxBool8 rotation; // rotation information is available.
    afxBool8 tiltX; // tilt information is available for the X-axis.
    afxBool8 tiltY; // tilt information is available for the Y-axis.
    afxBool8 tanPressure; // tangential pressure information is available.
    afxBool8 zPos; // position information for the Z-axis is available.
    afxBool8 touchScreen; // has display integrated. This means the surface and display typically have the same size, such that there is a direct relationship between the touch points' physical positions and the coordinate reported by events.    
};

AFX_DEFINE_STRUCT(afxHidLimits)
{
    afxUnit  keyCnt;
    afxUnit  fnKeyCnt;
    afxUnit  indicatorCnt;
    afxUnit  mouseBtnCnt;
    afxUnit  mouseSampleRate;
    afxUnit  wheelCnt; // usually 1 in common mouses; the vertical scroll.
    afxUnit  axisCnt;
    afxUnit  analogStickCnt; // usually 2 in common gamepads.
    afxUnit  throttleCnt; // usually 2 in common gamepads. Ex.: L2 and R2.
};

AUX afxClass const* AfxGetHidClass(void);

// HID DISCOVERY ///////////////////////////////////////////////////////////////

AUX afxUnit         AfxInvokeHids(afxUnit first, afxUnit cnt, afxBool(*f)(afxHid, void*), void *udd);
AUX afxUnit         AfxEvokeHids(afxBool(*flt)(afxHid, void*), void* fdd, afxUnit first, afxUnit cnt, afxHid hids[]);
AUX afxUnit         AfxEnumerateHids(afxUnit first, afxUnit cnt, afxHid hids[]);

AUX afxBool         AfxGetHid(afxUnit port, afxHid* hid);

// HID HANDLING ////////////////////////////////////////////////////////////////

AUX afxBool         AfxIsHidPrompt(afxHid hid);

AUX afxUnit         AfxGetHidPort(afxHid hid);
AUX afxResult       AfxTestHidFlags(afxHid hid, afxHidFlag flags);

AUX void*           AfxGetHidIdd(afxHid hid);

// HID CONTEXTUALIZATION ///////////////////////////////////////////////////////

#endif//AUX_HID_H
