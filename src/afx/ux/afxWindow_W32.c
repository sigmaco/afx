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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_SHELL_IMPL
#define _AUX_WINDOW_C
#define _AUX_WINDOW_IMPL
#include "dev/AuxDevKit.h"
#include "dev/AuxOverWin32.h"
#pragma comment(lib,"dwmapi.lib")

extern afxKey const Scan1MakeToQwadroDereferenceMap[afxKey_TOTAL];
extern afxClassConfig const _AuxWidMgrCfg;
extern AFX_OBJECT(afxShell) TheShell;

_AUX void CalcWindowValuesW32(HWND window, afxInt* out_extra_width, afxInt32* out_extra_height)
{
    RECT r, c;
    GetWindowRect(window, &r);
    GetClientRect(window, &c);
    *out_extra_width = (r.right - r.left) - (c.right - c.left);
    *out_extra_height = (r.bottom - r.top) - (c.bottom - c.top);
}

_AUX void CalcWindowMarginsW32(HWND window, afxNat* left, afxNat* top, afxNat* right, afxNat* bottom)
{
    RECT r, c;
    GetWindowRect(window, &r);
    GetClientRect(window, &c);
    *left = c.left - r.left;
    *top = c.top - r.top;
    *right = r.right - c.right;
    *bottom = r.bottom - c.bottom;
}

_AUX HICON _AuxCreateWin32Icon(afxTarga const* tga, afxNat xHotspot, afxNat yHotspot, afxBool icon)
// Creates an RGBA icon or cursor
{
    afxError err = NIL;
    HICON handle = NIL;

    BITMAPV5HEADER bi = { 0 };
    bi.bV5Size = sizeof(bi);
    bi.bV5Width = tga->whd[0];
    bi.bV5Height = -((LONG)tga->whd[1]);
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5AlphaMask = 0xff000000; // ARGB
    bi.bV5RedMask   = 0x00ff0000;
    bi.bV5GreenMask = 0x0000ff00;
    bi.bV5BlueMask  = 0x000000ff;

    afxByte* dst = NIL;
    HDC dc = GetDC(NULL);
    HBITMAP color = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&dst, NULL, (DWORD)0);
    ReleaseDC(NULL, dc);

    if (!color) AfxThrowError();
    else
    {
        HBITMAP mask;

        if (!(mask = CreateBitmap(tga->whd[0], tga->whd[1], 1, 1, NULL))) AfxThrowError();
        else
        {
            afxByte const* src = tga->data.src;

            for (afxNat i = 0; i < tga->whd[0] * tga->whd[1]; i++)
            {
                dst[0] = src[2];
                dst[1] = src[1];
                dst[2] = src[0];
                dst[3] = src[3];
                dst += 4;
                src += 4;
            }

            ICONINFO ii = { 0 };
            ii.fIcon = icon;
            ii.xHotspot = xHotspot;
            ii.yHotspot = yHotspot;
            ii.hbmMask = mask;
            ii.hbmColor = color;

            if (!(handle = CreateIconIndirect(&ii)))
                AfxThrowError();

            DeleteObject(mask);
        }
        DeleteObject(color);
    }
    return handle;
}

_AUX HICON _AuxCreateWin32IconFromRaster(afxRaster ras, afxNat xHotspot, afxNat yHotspot, afxBool icon)
// Creates an RGBA icon or cursor
{
    afxError err = NIL;
    HICON handle = NIL;

    afxWhd whd;
    AfxGetRasterExtent(ras, 0, whd);
    afxPixelFormat fmt = AfxGetRasterFormat(ras);
    AfxAssert(fmt == afxPixelFormat_ARGB8);

    BITMAPV5HEADER bi = { 0 };
    bi.bV5Size = sizeof(bi);
    bi.bV5Width = whd[0];
    bi.bV5Height = -((LONG)whd[1]);
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5AlphaMask = 0xff000000; // ARGB
    bi.bV5RedMask = 0x00ff0000;
    bi.bV5GreenMask = 0x0000ff00;
    bi.bV5BlueMask = 0x000000ff;

    afxByte* dst = NIL;
    HDC dc = GetDC(NULL);
    HBITMAP color = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&dst, NULL, (DWORD)0);
    ReleaseDC(NULL, dc);

    if (!color) AfxThrowError();
    else
    {
        afxRasterIo iop = { 0 };
        iop.rowStride = whd[0] * (bi.bV5BitCount / 8);
        AfxWhdCopy(iop.rgn.whd, whd);

        if (AfxDumpRaster(ras, 0, 1, &iop, dst))
            AfxThrowError();

        afxDrawContext dctx = AfxGetRasterContext(ras);
        AfxWaitForIdleDrawBridge(dctx, 0);

        HBITMAP mask;

        if (!(mask = CreateBitmap(whd[0], whd[1], 1, 1, NULL))) AfxThrowError();
        else
        {
            ICONINFO ii = { 0 };
            ii.fIcon = icon;
            ii.xHotspot = xHotspot;
            ii.yHotspot = yHotspot;
            ii.hbmMask = mask;
            ii.hbmColor = color;

            if (!(handle = CreateIconIndirect(&ii)))
                AfxThrowError();

            DeleteObject(mask);
        }
        DeleteObject(color);
    }
    return handle;
}

_AUX LRESULT WINAPI _AuxWndHndlngPrcW32Callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    afxError err = AFX_ERR_NONE;
    afxWindow wnd = (afxWindow)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (wnd)
    {
        AfxAssertObjects(1, &wnd, afxFcc_WND);

        static afxByte buf[4096] = { 0 };
        afxNat bufSiz = sizeof(buf);

        afxNat32 tid = AfxGetTid();
        afxNat32 doutTid = AfxGetObjectTid(wnd);
        AfxAssert(doutTid == tid);
        
        AfxAssert(wnd->hWnd == hWnd);
#if 0
        HDC dc = wnd->m.w32.hDc;

        if (dc)
        {
            SwapBuffers(dc); // deadlocks all
        }
#endif

        switch (message)
        {
        case WM_INPUT_DEVICE_CHANGE:
        {
            HANDLE hDevice = (void*)lParam;
            AfxAssert(hDevice);
            RID_DEVICE_INFO info = { 0 };
            info.cbSize = sizeof(info);
            UINT cbSize = info.cbSize;
            GetRawInputDeviceInfoA(hDevice, RIDI_DEVICEINFO, &info, &cbSize);

            if (wParam == GIDC_ARRIVAL)
            {
                AfxLogAdvertence("HID %p connected.", hDevice);
            }
            else if (wParam == GIDC_REMOVAL)
            {
                AfxLogAdvertence("HID %p disconnected.", hDevice);
            }
            else
            {
                AfxThrowError();
            }
            break;
        }
        case WM_INPUT:
        {
            afxBool foregroundInput = (GET_RAWINPUT_CODE_WPARAM(wParam) == RIM_INPUT); // Only handle foreground events.
            
            //if (foregroundInput) // Only handle foreground events.
            {
                HRAWINPUT hRawInput = (void*)lParam;
                GetRawInputData(hRawInput, RID_INPUT, NIL, &(bufSiz), sizeof(RAWINPUTHEADER));
                
                if (bufSiz > 0) // Ignore empty packets.
                {
                    if ((bufSiz != GetRawInputData(hRawInput, RID_INPUT, buf, &(bufSiz), sizeof(RAWINPUTHEADER)))) AfxThrowError();
                    else
                    {
                        RAWINPUT* rid = (RAWINPUT*)buf;

                        if (rid->header.dwType == RIM_TYPEMOUSE)
                        {
                            afxNat butChangeCnt = 0;
                            afxMouseButton buttons[AFX_MB_TOTAL];
                            afxBool pressed[AFX_MB_TOTAL];
                            USHORT usButtonFlags = rid->data.mouse.usButtonFlags;

                            if (usButtonFlags & (RI_MOUSE_LEFT_BUTTON_DOWN | RI_MOUSE_LEFT_BUTTON_UP))
                            {
                                buttons[butChangeCnt] = AFX_LMB;
                                pressed[butChangeCnt] = (RI_MOUSE_LEFT_BUTTON_DOWN == (usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN));
                                butChangeCnt++;
                            }

                            if (usButtonFlags & (RI_MOUSE_RIGHT_BUTTON_DOWN | RI_MOUSE_RIGHT_BUTTON_UP))
                            {
                                buttons[butChangeCnt] = AFX_RMB;
                                pressed[butChangeCnt] = (RI_MOUSE_RIGHT_BUTTON_DOWN == (usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN));
                                butChangeCnt++;
                            }

                            if (usButtonFlags & (RI_MOUSE_MIDDLE_BUTTON_DOWN | RI_MOUSE_MIDDLE_BUTTON_UP))
                            {
                                buttons[butChangeCnt] = AFX_MMB;
                                pressed[butChangeCnt] = (RI_MOUSE_MIDDLE_BUTTON_DOWN == (usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN));
                                butChangeCnt++;
                            }

                            if (usButtonFlags & ((RI_MOUSE_BUTTON_4_DOWN | RI_MOUSE_BUTTON_4_UP) | (RI_MOUSE_BUTTON_5_DOWN | RI_MOUSE_BUTTON_5_UP)))
                            {
                                if (usButtonFlags & (RI_MOUSE_BUTTON_4_DOWN | RI_MOUSE_BUTTON_4_UP))
                                {
                                    buttons[butChangeCnt] = AFX_XMB1;
                                    pressed[butChangeCnt] = (RI_MOUSE_BUTTON_4_DOWN == (usButtonFlags & RI_MOUSE_BUTTON_4_DOWN));
                                    butChangeCnt++;
                                }

                                if (usButtonFlags & (RI_MOUSE_BUTTON_5_DOWN | RI_MOUSE_BUTTON_5_UP))
                                {
                                    buttons[butChangeCnt] = AFX_XMB2;
                                    pressed[butChangeCnt] = (RI_MOUSE_BUTTON_5_DOWN == (usButtonFlags & RI_MOUSE_BUTTON_5_DOWN));
                                    butChangeCnt++;
                                }
                            }

                            if (butChangeCnt)
                            {
                                AfxEmulateMouseButtonActions(0, butChangeCnt, buttons, pressed);

                                for (afxNat i = 0; i < butChangeCnt; i++)
                                {
                                    auxEventId evtype;

                                    switch (buttons[i])
                                    {
                                    case AFX_LMB:
                                    {
                                        evtype = auxEventId_LMB;
                                        break;
                                    }
                                    case AFX_RMB:
                                    {
                                        evtype = auxEventId_RMB;
                                        break;
                                    }
                                    case AFX_MMB:
                                    {
                                        evtype = auxEventId_MMB;
                                        break;
                                    }
                                    case AFX_XMB1:
                                    {
                                        evtype = auxEventId_XMB1;
                                        break;
                                    }
                                    case AFX_XMB2:
                                    {
                                        evtype = auxEventId_XMB2;
                                        break;
                                    }
                                    default:
                                    {
                                        AfxThrowError();
                                        evtype = NIL;
                                        break;
                                    }
                                    }

                                    if (evtype)
                                    {
                                        auxEvent ev = { 0 };
                                        ev.id = evtype;
                                        ev.wnd = wnd;
                                        ev.hidPortIdx = 0;
                                        //AfxNotifyObject(wnd, (void*)&ev);
                                        AfxEmitEvent(wnd, (void*)&ev);
                                    }
                                }
                            }

                            afxReal motion[2] = { rid->data.mouse.lLastX, rid->data.mouse.lLastY };
                            AfxEmulateMouseMotion(0, motion);

                            auxEvent ev = { 0 };
                            ev.id = auxEventId_AXIS;
                            ev.wnd = wnd;
                            ev.hidPortIdx = 0;
                            //AfxNotifyObject(wnd, (void*)&ev);
                            AfxEmitEvent(wnd, (void*)&ev);

                            if (RI_MOUSE_WHEEL == (usButtonFlags & RI_MOUSE_WHEEL))
                            {
                                afxReal wheel = (afxInt16)rid->data.mouse.usButtonData;
                                AfxEmulateMouseWheelAction(0, wheel);

                                auxEvent ev = { 0 };
                                ev.id = auxEventId_WHEEL;
                                ev.wnd = wnd;
                                ev.hidPortIdx = 0;
                                //AfxNotifyObject(wnd, (void*)&ev);
                                AfxEmitEvent(wnd, (void*)&ev);
                            }
                        }
                        else if (rid->header.dwType == RIM_TYPEKEYBOARD)
                        {
                            afxKey key2 = _win32VkToQwadro[rid->data.keyboard.VKey];
                            //afxKey key = vkDereferenceMap[rid->data.keyboard.VKey];
                            afxNat8 pressure = (RI_KEY_BREAK == (rid->data.keyboard.Flags & RI_KEY_BREAK)) ? 0x00 : 0xFF; //!!(rid->data.keyboard.Message == WM_KEYDOWN || rid->data.keyboard.Message == WM_SYSKEYDOWN);
                            AfxEmulatePressedKeys(0, 1, &key2, &pressure, wnd, NIL);

                            auxEvent ev = { 0 };
                            ev.id = auxEventId_KEY;
                            ev.hidPortIdx = 0;
                            ev.wnd = wnd;
                            ev.udd = key2;
                            //AfxNotifyObject(wnd, (void*)&ev);
                            AfxEmitEvent(wnd, (void*)&ev);
                        }
                    }
                }
            }
            break;
        }
        case WM_SYSCOMMAND: // Intercept System Commands
        {
            switch (wParam & 0xfff0)
            {
            case SC_SCREENSAVE: // screensaver trying to start
            case SC_MONITORPOWER: // monitor trying to enter powersave mode
            {
                if (!wnd->m.fullscreen) break;
                else
                {
                    // We are running in full screen mode, so disallow screen saver and screen blanking
                    return 0;
                }
            }case SC_KEYMENU: // user trying to access window menu (using ALT key)
            {
                //if (!wnd->m.w32.keymenu)
                  //  return 0;

                break;
            }
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
            ShowWindow(hWnd, SW_MINIMIZE);
            return 0; // handled. Also prevent process at end of this function
        }
        case WM_ACTIVATE: // Watch For Window Activate Message
        {
            if (!HIWORD(wParam)) // Check Minimization State
            {
                wnd->m.active = TRUE; // Program Is Active
            }
            else
            {
                wnd->m.active = FALSE; // Program Is No Longer Active
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

                afxRect cr = { 0 };
                cr.w = LOWORD(lParam);
                cr.h = HIWORD(lParam);

                if (cr.w * cr.h) // don't set to zero
                {
                    AfxAdjustWindow(wnd, NIL, &cr);
                }
            }
            //AfxDrawOutputProcess(dout);
            break;
        }
        case WM_DISPLAYCHANGE: // The WM_DISPLAYCHANGE message is sent to all windows when the display resolution has changed.
        {
            HDC dc;
            afxDrawOutput dout = wnd->m.dout;
            AfxGetDrawOutputIdd(dout, 0, &dc);
            afxWhd const resolution = { GetDeviceCaps(dc, HORZRES), GetDeviceCaps(dc, VERTRES), GetDeviceCaps(dc, PLANES) };
            afxReal64 physAspRatio = AfxFindPhysicalAspectRatio(GetDeviceCaps(dc, HORZSIZE), GetDeviceCaps(dc, VERTSIZE));
            afxReal refreshRate = GetDeviceCaps(dc, VREFRESH);
            AfxResetDrawOutputResolution(dout, resolution, refreshRate, physAspRatio);

            afxDesktop* dwm = wnd->m.dwm;
            dwm->wpOverHp = physAspRatio;
            dwm->refreshRate = refreshRate;
            dwm->res[0] = resolution[0];
            dwm->res[1] = resolution[1];
            dwm->res[2] = resolution[2];
            dwm->wrOverHr = (afxReal64)dwm->res[0] / (afxReal64)dwm->res[1];
            
            break;
        }
        case WM_STYLECHANGED: // Sent to a window after the SetWindowLong function has changed one or more of the window's styles.
        {
            // Sent to a window after the SetWindowLong function has changed one or more of the window's styles. A window receives this message through its WindowProc function.
            // wParam = Indicates whether the window's styles or extended window styles have changed. This parameter can be one or more of the following values.
            
            afxNat mleft, mtop, mright, mbottom;
            CalcWindowMarginsW32(wnd->hWnd, &mleft, &mtop, &mright, &mbottom);
            wnd->m.marginL = mleft;
            wnd->m.marginT = mtop;
            wnd->m.marginR = mright;
            wnd->m.marginB = mbottom;

            break;
        }
        case WM_MOUSEMOVE:
        {
            // TODO build a kind of handler for draw input to deal with it.

            POINTS points = MAKEPOINTS(lParam);

            afxV2d curr = { AfxScalar(points.x), AfxScalar(points.y) };

            AfxV2dSub(wnd->m.cursorMove, wnd->m.cursorPos, curr);
            AfxV2dCopy(wnd->m.cursorPos, curr);

            afxV2d screen = { AfxScalar(wnd->m.frameRect.w), AfxScalar(wnd->m.frameRect.h) };

            AfxNdcV2d(wnd->m.cursorPosNdc, wnd->m.cursorPos, screen);
            AfxNdcV2d(wnd->m.cursorMoveNdc, wnd->m.cursorMove, screen);

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
            if (wnd->m.cursorDisabled)
                0;//AfxEnableCursor(wnd);
            else if (wnd->m.cursorConfined)
                AfxLiberateCursor(wnd);

            wnd->m.focused = FALSE;
            break;
        }
        case WM_SETFOCUS: // Sent to a window after it has gained the keyboard focus.
        {
            wnd->m.focused = TRUE;

            if (!AfxCursorIsOnSurface(wnd)) break; // Don't handle frame interaction; just handle cursor in surface.
            else
            {
                if (wnd->m.cursorDisabled)
                    0;//AfxDisableCursor(wnd);
                else if (wnd->m.cursorConfined)
                    AfxConfineCursor(wnd);

                return 0;
            }
        }
        case WM_PAINT:
        {
            ValidateRect(hWnd, NULL);
            //SetWindowTextA(wnd->m.wnd, AfxGetStringData(&wnd->m.caption.str, 0));
            AfxRedrawWindow(wnd, NIL);
            return 0; // An application returns zero if it processes this message.
        }
        case WM_ERASEBKGND:
        {
            // Flicker is usually caused by interference via WM_ERASEBKGND. 
            // If you haven't already, intercept WM_ERASEBKGND and do nothing in the regions where you are displaying OpenGL content.            
            ValidateRect(hWnd, NULL);
            AfxRedrawWindow(wnd, NIL);
            return 1; // An application should return nonzero if it erases the background; otherwise, it should return zero.
        }
        default: break;
        }
    }
    return DefWindowProcA(hWnd, message, wParam, lParam);
}

#ifdef AFX_OS_WIN10
_AUX BOOL CALLBACK FindShellWorkerWindowW32(HWND hwnd, LPARAM lParam)
{
    HWND* found = (HWND*)lParam;

    if (FindWindowExA(hwnd, NULL, "SHELLDLL_DefView", NULL))
        *found = FindWindowExA(NULL, hwnd, "WorkerW", NULL);

    return TRUE;
}
#endif

_AUX HWND FindShellBackgroundWindowW32(void)
{
    HWND hwnd = 0;
#ifdef AFX_OS_WIN10
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

_AUX afxError AfxChangeWindowIcon(afxWindow wnd, afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    HICON hIcon = NIL;

    if (ras)
    {
        if (!(hIcon = _AuxCreateWin32IconFromRaster(ras, 0, 0, TRUE))) AfxThrowError();
        else
        {
            
            HWND hWnd = wnd->hWnd;
            //Change both icons to the same icon handle.
            SendMessageA(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessageA(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            //This will ensure that the application icon gets changed too.
            SendMessageA(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessageA(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        }
    }

    if (!err)
    {
        if (wnd->hIcon)
            DestroyIcon(wnd->hIcon);

        wnd->hIcon = hIcon;
    }
    return err;
}

_AUX afxError AfxLoadWindowIcon(afxWindow wnd, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(uri);
    afxDrawContext dctx;

    if (!AfxGetDrawOutputContext(wnd->m.dout ? wnd->m.dout : wnd->m.frameDout, &dctx)) AfxThrowError();
    else
    {
        afxRaster ras;

        if (AfxLoadRasters(dctx, afxRasterUsage_SRC, afxRasterFlag_2D, 1, uri, &ras)) AfxThrowError();
        else
        {
            if (AfxChangeWindowIcon(wnd, ras))
                AfxThrowError();

            AfxReleaseObjects(1, &ras);
        }
    }
    return err;
}

_AUX afxBool DoutNotifyOvy(afxWindow wnd, afxNat bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    wnd->lastBufIdx = bufIdx;
    wnd->swap = 1;

    return 1;
}

_AUX afxError AfxRedrawWindow(afxWindow wnd, afxRect const* surr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    HDC dc = wnd->hSurfaceDc;
    
    if (dc)
    {
        if (wnd->swap)
        {
            wnd->swap = FALSE;
            //SwapBuffers(dc);
            //DwmFlush();

            afxNat rate = 0;
            AfxGetDrawOutputFrequency(wnd->m.dout, &rate);
            AfxFormatWindowCaption(wnd, "%u --- Draw I/O System --- Qwadro Execution Ecosystem (c) 2017 SIGMA Technology --- Public Test Build", rate);
        }
    }
    return err;
}

_AUX afxClass* AfxGetWidgetClass(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxClass *cls = &wnd->m.widCls;
    AfxAssertClass(cls, afxFcc_WID);
    return cls;
}

_AUX afxBool AfxGetWindowDrawOutput(afxWindow wnd, afxDrawOutput* frame, afxDrawOutput* surface)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxDrawOutput dout;
    afxBool rslt = 0;

    if (frame)
    {
        if ((dout = wnd->m.frameDout))
        {
            AfxAssertObjects(1, &dout, afxFcc_DOUT);
            ++rslt;
        }
        *frame = dout;
    }

    if (surface)
    {
        if ((dout = wnd->m.dout))
        {
            AfxAssertObjects(1, &dout, afxFcc_DOUT);
            ++rslt;
        }
        *surface = dout;
    }
    return rslt;
}

_AUX void AfxStepWindow(afxWindow wnd, afxReal64* ct, afxReal64* dt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxClock currClock;
    AfxGetClock(&currClock);
    AfxAssert(ct);
    *ct = AfxGetSecondsElapsed(&wnd->m.startClock, &currClock);
    AfxAssert(dt);
    *dt = AfxGetSecondsElapsed(&wnd->m.lastClock, &currClock);
    wnd->m.lastClock = currClock;
}

_AUX afxNat AfxFormatWindowCaption(afxWindow wnd, afxChar const* fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(fmt);
    va_list va;
    va_start(va, fmt);
    afxNat len = AfxFormatStringArg(&wnd->m.caption.str, fmt, va);
    SetWindowTextA(wnd->hWnd, AfxGetStringData(&wnd->m.caption.str, 0));
    va_end(va);
    return len;
}

_AUX void AfxGetWindowRect(afxWindow wnd, afxRect* frame, afxRect* surface)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(frame || surface);

    if (frame)
        *frame = wnd->m.frameRect;

    if (surface)
        *surface = wnd->m.surfaceRect;
}

_AUX afxBool AfxTraceScreenToSurface(afxWindow wnd, afxNat const screenPos[2], afxNat surfPos[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(screenPos);
    POINT point = { .x = screenPos[0],.y = screenPos[1] };
    afxBool rslt = !!ScreenToClient(wnd->hWnd, &point);
    AfxAssert(surfPos);
    surfPos[0] = point.x;
    surfPos[1] = point.y;
    return rslt;
}

_AUX afxBool AfxTraceSurfaceToScreen(afxWindow wnd, afxNat const surfPos[2], afxNat screenPos[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(surfPos);
    POINT point = { .x = surfPos[0], .y = surfPos[1] };
    afxBool rslt = !!ClientToScreen(wnd->hWnd, &point);
    AfxAssert(screenPos);
    screenPos[0] = point.x;
    screenPos[1] = point.y;
    return rslt;
}

_AUX afxBool AfxMoveWindow(afxWindow wnd, afxNat const pos[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    return !!SetWindowPos(wnd->hWnd, NIL, pos[0], pos[1], 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}

_AUX afxError AfxAdjustWindow(afxWindow wnd, afxRect const* frame, afxRect const* surface)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxDesktop* dwm = wnd->m.dwm;

    if (frame)
    {
        AfxAssert3(frame, frame->w, frame->h);

        afxRect rc2;
        rc2.x = AfxMinu(frame->x, dwm->res[0] - 1);
        rc2.y = AfxMinu(frame->y, dwm->res[1] - 1);
        rc2.w = AfxMax(1, AfxMin(frame->w, dwm->res[0]));
        rc2.h = AfxMax(1, AfxMin(frame->h, dwm->res[1]));

        if ((wnd->m.frameRect.x != rc2.x) ||
            (wnd->m.frameRect.y != rc2.y) ||
            (wnd->m.frameRect.w != rc2.w) ||
            (wnd->m.frameRect.h != rc2.h))
        {
            afxInt32 extraWndWidth, extraWndHeight;
            CalcWindowValuesW32(wnd->hWnd, &extraWndWidth, &extraWndHeight);

            AfxAssert2(rc2.w, rc2.h);
            wnd->m.frameRect = rc2;

            wnd->m.surfaceRect.x = wnd->m.marginL;
            wnd->m.surfaceRect.y = wnd->m.marginT;
            wnd->m.surfaceRect.w = AfxMin(AfxMax(1, rc2.w - wnd->m.marginR - wnd->m.marginL), wnd->m.frameRect.w);
            wnd->m.surfaceRect.h = AfxMin(AfxMax(1, rc2.h - wnd->m.marginB - wnd->m.marginT), wnd->m.frameRect.h);

            if (!SetWindowPos(wnd->hWnd, NULL, 0, 0, wnd->m.frameRect.w, wnd->m.frameRect.h, SWP_NOZORDER))
                AfxThrowError();

            afxWhd whd;
            afxDrawOutput dout = wnd->m.dout;
            AfxAssertObjects(1, &dout, afxFcc_DOUT);
            AfxGetDrawOutputExtent(dout, whd);
            whd[0] = wnd->m.surfaceRect.w;
            whd[1] = wnd->m.surfaceRect.h;

            if (AfxAdjustDrawOutput(dout, whd))
                AfxThrowError();
        }
    }

    if (surface)
    {
        AfxAssert2(wnd->m.frameRect.w > (afxNat)surface->x, wnd->m.frameRect.h > (afxNat)surface->y);
        //AfxAssert4(surface->w, wnd->m.frameRect.w > (afxNat)area->w, surface->h, wnd->m.frameRect.h > (afxNat)surface->h);

        afxRect rc2;
        rc2.x = AfxMinu(surface->x, wnd->m.frameRect.w - 1);
        rc2.y = AfxMinu(surface->y, wnd->m.frameRect.h - 1);
        rc2.w = AfxMax(1, surface->w/*AfxMin(surface->w, wnd->m.frameRect.w)*/);
        rc2.h = AfxMax(1, surface->h/*AfxMin(surface->h, wnd->m.frameRect.h)*/);

        if ((wnd->m.surfaceRect.x != rc2.x) ||
            (wnd->m.surfaceRect.y != rc2.y) ||
            (wnd->m.surfaceRect.w != rc2.w) ||
            (wnd->m.surfaceRect.h != rc2.h))
        {
            afxInt32 extraWndWidth, extraWndHeight;
            CalcWindowValuesW32(wnd->hWnd, &extraWndWidth, &extraWndHeight);

            AfxAssert2(rc2.w, rc2.h);
            wnd->m.frameRect.w = rc2.w + extraWndWidth;
            wnd->m.frameRect.h = rc2.h + extraWndHeight;
            wnd->m.surfaceRect = rc2;

            if (!SetWindowPos(wnd->hWnd, NULL, 0, 0, wnd->m.frameRect.w, wnd->m.frameRect.h, SWP_NOMOVE | SWP_NOZORDER))
                AfxThrowError();

            afxDrawOutput dout = wnd->m.dout;

            if (dout)
            {
                AfxAssertObjects(1, &dout, afxFcc_DOUT);

                afxWhd whd;
                AfxGetDrawOutputExtent(dout, whd);
                whd[0] = wnd->m.surfaceRect.w;
                whd[1] = wnd->m.surfaceRect.h;

                if (AfxAdjustDrawOutput(dout, whd))
                    AfxThrowError();
            }
        }
    }
    return err;
}

_AUX afxError AfxAdjustWindowFromNdc(afxWindow wnd, afxV2d const offset, afxV2d const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(extent);

    afxDesktop* dwm = wnd->m.dwm;
    afxRect rc = { 0 };

    if (offset)
    {
        rc.x = (afxNat)AfxUnndcf(offset[0], dwm->res[0] - 1);
        rc.y = (afxNat)AfxUnndcf(offset[1], dwm->res[1] - 1);
    }

    AfxAssert(extent);
    rc.w = (afxNat)AfxUnndcf(extent[0], dwm->res[0]);
    rc.h = (afxNat)AfxUnndcf(extent[1], dwm->res[1]);

    if (AfxAdjustWindow(wnd, &rc, NIL))
        AfxThrowError();

    return err;
}

_AUX afxError AfxAdjustWindowSurfaceFromNdc(afxWindow wnd, afxV2d const offset, afxV2d const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    afxDesktop* dwm = wnd->m.dwm;
    afxRect rc = { 0 };

    if (offset)
    {
        rc.x = (afxNat)AfxUnndcf(offset[0], wnd->m.frameRect.w - 1);
        rc.y = (afxNat)AfxUnndcf(offset[1], wnd->m.frameRect.h - 1);
    }

    AfxAssert(extent);
    rc.w = (afxNat)AfxUnndcf(extent[0], dwm->res[0]);
    rc.h = (afxNat)AfxUnndcf(extent[1], dwm->res[1]);

    if (AfxAdjustWindow(wnd, NIL, &rc))
        AfxThrowError();

    return err;
}

_AUX afxBool _AuxWndStdEventCb(afxWindow wnd, auxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    switch (ev->id)
    {
    case auxEventId_KEY:
    {
        if (AfxWasKeyPressed(0, afxKey_PRINT))
        {
            if (wnd->m.dout)
            {
                afxUri2048 uri;
                AfxMakeUri2048(&uri, NIL);
                AfxFormatUri(&uri.uri, "tmp/ss-wnd%u-%u.tga", AfxGetObjectId((void*)wnd), AfxGetTimer());
                AfxPrintDrawOutputBuffer(wnd->m.dout, 0, 0, &uri.uri);
            }
        }   
        break;
    }
    }
    return TRUE;
}

_AUX afxError _AuxWndCtorCb(afxWindow wnd, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    afxShell usys = cookie->udd[0];
    afxWindowConfig const* cfg = cookie->udd[1];
    afxDrawContext dctx = cookie->udd[2];

    if (cfg && cfg->eventCb)
        AfxResetEventHandler((void*)wnd, (void*)cfg->eventCb);

    wnd->m.dwm = &TheShell.m.dwm;

    wnd->m.active = FALSE;
    wnd->m.focused = FALSE;
    AfxV2dZero(wnd->m.cursorPos);
    AfxV2dZero(wnd->m.cursorMove);
    AfxV2dZero(wnd->m.cursorPosNdc);
    AfxV2dZero(wnd->m.cursorMoveNdc);

    AfxGetClock(&wnd->m.startClock);
    wnd->m.lastClock = wnd->m.startClock;

    AfxV2dZero(wnd->m.grabPoint);
    AfxV2dZero(wnd->m.hoveredPoint);

    wnd->m.grabbedWidg = NIL;
    wnd->m.hoveredWidg = NIL;
    wnd->m.focusedWidg = NIL;

    AfxDeployChain(&wnd->m.classes, wnd);
    AfxRegisterClass(&wnd->m.widCls, NIL, &wnd->m.classes, &_AuxWidMgrCfg);

    afxString s;
    AfxMakeString(&s, 0, usys->wndClss.lpszClassName, 0);
    AfxMakeString2048(&wnd->m.caption, &s);

    afxBool fullscreen = FALSE;
    afxBool decorated = TRUE;
    afxBool resizable = TRUE;
    afxBool floating = FALSE;

    DWORD dwExStyle = WS_EX_APPWINDOW; // Window Extended Style
    DWORD dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS; // Window Style

    if (fullscreen)
    {
        dwExStyle |= WS_EX_TOPMOST;
        dwStyle |= WS_POPUP;
        //ShowCursor(FALSE);
    }
    else
    {
        if (floating)
            dwExStyle |= WS_EX_TOPMOST;

        dwStyle |= WS_SYSMENU | WS_MINIMIZEBOX;

        if (!decorated) dwStyle |= WS_POPUP;
        else
        {
            dwStyle |= WS_CAPTION;

            if (resizable)
                dwStyle |= WS_MAXIMIZEBOX | WS_THICKFRAME;
        }
    }

    HWND hWnd = CreateWindowExA(dwExStyle, usys->wndClss.lpszClassName, usys->wndClss.lpszClassName, dwStyle, 0, 0, 1, 1, NIL, NIL, usys->wndClss.hInstance, NIL);

    if (!hWnd) AfxThrowError();
    else
    {
        wnd->hIcon = NIL;
        wnd->hFrameDc = NIL;
        wnd->hSurfaceDc = NIL;
        wnd->hWnd = hWnd;
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)wnd);

        DragAcceptFiles(hWnd, TRUE);
            
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);
        afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxNat ddevId = AfxGetObjectId(ddev);
        AfxAssert(ddevId != AFX_INVALID_INDEX);
            
        afxDrawOutput dout;
        afxDrawOutputConfig doutCfg = { 0 };
        AfxConfigureDrawOutput(ddevId, &doutCfg);

        if (cfg)
        {
            doutCfg = cfg->surface;
        }
        else
        {
            doutCfg.pixelFmtDs[0] = afxPixelFormat_D24;
            //doutCfg.pixelFmtDs[0] = afxPixelFormat_D24S8;
            //doutCfg.pixelFmtDs[1] = afxPixelFormat_D24S8;
            doutCfg.bufCnt = 3; // triple buffered            
        }

        doutCfg.endpointNotifyObj = wnd;
        doutCfg.endpointNotifyFn = (void*)DoutNotifyOvy;
        doutCfg.w32.hInst = usys->wndClss.hInstance;
        doutCfg.w32.hWnd = hWnd;
        doutCfg.udd[0] = &wnd->hSurfaceDc;

        if (AfxAcquireDrawOutput(ddevId, &doutCfg, &dout)) AfxThrowError();
        else
        {
            wnd->m.dout = dout;
            wnd->m.dctx = dctx;
            //AfxReconnectDrawOutput(dout, dctx);

            if (!cfg) AfxAdjustWindowFromNdc(wnd, NIL, AfxV2d(0.5, 0.5));
            else
            {
                afxRect rc = cfg->rc;
                rc.w = AfxMax(1, rc.w);;
                rc.h = AfxMax(1, rc.h);
                AfxAdjustWindow(wnd, &rc, NIL);
            }                
#if !0
            if (doutCfg.presentAlpha)
            {
                DWM_BLURBEHIND bb = { 0 };
                bb.dwFlags = DWM_BB_ENABLE;
                bb.fEnable = TRUE;
                DwmEnableBlurBehindWindow(wnd->hWnd, &(bb)); // não functiona no chad Windows XP
            }
#endif

            ShowWindow(hWnd, SHOW_OPENWINDOW);
        }

        if (err)
            DestroyWindow(wnd->hWnd);
    }
    return err;
}

_AUX afxError _AuxWndDtorCb(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    AfxCleanUpChainedClasses(&wnd->m.classes);

    //AfxReleaseObjects(1, &wnd->m.dout);

    AfxChangeWindowIcon(wnd, NIL); // detach any custom icon
    AfxAssert(!wnd->hIcon);

    DragAcceptFiles(wnd->hWnd, FALSE);
    DestroyWindow(wnd->hWnd);
    
    return err;
}

_AUX afxClassConfig const _AuxWndMgrCfg =
{
    .fcc = afxFcc_WND,
    .name = "Window",
    .desc = "Desktop Window",
    .fixedSiz = sizeof(AFX_OBJECT(afxWindow)),
    .ctor = (void*)_AuxWndCtorCb,
    .dtor = (void*)_AuxWndDtorCb,
    .event = (void*)_AuxWndStdEventCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireWindow(afxDrawContext dctx, afxWindowConfig const* cfg, afxWindow* window)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
#if 0
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssertRange(TheShell.m.dwmCnt, dwmId, 1);
    afxDesktop* dwm = &TheShell.m.dwms[dwmId];
#endif
    afxClass* cls = AfxGetWindowClass();
    AfxAssertClass(cls, afxFcc_WND);
    afxWindow wnd;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&wnd, (void const*[]) { &TheShell, cfg, dctx })) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &wnd, afxFcc_WND);
        AfxAssert(window);
        *window = wnd;
    }
    return err;
}
