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

#define _AUX_UX_C
#define _AFX_SHELL_C
#define _AFX_WINDOW_C
#define _AFX_WINDOW_IMPL
#include "qwadro/../_luna/luna_vm.h"
#include "qwadro/../_luna/luna.h"
#include "qwadro/ux/afxShell.h"

_AUX afxBool shReady = FALSE;
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

_AUX afxBool AfxGetShell(afxShell* shell)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(shell);
    *shell = TheShell;
    return shReady;
}

_AUX afxManager* AfxGetControllerManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, &sh, afxFcc_ENV);
    afxManager *cls = &sh->ctrlMgr;
    AfxAssertClass(cls, afxFcc_CTRL);
    return cls;
}

_AUX afxManager* AfxGetHidManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, &sh, afxFcc_ENV);
    afxManager *cls = &sh->hidMgr;
    AfxAssertClass(cls, afxFcc_HID);
    return cls;
}

_AUX afxManager* AfxGetWindowManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, &sh, afxFcc_ENV);
    afxManager *cls = &sh->wndMgr;
    AfxAssertClass(cls, afxFcc_WND);
    return cls;
}

_AUX afxManager* AfxGetScriptManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, &sh, afxFcc_ENV);
    afxManager *cls = &sh->xssMgr;
    AfxAssertClass(cls, afxFcc_XSS);
    return cls;
}

// Confines the cursor to a rectangular area on the screen. 
// If a subsequent cursor position (set by the SetCursorPos function or the mouse) lies outside the rectangle, 
// the system automatically adjusts the position to keep the cursor inside the rectangular area.

_AUX afxBool AfxConfineCursor(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, &sh, afxFcc_ENV);
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
    sh->curCapturedOn = wnd;
    return rslt;
}

_AUX afxBool AfxLiberateCursor(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, &sh, afxFcc_ENV);
    afxBool rslt = FALSE;

    if (!wnd || (wnd == sh->curCapturedOn))
        rslt = AfxConfineCursor(NIL);

    return rslt;
}

_AUX afxBool AfxCursorIsOnWindowSurface(afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, &sh, afxFcc_ENV);
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

_AUX afxError _AuxShCtor(afxShell sh, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sh, afxFcc_ENV);

    if (!(sh->vm = xssCreateVm(NIL)))
        AfxThrowError();

    afxChain* mgrChn = &sh->mgrChn;
    AfxSetUpChain(mgrChn, sh);
    afxClassConfig clsCfg;

    AfxEstablishManager(&sh->hidMgr, /*AfxGetDeviceClass()*/NIL, mgrChn, &_AuxHidMgrCfg);
    AfxEstablishManager(&sh->ctrlMgr, AfxGetHidManager(), mgrChn, &_AuxCtrlMgrCfg); // require hid
    AfxEstablishManager(&sh->wndMgr, NIL, mgrChn, &_AuxWndMgrCfg);
    AfxEstablishManager(&sh->xssMgr, NIL, mgrChn, &_AuxXssMgrCfg);

    sh->focusedWnd = NIL;
    sh->curCapturedOn = NIL;

    sh->w32.wndClss.cbSize = sizeof(sh->w32.wndClss); // only on EX
    sh->w32.wndClss.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    sh->w32.wndClss.lpfnWndProc = _AuxWndHndlngPrcW32Callback;
    sh->w32.wndClss.cbClsExtra = sizeof(afxObject);
    sh->w32.wndClss.cbWndExtra = sizeof(afxObject);
    sh->w32.wndClss.hInstance = GetModuleHandleA(NULL);
    sh->w32.wndClss.hIcon = LoadIconA(NULL, IDI_SHIELD);
    sh->w32.wndClss.hCursor = LoadCursorA(NULL, IDC_ARROW);
    sh->w32.wndClss.hbrBackground = NULL;
    sh->w32.wndClss.lpszMenuName = NULL;
    sh->w32.wndClss.lpszClassName = "The Unified UX Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build";
    sh->w32.wndClss.hIconSm = LoadIconA(NULL, IDI_SHIELD);

    if (!(RegisterClassExA(&(sh->w32.wndClss)))) AfxThrowError();
    else
    {
        afxHid hid;

        if (AfxAcquireHid(0, &hid)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &hid, afxFcc_HID);
            sh->stdHid = hid;

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

        if (err)
            UnregisterClassA(sh->w32.wndClss.lpszClassName, sh->w32.wndClss.hInstance);
    }
    return err;
}

_AUX afxError _AuxShDtor(afxShell sh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sh, afxFcc_ENV);

    if (sh->stdHid)
    {
        AfxReleaseObjects(1, (void*[]) { sh->stdHid });
        sh->stdHid = NIL;
    }

    AfxCleanUpChainedManagers(&sh->mgrChn);

    afxUri point, location;
    AfxMakeUri(&point, "code", 0);
    AfxMakeUri(&location, "system", 0);
    AfxDismountStorageUnit(&point, &location, afxFileFlag_RWX);

    if (xssDestroyVm(sh->vm))
        AfxThrowError();

    UnregisterClassA(sh->w32.wndClss.lpszClassName, sh->w32.wndClss.hInstance);

    return err;
}

_AUX afxManager* AfxGetShellClass(void)
{
    afxError err = AFX_ERR_NONE;
    static afxManager shMgr = { 0 };
    static afxBool shMgrMounted = FALSE;
    static afxClassConfig shMgrCfg =
    {
        .fcc = afxFcc_ENV,
        .name = "Shell",
        .desc = "Execution Environment",
        .unitsPerPage = 1,
        //.size = sizeof(AFX_OBJECT(afxShell)),
        .ctor = (void*)_AuxShCtor,
        .dtor = (void*)_AuxShDtor
    };

    if (shMgr.fcc != afxFcc_CLS)
    {
        AfxEstablishManager(&shMgr, NIL, /*_AfxGetSystemClassChain()*/NIL, &shMgrCfg);
        shMgrMounted = TRUE;
    }
    AfxAssertClass(&shMgr, afxFcc_ENV);
    return &shMgr;
}

_AUX afxError AfxEntryPoint(afxModule mdle, afxNat reqCode, void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    switch (reqCode)
    {
    case afxFcc_SYS:
    {
        afxShell sh;

        if (!AfxGetShell(&sh))
        {
            AfxAssert(TheShell == sh);
            AfxZero(TheShell, sizeof(afxObjectBase));

            afxManager* mgr = AfxGetShellClass();
            AfxAssertClass(mgr, afxFcc_ENV);

            if (_AfxConstructObjects(mgr, 1, (void**)&TheShell, udd)) AfxThrowError();
            else
            {
                AfxAssert(TheShell != sh); // Attention! Ctor moves the object pointer to hide out the object base.
                sh = TheShell;
                AfxAssertObjects(1, &sh, afxFcc_ENV);
                shReady = TRUE;
            }
        }
        else
        {
            AfxAssert(TheShell == sh);
            AfxAssertObjects(1, &sh, afxFcc_ENV);
            shReady = FALSE;

            afxManager* mgr = AfxGetShellClass();
            AfxAssertClass(mgr, afxFcc_ENV);

            if (_AfxDestructObjects(mgr, 1, (void**)&TheShell))
                AfxThrowError();

            AfxAssert(TheShell != sh); // Attention! Dtor moves the object pointer to expose the object base.
            AfxZero(TheShell, sizeof(afxObjectBase));
        }
    }
    default: break;
    }
    return err;
}
