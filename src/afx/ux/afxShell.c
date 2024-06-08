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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_WINDOW_C
#define _AUX_WINDOW_IMPL
#include "qwadro/../_luna/luna_vm.h"
#include "qwadro/../_luna/luna.h"
#include "qwadro/ux/afxShell.h"

//_AUX afxBool shReady = FALSE;
_AUX afxByte theShData[AFX_ALIGN(sizeof(afxObjectBase), 16) + AFX_ALIGN(sizeof(AFX_OBJECT(afxShell)), 16)] = { 0 };
_AUX afxShell TheShell = (void*)&theShData;
AFX_STATIC_ASSERT(sizeof(theShData) >= (sizeof(afxObjectBase) + sizeof(TheShell[0])), "");

extern afxClassConfig const _AuxCtrlMgrCfg;
extern afxClassConfig const _AuxHidMgrCfg;
extern afxClassConfig const _AuxWndMgrCfg;
extern afxClassConfig const _AuxXssMgrCfg;

extern afxError xssDestroyVm(xssVm env);
extern xssVm xssCreateVm(LunaConfiguration const* config);
extern LRESULT WINAPI _AuxWndHndlngPrcW32Callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

_AUX afxManager* AfxGetControllerManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxShell usys;
    AfxGetShell(&usys);
    AfxAssertObjects(1, &usys, afxFcc_USYS);
    afxManager *cls = &usys->ctrlMgr;
    AfxAssertClass(cls, afxFcc_CTRL);
    return cls;
}

_AUX afxManager* AfxGetHidManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxShell usys;
    AfxGetShell(&usys);
    AfxAssertObjects(1, &usys, afxFcc_USYS);
    afxManager *cls = &usys->hidMgr;
    AfxAssertClass(cls, afxFcc_HID);
    return cls;
}

_AUX afxManager* AfxGetWindowManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxShell usys;
    AfxGetShell(&usys);
    AfxAssertObjects(1, &usys, afxFcc_USYS);
    afxManager *cls = &usys->wndMgr;
    AfxAssertClass(cls, afxFcc_WND);
    return cls;
}

_AUX afxManager* AfxGetScriptManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxShell usys;
    AfxGetShell(&usys);
    AfxAssertObjects(1, &usys, afxFcc_USYS);
    afxManager *cls = &usys->xssMgr;
    AfxAssertClass(cls, afxFcc_XSS);
    return cls;
}

// Confines the cursor to a rectangular area on the screen. 
// If a subsequent cursor position (set by the SetCursorPos function or the mouse) lies outside the rectangle, 
// the system automatically adjusts the position to keep the cursor inside the rectangular area.

_AUX afxBool AfxConfineCursor(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    afxShell usys;
    AfxGetShell(&usys);
    AfxAssertObjects(1, &usys, afxFcc_USYS);
    afxBool rslt = FALSE;

    if (!wnd)
    {
        rslt = !!ClipCursor(NULL);
    }
    else
    {
        AfxAssertObjects(1, &wnd, afxFcc_WND);
        HWND hWnd = wnd->w32.hWnd;

        RECT cr;
        GetClientRect(hWnd, &cr);
        ClientToScreen(hWnd, (POINT*)&cr.left);
        ClientToScreen(hWnd, (POINT*)&cr.right);
        rslt = !!ClipCursor(&cr);
    }
    usys->curCapturedOn = wnd;
    return rslt;
}

_AUX afxBool AfxLiberateCursor(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    afxShell usys;
    AfxGetShell(&usys);
    AfxAssertObjects(1, &usys, afxFcc_USYS);
    afxBool rslt = FALSE;

    if (!wnd || (wnd == usys->curCapturedOn))
        rslt = AfxConfineCursor(NIL);

    return rslt;
}

_AUX afxBool AfxCursorIsOnSurface(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    afxShell usys;
    AfxGetShell(&usys);
    AfxAssertObjects(1, &usys, afxFcc_USYS);
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    HWND hWnd = wnd->w32.hWnd;
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

_AUX afxError _AuxUsysCtor(afxShell usys, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &usys, afxFcc_USYS);

    //TheShell = usys;

    if (!(usys->vm = xssCreateVm(NIL)))
        AfxThrowError();

    afxChain* mgrChn = &usys->mgrChn;
    AfxSetUpChain(mgrChn, usys);
    afxClassConfig clsCfg;

    AfxEstablishManager(&usys->hidMgr, /*AfxGetDeviceClass()*/NIL, mgrChn, &_AuxHidMgrCfg);
    AfxEstablishManager(&usys->ctrlMgr, &usys->hidMgr, mgrChn, &_AuxCtrlMgrCfg); // require hid
    AfxEstablishManager(&usys->wndMgr, NIL, mgrChn, &_AuxWndMgrCfg);
    AfxEstablishManager(&usys->xssMgr, NIL, mgrChn, &_AuxXssMgrCfg);

    usys->focusedWnd = NIL;
    usys->curCapturedOn = NIL;

    usys->w32.wndClss.cbSize = sizeof(usys->w32.wndClss); // only on EX
    usys->w32.wndClss.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    usys->w32.wndClss.lpfnWndProc = _AuxWndHndlngPrcW32Callback;
    usys->w32.wndClss.cbClsExtra = sizeof(afxObject);
    usys->w32.wndClss.cbWndExtra = sizeof(afxObject);
    usys->w32.wndClss.hInstance = GetModuleHandleA(NULL);
    usys->w32.wndClss.hIcon = LoadIconA(NULL, IDI_SHIELD);
    usys->w32.wndClss.hCursor = LoadCursorA(NULL, IDC_ARROW);
    usys->w32.wndClss.hbrBackground = NULL;
    usys->w32.wndClss.lpszMenuName = NULL;
    usys->w32.wndClss.lpszClassName = "Advanced User Experience --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build";
    usys->w32.wndClss.hIconSm = LoadIconA(NULL, IDI_SHIELD);

    if (!(RegisterClassExA(&(usys->w32.wndClss)))) AfxThrowError();
    else
    {
        afxNat dwmCnt = 1;
        usys->dwms = NIL;

        if (!(usys->dwms = AfxAllocate(dwmCnt, sizeof(usys->dwms[0]), NIL, AfxHere()))) AfxThrowError();
        else
        {
            usys->dwmCnt = dwmCnt;

            for (afxNat i = 0; i < dwmCnt; i++)
            {
                afxDesktop* dwm = &usys->dwms[i];

#ifdef AFX_OS_WIN
                
                dwm->res[0] = GetSystemMetrics(SM_CXSCREEN);
                dwm->res[1] = GetSystemMetrics(SM_CYSCREEN);
                dwm->res[2] = 1;
                dwm->dout = NIL;
                dwm->refreshRate = 1;
#endif
            }

            afxHid hid;

            if (AfxAcquireHid(0, &hid)) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &hid, afxFcc_HID);
                usys->stdHid = hid;

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
                    AfxLoadScript(&s, &uri);

                    if (err)
                        AfxDismountStorageUnit(&point, &location, afxFileFlag_RWX);
                }

                if (err)
                {
                    AfxReleaseObjects(1, &hid);
                }
            }
        }

        if (err)
            UnregisterClassA(usys->w32.wndClss.lpszClassName, usys->w32.wndClss.hInstance);
    }
    return err;
}

_AUX afxError _AuxUsysDtor(afxShell usys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &usys, afxFcc_USYS);

    if (usys->stdHid)
    {
        AfxReleaseObjects(1, (void*[]) { usys->stdHid });
        usys->stdHid = NIL;
    }

    AfxCleanUpChainedManagers(&usys->mgrChn);

    afxUri point, location;
    AfxMakeUri(&point, "code", 0);
    AfxMakeUri(&location, "system", 0);
    AfxDismountStorageUnit(&point, &location, afxFileFlag_RWX);

    //if (xssDestroyVm(usys->vm))
      //  AfxThrowError();

    UnregisterClassA(usys->w32.wndClss.lpszClassName, usys->w32.wndClss.hInstance);
    //TheShell = NIL;

    return err;
}

_AUX afxClassConfig const _AuxUsysMgrCfg =
{
    .fcc = afxFcc_USYS,
    .name = "Shell",
    .desc = "User I/O System",
    .unitsPerPage = 1,
    .maxCnt = 1,
    //.size = sizeof(AFX_OBJECT(afxShell)),
    .ctor = (void*)_AuxUsysCtor,
    .dtor = (void*)_AuxUsysDtor
};

_AUX afxError AfxSystemIoctl(afxSystem sys, afxModule mdle, afxNat reqCode, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    switch (reqCode)
    {
    case 2:
    {
        AfxAssert(udd);
        udd[0] = (void*)&_AuxUsysMgrCfg;
        udd[1] = TheShell;
        break;
    }
    default: break;
    }
    return err;
}
