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

#ifndef AFX_SHELL_H
#define AFX_SHELL_H

#include "qwadro/ux/afxUxDefs.h"
// ux
#include "qwadro/ux/afxHid.h"
#include "qwadro/ux/afxGamepad.h"
#include "qwadro/ux/afxWindow.h"
#include "qwadro/ux/afxScript.h"
#include "qwadro/ux/xss/xss.h"

#ifdef _AUX_UX_C
#ifdef _AFX_SHELL_C
AFX_OBJECT(afxShell)
{
    afxChain        mgrChn;
    afxManager      wndMgr;
    afxWindow       focusedWnd;
    afxWindow       curCapturedOn;
    afxManager      hidMgr;
    afxManager      ctrlMgr;
    afxManager      xssMgr;
    afxHid          stdHid;

    void*           vm;
    afxNat          scriptedEvCnt;
    afxString const*scriptedEvNames;
    afxStringBase   scriptedEvNameDb;
    afxPool         scriptedEvHandlers;

#ifdef AFX_OS_WIN
    struct
    {
        WNDCLASSEX  wndClss;
    }               w32;
#endif//AFX_OS_WIN
};
#endif//_AFX_SHELL_C
#endif//_AUX_UX_C

AUX afxManager*     AfxGetShellClass(void);

AUX afxBool         AfxGetShell(afxShell* shell);

////////////////////////////////////////////////////////////////////////////////

AUX afxNat          AfxCountHids(void);
AUX afxNat          AfxCountScripts(void);

AUX afxManager*     AfxGetControllerManager(void);
AUX afxManager*     AfxGetHidManager(void);
AUX afxManager*     AfxGetWindowManager(void);
AUX afxManager*     AfxGetScriptManager(void);

AUX afxNat          AfxEnumerateHids(afxNat first, afxNat cnt, afxHid hids[]);
AUX afxNat          AfxEnumerateScripts(afxNat first, afxNat cnt, afxScript applications[]);

AUX afxNat          AfxInvokeHids(afxNat first, afxNat cnt, afxBool(*f)(afxHid, void*), void *udd);
AUX afxNat          AfxInvokeScripts(afxNat first, afxNat cnt, afxBool(*f)(afxScript, void*), void *udd);

AUX afxBool         AfxGetUxEventId(afxString const* name, afxUxEventId* id);
AUX afxBool         AfxTriggerUxEventId(afxString const* name, afxObject target);
AUX afxError        AfxRegisterUxEventId(afxString const* name, afxUxEventId* id);
//AUX afxBool         AfxAddUxEventHandler(afxString const* name, afxObject target, fn, afxNat priority);


// Confines the cursor to a rectangular area on the screen. 
// If a subsequent cursor position (set by the SetCursorPos function or the mouse) lies outside the rectangle, 
// the system automatically adjusts the position to keep the cursor inside the rectangular area.

AUX afxBool         AfxConfineCursor(afxWindow wnd);

// Liberates a cursor whether confined on a specified window or on any if no window is specified.

AUX afxBool         AfxLiberateCursor(afxWindow wnd);

// Test wether the cursor is actually on surface of the specified window.

AUX afxBool         AfxCursorIsOnWindowSurface(afxWindow wnd);

#endif//AFX_SHELL_H