/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
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


    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);

    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    struct _afxDoutD *doutD;
    _AfxGetDoutD(dout, &doutD,dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    (void)timeout;
    *bufIdx = AFX_INVALID_INDEX;

    afxNat idx = doutD->lastReqBufIdx;
    afxTime time, t2;
    AfxGetTime(&time);

    for (;;)
    {
        idx = (idx + 1) % doutD->bufCnt;
        afxSurface surf = doutD->buffers[idx].surf;

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
        doutD->lastReqBufIdx = idx;

    return err;
}

_SGL afxError _AfxDoutVmtFlushCb(afxDrawOutput dout, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    struct _afxDoutD *doutD;
    _AfxGetDoutD(dout, &doutD,dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    (void)timeout;
    return err;
}

_SGL afxError _AfxDoutVmtDctxCb(afxDrawOutput dout, afxDrawContext from, afxDrawContext to, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);

    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    struct _afxDoutD *doutD;
    _AfxGetDoutD(dout, &doutD,dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    // TODO discard pull request submissions too.

    struct _afxDctxD *fromD;

    if (from)
    {
        AfxAssertObjects(1, &from, AFX_FCC_DCTX);
        _AfxGetDctxD(from, &fromD, dsysD);
    }

    struct _afxDctxD *toD;

    if (to)
    {
        AfxAssertObjects(1, &to, AFX_FCC_DCTX);
        _AfxGetDctxD(to, &toD, dsysD);
    }

    if (from && fromD->vmt->cout(from, dout, FALSE, slotIdx)) AfxThrowError(); // ask dctx to unlink this dout
    else
    {
        //AfxRegenerateDrawOutputBuffers(dout);

        if (to && toD->vmt->cout(to, dout, TRUE, slotIdx)) AfxThrowError(); // ask dctx to unlink this dout
        else
        {

        }
    }

    AfxFormatString(doutD->caption, "Draw Output %p (%s) --- OpenGL/Vulkan Continuous Integration (c) 2017 SIGMA Technology Group --- Public Test Build", dout, to && !err ? "On line" : "Off line");

    return err;
}

_SGL afxError _SglDoutVmtFlushCb(afxDrawOutput dout, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);

    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    struct _afxDoutD *doutD;
    _AfxGetDoutD(dout, &doutD,dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    (void)timeout;
    //_SglSwapBuffersNow(dout); // do black screen
    sglDoutIdd *idd = doutD->idd;

    //if (idd->fcc == AFX_FCC_WND || idd->fcc == AFX_FCC_WPP)
    {
        SetWindowTextA(idd->wnd, AfxGetStringDataConst(doutD->caption, 0));
        //UpdateWindow(doutD->wglWnd);
    }
    return err;
}

_SGL afxError _SglDoutVmtProcCb(afxDrawOutput dout, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);

    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    struct _afxDoutD *doutD;
    _AfxGetDoutD(dout, &doutD,dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    return err;
}

_SGL afxError _SglDoutVmtDtorCb(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    struct _afxDoutD *doutD;
    _AfxGetDoutD(dout, &doutD,dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    sglDoutIdd *idd = doutD->idd;

    DragAcceptFiles(idd->wnd, FALSE);
    ReleaseDC(idd->wnd, idd->dc);
    CloseWindow(idd->wnd);
    DestroyWindow(idd->wnd);
    
    AfxDeallocate(AfxGetDrawMemory(), idd);
    doutD->idd = NIL;

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
    // Windows 7 Method
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

static const afxString g_str_desktop = AFX_STRING_LITERAL("desktop");
static const afxString g_str_window = AFX_STRING_LITERAL("window");

_SGL afxError _SglDdrvVmtDoutCb(afxDrawOutput dout, afxUri const* endpoint, afxDrawOutputConfig const *spec)
{
    AfxEntry("dout=%p,uri=%.*s", dout, AfxPushString(endpoint ? AfxUriGetStringConst(endpoint) : &AFX_STR_EMPTY));
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

    afxDrawDriver ddrv = AfxGetDrawOutputDriver(dout);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    _sglDdrvIdd *ddrvIdd = ddrv->idd;
    //AfxAssert(ddrvIdd->wglPrimeGlrc == _wglGetCurrentContext());
    //AfxAssert(ddrvIdd->wglPrimeDc == _wglGetCurrentDC());
    afxNat unitIdx;
    AfxGetThreadingUnit(&unitIdx);
    sglDpuIdd *dpu = &ddrvIdd->dpus[unitIdx];
    wglVmt const*wgl = &dpu->wgl;

    HGLRC rc = wgl->GetCurrentContext();

    if (rc != dpu->glrc)
    {
        if (!wgl->MakeCurrent(dpu->dc, dpu->glrc))
        {
            AfxThrowError();
            AfxError("DPU %u failed to be set.", unitIdx);
            wgl->MakeCurrent(dpu->dc, dpu->glrc);
        }
    }
    afxString title;
    AfxWrapStringLiteral(&title, ddrvIdd->wndClss.lpszClassName, 0);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    _AfxGetDoutD(dout, &doutD,dsysD);
    AfxCopyString(doutD->caption, &title);

    sglDoutIdd *idd = AfxAllocate(AfxGetDrawMemory(), sizeof(*idd), 0, AfxSpawnHint());
    doutD->idd = idd;

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
        idd->wnd = CreateWindowA(ddrvIdd->wndClss.lpszClassName, ddrvIdd->wndClss.lpszClassName, /*WS_POPUP*/WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NIL, NIL, ddrvIdd->wndClss.hInstance, NIL);
        isWnd = TRUE;
    }
    else if (0 == AfxCompareString(surface, &g_str_desktop))
    {
        idd->wnd = _AfxFindShellBackgroundWindowW32();// GetDesktopWindow();
        isDesk = TRUE;
    }

    doutD->vmt = &SglDoutVmtWnd;

    if (!(idd->wnd)) AfxThrowError();
    else
    {
        SetWindowLongPtr(idd->wnd, GWLP_USERDATA, (LONG_PTR)dout);

        idd->dc = GetDC(idd->wnd);
        //hdc = GetWindowDC(hwnd);

        if (!(idd->dc)) AfxThrowError();
        else
        {
            afxWhd const resolution = { GetDeviceCaps(idd->dc, HORZRES), GetDeviceCaps(idd->dc, VERTRES), GetDeviceCaps(idd->dc, PLANES) };
            doutD->refreshRate = GetDeviceCaps(idd->dc, VREFRESH);
            afxReal64 physAspRatio = (afxReal64)GetDeviceCaps(idd->dc, HORZSIZE) / (afxReal64)GetDeviceCaps(idd->dc, VERTSIZE);
            AfxReadjustDrawOutputProportion(dout, physAspRatio, resolution);
            
            if (isWnd)
                AfxReadjustDrawOutputNdc(dout, AfxSpawnV3d(0.5, 0.5, 1));
            else
                AfxReadjustDrawOutputNdc(dout, AfxSpawnV3d(1, 1, 1));

            afxNat bpp = 0;

            if (!doutD->pixelFmt)
            {
                bpp = GetDeviceCaps(idd->dc, BITSPIXEL);

                switch (bpp > 24)
                {
                case 8: doutD->pixelFmt = AFX_PFD_R8; break;
                case 16: doutD->pixelFmt = AFX_PFD_GR8; break;
                case 24: doutD->pixelFmt = AFX_PFD_BGR8; break;
                case 32: doutD->pixelFmt = AFX_PFD_BGRA8; break;
                default: doutD->pixelFmt = AFX_PFD_ARGB8; break;
                }
            }

            afxPixelLayout pfl;
            AfxDescribePixelFormat(doutD->pixelFmt, &pfl);
            bpp = pfl.bpp;

            afxNat alphaBpp = pfl.bpc[3];

            if (!doutD->bufCnt)
                doutD->bufCnt = 1;

            if (!doutD->bufUsage)
                doutD->bufUsage = AFX_TEX_USAGE_DRAW;

            int pxlAttrPairs[][2] =
            {
                { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
                { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
                { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
                { WGL_DOUBLE_BUFFER_ARB, TRUE }, // single buffer not supported anymore today. Qwadro will just virtualizes it.
                { WGL_PIXEL_TYPE_ARB, AfxPixelFormatIsReal(doutD->pixelFmt) ? WGL_TYPE_RGBA_FLOAT_ARB : WGL_TYPE_RGBA_ARB },
                { WGL_COLOR_BITS_ARB, bpp },
                { WGL_ALPHA_BITS_ARB, alphaBpp },
                //{ WGL_DEPTH_BITS_ARB, 24 }, // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                //{ WGL_STENCIL_BITS_ARB, 8 },  // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                { WGL_TRANSPARENT_ARB, (doutD->presentAlpha != AFX_PRESENT_ALPHA_OPAQUE) },
                { WGL_SWAP_METHOD_ARB, (doutD->presentMode == AFX_PRESENT_MODE_IMMEDIATE) ? WGL_SWAP_COPY_ARB : WGL_SWAP_EXCHANGE_ARB },
                //{ WGL_SAMPLE_BUFFERS_ARB,  GL_TRUE },  // works on Intel, didn't work on Mesa
                //{ WGL_SAMPLES_ARB, 8 }, // works on Intel, didn't work on Mesa
                //{ WGL_COLORSPACE_EXT, doutD->colorSpc == AFX_COLOR_SPACE_SRGB ? WGL_COLORSPACE_SRGB_EXT : (doutD->colorSpc == AFX_COLOR_SPACE_LINEAR ? WGL_COLORSPACE_LINEAR_EXT : NIL) }, // works on Mesa, didn't work on Intel
                //{ WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, (doutD->colorSpc == AFX_COLOR_SPACE_SRGB) }, // works on Mesa, didn't work on Intel
                { NIL, NIL },
            };

            UINT formatCount;
            idd->dcPxlFmt = 0;

            if (!wgl->ChoosePixelFormatARB(idd->dc, &pxlAttrPairs[0][0], 0, 1, &(idd->dcPxlFmt), &(formatCount))) AfxThrowError();
            else
            {
                AfxAssert(idd->dcPxlFmt);
                AfxAssert(formatCount);

                PIXELFORMATDESCRIPTOR pfd;
                AFX_ZERO(&pfd);
                SglDescribePixelFormat(idd->dc, idd->dcPxlFmt, sizeof(pfd), &pfd, wgl);
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
                //doutD->wglDcPxlFmt = _SglChoosePixelFormat(doutD->wglDc, &(pfd));

                if (!SglSetPixelFormat(idd->dc, idd->dcPxlFmt, &(pfd), wgl)) AfxThrowError();
                else
                {
                    if (doutD->presentAlpha)
                    {
                        DWM_BLURBEHIND bb = { 0 };
                        bb.dwFlags = DWM_BB_ENABLE;
                        bb.fEnable = TRUE;
                        bb.hRgnBlur = NULL;
                        DwmEnableBlurBehindWindow(idd->wnd, &(bb));
                    }

                    //DragAcceptFiles(doutD->wglWnd, TRUE);

                    if (isWnd)
                    {
                        SetWindowPos(idd->wnd, NULL, 0, 0, doutD->resolution[0] / 2, doutD->resolution[1] / 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
                    }

                    UpdateWindow(idd->wnd);
                    ShowWindow(idd->wnd, SHOW_OPENWINDOW);

                    idd->instanced = TRUE;
                }
            }

            if (err)
                ReleaseDC(idd->wnd, idd->dc);
        }

        if (err && isWnd)
            DestroyWindow(idd->wnd);
    }
    return err;
}
