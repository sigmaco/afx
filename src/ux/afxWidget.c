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
#define _AUX_WIDGET_C
#define _AUX_WINDOW_C
#include "auxIcd.h"

_AUX afxError AfxResetWidget(afxWidget wid)
{
    afxError err = { 0 };

    return err;
}

_AUX afxError AfxDoWidgetInput(afxWidget wid)
{
    afxError err = { 0 };

    afxWindow wnd = AfxGetHost(wid);
    afxEnvironment env = AfxGetHost(wnd);

    return err;
}

_AUX afxError AfxTestWidget(afxWidget wid)
{
    afxError err = { 0 };

    return err;
}

_AUX afxError AfxLockWidget(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);
    
    // reset chain
    AfxMakeChain(&wid->commands, wid);
    // exhaust arena
    if (wid->cmdArena.cleanupCnt > 3)
        AfxExhaustArena(&wid->cmdArena);

    wid->currNode = NIL;

    return err;
}

_AUX afxError AfxUnlockWidget(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    wid->updateCb(wid, 0);

    return err;
}

_AUX afxError AfxDrawWidget(afxWidget wid, afxRect const* area, afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);
    wid->renderCb(wid, area, dctx);
    return err;
}

_AUX afxError _AuxWidDtorCb(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    AfxDismantleArena(&wid->cmdArena);

    return err;
}

_AUX afxError _AuxWidCtorCb(afxWidget wid, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxWindow wnd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    afxWidgetConfig const* cfg = AFX_CAST(afxWidgetConfig const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    afxSurface dout;
    AfxGetWindowSurface(wnd, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxDrawSystem dsys = AvxGetSurfaceHost(dout);


    AfxMakeArena(&wid->cmdArena, NIL, AfxHere());
    AfxMakeChain(&wid->commands, wid);

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
    afxError err = { 0 };
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
