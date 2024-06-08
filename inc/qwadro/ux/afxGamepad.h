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

#ifndef AUX_GAMEPAD_H
#define AUX_GAMEPAD_H

#include "qwadro/ux/afxHid.h"
#include "qwadro/math/afxVector.h"

#ifdef _AUX_UX_C
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
#endif//_AUX_UX_C

AUX afxBool         AfxTabKeyWasPressed(afxNat port); // Tab
AUX afxBool         AfxQuickKeyWasPressed(afxNat port); // Q
AUX afxBool         AfxForwardKeyWasPressed(afxNat port); // W
AUX afxBool         AfxBackwardKeyWasPressed(afxNat port); // S
AUX afxBool         AfxWestwardKeyWasPressed(afxNat port); // A
AUX afxBool         AfxEastwardKeyWasPressed(afxNat port); // D
AUX afxBool         AfxExecuteKeyWasPressed(afxNat port); // E
AUX afxBool         AfxReuseKeyWasPressed(afxNat port); // R
AUX afxBool         AfxFunctionKeyWasPressed(afxNat port); // F
AUX afxBool         AfxGiveKeyWasPressed(afxNat port); // G
AUX afxBool         AfxShiftKeyWasPressed(afxNat port); // Shift
AUX afxBool         AfxFireKeyWasPressed(afxNat port); // LMB
AUX afxBool         AfxPrecisionKeyWasPressed(afxNat port); // RMB

AUX void            AfxGetLeftStickAxis(afxNat port, afxV2d axis);
AUX void            AfxGetRightStickAxis(afxNat port, afxV2d axis);

AUX afxReal         AfxGetLeftTriggerPressure(afxNat port);
AUX afxReal         AfxGetRightTriggerPressure(afxNat port);

AUX void            AfxGetLhb(afxNat port, afxNat first, afxNat cnt);
AUX void            AfxGetRhb(afxNat port, afxNat first, afxNat cnt);

AUX void            AfxGetGamepadFnButton(afxNat port, afxNat first, afxNat cnt);

////////////////////////////////////////////////////////////////////////////////

AUX afxController   AfxGetController(afxNat port);

AUX afxError        AfxAcquireControllers(afxNat cnt, afxNat const port[], afxController controllers[]);

#endif//AUX_GAMEPAD_H
