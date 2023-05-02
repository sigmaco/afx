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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
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

AFX_DEFINE_HANDLE(afxMouse);

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

AFX void*       AfxMouseGetSystem(afxMouse mse);

AFX afxResult   AfxMouseEmulateButtonActions(afxMouse mse, afxNat cnt, afxMouseButton const butt[], afxBool const pressed[]);
AFX afxResult   AfxMouseEmulateMotion(afxMouse mse, afxReal const motion[2]);
AFX afxResult   AfxMouseEmulateWheelAction(afxMouse mse, afxReal delta);
AFX afxBool     AfxMouseHasHorizontalChanged(afxMouse mse, afxInt tolerance);
AFX afxBool     AfxMouseHasVerticalChanged(afxMouse mse, afxInt tolerance);
AFX afxBool     AfxMouseIsLmbPressed(afxMouse mse);
AFX afxBool     AfxMouseIsMmbPressed(afxMouse mse);
AFX afxBool     AfxMouseIsRmbPressed(afxMouse mse);
AFX afxBool     AfxMouseIsXmb1Pressed(afxMouse mse);
AFX afxBool     AfxMouseIsXmb2Pressed(afxMouse mse);
AFX afxBool     AfxMouseWasLmbPressed(afxMouse mse);
AFX afxBool     AfxMouseWasLmbReleased(afxMouse mse);
AFX afxBool     AfxMouseWasMmbPressed(afxMouse mse);
AFX afxBool     AfxMouseWasMmbReleased(afxMouse mse);
AFX afxBool     AfxMouseWasRmbPressed(afxMouse mse);
AFX afxBool     AfxMouseWasRmbReleased(afxMouse mse);
AFX afxBool     AfxMouseWasXmb1Pressed(afxMouse mse);
AFX afxBool     AfxMouseWasXmb1Released(afxMouse mse);
AFX afxBool     AfxMouseWasXmb2Pressed(afxMouse mse);
AFX afxBool     AfxMouseWasXmb2Released(afxMouse mse);

#endif//AFX_MOUSE_H