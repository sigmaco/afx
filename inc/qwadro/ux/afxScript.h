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

#ifndef AFX_APPLICATION_H
#define AFX_APPLICATION_H

#include "qwadro/ux/afxWidget.h"
#include "qwadro/ux/afxWindow.h"
#include "qwadro/ux/afxHid.h"

// Add concept of environmental variables, such as $(name) -> value, to be used to form strings, paths, etc.

typedef afxResult(*afxApplicationProc)(afxScript app, afxUxEvent* ev);

#ifdef _AUX_UX_C
#ifdef _AFX_SCRIPT_C
AFX_OBJECT(afxScript)
{
    xssVm*              vm;
    afxNat              argc;
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
#endif//_AFX_SCRIPT_C
#endif//_AUX_UX_C

AUX void                AfxGetScriptTime(afxScript xss, afxReal64* ct, afxReal64* dt);

////////////////////////////////////////////////////////////////////////////////

AUX afxResult           AfxRunScript(afxScript xss, afxUri const* uri); // Enters the main event loop and waits until exit() is called. Returns the value that was passed to exit() (which is 0 if exit() is called via quit()).

AUX afxError            AfxLoadScript(afxString const* scope, afxUri const* uri);

AUX afxError            AfxInjectScript(afxString const* scope, afxString const* code);

#endif//AFX_APPLICATION_H
