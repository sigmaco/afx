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

AUX afxBool         AfxIsLmbPressed(afxSession ses, afxUnit seat);
AUX afxBool         AfxWasLmbPressed(afxSession ses, afxUnit seat);
AUX afxBool         AfxWasLmbReleased(afxSession ses, afxUnit seat);

AUX afxBool         AfxIsRmbPressed(afxSession ses, afxUnit seat);
AUX afxBool         AfxWasRmbPressed(afxSession ses, afxUnit seat);
AUX afxBool         AfxWasRmbReleased(afxSession ses, afxUnit seat);

AUX afxBool         AfxIsMmbPressed(afxSession ses, afxUnit seat);
AUX afxBool         AfxWasMmbPressed(afxSession ses, afxUnit seat);
AUX afxBool         AfxWasMmbReleased(afxSession ses, afxUnit seat);

AUX afxBool         AfxIsXmb1Pressed(afxSession ses, afxUnit seat);
AUX afxBool         AfxWasXmb1Pressed(afxSession ses, afxUnit seat);
AUX afxBool         AfxWasXmb1Released(afxSession ses, afxUnit seat);

AUX afxBool         AfxIsXmb2Pressed(afxSession ses, afxUnit seat);
AUX afxBool         AfxWasXmb2Pressed(afxSession ses, afxUnit seat);
AUX afxBool         AfxWasXmb2Released(afxSession ses, afxUnit seat);

AUX void            AfxGetMouseMotion(afxSession ses, afxUnit seat, afxReal motion[2]);
AUX afxReal         AfxGetMouseWheelDelta(afxSession ses, afxUnit seat);

AUX afxBool         AfxMouseHasHorizontalChanged(afxSession ses, afxUnit seat, afxInt tolerance);
AUX afxBool         AfxMouseHasVerticalChanged(afxSession ses, afxUnit seat, afxInt tolerance);

AUX afxError        AfxEmulateMouseMotion(afxSession ses, afxUnit seat, afxReal const motion[2], afxWindow wnd);
AUX afxError        AfxEmulateMouseWheelAction(afxSession ses, afxUnit seat, afxReal delta, afxWindow wnd);
AUX afxError        AfxEmulateMouseButtonActions(afxSession ses, afxUnit seat, afxUnit cnt, afxMouseButton const butt[], afxBool const pressed[], afxWindow wnd);
AUX afxError        AfxReleaseMouseButtons(afxSession ses, afxUnit seat, afxWindow wnd);

#endif//AUX_MOUSE_H
