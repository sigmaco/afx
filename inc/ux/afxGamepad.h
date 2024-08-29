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

#include "qwadro/inc/ux/afxHid.h"
#include "qwadro/inc/math/afxVector.h"

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
