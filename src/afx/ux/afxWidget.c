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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <stdio.h>
#include <Windows.h>

#define _AUX_UX_C
#define _AUX_WIDGET_C
#include "qwadro/ux/afxWidget.h"
#include "qwadro/ux/afxScript.h"


_AUX afxError _AuxWidCtor(afxWidget wid, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;

    afxScript app = cookie->udd[0];
    afxWidgetConfig const *config = ((afxWidgetConfig const *)cookie->udd[1]) + cookie->no;

    

    return err;
}

_AUX afxError _AuxWidDtor(afxWidget wid)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wid, afxFcc_WID);

    afxScript app = AfxGetObjectProvider(wid);
    AfxAssertObjects(1, &app, afxFcc_APP);

    return err;
}

_AUX afxClassConfig _AuxWidMgrCfg =
{
    .fcc = afxFcc_WID,
    .name = "Widget",
    .maxCnt = 1,
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxWidget)),
    .mmu = NIL,
    .ctor = (void*)_AuxWidCtor,
    .dtor = (void*)_AuxWidDtor
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireWidgets(afxWindow ovy, afxNat cnt, afxWidgetConfig config[], afxWidget widgets[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_WND);

    afxManager* cls = AfxGetWidgetClass(ovy);
    AfxAssertClass(cls, afxFcc_WID);

    if (AfxAcquireObjects(cls, cnt, AfxObjects(widgets), (void const*[]) { ovy, config }))
        AfxThrowError();

    return err;
}

_AUX afxNat AfxEnumerateWidgets(afxWindow ovy, afxNat first, afxNat cnt, afxWidget widgets[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_WND);
    afxManager *cls = AfxGetWidgetClass(ovy);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxEnumerateObjects(cls, first, cnt, AfxObjects(widgets));
}
