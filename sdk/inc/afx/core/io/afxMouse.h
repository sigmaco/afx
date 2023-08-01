/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MOUSE_H
#define AFX_MOUSE_H

#include "afx/core/io/afxHid.h"

typedef enum
{
    AFX_LMB,
    AFX_RMB,
    AFX_MMB,
    AFX_XMB1,
    AFX_XMB2,

    AFX_MB_TOTAL
} afxMouseButton;

AFX_OBJECT(afxMouse)
{
    AFX_OBJECT(afxHid)  hid;
#ifdef _AFX_MOUSE_C
    afxNat              buttonCnt;
    afxNat              sampleRate;
    afxBool             currState[AFX_MB_TOTAL];
    afxBool             prevState[AFX_MB_TOTAL];
    afxReal             lastWheelDelta, prevWheelDelta;
    afxReal             lastMotion[2], prevMotion[2];
#endif
};

AFX afxError    AfxEmulateMouseButtonActions(afxMouse mse, afxNat cnt, afxMouseButton const butt[], afxBool const pressed[]);
AFX afxError    AfxEmulateMouseMotion(afxMouse mse, afxReal const motion[2]);
AFX afxError    AfxEmulateMouseWheelAction(afxMouse mse, afxReal delta);

AFX void        AfxGetLastMouseMotion(afxMouse mse, afxReal motion[2]);
AFX afxReal     AfxGetLastMouseWheelData(afxMouse mse);

AFX afxBool     AfxMouseHasHorizontalChanged(afxMouse mse, afxInt tolerance);
AFX afxBool     AfxMouseHasVerticalChanged(afxMouse mse, afxInt tolerance);

AFX afxBool     AfxIsLmbPressed(afxMouse mse);
AFX afxBool     AfxIsMmbPressed(afxMouse mse);
AFX afxBool     AfxIsRmbPressed(afxMouse mse);
AFX afxBool     AfxIsXmb1Pressed(afxMouse mse);
AFX afxBool     AfxIsXmb2Pressed(afxMouse mse);
AFX afxBool     AfxWasLmbPressed(afxMouse mse);
AFX afxBool     AfxWasLmbReleased(afxMouse mse);
AFX afxBool     AfxWasMmbPressed(afxMouse mse);
AFX afxBool     AfxWasMmbReleased(afxMouse mse);
AFX afxBool     AfxWasRmbPressed(afxMouse mse);
AFX afxBool     AfxWasRmbReleased(afxMouse mse);
AFX afxBool     AfxWasXmb1Pressed(afxMouse mse);
AFX afxBool     AfxWasXmb1Released(afxMouse mse);
AFX afxBool     AfxWasXmb2Pressed(afxMouse mse);
AFX afxBool     AfxWasXmb2Released(afxMouse mse);

#endif//AFX_MOUSE_H