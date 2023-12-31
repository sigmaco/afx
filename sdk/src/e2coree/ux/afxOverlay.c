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

#define _AFX_UX_C
#define _AFX_OVERLAY_C
#include "qwadro/core/afxClass.h"
#include "qwadro/ux/afxOverlay.h"

_AFX afxError _AfxOvlyCtor(afxOverlay ovly, afxCookie const *cookie)
{
    AfxEntry("ovly=%p", ovly);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovly, afxFcc_OVLY);

    afxApplication app = cookie->udd[0];


    return err;
}

_AFX afxError _AfxOvlyDtor(afxOverlay ovly)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovly, afxFcc_OVLY);

    afxApplication app = AfxGetObjectProvider(ovly);
    AfxAssertObjects(1, &app, afxFcc_APP);

#if 0
    if (appD->hoveredWidg == wid)
        AfxApplicationHoverWidget(app, NIL, NIL);

    if (appD->focusedWidg == wid)
        AfxApplicationFocusWidget(app, NIL, NIL);

    if (appD->grabbedWidg == wid)
        AfxApplicationGrabWidget(app, NIL, NIL);
#endif

    return err;
}

_AFX afxClassConfig _AfxOvlyClsConfig =
{
    .fcc = afxFcc_OVLY,
    .name = "Overlay",
    .maxCnt = 1,
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxOverlay)),
    .mmu = NIL,
    .ctor = (void*)_AfxOvlyCtor,
    .dtor = (void*)_AfxOvlyDtor
};
