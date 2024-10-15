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

#include "qwadro/inc/ux/afxUxDefs.h"
// ux
#include "qwadro/inc/ux/afxHid.h"
#include "qwadro/inc/ux/afxGamepad.h"
#include "qwadro/inc/ux/afxWindow.h"
#include "qwadro/inc/ux/afxWidget.h"
#include "qwadro/inc/ux/afxSession.h"

AFX_DEFINE_STRUCT(afxShellCaps)
{
    afxBool8        windows;
};

AUX afxBool         AfxGetUxEventId(afxString const* name, auxEventId* id);
AUX afxBool         AfxTriggerUxEventId(afxString const* name, afxObject target);
AUX afxError        AfxRegisterUxEventId(afxString const* name, auxEventId* id);
//AUX afxBool         AfxAddUxEventHandler(afxString const* name, afxObject target, fn, afxNat priority);

AUX afxTime         AfxPollInput(void);

AUX afxClass const* AuxGetShellClass(void);

// SHELL DISCOVERY /////////////////////////////////////////////////////////////

AUX afxNat          AfxInvokeShells(afxNat first, afxNat cnt, afxBool(*f)(afxShell, void*), void* udd);
AUX afxNat          AfxEvokeShells(afxBool(*flt)(afxShell, void*), void* fdd, afxNat first, afxNat cnt, afxShell shells[]);
AUX afxNat          AfxEnumerateShells(afxNat first, afxNat cnt, afxShell shells[]);

AUX afxBool         AfxGetShell(afxNat sshId, afxShell* shell);

// SHELL HANDLING //////////////////////////////////////////////////////////////

AUX afxNat          AfxInvokeSessions(afxShell ssh, afxNat first, afxNat cnt, afxBool(*f)(afxSession, void*), void *udd);
AUX afxNat          AfxEvokeSessions(afxShell ssh, afxBool(*flt)(afxSession, void*), void* fdd, afxNat first, afxNat cnt, afxSession sessions[]);
AUX afxNat          AfxEnumerateSessions(afxShell ssh, afxNat first, afxNat cnt, afxSession sessions[]);

AUX afxClass const* AfxGetMouseClass(afxShell ssh);
AUX afxClass const* AfxGetKeyboardClass(afxShell ssh);
AUX afxClass const* AfxGetControllerClass(afxShell ssh);
AUX afxClass const* AfxGetSessionClass(afxShell ssh);

AUX void*           AfxGetShellIdd(afxShell ssh);

AUX afxBool         AfxIsShellPrompt(afxShell ssh);

AUX afxBool         AfxPollEvents();

// SHELL CONTEXTUALIZATION /////////////////////////////////////////////////////



#endif//AUX_SHELL_H
