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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
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
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/core/afxTxu.h"
#include "qwadro/draw/afxDrawOutput.h"
#include "qwadro/core/afxString.h"
#include "qwadro/math/afxVector.h"

_SGL afxNat _SglDoutBuffersAreLocked(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->buffersLock);
    afxNat bufferLockCnt = dout->bufferLockCnt;
    AfxExitSlockShared(&dout->buffersLock);
    return bufferLockCnt;
}

_SGL afxNat _SglDoutLockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = ++dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
}

_SGL afxNat _SglDoutUnlockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = --dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
}

_SGL afxNat _SglDoutIsSuspended(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->suspendSlock);
    afxNat suspendCnt = dout->suspendCnt;
    AfxExitSlockShared(&dout->suspendSlock);
    return suspendCnt;
}

_SGL afxNat _SglDoutSuspendFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = ++dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_SGL afxNat _SglDoutResumeFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = --dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_SGL afxError _SglDoutFreeAllBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    for (afxNat i = 0; i < dout->bufCnt; i++)
    {
        afxCanvas canv = dout->buffers[i].canv;
        //afxRaster tex = dout->buffers[i].ras;

        if (/*!tex*/!canv)
        {
            AfxAssert(_SglDoutIsSuspended(dout)); // dout sem surfaces é inoperante
        }
        else
        {
            //AfxAssertObjects(1, &tex, afxFcc_RAS);
            AfxAssertObjects(1, &canv, afxFcc_CANV);
            //AfxReleaseObjects(1, (void*[]) { tex });
            AfxReleaseObjects(1, (void*[]) { canv });
            //dout->buffers[i].ras = NIL;
            dout->buffers[i].canv = NIL;
        }
    }

    //dout->bufCnt = 0;
    return err;
}

_SGL void Calc_window_values(HWND window, afxInt* out_extra_width, afxInt32* out_extra_height)
{
    RECT r, c;

    // Get the current window rect (in screen space).
    GetWindowRect(window, &r);
    // Get the client rectangle of the window.
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

    afxNat idx = dout->lastReqBufIdx;
    afxTime time, t2;
    AfxGetTime(&time);

    for (;;)
    {
        idx = (idx + 1) % dout->bufCnt;
        //afxRaster tex = dout->buffers[idx].ras;
        afxCanvas canv = dout->buffers[idx].canv;

        //if (!tex) AfxThrowError();
        if (!canv) AfxThrowError();
        else
        {
            //AfxAssertObjects(1, &tex, afxFcc_RAS);
            AfxAssertObjects(1, &canv, afxFcc_CANV);

            if (!dout->buffers[idx].booked)
            {
                //AfxAssert(!surf->swapchain.chain);
                dout->buffers[idx].booked = TRUE;
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

_SGL afxError _SglDoutVmtFlushCb(afxDrawOutput dout, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    (void)timeout;
    //_SglSwapBuffersNow(dout); // do black screen

    //if (idd->fcc == afxFcc_WND || idd->fcc == afxFcc_WPP)
    {
        SetWindowTextA(dout->idd->wnd, AfxGetStringData(&dout->caption.str, 0));
        //UpdateWindow(dout->wglWnd);
    }
    return err;
}

_SGL afxError _SglDdevDeinitDout(afxDrawDevice ddev, afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;

    AfxDisconnectDrawOutput(dout);

    _SglDoutSuspendFunction(dout);
    _SglDoutLockBuffers(dout);

    DragAcceptFiles(dout->idd->wnd, FALSE);
    ReleaseDC(dout->idd->wnd, dout->idd->dc);
    CloseWindow(dout->idd->wnd);
    DestroyWindow(dout->idd->wnd);
    AfxDeallocate(dout->idd);
    dout->idd = NIL;

    return err;
}

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

_SGL afxError _SglDdevInitDout(afxDrawDevice ddev, afxDrawOutput dout, afxDrawOutputConfig const* config, afxUri const* endpoint)
{
    afxError err = AFX_ERR_NONE;

    //AfxAssert(ddrvIdd->wglPrimeGlrc == _wglGetCurrentContext());
    //AfxAssert(ddrvIdd->wglPrimeDc == _wglGetCurrentDC());
    afxNat unitIdx;
    AfxGetThreadingUnit(&unitIdx);
    sglDpuIdd *dpu = &ddev->idd->dpus[unitIdx];
    //wglVmt const*wgl = &dpu->wgl;

    HGLRC rc = dpu->wgl.GetCurrentContext();

    if (rc != dpu->glrc)
    {
        if (!dpu->wgl.MakeCurrent(dpu->dc, dpu->glrc))
        {
            AfxThrowError();
            AfxLogError("DPU %u failed to be set.", unitIdx);
            dpu->wgl.MakeCurrent(dpu->dc, dpu->glrc);
        }
    }
    afxString title;
    AfxMakeString(&title, ddev->idd->wndClss.lpszClassName, 0);

    afxBool isDesk = FALSE;
    afxBool isWnd = FALSE;

    afxUri name;

    if (endpoint)
        AfxPickUriObject(endpoint, &name);
    else
        AfxResetUri(&name);

    dout->lockCb = _SglDoutVmtReqCb;
    
    afxString const *surface = AfxGetUriString(&name);

    dout->idd = AfxAllocate(1, sizeof(dout->idd[0]), 0, AfxHint());

    if (AfxStringIsEmpty(surface) || 0 == AfxCompareString(surface, &AfxStaticString("window"))) // print to window surface
    {
        dout->idd->wnd = CreateWindowEx(WS_EX_CONTEXTHELP, ddev->idd->wndClss.lpszClassName, ddev->idd->wndClss.lpszClassName, /*WS_POPUP*/WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NIL, NIL, ddev->idd->wndClss.hInstance, NIL);
        isWnd = TRUE;
    }
    else if (0 == AfxCompareString(surface, &AfxStaticString("desktop")))
    {
        dout->idd->wnd = _SglFindShellBackgroundWindowW32();// GetDesktopWindow();
        isDesk = TRUE;
    }

    if (!(dout->idd->wnd)) AfxThrowError();
    else
    {
        SetWindowLongPtr(dout->idd->wnd, GWLP_USERDATA, (LONG_PTR)dout);

        dout->idd->dc = GetDC(dout->idd->wnd);
        //hdc = GetWindowDC(hwnd);

        if (!(dout->idd->dc)) AfxThrowError();
        else
        {
            afxWhd const resolution = { GetDeviceCaps(dout->idd->dc, HORZRES), GetDeviceCaps(dout->idd->dc, VERTRES), GetDeviceCaps(dout->idd->dc, PLANES) };
            dout->refreshRate = GetDeviceCaps(dout->idd->dc, VREFRESH);
            afxReal64 physAspRatio = (afxReal64)GetDeviceCaps(dout->idd->dc, HORZSIZE) / (afxReal64)GetDeviceCaps(dout->idd->dc, VERTSIZE);
            AfxAdjustDrawOutputProportion(dout, physAspRatio, resolution);

            if (isWnd)
                AfxAdjustDrawOutputNdc(dout, AfxSpawnV3d(0.6666666, 0.6666666, 1));
            else
                AfxAdjustDrawOutputNdc(dout, AfxSpawnV3d(1, 1, 1));

            afxNat bpp = 0;

            if (!dout->pixelFmt)
            {
                bpp = GetDeviceCaps(dout->idd->dc, BITSPIXEL);

                switch (bpp)
                {
                case 8: dout->pixelFmt = afxPixelFormat_R8; break;
                case 16: dout->pixelFmt = afxPixelFormat_GR8; break;
                case 24: dout->pixelFmt = afxPixelFormat_BGR8; break;
                case 32: dout->pixelFmt = afxPixelFormat_BGRA8; break;
                default: dout->pixelFmt = afxPixelFormat_ARGB8; break;
                }
            }

            afxPixelLayout pfl;
            AfxDescribePixelFormat(dout->pixelFmt, &pfl);
            bpp = pfl.bpp;

            afxNat alphaBpp = pfl.bpc[3];

            if (!dout->bufCnt)
                dout->bufCnt = 1;

            if (!dout->bufUsage)
                dout->bufUsage = afxRasterUsage_DRAW;

            int pxlAttrPairs[][2] =
            {
                { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
                { WGL_SUPPORT_OPENGL_ARB, GL_TRUE }, // suportar o que se não OpenGL na fucking API do OpenGL? Direct3D, filha da puta?!
                { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
                { WGL_DOUBLE_BUFFER_ARB, TRUE }, // single buffer not supported anymore today. Qwadro will just virtualizes it.
                { WGL_PIXEL_TYPE_ARB, AfxPixelFormatIsReal(dout->pixelFmt) ? WGL_TYPE_RGBA_FLOAT_ARB : WGL_TYPE_RGBA_ARB },
                { WGL_COLOR_BITS_ARB, bpp },
                { WGL_ALPHA_BITS_ARB, alphaBpp },
                //{ WGL_DEPTH_BITS_ARB, 24 }, // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                //{ WGL_STENCIL_BITS_ARB, 8 },  // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                { WGL_TRANSPARENT_ARB, (dout->presentAlpha && dout->presentAlpha != afxPresentAlpha_OPAQUE) },
                { WGL_SWAP_METHOD_ARB, (dout->presentMode && dout->presentMode == afxPresentMode_IMMEDIATE) ? WGL_SWAP_COPY_ARB : WGL_SWAP_EXCHANGE_ARB },
                { WGL_SAMPLE_BUFFERS_ARB,  GL_FALSE },  // works on Intel, didn't work on Mesa
                //{ WGL_SAMPLES_ARB, 8 }, // works on Intel, didn't work on Mesa
                //{ WGL_COLORSPACE_EXT, dout->colorSpc == afxColorSpace_SRGB ? WGL_COLORSPACE_SRGB_EXT : (dout->colorSpc == afxColorSpace_LINEAR ? WGL_COLORSPACE_LINEAR_EXT : NIL) }, // works on Mesa, didn't work on Intel
                //{ WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, (dout->colorSpc == afxColorSpace_SRGB) }, // works on Mesa, didn't work on Intel
                { NIL, NIL },
            };

            UINT formatCount;
            dout->idd->dcPxlFmt = 0;
            
            if (!dpu->wgl.ChoosePixelFormatARB(dout->idd->dc, &pxlAttrPairs[0][0], 0, 1, &(dout->idd->dcPxlFmt), &(formatCount))) AfxThrowError();
            else
            {
                AfxAssert(dout->idd->dcPxlFmt);
                AfxAssert(formatCount);

                PIXELFORMATDESCRIPTOR pfd;
                AfxZero2(1, sizeof(pfd), &pfd);
                SglDescribePixelFormat(dout->idd->dc, dout->idd->dcPxlFmt, sizeof(pfd), &pfd, dpu);
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
                DWORD pfdFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DIRECT3D_ACCELERATED | PFD_SWAP_EXCHANGE /* | PFD_SUPPORT_COMPOSITION*/;
                AfxAssert(dpu->dcPfd.dwFlags & pfdFlags);

                if (!SglSetPixelFormat(dout->idd->dc, dout->idd->dcPxlFmt, &(pfd), dpu)) AfxThrowError();
                else
                {
                    if (dout->presentAlpha)
                    {
                        DWM_BLURBEHIND bb = { 0 };
                        bb.dwFlags = DWM_BB_ENABLE;
                        bb.fEnable = TRUE;
                        bb.hRgnBlur = NULL;
                        DwmEnableBlurBehindWindow(dout->idd->wnd, &(bb)); // não functiona no chad Windows XP
                    }

                    //DragAcceptFiles(dout->wglWnd, TRUE);
                    if (isWnd)
                    {
#if 0
                        SetWindowPos(dout->wnd, NULL, AfxUnndcf(0.166666666666666, dout->resolution[0]), AfxUnndcf(0.166666666666666, dout->resolution[1]), dout->extent[0], dout->extent[1], 0);
                        AfxAdjustDrawOutputNdc(dout, AfxSpawnV3d(0.6666666, 0.6666666, 1));
#else
                        SetWindowPos(dout->idd->wnd, NULL, 0, 0, dout->whd[0], dout->whd[1], SWP_NOMOVE);
                        afxInt32 extraWndWidth, extraWndHeight;
                        Calc_window_values(dout->idd->wnd, &extraWndWidth, &extraWndHeight);
                        SetWindowPos(dout->idd->wnd, NULL, 0, 0, dout->whd[0] + extraWndWidth, dout->whd[1] + extraWndHeight, SWP_NOMOVE);
#endif
                    }

                    {
                        AfxAssert(dout->resolution[0]);
                        AfxAssert(dout->resolution[1]);
                        AfxAssert(dout->resolution[2]);
                        AfxAssertExtent(dout->resolution[0], dout->whd[0]);
                        AfxAssertExtent(dout->resolution[1], dout->whd[1]);
                        AfxAssertExtent(dout->resolution[2], dout->whd[2]);

                        AfxAssert(dout->bufCnt);
                        AfxAssert(dout->bufUsage & afxRasterUsage_DRAW);
                        AfxAssert(dout->refreshRate);
                        AfxAssert(dout->wpOverHp);
                        AfxAssert(dout->wrOverHr);
                        AfxAssert(dout->wwOverHw);
                    }

                    UpdateWindow(dout->idd->wnd);
                    ShowWindow(dout->idd->wnd, SHOW_OPENWINDOW);

                    dout->idd->instanced = TRUE;
                }
            }

            if (err)
                ReleaseDC(dout->idd->wnd, dout->idd->dc);
        }

        if (err && isWnd)
            DestroyWindow(dout->idd->wnd);
    }
    return err;
}
