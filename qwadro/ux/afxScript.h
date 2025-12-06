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

#ifndef AUX_SCRIPT_H
#define AUX_SCRIPT_H

#include "qwadro/ux/afxUxDefs.h"

// Add concept of environmental variables, such as $(name) -> value, to be used to form strings, paths, etc.

AFX_DEFINE_HANDLE(afxHandle);

#ifdef _AUX_UX_C
#ifdef _AUX_SCRIPT_C
AFX_OBJECT(afxScript)
{
    xssVm*              vm;
    afxUnit              argc;
    afxChar const**     argv;
    afxString           path;
    afxClock            startClock;
    afxClock            lastClock;

#if !0
    afxHandle           xssInitFn;
    afxHandle           xssQuitFn;
    afxHandle           xssStepFn;
    afxHandle           xssDrawFn;
    afxHandle           xssMainVar;
#endif
};
#endif//_AUX_SCRIPT_C
#endif//_AUX_UX_C

AUX void                AfxGetScriptTime(afxScript xss, afxReal64* ct, afxReal64* dt);

////////////////////////////////////////////////////////////////////////////////

AUX afxResult           AfxRunScript(afxScript xss, afxUri const* uri); // Enters the main event loop and waits until exit() is called. Returns the value that was passed to exit() (which is 0 if exit() is called via quit()).

AUX afxError            AfxLoadScript(afxEnvironment env, afxString const* scope, afxUri const* uri);

AUX afxError            AfxInjectScript(afxEnvironment env, afxString const* scope, afxString const* code);

#endif//AUX_SCRIPT_H
