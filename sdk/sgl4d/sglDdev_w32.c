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

#define _AFX_CORE_C
//#define _AFX_EXECUTABLE_C
//#define _AFX_ICD_C
#define _AFX_DRAW_C
#define _AFX_THREAD_C
#define _AFX_DRAW_THREAD_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AFX_DRAW_DRIVER_C
#include "sgl.h"
#include "sglDdrv.h"
#include "qwadro/core/afxSystem.h"
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
    //AfxYieldThread();

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
        if ((delRes = AfxPullNextQueueUnit(&dpu->deletionQueue)))
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

            AfxPopNextQueue(&dpu->deletionQueue);
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
    
    if (AfxPushQueueUnit(&dpu->deletionQueue, &delRes))
        AfxThrowError();

    AfxExitSlockExclusive(&dpu->deletionLock);
};

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

    afxManifest const* ini = AfxGetDeviceManifest(&ddev->dev);

    HMODULE opengl32 = ddev->idd->opengl32;

    _LoadWglBaseSymbols(opengl32, (wglVmt*)&dpu->wgl);

    HDC bkpHdc = dpu->wgl.GetCurrentDC();
    HGLRC bkpGlrc = dpu->wgl.GetCurrentContext();

    HWND tmpHwnd = NIL;
    HDC tmpHdc = NIL;
    HGLRC tmpHrc = NIL;
    DWORD pfdFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DIRECT3D_ACCELERATED | PFD_SWAP_EXCHANGE | PFD_SUPPORT_COMPOSITION;
    DWORD dwExStyle = WS_EX_APPWINDOW;
    DWORD wndStyles = WS_POPUP;

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
                    SglLoadGlVmt(opengl32, offsetof(glVmt,GetString) / sizeof(void*), 1, (void**)&_glGetString, FALSE);
                    AfxMakeString(&ver, (afxChar const*)_glGetString(GL_VERSION), 0);
                    AfxScanString(&ver, "%u.%u.%u", &verMajor, &verMinor, &verPatch);
                    dpu->verMajor = verMajor;
                    dpu->verMinor = verMinor;
                    dpu->verPatch = verPatch;
                }
            }
        }
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
                _LoadWglExtendedSymbols(opengl32, &tmpWglExt);

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

                    dpu->wgl.DeleteContext(tmpHrc); // to avoid a anomalous error in kernel base it is needed to delete the temp context in order to create the definitive one.
                    tmpHrc = NIL;

                    if (!(dpu->glrc = tmpWglExt.CreateContextAttribsARB(dpu->dc, unitIdx ? ddev->idd->dpus[0].glrc : NIL, (void*)ctxAttrPairs))) AfxThrowError();
                    else
                    {
                        if (!(dpu->wgl.MakeCurrent(dpu->dc, dpu->glrc))) AfxThrowError();
                        else
                        {
                            _LoadWglExtendedSymbols(opengl32, (wglVmt*)&dpu->wgl);
                            //SglLoadOpenGlVmt(opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, (void**)&dpu->wgl.ptr, 0 == unitIdx);
                            SglLoadGlVmt(opengl32, 0, (sizeof(dpu->gl) / sizeof(dpu->gl.ptr)), (void**)&dpu->gl.ptr, 0 == unitIdx);

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

                        if (err)
                            dpu->wgl.MakeCurrent(tmpHdc, tmpHrc);

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

        dpu->activeRasterState.depthCompareOp = afxCompareOp_LESS;
        dpu->activeRasterState.depthWriteDisabled = FALSE;
        dpu->activeRasterState.depthBounds[0] = 0.f;
        dpu->activeRasterState.depthBounds[1] = 1.f;
        dpu->activeRasterState.stencilFront.compareOp = afxCompareOp_ALWAYS;
        dpu->activeRasterState.stencilFront.reference = 0;
        dpu->activeRasterState.stencilFront.compareMask = (dpu->activeRasterState.stencilFront.writeMask = 0xFFFFFFFF);
        dpu->activeRasterState.stencilFront.failOp = (dpu->activeRasterState.stencilFront.passOp = (dpu->activeRasterState.stencilFront.depthFailOp = afxStencilOp_KEEP));
        dpu->activeRasterState.stencilBack.compareOp = afxCompareOp_ALWAYS;
        dpu->activeRasterState.stencilBack.reference = 0;
        dpu->activeRasterState.stencilBack.compareMask = (dpu->activeRasterState.stencilBack.writeMask = 0xFFFFFFFF);
        dpu->activeRasterState.stencilBack.failOp = (dpu->activeRasterState.stencilBack.passOp = (dpu->activeRasterState.stencilBack.depthFailOp = afxStencilOp_KEEP));
        dpu->activeRasterState.depthBiasConstFactor = 0.f;
        dpu->activeRasterState.depthBiasSlopeScale = 0.f;

        for (afxNat i = 0; i < 8; i++)
        {
            dpu->activeRasterState.outs[i].blendConfig.aBlendOp = afxBlendOp_ADD;
            dpu->activeRasterState.outs[i].blendConfig.rgbBlendOp = afxBlendOp_ADD;
            dpu->activeRasterState.outs[i].blendConfig.aSrcFactor = afxBlendFactor_ONE;
            dpu->activeRasterState.outs[i].blendConfig.rgbSrcFactor = afxBlendFactor_ONE;
            dpu->activeRasterState.outs[i].blendConfig.aDstFactor = afxBlendFactor_ZERO;
            dpu->activeRasterState.outs[i].blendConfig.rgbDstFactor = afxBlendFactor_ZERO;
            dpu->activeRasterState.outs[i].discardMask = NIL;
        }

        ///

        AfxSetUpSlock(&dpu->deletionLock);
        AfxAllocateQueue(&dpu->deletionQueue, sizeof(_sglDeleteGlRes), 32);

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
        afxDrawDeviceLimits limits = { 0 };
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
        limits.maxFramebufferWidth = datai; // at least 16384
        gl->GetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &datai); _SglThrowErrorOccuried();
        limits.maxFramebufferHeight = datai; // at least 16384
        gl->GetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &datai); _SglThrowErrorOccuried();
        limits.maxFramebufferLayers = datai; // at least 2048
        gl->GetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &datai); _SglThrowErrorOccuried();
        limits.framebufferNoAttachmentsSampleCounts = datai; // at least 4

        gl->GetIntegerv(GL_MAX_TEXTURE_SIZE, &datai); _SglThrowErrorOccuried();
        limits.maxImageDimension1D = datai; // at least 1024
        limits.maxImageDimension2D = datai; // at least 1024
        gl->GetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &datai); _SglThrowErrorOccuried();
        limits.maxImageDimension3D = datai; // at least 64
        gl->GetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &datai); _SglThrowErrorOccuried();
        limits.maxImageArrayLayers = datai; // at least 256
        gl->GetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &datai); _SglThrowErrorOccuried();
        limits.maxImageDimensionCube = datai; // at least 1024
        gl->GetFloatv(GL_MAX_TEXTURE_LOD_BIAS, &dataf); _SglThrowErrorOccuried();
        limits.maxSamplerLodBias = dataf; // at least 2.0

        gl->GetIntegerv(GL_MAX_INTEGER_SAMPLES, &datai); _SglThrowErrorOccuried();
        limits.sampledImageIntegerSampleCounts = datai;
        gl->GetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &datai); _SglThrowErrorOccuried();
        limits.sampledImageColorSampleCounts = datai;
        limits.framebufferColorSampleCounts = datai;
        gl->GetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &datai); _SglThrowErrorOccuried();
        limits.sampledImageDepthSampleCounts = datai;
        limits.sampledImageStencilSampleCounts = datai;
        limits.framebufferDepthSampleCounts = datai;
        limits.framebufferStencilSampleCounts = datai;

        gl->GetIntegerv(GL_MIN_MAP_BUFFER_ALIGNMENT, &datai); _SglThrowErrorOccuried();
        limits.minMemoryMapAlignment = datai; // at least 64
        gl->GetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &datai); _SglThrowErrorOccuried();
        limits.minStorageBufferOffsetAlignment = datai; // at least 64
        gl->GetIntegerv(GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT, &datai); _SglThrowErrorOccuried();
        limits.minTexelBufferOffsetAlignment = datai; // at least 64
        gl->GetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &datai); _SglThrowErrorOccuried();
        limits.minUniformBufferOffsetAlignment = datai; // at least 64

        gl->GetFloatv(GL_POINT_SIZE_RANGE, dataf2); _SglThrowErrorOccuried();
        limits.pointSizeRange[0] = dataf2[0]; // at least 1
        limits.pointSizeRange[1] = dataf2[1]; // at least 1
        gl->GetFloatv(GL_POINT_SIZE_GRANULARITY, &dataf); _SglThrowErrorOccuried();
        limits.pointSizeGranularity = dataf;
        gl->GetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, dataf2); _SglThrowErrorOccuried();
        limits.lineWidthRange[0] = dataf2[0];
        limits.lineWidthRange[1] = dataf2[1];
        gl->GetFloatv(GL_SMOOTH_LINE_WIDTH_GRANULARITY, &dataf); _SglThrowErrorOccuried();
        limits.lineWidthGranularity = dataf;

        gl->GetIntegerv(GL_MAX_VERTEX_ATTRIBS, &datai); _SglThrowErrorOccuried();
        limits.maxVertexInputAttributes = datai; // at least 16
        gl->GetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
        limits.maxVertexOutputComponents = datai; // at least 64
        gl->GetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &datai); _SglThrowErrorOccuried();
        limits.maxVertexInputBindings = datai;
        gl->GetIntegerv(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, &datai); _SglThrowErrorOccuried();
        limits.maxVertexInputAttributeOffset = datai;
        gl->GetIntegerv(GL_VERTEX_BINDING_STRIDE, &datai); _SglThrowErrorOccuried();
        limits.maxVertexInputBindingStride = datai;

        gl->GetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
        limits.maxFragmentInputComponents = datai; // at least 128
        gl->GetIntegerv(GL_MAX_DRAW_BUFFERS, &datai); _SglThrowErrorOccuried();
        limits.maxFragmentOutputAttachments = datai; // at least 8
        gl->GetIntegerv(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, &datai); _SglThrowErrorOccuried();
        limits.maxFragmentDualSrcAttachments = datai; // at least 1

        gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &datai); _SglThrowErrorOccuried();
        limits.maxComputeWorkGroupInvocations = datai;
        gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, datai2); _SglThrowErrorOccuried();
        limits.maxComputeWorkGroupCount[0] = datai2[0];
        limits.maxComputeWorkGroupCount[1] = datai2[1];
        limits.maxComputeWorkGroupCount[2] = datai2[2];
        gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE, datai2); _SglThrowErrorOccuried();
        limits.maxComputeWorkGroupSize[0] = datai2[0];
        limits.maxComputeWorkGroupSize[1] = datai2[1];
        limits.maxComputeWorkGroupSize[2] = datai2[2];


        gl->GetIntegerv(GL_MAX_GEOMETRY_INPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
        limits.maxGeometryInputComponents = datai; // at least 64
        gl->GetIntegerv(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
        limits.maxGeometryOutputComponents = datai; // at least 128

        limits.maxBoundDescriptorSets = 4;
        limits.maxPushConstantsSize = 0;

        ddev->limits = limits;

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
    AfxDeallocateQueue(&dpu->deletionQueue);

    //AfxAbolishManager(&ddev->ports[unitIdx].scripts);
    //AfxAbolishManager(&ddev->ports[unitIdx].queues);

    gl->DeleteVertexArrays(1, &dpu->emptyVao); _SglThrowErrorOccuried();

    //wglVmt const* wgl = &dpu->wgl;

    HDC dc = dpu->wgl.GetCurrentDC();
    HGLRC rc = dpu->wgl.GetCurrentContext();

    if (dc == dpu->dc || rc == dpu->glrc)
        dpu->wgl.MakeCurrent(NIL, NIL);

    dpu->wgl.DeleteContext(dpu->glrc);

    ReleaseDC(dpu->wnd, dpu->dc);
    DestroyWindow(dpu->wnd);
    return err;
}

_SGL afxBool _SglDinProcCb(afxDrawInput din, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxDrawEvent* ev = udd;

    if (din->procCb && (ev->accepted |= !!din->procCb(din, ev)))
        AfxThrowError();

    return FALSE; // don't interrupt curation;
}

_SGL afxBool _SglDoutProcCb(afxDrawOutput dout, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    afxThread thr = (afxThread)udd;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    return FALSE; // don't interrupt curation;
}

_SGL afxBool _SglDctxProcCb(afxDrawContext dctx, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxThread thr = (afxThread)udd;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    afxDrawDevice ddev = AfxGetObjectProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxDrawEvent ev = { 0 };
    ev.id = afxDrawEventId_PREFETCH;

    //AfxInvokeConnectedDrawInputs(dctx, 0, AFX_N32_MAX, _SglDinProcCb, &ev);

    _SglDdevProcDpuCb(thr, dctx);
    _SglDdevProcessResDel(ddev, 0); // delete after is safer?

    //AfxInvokeConnectedDrawOutputs(dctx, 0, AFX_N32_MAX, _SglDoutProcCb, (void*)thr);

    return FALSE; // don't interrupt curation;
}

_SGL afxError _SglDdevProcCb(afxDrawDevice ddev, afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertObjects(1, &thr, afxFcc_THR);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    AfxInvokeDrawContexts(ddev, 0, AFX_N32_MAX, _SglDctxProcCb, (void*)thr);

    return err;
}

_SGL afxResult DrawThreadProc(afxThread thr, afxEvent* ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    afxDrawDevice ddev = AfxGetThreadUdd(thr);

    switch (ev->id)
    {
    case afxThreadEvent_RUN:
    {
        if (_SglBuildDpu(ddev, 0))
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
        if (_SglDestroyDpu(ddev, 0))
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
        if (ddev->dev.serving)
            _SglDdevProcCb(ddev, thr);

        break;
    }
    }

    return 0;
}

_SGL afxError _SglDdevIddDtor(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    AfxExhaustChainedManagers(&ddev->dev.classes);

#if 0
    for (afxNat i = ddev->idd->dpuCnt; i-- > 0;)
        if (_SglDestroyDpu(ddev, i))
            AfxThrowError();
#endif

    AfxCleanUpChainedManagers(&ddev->dev.classes);

    AfxDeallocate(ddev->idd->dpus);
    ddev->idd->dpus = NIL;

    AfxCleanUpMutex(&ddev->idd->ioConMtx);

    UnregisterClassA(ddev->idd->wndClss.lpszClassName, ddev->idd->wndClss.hInstance);

    FreeLibrary(ddev->idd->opengl32);

    AfxDeallocate(ddev->idd);
    ddev->idd = NIL;

    if (ddev->ports)
        AfxDeallocate(ddev->ports);


    return err;
}

_SGL afxError _SglDdevIddCtor(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);


    afxDrawDeviceCaps const devCaps = { 0 };
    afxDrawPortCaps const portCaps[] =
    {
        {.queFlags = afxDrawBridgeFlag_DRAW | afxDrawBridgeFlag_COMPUTE | afxDrawBridgeFlag_TRANSFER | afxDrawBridgeFlag_VHS, .queCnt = 16 },
        {.queFlags = afxDrawBridgeFlag_COMPUTE | afxDrawBridgeFlag_TRANSFER, .queCnt = 2 },
    };
    static afxString devDomain, devName;
    AfxMakeString(&devDomain, "targa", 0);
    AfxMakeString(&devName, "Vesa", 0);
    

    AfxReflectString(&devDomain, &ddev->dev.domain);
    AfxReflectString(&devName, &ddev->dev.name);

    //AfxPushLinkage(&ddev->dev.icd, (afxChain*)AfxGetRegisteredDevices(icd));

    ddev->caps = devCaps;
    ddev->limits = (afxDrawDeviceLimits) { 0 };

    ddev->dev.serving = FALSE;

    ddev->clipCfg = AFX_CLIP_SPACE_QWADRO;

    AfxSetUpChain(&ddev->outputs, ddev);
    AfxSetUpChain(&ddev->inputs, ddev);

    ddev->procCb = NIL;

    afxNat portCnt = AfxMax(1, AfxMin(AfxGetThreadingCapacity(), 1));
    ddev->portCnt = portCnt;
    AfxAssert(ddev->portCnt);

    if (!(ddev->ports = AfxAllocate(portCnt, sizeof(ddev->ports[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        afxChain *classes = &ddev->dev.classes;
        AfxSetUpChain(classes, ddev);
        afxClassConfig tmpClsCfg;

        for (afxNat i = 0; i < portCnt; i++)
        {
            ddev->ports[i].portCaps = portCaps[i];

            tmpClsCfg = _SglDdgeMgrCfg;
            AfxEstablishManager(&ddev->ports[i].queues, NIL, classes, &tmpClsCfg);
        }

        // dctx must be after ddge
        tmpClsCfg = _SglDctxMgrCfg;
        AfxEstablishManager(&ddev->contexts, NIL, classes, &tmpClsCfg); // require ddge, diob

        ddev->procCb = NIL;

        ddev->idd = NIL;

        afxClassConfig tmpClsConf;
        tmpClsConf = _SglSemMgrCfg;
        AfxEstablishManager(&ddev->semaphores, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglFencMgrCfg;
        AfxEstablishManager(&ddev->fences, NIL, classes, &tmpClsConf);

        tmpClsConf = _SglQrypMgrCfg;
        AfxEstablishManager(&ddev->queries, NIL, classes, &tmpClsConf);


        if (!(ddev->idd = AfxAllocate(1, sizeof(ddev->idd[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            afxManifest const* ini = AfxGetDeviceManifest(&ddev->dev);

            afxUri uri;
            AfxIniGetUri(ini, &AfxString("OpenGL"), &AfxString("ApiFile"), &uri);

            if (!(ddev->idd->opengl32 = LoadLibraryA(AfxGetUriData(&uri, 0)))) AfxLogError("");
            else
            {
                //SglLoadOpenGlVmt(ddev->opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, ddev->p);

                ddev->idd->wndClss.cbSize = sizeof(ddev->idd->wndClss); // only on EX
                ddev->idd->wndClss.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
                ddev->idd->wndClss.lpfnWndProc = _SglWndHndlngPrcW32Callback;
                ddev->idd->wndClss.cbClsExtra = sizeof(ddev);
                ddev->idd->wndClss.cbWndExtra = sizeof((afxDrawOutput)0);
                ddev->idd->wndClss.hInstance = GetModuleHandleA(NULL);
                ddev->idd->wndClss.hIcon = NULL;
                ddev->idd->wndClss.hCursor = NULL;
                ddev->idd->wndClss.hbrBackground = NULL;
                ddev->idd->wndClss.lpszMenuName = NULL;
                ddev->idd->wndClss.lpszClassName = "OpenGL/Vulkan Continuous Integration --- SIGMA GL/2 --- Qwadro Execution Ecosystem (c) 2017 SIGMA Technology Group --- Public Test Build";
                ddev->idd->wndClss.hIconSm = NULL;

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

                    ddev->clipCfg = AFX_CLIP_SPACE_OPENGL;
                    ddev->procCb = _SglDdevProcCb;
                    ddev->dev.ioctl = (void*)_SglDdevIoctl;

                    ddev->idd->dpuCnt = ddev->portCnt;

                    if (!(ddev->idd->dpus = AfxAllocate(ddev->idd->dpuCnt, sizeof(ddev->idd->dpus[0]), 0, AfxHere()))) AfxThrowError();
                    else
                    {
                        AfxZero2(ddev->idd->dpuCnt, sizeof(ddev->idd->dpus[0]), ddev->idd->dpus);
#if 0
                        for (afxNat i = 0; i < ddev->idd->dpuCnt; i++)
                        {
                            if (_SglBuildDpu(ddev, i))
                            {
                                AfxThrowError();

                                for (afxNat j = i; j-- > 0;)
                                    if (_SglDestroyDpu(ddev, j))
                                        AfxThrowError();
                            }
                        }
#endif
                        if (!err)
                        {
                            afxNat unitIdx = 0;

                            //wglVmt const* wgl = &ddev->idd->dpus[unitIdx].wgl;
#if 0
                            if (!ddev->idd->dpus[unitIdx].wgl.MakeCurrent(ddev->idd->dpus[unitIdx].dc, ddev->idd->dpus[unitIdx].glrc))
                                AfxThrowError();
#endif

                            AfxSetUpMutex(&ddev->idd->ioConMtx, AFX_MTX_PLAIN);
                            AfxLockMutex(&ddev->idd->ioConMtx);
                            AfxUnlockMutex(&ddev->idd->ioConMtx);
                            AfxSetUpCondition(&ddev->idd->ioConCnd);

                            ddev->dev.serving = FALSE;

                            //ddev->idd->dpuCnt = 1;
                            afxThreadConfig dtCfg = { 0 };
                            dtCfg.procCb = DrawThreadProc;
                            dtCfg.udd = ddev;

                            if (AfxAcquireThread(AfxHere(), &dtCfg, &ddev->idd->dedThread)) AfxThrowError();
                            else
                            {
                                AfxAssert(ddev->procCb);
                                ddev->dev.serving = TRUE;
                                AfxRunThread(ddev->idd->dedThread);
                            }
                            AfxAssert(ddev->procCb);
                            ddev->dev.serving = FALSE;
                        }

                        if (err)
                        {
                            AfxDeallocate(ddev->idd->dpus);
                            ddev->idd->dpus = NIL;
                        }
                    }

                    if (err)
                    {
                        UnregisterClassA(ddev->idd->wndClss.lpszClassName, ddev->idd->wndClss.hInstance);
                    }
                }

                if (err)
                    FreeLibrary(ddev->idd->opengl32);
            }

            if (err)
                AfxDeallocate(ddev->idd);
        }

        if (err)
        {
            AfxCleanUpChainedManagers(&ddev->dev.classes);
            AfxDeallocate(ddev->ports);
        }
    }
    return err;
}

_SGL afxResult _SglDdevIoctl(afxDrawDevice ddev, afxNat reqCode, va_list va)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxResult rslt = 0;

    switch (reqCode)
    {
    case _ddevReqCode_0:
    {
        rslt = _SglDdevIddDtor(ddev);
        break;
    }
    case _ddevReqCode_1:
    {
        rslt = _SglDdevIddCtor(ddev);
        break;
    }
    case _ddevReqCode_DIN_CTOR:
    {
        afxDrawInput din = va_arg(va, afxDrawInput);
        AfxAssertObjects(1, &din, afxFcc_DIN);
        afxDrawInputConfig const* cfg = va_arg(va, afxDrawInputConfig const*);
        afxUri const* endpoint = va_arg(va, afxUri const*);
        break;
    }
    case _ddevReqCode_DIN_DTOR:
    {
        afxDrawInput din = va_arg(va, afxDrawInput);
        AfxAssertObjects(1, &din, afxFcc_DIN);
        break;
    }
    case _ddevReqCode_DIN_RLNK:
    {
        afxDrawContext dctx = va_arg(va, afxDrawContext);
        AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);
        afxNat cnt = va_arg(va, afxNat);
        AfxAssert(cnt);
        afxDrawInput* pdin = va_arg(va, afxDrawInput*);
        AfxAssertObjects(cnt, pdin, afxFcc_DIN);
        rslt = _SglDdevRelinkDinCb(ddev, dctx, cnt, pdin);
        break;
    }
    case _ddevReqCode_DOUT_CTOR:
    {
        afxDrawOutput dout = va_arg(va, afxDrawOutput);
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        afxDrawOutputConfig const* cfg = va_arg(va, afxDrawOutputConfig const*);
        afxUri const* endpoint = va_arg(va, afxUri const*);
        rslt = _SglDdevInitDout(ddev, dout, cfg, endpoint);
        break;
    }
    case _ddevReqCode_DOUT_DTOR:
    {
        afxDrawOutput dout = va_arg(va, afxDrawOutput);
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        rslt = _SglDdevDeinitDout(ddev, dout);
        break;
    }
    case _ddevReqCode_DOUT_RLNK:
    {
        afxDrawContext dctx = va_arg(va, afxDrawContext);
        AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);
        afxNat cnt = va_arg(va, afxNat);
        AfxAssert(cnt);
        afxDrawOutput* pdout = va_arg(va, afxDrawOutput*);
        AfxAssertObjects(cnt, pdout, afxFcc_DOUT);
        rslt = _SglDdevRelinkDoutCb(ddev, dctx, cnt, pdout);
        break;
    }
    default:
        break;
    }

    return rslt;
}

_SGL afxError AfxIcdHookPoint(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DDEV);
    dev->ioctl = (void*)_SglDdevIoctl;
    return err;
}
