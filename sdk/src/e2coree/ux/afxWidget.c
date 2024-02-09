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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <stdio.h>
#include <Windows.h>

#define _AFX_UX_C
#define _AFX_WIDGET_C
#include "qwadro/ux/afxWidget.h"
#include "qwadro/ux/afxApplication.h"


_AFX afxError _AfxWidCtor(afxWidget wid, afxCookie const *cookie)
{
    AfxEntry("wid=%p", wid);
    afxError err = AFX_ERR_NONE;

    afxApplication app = cookie->udd[0];
    afxWidgetConfig const *config = ((afxWidgetConfig const *)cookie->udd[1]) + cookie->no;

    

    return err;
}

_AFX afxError _AfxWidDtor(afxWidget wid)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wid, afxFcc_WID);

    afxApplication app = AfxGetObjectProvider(wid);
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

_AFX afxClassConfig _AfxWidClsConfig =
{
    .fcc = afxFcc_WID,
    .name = "Widget",
    .maxCnt = 1,
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxWidget)),
    .mmu = NIL,
    .ctor = (void*)_AfxWidCtor,
    .dtor = (void*)_AfxWidDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireWidgets(afxApplication app, afxNat cnt, afxWidget wid[], afxWidgetConfig config[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);

    afxClass* cls = AfxGetWidgetClass(app);
    AfxAssertClass(cls, afxFcc_WID);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)wid, (void const*[]) { app, config }))
        AfxThrowError();

    return err;
}
