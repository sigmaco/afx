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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <dwmapi.h>
#include <shellapi.h>
#pragma comment(lib,"dwmapi.lib")

#define _AUX_UX_C
#define _AFX_SHELL_C
#define _AFX_WINDOW_C
#define _AFX_WINDOW_IMPL
#include "qwadro/ux/afxShell.h"

extern afxKey const Scan1MakeToQwadroDereferenceMap[afxKey_TOTAL];

extern afxClassConfig const _AuxWidMgrCfg;

_AUX void Calc_window_values(HWND window, afxInt* out_extra_width, afxInt32* out_extra_height)
{
    RECT r, c;
    GetWindowRect(window, &r);
    GetClientRect(window, &c);
    *out_extra_width = (r.right - r.left) - (c.right - c.left);
    *out_extra_height = (r.bottom - r.top) - (c.bottom - c.top);
}

_AUX HICON _AuxCreateWin32Icon(afxTarga const* tga, afxNat xHotspot, afxNat yHotspot, afxBool icon)
// Creates an RGBA icon or cursor
{
    afxError err = NIL;
    HICON handle = NIL;

    BITMAPV5HEADER bi = { 0 };
    bi.bV5Size = sizeof(bi);
    bi.bV5Width = tga->whd[0];
    bi.bV5Height = -tga->whd[1];
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
        
        AfxAssert(wnd->w32.hWnd == hWnd);
#if 0
        HDC dc = wnd->w32.hDc;

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
                                    afxUxEventId evtype;

                                    switch (buttons[i])
                                    {
                                    case AFX_LMB:
                                    {
                                        evtype = afxUxEventId_LMB;
                                        break;
                                    }
                                    case AFX_RMB:
                                    {
                                        evtype = afxUxEventId_RMB;
                                        break;
                                    }
                                    case AFX_MMB:
                                    {
                                        evtype = afxUxEventId_MMB;
                                        break;
                                    }
                                    case AFX_XMB1:
                                    {
                                        evtype = afxUxEventId_XMB1;
                                        break;
                                    }
                                    case AFX_XMB2:
                                    {
                                        evtype = afxUxEventId_XMB2;
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
                                        afxUxEvent ev = { 0 };
                                        ev.id = evtype;
                                        ev.wnd = wnd;
                                        ev.hidPortIdx = 0;
                                        //AfxNotifyObject(wnd, (void*)&ev);
                                        AfxEmitEvent(wnd, (void*)&ev);
                                    }
                                }
                            }

                            afxReal motion[2] = { (afxReal64)rid->data.mouse.lLastX, (afxReal64)rid->data.mouse.lLastY };
                            AfxEmulateMouseMotion(0, motion);

                            afxUxEvent ev = { 0 };
                            ev.id = afxUxEventId_AXIS;
                            ev.wnd = wnd;
                            ev.hidPortIdx = 0;
                            //AfxNotifyObject(wnd, (void*)&ev);
                            AfxEmitEvent(wnd, (void*)&ev);

                            if (RI_MOUSE_WHEEL == (usButtonFlags & RI_MOUSE_WHEEL))
                            {
                                afxReal wheel = (afxInt16)rid->data.mouse.usButtonData;
                                AfxEmulateMouseWheelAction(0, wheel);

                                afxUxEvent ev = { 0 };
                                ev.id = afxUxEventId_WHEEL;
                                ev.wnd = wnd;
                                ev.hidPortIdx = 0;
                                //AfxNotifyObject(wnd, (void*)&ev);
                                AfxEmitEvent(wnd, (void*)&ev);
                            }
                        }
                        else if (rid->header.dwType == RIM_TYPEKEYBOARD)
                        {
                            afxKey key2 = Scan1MakeToQwadroDereferenceMap[rid->data.keyboard.MakeCode];
                            //afxKey key = vkDereferenceMap[rid->data.keyboard.VKey];
                            afxNat8 pressure = (RI_KEY_BREAK == (rid->data.keyboard.Flags & RI_KEY_BREAK)) ? 0x00 : 0xFF; //!!(rid->data.keyboard.Message == WM_KEYDOWN || rid->data.keyboard.Message == WM_SYSKEYDOWN);
                            AfxEmulatePressedKeys(0, 1, &key2, &pressure, wnd, NIL);

                            afxUxEvent ev = { 0 };
                            ev.id = afxUxEventId_KEY;
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
                if (!wnd->fullscreen) break;
                else
                {
                    // We are running in full screen mode, so disallow screen saver and screen blanking
                    return 0;
                }
            }case SC_KEYMENU: // user trying to access window menu (using ALT key)
            {
                //if (!wnd->w32.keymenu)
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
                wnd->active = TRUE; // Program Is Active
            }
            else
            {
                wnd->active = FALSE; // Program Is No Longer Active
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
                    AfxAdjustWindow(wnd, whdNew);
                }
            }
            //AfxDrawOutputProcess(dout);
            break;
        }
        case WM_DISPLAYCHANGE: // The WM_DISPLAYCHANGE message is sent to all windows when the display resolution has changed.
        {
            afxDrawOutput dout = wnd->dout;
            HDC dc;
            AfxGetDrawOutputIdd(dout, 0, &dc);
            afxWhd const resolution = { GetDeviceCaps(dc, HORZRES), GetDeviceCaps(dc, VERTRES), GetDeviceCaps(dc, PLANES) };
            afxReal64 physAspRatio = AfxFindPhysicalAspectRatio(GetDeviceCaps(dc, HORZSIZE), GetDeviceCaps(dc, VERTSIZE));
            afxNat refreshRate = GetDeviceCaps(dc, VREFRESH);
            AfxAdaptDrawOutputResolution(dout, resolution, refreshRate, physAspRatio);
            break;
        }
        case WM_STYLECHANGED: // Sent to a window after the SetWindowLong function has changed one or more of the window's styles.
        {
            // Sent to a window after the SetWindowLong function has changed one or more of the window's styles. A window receives this message through its WindowProc function.
            // wParam = Indicates whether the window's styles or extended window styles have changed. This parameter can be one or more of the following values.

            RECT rect;
            GetClientRect(wnd->w32.hWnd, &rect);
            afxWhd whdNew = { rect.right - rect.left, rect.bottom - rect.top, 1 };

            if (whdNew[0] * whdNew[1] * whdNew[2]) // don't set to zero
            {
                AfxAdjustWindow(wnd, whdNew);
            }
            break;
        }
        case WM_MOUSEMOVE:
        {
            // TODO build a kind of handler for draw input to deal with it.

            POINTS points = MAKEPOINTS(lParam);

            afxV2d curr = { AfxScalar(points.x), AfxScalar(points.y) };

            AfxSubV2d(wnd->cursorMove, wnd->cursorPos, curr);
            AfxCopyV2d(wnd->cursorPos, curr);

            afxV2d screen = { AfxScalar(wnd->whd[0]), AfxScalar(wnd->whd[1]) };

            AfxNdcV2d(wnd->cursorPosNdc, wnd->cursorPos, screen);
            AfxNdcV2d(wnd->cursorMoveNdc, wnd->cursorMove, screen);

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
            if (wnd->cursorDisabled)
                0;//AfxEnableCursor(wnd);
            else if (wnd->cursorConfined)
                AfxLiberateCursor(wnd);

            wnd->focused = FALSE;
            break;
        }
        case WM_SETFOCUS: // Sent to a window after it has gained the keyboard focus.
        {
            wnd->focused = TRUE;

            if (!AfxCursorIsOnWindowSurface(wnd)) break; // Don't handle frame interaction; just handle cursor in surface.
            else
            {
                if (wnd->cursorDisabled)
                    0;//AfxDisableCursor(wnd);
                else if (wnd->cursorConfined)
                    AfxConfineCursor(wnd);

                return 0;
            }
        }
        case WM_PAINT:
        {
            ValidateRect(hWnd, NULL);
            //SetWindowTextA(wnd->wnd, AfxGetStringData(&wnd->caption.str, 0));
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

_AUX afxError AfxChangeWindowIcon(afxWindow wnd, afxTarga* tga)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    HICON hIcon = NIL;

    if (tga)
    {
        if (!(hIcon = _AuxCreateWin32Icon(tga, 0, 0, TRUE))) AfxThrowError();
        else
        {
            HWND hWnd = wnd->w32.hWnd;
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
        if (wnd->w32.hIcon)
            DestroyIcon(wnd->w32.hIcon);

        wnd->w32.hIcon = hIcon;
    }
    return err;
}

_AUX afxError AfxLoadWindowIcon(afxWindow wnd, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(uri);

    AfxThrowError();

    return err;
}

_AUX afxBool DoutNotifyOvy(afxWindow wnd, afxNat bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    wnd->w32.lastBufIdx = bufIdx;
    wnd->w32.swap = 1;
    return 1;
}

_AUX afxError AfxRedrawWindow(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    
    HDC dc = wnd->w32.hSurfaceDc;

    if (dc)
    {
        if (wnd->w32.swap)
        {
            wnd->w32.swap = FALSE;
            //SwapBuffers(dc);
            DwmFlush();
        }
    }
    return err;
}

_AUX afxManager* AfxGetWidgetClass(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxManager *cls = &wnd->widMgr;
    AfxAssertClass(cls, afxFcc_WID);
    return cls;
}

_AUX afxBool AfxGetWindowDrawOutput(afxWindow wnd, afxDrawOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxDrawOutput dout = wnd->dout;
    AfxTryAssertObjects(1, &dout, afxFcc_DOUT);
    *output = dout;
    return !!dout;
}

_AUX void AfxStepWindow(afxWindow wnd, afxReal64* ct, afxReal64* dt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    afxClock currClock;
    AfxGetClock(&currClock);
    AfxAssert(ct);
    *ct = AfxGetSecondsElapsed(&wnd->startClock, &currClock);
    AfxAssert(dt);
    *dt = AfxGetSecondsElapsed(&wnd->lastClock, &currClock);
    wnd->lastClock = currClock;
}

_AUX afxNat AfxFormatWindowCaption(afxWindow wnd, afxChar const* fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(fmt);
    va_list va;
    va_start(va, fmt);
    afxNat len = AfxFormatStringArg(&wnd->caption, fmt, va);
    SetWindowTextA(wnd->w32.hWnd, AfxGetStringData(&wnd->caption.str, 0));
    va_end(va);
    return len;
}

_AUX afxError AfxAdjustWindow(afxWindow wnd, afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(whd);
    //AfxAssertExtent(wnd->resolution[0], whd[0]);
    //AfxAssertExtent(wnd->resolution[1], whd[1]);
    //AfxAssertExtent(wnd->resolution[2], whd[2]);

    afxBool changed = (wnd->whd[0] != whd[0] || wnd->whd[1] != whd[1] || wnd->whd[2] != whd[2]);

    if (changed)
    {
        afxWhd whd2;
        whd2[0] = AfxMax(whd[0], 1);
        whd2[1] = AfxMax(whd[1], 1);
        whd2[2] = AfxMax(whd[2], 1);

        afxInt32 extraWndWidth, extraWndHeight;
        Calc_window_values(wnd->w32.hWnd, &extraWndWidth, &extraWndHeight);
        SetWindowPos(wnd->w32.hWnd, NULL, 0, 0, whd2[0] + extraWndWidth, whd2[1] + extraWndHeight, SWP_NOMOVE);

        wnd->whd[0] = whd2[0];
        wnd->whd[1] = whd2[1];
        wnd->whd[2] = whd2[2];
        
        if (AfxAdjustDrawOutput(wnd->dout, whd2))
            AfxThrowError();
    }
    return err;
}

_AUX afxError AfxAdjustWindowNdc(afxWindow wnd, afxV3d const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxAssert(whd);

    afxWhd res;
    afxNat refRate;
    AfxGetDrawOutputResolution(wnd->dout, res, &refRate);
    afxWhd const whd2 =
    {
        (afxNat)AfxUnndcf(whd[0], res[0]),
        (afxNat)AfxUnndcf(whd[1], res[1]),
        1
    };
    return AfxAdjustWindow(wnd, whd2);
}

_AUX afxError _AuxWndCtor(afxWindow wnd, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    afxShell sh = cookie->udd[0];
    afxDrawContext dctx = cookie->udd[1];
    afxNat const* whd = cookie->udd[2];

    wnd->active = FALSE;
    wnd->focused = FALSE;
    AfxZeroV2d(wnd->cursorPos);
    AfxZeroV2d(wnd->cursorMove);
    AfxZeroV2d(wnd->cursorPosNdc);
    AfxZeroV2d(wnd->cursorMoveNdc);

    AfxGetClock(&wnd->startClock);
    wnd->lastClock = wnd->startClock;

    AfxZeroV2d(wnd->grabPoint);
    AfxZeroV2d(wnd->hoveredPoint);

    wnd->grabbedWidg = NIL;
    wnd->hoveredWidg = NIL;
    wnd->focusedWidg = NIL;

    AfxSetUpChain(&wnd->mgrChn, (void*)wnd);

    AfxEstablishManager(&wnd->widMgr, NIL, &wnd->mgrChn, &_AuxWidMgrCfg);

    AfxAllocateString(&wnd->caption, 512, sh->w32.wndClss.lpszClassName, 0);

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

    HWND hWnd = CreateWindowExA(dwExStyle, sh->w32.wndClss.lpszClassName, sh->w32.wndClss.lpszClassName, dwStyle, 0, 0, 1, 1, NIL, NIL, sh->w32.wndClss.hInstance, NIL);

    if (!hWnd) AfxThrowError();
    else
    {
        wnd->w32.hIcon = NIL;
        wnd->w32.hFrameDc = NIL;
        wnd->w32.hSurfaceDc = NIL;
        wnd->w32.hWnd = hWnd;
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)wnd);

        DragAcceptFiles(hWnd, TRUE);
            
        afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxNat ddevId = AfxGetObjectId(ddev);
            
        afxDrawOutput dout;
        afxDrawOutputConfig doutCfg = { 0 };
        doutCfg.pixelFmtDs[0] = afxPixelFormat_D24;
        doutCfg.bufCnt = 3;
        doutCfg.endpointNotifyObj = wnd;
        doutCfg.endpointNotifyFn = (void*)DoutNotifyOvy;
        doutCfg.w32.hinstance = sh->w32.wndClss.hInstance;
        doutCfg.w32.hwnd = hWnd;
        doutCfg.udd = &wnd->w32.hSurfaceDc;

        if (AfxAcquireDrawOutput(ddevId, &doutCfg, &dout)) AfxThrowError();
        else
        {
            wnd->dout = dout;
            wnd->dctx = dctx;
            AfxReconnectDrawOutput(dout, dctx);

            if (whd)
                AfxAdjustWindow(wnd, whd);
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
            DestroyWindow(wnd->w32.hWnd);
    }
    return err;
}

_AUX afxError _AuxWndDtor(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &wnd, afxFcc_WND);

    AfxCleanUpChainedManagers(&wnd->mgrChn);

    AfxReleaseObjects(1, &wnd->dout);

    AfxChangeWindowIcon(wnd, NIL); // detach any custom icon
    AfxAssert(!wnd->w32.hIcon);

    DragAcceptFiles(wnd->w32.hWnd, FALSE);
    DestroyWindow(wnd->w32.hWnd);
    
    AfxDeallocateString(&wnd->caption);

    return err;
}

_AUX afxClassConfig const _AuxWndMgrCfg =
{
    .fcc = afxFcc_WND,
    .name = "Window",
    .desc = "Desktop Window",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxWindow)),
    .ctor = (void*)_AuxWndCtor,
    .dtor = (void*)_AuxWndDtor
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireWindow(afxDrawContext dctx, afxWhd const whd, afxWindow* window)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    
    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, &sh, afxFcc_ENV);
    afxManager* cls = AfxGetWindowManager();
    AfxAssertClass(cls, afxFcc_WND);
    afxWindow wnd;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&wnd, (void const*[]) { sh, dctx, whd })) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &wnd, afxFcc_WND);
        AfxAssert(window);
        *window = wnd;
    }
    return err;
}
