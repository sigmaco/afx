/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
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
#include "qwadro/draw/dev/afxDrawOutput.h"
#include "qwadro/core/afxString.h"
#include "qwadro/math/afxVector.h"

_SGL afxNat _SglDoutBuffersAreLocked(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->buffersLock);
    afxNat lockedBufCnt = dout->lockedBufCnt;
    AfxExitSlockShared(&dout->buffersLock);
    return lockedBufCnt;
}

_SGL afxNat _SglDoutLockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat lockedBufCnt = AfxIncAtom32(&dout->lockedBufCnt);
    AfxExitSlockExclusive(&dout->buffersLock);
    return lockedBufCnt;
}

_SGL afxNat _SglDoutUnlockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat lockedBufCnt = AfxDecAtom32(&dout->lockedBufCnt);
    AfxExitSlockExclusive(&dout->buffersLock);
    return lockedBufCnt;
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

_SGL afxBool _SglDouVmtGetIddCb(afxDrawOutput dout, afxNat code, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    switch (code)
    {
    case 0:
    {
        *(HDC*)dst = dout->w32.hDc;
        break;
    }
    case 1:
    {
        *(int*)dst = dout->w32.dcPxlFmt;
        break;
    }
    default: break;
    }
    return FALSE;
}

_SGL afxError _SglDoutVmtReqCb(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    (void)timeout;
    *bufIdx = AFX_INVALID_INDEX;

    afxBool success = FALSE;
    afxTime time, t2;
    AfxGetTime(&time);

    afxClock start, last;
    
    if (timeout)
    {
        AfxGetClock(&start);
        last = start;
    }

    while (1)
    {
        afxNat lastLockedBufIdx = AfxLoadAtom32(&dout->lastLockedBufIdx);
        afxNat bufCnt2 = dout->bufCnt;

        while (bufCnt2--)
        {
            lastLockedBufIdx = (lastLockedBufIdx + 1) % dout->bufCnt;
            afxInt32 boolFalse = FALSE;

            if (AfxCasAtom32(&dout->buffers[lastLockedBufIdx].booked, &boolFalse, TRUE))
            {
                AfxIncAtom32(&dout->lockedBufCnt);
                afxCanvas canv = dout->buffers[lastLockedBufIdx].canv;

                if (canv)
                {
                    AfxAssertObjects(1, &canv, afxFcc_CANV);

                    *bufIdx = lastLockedBufIdx;
                    AfxStoreAtom32(&dout->lastLockedBufIdx, lastLockedBufIdx);
                    success = TRUE;
                    break;
                }
            }
        }

        if (success)
            break;

        if (!success && (!timeout || timeout < AfxGetTime(&t2) - time))
        {
            err = __LINE__;
            *bufIdx = AFX_INVALID_INDEX;
            break;
        }
    }
    return err;
}

_SGL afxError _SglDdevDeinitDoutCb(afxDrawDevice ddev, afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    _SglDoutSuspendFunction(dout);
    _SglDoutLockBuffers(dout);

    ReleaseDC(dout->w32.hWnd, dout->w32.hDc);

    if (dout->idd)
    {
        AfxDeallocate(dout->idd);
        dout->idd = NIL;
    }
    return err;
}

_SGL afxError _SglActivateDout(sglDpu* dpu, afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    afxBool fallback = TRUE;
    HDC hDc = dout->w32.hDc;
    HDC currDc = dpu->wgl.GetCurrentDC();

    if (hDc)
    {
        if (!dout->w32.dcPxlFmt)
        {
            afxPixelLayout pfl;
            afxPixelFormat pixelFmt = dout->pixelFmt;
            AfxDescribePixelFormat(pixelFmt, &pfl);
            afxNat colorBpp = pfl.bpp;
            afxNat alphaBpp = pfl.bpc[3];

            HWND hDskWnd = GetDesktopWindow();
            HDC hDskDc = GetDC(hDskWnd);
            PIXELFORMATDESCRIPTOR pfd2;
            int dskPf = SglGetPixelFormat(hDskDc, &dpu->wgl);
            SglDescribePixelFormat(hDskDc, dskPf, sizeof(pfd2), &pfd2, &dpu->wgl);


            int pxlAttrPairCnt = 0;
            int pxlAttrPairs[][2] =
            {
                { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
                { WGL_SUPPORT_OPENGL_ARB, GL_TRUE }, // suportar o que se não OpenGL na fucking API do OpenGL? Direct3D, filha da puta?
                { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
                { WGL_DOUBLE_BUFFER_ARB, TRUE }, // single buffer not supported anymore today. Qwadro will just virtualizes it.
                { WGL_PIXEL_TYPE_ARB, AfxPixelFormatIsReal(pixelFmt) ? WGL_TYPE_RGBA_FLOAT_ARB : WGL_TYPE_RGBA_ARB },
                { WGL_COLOR_BITS_ARB, colorBpp },
                { WGL_ALPHA_BITS_ARB, alphaBpp },
                //{ WGL_DEPTH_BITS_ARB, 24 }, // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                //{ WGL_STENCIL_BITS_ARB, 8 },  // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                //{ WGL_TRANSPARENT_ARB, (dout->presentAlpha && dout->presentAlpha != afxPresentAlpha_OPAQUE) },
                //{ WGL_SWAP_METHOD_ARB, (dout->presentMode && dout->presentMode == afxPresentMode_IMMEDIATE) ? WGL_SWAP_COPY_ARB : WGL_SWAP_EXCHANGE_ARB },
                //{ WGL_SAMPLE_BUFFERS_ARB,  GL_FALSE },  // works on Intel, didn't work on Mesa
                //{ WGL_SAMPLES_ARB, 8 }, // works on Intel, didn't work on Mesa
                //{ WGL_COLORSPACE_EXT, dout->colorSpc == afxColorSpace_SRGB ? WGL_COLORSPACE_SRGB_EXT : (dout->colorSpc == afxColorSpace_LINEAR ? WGL_COLORSPACE_LINEAR_EXT : NIL) }, // works on Mesa, didn't work on Intel
                //{ WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, (dout->colorSpc == afxColorSpace_SRGB) }, // works on Mesa, didn't work on Intel
                { NIL, NIL },
            };

            UINT fmtCnt;
            int dcPxlFmt = 0;

            if (!dpu->wgl.ChoosePixelFormatARB(hDc, &pxlAttrPairs[0][0], 0, 1, &dcPxlFmt, &fmtCnt)) AfxThrowError();
            else
            {
                AfxAssert(dcPxlFmt);
                AfxAssert(fmtCnt);

                PIXELFORMATDESCRIPTOR pfd;
                AfxZero2(1, sizeof(pfd), &pfd);
                SglDescribePixelFormat(hDc, dcPxlFmt, sizeof(pfd), &pfd, &dpu->wgl);
                dout->w32.dcPxlFmtBkp = SglGetPixelFormat(hDc, &dpu->wgl);
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
                AfxAssert(pfd.dwFlags & pfdFlags);

                if (!SglSetPixelFormat(hDc, dcPxlFmt, &(pfd), &dpu->wgl)) AfxThrowError();
                else
                {
                    dout->w32.dcPxlFmt = dcPxlFmt;
                    fallback = FALSE;
                }
            }
        }
        else
        {
            fallback = FALSE;
        }
    }

    if (fallback)
        hDc = dpu->dc;

    if (currDc != hDc)
    {
        if (!dpu->wgl.MakeCurrent(hDc, dpu->glrc))
        {
            AfxThrowError();

            if (!fallback)
            {
                AfxLogError("DPU %u failed to set to draw output device %u.", dpu->portIdx, AfxGetObjectId(dout));
            }
            else
            {
                AfxLogError("Fallback failure on DPU %u.", dpu->portIdx);
            }

            //if (AfxDisconnectDrawOutput(dout))
                //AfxThrowError();

            if (!dpu->wgl.MakeCurrent(dpu->dc, dpu->glrc))
            {
                AfxThrowError();
                AfxLogError("Fallback failure on DPU %u.", dpu->portIdx);

                if (dout->presentMode == afxPresentMode_FIFO)
                    dpu->wgl.SwapIntervalEXT(0); // revert V-sync
            }
        }
        else
        {
#if !0
            if (dout->presentMode == afxPresentMode_FIFO)
            {
                dpu->wgl.SwapIntervalEXT(1); // causes lag when 1
            }
            else
            {
                dpu->wgl.SwapIntervalEXT(0);
            }
#endif
        }
    }
    return err;
}

_SGL afxError _SglDdevInitDoutCb(afxDrawDevice ddev, afxDrawOutput dout, afxDrawOutputConfig const* cfg, afxUri const* endpoint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    AfxZero(&dout->w32, sizeof(dout->w32));

    dout->lockCb = _SglDoutVmtReqCb;
    dout->getIddCb = _SglDouVmtGetIddCb;

    HWND wnd = cfg->w32.hwnd;

    if (wnd)
    {
        HDC dc = GetDC(wnd);
        //HDC wndDc = GetWindowDC(wnd);

        if (!dc) AfxThrowError();
        else
        {
            afxWhd const screenRes =
            {
                GetDeviceCaps(dc, HORZRES),
                GetDeviceCaps(dc, VERTRES),
                GetDeviceCaps(dc, PLANES)
            };
            afxReal64 physAspRatio = (afxReal64)GetDeviceCaps(dc, HORZSIZE) / (afxReal64)GetDeviceCaps(dc, VERTSIZE);
            afxNat refreshRate = GetDeviceCaps(dc, VREFRESH);
            AfxAdaptDrawOutputResolution(dout, screenRes, refreshRate, physAspRatio);

            //AfxAdjustDrawOutputNdc(dout, AfxSpawnV3d(0.6666666, 0.6666666, 1));

            dout->w32.hInst = cfg->w32.hinstance;
            dout->w32.hWnd = cfg->w32.hwnd;
            dout->w32.hDc = dc;

            if (cfg->udd)
                ((HDC*)cfg->udd)[0] = dc;

            if (err)
                ReleaseDC(wnd, dc);
        }
    }
    return err;
}
