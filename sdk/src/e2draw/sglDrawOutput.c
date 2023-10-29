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

#include "sgl.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/core/afxString.h"
#include "afx/math/afxVector.h"

_SGL afxNat _SglDoutBuffersAreLocked(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->base.buffersLock);
    afxNat bufferLockCnt = dout->base.bufferLockCnt;
    AfxExitSlockShared(&dout->base.buffersLock);
    return bufferLockCnt;
}

_SGL afxNat _SglDoutLockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->base.buffersLock);
    afxNat bufferLockCnt = ++dout->base.bufferLockCnt;
    AfxExitSlockExclusive(&dout->base.buffersLock);
    return bufferLockCnt;
}

_SGL afxNat _SglDoutUnlockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->base.buffersLock);
    afxNat bufferLockCnt = --dout->base.bufferLockCnt;
    AfxExitSlockExclusive(&dout->base.buffersLock);
    return bufferLockCnt;
}

_SGL afxNat _SglDoutIsSuspended(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->base.suspendSlock);
    afxNat suspendCnt = dout->base.suspendCnt;
    AfxExitSlockShared(&dout->base.suspendSlock);
    return suspendCnt;
}

_SGL afxNat _SglDoutSuspendFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->base.suspendSlock);
    afxNat suspendCnt = ++dout->base.suspendCnt;
    AfxExitSlockExclusive(&dout->base.suspendSlock);
    return suspendCnt;
}

_SGL afxNat _SglDoutResumeFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->base.suspendSlock);
    afxNat suspendCnt = --dout->base.suspendCnt;
    AfxExitSlockExclusive(&dout->base.suspendSlock);
    return suspendCnt;
}

_SGL afxError _SglDoutFreeAllBuffers(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    for (afxNat i = 0; i < dout->base.bufCnt; i++)
    {
        //afxCanvas canv = dout->base.buffers[i].canv;
        afxTexture tex = dout->base.buffers[i].tex;

        if (!tex)
        {
            AfxAssert(_SglDoutIsSuspended(dout)); // dout sem surfaces é inoperante
        }
        else
        {
            AfxAssertObjects(1, &tex, afxFcc_TEX);
            //AfxAssertObjects(1, &canv, afxFcc_CANV);
            AfxReleaseObjects(1, (void*[]) { tex });
            //AfxReleaseObjects(1, (void*[]) { canv });
            dout->base.buffers[i].tex = NIL;
            //dout->base.buffers[i].canv = NIL;
        }
    }

    //dout->base.bufCnt = 0;
    return err;
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


_SGL afxError _SglDoutVmtReqCb(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    (void)timeout;
    *bufIdx = AFX_INVALID_INDEX;

    afxNat idx = dout->base.lastReqBufIdx;
    afxTime time, t2;
    AfxGetTime(&time);

    for (;;)
    {
        idx = (idx + 1) % dout->base.bufCnt;
        afxTexture tex = dout->base.buffers[idx].tex;

        if (!tex) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &tex, afxFcc_TEX);

            if (!dout->base.buffers[idx].booked)
            {
                //AfxAssert(!surf->swapchain.chain);
                dout->base.buffers[idx].booked = TRUE;
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
        dout->base.lastReqBufIdx = idx;

    return err;
}

_SGL afxError _SglDoutVmtFlushCb(afxDrawOutput dout, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    (void)timeout;
    //_SglSwapBuffersNow(dout); // do black screen

    //if (idd->fcc == afxFcc_WND || idd->fcc == afxFcc_WPP)
    {
        SetWindowTextA(dout->wnd, AfxGetStringDataConst(&dout->base.caption, 0));
        //UpdateWindow(dout->base.wglWnd);
    }
    return err;
}

_SGL afxError _SglDoutProcCb(afxDrawOutput dout, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    return err;
}

_SGL afxError _SglDoutDtor(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;

    AfxDisconnectDrawOutput(dout);

    _SglDoutSuspendFunction(dout);
    _SglDoutLockBuffers(dout);

    _SglDoutFreeAllBuffers(dout);

    DragAcceptFiles(dout->wnd, FALSE);
    ReleaseDC(dout->wnd, dout->dc);
    CloseWindow(dout->wnd);
    DestroyWindow(dout->wnd);

    AfxAssert(dout->base.swapchain.cnt == 0);

    AfxDeallocateString(&dout->base.caption);

    AfxReleaseSlock(&dout->base.buffersLock);
    AfxReleaseSlock(&dout->base.suspendSlock);

    return err;
}

_SGL _afxDoutVmt SglDoutVmtWnd =
{
    _SglDoutVmtFlushCb,
    _SglDoutVmtReqCb,
};

_SGL _afxDoutVmt SglDoutVmtDsktp =
{
    _SglDoutVmtFlushCb,
    _SglDoutVmtReqCb,
};

_SGL BOOL CALLBACK _SglFindShellWorkerWindowW32(HWND hwnd, LPARAM lParam)
{
    HWND* found = (HWND*)lParam;

    if (FindWindowExA(hwnd, NULL, "SHELLDLL_DefView", NULL))
        *found = FindWindowExA(NULL, hwnd, "WorkerW", NULL);

    return TRUE;
}

_SGL HWND _SglFindShellBackgroundWindowW32(void)
{
    // Windows 7 Method
    HWND p = FindWindow("ProgMan", NULL);
    HWND s = FindWindowEx(p, NULL, "SHELLDLL_DefView", NULL);
    HWND dtw = FindWindowEx(s, NULL, "SysListView32", NULL);
    return dtw;

    // Windows 10 Method

    SendMessageTimeoutA(FindWindowA("ProgMan", NULL), 0x052C, 0, 0, SMTO_NORMAL, 1000, NULL);
    HWND hwnd = 0;
    EnumWindows(_SglFindShellWorkerWindowW32, (LPARAM)&(hwnd));
    return hwnd;
}

static const afxString g_str_desktop = AFX_STRING_LITERAL("desktop");
static const afxString g_str_window = AFX_STRING_LITERAL("window");

_SGL afxError _SglDoutCtor(afxDrawOutput dout, afxCookie const* cookie)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;

    afxNat devId = *(afxNat const *)(cookie->udd[0]);
    afxDrawOutputConfig const *config = ((afxDrawOutputConfig const *)cookie->udd[1]) + cookie->no;

    //afxDrawDevice ddev = AfxGetDrawOutputDevice(dout);
    //AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxDrawDevice ddev = AfxGetObjectProvider(dout);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawIcd ddrv = AfxGetObjectProvider(ddev);
    afxDrawSystem dsys = AfxGetObjectProvider(ddrv);

    afxContext ctx = AfxGetDrawSystemMemory(dsys);
    AfxPushLinkage(&dout->base.dctx, NIL);

    AfxAssignFcc(dout, afxFcc_DOUT);

    dout->base.suspendCnt = 1;
    AfxAcquireSlock(&dout->base.suspendSlock);

    // absolute extent
    dout->base.resolution[0] = 1280;
    dout->base.resolution[1] = 720;
    dout->base.resolution[2] = 1;

    dout->base.extent[0] = 1280;
    dout->base.extent[1] = 720;
    dout->base.extent[2] = 1;
    dout->base.resizable = TRUE;

    // swapchain-related data
    dout->base.flags = NIL;
    dout->base.pixelFmt = AFX_PFD_RGBA8; // or AFX_PFD_RGBA8R ?
    dout->base.colorSpc = AFX_COLOR_SPACE_SRGB; // sRGB is the default
    dout->base.bufUsage = AFX_TEX_USAGE_DRAW;
    dout->base.bufferLockCnt = 1;
    dout->base.bufCnt = 2;// 3; // 2 or 3; double or triple buffered for via-memory presentation.
    dout->base.lastReqBufIdx = dout->base.bufCnt - 1; // to start at 0 instead of 1 we set to last one.
    AfxAcquireChain(&dout->base.swapchain, (void*)dout);
    AfxAcquireSlock(&dout->base.buffersLock);

    dout->base.presentAlpha = TRUE; // consider transparency for window composing.
    dout->base.presentTransform = AFX_PRESENT_TRANSFORM_FLIP_V; // NIL leaves it as it is.
    dout->base.presentMode = AFX_PRESENT_MODE_FIFO;
    dout->base.clipped = TRUE; // usually true to don't spend resources doing off-screen draw.
    dout->base.swapping = FALSE;

    AfxZeroV2d(dout->base.absCursorPos);
    AfxZeroV2d(dout->base.absCursorMove);
    AfxZeroV2d(dout->base.ndcCursorPos);
    AfxZeroV2d(dout->base.ndcCursorMove);

    if (config)
    {
        dout->base.extent[0] = config->whd[0];
        dout->base.extent[1] = config->whd[1];
        dout->base.extent[2] = config->whd[2];

        // swapchain-related data
        dout->base.pixelFmt = config->pixelFmt;
        dout->base.colorSpc = config->colorSpc;
        dout->base.bufUsage |= config->bufUsage;
        dout->base.bufCnt = config->bufCnt;

        dout->base.presentAlpha = config->presentAlpha;
        dout->base.presentTransform = config->presentTransform;
        dout->base.presentMode = config->presentMode;
        dout->base.clipped = !!config->clipped;
    }

    dout->base.auxDsFmt[0] = NIL;
    dout->base.auxDsFmt[1] = NIL;

    dout->base.vmt = NIL;
    dout->base.procCb = NIL;

    AfxAllocateString(&dout->base.caption, 512, NIL, 0);
    dout->base.buffers = NIL;

    if (config && config->udd)
        dout->base.udd = config->udd;

    //AfxAssert(ddrvIdd->wglPrimeGlrc == _wglGetCurrentContext());
    //AfxAssert(ddrvIdd->wglPrimeDc == _wglGetCurrentDC());
    afxNat unitIdx;
    AfxGetThreadingUnit(&unitIdx);
    sglDpuIdd *dpu = &ddev->dpus[unitIdx];
    //wglVmt const*wgl = &dpu->wgl;

    HGLRC rc = dpu->GetCurrentContext();

    if (rc != dpu->glrc)
    {
        if (!dpu->MakeCurrent(dpu->dc, dpu->glrc))
        {
            AfxThrowError();
            AfxError("DPU %u failed to be set.", unitIdx);
            dpu->MakeCurrent(dpu->dc, dpu->glrc);
        }
    }
    afxString title;
    AfxWrapStringLiteral(&title, ddev->wndClss.lpszClassName, 0);

    afxBool isDesk = FALSE;
    afxBool isWnd = FALSE;

    afxUri name;

    if (config->endpoint)
        AfxExcerptUriName(&name, config->endpoint);
    else
        AfxResetUri(&name);

    dout->base.vmt = &SglDoutVmtWnd;
    dout->base.procCb = _SglDoutProcCb;

    afxString const *surface = AfxUriGetStringConst(&name);

    if (AfxStringIsEmpty(surface) || 0 == AfxCompareString(surface, &g_str_window)) // print to window surface
    {
        dout->wnd = CreateWindowEx(WS_EX_CONTEXTHELP, ddev->wndClss.lpszClassName, ddev->wndClss.lpszClassName, /*WS_POPUP*/WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NIL, NIL, ddev->wndClss.hInstance, NIL);
        isWnd = TRUE;
    }
    else if (0 == AfxCompareString(surface, &g_str_desktop))
    {
        dout->wnd = _SglFindShellBackgroundWindowW32();// GetDesktopWindow();
        isDesk = TRUE;
    }

    if (!(dout->wnd)) AfxThrowError();
    else
    {
        SetWindowLongPtr(dout->wnd, GWLP_USERDATA, (LONG_PTR)dout);

        dout->dc = GetDC(dout->wnd);
        //hdc = GetWindowDC(hwnd);

        if (!(dout->dc)) AfxThrowError();
        else
        {
            afxWhd const resolution = { GetDeviceCaps(dout->dc, HORZRES), GetDeviceCaps(dout->dc, VERTRES), GetDeviceCaps(dout->dc, PLANES) };
            dout->base.refreshRate = GetDeviceCaps(dout->dc, VREFRESH);
            afxReal64 physAspRatio = (afxReal64)GetDeviceCaps(dout->dc, HORZSIZE) / (afxReal64)GetDeviceCaps(dout->dc, VERTSIZE);
            AfxReadjustDrawOutputProportion(dout, physAspRatio, resolution);

            if (isWnd)
                AfxReadjustDrawOutputNdc(dout, AfxSpawnV3d(0.5, 0.5, 1));
            else
                AfxReadjustDrawOutputNdc(dout, AfxSpawnV3d(1, 1, 1));

            afxNat bpp = 0;

            if (!dout->base.pixelFmt)
            {
                bpp = GetDeviceCaps(dout->dc, BITSPIXEL);

                switch (bpp)
                {
                case 8: dout->base.pixelFmt = AFX_PFD_R8; break;
                case 16: dout->base.pixelFmt = AFX_PFD_GR8; break;
                case 24: dout->base.pixelFmt = AFX_PFD_BGR8; break;
                case 32: dout->base.pixelFmt = AFX_PFD_BGRA8; break;
                default: dout->base.pixelFmt = AFX_PFD_ARGB8; break;
                }
            }

            afxPixelLayout pfl;
            AfxDescribePixelFormat(dout->base.pixelFmt, &pfl);
            bpp = pfl.bpp;

            afxNat alphaBpp = pfl.bpc[3];

            if (!dout->base.bufCnt)
                dout->base.bufCnt = 1;

            if (!dout->base.bufUsage)
                dout->base.bufUsage = AFX_TEX_USAGE_DRAW;

            int pxlAttrPairs[][2] =
            {
                { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
                { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
                { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
                { WGL_DOUBLE_BUFFER_ARB, TRUE }, // single buffer not supported anymore today. Qwadro will just virtualizes it.
                { WGL_PIXEL_TYPE_ARB, AfxPixelFormatIsReal(dout->base.pixelFmt) ? WGL_TYPE_RGBA_FLOAT_ARB : WGL_TYPE_RGBA_ARB },
                { WGL_COLOR_BITS_ARB, bpp },
                { WGL_ALPHA_BITS_ARB, alphaBpp },
                //{ WGL_DEPTH_BITS_ARB, 24 }, // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                //{ WGL_STENCIL_BITS_ARB, 8 },  // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                { WGL_TRANSPARENT_ARB, (dout->base.presentAlpha != AFX_PRESENT_ALPHA_OPAQUE) },
                { WGL_SWAP_METHOD_ARB, (dout->base.presentMode == AFX_PRESENT_MODE_IMMEDIATE) ? WGL_SWAP_COPY_ARB : WGL_SWAP_EXCHANGE_ARB },
                //{ WGL_SAMPLE_BUFFERS_ARB,  GL_TRUE },  // works on Intel, didn't work on Mesa
                //{ WGL_SAMPLES_ARB, 8 }, // works on Intel, didn't work on Mesa
                //{ WGL_COLORSPACE_EXT, dout->base.colorSpc == AFX_COLOR_SPACE_SRGB ? WGL_COLORSPACE_SRGB_EXT : (dout->base.colorSpc == AFX_COLOR_SPACE_LINEAR ? WGL_COLORSPACE_LINEAR_EXT : NIL) }, // works on Mesa, didn't work on Intel
                //{ WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, (dout->base.colorSpc == AFX_COLOR_SPACE_SRGB) }, // works on Mesa, didn't work on Intel
                { NIL, NIL },
            };

            PFNWGLCHOOSEPIXELFORMATARBPROC _wglChoosePixelFormatARB = (void*)dpu->GetProcAddress("wglChoosePixelFormatARB");
            AfxAssert(_wglChoosePixelFormatARB);
            UINT formatCount;
            dout->dcPxlFmt = 0;

            if (!_wglChoosePixelFormatARB(dout->dc, &pxlAttrPairs[0][0], 0, 1, &(dout->dcPxlFmt), &(formatCount))) AfxThrowError();
            else
            {
                AfxAssert(dout->dcPxlFmt);
                AfxAssert(formatCount);

                PIXELFORMATDESCRIPTOR pfd;
                AFX_ZERO(&pfd);
                SglDescribePixelFormat(dout->dc, dout->dcPxlFmt, sizeof(pfd), &pfd, dpu);
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
                //dout->base.wglDcPxlFmt = _SglChoosePixelFormat(dout->base.wglDc, &(pfd));

                if (!SglSetPixelFormat(dout->dc, dout->dcPxlFmt, &(pfd), dpu)) AfxThrowError();
                else
                {
                    if (dout->base.presentAlpha)
                    {
                        DWM_BLURBEHIND bb = { 0 };
                        bb.dwFlags = DWM_BB_ENABLE;
                        bb.fEnable = TRUE;
                        bb.hRgnBlur = NULL;
                        DwmEnableBlurBehindWindow(dout->wnd, &(bb));
                    }

                    //DragAcceptFiles(dout->base.wglWnd, TRUE);

                    if (isWnd)
                    {
                        SetWindowPos(dout->wnd, NULL, 0, 0, dout->base.resolution[0] / 2, dout->base.resolution[1] / 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
                    }

                    {

                        AfxAssert(dout->base.vmt);

                        AfxAssert(dout->base.resolution[0]);
                        AfxAssert(dout->base.resolution[1]);
                        AfxAssert(dout->base.resolution[2]);
                        AfxAssertExtent(dout->base.resolution[0], dout->base.extent[0]);
                        AfxAssertExtent(dout->base.resolution[1], dout->base.extent[1]);
                        AfxAssertExtent(dout->base.resolution[2], dout->base.extent[2]);

                        AfxAssert(dout->base.bufCnt);
                        AfxAssert(dout->base.bufUsage & AFX_TEX_USAGE_DRAW);
                        AfxAssert(dout->base.refreshRate);
                        AfxAssert(dout->base.wpOverHp);
                        AfxAssert(dout->base.wrOverHr);
                        AfxAssert(dout->base.wwOverHw);

                        if (!dout->base.buffers && !(dout->base.buffers = AfxAllocate(ctx, dout->base.bufCnt * sizeof(dout->base.buffers[0]), 0, AfxSpawnHint()))) AfxThrowError();
                        else for (afxNat i = 0; i < dout->base.bufCnt; i++)
                            dout->base.buffers[i].tex = NIL, /*dout->base.buffers[i].canv = NIL,*/ dout->base.buffers[i].booked = FALSE;

                        if (err && dout->base.buffers)
                        {
                            _SglDoutFreeAllBuffers(dout);
                            AfxDeallocate(ctx, dout->base.buffers);
                        }
                    }

                    UpdateWindow(dout->wnd);
                    ShowWindow(dout->wnd, SHOW_OPENWINDOW);

                    dout->instanced = TRUE;
                }
            }

            if (err)
                ReleaseDC(dout->wnd, dout->dc);
        }

        if (err && isWnd)
            DestroyWindow(dout->wnd);
    }
    return err;
}

_SGL afxClassConfig _SglDoutClsConfig =
{
    .fcc = afxFcc_DOUT,
    .name = "Draw Output",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawOutput)),
    .ctx = NIL,
    .ctor = (void*)_SglDoutCtor,
    .dtor = (void*)_SglDoutDtor
};
