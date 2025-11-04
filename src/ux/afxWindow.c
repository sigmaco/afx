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
//#define _AUX_SHELL_C
#define _AUX_ENVIRONMENT_C
#define _AUX_WINDOW_C
#include "impl/auxImplementation.h"

_AUX void* AfxGetWindowUdd(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    return wnd->udd;
}

_AUX afxClass const* _AuxWndGetWidClass(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    afxClass const* cls = &wnd->widCls;
    AFX_ASSERT_CLASS(cls, afxFcc_WID);
    return cls;
}

_AUX afxError AfxChangeWindowIcon(afxWindow wnd, avxRaster icon, avxRasterRegion const* rgn)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    if (wnd->pimpl->chIconCb)
    {
        if (wnd->pimpl->chIconCb(wnd, icon, rgn))
            AfxThrowError();

        return err;
    }

    // If the host platform doesn't offer custom icon support, we do with via draw system.
    avxRaster curr = wnd->icon;

    if (curr != icon)
    {
        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &curr);
            AfxDisposeObjects(1, &curr);
            wnd->icon = NIL;
        }

        if (icon)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &icon);
            AfxReacquireObjects(1, &icon);
            wnd->icon = icon;
            wnd->iconCrop = *rgn;
        }
    }
    return err;
}

_AUX afxError AfxChangeWindowCursor(afxWindow wnd, avxRaster curs, avxRasterRegion const* rgn, afxInt hotspotX, afxInt hotspotY)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    
    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    if (wnd->pimpl->chCursCb)
    {
        if (wnd->pimpl->chCursCb(wnd, curs, rgn, hotspotX, hotspotY))
            AfxThrowError();

        return err;
    }

    // If the host platform doesn't offer custom icon support, we do with via draw system.
    avxRaster curr = wnd->curs;

    if (curr != curs)
    {
        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &curr);
            AfxDisposeObjects(1, &curr);
            wnd->curs = NIL;
        }

        if (curs)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &curs);
            AfxReacquireObjects(1, &curs);
            wnd->curs = curs;
            wnd->cursCrop = *rgn;
        }
    }
    return err;
}

_AUX afxError AfxLoadWindowIcon(afxWindow wnd, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(uri);

    afxDrawSystem dsys = AvxGetSurfaceHost(wnd->dout ? wnd->dout : wnd->frameDout);
    
    avxRasterInfo rasi = { 0 };
    rasi.flags = avxRasterFlag_2D;
    rasi.usage = avxRasterUsage_SRC;
    
    avxRaster ras;
    avxRasterFile tga = { 0 };
    if (AvxLoadRasters(dsys, 1, &rasi, &uri[0], &tga, &ras))
    {
        AfxThrowError();
        return err;
    }

    avxRasterRegion rgn = { 0 };
    rgn.whd = AvxGetRasterExtent(ras, rgn.lodIdx);

    if (AfxChangeWindowIcon(wnd, ras, &rgn))
        AfxThrowError();

    AfxDisposeObjects(1, &ras);

    return err;
}

_AUX afxError AfxLoadWindowCursor(afxWindow wnd, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(uri);

    afxDrawSystem dsys = AvxGetSurfaceHost(wnd->dout ? wnd->dout : wnd->frameDout);

    avxRasterInfo rasi = { 0 };
    rasi.flags = avxRasterFlag_2D;
    rasi.usage = avxRasterUsage_SRC;

    avxRaster ras;
    avxRasterFile tga = { 0 };
    if (AvxLoadRasters(dsys, 1, &rasi, &uri[0], &tga, &ras))
    {
        AfxThrowError();
        return err;
    }

    avxRasterRegion rgn = { 0 };
    rgn.whd = AvxGetRasterExtent(ras, rgn.lodIdx);

    if (AfxChangeWindowCursor(wnd, ras, &rgn, 0, 0))
        AfxThrowError();

    AfxDisposeObjects(1, &ras);

    return err;
}

_AUX afxError AfxRedrawWindow(afxWindow wnd, afxRect const* area)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    if (wnd->pimpl->redrawCb)
    {
        if (wnd->pimpl->redrawCb(wnd, area))
            AfxThrowError();
    }
    else
    {

    }
    return err;
}

_AUX afxError AfxRedrawWidgets(afxWindow wnd, afxRect const* area, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxClass const* cls = _AuxWndGetWidClass(wnd);

    afxRect area2 = area ? *area : wnd->areaRc;

    // Desist if there is not area.
    if (1 >= AfxGetRectArea(&area2))
        return err;

    afxUnit i = 0;
    afxWidget wid;
    while (AfxEnumerateObjects(cls, i, 1, (afxObject*)&wid))
    {
        AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);
        AfxDrawWidget(wid, &area2, dctx);
        i++;
    }
    return err;
}

_AUX afxError AfxGetWindowSurface(afxWindow wnd, afxSurface* surface)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(surface);
    afxMask found = NIL;
    afxSurface dout;
#if 0
    if (frame)
    {
        *frame = (dout = wnd->frameDout);

        if (dout)
        {
            AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
            found |= AFX_BITMASK(0);
        }
    }
#endif
    if (surface)
    {
        *surface = (dout = wnd->dout);
        
        if (dout)
        {
            AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
            found |= AFX_BITMASK(1);
        }
    }

    if (!found)
        err = afxError_NOT_READY;

    return err;
}

_AUX void _AfxStepWindow(afxWindow wnd, afxReal64* ct, afxReal64* dt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    afxClock currClock;
    AfxGetClock(&currClock);
    AFX_ASSERT(ct);
    *ct = AfxGetSecondsElapsed(&wnd->startClock, &currClock);
    AFX_ASSERT(dt);
    *dt = AfxGetSecondsElapsed(&wnd->lastClock, &currClock);
    wnd->lastClock = currClock;
}

/*
    The AfxMakeWindowCursory() method makes a window cursory, that is, superficial, lacking in depth.
    At this mode, which is the default one, the cursor is enabled to work with widgets and other interactive elements.
    Disabling cursory may be used to grab and/or center the mouse to be used in window area as, for example, a interactive aim.
*/

_AUX afxError AfxMakeWindowCursory(afxWindow wnd, afxRect const* confinement, afxBool cursory)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    wnd->cursHidden = !cursory;

    if (!confinement)
    {
        wnd->cursConfined = FALSE;
    }
    else
    {
        // Possibly replace Union with Intersection if that's more appropriate.
        if (AfxIntersectRects(&wnd->cursConfinRect, &wnd->areaRc, confinement))
        {
            wnd->cursConfined = TRUE;
        }
        else
        {
            // Optionally handle error here.
            wnd->cursConfined = FALSE;
            err = afxError_INSUFFICIENT; // confinement is outside frame.
        }
    }
    return err;
}

_AUX afxUnit AfxFormatWindowTitle(afxWindow wnd, afxChar const* fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    if (!fmt)
    {
        AfxEraseString(&wnd->title.s, 0, wnd->title.s.len);
    }
    else
    {
        AFX_ASSERT(fmt);
        va_list va;
        va_start(va, fmt);
        afxUnit len = AfxFormatStringArg(&wnd->title.s, fmt, va);
        wnd->redrawFrameRequested = TRUE;
        va_end(va);
    }

    if (wnd->pimpl->titleCb)
        if (wnd->pimpl->titleCb(wnd))
            AfxThrowError();

    return wnd->title.s.len;
}

_AUX afxBool AfxGetOnSurfaceScreenPosition(afxWindow wnd, afxUnit const screenPos[2], afxUnit surfPos[2])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(screenPos);
    AFX_ASSERT(surfPos);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    afxBool rslt = 0;
    surfPos[0] = screenPos[0] - wnd->frameRc.x;
    surfPos[1] = screenPos[1] - wnd->frameRc.y;
    return rslt;
}

_AUX afxBool AfxGetOnScreenSurfacePosition(afxWindow wnd, afxUnit const surfPos[2], afxUnit screenPos[2])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(screenPos);
    AFX_ASSERT(surfPos);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    afxBool rslt = 0;
    screenPos[0] = surfPos[0] + wnd->frameRc.x;
    screenPos[1] = surfPos[1] + wnd->frameRc.y;
    return rslt;
}

_AUX void AfxGetWindowRect(afxWindow wnd, afxRect* frame, afxRect* area)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(frame || area);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    if (frame)
        *frame = wnd->frameRc;

    if (area)
        *area = wnd->areaRc;
}

_AUX afxError AfxAdjustWindow(afxWindow wnd, afxRect const* area)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(area);

    if (wnd->pimpl->adjustCb)
    {
        if (wnd->pimpl->adjustCb(wnd, area))
            AfxThrowError();

        return err;
    }

    afxDesktop* dwm = wnd->dwm;

    if (area)
    {
        afxRect rc2 = *area;
        rc2.x += wnd->frameRc.x;
        rc2.y += wnd->frameRc.y;
        rc2.w = AFX_MAX(1, rc2.w);
        rc2.h = AFX_MAX(1, rc2.h);

        if ((wnd->areaRc.w != rc2.w) ||
            (wnd->areaRc.h != rc2.h) ||
            (wnd->frameRc.x != rc2.x) ||
            (wnd->frameRc.y != rc2.y))
        {
            AFX_ASSERT2(rc2.w, rc2.h);
            wnd->areaRc.w = rc2.w;
            wnd->areaRc.h = rc2.h;
            wnd->frameRc.w = wnd->areaRc.w + wnd->marginR + wnd->marginL;
            wnd->frameRc.h = wnd->areaRc.h + wnd->marginB + wnd->marginT;
            wnd->frameRc.x = rc2.x;
            wnd->frameRc.y = rc2.y;
        }
    }

    afxRect whd;
    afxSurface dout = wnd->dout;
    if (dout)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
        AvxGetSurfaceArea(dout, &whd);
        whd.w = wnd->areaRc.w;
        whd.h = wnd->areaRc.h;

        if (AvxAdjustSurface(dout, &whd, wnd->fullscreen))
            AfxThrowError();
    }
    return err;
}

_AUX afxError _AuxAdjustWindowFromNdc(afxWindow wnd, afxV2d const offset, afxV2d const extent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(extent);

    afxDesktop* dwm = wnd->dwm;
    afxRect rc = { 0 };

    if (offset)
    {
        rc.x = (afxUnit)AfxUnndcf(offset[0], dwm->res.x - 1);
        rc.y = (afxUnit)AfxUnndcf(offset[1], dwm->res.y - 1);
    }

    AFX_ASSERT(extent);
    rc.w = (afxUnit)AfxUnndcf(extent[0], dwm->res.w);
    rc.h = (afxUnit)AfxUnndcf(extent[1], dwm->res.h);

    if (AfxAdjustWindow(wnd, &rc))
        AfxThrowError();

    return err;
}

_AUX afxError _AuxAdjustWindowSurfaceFromNdc(afxWindow wnd, afxV2d const offset, afxV2d const extent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxDesktop* dwm = wnd->dwm;
    afxRect rc = { 0 };

    if (offset)
    {
        rc.x = (afxUnit)AfxUnndcf(offset[0], wnd->frameRc.w - 1);
        rc.y = (afxUnit)AfxUnndcf(offset[1], wnd->frameRc.h - 1);
    }

    AFX_ASSERT(extent);
    rc.w = (afxUnit)AfxUnndcf(extent[0], dwm->res.w);
    rc.h = (afxUnit)AfxUnndcf(extent[1], dwm->res.h);

    if (AfxAdjustWindow(wnd, &rc))
        AfxThrowError();

    return err;
}

_AUX afxBool AFX_WND_EVENT_HANDLER(afxWindow wnd, auxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    switch (ev->id)
    {
    case auxEventId_KEY:
    {
        if (AfxWasKeyPressed(0, afxKey_ESC))
        {
            if (wnd->cursConfined)
            {
                
            }
        }
        else if (AfxWasKeyPressed(0, afxKey_LALT) || AfxWasKeyPressed(0, afxKey_RALT))
        {
            if (AfxWasKeyPressed(0, afxKey_PRINT))
            {
                if (wnd->dout)
                {
                    afxUri2048 uri;
                    AfxMakeUri2048(&uri, NIL);
                    AfxFormatUri(&uri.uri, "../tmp/ss-wnd%u-%u.tga", AfxGetObjectId((void*)wnd), AfxGetTimer());
                    AvxPrintSurfaceBuffer(wnd->dout, 0, NIL, &uri.uri, 0);
                }
            }
            else if (AfxWasKeyPressed(0, afxKey_F11))
            {
                AfxTakeFullscreen(wnd, !wnd->fullscreen);
            }
        }
        break;
    }
    }
    return TRUE;
}

_AUX afxError _AuxWndDtorCb(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AfxDeregisterChainedClasses(&wnd->classes);

    //AfxDisposeObjects(1, &wnd->m.dout);

    AfxChangeWindowIcon(wnd, NIL, NIL); // detach any custom icon
    AfxChangeWindowCursor(wnd, NIL, NIL, 0, 0); // detach any custom icon

    return err;
}

_AUX afxError _AuxWndCtorCb(afxWindow wnd, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxEnvironment env = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    afxWindowConfig const* cfg = (afxWindowConfig const*)(args[1]) + invokeNo;
    afxClassConfig const* widClsCfg = args[2];

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }
    
    wnd->pimpl = &_AUX_WND_IMPL;
    wnd->dwm = &env->dwm;

    wnd->alwaysOnTop = FALSE;
    wnd->active = FALSE;
    wnd->focused = FALSE;
    AfxV2dZero(wnd->cursPos);
    AfxV2dZero(wnd->cursMove);
    AfxV2dZero(wnd->cursPosNdc);
    AfxV2dZero(wnd->cursMoveNdc);

    AfxGetClock(&wnd->startClock);
    wnd->lastClock = wnd->startClock;

    AfxV2dZero(wnd->grabPoint);
    AfxV2dZero(wnd->hoveredPoint);

    wnd->grabbedWidg = NIL;
    wnd->hoveredWidg = NIL;
    wnd->focusedWidg = NIL;

    AfxMakeChain(&wnd->classes, wnd);

    AfxMakeString2048(&wnd->title, &AFX_STRING("Multimedia UX Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build"));

    afxBool fullscreen = FALSE;
    afxBool decorated = TRUE;
    afxBool resizable = TRUE;
    afxBool floating = FALSE;

    wnd->decorated = decorated;
    wnd->floating = floating;
    wnd->focused = FALSE;
    wnd->resizable = resizable;

    wnd->frameRcMax = AFX_RECT_MAX;
    wnd->frameRcMin = AFX_RECT_MIN;

    afxClassConfig clsCfg;
    clsCfg = widClsCfg ? *widClsCfg : _AUX_WID_CLASS_CONFIG;
    AfxMountClass(&wnd->widCls, NIL, &wnd->classes, &clsCfg);
    
    wnd->udd = cfg->udd;

    AfxResetEventHandler(wnd, (void*)(cfg->eventCb ? cfg->eventCb : AFX_WND_EVENT_HANDLER));

    return err;
}

_AUX afxClassConfig const _AUX_WND_CLASS_CONFIG =
{
    .fcc = afxFcc_WND,
    .name = "Window",
    .desc = "Desktop Environment Window",
    .fixedSiz = sizeof(AFX_OBJECT(afxWindow)),
    .ctor = (void*)_AuxWndCtorCb,
    .dtor = (void*)_AuxWndDtorCb,
    .event = (void*)AFX_WND_EVENT_HANDLER
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxConfigureWindow(afxEnvironment env, afxWindowConfig* cfg, afxV2d const origin, afxV2d const extent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cfg);
#if 0
    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
#endif
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    afxWindowConfig cfg2 = { 0 };
    cfg2 = *cfg;

    if (!cfg2.dsys)
        AfxGetEnvironmentVideo(&cfg2.dsys);

    cfg2.eventCb = AFX_WND_EVENT_HANDLER;

    afxDesktop* dwm = &env->dwm;
    afxRect rc = { .w = cfg2.dout.ccfg.whd.w, .h = cfg2.dout.ccfg.whd.h };

    if (origin)
    {
        rc.x = (afxUnit)AfxUnndcf(origin[0], dwm->res.x - 1);
        rc.y = (afxUnit)AfxUnndcf(origin[1], dwm->res.y - 1);
    }

    if (extent)
    {
        rc.w = (afxUnit)AfxUnndcf(extent[0], dwm->res.w);
        rc.h = (afxUnit)AfxUnndcf(extent[1], dwm->res.h);
    }
    else
    {
        rc.w = (afxUnit)dwm->res.w / 2;
        rc.h = (afxUnit)dwm->res.h / 2;
    }

    cfg2.x = rc.x;
    cfg2.y = rc.y;
    cfg2.dout.ccfg.whd.w = rc.w;
    cfg2.dout.ccfg.whd.h = rc.h;

    AvxConfigureSurface(cfg2.dsys, &cfg2.dout);
    cfg2.dout.doNotClip = FALSE;
    //cfg2.dout.presentAlpha = avxVideoAlpha_PREMUL;

    *cfg = cfg2;

    return err;
}

_AUX afxError AfxAcquireWindow(afxEnvironment env, afxWindowConfig const* cfg, afxWindow* window)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(window);
    AFX_ASSERT(cfg);
#if 0
    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
#endif
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AuxEnvGetWndClass(env);
    AFX_ASSERT_CLASS(cls, afxFcc_WND);

    afxWindow wnd;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&wnd, (void const*[]) { env, cfg, (void*)&_AUX_WID_CLASS_CONFIG }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(window);
    *window = wnd;

    return err;
}
