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

  //////////////////////////////////////////////////////////////////////////////
 // The Unified Shell                                                        //
//////////////////////////////////////////////////////////////////////////////

/// The Unified I/O System, also called Unified Shell, is a full integration between Qwadro systems to improve the user experience and unify it with all the applications

#ifndef AUX_SHELL_H
#define AUX_SHELL_H

#include "qwadro/ux/afxUxDefs.h"
// ux
#include "qwadro/ux/afxHid.h"
#include "qwadro/ux/afxGamepad.h"
#include "qwadro/ux/afxWindow.h"
#include "qwadro/ux/afxScript.h"
#include "qwadro/ux/xss/xss.h"

AFX_DEFINE_HANDLE(afxSession);

AFX_DEFINE_STRUCT(afxSessionConfig)
{
    afxDrawContext  dctx;
    afxSoundContext sctx;
};

AUX afxError        AfxOpenSession(afxNat userId, afxSessionConfig const* cfg, afxSession* session);

AUX afxClass*       AfxGetHidClass(void);
AUX afxClass*       AfxGetWindowClass(void);
AUX afxClass*       AfxGetScriptClass(void);
AUX afxClass*       AfxGetControllerClass(void);

AUX afxNat          AfxEnumerateHids(afxNat first, afxNat cnt, afxHid hids[]);
AUX afxNat          AfxInvokeHids(afxNat first, afxNat cnt, afxBool(*f)(afxHid, void*), void *udd);

AUX afxBool         AfxGetUxEventId(afxString const* name, auxEventId* id);
AUX afxBool         AfxTriggerUxEventId(afxString const* name, afxObject target);
AUX afxError        AfxRegisterUxEventId(afxString const* name, auxEventId* id);
//AUX afxBool         AfxAddUxEventHandler(afxString const* name, afxObject target, fn, afxNat priority);

/// Confines the cursor to a rectangular area on the screen. 
/// If a subsequent cursor position (set by the SetCursorPos function or the mouse) lies outside the rectangle, 
/// the system automatically adjusts the position to keep the cursor inside the rectangular area.

AUX afxBool         AfxConfineCursor(afxWindow wnd);

/// Liberates a cursor whether confined on a specified window or on any if no window is specified.

AUX afxBool         AfxLiberateCursor(afxWindow wnd);

/// Test wether the cursor is actually on surface of the specified window.

AUX afxBool         AfxCursorIsOnSurface(afxWindow wnd);

#endif//AUX_SHELL_H
