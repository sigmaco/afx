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

#ifndef AUX_MOUSE_H
#define AUX_MOUSE_H

#include "qwadro/inc/ux/afxHid.h"
#include "qwadro/inc/math/afxVector.h"

typedef enum afxMouseButton
{
    AFX_LMB,
    AFX_RMB,
    AFX_MMB,
    AFX_XMB1,
    AFX_XMB2,

    AFX_MB_TOTAL
} afxMouseButton;

// MOUSE AND POINTER

AUX afxBool         AfxIsLmbPressed(afxSession ses, afxNat seat);
AUX afxBool         AfxWasLmbPressed(afxSession ses, afxNat seat);
AUX afxBool         AfxWasLmbReleased(afxSession ses, afxNat seat);

AUX afxBool         AfxIsRmbPressed(afxSession ses, afxNat seat);
AUX afxBool         AfxWasRmbPressed(afxSession ses, afxNat seat);
AUX afxBool         AfxWasRmbReleased(afxSession ses, afxNat seat);

AUX afxBool         AfxIsMmbPressed(afxSession ses, afxNat seat);
AUX afxBool         AfxWasMmbPressed(afxSession ses, afxNat seat);
AUX afxBool         AfxWasMmbReleased(afxSession ses, afxNat seat);

AUX afxBool         AfxIsXmb1Pressed(afxSession ses, afxNat seat);
AUX afxBool         AfxWasXmb1Pressed(afxSession ses, afxNat seat);
AUX afxBool         AfxWasXmb1Released(afxSession ses, afxNat seat);

AUX afxBool         AfxIsXmb2Pressed(afxSession ses, afxNat seat);
AUX afxBool         AfxWasXmb2Pressed(afxSession ses, afxNat seat);
AUX afxBool         AfxWasXmb2Released(afxSession ses, afxNat seat);

AUX void            AfxGetMouseMotion(afxSession ses, afxNat seat, afxReal motion[2]);
AUX afxReal         AfxGetMouseWheelDelta(afxSession ses, afxNat seat);

AUX afxBool         AfxMouseHasHorizontalChanged(afxSession ses, afxNat seat, afxInt tolerance);
AUX afxBool         AfxMouseHasVerticalChanged(afxSession ses, afxNat seat, afxInt tolerance);

AUX afxError        AfxEmulateMouseMotion(afxSession ses, afxNat seat, afxReal const motion[2], afxWindow wnd);
AUX afxError        AfxEmulateMouseWheelAction(afxSession ses, afxNat seat, afxReal delta, afxWindow wnd);
AUX afxError        AfxEmulateMouseButtonActions(afxSession ses, afxNat seat, afxNat cnt, afxMouseButton const butt[], afxBool const pressed[], afxWindow wnd);
AUX afxError        AfxReleaseMouseButtons(afxSession ses, afxNat seat, afxWindow wnd);

#endif//AUX_MOUSE_H
