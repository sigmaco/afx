/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MOUSE_H
#define AFX_MOUSE_H

#include "afx/core/diag/afxDebug.h"
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

struct _afxMseD
#ifdef _AFX_MOUSE_C
{
    _AFX_DBG_FCC
    afxMouse            mseObj;
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
}
#endif
;

AFX afxNat      AfxEnumerateMouses(afxNat first, afxNat cnt, afxMouse mse[]);
AFX afxMouse    AfxFindMouse(afxNat port);

AFX afxError    AfxAcquireMouses(afxNat cnt, afxMouse mse[], afxNat port[]);
AFX void        AfxReleaseMouses(afxNat cnt, afxMouse mse[]);

AFX afxError    AfxEmulateMouseButtonActions(afxNat cnt, afxMouseButton const butt[], afxBool const pressed[], afxNat port);
AFX afxError    AfxEmulateMouseWheelAction(afxReal delta, afxNat port);
AFX afxError    AfxEmulateMouseMotion(afxReal const motion[2], afxNat port);

AFX void        AfxGetLastMouseMotion(afxReal motion[2], afxNat port);
AFX afxReal     AfxGetLastMouseWheelData(afxNat port);

AFX afxBool     AfxMouseHasHorizontalChanged(afxInt tolerance, afxNat port);
AFX afxBool     AfxMouseHasVerticalChanged(afxInt tolerance, afxNat port);

AFX afxBool     AfxLmbIsPressed(afxNat port);
AFX afxBool     AfxMmbIsPressed(afxNat port);
AFX afxBool     AfxRmbIsPressed(afxNat port);
AFX afxBool     AfxXmbIs1Pressed(afxNat port);
AFX afxBool     AfxXmbIs2Pressed(afxNat port);
AFX afxBool     AfxLmbWasPressed(afxNat port);
AFX afxBool     AfxLmbWasReleased(afxNat port);
AFX afxBool     AfxMmbWasPressed(afxNat port);
AFX afxBool     AfxMmbWasReleased(afxNat port);
AFX afxBool     AfxRmbWasPressed(afxNat port);
AFX afxBool     AfxRmbWasReleased(afxNat port);
AFX afxBool     AfxXmbWas1Pressed(afxNat port);
AFX afxBool     AfxXmbWas1Released(afxNat port);
AFX afxBool     AfxXmbWas2Pressed(afxNat port);
AFX afxBool     AfxXmbWas2Released(afxNat port);

#endif//AFX_MOUSE_H