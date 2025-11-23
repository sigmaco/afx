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

// This software is part of Advanced User Experiences Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // The Unified Shell                                                        //
//////////////////////////////////////////////////////////////////////////////

/// The Unified I/O System, also called Unified Shell, is a full integration between Qwadro systems to improve the user experience and unify it with all the applications

#ifndef AUX_SHELL_H
#define AUX_SHELL_H

#include "qwadro/ux/afxUxDefs.h"
// ux
#include "qwadro/hid/afxHid.h"
#include "qwadro/hid/afxGamepad.h"
#include "qwadro/ux/afxWindow.h"
#include "qwadro/ux/afxWidget.h"
#include "qwadro/ux/afxEnvironment.h"

AFX_DEFINE_STRUCT(afxFileDrop)
{
    afxUnit  x, y;
    afxArray files; // afxChar const*
};

AFX_DEFINE_STRUCT(afxShellFeatures)
{
    afxBool8    windows;
    afxBool8    placements;
    afxBool8    widgets;
    afxBool8    themes;
    afxBool8    fonts;
    afxBool8    deviceFonts;
    afxBool8    clipboard;
    afxBool8    multiClipboards;
};

// SHELL HANDLING //////////////////////////////////////////////////////////////

AUX afxBool         AfxIsShellPrompt(afxShell ssh);

AUX afxUnit         AfxInvokeSessions(afxShell ssh, afxUnit first, afxUnit cnt, afxBool(*f)(afxEnvironment, void*), void *udd);
AUX afxUnit         AfxEvokeSessions(afxShell ssh, afxBool(*flt)(afxEnvironment, void*), void* fdd, afxUnit first, afxUnit cnt, afxEnvironment sessions[]);
AUX afxUnit         AfxEnumerateSessions(afxShell ssh, afxUnit first, afxUnit cnt, afxEnvironment sessions[]);

AUX void*           AfxGetShellIdd(afxShell ssh);

// SHELL DISCOVERY /////////////////////////////////////////////////////////////

AUX afxUnit         AfxInvokeShells(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxShell), afxUnit cnt);
AUX afxUnit         AfxEvokeShells(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxShell), afxUnit cnt, afxShell shells[]);
AUX afxUnit         AfxEnumerateShells(afxUnit icd, afxUnit first, afxUnit cnt, afxShell shells[]);

AUX afxBool         AfxPollEvents();

// IMPLEMENTATION DISCOVERY ////////////////////////////////////////////////////



#endif//AUX_SHELL_H
