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
#include <dwmapi.h>
#include <shellapi.h>
#pragma comment(lib,"dwmapi.lib")

#define _AFX_UX_C
#define _AFX_OVERLAY_C
#define _AFX_OVERLAY_IMPL
#define _AFX_APPLICATION_C
#define _AFX_APPLICATION_IMPL

#define _AFX_DRAW_C
#define _AFX_DRAW_OUTPUT_IMPL
#define _AFX_DRAW_OUTPUT_C
#include "qwadro/ux/afxApplication.h"

extern afxClassConfig const _AuxWidMgrCfg;

_AUX void Calc_window_values(HWND window, afxInt* out_extra_width, afxInt32* out_extra_height)
{
    RECT r, c;
    GetWindowRect(window, &r);
    GetClientRect(window, &c);
    *out_extra_width = (r.right - r.left) - (c.right - c.left);
    *out_extra_height = (r.bottom - r.top) - (c.bottom - c.top);
}

_AUX LRESULT WINAPI _AuxWndHndlngPrcW32Callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    afxError err = AFX_ERR_NONE;
    afxOverlay ovy = (afxOverlay)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (ovy)
    {
        AfxAssertObjects(1, &ovy, afxFcc_OVY);

        afxNat32 tid;
        AfxGetTid(&tid);
        afxNat32 doutTid = AfxGetObjectTid(ovy);
        AfxAssert(doutTid == tid);

        AfxAssert(ovy->w32.hWnd == hWnd);
#if 0
        HDC dc = ovy->w32.hDc;

        if (dc)
        {
            SwapBuffers(dc); // deadlocks all
        }
#endif

        switch (message)
        {
        case WM_SYSCOMMAND: // Intercept System Commands
        {
            switch (wParam) // Check System Calls
            {
            case SC_SCREENSAVE: // Screensaver Trying To Start?
            case SC_MONITORPOWER: // Monitor Trying To Enter Powersave?
                return 0; // Prevent From Happening
            }
            break;
        }
        case WM_CLOSE: // Did We Receive A Close Message?
        {
            // Sent as a signal that a window or an application should terminate.
            // wParam = This parameter is not used.
            // lParam = This parameter is not used.
            // If an application processes this message, it should return zero.

            //PostQuitMessage(0); // Send A Quit Message
            return 0; // handled. Also prevent process at end of this function
        }
        case WM_ACTIVATE: // Watch For Window Activate Message
        {
            if (!HIWORD(wParam)) // Check Minimization State
            {
                ovy->active = TRUE; // Program Is Active
            }
            else
            {
                ovy->active = FALSE; // Program Is No Longer Active
            }
            return 0; // Return To The Message Loop
        }
        case WM_SHOWWINDOW:
        {
            //UpdateWindow(dout->idd->wnd);
            break;
        }
        case WM_SIZE:
        {
            //if (doutD->resizable)
            {
                /*
                    WM_SIZE
                        Sent to a window after its size has changed.

                    wParam --- The type of resizing requested. This parameter canv be one of the following values.
                        SIZE_MAXHIDE = 4 --- Message is sent to mem pop-up windows when some other window is maximized.
                        SIZE_MAXIMIZED = 2 --- The window has been maximized.
                        SIZE_MAXSHOW = 3 --- Message is sent to mem pop-up windows when some other window has been restored to its former size.
                        SIZE_MINIMIZED = 1 --- The window has been minimized.
                        SIZE_RESTORED = 0 --- The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.

                    lParam
                        The low-order word of lParam specifies the new width of the client area.
                        The high-order word of lParam specifies the new height of the client area.

                */

                afxWhd whdNew = { LOWORD(lParam), HIWORD(lParam), 1 }; // já é client area, filha da puta.

                if (whdNew[0] * whdNew[1] * whdNew[2]) // don't set to zero
                {
                    AfxAdjustOverlay(ovy, whdNew);
                }
            }
            //AfxDrawOutputProcess(dout);
            break;
        }
        case WM_DISPLAYCHANGE: // The WM_DISPLAYCHANGE message is sent to all windows when the display resolution has changed.
        {
            afxDrawOutput dout = ovy->dout;
            HDC dc = dout->w32.hDc;
            afxWhd const resolution = { GetDeviceCaps(dc, HORZRES), GetDeviceCaps(dc, VERTRES), GetDeviceCaps(dc, PLANES) };
            afxReal64 physAspRatio = AfxDeterminePhysicalAspectRatio(GetDeviceCaps(dc, HORZSIZE), GetDeviceCaps(dc, VERTSIZE));
            afxNat refreshRate = GetDeviceCaps(dc, VREFRESH);
            AfxAdaptDrawOutputResolution(dout, resolution, refreshRate, physAspRatio);
            break;
        }
        case WM_STYLECHANGED: // Sent to a window after the SetWindowLong function has changed one or more of the window's styles.
        {
            // Sent to a window after the SetWindowLong function has changed one or more of the window's styles. A window receives this message through its WindowProc function.
            // wParam = Indicates whether the window's styles or extended window styles have changed. This parameter can be one or more of the following values.

            RECT rect;
            GetClientRect(ovy->w32.hWnd, &(rect));
            afxWhd whdNew = { rect.right - rect.left, rect.bottom - rect.top, 1 };

            if (whdNew[0] * whdNew[1] * whdNew[2]) // don't set to zero
            {
                AfxAdjustOverlay(ovy, whdNew);
            }
            break;
        }
        case WM_MOUSEMOVE:
        {
            // TODO build a kind of handler for draw input to deal with it.

            POINTS points = MAKEPOINTS(lParam);

            afxV2d curr = { AfxScalar(points.x), AfxScalar(points.y) };

            AfxSubV2d(ovy->cursorMove, ovy->cursorPos, curr);
            AfxCopyV2d(ovy->cursorPos, curr);

            afxV2d screen = { AfxScalar(ovy->whd[0]), AfxScalar(ovy->whd[1]) };

            AfxNdcV2d(ovy->cursorPosNdc, ovy->cursorPos, screen);
            AfxNdcV2d(ovy->cursorMoveNdc, ovy->cursorMove, screen);

            //data2->breake = TRUE;
            break;
        }
#if 0
        case WM_DROPFILES: // Sent when the user drops a file on the window of an application that has registered itself as a recipient of dropped files.
        {
            // TODO build a kind of handler for draw input to deal with it.

            afxFileDrop fdrop;
            AFX_ZERO(&fdrop);

            POINT ppt;
            HDROP hDrop = (HDROP)wParam;
            DragQueryPoint(hDrop, &ppt);

            fdrop.x = ppt.x;
            fdrop.y = ppt.y;
            afxNat cnt = DragQueryFileA(hDrop, 0xFFFFFFFF, NIL, NIL);

            AfxAllocateArray(&fdrop.files, sizeof(afxChar const*), 0, AfxHere());

            afxChar* name = NIL;
            afxNat len = 0;
            afxNat i;

            afxDrawContext dctx;
            AfxGetDrawOutputContext(dout, &dctx);
            AfxAssertType(dctxD, afxFcc_DCTX);
            afxMmu mmu = AfxGetDrawContextMmu(dctx);
            AfxAssertObjects(1, &mmu, afxFcc_MMU);

            for (i = 0; i < cnt; i++)
            {
                len = DragQueryFileA(hDrop, i, NIL, 0);

                if (!(name = AfxAllocate(mmu, len + 1, 1, 0, AfxHere()))) AfxLogError("");
                else
                {
                    DragQueryFileA(hDrop, i, name, len + 1);
                    afxNat arrIdx;
                    void *arrel = AfxInsertArrayUnit(&fdrop.files, &arrIdx);
                    AfxCopy2(1, sizeof(name), name, arrel);
                }
            }

            //afxEvent ev;
            //AfxEventDeploy(&ev, AFX_EVENT_DOUT_DRAGNDROP, &doutD->obj, &fdrop);
            //AfxNotifyObject(&doutD->obj, &ev);

            for (i = 0; i < AfxCountArrayElements(&fdrop.files); i++)
            {
                AfxLogEcho("%s", *(afxChar const**)AfxGetArrayUnit(&fdrop.files, i));
            }

            for (i = 0; i < AfxCountArrayElements(&fdrop.files); i++)
            {
                AfxDeallocate(mmu, *(afxChar**)AfxGetArrayUnit(&fdrop.files, i));
            }

            AfxDeallocateArray(&fdrop.files);

            DragFinish(hDrop);
            //data2->breake = TRUE;
            //return 0;
            break;
        }
#endif
        case WM_KILLFOCUS: // Sent to a window immediately before it loses the keyboard focus.
        {
            ovy->focused = FALSE;
            break;
        }
        case WM_SETFOCUS: // Sent to a window after it has gained the keyboard focus.
        {
            ovy->focused = TRUE;
            break;
        }
        case WM_PAINT:
        {
            ValidateRect(hWnd, NULL);
            //SetWindowTextA(ovy->wnd, AfxGetStringData(&ovy->caption.str, 0));
            return 0; // An application returns zero if it processes this message.
        }
        case WM_ERASEBKGND:
        {
            // Flicker is usually caused by interference via WM_ERASEBKGND. 
            // If you haven't already, intercept WM_ERASEBKGND and do nothing in the regions where you are displaying OpenGL content.            
            ValidateRect(hWnd, NULL);
            return 1; // An application should return nonzero if it erases the background; otherwise, it should return zero.
        }
        default: break;
        }
    }
    return DefWindowProcA(hWnd, message, wParam, lParam);
}

_AUX BOOL CALLBACK FindShellWorkerWindowW32(HWND hwnd, LPARAM lParam)
{
    HWND* found = (HWND*)lParam;

    if (FindWindowExA(hwnd, NULL, "SHELLDLL_DefView", NULL))
        *found = FindWindowExA(NULL, hwnd, "WorkerW", NULL);

    return TRUE;
}

_AUX HWND FindShellBackgroundWindowW32(void)
{
    HWND hwnd = 0;
#ifdef AFX_PLATFORM_WIN10
    // Windows 10 Method

    SendMessageTimeoutA(FindWindowA("ProgMan", NULL), 0x052C, 0, 0, SMTO_NORMAL, 1000, NULL);
    hwnd = 0;
    EnumWindows(FindShellWorkerWindowW32, (LPARAM)&(hwnd));
#else
    // Windows 7 Method
    HWND p = FindWindowA("ProgMan", NULL);
    HWND s = FindWindowExA(p, NULL, "SHELLDLL_DefView", NULL);
    hwnd = FindWindowExA(s, NULL, "SysListView32", NULL);
#endif
    return hwnd;
}

_AUX afxBool DoutNotifyOvy(afxOverlay ovy, afxNat bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_OVY);
    ovy->w32.lastBufIdx = bufIdx;
    ovy->w32.swap = 1;
}

_AUX afxError AfxDoOverlay(afxOverlay ovy)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_OVY);
    
    HDC dc = ovy->w32.hDc;

    if (dc)
    {
        if (ovy->w32.swap)
        {
            ovy->w32.swap = FALSE;
            //SwapBuffers(dc);
            DwmFlush();
        }
    }
    return err;
}

_AUX afxManager* AfxGetWidgetClass(afxOverlay ovy)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_OVY);
    afxManager *cls = &ovy->widMgr;
    AfxAssertClass(cls, afxFcc_WID);
    return cls;
}

_AUX afxNat AfxFormatOverlayCaption(afxOverlay ovy, afxChar const* fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_OVY);
    AfxAssert(fmt);
    va_list va;
    va_start(va, fmt);
    afxNat len = AfxFormatStringArg(&ovy->caption, fmt, va);
    SetWindowTextA(ovy->w32.hWnd, AfxGetStringData(&ovy->caption.str, 0));
    va_end(va);
    return len;
}

_AUX afxError AfxAdjustOverlay(afxOverlay ovy, afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_OVY);
    AfxAssert(whd);
    //AfxAssertExtent(ovy->resolution[0], whd[0]);
    //AfxAssertExtent(ovy->resolution[1], whd[1]);
    //AfxAssertExtent(ovy->resolution[2], whd[2]);

    afxBool changed = (ovy->whd[0] != whd[0] || ovy->whd[1] != whd[1] || ovy->whd[2] != whd[2]);

    if (changed)
    {
        afxInt32 extraWndWidth, extraWndHeight;
        Calc_window_values(ovy->w32.hWnd, &extraWndWidth, &extraWndHeight);
        SetWindowPos(ovy->w32.hWnd, NULL, 0, 0, whd[0] + extraWndWidth, whd[1] + extraWndHeight, SWP_NOMOVE);

        ovy->whd[0] = whd[0];
        ovy->whd[1] = whd[1];
        ovy->whd[2] = whd[2];
        
        if (AfxAdjustDrawOutput(ovy->dout, whd))
            AfxThrowError();
    }
    return err;
}

_AUX afxError AfxAdjustOverlayNdc(afxOverlay ovy, afxV3d const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_OVY);
    AfxAssert(whd);

    afxWhd res;
    afxNat refRate;
    AfxGetDrawOutputResolution(ovy->dout, res, &refRate);
    afxWhd const whd2 =
    {
        (afxNat)AfxUnndcf(whd[0], res[0]),
        (afxNat)AfxUnndcf(whd[1], res[1]),
        1
    };
    return AfxAdjustOverlay(ovy, whd2);
}

_AUX afxError _AuxOvyCtor(afxOverlay ovy, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_OVY);

    afxDrawContext dctx = cookie->udd[1];
    afxNat const* whd = cookie->udd[2];

    ovy->active = FALSE;
    ovy->focused = FALSE;
    AfxZeroV2d(ovy->cursorPos);
    AfxZeroV2d(ovy->cursorMove);
    AfxZeroV2d(ovy->cursorPosNdc);
    AfxZeroV2d(ovy->cursorMoveNdc);

    AfxZeroV2d(ovy->grabPoint);
    AfxZeroV2d(ovy->hoverPoint);

    ovy->grabbedWidg = NIL;
    ovy->hoveredWidg = NIL;
    ovy->focusedWidg = NIL;

    AfxSetUpChain(&ovy->mgrChn, (void*)ovy);

    AfxEstablishManager(&ovy->widMgr, NIL, &ovy->mgrChn, &_AuxWidMgrCfg);

    ovy->w32.wndClss.cbSize = sizeof(ovy->w32.wndClss); // only on EX
    ovy->w32.wndClss.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    ovy->w32.wndClss.lpfnWndProc = _AuxWndHndlngPrcW32Callback;
    ovy->w32.wndClss.cbClsExtra = sizeof(afxObject);
    ovy->w32.wndClss.cbWndExtra = sizeof(afxObject);
    ovy->w32.wndClss.hInstance = GetModuleHandleA(NULL);
    ovy->w32.wndClss.hIcon = LoadIconA(NULL, IDI_WINLOGO);
    ovy->w32.wndClss.hCursor = LoadCursorA(NULL, IDC_ARROW);
    ovy->w32.wndClss.hbrBackground = NULL;
    ovy->w32.wndClss.lpszMenuName = NULL;
    ovy->w32.wndClss.lpszClassName = "The Unified UX Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build";
    ovy->w32.wndClss.hIconSm = NULL;

    if (!(RegisterClassExA(&(ovy->w32.wndClss)))) AfxThrowError();
    else
    {
        AfxAllocateString(&ovy->caption, 512, ovy->w32.wndClss.lpszClassName, 0);

        afxBool fullscreen = FALSE;
        DWORD dwExStyle;              // Window Extended Style
        DWORD dwStyle;                // Window Style

        if (fullscreen)                                             // Are We Still In Fullscreen Mode?
        {
            dwExStyle = WS_EX_APPWINDOW;                              // Window Extended Style
            dwStyle = WS_POPUP;                                       // Windows Style
            //ShowCursor(FALSE);                                      // Hide Mouse Pointer
        }
        else
        {
            dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
            dwStyle = WS_OVERLAPPEDWINDOW;                            // Windows Style
        }

        HWND hWnd = CreateWindowExA(dwExStyle, ovy->w32.wndClss.lpszClassName, ovy->w32.wndClss.lpszClassName, dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NIL, NIL, ovy->w32.wndClss.hInstance, NIL);

        if (!hWnd) AfxThrowError();
        else
        {
            ovy->w32.hWnd = hWnd;
            SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)ovy);

            DragAcceptFiles(hWnd, TRUE);
            
            afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);
            AfxAssertObjects(1, &ddev, afxFcc_DDEV);
            afxNat ddevId = AfxGetObjectId(ddev);
            
            afxDrawOutput dout;
            afxDrawOutputConfig doutCfg = { 0 };
            doutCfg.pixelFmtDs[0] = afxPixelFormat_D24;
            doutCfg.bufCnt = 3;
            doutCfg.endpointObj = ovy;
            doutCfg.endpointNotifier = DoutNotifyOvy;
            doutCfg.w32.hinstance = ovy->w32.wndClss.hInstance;
            doutCfg.w32.hwnd = hWnd;
            doutCfg.udd = &ovy->w32.hDc;

            if (AfxAcquireDrawOutput(ddevId, &doutCfg, &dout)) AfxThrowError();
            else
            {
                ovy->dout = dout;
                ovy->dctx = dctx;
                AfxReconnectDrawOutput(dout, dctx);

                if (whd)
                    AfxAdjustOverlay(ovy, whd);
#if 0
                if (presentAlpha)
                {
                    DWM_BLURBEHIND bb = { 0 };
                    bb.dwFlags = DWM_BB_ENABLE;
                    bb.fEnable = TRUE;
                    DwmEnableBlurBehindWindow(wnd, &(bb)); // não functiona no chad Windows XP
                }
#endif

                ShowWindow(hWnd, SHOW_OPENWINDOW);
            }

            if (err)
                DestroyWindow(ovy->w32.hWnd);
        }

        if (err)
            UnregisterClassA(ovy->w32.wndClss.lpszClassName, ovy->w32.wndClss.hInstance);
    }
    return err;
}

_AUX afxError _AuxOvyDtor(afxOverlay ovy)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ovy, afxFcc_OVY);

    AfxCleanUpChainedManagers(&ovy->mgrChn);

    DragAcceptFiles(ovy->w32.hWnd, FALSE);
    DestroyWindow(ovy->w32.hWnd);
    
    AfxDeallocateString(&ovy->caption);

    UnregisterClassA(ovy->w32.wndClss.lpszClassName, ovy->w32.wndClss.hInstance);

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireOverlay(afxDrawContext dctx, afxWhd const whd, afxOverlay* overlay)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    
    afxManager* cls = AfxGetOverlayClass();
    AfxAssertClass(cls, afxFcc_OVY);
    afxOverlay ovy;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&ovy, (void const*[]) { NIL, dctx, whd })) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ovy, afxFcc_OVY);
        AfxAssert(overlay);
        *overlay = ovy;
    }
    return err;
}

_AUX afxManager* AfxGetOverlayClass(void)
{
    afxError err = AFX_ERR_NONE;
    static afxManager ovyMgr = { 0 };
    static afxBool ovyMgrMounted = FALSE;
    static afxClassConfig ovyMgrCfg =
    {
        .fcc = afxFcc_OVY,
        .name = "Overlay",
        .desc = "Overlay",
        .unitsPerPage = 1,
        .size = sizeof(AFX_OBJECT(afxOverlay)),
        .ctor = (void*)_AuxOvyCtor,
        .dtor = (void*)_AuxOvyDtor
    };

    if (ovyMgr.fcc != afxFcc_CLS)
    {
        AfxEstablishManager(&ovyMgr, NIL, /*_AfxGetSystemClassChain()*/NIL, &ovyMgrCfg);
        ovyMgrMounted = TRUE;
    }
    AfxAssertClass(&ovyMgr, afxFcc_OVY);
    return &ovyMgr;
}
