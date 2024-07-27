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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <shellapi.h>
#include <dwmapi.h>
#include <stdarg.h>
#include <stdio.h>
//#pragma comment(lib,"dwmapi.lib")
//#pragma comment(lib, "opengl32")

#include "sgl.h"

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
        *(HDC*)dst = dout->idd->hDc;
        break;
    }
    case 1:
    {
        *(int*)dst = dout->idd->dcPxlFmt;
        break;
    }
    default: break;
    }
    return FALSE;
}

_SGL afxError _DpuPresentDout(sglDpu* dpu, afxDrawOutput dout, afxNat outBufIdx)
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx = dpu->activeDctx;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    AfxAssert(dout->presentingBufIdx == (afxAtom32)AFX_INVALID_INDEX);

    if (_SglActivateDout(dpu, dout)) AfxThrowError();
    else
    {
        dout->presentingBufIdx = outBufIdx;

        //wglVmt const*wgl = &dpu->wgl;
        
        afxRaster surf;
        AfxEnumerateDrawOutputBuffers(dout, outBufIdx, 1, &surf);
        avxCanvas canv;
        AfxEnumerateDrawOutputCanvases(dout, outBufIdx, 1, &canv);
        AfxAssertObjects(1, &surf, afxFcc_RAS);
        //AfxAssert(surf->state == AFX_SURF_STATE_PENDING);
        glVmt const* gl = &dpu->gl;
#if !0
#if 0
        dpu->activeTmpFboIdx = (dpu->activeTmpFboIdx + 1) % (sizeof(dpu->tmpFbo) / sizeof(dpu->tmpFbo[0]));
        GLuint tmpFbo = dpu->tmpFbo[dpu->activeTmpFboIdx];

        if (!tmpFbo)
        {
            gl->GenFramebuffers(1, &(tmpFbo)); _SglThrowErrorOccuried();
        }
        else
        {
            gl->DeleteFramebuffers(1, &(tmpFbo)); _SglThrowErrorOccuried();
            gl->GenFramebuffers(1, &(tmpFbo)); _SglThrowErrorOccuried();
        }
        gl->BindFramebuffer(GL_READ_FRAMEBUFFER, tmpFbo); _SglThrowErrorOccuried();
        AfxAssert(gl->IsFramebuffer(tmpFbo));
        gl->FramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, surf->glTarget, surf->glHandle, 0); _SglThrowErrorOccuried();
#else
        dpu->activeRasterState.pass.canv = NIL;
        gl->BindFramebuffer(GL_READ_FRAMEBUFFER, canv->glHandle); _SglThrowErrorOccuried();
#endif
        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        gl->Clear(GL_COLOR_BUFFER_BIT);  _SglThrowErrorOccuried();
        afxBool useLerp = (surf->m.whd[0] != canv->m.whd[0]) && (surf->m.whd[1] != canv->m.whd[1]);

        afxInt x = (dout->presentTransform & avxPresentTransform_FLIP_H) ? surf->m.whd[0] : 0;
        afxInt y = (dout->presentTransform & avxPresentTransform_FLIP_V) ? surf->m.whd[1] : 0;
        afxInt w = (dout->presentTransform & avxPresentTransform_FLIP_H) ? 0 : surf->m.whd[0];
        afxInt h = (dout->presentTransform & avxPresentTransform_FLIP_V) ? 0 : surf->m.whd[1];

        gl->BlitFramebuffer(0, 0, surf->m.whd[0], surf->m.whd[1], x, y, w, h, GL_COLOR_BUFFER_BIT, useLerp ? GL_LINEAR : GL_NEAREST); _SglThrowErrorOccuried();
        gl->BindFramebuffer(GL_READ_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        //gl->Flush(); _SglThrowErrorOccuried();
#else

#if !0
        AfxAssert(gl->ClipControl);

        //gl->ClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE); _SglThrowErrorOccuried();// set screen origin to top to bottm, and set depth to [ 0, 1 ]

        afxWhd extent;
        AfxGetRasterExtent(surf, 0, extent);

        _sglCmdBeginSynthesis cmdBeginOp = { 0 };
        cmdBeginOp.defFbo = TRUE;
        cmdBeginOp.canv = NIL;
        cmdBeginOp.area = (afxRect const) { { { 0, 0 } }, { { extent[0], extent[1] } } };
        cmdBeginOp.rasterCnt = 1;
        cmdBeginOp.rasters[0] = (avxDrawTarget const) { .loadOp = avxLoadOp_CLEAR, .storeOp = avxStoreOp_STORE, .clearValue = { .color = { 0.3, 0.1, 0.3, 1 } } };

        _DpuBeginSynthesis(dpu, &cmdBeginOp);

        _sglCmdPipeline cmdBindPip = { 0 };
        cmdBindPip.pip = dctx->presentPip;
        _DpuBindPipeline(dpu, &cmdBindPip);

#else
        avxDrawTarget const rasterRt = { NIL, avxLoadOp_CLEAR, NIL, { 0.3, 0.1, 0.3, 1.0 } };
        _DpuBeginCombination(dpu, NIL, 1, 0, &rasterRt, NIL, NIL);

        avxPipeline pip = AfxDrawOperationGetPipeline(idd->presentDop, 0, 0);
        AfxAssertObjects(1, &pip, afxFcc_PIP);
        _DpuBindPipeline(dpu, pip);
#endif   
        //_DpuEmployTechnique(ddge, 0);

        _sglCmdViewport cmdSetVp = { 0 };
        cmdSetVp.cnt = 1;
        cmdSetVp.vp[0].extent[0] = extent[0];
        cmdSetVp.vp[0].extent[1] = extent[1];
        cmdSetVp.vp[0].depth[1] = 1;
        cmdSetVp.reset = TRUE;

        _DpuSetViewports(dpu, &cmdSetVp);

#if 0 // already set by pipeline
        avxPipelineRasterizerState const ras = { FALSE, FALSE, avxFillMode_SOLID, avxCullMode_BACK, avxFrontFace_CCW, FALSE, 0, 0, 0, 1 };
        _DpuSetRasterizerState(ddge, &ras);

        avxPipelineDepthState const depth = { 0 };
        _DpuSetDepthState(ddge, &depth);
#endif

#if 0 // already set by pipeline
        avxPipelinePrimitiveState ia;
        ia.topology = avxTopology_TRI_STRIP;
        ia.primRestartEnable = FALSE;
        _DpuSetInputAssemblyState(ddge, &ia);
#endif
#if 0
        afxNat const baseVtxs[] = { 0, 0 };
        afxNat const vtxArrs[] = { 0, 1 };
        afxVertexBuffer vbufs[] = { idd->rgbRectVbo, idd->rgbRectVbo };
        _DpuBindVertexBuffers(ddge, 0, 2, vbufs, baseVtxs, vtxArrs);
#endif

#if 0
        _sglCmdVertexAttributes const vtxAttrs =
        {
            .cnt = 1,
            .location[0] = 0,
            .fmt[0] = afxVertexFormat_V2D,
            .srcIdx[0] = 7,
            .offset[0] = 0
        };
        _DpuResetVertexAttributes(dpu, &vtxAttrs);
        _sglCmdVertexStreams const vtxStreams =
        {
            .cnt = 1,
            .srcIdx[0] = 7,
            .stride[0] = sizeof(afxV2d),
            .instance[0] = FALSE
        };
        _DpuResetVertexStreams(dpu, &vtxStreams);
        _sglCmdVertexSources bindVbuf = { .first = 7,.cnt = 1,.offset[0] = 0,.range[0] = sizeof(afxV2d) * 4 };
        bindVbuf.buf[0] = dctx->tristrippedQuad2dPosBuf;
        _DpuBindVertexSources(dpu, &bindVbuf);
#endif
        //_DpuBindLegos(ddge, 0, 1, &idd->presentLego);
        _sglCmdBindRasters cmdBindTex = { 0 };
        cmdBindTex.first = 0;
        cmdBindTex.cnt = 1;
        cmdBindTex.smp[0] = dctx->presentSmp;
        cmdBindTex.tex[0] = surf;
        _DpuBindRasters(dpu, &cmdBindTex);

        _sglCmdDraw cmdDraw = { 0 };
        cmdDraw.vtxCnt = 4;
        cmdDraw.instCnt = 1;
        _DpuDraw(dpu, &cmdDraw);

#if !0
        _DpuFinishSynthesis(dpu, NIL);
#else
        _DpuEndCombination(dpu, NIL);
#endif
#endif

        HDC dc = dout->idd->hDc;

        if (dc)
        {
            //afxNat cnt = dout->refreshRate;
            //while (--cnt) DwmFlush();
            SglSwapBuffers(dc, &dpu->wgl); // deadlocks all
            //SwapBuffers(dc);
            //Sleep(1);
            //AfxYield();
        }
        //gl->Flush();

        //gl->ClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE); _SglThrowErrorOccuried(); // reset GL default

        afxClock currClock;
        AfxGetClock(&currClock);

        if (1.0 > AfxGetSecondsElapsed(&dout->outCntResetClock, &currClock)) ++dout->outNo;
        else
        {
            dout->outCntResetClock = currClock;
            dout->outRate = dout->outNo; // 681 no showing (presenting from overlay thread (acquirer)), 818 frozen (present from draw thread (worker))
            dout->outNo = 0;

            afxReal64 ct = AfxGetSecondsElapsed(&dout->startClock, &currClock);
            afxReal64 dt = AfxGetSecondsElapsed(&dout->lastClock, &currClock);
            dout->lastClock = currClock;

            if (AfxTestObjectFcc(dout->endpointNotifyObj, afxFcc_WND))
            {
                //AfxFormatWindowCaption(dout->endpointNotifyObj, "%0f, %u --- OpenGL/Vulkan Continuous Integration --- SIGMA GL/2 --- Qwadro Execution Ecosystem (c) 2017 SIGMA Technology --- Public Test Build", dt, dout->outRate);
            }

            if (dout->endpointNotifyFn)
            {
                dout->endpointNotifyFn(dout->endpointNotifyObj, outBufIdx);
            }
        }
        
        dout->presentingBufIdx = (afxAtom32)AFX_INVALID_INDEX;
        AfxPushInterlockedQueue(&dout->freeBuffers, (afxNat[]){ outBufIdx });
        AfxDecAtom32(&dout->submCnt);
    }
    return err;
}

_SGL afxError _SglActivateDout(sglDpu* dpu, afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    afxBool fallback = TRUE;
    HDC hDc = dout->idd->hDc;
    HDC currDc = dpu->wgl.GetCurrentDC();

    if (hDc)
    {
        if (!dout->idd->dcPxlFmt)
        {
            afxPixelLayout pfl;
            afxPixelFormat pixelFmt = dout->pixelFmt;
            AfxDescribePixelFormat(pixelFmt, &pfl);
            afxNat colorBpp = pfl.bpp;
            afxNat alphaBpp = pfl.bpc[3];

#if 0
            HWND hDskWnd = GetDesktopWindow();
            HDC hDskDc = GetDC(hDskWnd);
            PIXELFORMATDESCRIPTOR pfd2;
            int dskPf = SglGetPixelFormat(hDskDc, &dpu->wgl);
            SglDescribePixelFormat(hDskDc, dskPf, sizeof(pfd2), &pfd2, &dpu->wgl);
#endif

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
                //{ WGL_DEPTH_BITS_ARB, 0 }, // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                //{ WGL_STENCIL_BITS_ARB, 0 },  // No Qwadro, não é possível desenhar arbitrariamente no default framebuffer. Logo, não há necessidade de stencil.
                { WGL_TRANSPARENT_ARB, (dout->presentAlpha && dout->presentAlpha != avxPresentAlpha_OPAQUE) },
                { WGL_SWAP_METHOD_ARB, (dout->presentMode && dout->presentMode == avxPresentMode_IMMEDIATE) ? WGL_SWAP_COPY_ARB : WGL_SWAP_EXCHANGE_ARB },
                //{ WGL_SAMPLE_BUFFERS_ARB,  GL_FALSE },  // works on Intel, didn't work on Mesa
                //{ WGL_SAMPLES_ARB, 0 }, // works on Intel, didn't work on Mesa
                //{ WGL_COLORSPACE_EXT, dout->colorSpc == avxColorSpace_SRGB ? WGL_COLORSPACE_SRGB_EXT : (dout->colorSpc == avxColorSpace_LINEAR ? WGL_COLORSPACE_LINEAR_EXT : NIL) }, // works on Mesa, didn't work on Intel
                //{ WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, (dout->colorSpc == avxColorSpace_SRGB) }, // works on Mesa, didn't work on Intel
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
                dout->idd->dcPxlFmtBkp = SglGetPixelFormat(hDc, &dpu->wgl);
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
                DWORD pfdFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SWAP_EXCHANGE
#if 0
                    | PFD_DIRECT3D_ACCELERATED
#endif
#if 0
                    | PFD_SUPPORT_COMPOSITION
#endif
                    ;
                AfxAssert(pfd.dwFlags & pfdFlags);

                if (!SglSetPixelFormat(hDc, dcPxlFmt, &(pfd), &dpu->wgl)) AfxThrowError();
                else
                {
                    dout->idd->dcPxlFmt = dcPxlFmt;
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

                if (dout->presentMode == avxPresentMode_FIFO)
                    dpu->wgl.SwapIntervalEXT(0); // revert V-sync
            }
        }
        else
        {
#if !0
            if (dout->presentMode == avxPresentMode_FIFO)
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

_SGL afxError _SglConDoutOnDpuCb(sglDpu* dpu, afxDrawBridge ddge, afxNat queIdx, void* udd)
{
    afxError err = AFX_ERR_NONE;
    struct
    {
        afxDrawOutput dout;
        afxError*     rslt;
    }* udd2 = udd;
    *udd2->rslt = _SglActivateDout(dpu, udd2->dout);
    return err;
}

_SGL afxError _SglRelinkDoutCb(afxDrawDevice ddev, afxDrawContext dctx, afxNat cnt, afxDrawOutput douts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxDrawOutput dout = douts[i];
        AfxAssertObjects(1, &dout, afxFcc_DOUT);

        if (dctx)
        {
            avxSubmission req = { 0 };
            req.f = (void*)_SglConDoutOnDpuCb;
            req.udd = (void*[]) { dout, &err };
            afxDrawBridge ddge;
            AfxGetDrawBridge(dctx, 0, &ddge);
            afxNat queIdx = AvxRequestDrawWork(ddge, 1, &req);
            AfxWaitForIdleDrawQueue(dctx, 0, queIdx);
        }
    }
    return err;
}

_SGL afxError _SglDdevOpenDoutCb(afxDrawDevice ddev, afxDrawOutput dout, afxDrawOutputConfig const* cfg, afxUri const* endpoint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (!(dout->idd = AfxAllocate(1, sizeof(*dout->idd), 0, AfxHere())))
    {
        AfxThrowError();
        return err;
    }

    AfxZero(dout->idd, sizeof(*dout->idd));

    dout->lockCb = NIL;
    dout->iddCb = _SglDouVmtGetIddCb;

    HWND wnd = cfg->w32.hWnd;

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
            afxReal refreshRate = GetDeviceCaps(dc, VREFRESH);
            AfxResetDrawOutputResolution(dout, screenRes, refreshRate, physAspRatio);

            //AfxAdjustDrawOutputFromNdc(dout, AfxSpawnV3d(0.6666666, 0.6666666, 1));

            dout->idd->hInst = cfg->w32.hInst;
            dout->idd->hWnd = cfg->w32.hWnd;
            dout->idd->hDc = dc;

            if (cfg->udd[0])
                ((HDC*)cfg->udd[0])[0] = dc;

            if (err)
                ReleaseDC(wnd, dc);
        }
    }
    return err;
}

_SGL afxError _SglDdevCloseDoutCb(afxDrawDevice ddev, afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    _SglDoutSuspendFunction(dout);
    
    ReleaseDC(dout->idd->hWnd, dout->idd->hDc);

    if (dout->idd)
    {
        AfxDeallocate(dout->idd);
        dout->idd = NIL;
    }
    return err;
}
