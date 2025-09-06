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

// This software is part of Advanced Multimedia UX Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience on Qwadro                                       //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_GAMEPAD_H
#define AUX_GAMEPAD_H

#include "qwadro/ux/afxHid.h"
#include "qwadro/math/afxVector.h"

AUX afxBool         AfxTabKeyWasPressed(afxUnit port); // Tab
AUX afxBool         AfxQuickKeyWasPressed(afxUnit port); // Q
AUX afxBool         AfxForwardKeyWasPressed(afxUnit port); // W
AUX afxBool         AfxBackwardKeyWasPressed(afxUnit port); // S
AUX afxBool         AfxWestwardKeyWasPressed(afxUnit port); // A
AUX afxBool         AfxEastwardKeyWasPressed(afxUnit port); // D
AUX afxBool         AfxExecuteKeyWasPressed(afxUnit port); // E
AUX afxBool         AfxReuseKeyWasPressed(afxUnit port); // R
AUX afxBool         AfxFunctionKeyWasPressed(afxUnit port); // F
AUX afxBool         AfxGiveKeyWasPressed(afxUnit port); // G
AUX afxBool         AfxShiftKeyWasPressed(afxUnit port); // Shift
AUX afxBool         AfxFireKeyWasPressed(afxUnit port); // LMB
AUX afxBool         AfxPrecisionKeyWasPressed(afxUnit port); // RMB

AUX void            AfxGetLeftStickAxis(afxUnit port, afxV2d axis);
AUX void            AfxGetRightStickAxis(afxUnit port, afxV2d axis);

AUX afxReal         AfxGetLeftTriggerPressure(afxUnit port);
AUX afxReal         AfxGetRightTriggerPressure(afxUnit port);

AUX void            AfxGetLhb(afxUnit port, afxUnit first, afxUnit cnt);
AUX void            AfxGetRhb(afxUnit port, afxUnit first, afxUnit cnt);

AUX void            AfxGetGamepadFnButton(afxUnit port, afxUnit first, afxUnit cnt);

////////////////////////////////////////////////////////////////////////////////

AUX afxController   AfxGetController(afxUnit port);

AUX afxError        AfxAcquireControllers(afxUnit cnt, afxUnit const port[], afxController controllers[]);

#endif//AUX_GAMEPAD_H
