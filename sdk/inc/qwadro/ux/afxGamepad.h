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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_GAMEPAD_H
#define AFX_GAMEPAD_H

#include "qwadro/ux/afxHid.h"
#include "qwadro/math/afxVector.h"

#ifdef _AFX_CORE_C
#ifdef _AFX_GAMEPAD_C
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
#endif//_AFX_GAMEPAD_C
#ifdef _AFX_CONTROLLER_C
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
#endif//_AFX_CONTROLLER_C
#endif//_AFX_CORE_C

AFX afxBool         AfxTabKeyWasPressed(afxNat port); // Tab
AFX afxBool         AfxQuickKeyWasPressed(afxNat port); // Q
AFX afxBool         AfxForwardKeyWasPressed(afxNat port); // W
AFX afxBool         AfxBackwardKeyWasPressed(afxNat port); // S
AFX afxBool         AfxWestwardKeyWasPressed(afxNat port); // A
AFX afxBool         AfxEastwardKeyWasPressed(afxNat port); // D
AFX afxBool         AfxExecuteKeyWasPressed(afxNat port); // E
AFX afxBool         AfxReuseKeyWasPressed(afxNat port); // R
AFX afxBool         AfxFunctionKeyWasPressed(afxNat port); // F
AFX afxBool         AfxGiveKeyWasPressed(afxNat port); // G
AFX afxBool         AfxShiftKeyWasPressed(afxNat port); // Shift
AFX afxBool         AfxFireKeyWasPressed(afxNat port); // LMB
AFX afxBool         AfxPrecisionKeyWasPressed(afxNat port); // RMB

AFX void            AfxGetLeftStickAxis(afxNat port, afxV2d axis);
AFX void            AfxGetRightStickAxis(afxNat port, afxV2d axis);

AFX afxReal         AfxGetLeftTriggerPressure(afxNat port);
AFX afxReal         AfxGetRightTriggerPressure(afxNat port);

AFX void            AfxGetLhb(afxNat port, afxNat first, afxNat cnt);
AFX void            AfxGetRhb(afxNat port, afxNat first, afxNat cnt);

AFX void            AfxGetGamepadFnButton(afxNat port, afxNat first, afxNat cnt);

////////////////////////////////////////////////////////////////////////////////

AFX afxController   AfxGetController(afxNat port);

AFX afxError        AfxAcquireControllers(afxNat cnt, afxNat const port[], afxController controllers[]);

#endif//AFX_GAMEPAD_H
