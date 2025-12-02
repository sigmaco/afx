/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

#define _AUX_UX_C
#define _AUX_LAYOUT_C
#include "auxIcd.h"

AFX_DEFINE_HANDLE(afxLayout);

_AUX afxError _AuxLayDtorCb(afxLayout lay)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_LAY, 1, &lay);

    return err;
}

_AUX afxError _AuxLayCtorCb(afxLayout lay, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_LAY, 1, &lay);

    afxWindow wnd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    afxWidgetConfig const* cfg = AFX_CAST(afxWidgetConfig const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    return err;
}

_AUX afxClassConfig const _AUX_LAY_CLASS_CONFIG =
{
    .fcc = afxFcc_LAY,
    .name = "Layout",
    .desc = "Layout",
    //.fixedSiz = sizeof(AFX_OBJECT(afxLayout)),
    .ctor = (void*)_AuxLayCtorCb,
    .dtor = (void*)_AuxLayDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireLayouts(afxWindow wnd, afxUnit cnt, afxWidgetConfig const cfg[], afxWidget widgets[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxClass* cls = (afxClass*)_AuxWndGetWidClass(wnd);
    AFX_ASSERT_CLASS(cls, afxFcc_LAY);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)widgets, (void const*[]) { wnd, cfg }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}
