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

#define _AFX_CORE_C
//#define _AFX_MODULE_C
//#define _AFX_ICD_C
#define _AVX_DRAW_C
#define _AFX_THREAD_C
#define _AVX_DRAW_SYSTEM_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_DRIVER_C
#include "sgl.h"
#include "WglLoader.h"
#include "qwadro/exec/afxSystem.h"
#pragma comment(lib, "opengl32")

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <dwmapi.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <assert.h>

//#define _USE_WGL_FUNCS 1

extern afxClassConfig _SglDdgeMgrCfg;

_SGL BOOL SglMakeCurrent(HDC hdc, HGLRC hrc, wglVmt const* vmt)
{
    afxError err = AFX_ERR_NONE;
    BOOL rslt;
    
    if (!(rslt = vmt->MakeCurrent(hdc, hrc)))
        //if (!(rslt = wglMakeCurrent(hdc, hrc)))
            AfxThrowError();

    return rslt;
}

_SGL afxError SglSwapBuffers(HDC hdc, wglVmt const* vmt)
{
    afxError err = AFX_ERR_NONE;

#ifndef _USE_WGL_FUNCS
    if (!(vmt->SwapBuffers(hdc)))
#endif
        if (!(SwapBuffers(hdc)))
            AfxThrowError();

    //SetWindowTextA(doutD->wglWnd, doutD->title.buf); // deadlocks mem threads on exit
    //UpdateWindow(doutD->wglWnd);
    //AfxYield();

    return err;
}

_SGL int SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd, wglVmt const* vmt)
{
    afxError err = AFX_ERR_NONE;
    int fmt;

#ifndef _USE_WGL_FUNCS
    if (!(fmt = vmt->ChoosePixelFormat(hdc, ppfd)))
#endif
        if (!(fmt = ChoosePixelFormat(hdc, ppfd)))
            AfxThrowError();

    return fmt;
}

_SGL BOOL SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd, wglVmt const* vmt)
{
    afxError err = AFX_ERR_NONE;
    BOOL rslt;

#ifndef _USE_WGL_FUNCS
    if (!(rslt = vmt->SetPixelFormat(hdc, format, ppfd)))
#endif
        if (!(rslt = SetPixelFormat(hdc, format, ppfd)))
            AfxThrowError();

    return rslt;
}

_SGL int SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd, wglVmt const* vmt)
{
    afxError err = AFX_ERR_NONE;
    int rslt;

#ifndef _USE_WGL_FUNCS
    if (!(rslt = vmt->DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd)))
#endif
        if (!(rslt = DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd)))
            AfxThrowError();

    return rslt;
}

_SGL int SglGetPixelFormat(HDC hdc, wglVmt const* vmt)
{
    afxError err = AFX_ERR_NONE;
    int rslt;

#ifndef _USE_WGL_FUNCS
    if (!(rslt = vmt->GetPixelFormat(hdc)))
#endif
        if (!(rslt = GetPixelFormat(hdc)))
            AfxThrowError();

    return rslt;
}

_SGL LRESULT WINAPI _SglWndHndlngPrcW32Callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    afxError err = AFX_ERR_NONE;
    //afxObject obj = (afxObject)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (message == WM_SIZE)
    {
        return 0; // block our fake window of being resized.
    }
    return DefWindowProcA(hWnd, message, wParam, lParam);
}

_SGL afxError _SglDdevProcessResDel(afxDrawDevice ddev, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);


    sglDpu *dpu = &ddev->idd->dpus[unitIdx];
    //wglVmt const* wgl = &dpu->wgl;
    glVmt const *gl = &dpu->gl;

    //HDC dc = wgl->wgl.GetCurrentDC();
    HGLRC rc = dpu->wgl.GetCurrentContext();
    AfxAssert(rc == dpu->glrc);

    if (/*dc != idd->wglDc || */rc != dpu->glrc)
    {
        if (!dpu->wgl.MakeCurrent(dpu->dc, dpu->glrc))
        {
            AfxThrowError();
            AfxLogError("DPU %u failed to be set.", unitIdx);
            dpu->wgl.MakeCurrent(dpu->dc, dpu->glrc);
        }
    }

    _sglDeleteGlRes *delRes;

    if (AfxTryEnterSlockExclusive(&dpu->deletionLock))
    {
        if ((delRes = AfxPeekQueue(&dpu->deletionQueue)))
        {
            switch (delRes->type)
            {
            case 0: // buf
                AfxAssert(gl->IsBuffer(delRes->gpuHandle));
                gl->DeleteBuffers(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsBuffer(delRes->gpuHandle));
                break;
            case 1: // tex
                AfxAssert(gl->IsTexture(delRes->gpuHandle));
                gl->DeleteTextures(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsTexture(delRes->gpuHandle));
                break;
            case 2: // surf
                AfxAssert(gl->IsRenderBuffer(delRes->gpuHandle));
                gl->DeleteRenderbuffers(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsRenderBuffer(delRes->gpuHandle));
                break;
            case 3: // canv
                AfxAssert(gl->IsFramebuffer(delRes->gpuHandle));
                gl->DeleteFramebuffers(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsFramebuffer(delRes->gpuHandle));
                break;
            case 4: // smp
                AfxAssert(gl->IsSampler(delRes->gpuHandle));
                gl->DeleteSamplers(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsSampler(delRes->gpuHandle));
                break;
            case 5: // pip
                AfxAssert(gl->IsProgram(delRes->gpuHandle));
                gl->DeleteProgram(delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsProgram(delRes->gpuHandle));
                break;
            case 6: // shd
                AfxAssert(gl->IsShader(delRes->gpuHandle));
                gl->DeleteShader(delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsShader(delRes->gpuHandle));
                break;
            case 7: // vao
                AfxAssert(gl->IsVertexArray(delRes->gpuHandle));
                gl->GenVertexArrays(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsVertexArray(delRes->gpuHandle));
                break;
            case 8: // pip
            {
                AfxAssert(gl->IsProgramPipeline(delRes->gpuHandle));
                gl->DeleteProgramPipelines(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsProgramPipeline(delRes->gpuHandle));
                break;
            }
            case 9: // fence
            {
                break;
            }
            case 10: // query pool
            {
                AfxAssert(gl->IsQuery(delRes->gpuHandle));
                gl->DeleteQueries(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsQuery(delRes->gpuHandle));
                break;
            }
            default:
                AfxLogError("");
                break;
            }

            delRes->gpuHandle = 0;
            delRes->type = 0;

            AfxPopQueue(&dpu->deletionQueue);
        }
        AfxExitSlockExclusive(&dpu->deletionLock);
    }
    return err;
}

_SGL void _SglDctxDeleteGlRes(afxDrawContext dctx, afxNat type, void* gpuHandle)
{
    //AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxDrawDevice ddev = AfxGetObjectProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);    
    
    afxNat32 unitIdx = 0;
    sglDpu*dpu = &ddev->idd->dpus[unitIdx];
    AfxEnterSlockExclusive(&dpu->deletionLock);
    _sglDeleteGlRes delRes;
    delRes.gpuHandlePtr = gpuHandle;
    delRes.type = type;
    
    if (AfxPushQueue(&dpu->deletionQueue, &delRes))
        AfxThrowError();

    AfxExitSlockExclusive(&dpu->deletionLock);
}

_SGL void APIENTRY _glDbgMsgCb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    afxChar*sourceStr, *typeStr, *severityStr;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API: sourceStr = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "WINDOW SYSTEM"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "SHADER COMPILER"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "THIRD PARTY"; break;
    case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "APPLICATION"; break;
    case GL_DEBUG_SOURCE_OTHER: sourceStr = "OTHER"; break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: typeStr = "ERROR"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "DEPRECATED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "UNDEFINED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_PORTABILITY: typeStr = "PORTABILITY"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "PERFORMANCE"; break;
    case GL_DEBUG_TYPE_MARKER: typeStr = "MARKER"; break;
    case GL_DEBUG_TYPE_OTHER: typeStr = "OTHER"; break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "NOTIFICATION"; break;
    case GL_DEBUG_SEVERITY_LOW: severityStr = "LOW"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: severityStr = "MEDIUM"; break;
    case GL_DEBUG_SEVERITY_HIGH: severityStr = "HIGH"; break;
    }
    AfxLogError("%s %s %s %u %s", sourceStr, typeStr, severityStr, id, message);
}

_SGL afxError _SglBuildDpu(afxDrawDevice ddev, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    //afxNat procUnitIdx = AfxGetThreadingUnit();
    sglDpu *dpu = &ddev->idd->dpus[unitIdx];
    //wglVmt const* wgl = &dpu->wgl;
    glVmt const* gl = &dpu->gl;

    while (unitIdx != 0 && !ddev->idd->dpus[0].instanced)
        AfxYield();

    afxNat devPagIdx;
    afxManifest const* ini = AfxGetDeviceManifest(&ddev->dev, &devPagIdx);

    HMODULE opengl32 = AfxGetModuleIdd(ddev->idd->opengl32);

    _LoadWglBaseSymbols(opengl32, (wglVmt*)&dpu->wgl);

    HDC bkpHdc = dpu->wgl.GetCurrentDC();
    HGLRC bkpGlrc = dpu->wgl.GetCurrentContext();

    HWND tmpHwnd = NIL;
    HDC tmpHdc = NIL;
    HGLRC tmpHrc = NIL;
    DWORD pfdFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SWAP_EXCHANGE
#if 0
        | PFD_DIRECT3D_ACCELERATED
#endif
#if 0
        | PFD_SUPPORT_COMPOSITION
#endif
    ;
    DWORD dwExStyle = WS_EX_APPWINDOW;
    DWORD wndStyles = WS_POPUP;

    if (unitIdx == 0)
    {
        if (!(tmpHwnd = CreateWindowExA(dwExStyle, ddev->idd->wndClss.lpszClassName, "", wndStyles | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NIL, NIL, ddev->idd->wndClss.hInstance, ddev))) AfxThrowError();
        else
        {
            if (!(tmpHdc = GetDC(tmpHwnd))) AfxThrowError();
            else
            {
                PIXELFORMATDESCRIPTOR pfd = { 0 };
                pfd.nSize = sizeof(pfd);
                pfd.nVersion = 1;
                pfd.dwFlags = pfdFlags;
                pfd.iPixelType = PFD_TYPE_RGBA;
                pfd.cColorBits = GetDeviceCaps(tmpHdc, BITSPIXEL);
                pfd.cAlphaBits = 8;
                pfd.cDepthBits = 24;
                pfd.cStencilBits = 8;
                int pxlfmt;

                if (!(pxlfmt = ChoosePixelFormat(tmpHdc, &(pfd))))
                    AfxThrowError();

                if (!(DescribePixelFormat(tmpHdc, pxlfmt, sizeof(pfd), &pfd)))
                    AfxThrowError();

                if (!SetPixelFormat(tmpHdc, pxlfmt, &(pfd)))
                    AfxThrowError();

                AfxLogComment("Creating transient OpenGL context");

                if (!(tmpHrc = dpu->wgl.CreateContext(tmpHdc))) AfxThrowError();
                else
                {
                    if (!(dpu->wgl.MakeCurrent(tmpHdc, tmpHrc))) AfxThrowError();
                    else
                    {
                        afxString ver;
                        afxNat verMajor, verMinor, verPatch;
                        PFNGLGETSTRINGPROC _glGetString;
                        SglLoadGlVmt(opengl32, offsetof(glVmt, GetString) / sizeof(void*), 1, (void**)&_glGetString, FALSE);
                        AfxMakeString(&ver, (afxChar const*)_glGetString(GL_VERSION), 0);
                        AfxScanString(&ver, "%u.%u.%u", &verMajor, &verMinor, &verPatch);
                        dpu->verMajor = verMajor;
                        dpu->verMinor = verMinor;
                        dpu->verPatch = verPatch;
                    }
                }
            }
        }
    }
    else
    {
        dpu->verMajor = ddev->idd->dpus[0].verMajor;
        dpu->verMinor = ddev->idd->dpus[0].verMinor;
        dpu->verPatch = ddev->idd->dpus[0].verPatch;
    }

    if (!err)
    {
        if (!(dpu->wnd = CreateWindowExA(dwExStyle, ddev->idd->wndClss.lpszClassName, ddev->idd->wndClss.lpszClassName, wndStyles | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NIL, NIL, ddev->idd->wndClss.hInstance, dpu))) AfxThrowError();
        else
        {
            if (!(dpu->dc = GetDC(dpu->wnd))) AfxThrowError();
            else
            {
                /*
                WGL_SWAP_METHOD_ARB
                If the pixel format supports a back buffer, then this indicates
                how they are swapped. If this attribute is set to
                WGL_SWAP_EXCHANGE_ARB then swapping exchanges the front and back
                buffer contents; if it is set to WGL_SWAP_COPY_ARB then swapping
                copies the back buffer contents to the front buffer; if it is
                set to WGL_SWAP_UNDEFINED_ARB then the back buffer contents are
                copied to the front buffer but the back buffer contents are
                undefined after the operation. If the pixel format does not
                support a back buffer then this parameter is set to
                WGL_SWAP_UNDEFINED_ARB. The <iLayerPlane> parameter is ignored
                if this attribute is specified.
                */

                int pxlAttrPairs[][2] =
                {
                    { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
                    { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
                    //{ WGL_SUPPORT_GDI_ARB, GL_TRUE },
                    { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
                    { WGL_DOUBLE_BUFFER_ARB, GL_TRUE },
                    { WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB }, // WGL_TYPE_RGBA_FLOAT_ARB
                    { WGL_COLOR_BITS_ARB, GetDeviceCaps(dpu->dc, BITSPIXEL) },
                    { WGL_ALPHA_BITS_ARB, 8 },
                    //{ WGL_DEPTH_BITS_ARB, 24 },
                    //{ WGL_STENCIL_BITS_ARB, 8 },
                    //{ WGL_ACCUM_BITS_ARB, 0 },
                    //{ WGL_TRANSPARENT_ARB, GL_TRUE },
                    //{ WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB },
                    //{ WGL_SAMPLE_BUFFERS_ARB, GL_FALSE },
                    //{ WGL_SAMPLE_BUFFERS_ARB, GL_TRUE },  // works on Intel, didn't work on Mesa
                    //{ WGL_SAMPLES_ARB, 8 },  // works on Intel, didn't work on Mesa
                    //{ WGL_COLORSPACE_EXT, WGL_COLORSPACE_SRGB_EXT }, // WGL_COLORSPACE_LINEAR_EXT // works on Mesa, didn't work on Intel
                    //{ WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE }, // works on Mesa, didn't work on Intel
                    { NIL, NIL },
                };

                UINT formatCount;
                dpu->dcPxlFmt = 0;
                wglVmt tmpWglExt;

                if (unitIdx == 0)
                {
                    _LoadWglExtendedSymbols(opengl32, &tmpWglExt);
                }
                else
                {
                    tmpWglExt = ddev->idd->dpus[0].wgl;
                }

                if (!tmpWglExt.ChoosePixelFormatARB(dpu->dc, &pxlAttrPairs[0][0], 0, 1, &(dpu->dcPxlFmt), &(formatCount))) AfxThrowError();
                else
                {
                    AfxAssert(dpu->dcPxlFmt);
                    AfxAssert(formatCount);
                    AfxZero2(1, sizeof(dpu->dcPfd), &dpu->dcPfd);

                    afxBool fallbackToGdi = FALSE;

                    if (!(dpu->wgl.DescribePixelFormat(dpu->dc, dpu->dcPxlFmt, sizeof(dpu->dcPfd), &dpu->dcPfd)) ||
                        !(dpu->wgl.SetPixelFormat(dpu->dc, dpu->dcPxlFmt, &(dpu->dcPfd))))
                        fallbackToGdi = TRUE;

                    if (fallbackToGdi)
                    {
                        if (!(DescribePixelFormat(dpu->dc, dpu->dcPxlFmt, sizeof(dpu->dcPfd), &dpu->dcPfd)))
                            AfxThrowError();
                        else if (!SetPixelFormat(dpu->dc, dpu->dcPxlFmt, &(dpu->dcPfd)))
                            AfxThrowError();
                    }

                    AfxAssert(dpu->dcPfd.dwFlags & pfdFlags);

                    int ctxAttrPairs[][2] =
                    {
                        { WGL_CONTEXT_MAJOR_VERSION_ARB, dpu->verMajor },
                        { WGL_CONTEXT_MINOR_VERSION_ARB, dpu->verMinor },
                        { WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB }, // WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
                        { WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#ifndef SGL_DONT_USE_ROBUST_ACCESS
                        | WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB
#endif
#ifdef _AFX_DEBUG
                        | WGL_CONTEXT_DEBUG_BIT_ARB
#endif
                        },
                        { NIL, NIL }
                    };


                    AfxLogComment("Creating definitive OpenGL context");

                    if (unitIdx == 0)
                    {
                        dpu->wgl.DeleteContext(tmpHrc); // to avoid a anomalous error in kernel base it is needed to delete the temp context in order to create the definitive one.
                        tmpHrc = NIL;
                    }

                    if (!(dpu->glrc = tmpWglExt.CreateContextAttribsARB(dpu->dc, unitIdx ? ddev->idd->dpus[0].glrc : NIL, (void*)ctxAttrPairs))) AfxThrowError();
                    else
                    {
                        if (!(dpu->wgl.MakeCurrent(dpu->dc, dpu->glrc))) AfxThrowError();
                        else
                        {
                            if (unitIdx == 0)
                            {
                                _LoadWglExtendedSymbols(opengl32, (wglVmt*)&dpu->wgl);
                                //SglLoadOpenGlVmt(opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, (void**)&dpu->wgl.ptr, 0 == unitIdx);
                                SglLoadGlVmt(opengl32, 0, (sizeof(dpu->gl) / sizeof(dpu->gl.ptr)), (void**)&dpu->gl.ptr, 0 == unitIdx);
                            }
                            else
                            {
                                dpu->wgl = ddev->idd->dpus[0].wgl;
                                dpu->gl = ddev->idd->dpus[0].gl;
                            }

#if 0
                            if (0 == unitIdx)
                            {
                                GLint n = 0;
                                gl->GetIntegerv(GL_NUM_EXTENSIONS, &n); _SglThrowErrorOccuried();
                                AfxLogEcho("Listing #%03u \"core\" GL supported extensions.", n);

                                for (GLint i = 0; i < n; i++)
                                {
                                    const char* extension = (const char*)gl->GetStringi(GL_EXTENSIONS, i); _SglThrowErrorOccuried();
                                    AfxLogEcho("%.3u %s ext found.", i, extension);
                                }
                            }
#endif
                        }

                        if (unitIdx == 0)
                        {
                            if (err)
                                dpu->wgl.MakeCurrent(tmpHdc, tmpHrc);
                        }

                        if (err)
                            dpu->wgl.DeleteContext(dpu->glrc);
                    }
                }
            }
        }

        if (err)
        {
            if (dpu->dc)
                ReleaseDC(dpu->wnd, dpu->dc);

            if (dpu->wnd)
                DestroyWindow(dpu->wnd);
        }
    }

    // release temp objects used to get a GL context

    if (unitIdx == 0)
    {
        if (tmpHrc)
        {
            if (err)
                dpu->wgl.MakeCurrent(bkpHdc, bkpGlrc);

            dpu->wgl.DeleteContext(tmpHrc); // disable for detour of some bug caused by OBS
        }

        if (tmpHdc)
            ReleaseDC(tmpHwnd, tmpHdc);

        if (tmpHwnd)
            DestroyWindow(tmpHwnd);
    }

    if (!err)
    {
        AfxAssert(dpu->dc == dpu->wgl.GetCurrentDC());
        AfxAssert(dpu->glrc = dpu->wgl.GetCurrentContext());

        afxNat aa = GetDeviceCaps(dpu->dc, HORZSIZE);
        afxNat ab = GetDeviceCaps(dpu->dc, VERTSIZE);
        afxNat ac = GetDeviceCaps(dpu->dc, HORZRES);
        afxNat ad = GetDeviceCaps(dpu->dc, VERTRES);
        afxNat ae = GetDeviceCaps(dpu->dc, LOGPIXELSX);
        afxNat af = GetDeviceCaps(dpu->dc, LOGPIXELSY);
        afxNat ag = GetDeviceCaps(dpu->dc, BITSPIXEL);
        afxNat ah = GetDeviceCaps(dpu->dc, PLANES);
        afxNat ai = GetDeviceCaps(dpu->dc, ASPECTX);
        afxNat aj = GetDeviceCaps(dpu->dc, ASPECTY);
        afxNat ak = GetDeviceCaps(dpu->dc, ASPECTXY);
        afxNat al = GetDeviceCaps(dpu->dc, VREFRESH);
        afxNat am = GetDeviceCaps(dpu->dc, BLTALIGNMENT);
        AfxLogY("%i %i %i %i %i %i %i %i %i %i %i %i %i ", aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am);

#if 0
        HWND dsk = GetDesktopWindow();
        HDC dskWndDc2 = GetWindowDC(dsk);
        HDC dskWndDc = GetWindowDC(NULL);
        HDC dskDc2 = GetDC(dsk);
        HDC dskDc = GetDC(NULL);
        int dskPf = SglGetPixelFormat(dskDc, wgl);
        int dskWndPf = SglGetPixelFormat(dskWndDc, wgl);
        PIXELFORMATDESCRIPTOR pfd, pfd2;
        SglDescribePixelFormat(dskDc, dskPf, sizeof(pfd), &pfd, wgl);
        SglDescribePixelFormat(dskWndDc, dskWndPf, sizeof(pfd2), &pfd2, wgl);
#endif
        //SglLoadOpenGlVmt(ddev->opengl32, 30, glVmtRanges[1][0] - 30, glVmtNames, &gl->ptr[0], 0); // only load get integer, string, etc.

        //afxString ver;
        //AfxMakeString(&ver, (afxChar const*)gl->GetString(GL_VERSION), 0);
        //AfxScanString(&ver, "%u.%u.%u", &dpu->verMajor, &dpu->verMinor, &dpu->verPatch);
        //gl->GetIntegerv(GL_MAJOR_VERSION, (void*)&(dpu->verMajor)); _SglThrowErrorOccuried();
        //gl->GetIntegerv(GL_MINOR_VERSION, (void*)&(dpu->verMinor)); _SglThrowErrorOccuried();
        AfxMakeString(&dpu->subsysName, (afxChar const*)gl->GetString(GL_RENDERER), 0); _SglThrowErrorOccuried();
        AfxMakeString(&dpu->subsysVer, (afxChar const*)gl->GetString(GL_VERSION), 0); _SglThrowErrorOccuried();

        AfxDbgLogf(6, NIL, "SIGMA GL/2 Hardware-accelerated Draw Processing Unit %.03u\n\t%.*s %.*s", unitIdx, AfxPushString(&dpu->subsysName), AfxPushString(&dpu->subsysVer));

#if 0
        AfxLogAssistence("%s", dpu->wgl.GetExtensionsStringARB(dpu->dc));
#endif

        dpu->numOfFedIndices = 0;
        dpu->numOfFedVertices = 0;
        dpu->numOfFedInstances = 0;

        dpu->submissionSuspended = FALSE;

        dpu->activeTmpFboIdx = 0;
        dpu->activeVaoIdx = 0;

        AfxZero2(1, sizeof(dpu->vao), dpu->vao);
        AfxZero2(1, sizeof(dpu->tmpFbo), dpu->tmpFbo);

        // RESET DEFAULT STATE TO OPENGL DEFAULTS

        //AfxZero2(1, sizeof(dpu->state), &dpu->state);
        AfxZero2(1, sizeof(dpu->activeRasterState), &dpu->activeRasterState);
        AfxZero2(1, sizeof(dpu->nextRasterState), &dpu->nextRasterState);
        AfxZero2(1, sizeof(dpu->activeResBind), &dpu->activeResBind);
        AfxZero2(1, sizeof(dpu->nextResBind), &dpu->nextResBind);
        AfxZero2(1, sizeof(dpu->activeXformState), &dpu->activeXformState);
        AfxZero2(1, sizeof(dpu->nextXformState), &dpu->nextXformState);
        dpu->nextScissorUpdCnt = 0;
        dpu->nextViewportUpdCnt = 0;
        dpu->nextVinAttribUpdCnt = 0;
        dpu->nextVinBindingsCnt = 0;

        dpu->activeRasterState.depthCompareOp = avxCompareOp_LESS;
        dpu->activeRasterState.depthWriteDisabled = FALSE;
        dpu->activeRasterState.depthBounds[0] = 0.f;
        dpu->activeRasterState.depthBounds[1] = 1.f;
        dpu->activeRasterState.stencilFront.compareOp = avxCompareOp_ALWAYS;
        dpu->activeRasterState.stencilFront.reference = 0;
        dpu->activeRasterState.stencilFront.compareMask = (dpu->activeRasterState.stencilFront.writeMask = 0xFFFFFFFF);
        dpu->activeRasterState.stencilFront.failOp = (dpu->activeRasterState.stencilFront.passOp = (dpu->activeRasterState.stencilFront.depthFailOp = avxStencilOp_KEEP));
        dpu->activeRasterState.stencilBack.compareOp = avxCompareOp_ALWAYS;
        dpu->activeRasterState.stencilBack.reference = 0;
        dpu->activeRasterState.stencilBack.compareMask = (dpu->activeRasterState.stencilBack.writeMask = 0xFFFFFFFF);
        dpu->activeRasterState.stencilBack.failOp = (dpu->activeRasterState.stencilBack.passOp = (dpu->activeRasterState.stencilBack.depthFailOp = avxStencilOp_KEEP));
        dpu->activeRasterState.depthBiasConstFactor = 0.f;
        dpu->activeRasterState.depthBiasSlopeScale = 0.f;

        for (afxNat i = 0; i < 8; i++)
        {
            dpu->activeRasterState.outs[i].blendConfig.aBlendOp = avxBlendOp_ADD;
            dpu->activeRasterState.outs[i].blendConfig.rgbBlendOp = avxBlendOp_ADD;
            dpu->activeRasterState.outs[i].blendConfig.aSrcFactor = avxBlendFactor_ONE;
            dpu->activeRasterState.outs[i].blendConfig.rgbSrcFactor = avxBlendFactor_ONE;
            dpu->activeRasterState.outs[i].blendConfig.aDstFactor = avxBlendFactor_ZERO;
            dpu->activeRasterState.outs[i].blendConfig.rgbDstFactor = avxBlendFactor_ZERO;
            dpu->activeRasterState.outs[i].discardMask = NIL;
        }

        ///

        AfxSetUpSlock(&dpu->deletionLock);
        AfxSetUpQueue(&dpu->deletionQueue, sizeof(_sglDeleteGlRes), 32);

        //gl->Enable(GL_FRAMEBUFFER_SRGB);
        gl->Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS); _SglThrowErrorOccuried();
        //gl->ClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE); _SglThrowErrorOccuried();// set screen origin to top to bottm, and set depth to [ 0, 1 ]

#if _AFX_DEBUG
        gl->Enable(GL_DEBUG_OUTPUT);
        gl->Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        gl->DebugMessageCallback(_glDbgMsgCb, NIL);
        //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

        GLfloat dataf;
        GLfloat dataf2[3];
        GLint datai;
        GLint datai2[3];
        static afxDrawDeviceLimits limits = { 0 };
        gl->GetIntegerv(GL_MAX_SAMPLE_MASK_WORDS, &datai); _SglThrowErrorOccuried();
        limits.maxSampleMaskWords = datai;
        gl->GetIntegerv(GL_MAX_CLIP_DISTANCES, &datai); _SglThrowErrorOccuried();
        limits.maxClipDistances = datai; // at least 8

        gl->GetIntegerv(GL_MAX_VIEWPORTS, &datai); _SglThrowErrorOccuried();
        limits.maxViewports = datai; // at least 16
        gl->GetIntegerv(GL_MAX_VIEWPORT_DIMS, datai2); _SglThrowErrorOccuried();
        limits.maxViewportDimensions[0] = datai2[0];
        limits.maxViewportDimensions[1] = datai2[1];
        gl->GetFloatv(GL_VIEWPORT_BOUNDS_RANGE, dataf2); _SglThrowErrorOccuried();
        limits.viewportBoundsRange[0] = dataf2[0];  // at least [-32768, 32767]
        limits.viewportBoundsRange[1] = dataf2[1];  // at least [-32768, 32767]
        gl->GetIntegerv(GL_VIEWPORT_SUBPIXEL_BITS, &datai); _SglThrowErrorOccuried();
        limits.viewportSubPixelBits = datai; // at least 0

        gl->GetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &datai); _SglThrowErrorOccuried();
        limits.maxColorAttachments = datai; // at least 16384
        gl->GetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &datai); _SglThrowErrorOccuried();
        limits.maxCanvasWidth = datai; // at least 16384
        gl->GetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &datai); _SglThrowErrorOccuried();
        limits.maxCanvasHeight = datai; // at least 16384
        gl->GetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &datai); _SglThrowErrorOccuried();
        limits.maxCanvasLayers = datai; // at least 2048
        gl->GetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &datai); _SglThrowErrorOccuried();
        limits.canvasNoAttachmentsSampleCnts = datai; // at least 4

        gl->GetIntegerv(GL_MAX_TEXTURE_SIZE, &datai); _SglThrowErrorOccuried();
        limits.maxRasterDim1D = datai; // at least 1024
        limits.maxRasterDim2D = datai; // at least 1024
        gl->GetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &datai); _SglThrowErrorOccuried();
        limits.maxRasterDim3D = datai; // at least 64
        gl->GetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &datai); _SglThrowErrorOccuried();
        limits.maxRasterArrayLayers = datai; // at least 256
        gl->GetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &datai); _SglThrowErrorOccuried();
        limits.maxRasterDimCube = datai; // at least 1024
        gl->GetFloatv(GL_MAX_TEXTURE_LOD_BIAS, &dataf); _SglThrowErrorOccuried();
        limits.maxSamplerLodBias = dataf; // at least 2.0

        gl->GetIntegerv(GL_MAX_INTEGER_SAMPLES, &datai); _SglThrowErrorOccuried();
        limits.sampledRasterIntegerSampleCnts = datai;
        gl->GetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &datai); _SglThrowErrorOccuried();
        limits.sampledRasterColorSampleCnts = datai;
        limits.canvasColorSampleCnts = datai;
        gl->GetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &datai); _SglThrowErrorOccuried();
        limits.sampledRasterDepthSampleCnts = datai;
        limits.sampledRasterStencilSampleCnts = datai;
        limits.canvasDepthSampleCnts = datai;
        limits.canvasStencilSampleCnts = datai;

        gl->GetIntegerv(GL_MIN_MAP_BUFFER_ALIGNMENT, &datai); _SglThrowErrorOccuried();
        limits.minMemMapAlign = datai; // at least 64
        gl->GetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &datai); _SglThrowErrorOccuried();
        limits.minStorageBufOffsetAlign = datai; // at least 64
        gl->GetIntegerv(GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT, &datai); _SglThrowErrorOccuried();
        limits.minTexelBufOffsetAlign = datai; // at least 64
        gl->GetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &datai); _SglThrowErrorOccuried();
        limits.minUniformBufOffsetAlign = datai; // at least 64

        gl->GetFloatv(GL_POINT_SIZE_RANGE, dataf2); _SglThrowErrorOccuried();
        limits.pointSizRange[0] = dataf2[0]; // at least 1
        limits.pointSizRange[1] = dataf2[1]; // at least 1
        gl->GetFloatv(GL_POINT_SIZE_GRANULARITY, &dataf); _SglThrowErrorOccuried();
        limits.pointSizGranularity = dataf;
        gl->GetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, dataf2); _SglThrowErrorOccuried();
        limits.lineWidthRange[0] = dataf2[0];
        limits.lineWidthRange[1] = dataf2[1];
        gl->GetFloatv(GL_SMOOTH_LINE_WIDTH_GRANULARITY, &dataf); _SglThrowErrorOccuried();
        limits.lineWidthGranularity = dataf;

        gl->GetIntegerv(GL_MAX_VERTEX_ATTRIBS, &datai); _SglThrowErrorOccuried();
        limits.maxVtxInputAttributes = datai; // at least 16
        gl->GetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
        limits.maxVtxOutputCompos = datai; // at least 64
        gl->GetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &datai); _SglThrowErrorOccuried();
        limits.maxVtxInputBindings = datai;
        gl->GetIntegerv(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, &datai); _SglThrowErrorOccuried();
        limits.maxVtxInputAttributeOffset = datai;
        gl->GetIntegerv(GL_VERTEX_BINDING_STRIDE, &datai); _SglThrowErrorOccuried();
        limits.maxVtxInputBindingStride = datai;

        gl->GetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
        limits.maxFragInComps = datai; // at least 128
        gl->GetIntegerv(GL_MAX_DRAW_BUFFERS, &datai); _SglThrowErrorOccuried();
        limits.maxFragOutAttachments = datai; // at least 8
        gl->GetIntegerv(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, &datai); _SglThrowErrorOccuried();
        limits.maxFragDualSrcAttachments = datai; // at least 1

        gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &datai); _SglThrowErrorOccuried();
        limits.maxComputeWorkGroupInvocations = datai;
        gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, datai2); _SglThrowErrorOccuried();
        limits.maxComputeWorkGroupCnt[0] = datai2[0];
        limits.maxComputeWorkGroupCnt[1] = datai2[1];
        limits.maxComputeWorkGroupCnt[2] = datai2[2];
        gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE, datai2); _SglThrowErrorOccuried();
        limits.maxComputeWorkGroupSiz[0] = datai2[0];
        limits.maxComputeWorkGroupSiz[1] = datai2[1];
        limits.maxComputeWorkGroupSiz[2] = datai2[2];


        gl->GetIntegerv(GL_MAX_GEOMETRY_INPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
        limits.maxPrimInComps = datai; // at least 64
        gl->GetIntegerv(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
        limits.maxPrimOutComps = datai; // at least 128

        limits.maxBoundDescriptorSets = 4;
        limits.maxPushConstantsSiz = 0;

        ddev->limits = &limits;

        //dthrD->thr.proc = _SglDthrProcCb;

        gl->GenVertexArrays(1, &dpu->emptyVao); _SglThrowErrorOccuried();
        gl->BindVertexArray(dpu->emptyVao); _SglThrowErrorOccuried();
        gl->BindVertexArray(0); _SglThrowErrorOccuried();

        gl->GenFramebuffers(1, &dpu->fboOpSrc); _SglThrowErrorOccuried();
        gl->BindFramebuffer(GL_FRAMEBUFFER, dpu->fboOpSrc); _SglThrowErrorOccuried();
        gl->GenFramebuffers(1, &dpu->fboOpDst); _SglThrowErrorOccuried();
        gl->BindFramebuffer(GL_FRAMEBUFFER, dpu->fboOpDst); _SglThrowErrorOccuried();
        gl->BindFramebuffer(GL_FRAMEBUFFER, 0); _SglThrowErrorOccuried();

        dpu->fboOpSrcAnnex = NIL;
        dpu->fboOpDstAnnex = NIL;

        dpu->instanced = TRUE;
        dpu->running = TRUE;

        dpu->wgl.MakeCurrent(bkpHdc, bkpGlrc);
    }

    return err;
}

_SGL afxError _SglDestroyDpu(afxDrawDevice ddev, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    sglDpu *dpu = &ddev->idd->dpus[unitIdx];
    glVmt const* gl = &dpu->gl;

    _SglDdevProcessResDel(ddev, unitIdx);
    AfxCleanUpSlock(&dpu->deletionLock);
    AfxCleanUpQueue(&dpu->deletionQueue);

    //AfxDeregisterClass(&ddev->ports[unitIdx].scripts);
    //AfxDeregisterClass(&ddev->ports[unitIdx].queues);

    gl->DeleteVertexArrays(1, &dpu->emptyVao); _SglThrowErrorOccuried();

    //wglVmt const* wgl = &dpu->wgl;

    HDC dc = dpu->wgl.GetCurrentDC();
    HGLRC rc = dpu->wgl.GetCurrentContext();

    if (dc == dpu->dc || rc == dpu->glrc)
        dpu->wgl.MakeCurrent(NIL, NIL);

    dpu->wgl.DeleteContext(dpu->glrc);
    dpu->glrc = NIL;
    ReleaseDC(dpu->wnd, dpu->dc);
    DestroyWindow(dpu->wnd);
    dpu->dc = NIL;
    dpu->wnd = NIL;
    return err;
}

_SGL afxResult DrawThreadProc(afxThread thr, afxEvent* ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    afxDrawDevice ddev = AfxGetThreadUdd(thr)[0];
    afxNat dpuIdx = (afxNat)(AfxGetThreadUdd(thr)[1]);

    switch (ev->id)
    {
    case afxThreadEvent_RUN:
    {
        if (_SglBuildDpu(ddev, dpuIdx))
        {
            AfxThrowError();
            AfxExitThread(err);
        }
        else
        {
            ddev->dev.serving = TRUE;
        }
        break;
    }
    case afxThreadEvent_QUIT:
    {
        if (_SglDestroyDpu(ddev, dpuIdx))
        {
            AfxThrowError();
        }
        else
        {
            ddev->dev.serving = FALSE;
        }
        break;
    }
    default:
    {
        AfxDoDeviceService(&ddev->dev);
        break;
    }
    }

    return 0;
}

_SGL afxError _SglDdevStopCb(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    {
        for (afxNat i = 0; i < ddev->idd->dpuCnt; i++)
        {
            afxThread dedThread = ddev->idd->dpus[i].dedThread;

            afxThread thr;
            AfxGetThread(&thr);

            if (dedThread == thr)
                return err; // do not let a DPU thread do it
        }
    }

    AfxExhaustChainedClasses(&ddev->dev.classes);

    if (ddev->idd->dpus)
    {
        for (afxNat i = 0; i < ddev->idd->dpuCnt; i++)
        {
            afxThread dedThread = ddev->idd->dpus[i].dedThread;

            afxThread thr;
            AfxGetThread(&thr);

            if (dedThread != thr)
            {
                afxResult exitCode;
                do AfxRequestThreadInterruption(dedThread);
                while (!AfxWaitForThread(dedThread, &exitCode));
                AfxReleaseObjects(1, &dedThread);
            }
        }
        AfxDeallocate(ddev->idd->dpus);
        ddev->idd->dpus = NIL;
    }

    UnregisterClassA(ddev->idd->wndClss.lpszClassName, ddev->idd->wndClss.hInstance);
    
    if (ddev->idd->opengl32)
        AfxReleaseObjects(1, &ddev->idd->opengl32);

    AfxDeallocate(ddev->idd);
    ddev->idd = NIL;

    return err;
}

_SGL afxError _SglDdevStartCb(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(!ddev->idd);

    if (!(ddev->idd = AfxAllocate(1, sizeof(ddev->idd[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        afxUri uri;
        afxNat devPagIdx, recIdx;
        afxManifest const* ini = AfxGetDeviceManifest(&ddev->dev, &devPagIdx);

        if (!AfxFindManifestRecord(ini, devPagIdx, &AfxString("OpenglDll"), &recIdx) || !AfxGetManifestUri(ini, devPagIdx, recIdx, &uri)) AfxThrowError();
        else
        {
            if (AfxLoadModule(&uri, NIL, &ddev->idd->opengl32)) AfxThrowError();
            else
            {
                //SglLoadOpenGlVmt(ddev->opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, ddev->p);

                ddev->idd->wndClss.cbSize = sizeof(ddev->idd->wndClss); // only on EX
                ddev->idd->wndClss.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
                ddev->idd->wndClss.lpfnWndProc = _SglWndHndlngPrcW32Callback;
                ddev->idd->wndClss.cbClsExtra = sizeof(ddev);
                ddev->idd->wndClss.cbWndExtra = sizeof((afxDrawOutput)0);
                ddev->idd->wndClss.hInstance = GetModuleHandleA(NULL);
                ddev->idd->wndClss.hIcon = LoadIconA(NULL, IDI_SHIELD);
                ddev->idd->wndClss.hCursor = LoadCursorA(NULL, IDC_ARROW);
                ddev->idd->wndClss.hbrBackground = NULL;
                ddev->idd->wndClss.lpszMenuName = NULL;
                ddev->idd->wndClss.lpszClassName = "OpenGL/Vulkan Continuous Integration --- SIGMA GL/2 --- Qwadro Execution Ecosystem (c) 2017 SIGMA Technology Group --- Public Test Build";
                ddev->idd->wndClss.hIconSm = LoadIconA(NULL, IDI_SHIELD);

                if (!(RegisterClassExA(&(ddev->idd->wndClss)))) AfxThrowError();
                else
                {
                    AfxLogComment("Listing mem available display devices...\n");

                    //HDC hdc = NIL;
                    afxNat idx = 0;
                    while (1)
                    {
                        DISPLAY_DEVICE dispdev = { 0 };
                        dispdev.cb = sizeof(dispdev);

                        if (!(EnumDisplayDevicesA(NULL, idx, &dispdev, NIL))) break;
                        else
                        {
                            AfxLogComment("#%u = %s (%s) %x", idx, dispdev.DeviceString, dispdev.DeviceName, dispdev.StateFlags);
                            idx++;
                        }
                    };

                    if (!AfxFindManifestRecord(ini, devPagIdx, &AfxString("DpuCount"), &recIdx) || !AfxGetManifestNat(ini, devPagIdx, recIdx, &ddev->idd->dpuCnt))
                        ddev->idd->dpuCnt = ddev->portCnt;

                    if (!(ddev->idd->dpus = AfxAllocate(ddev->idd->dpuCnt, sizeof(ddev->idd->dpus[0]), 0, AfxHere()))) AfxThrowError();
                    else
                    {
                        AfxZero2(ddev->idd->dpuCnt, sizeof(ddev->idd->dpus[0]), ddev->idd->dpus);

                        for (afxNat i = 0; i < ddev->idd->dpuCnt; i++)
                        {
                            ddev->idd->dpus[i].portIdx = i;

                            afxThreadConfig dtCfg = { 0 };
                            dtCfg.procCb = DrawThreadProc;
                            dtCfg.udd[0] = ddev;
                            dtCfg.udd[1] = (void*)ddev->idd->dpus[i].portIdx;

                            if (AfxAcquireThread(AfxHere(), &dtCfg, &ddev->idd->dpus[i].dedThread)) AfxThrowError();
                            else
                            {
                                AfxAssert(ddev->dev.procCb);
                                //ddev->dev.serving = TRUE;
                                AfxRunThread(ddev->idd->dpus[i].dedThread);

                                while (!ddev->dev.serving)
                                    AfxYield();
                            }
                            AfxAssert(ddev->dev.procCb);
                        }

                        if (err)
                            AfxDeallocate(ddev->idd->dpus), ddev->idd->dpus = NIL;
                    }

                    if (err)
                        UnregisterClassA(ddev->idd->wndClss.lpszClassName, ddev->idd->wndClss.hInstance);
                }

                if (err)
                    AfxReleaseObjects(1, &ddev->idd->opengl32);
            }
        }

        if (err)
            AfxDeallocate(ddev->idd);
    }
    return err;
}

_SGL afxBool _SglDinProcCb(afxDrawInput din, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    avxEvent* ev = udd;

    //if (din->procCb && (ev->accepted |= !!din->procCb(din, ev)))
      //  AfxThrowError();

    return TRUE; // don't interrupt curation;
}

_SGL afxBool _SglDoutProcCb(afxDrawOutput dout, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxThread thr = (afxThread)udd;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    return TRUE; // don't interrupt curation;
}

_SGL afxBool _SglDctxProcCb(afxDrawContext dctx, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxThread thr = (afxThread)udd;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    afxDrawDevice ddev = AfxGetObjectProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    AfxInvokeConnectedDrawInputs(dctx, 0, AFX_N32_MAX, _SglDinProcCb, thr);

    for (afxNat i = 0; i < dctx->m.ownedBridgeCnt; i++)
    {
        if (ddev->idd->dpus[i].dedThread == thr)
        {
            afxDrawBridge ddge = dctx->m.ownedBridges[i];
            AfxAssertObjects(1, &ddge, afxFcc_DDGE);
            _DdgeProcCb(ddge, thr);
        }
    }

    _SglDdevProcessResDel(ddev, 0); // delete after is safer?

    AfxInvokeConnectedDrawOutputs(dctx, 0, AFX_N32_MAX, _SglDoutProcCb, thr);

    return TRUE; // don't interrupt curation;
}

_SGL afxError _SglDdevExecDpuCb(afxDrawDevice ddev, afxNat dpuIdx, afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertRange(ddev->idd->dpuCnt, dpuIdx, 1);
    sglDpu* dpu = &ddev->idd->dpus[dpuIdx];

    if (dpu->dedThread == thr)
    {
        AfxInvokeDrawContexts(ddev, 0, AFX_N32_MAX, _SglDctxProcCb, (void*)thr);

        afxClass* cls = &ddev->ddgeCls;
        AfxAssertClass(cls, afxFcc_DDGE);
        AfxInvokeClassInstances(cls, 0, AFX_N32_MAX, (void*)_DdgeProcCb, thr);
    }
    return err;
}

_SGL afxError _SglDdevProcCb(afxDrawDevice ddev, afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertObjects(1, &thr, afxFcc_THR);

    if (ddev->ddgeCls.instCnt && !AfxDrawDeviceIsRunning(ddev))
    {
        if (_SglDdevStartCb(ddev)) AfxThrowError(); // start or resume
        else
        {
            AfxAssert(AfxDrawDeviceIsRunning(ddev));
        }
    }

    if (!err)
    {
        if (AfxDrawDeviceIsRunning(ddev))
        {
            for (afxNat i = 0; i < ddev->idd->dpuCnt; i++)
            {
                if (_SglDdevExecDpuCb(ddev, i, thr))
                    AfxThrowError();
            }
        }
    }

    if (ddev->ddgeCls.instCnt == 0 && AfxDrawDeviceIsRunning(ddev))
    {
        if (_SglDdevStopCb(ddev)) // suspend or stop
            AfxThrowError();

        //AfxAssert(!AfxDrawDeviceIsRunning(ddev));
    }
    return err;
}

_SGL afxResult AfxDeviceIoctl(afxDrawDevice ddev, afxNat reqCode, va_list va)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxResult rslt = 0;

    switch (reqCode)
    {
    case afxFcc_DSYS:
    {
        static afxDrawPortCaps const portCaps[] =
        {
            {
                .queFlags = afxDrawBridgeFlag_DRAW | afxDrawBridgeFlag_COMPUTE | afxDrawBridgeFlag_TRANSFER | afxDrawBridgeFlag_VHS,
                .queCnt = 16
            },
            {
                .queFlags = afxDrawBridgeFlag_COMPUTE | afxDrawBridgeFlag_TRANSFER,
                .queCnt = 2
            },
        };

        ddev->dev.procCb = (void*)_SglDdevProcCb;
        ddev->stopCb = _SglDdevStopCb;
        ddev->startCb = _SglDdevStartCb;
        ddev->openCb = _SglDdevOpenCb;
        ddev->closeCb = _SglDdevCloseCb;
        ddev->openDinCb = _SglDdevOpenDinCb;
        ddev->openDoutCb = _SglDdevOpenDoutCb;
        ddev->closeDoutCb = _SglDdevCloseDoutCb;
        ddev->portCnt = 1;
        ddev->portCaps = portCaps;
        ddev->clipSpace = AVX_CLIP_SPACE_OPENGL;

        static afxDrawDeviceCaps caps =
        {
            .robustBufAccess = TRUE,
            .fullDrawIdxUint32 = TRUE,
            .rasterCubeArray = TRUE,
            .independentBlend = TRUE,
            .primShader = TRUE,
            .dualSrcBlend = TRUE,
            .logicOp = TRUE,
            .multiDrawIndirect = TRUE,
            .drawIndirectFirstInst = TRUE,
            .depthClamp = TRUE,
            .depthBiasClamp = TRUE,
            .fillModeNonSolid = TRUE,
            .multiViewport = TRUE,
            .dxt = TRUE,
            .shaderClipDist = TRUE,
            .shaderCullDist = TRUE,
            .alphaToOne = TRUE
        };

        afxClassConfig clsCfg = _AvxDdgeStdImplementation;        
        clsCfg.fixedSiz = sizeof(AFX_OBJECT(afxDrawBridge));
        //clsCfg.maxCnt = ddev->portCnt;
        //clsCfg.unitsPerPage = ddev->portCnt;
        clsCfg.ctor = (void*)_SglDdgeCtor;
        AfxRegisterClass(&ddev->ddgeCls, NIL, &ddev->dev.classes, &clsCfg);

        clsCfg = _AvxDctxStdImplementation;
        clsCfg.fixedSiz = sizeof(AFX_OBJECT(afxDrawContext));
        AfxRegisterClass(&ddev->dctxCls, NIL, &ddev->dev.classes, &clsCfg); // require ddge

        break;
    }
    case afxFcc_DDEV:
    {

        break;
    }
    default: break;
    }

    return rslt;
}
