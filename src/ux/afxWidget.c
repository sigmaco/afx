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

#define _AUX_UX_C
#define _AUX_WIDGET_C
#define _AUX_WINDOW_C
#include "impl/auxImplementation.h"

_AUX afxCmdId AfxWidCommencePanel()
{

}

_AUX afxError AfxResetWidget(afxWidget wid)
{
    afxError err = NIL;

    return err;
}

_AUX afxError AfxDoWidgetInput(afxWidget wid)
{
    afxError err = NIL;

    afxWindow wnd = AfxGetProvider(wid);
    afxSession ses = AfxGetProvider(wnd);

    return err;
}

_AUX afxError AfxTestWidget(afxWidget wid)
{
    afxError err = NIL;

    return err;
}

_AUX afxError AfxDrawWidget(afxWidget wid, afxWarp const whd, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);
    wid->renderCb(wid, dctx);
    return err;
}

_AUX afxError _AuxWidDtorCb(afxWidget wid)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    return err;
}

_AUX afxError _AuxWidCtorCb(afxWidget wid, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxWindow wnd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    afxWidgetConfig const* cfg = AFX_CAST(afxWidgetConfig const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    afxSurface dout;
    AfxGetWindowSurface(wnd, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxDrawSystem dsys = AvxGetSurfaceSystem(dout);

    return err;
}

_AUX afxClassConfig const _AUX_WID_CLASS_CONFIG =
{
    .fcc = afxFcc_WID,
    .name = "Widget",
    .desc = "Widget",
    .fixedSiz = sizeof(AFX_OBJECT(afxWidget)),
    .ctor = (void*)_AuxWidCtorCb,
    .dtor = (void*)_AuxWidDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireWidgets(afxWindow wnd, afxUnit cnt, afxWidgetConfig const cfg[], afxWidget widgets[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxClass* cls = (afxClass*)_AuxWndGetWidClass(wnd);
    AFX_ASSERT_CLASS(cls, afxFcc_WID);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)widgets, (void const*[]) { wnd, cfg }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}
