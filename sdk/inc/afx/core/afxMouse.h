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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MOUSE_H
#define AFX_MOUSE_H

#include "afx/core/afxDebug.h"
#include "afx/math/afxMathDefs.h"

typedef enum
{
    AFX_LMB,
    AFX_RMB,
    AFX_MMB,
    AFX_XMB1,
    AFX_XMB2,

    AFX_MB_TOTAL
} afxMouseButton;

#ifdef _AFX_MOUSE_C
AFX_OBJECT(afxMouse)
{
    afxNat              port;
    afxFlags            flags;
    afxNat              buttonCnt;
    afxNat              sampleRate;
    afxBool             currState[AFX_MB_TOTAL];
    afxBool             prevState[AFX_MB_TOTAL];
    afxReal             lastWheelDelta;
    afxReal             prevWheelDelta;
    afxV2d              lastMotion;
    afxV2d              prevMotion;
};
#endif

AFX afxError    AfxAcquireMouses(afxNat cnt, afxNat const port[], afxMouse mse[]);

AFX afxBool     AfxGetMouse(afxNat port, afxMouse* mse);

AFX afxBool     AfxLmbIsPressed(afxNat port);
AFX afxBool     AfxLmbWasPressed(afxNat port);
AFX afxBool     AfxLmbWasReleased(afxNat port);

AFX afxBool     AfxRmbIsPressed(afxNat port);
AFX afxBool     AfxRmbWasPressed(afxNat port);
AFX afxBool     AfxRmbWasReleased(afxNat port);

AFX afxBool     AfxMmbIsPressed(afxNat port);
AFX afxBool     AfxMmbWasPressed(afxNat port);
AFX afxBool     AfxMmbWasReleased(afxNat port);

AFX afxBool     AfxXmbIs1Pressed(afxNat port);
AFX afxBool     AfxXmbWas1Pressed(afxNat port);
AFX afxBool     AfxXmbWas1Released(afxNat port);

AFX afxBool     AfxXmbIs2Pressed(afxNat port);
AFX afxBool     AfxXmbWas2Pressed(afxNat port);
AFX afxBool     AfxXmbWas2Released(afxNat port);

AFX void        AfxGetLastMouseMotion(afxNat port, afxReal motion[2]);
AFX afxReal     AfxGetLastMouseWheelData(afxNat port);

AFX afxBool     AfxMouseHasHorizontalChanged(afxNat port, afxInt tolerance);
AFX afxBool     AfxMouseHasVerticalChanged(afxNat port, afxInt tolerance);

AFX afxError    AfxEmulateMouseMotion(afxNat port, afxReal const motion[2]);
AFX afxError    AfxEmulateMouseWheelAction(afxNat port, afxReal delta);
AFX afxError    AfxEmulateMouseButtonActions(afxNat port, afxNat cnt, afxMouseButton const butt[], afxBool const pressed[]);

#endif//AFX_MOUSE_H