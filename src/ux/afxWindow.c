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

_AUX afxClass const* _AuxWndGetWidgetClass(afxWindow wnd)
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

    afxDrawSystem dsys = AvxGetDrawOutputContext(wnd->dout ? wnd->dout : wnd->frameDout);
    
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

_AUX afxError AfxRedrawWindow(afxWindow wnd, afxRect const* rc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    afxError(*redrawCb)(afxWindow, afxRect const*) = wnd->pimpl->redrawCb;

    if (redrawCb)
    {
        if (redrawCb(wnd, rc))
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

    afxClass const* cls = _AuxWndGetWidgetClass(wnd);

    afxUnit i = 0;
    afxWidget wid;
    while (AfxEnumerateObjects(cls, i, 1, (afxObject*)&wid))
    {
        AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);
        afxWhd whd;
        whd.w = wnd->surfaceRect.w;
        whd.h = wnd->surfaceRect.h;
        whd.d = 1;
        AfxDrawWidget(wid, whd, dctx);
        i++;
    }
    return err;
}

_AUX afxBool AfxGetWindowDrawOutput(afxWindow wnd, afxBool frame, afxDrawOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(output);
    afxDrawOutput dout;
    afxBool rslt = 0;

    if (frame)
    {
        if ((dout = wnd->frameDout))
        {
            AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
            ++rslt;
        }
        *output = dout;
    }
    else
    {
        if ((dout = wnd->dout))
        {
            AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
            ++rslt;
        }
        *output = dout;
    }
    return rslt;
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

_AUX afxUnit AfxFormatWindowTitle(afxWindow wnd, afxChar const* fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    AFX_ASSERT(fmt);
    va_list va;
    va_start(va, fmt);
    afxUnit len = AfxFormatStringArg(&wnd->title.str, fmt, va);
    wnd->redrawFrameRequested = TRUE;
    va_end(va);

    return len;
}

_AUX void AfxGetWindowRect(afxWindow wnd, afxBool frame, afxRect* rc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(frame || rc);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    if (frame)
        *rc = wnd->frameRect;
    else
        *rc = wnd->surfaceRect;
}

_AUX afxBool AfxTraceScreenToSurface(afxWindow wnd, afxUnit const screenPos[2], afxUnit surfPos[2])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(screenPos);
    AFX_ASSERT(surfPos);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    afxBool rslt = 0;
    surfPos[0] = screenPos[0] - wnd->frameRect.x;
    surfPos[1] = screenPos[1] - wnd->frameRect.y;
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
    screenPos[0] = surfPos[0] + wnd->frameRect.x;
    screenPos[1] = surfPos[1] + wnd->frameRect.y;
    return rslt;
}

_AUX afxBool AfxMoveWindow(afxWindow wnd, afxUnit const pos[2])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(pos);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));
    
    afxBool(*moveCb)(afxWindow, afxUnit const[2]) = wnd->pimpl->moveCb;

    if (moveCb)
    {
        if (moveCb(wnd, pos))
            AfxThrowError();
        return err;
    }

    wnd->frameRect.x = pos[0];
    wnd->frameRect.y = pos[1];

    return !err;
}

_AUX afxError AfxAdjustWindow(afxWindow wnd, afxBool frame, afxRect const* rc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    afxDesktop* dwm = wnd->dwm;

    afxError(*adjustCb)(afxWindow, afxBool, afxRect const*) = wnd->pimpl->adjustCb;

    if (adjustCb)
    {
        if (adjustCb(wnd, frame, rc))
            AfxThrowError();

        return err;
    }

    if (frame)
    {
        afxRect rc2;
        rc2.x = AfxMinu(rc->x, dwm->res.x - 1);
        rc2.y = AfxMinu(rc->y, dwm->res.y - 1);
        rc2.w = AfxMax(1, AfxMin(rc->w, dwm->res.w));
        rc2.h = AfxMax(1, AfxMin(rc->h, dwm->res.y));

        if ((wnd->frameRect.x != rc2.x) ||
            (wnd->frameRect.y != rc2.y) ||
            (wnd->frameRect.w != rc2.w) ||
            (wnd->frameRect.h != rc2.h))
        {

            AFX_ASSERT2(rc2.w, rc2.h);
            wnd->frameRect = rc2;

            wnd->surfaceRect.x = wnd->marginL;
            wnd->surfaceRect.y = wnd->marginT;
            wnd->surfaceRect.w = AfxMin(AfxMax(1, rc2.w - wnd->marginR - wnd->marginL), wnd->frameRect.w);
            wnd->surfaceRect.h = AfxMin(AfxMax(1, rc2.h - wnd->marginB - wnd->marginT), wnd->frameRect.h);

            //if (!SetWindowPos(wnd->hWnd, NULL, 0, 0, wnd->frameRect.w, wnd->frameRect.h, SWP_NOZORDER))
                //  AfxThrowError();

            avxRange whd;
            afxDrawOutput dout = wnd->dout;
            AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
            whd;
            AvxQueryDrawOutputExtent(dout, &whd, NIL);
            whd.w = wnd->surfaceRect.w;
            whd.h = wnd->surfaceRect.h;

            if (AvxAdjustDrawOutput(dout, whd))
                AfxThrowError();
        }
    }
    else
    {
        AFX_ASSERT2(wnd->frameRect.w > (afxUnit)rc->x, wnd->frameRect.h > (afxUnit)rc->y);
        //AFX_ASSERT4(surface->w, wnd->m.frameRect.w > (afxUnit)area->w, surface->h, wnd->m.frameRect.h > (afxUnit)surface->h);

        afxRect rc2;
        rc2.x = AfxMinu(rc->x, wnd->frameRect.w - 1);
        rc2.y = AfxMinu(rc->y, wnd->frameRect.h - 1);
        rc2.w = AfxMax(1, rc->w/*AfxMin(surface->w, wnd->m.frameRect.w)*/);
        rc2.h = AfxMax(1, rc->h/*AfxMin(surface->h, wnd->m.frameRect.h)*/);

        if ((wnd->surfaceRect.x != rc2.x) ||
            (wnd->surfaceRect.y != rc2.y) ||
            (wnd->surfaceRect.w != rc2.w) ||
            (wnd->surfaceRect.h != rc2.h))
        {
            afxInt32 extraWndWidth = 0, extraWndHeight = 0;
            //CalcWindowValuesW32(wnd->hWnd, &extraWndWidth, &extraWndHeight);

            AFX_ASSERT2(rc2.w, rc2.h);
            wnd->frameRect.w = rc2.w + extraWndWidth;
            wnd->frameRect.h = rc2.h + extraWndHeight;
            wnd->surfaceRect = rc2;

            //if (!SetWindowPos(wnd->hWnd, NULL, 0, 0, wnd->m.frameRect.w, wnd->m.frameRect.h, SWP_NOMOVE | SWP_NOZORDER))
                //AfxThrowError();

            afxDrawOutput dout = wnd->dout;

            if (dout)
            {
                AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

                avxRange whd;
                AvxQueryDrawOutputExtent(dout, &whd, NIL);
                whd.w = wnd->surfaceRect.w;
                whd.h = wnd->surfaceRect.h;

                if (AvxAdjustDrawOutput(dout, whd))
                    AfxThrowError();
            }
        }
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
        rc.x = (afxUnit)AfxUnndcf(offset[0], wnd->frameRect.w - 1);
        rc.y = (afxUnit)AfxUnndcf(offset[1], wnd->frameRect.h - 1);
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
            if (wnd->cursorConfined)
            {
                
            }
        }
        else if (AfxWasKeyPressed(0, afxKey_LALT))
        {
            if (AfxWasKeyPressed(0, afxKey_PRINT))
            {
                if (wnd->dout)
                {
                    afxUri2048 uri;
                    AfxMakeUri2048(&uri, NIL);
                    AfxFormatUri(&uri.uri, "../tmp/ss-wnd%u-%u.tga", AfxGetObjectId((void*)wnd), AfxGetTimer());
                    AvxPrintDrawOutput(wnd->dout, 0, NIL, 0, &uri.uri);
                }
            }
            else if (AfxWasKeyPressed(0, afxKey_F11))
            {
                AfxImmergeWindow(wnd, !wnd->fullscreen);
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

    wnd->active = FALSE;
    wnd->focused = FALSE;
    AfxV2dZero(wnd->cursorPos);
    AfxV2dZero(wnd->cursorMove);
    AfxV2dZero(wnd->cursorPosNdc);
    AfxV2dZero(wnd->cursorMoveNdc);

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

    AvxConfigureDrawOutput(cfg->dsys, &cfg->frame);
    cfg2.frame.doNotClip = FALSE;
    cfg2.frame.presentAlpha = avxVideoAlpha_PREMUL;
    AvxConfigureDrawOutput(cfg->dsys, &cfg->surface);
    cfg2.surface.bufUsage[0] |= avxRasterUsage_DRAW | avxRasterUsage_RESAMPLE;
    cfg2.surface.bufUsage[1] |= avxRasterUsage_DRAW | avxRasterUsage_RESAMPLE;


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

    cfg2.rc = rc;
    cfg2.surface.extent.w = rc.w;
    cfg2.surface.extent.h = rc.h;
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

    afxClass* cls = (afxClass*)_AuxSesGetWndClass(ses);
    AFX_ASSERT_CLASS(cls, afxFcc_WND);
    afxWindow wnd;

    if (!cfg) AfxThrowError();
    else
    {
        if (AfxAcquireObjects(cls, 1, (afxObject*)&wnd, (void const*[]) { ses, cfg, (void*)&_AUX_WID_CLASS_CONFIG })) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
            AFX_ASSERT(window);
            *window = wnd;
        }
    }
    return err;
}
