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
//#define _AUX_SHELL_C
#define _AUX_SESSION_C
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

_AUX afxError AfxChangeWindowIcon(afxWindow wnd, avxRaster icon)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    
    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    afxError(*chIconCb)(afxWindow, avxRaster) = wnd->pimpl->chIconCb;

    if (chIconCb)
    {
        if (chIconCb(wnd, icon))
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
        }
    }
    return err;
}

_AUX afxError AfxLoadWindowIcon(afxWindow wnd, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(uri);

    afxDrawSystem dsys = AvxGetSurfaceSystem(wnd->dout ? wnd->dout : wnd->frameDout);
    
    avxRasterInfo rasi = { 0 };
    rasi.usage = avxRasterUsage_SRC;
    rasi.flags = avxRasterFlag_2D;
    
    avxRaster ras;
    if (AvxLoadRasters(dsys, 1, &rasi, &uri[0], &ras))
    {
        AfxThrowError();
        return err;
    }

    if (AfxChangeWindowIcon(wnd, ras))
        AfxThrowError();

    AfxDisposeObjects(1, &ras);

    return err;
}

_AUX afxError AfxRedrawWindow(afxWindow wnd, afxRect const* area)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    afxError(*redrawCb)(afxWindow, afxRect const*) = wnd->pimpl->redrawCb;

    if (redrawCb)
    {
        if (redrawCb(wnd, area))
            AfxThrowError();
    }
    else
    {

    }
    return err;
}

_AUX afxError AfxRedrawWidgets(afxWindow wnd, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxClass const* cls = _AuxWndGetWidClass(wnd);

    afxUnit i = 0;
    afxWidget wid;
    while (AfxEnumerateObjects(cls, i, 1, (afxObject*)&wid))
    {
        AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);
        afxWarp whd;
        whd.w = wnd->areaRc.w;
        whd.h = wnd->areaRc.h;
        whd.d = 1;
        AfxDrawWidget(wid, whd, dctx);
        i++;
    }
    return err;
}

_AUX afxMask AfxGetWindowDrawOutput(afxWindow wnd, afxSurface* frame, afxSurface* area)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(frame || area);
    afxMask found = NIL;
    afxSurface dout;

    if (frame)
    {
        *frame = (dout = wnd->frameDout);

        if (dout)
        {
            AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
            found |= AFX_BITMASK(0);
        }
    }
    
    if (area)
    {
        *area = (dout = wnd->dout);
        
        if (dout)
        {
            AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
            found |= AFX_BITMASK(1);
        }
    }

    if (!found)
        err = afxError_NOT_READY;

    return found;
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

_AUX afxError AfxMakeWindowCursory(afxWindow wnd, afxBool cursory, afxRect const* confinement)
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
        if (AfxIntersectRects(&wnd->cursConfinRect, &wnd->frameRc, confinement))
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

    if (wnd->pimpl->titleCb(wnd))
        AfxThrowError();

    return wnd->title.s.len;
}

_AUX afxBool AfxTraceScreenToSurface(afxWindow wnd, afxUnit const screenPos[2], afxUnit surfPos[2])
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

_AUX afxBool AfxTraceSurfaceToScreen(afxWindow wnd, afxUnit const surfPos[2], afxUnit screenPos[2])
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

_AUX afxError AfxAdjustWindow(afxWindow wnd, afxRect const* frame, afxRect const* area)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(frame || area);

    if (wnd->pimpl->adjustCb)
    {
        if (wnd->pimpl->adjustCb(wnd, frame, area))
            AfxThrowError();

        return err;
    }

    afxDesktop* dwm = wnd->dwm;

    if (frame)
    {
        afxRect rc2 = *frame;
        rc2.w = AFX_MAX(1, rc2.w);
        rc2.h = AFX_MAX(1, rc2.h);

        if (!AfxAreRectsEqual(&wnd->frameRc, &rc2))
        {
            AFX_ASSERT2(rc2.w, rc2.h);
            wnd->frameRc = rc2;
            wnd->areaRc.x = wnd->marginL;
            wnd->areaRc.y = wnd->marginT;
            wnd->areaRc.w = wnd->frameRc.w - wnd->marginR - wnd->marginL;
            wnd->areaRc.h = wnd->frameRc.h - wnd->marginB - wnd->marginT;
        }
    }
    
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
    afxSurface dout = wnd->frameDout;
    if (dout)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
        AvxGetSurfaceArea(dout, &whd);
        whd.w = wnd->frameRc.w;
        whd.h = wnd->frameRc.h;

        if (AvxAdjustSurface(dout, &whd, wnd->fullscreen))
            AfxThrowError();
    }

    dout = wnd->dout;
    if (dout && (dout != wnd->frameDout))
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

    if (AfxAdjustWindow(wnd, FALSE, &rc))
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

    if (AfxAdjustWindow(wnd, NIL, &rc))
        AfxThrowError();

    return err;
}

_AUX afxBool _AuxWndStdEventCb(afxWindow wnd, auxEvent *ev)
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
                AfxMakeWindowExclusive(wnd, !wnd->fullscreen);
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

    AfxChangeWindowIcon(wnd, NIL); // detach any custom icon

    return err;
}

_AUX afxError _AuxWndCtorCb(afxWindow wnd, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxSession ses = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    afxWindowConfig const* cfg = (afxWindowConfig const*)(args[1]) + invokeNo;
    afxClassConfig const* widClsCfg = args[2];

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }
    
    wnd->pimpl = &_AUX_WND_IMPL;
    wnd->dwm = &ses->dwm;

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

    AfxDeployChain(&wnd->classes, wnd);

    AfxMakeString2048(&wnd->title, &AFX_STRING("Multimedia UX Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build"));

    afxBool fullscreen = FALSE;
    afxBool decorated = TRUE;
    afxBool resizable = TRUE;
    afxBool floating = FALSE;

    wnd->decorated = decorated;
    wnd->floating = floating;
    wnd->focused = FALSE;
    wnd->resizable = resizable;

    wnd->frameRcMax = AVX_RECT_MAX;
    wnd->frameRcMin = AVX_RECT_MIN;

    afxClassConfig clsCfg;
    clsCfg = widClsCfg ? *widClsCfg : _AUX_WID_CLASS_CONFIG;
    AfxMountClass(&wnd->widCls, NIL, &wnd->classes, &clsCfg);
    
    wnd->udd = cfg->udd;

    return err;
}

_AUX afxClassConfig const _AUX_WND_CLASS_CONFIG =
{
    .fcc = afxFcc_WND,
    .name = "Window",
    .desc = "Desktop Window",
    .fixedSiz = sizeof(AFX_OBJECT(afxWindow)),
    .ctor = (void*)_AuxWndCtorCb,
    .dtor = (void*)_AuxWndDtorCb,
    .event = (void*)_AuxWndStdEventCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxConfigureWindow(afxWindowConfig* cfg, afxV2d const origin, afxV2d const extent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cfg);

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    afxDrawSystem dsys = cfg->dsys;
    
    afxWindowConfig cfg2 = { 0 };
    cfg2.dsys = dsys;
    cfg2.eventCb = _AuxWndStdEventCb;

    AvxConfigureSurface(cfg->dsys, &cfg->frame);
    cfg2.frame.doNotClip = FALSE;
    cfg2.frame.presentAlpha = avxVideoAlpha_PREMUL;
    AvxConfigureSurface(cfg->dsys, &cfg->area);
    cfg2.area.canvas.slots[0].usage |= avxRasterUsage_DRAW | avxRasterUsage_TEXTURE;
    cfg2.area.canvas.slots[1].usage |= avxRasterUsage_DRAW | avxRasterUsage_TEXTURE;


    afxDesktop* dwm = &ses->dwm;
    afxRect rc = { 0 };

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

    cfg2.atX = rc.x;
    cfg2.atY = rc.y;
    cfg2.area.canvas.whd.w = rc.w;
    cfg2.area.canvas.whd.h = rc.h;
    *cfg = cfg2;

    return err;
}

_AUX afxError AfxAcquireWindow(afxWindowConfig const* cfg, afxWindow* window)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(window);
    AFX_ASSERT(cfg);

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AuxSesGetWndClass(ses);
    AFX_ASSERT_CLASS(cls, afxFcc_WND);

    afxWindow wnd;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&wnd, (void const*[]) { ses, cfg, (void*)&_AUX_WID_CLASS_CONFIG }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(window);
    *window = wnd;

    return err;
}
