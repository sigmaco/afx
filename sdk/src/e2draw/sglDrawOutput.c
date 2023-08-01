/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <shellapi.h>
#include <dwmapi.h>
#include <stdarg.h>
#include <stdio.h>
#pragma comment(lib,"dwmapi.lib")
//#pragma comment(lib, "opengl32")

#define _AFX_DRAW_OUTPUT_C
#define _AFX_TEXTURE_C
#define _AFX_SURFACE_C
#include "sgl.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/core/afxString.h"
#include "afx/math/afxVector.h"


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


_SGL afxError _AfxDoutVmtReqCb(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    (void)timeout;
    *bufIdx = AFX_INVALID_INDEX;

    afxNat idx = dout->lastReqBufIdx;
    afxTime time, t2;
    AfxGetTime(&time);

    for (;;)
    {
        idx = (idx + 1) % dout->bufCnt;
        afxSurface surf = dout->buffers[idx];

        if (!surf) AfxThrowError();
        else
        {
            AfxAssertObject(surf, AFX_FCC_SURF);

            if (AFX_SURF_STATE_PRESENTABLE >= AfxGetSurfaceState(surf))
            {
                //AfxAssert(!surf->swapchain.chain);
                surf->state = AFX_SURF_STATE_PRESENTABLE;
                *bufIdx = idx;
                break;
            }
        }

        if (timeout > AfxGetTime(&t2) - time)
        {
            AfxThrowError();
            *bufIdx = AFX_INVALID_INDEX;
            break;
        }
    }

    if (!err)
        dout->lastReqBufIdx = idx;

    return err;
}

_SGL afxError _AfxDoutVmtFlushCb(afxDrawOutput dout, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    (void)timeout;
    return err;
}

_SGL afxError _AfxDoutVmtDctxCb(afxDrawOutput dout, afxDrawContext from, afxDrawContext to)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    // TODO discard pull request submissions too.
    (void)from;
    (void)to;

    if (from && from->vmt->cout(from, dout, FALSE)) AfxThrowError(); // ask dctx to unlink this dout
    else
    {
        //AfxRegenerateDrawOutputBuffers(dout);

        if (to && to->vmt->cout(to, dout, TRUE)) AfxThrowError(); // ask dctx to unlink this dout
        else
        {

        }
    }

    AfxFormatString(dout->caption, "Draw Output %p (%s) --- Public Test Build --- QWADRO (c) 2017 SIGMA", dout, to && !err ? "On line" : "Off line");

    return err;
}

_SGL afxError _SglDoutVmtFlushCb(afxDrawOutput dout, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    (void)timeout;
    //_SglSwapBuffersNow(dout); // do black screen
    sglDoutIdd *idd = dout->idd;

    //if (idd->fcc == AFX_FCC_WND || idd->fcc == AFX_FCC_WPP)
    {
        SetWindowTextA(idd->wglWnd, AfxGetStringDataConst(dout->caption, 0));
        //UpdateWindow(dout->wglWnd);
    }
    return err;
}

_SGL afxError _SglDoutVmtProcCb(afxDrawOutput dout, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    return err;
}

_SGL afxError _SglDoutVmtDtorCb(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    sglDoutIdd *idd = dout->idd;

    DragAcceptFiles(idd->wglWnd, FALSE);
    ReleaseDC(idd->wglWnd, idd->wglDc);
    CloseWindow(idd->wglWnd);
    DestroyWindow(idd->wglWnd);
    
    AfxDeallocate(AfxGetDrawMemory(), idd);
    dout->idd = NIL;

    return err;
}

_SGL _afxDoutVmt SglDoutVmtWnd =
{
    _SglDoutVmtDtorCb,
    _SglDoutVmtProcCb,
    _AfxDoutVmtDctxCb,
    _SglDoutVmtFlushCb,
    _AfxDoutVmtReqCb,
};

_SGL _afxDoutVmt SglDoutVmtDsktp =
{
    _SglDoutVmtDtorCb,
    _SglDoutVmtProcCb,
    _AfxDoutVmtDctxCb,
    _SglDoutVmtFlushCb,
    _AfxDoutVmtReqCb,
};

_SGL BOOL CALLBACK _AfxFindShellWorkerWindowW32(HWND hwnd, LPARAM lParam)
{
    HWND* found = (HWND*)lParam;

    if (FindWindowExA(hwnd, NULL, "SHELLDLL_DefView", NULL))
        *found = FindWindowExA(NULL, hwnd, "WorkerW", NULL);

    return TRUE;
}

_SGL HWND _AfxFindShellBackgroundWindowW32(void)
{
    HWND p = FindWindow("ProgMan", NULL);
    HWND s = FindWindowEx(p, NULL, "SHELLDLL_DefView", NULL);
    HWND dtw = FindWindowEx(s, NULL, "SysListView32", NULL);
    return dtw;

    // Windows 10 Method

    SendMessageTimeoutA(FindWindowA("ProgMan", NULL), 0x052C, 0, 0, SMTO_NORMAL, 1000, NULL);
    HWND hwnd = 0;
    EnumWindows(_AfxFindShellWorkerWindowW32, (LPARAM)&(hwnd));
    return hwnd;
}

static const afxString g_str_desktop = AFX_STRING_LITERAL("wallpapper");
static const afxString g_str_window = AFX_STRING_LITERAL("window");

_SGL afxError _SglDdrvVmtDoutCb(afxDrawOutput dout, afxUri const* endpoint, afxDrawOutputSpecification const *spec)
{
    AfxEntry("dout=%p,uri=%.*s", dout, AfxPushString(endpoint ? AfxUriGetStringConst(endpoint) : &AFX_STR_EMPTY));
    afxError err = AFX_ERR_NONE;

    afxDrawDriver ddrv = AfxGetDrawOutputDriver(dout);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    _sglDdrvIdd *ddrvIdd = ddrv->idd;
    //AfxAssert(ddrvIdd->wglPrimeGlrc == _wglGetCurrentContext());
    //AfxAssert(ddrvIdd->wglPrimeDc == _wglGetCurrentDC());
    afxNat unitIdx = AfxGetSystemProcessor();
    if (!_wglMakeCurrent(ddrvIdd->dpus[unitIdx].wglDc, ddrvIdd->dpus[unitIdx].wglGlrc))
        AfxThrowError();

    afxString title;
    AfxWrapStringLiteral(&title, ddrvIdd->oglWndClss.lpszClassName, 0);
    AfxCopyString(dout->caption, &title);

    sglDoutIdd *idd = AfxAllocate(AfxGetDrawMemory(), sizeof(*idd), 0, AfxSpawnHint());
    dout->idd = idd;

    afxBool isDesk = FALSE;
    afxBool isWnd = FALSE;

    afxUri name;

    if (endpoint)
        AfxExcerptUriName(&name, endpoint);
    else
        AfxResetUri(&name);

    afxString const *surface = AfxUriGetStringConst(&name);

    if (AfxStringIsEmpty(surface) || 0 == AfxCompareString(surface, &g_str_window)) // print to window surface
    {
        idd->wglWnd = CreateWindowA(ddrvIdd->oglWndClss.lpszClassName, ddrvIdd->oglWndClss.lpszClassName, /*WS_POPUP*/WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NIL, NIL, ddrvIdd->oglWndClss.hInstance, NIL);
        isWnd = TRUE;
    }
    else if (0 == AfxCompareString(surface, &g_str_desktop))
    {
        idd->wglWnd = _AfxFindShellBackgroundWindowW32();// GetDesktopWindow();
        isDesk = TRUE;
    }

    dout->vmt = &SglDoutVmtWnd;

    if (!(idd->wglWnd)) AfxThrowError();
    else
    {
        SetWindowLongPtr(idd->wglWnd, GWLP_USERDATA, (LONG_PTR)dout);

        idd->wglDc = GetDC(idd->wglWnd);
        //hdc = GetWindowDC(hwnd);

        if (!(idd->wglDc)) AfxThrowError();
        else
        {
            afxWhd const resolution = { GetDeviceCaps(idd->wglDc, HORZRES), GetDeviceCaps(idd->wglDc, VERTRES), GetDeviceCaps(idd->wglDc, PLANES) };
            dout->refreshRate = GetDeviceCaps(idd->wglDc, VREFRESH);
            afxReal64 physAspRatio = (afxReal64)GetDeviceCaps(idd->wglDc, HORZSIZE) / (afxReal64)GetDeviceCaps(idd->wglDc, VERTSIZE);
            AfxReadjustDrawOutputProportion(dout, physAspRatio, resolution);
            AfxReadjustDrawOutputNdc(dout, AfxSpawnV3d(0.5, 0.5, 1));

            PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)_wglGetProcAddress("wglChoosePixelFormatARB");

            if (!wglChoosePixelFormatARB) AfxThrowError();
            else
            {
                afxNat bpp = dout->pixelFmt ? AfxPixelFormatGetBpp(dout->pixelFmt) : GetDeviceCaps(idd->wglDc, BITSPIXEL);

                if (!dout->pixelFmt)
                    dout->pixelFmt = AFX_PIXEL_FMT_RGBA8;

                //afxNat colorBpp = (bpp == 32 ? 24 : bpp);
                afxNat alphaBpp = (bpp == 32 ? 8 : 0);

                if (!dout->bufCnt)
                    dout->bufCnt = 2;

                if (!dout->bufUsage)
                    dout->bufUsage = AFX_TEX_FLAG_SURFACE_RASTER;

                int pxlAttrPairs[][2] =
                {
                    { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
                    { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
                    { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
                    { WGL_DOUBLE_BUFFER_ARB, (dout->bufCnt > 1)},
                    { WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB}, // WGL_TYPE_RGBA_FLOAT_ARB
                    { WGL_COLOR_BITS_ARB, bpp },
                    { WGL_ALPHA_BITS_ARB, alphaBpp },
                  //{ WGL_DEPTH_BITS_ARB, 24 }, // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                  //{ WGL_STENCIL_BITS_ARB, 8 },  // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                    { WGL_TRANSPARENT_ARB, (dout->presentAlpha != AFX_PRESENT_ALPHA_OPAQUE) },
                    { WGL_SWAP_METHOD_ARB, (dout->presentMode == AFX_PRESENT_MODE_IMMEDIATE) ? WGL_SWAP_COPY_ARB : WGL_SWAP_EXCHANGE_ARB },
                  //{ WGL_SAMPLE_BUFFERS_ARB,  GL_TRUE },  // works on Intel, didn't work on Mesa
                  //{ WGL_SAMPLES_ARB, 8 }, // works on Intel, didn't work on Mesa
                  //{ WGL_COLORSPACE_EXT, dout->colorSpc == AFX_COLOR_SPACE_SRGB ? WGL_COLORSPACE_SRGB_EXT : (dout->colorSpc == AFX_COLOR_SPACE_LINEAR ? WGL_COLORSPACE_LINEAR_EXT : NIL) }, // works on Mesa, didn't work on Intel
                  //{ WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, (dout->colorSpc == AFX_COLOR_SPACE_SRGB) }, // works on Mesa, didn't work on Intel
                    { NIL, NIL },
                };

                UINT formatCount;
                idd->wglDcPxlFmt = 0;

                if (!wglChoosePixelFormatARB(idd->wglDc, &pxlAttrPairs[0][0], 0, 1, &(idd->wglDcPxlFmt), &(formatCount))) AfxThrowError();
                else
                {
                    AfxAssert(idd->wglDcPxlFmt);
                    AfxAssert(formatCount);

                    PIXELFORMATDESCRIPTOR pfd;
                    AFX_ZERO(&pfd);
                    SglDescribePixelFormat(idd->wglDc, idd->wglDcPxlFmt, sizeof(pfd), &pfd);
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

                    if (!SglSetPixelFormat(idd->wglDc, idd->wglDcPxlFmt, &(pfd))) AfxThrowError();
                    else
                    {
                        if (dout->presentAlpha)
                        {
                            DWM_BLURBEHIND bb = { 0 };
                            bb.dwFlags = DWM_BB_ENABLE;
                            bb.fEnable = TRUE;
                            bb.hRgnBlur = NULL;
                            DwmEnableBlurBehindWindow(idd->wglWnd, &(bb));
                        }

                        //DragAcceptFiles(dout->wglWnd, TRUE);

                        if (isWnd)
                        {
                            SetWindowPos(idd->wglWnd, NULL, 0, 0, dout->resolution[0] / 2, dout->resolution[1] / 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
                        }

                        UpdateWindow(idd->wglWnd);
                        ShowWindow(idd->wglWnd, SHOW_OPENWINDOW);

                        idd->instanced = TRUE;
                    }
                }
            }

            if (err)
                ReleaseDC(idd->wglWnd, idd->wglDc);
        }

        if (err && isWnd)
            DestroyWindow(idd->wglWnd);
    }
    return err;
}
