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
//#define _AUX_SHELL_C
#define _AUX_SESSION_C
#define _AUX_WINDOW_C
#include "../dev/AuxImplKit.h"

_AUX afxError AfxChangeWindowIcon(afxWindow wnd, afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    
    if (wnd->chIconCb && wnd->chIconCb(wnd, ras))
        AfxThrowError();

    return err;
}

_AUX afxError AfxLoadWindowIcon(afxWindow wnd, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(uri);
    afxDrawContext dctx;

    if (!AfxGetDrawOutputContext(wnd->dout ? wnd->dout : wnd->frameDout, &dctx)) AfxThrowError();
    else
    {
        afxRaster ras;

        if (AfxLoadRasters(dctx, afxRasterUsage_SRC, afxRasterFlag_2D, 1, uri, &ras)) AfxThrowError();
        else
        {
            if (AfxChangeWindowIcon(wnd, ras))
                AfxThrowError();

            AfxReleaseObjects(1, &ras);
        }
    }
    return err;
}

_AUX afxError AfxRedrawWindow(afxWindow wnd, afxRect const* rc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    
    if (wnd->redrawCb && wnd->redrawCb(wnd, rc))
        AfxThrowError();
    
    return err;
}

_AUX afxBool AfxGetWindowDrawOutput(afxWindow wnd, afxDrawOutput* frame, afxDrawOutput* surface)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(frame || surface);
    afxDrawOutput dout;
    afxBool rslt = 0;

    if (frame)
    {
        if ((dout = wnd->frameDout))
        {
            AfxAssertObjects(1, &dout, afxFcc_DOUT);
            ++rslt;
        }
        *frame = dout;
    }

    if (surface)
    {
        if ((dout = wnd->dout))
        {
            AfxAssertObjects(1, &dout, afxFcc_DOUT);
            ++rslt;
        }
        *surface = dout;
    }
    return rslt;
}

_AUX void AfxStepWindow(afxWindow wnd, afxReal64* ct, afxReal64* dt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxClock currClock;
    AfxGetClock(&currClock);
    AfxAssert(ct);
    *ct = AfxGetSecondsElapsed(&wnd->startClock, &currClock);
    AfxAssert(dt);
    *dt = AfxGetSecondsElapsed(&wnd->lastClock, &currClock);
    wnd->lastClock = currClock;
}

_AUX afxNat AfxFormatWindowCaption(afxWindow wnd, afxChar const* fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(fmt);
    va_list va;
    va_start(va, fmt);
    afxNat len = AfxFormatStringArg(&wnd->caption.str, fmt, va);
    wnd->redrawFrameRequested = TRUE;
    va_end(va);
    return len;
}

_AUX void AfxGetWindowRect(afxWindow wnd, afxRect* frame, afxRect* surface)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(frame || surface);

    if (frame)
        *frame = wnd->frameRect;

    if (surface)
        *surface = wnd->surfaceRect;
}

_AUX afxBool AfxTraceScreenToSurface(afxWindow wnd, afxNat const screenPos[2], afxNat surfPos[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(screenPos);
    AfxAssert(surfPos);
    afxBool rslt = 0;
    surfPos[0] = screenPos[0] - wnd->frameRect.x;
    surfPos[1] = screenPos[1] - wnd->frameRect.y;
    return rslt;
}

_AUX afxBool AfxTraceSurfaceToScreen(afxWindow wnd, afxNat const surfPos[2], afxNat screenPos[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(screenPos);
    AfxAssert(surfPos);
    afxBool rslt = 0;
    screenPos[0] = surfPos[0] + wnd->frameRect.x;
    screenPos[1] = surfPos[1] + wnd->frameRect.y;
    return rslt;
}

_AUX afxBool AfxMoveWindow(afxWindow wnd, afxNat const pos[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(pos);
    
    if (wnd->moveCb && wnd->moveCb(wnd, pos)) AfxThrowError();
    else
    {
        wnd->frameRect.x = pos[0];
        wnd->frameRect.y = pos[1];
    }
    return !err;
}

_AUX afxError AfxAdjustWindow(afxWindow wnd, afxRect const* frame, afxRect const* surface)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxDesktop* dwm = wnd->dwm;

    if (wnd->adjustCb && wnd->adjustCb(wnd, frame, surface)) AfxThrowError();
    else
    {
        if (frame)
        {
            AfxAssert3(frame, frame->w, frame->h);

            afxRect rc2;
            rc2.x = AfxMinu(frame->x, dwm->res[0] - 1);
            rc2.y = AfxMinu(frame->y, dwm->res[1] - 1);
            rc2.w = AfxMax(1, AfxMin(frame->w, dwm->res[0]));
            rc2.h = AfxMax(1, AfxMin(frame->h, dwm->res[1]));

            if ((wnd->frameRect.x != rc2.x) ||
                (wnd->frameRect.y != rc2.y) ||
                (wnd->frameRect.w != rc2.w) ||
                (wnd->frameRect.h != rc2.h))
            {

                AfxAssert2(rc2.w, rc2.h);
                wnd->frameRect = rc2;

                wnd->surfaceRect.x = wnd->marginL;
                wnd->surfaceRect.y = wnd->marginT;
                wnd->surfaceRect.w = AfxMin(AfxMax(1, rc2.w - wnd->marginR - wnd->marginL), wnd->frameRect.w);
                wnd->surfaceRect.h = AfxMin(AfxMax(1, rc2.h - wnd->marginB - wnd->marginT), wnd->frameRect.h);

                //if (!SetWindowPos(wnd->hWnd, NULL, 0, 0, wnd->frameRect.w, wnd->frameRect.h, SWP_NOZORDER))
                  //  AfxThrowError();

                afxWhd whd;
                afxDrawOutput dout = wnd->dout;
                AfxAssertObjects(1, &dout, afxFcc_DOUT);
                AfxGetDrawOutputExtent(dout, whd);
                whd[0] = wnd->surfaceRect.w;
                whd[1] = wnd->surfaceRect.h;

                if (AfxAdjustDrawOutput(dout, whd))
                    AfxThrowError();
            }
        }

        if (surface)
        {
            AfxAssert2(wnd->frameRect.w > (afxNat)surface->x, wnd->frameRect.h > (afxNat)surface->y);
            //AfxAssert4(surface->w, wnd->m.frameRect.w > (afxNat)area->w, surface->h, wnd->m.frameRect.h > (afxNat)surface->h);

            afxRect rc2;
            rc2.x = AfxMinu(surface->x, wnd->frameRect.w - 1);
            rc2.y = AfxMinu(surface->y, wnd->frameRect.h - 1);
            rc2.w = AfxMax(1, surface->w/*AfxMin(surface->w, wnd->m.frameRect.w)*/);
            rc2.h = AfxMax(1, surface->h/*AfxMin(surface->h, wnd->m.frameRect.h)*/);

            if ((wnd->surfaceRect.x != rc2.x) ||
                (wnd->surfaceRect.y != rc2.y) ||
                (wnd->surfaceRect.w != rc2.w) ||
                (wnd->surfaceRect.h != rc2.h))
            {
                afxInt32 extraWndWidth = 0, extraWndHeight = 0;
                //CalcWindowValuesW32(wnd->hWnd, &extraWndWidth, &extraWndHeight);

                AfxAssert2(rc2.w, rc2.h);
                wnd->frameRect.w = rc2.w + extraWndWidth;
                wnd->frameRect.h = rc2.h + extraWndHeight;
                wnd->surfaceRect = rc2;

                //if (!SetWindowPos(wnd->hWnd, NULL, 0, 0, wnd->m.frameRect.w, wnd->m.frameRect.h, SWP_NOMOVE | SWP_NOZORDER))
                    //AfxThrowError();

                afxDrawOutput dout = wnd->dout;

                if (dout)
                {
                    AfxAssertObjects(1, &dout, afxFcc_DOUT);

                    afxWhd whd;
                    AfxGetDrawOutputExtent(dout, whd);
                    whd[0] = wnd->surfaceRect.w;
                    whd[1] = wnd->surfaceRect.h;

                    if (AfxAdjustDrawOutput(dout, whd))
                        AfxThrowError();
                }
            }
        }
    }
    return err;
}

_AUX afxError AfxAdjustWindowFromNdc(afxWindow wnd, afxV2d const offset, afxV2d const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(extent);

    afxDesktop* dwm = wnd->dwm;
    afxRect rc = { 0 };

    if (offset)
    {
        rc.x = (afxNat)AfxUnndcf(offset[0], dwm->res[0] - 1);
        rc.y = (afxNat)AfxUnndcf(offset[1], dwm->res[1] - 1);
    }

    AfxAssert(extent);
    rc.w = (afxNat)AfxUnndcf(extent[0], dwm->res[0]);
    rc.h = (afxNat)AfxUnndcf(extent[1], dwm->res[1]);

    if (AfxAdjustWindow(wnd, &rc, NIL))
        AfxThrowError();

    return err;
}

_AUX afxError AfxAdjustWindowSurfaceFromNdc(afxWindow wnd, afxV2d const offset, afxV2d const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    afxDesktop* dwm = wnd->dwm;
    afxRect rc = { 0 };

    if (offset)
    {
        rc.x = (afxNat)AfxUnndcf(offset[0], wnd->frameRect.w - 1);
        rc.y = (afxNat)AfxUnndcf(offset[1], wnd->frameRect.h - 1);
    }

    AfxAssert(extent);
    rc.w = (afxNat)AfxUnndcf(extent[0], dwm->res[0]);
    rc.h = (afxNat)AfxUnndcf(extent[1], dwm->res[1]);

    if (AfxAdjustWindow(wnd, NIL, &rc))
        AfxThrowError();

    return err;
}

_AUX afxBool _AuxWndStdEventCb(afxWindow wnd, auxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    switch (ev->id)
    {
    case auxEventId_KEY:
    {
        if (AfxWasKeyPressed(AfxGetParent(wnd), 0, afxKey_PRINT))
        {
            if (wnd->dout)
            {
                afxUri2048 uri;
                AfxMakeUri2048(&uri, NIL);
                AfxFormatUri(&uri.uri, "tmp/ss-wnd%u-%u.tga", AfxGetObjectId((void*)wnd), AfxGetTimer());
                AfxPrintDrawOutputBuffer(wnd->dout, 0, 0, &uri.uri);
            }
        }   
        break;
    }
    }
    return TRUE;
}

_AUX afxError _AuxWndCtorCb(afxWindow wnd, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    afxSession ses = args[0];
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxWindowConfig const* cfg = (afxWindowConfig const*)(args[1]) + invokeNo;

    if (!cfg) AfxThrowError();
    else
    {
        //if (cfg->eventCb)
            //AfxResetEventHandler((void*)wnd, (void*)cfg->eventCb);

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

        AfxMakeString2048(&wnd->caption, &AfxString("Multimedia User Experience Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build"));

        afxBool fullscreen = FALSE;
        afxBool decorated = TRUE;
        afxBool resizable = TRUE;
        afxBool floating = FALSE;

        wnd->decorated = decorated;
        wnd->floating = floating;
        wnd->focused = FALSE;
        wnd->resizable = TRUE;


    }
    return err;
}

_AUX afxError _AuxWndDtorCb(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    AfxCleanUpChainedClasses(&wnd->classes);

    //AfxReleaseObjects(1, &wnd->m.dout);

    AfxChangeWindowIcon(wnd, NIL); // detach any custom icon
    
    return err;
}

_AUX afxClassConfig const _AuxWndStdImplementation =
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

_AUX afxError AfxConfigureWindow(afxSession ses, afxWindowConfig* cfg, afxV2d const nde)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    *cfg = (afxWindowConfig) { 0 };
    cfg->eventCb = _AuxWndStdEventCb;

    AfxConfigureDrawOutput(cfg->ddevId, &cfg->frame);
    cfg->frame.doNotClip = FALSE;
    cfg->frame.presentAlpha = avxPresentAlpha_PREMUL;
    AfxConfigureDrawOutput(cfg->ddevId, &cfg->surface);
    cfg->surface.pixelFmt = afxPixelFormat_ARGB8;
    cfg->surface.pixelFmtDs[0] = afxPixelFormat_D24;
    cfg->surface.bufFlags = afxRasterUsage_DRAW | afxRasterUsage_SAMPLING;
    cfg->surface.bufFlagsDs[0] = afxRasterUsage_DRAW | afxRasterUsage_SAMPLING;

    return err;
}

_AUX afxError AfxAcquireWindow(afxSession ses, afxWindowConfig const* cfg, afxWindow* window)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    afxClass* cls = (afxClass*)AfxGetWindowClass(ses);
    AfxAssertClass(cls, afxFcc_WND);
    afxWindow wnd;

    if (!cfg) AfxThrowError();
    else
    {
        if (AfxAcquireObjects(cls, 1, (afxObject*)&wnd, (void const*[]) { ses, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &wnd, afxFcc_WND);
            AfxAssert(window);
            *window = wnd;
        }
    }
    return err;
}
