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

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience on Qwadro                                       //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_MOUSE_H
#define AUX_MOUSE_H

#include "qwadro/inc/ux/hid/afxHid.h"
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

AUX afxBool         AfxIsLmbPressed(afxUnit seat);
AUX afxBool         AfxWasLmbPressed(afxUnit seat);
AUX afxBool         AfxWasLmbReleased(afxUnit seat);

AUX afxBool         AfxIsRmbPressed(afxUnit seat);
AUX afxBool         AfxWasRmbPressed(afxUnit seat);
AUX afxBool         AfxWasRmbReleased(afxUnit seat);

AUX afxBool         AfxIsMmbPressed(afxUnit seat);
AUX afxBool         AfxWasMmbPressed(afxUnit seat);
AUX afxBool         AfxWasMmbReleased(afxUnit seat);

AUX afxBool         AfxIsXmb1Pressed(afxUnit seat);
AUX afxBool         AfxWasXmb1Pressed(afxUnit seat);
AUX afxBool         AfxWasXmb1Released(afxUnit seat);

AUX afxBool         AfxIsXmb2Pressed(afxUnit seat);
AUX afxBool         AfxWasXmb2Pressed(afxUnit seat);
AUX afxBool         AfxWasXmb2Released(afxUnit seat);

AUX void            AfxGetMouseMotion(afxUnit seat, afxV2d motion, afxV2d vel, afxV2d accel);
AUX afxReal         AfxGetMouseWheelDelta(afxUnit seat);

AUX afxBool         AfxMouseHasHorizontalChanged(afxUnit seat, afxInt tolerance);
AUX afxBool         AfxMouseHasVerticalChanged(afxUnit seat, afxInt tolerance);

AUX afxError        AfxEmulateMouseMotion(afxUnit seat, afxV2d const motion, afxWindow wnd);
AUX afxError        AfxEmulateMouseWheelAction(afxUnit seat, afxReal delta, afxWindow wnd);
AUX afxError        AfxEmulateMouseButtonActions(afxUnit seat, afxUnit cnt, afxMouseButton const butt[], afxBool const pressed[], afxWindow wnd);
AUX afxError        AfxReleaseMouseButtons(afxUnit seat, afxWindow wnd);


AUX afxMask AfxIsLmbPressed2(afxMask seats);
AUX afxMask AfxIsRmbPressed2(afxMask seats);
AUX afxMask AfxIsMmbPressed2(afxMask seats);
AUX afxMask AfxIsXmb1Pressed2(afxMask seats);
AUX afxMask AfxIsXmb2Pressed2(afxMask seats);
AUX afxMask AfxWasLmbPressed2(afxMask seats);
AUX afxMask AfxWasRmbPressed2(afxMask seats);
AUX afxMask AfxWasMmbPressed2(afxMask seats);
AUX afxMask AfxWasXmb1Pressed2(afxMask seats);
AUX afxMask AfxWasXmb2Pressed2(afxMask seats);
AUX afxMask AfxWasLmbReleased2(afxMask seats);
AUX afxMask AfxWasRmbReleased2(afxMask seats);
AUX afxBool AfxWasMmbReleased2(afxMask seats);
AUX afxMask AfxWasXmb1Released2(afxMask seats);
AUX afxMask AfxWasXmb2Released2(afxMask seats);

#endif//AUX_MOUSE_H
