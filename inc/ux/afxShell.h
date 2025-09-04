/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E
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

AFX_DEFINE_STRUCT(afxFileDrop)
{
    afxUnit  x, y;
    afxArray files; // afxChar const*
};

AFX_DEFINE_STRUCT(afxShellFeatures)
{
    afxBool8    windows;
    afxBool8    widgets;
};

// SHELL HANDLING //////////////////////////////////////////////////////////////

AUX afxBool         AfxIsShellPrompt(afxShell ssh);

AUX afxUnit         AfxInvokeSessions(afxShell ssh, afxUnit first, afxUnit cnt, afxBool(*f)(afxSession, void*), void *udd);
AUX afxUnit         AfxEvokeSessions(afxShell ssh, afxBool(*flt)(afxSession, void*), void* fdd, afxUnit first, afxUnit cnt, afxSession sessions[]);
AUX afxUnit         AfxEnumerateSessions(afxShell ssh, afxUnit first, afxUnit cnt, afxSession sessions[]);

AUX void*           AfxGetShellIdd(afxShell ssh);

// SHELL DISCOVERY /////////////////////////////////////////////////////////////

AUX afxUnit         AfxInvokeShells(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxShell), afxUnit cnt);
AUX afxUnit         AfxEvokeShells(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxShell), afxUnit cnt, afxShell shells[]);
AUX afxUnit         AfxEnumerateShells(afxUnit icd, afxUnit first, afxUnit cnt, afxShell shells[]);

AUX afxBool         AfxPollEvents();

// IMPLEMENTATION DISCOVERY ////////////////////////////////////////////////////



#endif//AUX_SHELL_H
