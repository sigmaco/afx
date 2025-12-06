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
#include "auxIcd.h"

_AUX void* AfxGetWindowUdd(afxWindow wnd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    return wnd->udd;
}

_AUX afxClass const* _AuxWndGetWidClass(afxWindow wnd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    afxClass const* cls = &wnd->widCls;
    AFX_ASSERT_CLASS(cls, afxFcc_WID);
    return cls;
}

_AUX afxError _AfxWndChangeIconCb(afxWindow wnd, avxRaster icon, avxRasterRegion const* rgn)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

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

_AUX afxError AfxChangeWindowIcon(afxWindow wnd, avxRaster icon, avxRasterRegion const* rgn)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    if (!wnd->ddi->chIconCb)
    {
        if (afxError_UNSUPPORTED != (err = _AfxWndChangeIconCb(wnd, icon, rgn)))
            AfxThrowError();
    }
    else
    {
        if (afxError_UNSUPPORTED != (err = wnd->ddi->chIconCb(wnd, icon, rgn)))
            AfxThrowError();
    }
    return err;
}

_AUX afxError _AfxWndChangeCursorCb(afxWindow wnd, avxRaster curs, avxRasterRegion const* rgn, afxInt hotspotX, afxInt hotspotY)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

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

_AUX afxError AfxChangeWindowCursor(afxWindow wnd, avxRaster curs, avxRasterRegion const* rgn, afxInt hotspotX, afxInt hotspotY)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    
    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    if (!wnd->ddi->chCursCb)
    {
        if (_AfxWndChangeCursorCb(wnd, curs, rgn, hotspotX, hotspotY))
            AfxThrowError();
    }
    else
    {
        if (wnd->ddi->chCursCb(wnd, curs, rgn, hotspotX, hotspotY))
            AfxThrowError();
    }
    return err;
}

_AUX afxError AfxLoadWindowIcon(afxWindow wnd, afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(uri);

    afxDrawSystem dsys = AvxGetSurfaceHost(wnd->surfaceDout ? wnd->surfaceDout : wnd->frameDout);
    
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

    if (afxError_UNSUPPORTED != (err = AfxChangeWindowIcon(wnd, ras, &rgn)))
        AfxThrowError();

    AfxDisposeObjects(1, &ras);

    return err;
}

_AUX afxError AfxLoadWindowCursor(afxWindow wnd, afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(uri);

    afxDrawSystem dsys = AvxGetSurfaceHost(wnd->surfaceDout ? wnd->surfaceDout : wnd->frameDout);

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

_AUX afxBool _AuxWidRedrawnCb(afxWidget wid, void* udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    struct
    {
        afxRect area;
        afxDrawContext dctx;
    }
    *udd2 = udd;

    AfxDrawWidget(wid, &udd2->area, udd2->dctx);

    return FALSE;
}

_AUX afxError AfxRedrawWidgets(afxWindow wnd, afxRect const* area, afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxClass const* cls = _AuxWndGetWidClass(wnd);

    afxRect area2 = area ? *area : wnd->surfaceRc;

    // Desist if there is not area.
    if (1 >= AfxGetRectArea(&area2))
        return err;

    struct
    {
        afxRect area;
        afxDrawContext dctx;
    }
    udd =
    {
        .area = area2,
        .dctx = dctx
    };

    AfxInvokeObjects(cls, (void*)_AuxWidRedrawnCb, &udd, 0, AFX_U32_MAX);

    return err;
}

_AUX afxError AfxRedrawWindow(afxWindow wnd, afxRect const* area)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    // Modifier operations must be restricted to the owner thread.
    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    if (!wnd->ddi->redrawCb)
    {
        AfxThrowError();
    }
    else
    {
        if (wnd->ddi->redrawCb(wnd, area))
            AfxThrowError();
    }
    return err;
}

_AUX afxError AfxDamageWindow(afxWindow wnd, afxRect const* area)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    // Modifier operations must be restricted to the owner thread.
    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    if (!wnd->ddi->damageCb)
    {
        AfxThrowError();
    }
    else
    {
        if (wnd->ddi->damageCb(wnd, area))
            AfxThrowError();
    }
    return err;
}

_AUX afxError AfxGetWindowSurface(afxWindow wnd, afxSurface* surface)
{
    afxError err = { 0 };
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
        *surface = (dout = wnd->surfaceDout);
        
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
    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    // Modifier operations must be restricted to the owner thread.
    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    wnd->cursHidden = !cursory;

    if (!confinement)
    {
        wnd->cursConfined = FALSE;
    }
    else
    {
        // Possibly replace Union with Intersection if that's more appropriate.
        if (AfxIntersectRects(&wnd->cursConfinRect, &wnd->surfaceRc, confinement))
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

_AUX afxUnit _AfxWndFormatTitleCb(afxWindow wnd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    return 0;
}

_AUX afxUnit AfxFormatWindowTitle(afxWindow wnd, afxChar const* fmt, ...)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    // Modifier operations must be restricted to the owner thread.
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

    if (wnd->ddi->titleCb)
        if (wnd->ddi->titleCb(wnd))
            AfxThrowError();

    return wnd->title.s.len;
}

_AUX afxBool AfxGetOnSurfaceScreenPosition(afxWindow wnd, afxUnit const screenPos[2], afxUnit surfPos[2])
{
    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(screenPos);
    AFX_ASSERT(surfPos);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    afxBool rslt = 0;
    screenPos[0] = surfPos[0] + wnd->frameRc.x;
    screenPos[1] = surfPos[1] + wnd->frameRc.y;
    return rslt;
}


afxRect ResolveSurfaceRect(
    afxRect const* screen,
    afxRect const* desired,
    afxAnchor anchor,
    afxRect const* current)
{
    afxRect out = *desired;

    // If no anchor is specified, just preserve position
    if (anchor == 0)
    {
        out.x = current->x;
        out.y = current->y;
        return out;
    }

    // Compute the anchor points in both rects
    afxInt screenAx = screen->x;
    afxInt screenAy = screen->y;
    afxInt winAx = 0;
    afxInt winAy = 0;

    // Screen anchor point
    if (anchor & afxAnchor_LEFT)
        screenAx = screen->x;
    else if (anchor & afxAnchor_CENTER)
        screenAx = screen->x + screen->w / 2;
    else if (anchor & afxAnchor_RIGHT)
        screenAx = screen->x + screen->w;

    if (anchor & afxAnchor_TOP)
        screenAy = screen->y;
    else if (anchor & afxAnchor_MIDDLE)
        screenAy = screen->y + screen->h / 2;
    else if (anchor & afxAnchor_BOTTOM)
        screenAy = screen->y + screen->h;

    // Window anchor point (relative to window's own rect)
    if (anchor & afxAnchor_LEFT)
        winAx = 0;
    else if (anchor & afxAnchor_CENTER)
        winAx = desired->w / 2;
    else if (anchor & afxAnchor_RIGHT)
        winAx = desired->w;

    if (anchor & afxAnchor_TOP)
        winAy = 0;
    else if (anchor & afxAnchor_MIDDLE)
        winAy = desired->h / 2;
    else if (anchor & afxAnchor_BOTTOM)
        winAy = desired->h;

    // Position the window so its anchor point coincides with the screen's anchor point
    out.x = screenAx - winAx + desired->x;
    out.y = screenAy - winAy + desired->y;

    return out;
}


_AUX afxBool AfxGetWindowRect(afxWindow wnd, afxAnchor anchor, afxRect* surface)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(surface);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    afxRect rect = { 0, 0, 0, 0 };

    // Get client area size and position.

    afxRect rcClient = wnd->surfaceRc;

    afxInt origin[2] = { wnd->frameRc.x, wnd->frameRc.y };
    
    rect.w = rcClient.w - rcClient.x;
    rect.h = rcClient.h - rcClient.y;

    // Compute anchor-relative position.

    rect.x = origin[0];
    rect.y = origin[1];

    // Adjust coordinates according to anchor.
    if (anchor & afxAnchor_CENTER)
        rect.x += rect.w / 2;
    else if (anchor & afxAnchor_RIGHT)
        rect.x += rect.w;

    if (anchor & afxAnchor_MIDDLE)
        rect.y += rect.h / 2;
    else if (anchor & afxAnchor_BOTTOM)
        rect.y += rect.h;

    // This way, rect.x, rect.y represent the anchor point in screen space.

    if (anchor == NIL)
    {
        *surface = wnd->surfaceRc;
        return 1;
    }
    else
    {
        rect = wnd->surfaceRc;

        // Compute the anchor points in both rects
        afxInt screenAx = wnd->screenRc.x;
        afxInt screenAy = wnd->screenRc.y;
        afxInt winAx = 0;
        afxInt winAy = 0;

        if (!(anchor & (afxAnchor_LEFT | afxAnchor_CENTER | afxAnchor_RIGHT)))
        {
            screenAx = 0;
        }

        if (!(anchor & (afxAnchor_TOP | afxAnchor_MIDDLE | afxAnchor_BOTTOM)))
        {
            screenAy = 0;
        }

        if (anchor & (afxAnchor_LEFT | afxAnchor_CENTER | afxAnchor_RIGHT))
        {
            // Screen anchor point
            if (anchor & afxAnchor_LEFT)
                screenAx = wnd->screenRc.x;
            else if (anchor & afxAnchor_CENTER)
                screenAx = wnd->screenRc.x + wnd->screenRc.w / 2;
            else if (anchor & afxAnchor_RIGHT)
                screenAx = wnd->screenRc.x + wnd->screenRc.w;
        }

        if (anchor & (afxAnchor_TOP | afxAnchor_MIDDLE | afxAnchor_BOTTOM))
        {
            if (anchor & afxAnchor_TOP)
                screenAy = wnd->screenRc.y;
            else if (anchor & afxAnchor_MIDDLE)
                screenAy = wnd->screenRc.y + wnd->screenRc.h / 2;
            else if (anchor & afxAnchor_BOTTOM)
                screenAy = wnd->screenRc.y + wnd->screenRc.h;
        }

        if (anchor & (afxAnchor_LEFT | afxAnchor_CENTER | afxAnchor_RIGHT))
        {
            // Window anchor point (relative to window's own rect)
            if (anchor & afxAnchor_LEFT)
                winAx = 0;
            else if (anchor & afxAnchor_CENTER)
                winAx = wnd->frameRc.w / 2;
            else if (anchor & afxAnchor_RIGHT)
                winAx = wnd->frameRc.w;
        }

        if (anchor & (afxAnchor_TOP | afxAnchor_MIDDLE | afxAnchor_BOTTOM))
        {
            if (anchor & afxAnchor_TOP)
                winAy = 0;
            else if (anchor & afxAnchor_MIDDLE)
                winAy = wnd->frameRc.h / 2;
            else if (anchor & afxAnchor_BOTTOM)
                winAy = wnd->frameRc.h;
        }

        // Position the window so its anchor point coincides with the screen's anchor point
        rect.x = screenAx - winAx /*+ wnd->frameRc.x*/;
        rect.y = screenAy - winAy /*+ wnd->frameRc.y*/;

        if ((anchor & (afxAnchor_LEFT | afxAnchor_CENTER | afxAnchor_RIGHT)))
        {
            rect.x += wnd->frameRc.x;
        }

        if ((anchor & (afxAnchor_TOP | afxAnchor_MIDDLE | afxAnchor_BOTTOM)))
        {
            rect.y += wnd->frameRc.y;
        }

    }

    //rect = AfxResolveSurfaceRect(&wnd->screenRc, &wnd->surfaceRc, anchor, &wnd->frameRc);

    AFX_ASSERT(surface);
    *surface = rect;
    return 1;    
}

_AUX afxError AfxResetWindowBounds(afxWindow wnd, afxRect const* min, afxRect const* max)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    
    if (min)
        wnd->frameRcMin = *min;

    if (max)
        wnd->frameRcMax = *max;
}

_AUX afxError _AfxWndAdjustCb(afxWindow wnd, afxAnchor anchor, afxRect* c)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    if (c)
    {
        afxRect rc2 = *c;
        rc2.x += wnd->frameRc.x;
        rc2.y += wnd->frameRc.y;
        rc2.w = AFX_MAX(1, rc2.w);
        rc2.h = AFX_MAX(1, rc2.h);

        if ((wnd->surfaceRc.w != rc2.w) || (wnd->surfaceRc.h != rc2.h) ||
            (wnd->frameRc.x != rc2.x) || (wnd->frameRc.y != rc2.y))
        {
            AFX_ASSERT2(rc2.w, rc2.h);
            wnd->surfaceRc.w = rc2.w;
            wnd->surfaceRc.h = rc2.h;
            wnd->frameRc.w = wnd->surfaceRc.w + wnd->frameMarginR + wnd->frameMarginL;
            wnd->frameRc.h = wnd->surfaceRc.h + wnd->frameMarginB + wnd->frameMarginT;
            wnd->frameRc.x = rc2.x;
            wnd->frameRc.y = rc2.y;
        }
    }
#if 0
    afxRect whd;
    afxSurface dout = wnd->surfaceDout;

    if (dout)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
        AvxGetSurfaceArea(dout, &whd);
        whd.w = wnd->surfaceRc.w;
        whd.h = wnd->surfaceRc.h;

        if (AvxAdjustSurface(dout, &whd, wnd->fullscreen))
            AfxThrowError();
    }
#endif
    return err;
}

_AUX afxError AfxAdjustWindow(afxWindow wnd, afxDisplay disp, afxUnit dport, afxAnchor anchor, afxRect const* surface)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_VDU, 1, &disp);
    AFX_ASSERT(surface);

    // Modifier operations must be restricted to the owner thread.
    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    if (wnd->adjusting) return afxError_BUSY;
    // Lock readjustment to prevent recursion.
    wnd->adjusting = TRUE;

    afxRect src = *surface;

    // Call implementation to do stuff on host platform and potentially readjust the surface rectangle.
    AFX_ASSERT(wnd->ddi->adjustCb);    
    if (wnd->ddi->adjustCb(wnd, anchor, &src))
        AfxThrowError();

    AFX_ASSERT(AfxDoesRectContain(&wnd->frameRc, &wnd->surfaceRc));

    afxSurface frameDout = wnd->frameDout;
    afxSurface surfaceDout = wnd->surfaceDout;

    if (frameDout)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &frameDout);

        afxRect warea;
        AvxGetSurfaceArea(frameDout, &warea);
        warea.w = wnd->frameRc.w;
        warea.h = wnd->frameRc.h;

        if (AvxAdjustSurface(frameDout, &warea, wnd->fullscreen))
            AfxThrowError();
    }

    if (surfaceDout)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &surfaceDout);
        afxRect area;
        AfxGetWindowRect(wnd, /*wnd->anchor*/NIL, &area);
        area.w = wnd->surfaceRc.w;
        area.h = wnd->surfaceRc.h;

        if (AvxAdjustSurface(surfaceDout, &area, wnd->fullscreen))
            AfxThrowError();
    }

    auxEvent ev = { 0 };
    ev.ev.id = afxEvent_UX;
    ev.id = auxEventId_SIZE;
    ev.wnd = wnd;
    AfxEmitEvent(wnd, &ev.ev);

    if (anchor)
    {
        ev.id = auxEventId_MOVE;
        AfxEmitEvent(wnd, &ev.ev);
    }

    // Unlock readjustment.
    wnd->adjusting = FALSE;

    return err;
}

_AUX afxError _AuxAdjustWindowFromNdc(afxWindow wnd, afxDisplay disp, afxUnit dport, afxAnchor anchor, afxV2d const offset, afxV2d const extent)
{
    afxError err = { 0 };
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

    if (AfxAdjustWindow(wnd, disp, dport, anchor, &rc))
        AfxThrowError();

    return err;
}

_AUX afxError _AuxAdjustWindowSurfaceFromNdc(afxWindow wnd, afxDisplay disp, afxUnit dport, afxAnchor anchor, afxV2d const offset, afxV2d const extent)
{
    afxError err = { 0 };
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

    if (AfxAdjustWindow(wnd, disp, dport, anchor, &rc))
        AfxThrowError();

    return err;
}

_AUX afxBool _AuxWidEmitEventCb(afxWidget wid, auxEvent *ev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    

    auxEvent ev2 = *ev;
    AfxEmitEvent(wid, &ev2.ev);

    return FALSE;
}

_AUX afxBool AFX_WND_EVENT_HANDLER(afxWindow wnd, auxEvent *ev)
{
    afxError err = { 0 };
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
        
        if (AfxIsKeyPressed(0, afxKey_LALT) || AfxIsKeyPressed(0, afxKey_RALT))
        {
            if (AfxWasKeyPressed(0, afxKey_PRINT))
            {
                if (wnd->surfaceDout)
                {
                    afxUri2048 uri;
                    AfxMakeUri2048(&uri, NIL);
                    AfxFormatUri(&uri.uri, "../tmp/ss-wnd%u-%u.tga", AfxGetObjectId((void*)wnd), AfxGetTimer());
                    AvxPrintSurfaceBuffer(wnd->surfaceDout, 0, NIL, &uri.uri, 0);
                }
            }
            else if (AfxWasKeyPressed(0, afxKey_F11))
            {
                AfxTakeFullscreen(wnd, !wnd->fullscreen);
            }
        }
        break;
    }
    case auxEventId_CLOSE:
    {
        break;
    }
    case auxEventId_MOVE:
    {
        break;
    }
    case auxEventId_SIZE:
    {
        afxSurface dout = wnd->surfaceDout;

        if (dout)
        {
            AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
            afxRect whd = wnd->surfaceRc;
            if (AvxAdjustSurface(dout, &whd, wnd->fullscreen))
                AfxThrowError();
        }
        break;
    }
    case auxEventId_PAINT:
    {
        AfxRedrawWindow(wnd, NIL);
        break;
    }
    }

    AfxInvokeObjects(&wnd->widCls, (void*)_AuxWidEmitEventCb, ev, 0, AFX_U32_MAX);

    return TRUE;
}

_AUX _auxDdiWnd const _AUX_DDI_WND =
{
    .adjustCb = _AfxWndAdjustCb,
    .titleCb = _AfxWndFormatTitleCb
};

_AUX afxError _AuxWndDtorCb(afxWindow wnd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AfxDeregisterChainedClasses(&wnd->classes);

    //AfxDisposeObjects(1, &wnd->m.dout);

    AfxChangeWindowIcon(wnd, NIL, NIL); // detach any custom icon
    AfxChangeWindowCursor(wnd, NIL, NIL, 0, 0); // detach any custom icon

    return err;
}

_AUX afxError _AuxWndCtorCb(afxWindow wnd, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
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
    
    wnd->ddi = &_AUX_DDI_WND;
    wnd->dwm = &env->dwm;

    wnd->anchor = cfg->anchor;

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

    if (cfg->title.len)
        AfxMakeString512(&wnd->title, &cfg->title);
    else
        AfxMakeString512(&wnd->title, &AFX_STRING("Multimedia UX Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build"));

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

    AfxInstallEventHandler(wnd, (void*)(cfg->eventCb ? cfg->eventCb : AFX_WND_EVENT_HANDLER));

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
    afxError err = { 0 };
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
    afxError err = { 0 };
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
