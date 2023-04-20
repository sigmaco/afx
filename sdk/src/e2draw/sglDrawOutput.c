#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <shellapi.h>
#include <dwmapi.h>
#include <stdarg.h>
#include <stdio.h>
#pragma comment(lib,"dwmapi.lib")
//#pragma comment(lib, "opengl32")
#include "sgl.h"

#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/core/base/afxString.h"
#include "afx/math/afxVector.h"

typedef struct
{
    afxDrawContext dctx;
    afxNat const *whd;
    afxDrawOutputSpecification const *spec;
} _afxDoutCtorArgs;


_SGL afxError _SglSwapBuffersNow(afxDrawOutput dout)
{
    afxError err = NIL;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    afxFcc fcc = *(afxFcc *)AfxObjectGetVmt(&dout->obj);

    if (fcc == AFX_FCC_WND || fcc == AFX_FCC_WPP)
    {

        if (!(wsglSwapBuffers(dout->wglDc)))
            if (!(SwapBuffers(dout->wglDc)))
                AfxThrowError();

        //SetWindowTextA(dout->wglWnd, dout->title.buf); // deadlocks all threads on exit
        //UpdateWindow(dout->wglWnd);
        //AfxYield();

    }
    return err;
}

_SGL int _SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd)
{
    int fmt = wsglChoosePixelFormat(hdc, ppfd);

    if (!fmt)
        fmt = ChoosePixelFormat(hdc, ppfd);

    return fmt;
}

_SGL BOOL _SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd)
{
    BOOL rslt = wsglSetPixelFormat(hdc, format, ppfd);
    
    if (!rslt)
        rslt = SetPixelFormat(hdc, format, ppfd);

    return rslt;
}

_SGL int _SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd)
{
    int rslt = wsglDescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd);

    if (!rslt)
        rslt = DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd);

    return rslt;
}

//############################################################################
//##                                                                        ##
//## Calc_window_values - calculates the extra width and height to add to   ##
//##   the windows's size so that the video fits.                           ##
//##                                                                        ##
//############################################################################

// Democratized from Bink, by SIGMA Co., Technology Group.

_SGL void Calc_window_values(HWND window, afxInt* out_extra_width, afxInt32* out_extra_height)
{
    RECT r, c;

    //
    // Get the current window rect (in screen space).
    //

    GetWindowRect(window, &r);

    //
    // Get the client rectangle of the window.
    //

    GetClientRect(window, &c);

    *out_extra_width = (r.right - r.left) - (c.right - c.left);
    *out_extra_height = (r.bottom - r.top) - (c.bottom - c.top);
}


_SGL afxError _AfxDrawOutputProcess(afxDrawOutput dout)
{
    afxError err = NIL;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    //_SglSwapBuffersNow(dout); // do black screen
    //SetWindowTextA(dout->wglWnd, dout->title.buf);
    //UpdateWindow(dout->wglWnd);
    return err;
}

_SGL afxSurface _AfxDrawOutputGetBuffer(afxDrawOutput dout, afxNat idx)
{
    afxError err = NIL;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxTransistorEnterShared(&dout->buffersLock);
    AfxAssert(idx < dout->bufCnt);
    afxSurface surf = dout->buffers[idx];
    AfxAssertObject(surf, AFX_FCC_SURF);
    AfxTransistorExitShared(&dout->buffersLock);
    return surf;
}

_SGL afxError _AfxDrawOutputRequestBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
{
    (void)timeout;
    afxError err = NIL;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    *bufIdx = AFX_INVALID_INDEX;

    AfxTransistorEnterExclusive(&dout->buffersLock);

    afxNat idx = (dout->lastAcqBufIdx + 1) % dout->bufCnt;
    afxNat time = AfxGetTimer();

    for (;;)
    {
        afxSurface surf = dout->buffers[idx];
        AfxAssertObject(surf, AFX_FCC_SURF);

        if (NIL == surf->swapchain.chain && surf->state == AFX_SURF_STATE_IDLE)
        {
            AfxAssert(!surf->swapchain.chain);

            surf->state = AFX_SURF_STATE_PRESENTABLE;
            *bufIdx = idx;
            break;
        }
        else
        {
            idx = (idx + 1) % dout->bufCnt;
        }

        if (timeout > AfxGetTimer() - time)
        {
            AfxThrowError();
            *bufIdx = AFX_INVALID_INDEX;
            break;
        }
    }

    if (!err)
        dout->lastAcqBufIdx = idx;

    AfxTransistorExitExclusive(&dout->buffersLock);

    return err;
}

_SGL afxResult _AfxDrawOutputEnumerateBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxSurface surf[])
{
    afxError err = NIL;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssert(cnt);
    AfxAssert(dout->bufCnt >= first + cnt);
    AfxAssert(surf);
    afxResult cnt = 0;

    for (afxNat i = 0; i < dout->bufCnt; i++)
    {
        afxSurface surf2 = dout->buffers[first + i];
        AfxAssertObject(surf2, AFX_FCC_SURF);
        surf[i] = surf2;
        ++cnt;
    }
    return cnt;
}

_SGL afxReal* _AfxDrawOutputGetExtentNdc(afxDrawOutput dout, afxV3d extent) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = NIL;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssert(extent);
    return AfxV3dSet(extent, AfxToNdc(dout->whd[0], dout->whdMax[0]), AfxToNdc(dout->whd[1], dout->whdMax[1]), (afxReal)1);
}

_SGL afxNat* _AfxDrawOutputGetExtent(afxDrawOutput dout, afxWhd extent)
{
    afxError err = NIL;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    extent[0] = dout->whd[0];
    extent[1] = dout->whd[1];
    extent[2] = dout->whd[2];
    return extent;
}

_SGL afxError _AfxDrawOutputSetExtent(afxDrawOutput dout, afxWhd const extent)
{
    afxError err = NIL;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssert(extent);
    AfxAssert(extent[0]);
    AfxAssert(extent[1]);
    AfxAssert(extent[2]);
    AfxAssert(extent[0] != dout->whd[0] || extent[1] != dout->whd[1] || extent[2] != dout->whd[2]);
    afxBool resizable;

    if ((resizable = dout->resizable))
    {
        dout->resizable = FALSE; // temporarily lock it to avoid reentrance of platform hooks.

        AfxAssert(extent);
        AfxAssert(extent[0]);
        AfxAssert(extent[1]);
        AfxAssert(extent[2]);

        dout->whd[0] = extent[0];
        dout->whd[1] = extent[1];
        dout->whd[2] = extent[2];

        dout->aspectRatio = (dout->whd[0] * dout->whd[1]) ? dout->whd[0] / dout->whd[1] : 0;

        for (afxNat i = 0; i < dout->bufCnt; i++)
        {
            afxSurface surf = dout->buffers[i];
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxTextureSetExtent(&surf->tex, surf->tex.layerCnt, dout->whd);
        }

        //if (dout->impl->readjust(dout, whd))
            //AfxThrowError();
        // estava desativado?

        afxFcc fcc = *(afxFcc *)AfxObjectGetVmt(&dout->obj);

        if (fcc == AFX_FCC_WND || fcc == AFX_FCC_WPP)
        {
            RECT rect;
            GetClientRect(NULL, &(rect));
            dout->whdMax[0] = rect.right - rect.left;
            dout->whdMax[1] = rect.bottom - rect.top;
            dout->whdMax[2] = 1;
        }

        afxV3d extent;
        AfxDrawOutputGetExtentNdc(dout, extent);
        AfxEcho("Extent readjusted. (dout)%p -> %u, %f of %u; %u %f of %u", dout, dout->whd[0], extent[0], dout->whdMax[0], dout->whd[1], extent[1], dout->whdMax[1]);

        dout->resizable = resizable;
    }
    return err;
}

_SGL afxError _AfxDrawOutputSetExtentNdc(afxDrawOutput dout, afxV3d const extent) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = NIL;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssert(extent);
    dout->whd[0] = (afxNat)AfxFromNdc(extent[0], dout->whdMax[0]);
    dout->whd[1] = (afxNat)AfxFromNdc(extent[1], dout->whdMax[1]);
    dout->whd[2] = (afxNat)AfxFromNdc(extent[2], dout->whdMax[2]);
    return _AfxDrawOutputSetExtent(dout, dout->whd);
}

_SGL afxError _AfxDoutDropAllSurfaces(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = NIL;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    for (afxNat i = 0; i < dout->bufCnt; i++)
    {
        afxSurface surf = dout->buffers[i];
        AfxAssertObject(surf, AFX_FCC_SURF);

        if (surf->swapchain.chain)
        {
            AfxAssert(surf->state == AFX_SURF_STATE_PENDING);
            AfxAssert(surf->swapchain.chain == &dout->swapchain);
            AfxLinkageDrop(&surf->swapchain);
        }

        AfxObjectRelease(&surf->tex.res.obj);
        dout->buffers[i] = NIL;
    }

    dout->bufCnt = 0;
    return err;
}

_SGL afxDrawOutput _AfxDrawContextAcquireOutput(afxDrawContext dctx, afxWhd const extent, afxDrawOutputSpecification const *spec) // file, window, desktop, widget, etc; physical or virtual VDUs.
{
    AfxEntry("dctx=%p,extent=%p,spec=%p", dctx, extent, spec);
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawOutput dout = NIL;

    _afxDoutCtorArgs args =
    {
        dctx,
        extent,
        spec
    };

    if (!(dout = AfxObjectAcquire(AfxDrawContextGetOutputClass(dctx), &args, AfxSpawnHint())))
        AfxThrowError();

    return dout;
}

_SGL afxError _AfxDoutDtor(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = NIL;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    afxFcc fcc = *(afxFcc *)AfxObjectGetVmt(&dout->obj);

    if (fcc == AFX_FCC_WND || fcc == AFX_FCC_WPP)
    {
        DragAcceptFiles(dout->wglWnd, FALSE);
        ReleaseDC(dout->wglWnd, dout->wglDc);
        CloseWindow(dout->wglWnd);
        DestroyWindow(dout->wglWnd);
    }

    _AfxDoutDropAllSurfaces(dout);

    AfxAssert(dout->swapchain.cnt == 0);
    AfxTransistorDrop(&dout->buffersLock);

    return err;
}

_SGL afxDoutImpl const _AfxStdDoutWndImpl;
afxDoutImpl const _AfxStdDoutWndImpl =
{
    AFX_FCC_WND,
    "window",
    _AfxDrawOutputSetExtent,
    _AfxDrawOutputSetExtentNdc,
    _AfxDrawOutputGetExtent,
    _AfxDrawOutputGetExtentNdc,
    _AfxDrawOutputGetBuffer,
    _AfxDrawOutputEnumerateBuffers,
    _AfxDrawOutputRequestBuffer,
};

_SGL afxError _AfxStdDoutImplCtorWnd(afxDrawOutput dout)
{
    afxError err = NIL;
    AfxEntry("dout=%p", dout);
    AfxAssertObject(dout, AFX_FCC_DOUT);

    afxDrawContext dctx = AfxObjectGetProvider(&dout->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    afxDrawDriver ddrv = AfxDrawOutputGetDriver(dout);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    _sglDriverIdd *ddrvIdd = ddrv->idd;
    AfxAssert(ddrvIdd->wglPrimeGlrc == wsglGetCurrentContext());
    //AfxAssert(ddrvIdd->wglPrimeDc == wsglGetCurrentDC());

    afxDrawSystem dsys = AfxDrawContextGetDrawSystem(dctx);
    AfxAssertObject(dsys, AFX_FCC_DSYS);

    AfxString1024(&(dout->title), NIL);
    AfxStringUpdate(&(dout->title.str), 0, 0, ddrvIdd->oglWndClss.lpszClassName);

    dout->wglWnd = CreateWindowA(ddrvIdd->oglWndClss.lpszClassName, ddrvIdd->oglWndClss.lpszClassName, /*WS_POPUP*/WS_OVERLAPPEDWINDOW, 0, 0, dout->whd[0], dout->whd[1], NIL, NIL, ddrvIdd->oglWndClss.hInstance, NIL);

    if (!(dout->wglWnd)) AfxThrowError();
    else
    {
        SetWindowLongPtr(dout->wglWnd, GWLP_USERDATA, (LONG_PTR)dout);

        dout->wglDc = GetDC(dout->wglWnd);
        //hdc = GetWindowDC(hwnd);

        if (!(dout->wglDc)) AfxThrowError();
        else
        {
            PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)glcGetProcAddress(NIL, "wglChoosePixelFormatARB");
            
            if (!wglChoosePixelFormatARB) AfxThrowError();
            else
            {
                afxNat bpp = AfxPixelFormatGetBpp(dout->pixelFmt);
                afxNat colorBpp = (bpp == 32 ? 24 : bpp);
                afxNat alphaBpp = (bpp == 32 ? 8 : 0);
                
                int pxlAttrPairs[16][2];

                pxlAttrPairs[0][0] = WGL_DRAW_TO_WINDOW_ARB;
                pxlAttrPairs[0][1] = GL_TRUE;

                pxlAttrPairs[1][0] = WGL_SUPPORT_OPENGL_ARB;
                pxlAttrPairs[1][1] = GL_TRUE;

                pxlAttrPairs[2][0] = WGL_ACCELERATION_ARB;
                pxlAttrPairs[2][1] = WGL_FULL_ACCELERATION_ARB;

                pxlAttrPairs[3][0] = WGL_DOUBLE_BUFFER_ARB;
                pxlAttrPairs[3][1] = (dout->bufCnt > 1);

                pxlAttrPairs[4][0] = WGL_PIXEL_TYPE_ARB;
                pxlAttrPairs[4][1] = WGL_TYPE_RGBA_ARB; // WGL_TYPE_RGBA_FLOAT_ARB

                pxlAttrPairs[5][0] = WGL_COLOR_BITS_ARB;
                pxlAttrPairs[5][1] = colorBpp;

                pxlAttrPairs[6][0] = WGL_ALPHA_BITS_ARB;
                pxlAttrPairs[6][1] = alphaBpp;

                pxlAttrPairs[7][0] = WGL_DEPTH_BITS_ARB;
                pxlAttrPairs[7][1] = 24;

                pxlAttrPairs[8][0] = WGL_STENCIL_BITS_ARB;
                pxlAttrPairs[8][1] = 8;

                pxlAttrPairs[9][0] = WGL_TRANSPARENT_ARB;
                pxlAttrPairs[9][1] = (dout->presentAlpha != AFX_PRESENT_ALPHA_OPAQUE);

                pxlAttrPairs[10][0] = WGL_SWAP_METHOD_ARB;
                pxlAttrPairs[10][1] = dout->presentMode == AFX_PRESENT_MODE_IMMEDIATE ? WGL_SWAP_COPY_ARB : WGL_SWAP_EXCHANGE_ARB;

                //pxlAttrPairs[10][0] = WGL_SAMPLE_BUFFERS_ARB; // works on Intel, didn't work on Mesa
                //pxlAttrPairs[10][1] = GL_TRUE;

                //pxlAttrPairs[11][0] = WGL_SAMPLES_ARB; // works on Intel, didn't work on Mesa
                //pxlAttrPairs[11][1] = 8;

                //pxlAttrPairs[13][0] = WGL_COLORSPACE_EXT; // works on Mesa, didn't work on Intel
                //pxlAttrPairs[13][1] = dout->colorSpc == AFX_COLOR_SPACE_SRGB ? WGL_COLORSPACE_SRGB_EXT : (dout->colorSpc == AFX_COLOR_SPACE_LINEAR ? WGL_COLORSPACE_LINEAR_EXT : NIL);

                //pxlAttrPairs[14][0] = WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB; // works on Mesa, didn't work on Intel
                //pxlAttrPairs[14][1] = (dout->colorSpc == AFX_COLOR_SPACE_SRGB);


                pxlAttrPairs[11][0] = NIL;
                pxlAttrPairs[11][1] = NIL;

                UINT formatCount;
                dout->wglDcPxlFmt = 0;

                if (!wglChoosePixelFormatARB(dout->wglDc, pxlAttrPairs, 0, 1, &(dout->wglDcPxlFmt), &(formatCount))) AfxThrowError();
                else
                {
                    AfxAssert(dout->wglDcPxlFmt);
                    AfxAssert(formatCount);

                    PIXELFORMATDESCRIPTOR pfd;
                    AFX_ZERO(&pfd);
#if 0
                    pfd.nSize = sizeof(pfd);
                    pfd.nVersion = 1;
                    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DIRECT3D_ACCELERATED | PFD_SUPPORT_COMPOSITION;
                    pfd.iPixelType = PFD_TYPE_RGBA;
                    pfd.cColorBits = 24;
                    pfd.cAlphaBits = 8;
                    pfd.cDepthBits = 24;
                    pfd.cStencilBits = 8;
#endif
                    //dout->wglDcPxlFmt = _SglChoosePixelFormat(dout->wglDc, &(pfd));

                    if (!_SglSetPixelFormat(dout->wglDc, dout->wglDcPxlFmt, &(pfd))) AfxThrowError();
                    else
                    {
                        if (dout->presentAlpha)
                        {
                            DWM_BLURBEHIND bb = { 0 };
                            bb.dwFlags = DWM_BB_ENABLE;
                            bb.fEnable = TRUE;
                            bb.hRgnBlur = NULL;
                            DwmEnableBlurBehindWindow(dout->wglWnd, &(bb));
                        }

                        //DragAcceptFiles(dout->wglWnd, TRUE);

                        UpdateWindow(dout->wglWnd);
                        ShowWindow(dout->wglWnd, SHOW_OPENWINDOW);

                        dout->instanced = TRUE;
                    }
                }
            }

            if (err)
                ReleaseDC(dout->wglWnd, dout->wglDc);
        }

        if (err)
            DestroyWindow(dout->wglWnd);
    }
    return err;
}

_SGL afxError _AfxStdDoutImplCtor(afxDrawOutput dout, afxUri const *uri)
{
    AfxEntry("dout=%p,uri=%.*s", dout, AfxPushString(AfxUriGetStringConst(uri)));
    afxError err = NIL;
    afxUri name;
    AfxUriForkName(&name, uri);
    afxString const *surface = AfxUriGetStringConst(&name);
    afxString tmp;

    if (AfxStringIsEmpty(surface)) AfxThrowError();
    else
    {
        AfxStringMapConst(&tmp, _AfxStdDoutWndImpl.name, 0);

        if (0 == AfxStringCompare(surface, &tmp)) // print to window surface
        {
            if (_AfxStdDoutImplCtorWnd(dout))
                AfxThrowError();
        }
        else
        {
            //AfxStringMapConst(&tmp, _AfxStdDoutWppImpl.name, 0);

            if (0 == AfxStringCompare(surface, &tmp)) // print to desktop background surface
            {
                AfxError("Not implemented");
            }
            else
            {
                //AfxStringMapConst(&tmp, _AfxStdDoutFileImpl.name, 0);

                if (0 == AfxStringCompare(surface, &tmp)) // print to file
                {
                    AfxError("Not implemented");
                }
                else
                {
                    AfxThrowError();
                    AfxError("Not supported");
                }
            }
        }
    }

    return err;
}

_SGL afxError _AfxDoutCtor(afxDrawOutput dout, _afxDoutCtorArgs *args)
{
    AfxEntry("dout=%p", dout);
    afxError err = NIL;

    // absolute extent
    dout->whdMax[0] = 1280;
    dout->whdMax[1] = 720;
    dout->whdMax[2] = 1;

    dout->whd[0] = 1280;
    dout->whd[1] = 720;
    dout->whd[2] = 1;

    dout->aspectRatio = dout->whd[0] / dout->whd[1];
    dout->resizable = TRUE;

    // swapchain-related data
    dout->flags = NIL;
    dout->surface = NIL; // usually a OS window handle
    dout->pixelFmt = AFX_PIXEL_FMT_RGBA8; // or AFX_PIXEL_FMT_RGBA8R ?
    dout->colorSpc = AFX_COLOR_SPACE_SRGB; // sRGB is the default
    dout->bufUsage = AFX_TEX_USAGE_SURFACE_RASTER;
    dout->bufCnt = 0;// 3; // 2 or 3; double or triple buffered for via-memory presentation.
    dout->lastAcqBufIdx = 0;
    AfxChainDeploy(&dout->swapchain, dout);
    AfxTransistorDeploy(&dout->buffersLock);

    dout->presentAlpha = TRUE; // consider transparency for window composing.
    dout->presentTransform = AFX_PRESENT_TRANSFORM_FLIP_V; // NIL leaves it as it is.
    dout->presentMode = AFX_PRESENT_MODE_FIFO;
    dout->clipped = TRUE; // usually true to don't spend resources doing off-screen draw.

    AfxV2dZero(dout->absCursorPos);
    AfxV2dZero(dout->absCursorMove);
    AfxV2dZero(dout->ndcCursorPos);
    AfxV2dZero(dout->ndcCursorMove);

    if (args->whd)
    {
        AfxAssert(args->whd[0]);
        AfxAssert(args->whd[1]);
        AfxAssert(args->whd[2]);

        dout->whd[0] = args->whd[0];
        dout->whd[1] = args->whd[1];
        dout->whd[2] = args->whd[2];
    }

    dout->aspectRatio = dout->whd[0] / dout->whd[1];
    dout->resizable = TRUE;

    if (args->spec)
    {
        // swapchain-related data
        dout->pixelFmt = args->spec->pixelFmt;
        dout->colorSpc = args->spec->colorSpc;
        dout->bufUsage = args->spec->bufUsage;
        dout->bufCnt = 0;// args->spec->bufCnt;
        dout->presentAlpha = args->spec->presentAlpha;
        dout->presentTransform = args->spec->presentTransform;
        dout->presentMode = args->spec->presentMode;
        dout->clipped = args->spec->clipped;
    }

    AfxAssert(args->spec->bufCnt <= sizeof(dout->buffers) / sizeof(dout->buffers[0]));

    for (afxNat i = 0; i < args->spec->bufCnt; i++)
    {
        if (!(dout->buffers[i] = AfxDrawContextAcquireSurface(args->dctx, dout->pixelFmt, dout->whd, dout->bufUsage)))
        {
            AfxThrowError();
            break;
        }
        else
        {
            ++dout->bufCnt;
        }
    }

    dout->swapping = FALSE;
    dout->lastAcqBufIdx = dout->bufCnt - 1; // to start at 0 instead of 1.

    if (!err)
    {
        if ((AFX_SUCCESS != _AfxStdDoutImplCtor(dout, args->spec->endpoint))) AfxThrowError();
        else
        {
            dout->open = TRUE;
        }
    }

    if (err)
        _AfxDoutDropAllSurfaces(dout);

    return err;
}

_SGL afxClassSpecification const _AfxDoutClassSpec;

afxClassSpecification const _AfxDoutClassSpec =
{
    AFX_FCC_DOUT,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawOutput)),
    NIL,
    (void*)_AfxDoutCtor,
    (void*)_AfxDoutDtor,
    "Draw Output",
    &_AfxStdDoutWndImpl
};
