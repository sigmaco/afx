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
#include "../dev/AuxImplKit.h"


_AUX afxError _AuxWidCtorCb(afxWidget wid, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wid, afxFcc_WID);

    afxSession ses = args[0];
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxWidgetConfig const* cfg = ((afxWidgetConfig const *)args[1]) + invokeNo;

    

    return err;
}

_AUX afxError _AuxWidDtorCb(afxWidget wid)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wid, afxFcc_WID);

    return err;
}

_AUX afxClassConfig const _AuxWidStdImplementation =
{
    .fcc = afxFcc_WID,
    .name = "Widget",
    .desc = "Widget",
    .fixedSiz = sizeof(AFX_OBJECT(afxWidget)),
    .ctor = (void*)_AuxWidCtorCb,
    .dtor = (void*)_AuxWidDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireWidgets(afxSession ses, afxNat cnt, afxWidgetConfig cfg[], afxWidget widgets[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    afxClass* cls = (afxClass*)AfxGetWidgetClass(ses);
    AfxAssertClass(cls, afxFcc_WID);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)widgets, (void const*[]) { ses, cfg }))
        AfxThrowError();

    return err;
}

_AUX afxNat AfxEnumerateWidgets(afxSession ses, afxNat first, afxNat cnt, afxWidget widgets[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxClass const* cls = AfxGetWidgetClass(ses);
    AfxAssertClass(cls, afxFcc_WID);
    return AfxEnumerateClassInstances(cls, first, cnt, AfxObjects(widgets));
}
