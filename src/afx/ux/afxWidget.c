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

#define _AUX_UX_C
#define _AUX_WIDGET_C
#include "dev/AuxDevKit.h"
//#include "dev/AuxOverWin32.h"


_AUX afxError _AuxWidCtorCb(afxWidget wid, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;

    afxScript app = cookie->udd[0];
    afxWidgetConfig const *config = ((afxWidgetConfig const *)cookie->udd[1]) + cookie->no;

    

    return err;
}

_AUX afxError _AuxWidDtorCb(afxWidget wid)
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
    .fixedSiz = sizeof(AFX_OBJECT(afxWidget)),
    .ctor = (void*)_AuxWidCtorCb,
    .dtor = (void*)_AuxWidDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireWidgets(afxWindow ovy, afxNat cnt, afxWidgetConfig config[], afxWidget widgets[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_WND);

    afxClass* cls = AfxGetWidgetClass(ovy);
    AfxAssertClass(cls, afxFcc_WID);

    if (AfxAcquireObjects(cls, cnt, AfxObjects(widgets), (void const*[]) { ovy, config }))
        AfxThrowError();

    return err;
}

_AUX afxNat AfxEnumerateWidgets(afxWindow ovy, afxNat first, afxNat cnt, afxWidget widgets[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_WND);
    afxClass *cls = AfxGetWidgetClass(ovy);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxEnumerateClassInstances(cls, first, cnt, AfxObjects(widgets));
}
