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

#define _AFX_SYSTEM_C
#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_SHELL_IMPL
#define _AUX_WINDOW_C
#define _AUX_WINDOW_IMPL
#include "qwadro/../_luna/luna_vm.h"
#include "qwadro/../_luna/luna.h"
#include "dev/AuxOverWin32.h"

extern afxClassConfig const _AuxCtrlMgrCfg;
extern afxClassConfig const _AuxHidMgrCfg;
extern afxClassConfig const _AuxWndMgrCfg;
extern afxClassConfig const _AuxXssMgrCfg;

extern afxError xssDestroyVm(xssVm env);
extern xssVm xssCreateVm(LunaConfiguration const* config);
extern LRESULT WINAPI _AuxWndHndlngPrcW32Callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

_AUX AFX_OBJECT(afxShell) TheShell = { 0 };

_AUX afxClass* AfxGetControllerClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &TheShell.m.ctrlCls;
    AfxAssertClass(cls, afxFcc_CTRL);
    return cls;
}

_AUX afxClass* AfxGetHidClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &TheShell.m.hidCls;
    AfxAssertClass(cls, afxFcc_HID);
    return cls;
}

_AUX afxClass* AfxGetWindowClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &TheShell.m.wndCls;
    AfxAssertClass(cls, afxFcc_WND);
    return cls;
}

_AUX afxClass* AfxGetScriptClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass *cls = &TheShell.m.xssCls;
    AfxAssertClass(cls, afxFcc_XSS);
    return cls;
}

// Confines the cursor to a rectangular area on the screen. 
// If a subsequent cursor position (set by the SetCursorPos function or the mouse) lies outside the rectangle, 
// the system automatically adjusts the position to keep the cursor inside the rectangular area.

_AUX afxBool AfxConfineCursor(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxBool rslt = FALSE;

    if (!wnd)
    {
        rslt = !!ClipCursor(NULL);
    }
    else
    {
        AfxAssertObjects(1, &wnd, afxFcc_WND);
        HWND hWnd = wnd->hWnd;

        RECT cr;
        GetClientRect(hWnd, &cr);
        ClientToScreen(hWnd, (POINT*)&cr.left);
        ClientToScreen(hWnd, (POINT*)&cr.right);
        rslt = !!ClipCursor(&cr);
    }
    TheShell.m.curCapturedOn = wnd;
    return rslt;
}

_AUX afxBool AfxLiberateCursor(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxBool rslt = FALSE;

    if (!wnd || (wnd == TheShell.m.curCapturedOn))
        rslt = AfxConfineCursor(NIL);

    return rslt;
}

_AUX afxBool AfxCursorIsOnSurface(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    HWND hWnd = wnd->hWnd;
    afxBool rslt = FALSE;
    POINT pos;

    if (GetCursorPos(&pos))
    {
        if (hWnd == WindowFromPoint(pos))
        {
            RECT cr;
            GetClientRect(hWnd, &cr);
            ClientToScreen(hWnd, (POINT*)&cr.left);
            ClientToScreen(hWnd, (POINT*)&cr.right);
            rslt = PtInRect(&cr, pos);
        }
    }
    return rslt;
}

_AUX afxError AfxSystemIoctl(afxSystem sys, afxModule mdle, afxNat reqCode, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    switch (reqCode)
    {
    case 3:
    {
        AfxAssert(sys->aux.usys == NIL);

        //if (!(TheShell.m.vm = xssCreateVm(NIL)))
          //  AfxThrowError();

        afxClassConfig clsCfg;
        clsCfg = _AuxHidMgrCfg;
        AfxRegisterClass(&TheShell.m.hidCls, /*AfxGetDeviceClass()*/NIL, &sys->classes, &clsCfg);
        clsCfg = _AuxCtrlMgrCfg;
        AfxRegisterClass(&TheShell.m.ctrlCls, &TheShell.m.hidCls, &sys->classes, &clsCfg); // require hid
        clsCfg = _AuxWndMgrCfg;
        AfxRegisterClass(&TheShell.m.wndCls, NIL, &sys->classes, &clsCfg);
        //clsCfg = _AuxXssMgrCfg;
        AfxRegisterClass(&TheShell.m.xssCls, NIL, &sys->classes, &clsCfg);

        TheShell.m.focusedWnd = NIL;
        TheShell.m.curCapturedOn = NIL;

        TheShell.wndClss.cbSize = sizeof(TheShell.wndClss); // only on EX
        TheShell.wndClss.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        TheShell.wndClss.lpfnWndProc = _AuxWndHndlngPrcW32Callback;
        TheShell.wndClss.cbClsExtra = sizeof(afxObject);
        TheShell.wndClss.cbWndExtra = sizeof(afxObject);
        TheShell.wndClss.hInstance = GetModuleHandleA(NULL);
        TheShell.wndClss.hIcon = LoadIconA(NULL, IDI_SHIELD);
        TheShell.wndClss.hCursor = LoadCursorA(NULL, IDC_ARROW);
        TheShell.wndClss.hbrBackground = NULL;
        TheShell.wndClss.lpszMenuName = NULL;
        TheShell.wndClss.lpszClassName = "Unified Shell --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build";
        TheShell.wndClss.hIconSm = LoadIconA(NULL, IDI_SHIELD);

        if (!(RegisterClassExA(&TheShell.wndClss))) AfxThrowError();
        else
        {
            afxNat dwmCnt = 1;

            for (afxNat i = 0; i < dwmCnt; i++)
            {
                afxDesktop* dwm = &TheShell.m.dwm;

                dwm->res[0] = GetSystemMetrics(SM_CXSCREEN);
                dwm->res[1] = GetSystemMetrics(SM_CYSCREEN);
                dwm->res[2] = 1;
                dwm->dout = NIL;
                dwm->refreshRate = 1;
            }

            afxHid hid;

            if (AfxAcquireHid(0, &hid)) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &hid, afxFcc_HID);
                TheShell.m.stdHid = hid;

                afxUri point, location;
                AfxMakeUri(&point, "code", 0);
                AfxMakeUri(&location, "system", 0);

                if (AfxMountStorageUnit(&point, &location, afxFileFlag_RWX)) AfxThrowError();
                else
                {
                    afxUri uri;
                    AfxMakeUri(&uri, "system/qwadro.xss", 0);

                    afxString s;
                    AfxMakeString(&s, "qwadro", 0);
                    //AfxLoadScript(&s, &uri);

                    if (err)
                        AfxDismountStorageUnit(&point, &location, afxFileFlag_RWX);
                }

                if (err)
                {
                    AfxReleaseObjects(1, &hid);
                }
            }

            if (err)
                UnregisterClassA(TheShell.wndClss.lpszClassName, TheShell.wndClss.hInstance);
        }

        if (!err)
        {
            sys->aux.usys = &TheShell;
            TheShell.m.ready = TRUE;
        }
        break;
    }
    case 4:
    {
        AfxAssert(sys->aux.usys == &TheShell);
        TheShell.m.ready = FALSE;
        
        //sizeof(TheShell);

        if (TheShell.m.stdHid)
        {
            AfxReleaseObjects(1, &TheShell.m.stdHid);
            TheShell.m.stdHid = NIL;
        }

        AfxExhaustClass(&TheShell.m.xssCls);
        AfxExhaustClass(&TheShell.m.wndCls);
        AfxExhaustClass(&TheShell.m.ctrlCls);
        AfxExhaustClass(&TheShell.m.hidCls);

        afxUri point, location;
        AfxMakeUri(&point, "code", 0);
        AfxMakeUri(&location, "system", 0);
        AfxDismountStorageUnit(&point, &location, afxFileFlag_RWX);

        UnregisterClassA(TheShell.wndClss.lpszClassName, TheShell.wndClss.hInstance);

        sys->aux.usys = NIL;

        break;
    }
    default: break;
    }
    return err;
}
