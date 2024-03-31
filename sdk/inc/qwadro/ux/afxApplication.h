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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_APPLICATION_H
#define AFX_APPLICATION_H

#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/sound/afxSoundInput.h"
#include "qwadro/ux/afxWidget.h"
#include "qwadro/ux/afxOverlay.h"
#include "qwadro/ux/afxMouse.h"
#include "qwadro/core/afxThread.h"
#include "qwadro/env/afxEnvironment.h"
#include "qwadro/core/afxManager.h"

// Add concept of environmental variables, such as $(name) -> value, to be used to form strings, paths, etc.

AFX_DEFINE_STRUCT(afxApplicationConfig)
{
    afxNat                  argc;
    afxChar const**         argv;

    afxMmu                  mmu;
    afxDrawContext          dctx;
    afxSoundContext         sctx;
    afxDrawInput            din;

    afxError                (*proc)(afxApplication app, afxThreadOpcode opcode);

    afxNat                  maxWidCnt;
    afxNat                  maxSimCnt;
};

#ifdef _AFX_CORE_C
#ifdef _AFX_APPLICATION_C
AFX_OBJECT(afxApplication)
{
    afxChain                classes;
    afxManager                ovlyCls;
    afxManager                widCls;

    afxThread               thr;

    // memory allocation service
    afxMmu                  mmu;

    // human input service
    afxMouse                stdMse;

    afxNat                  argc;
    afxChar const           **argv;

    // application
    afxString               path; // 1024
    //afxThread               thr;
    afxError                (*proc)(afxApplication app, afxThreadOpcode opcode);

    afxDrawContext          dctx;
    afxDrawInput            din; // Used for draw widgets.
    afxWidget               hoveredWidg;
    afxV2d                  hoverPoint;
    afxWidget               focusedWidg;
    afxWidget               grabbedWidg;
    afxV2d                  grabPoint;

    afxClock                startClock;
    afxClock                lastClock;

    afxHandle               xssInitFn;
    afxHandle               xssQuitFn;
    afxHandle               xssStepFn;
    afxHandle               xssDrawFn;
    afxHandle               xssMainVar;
};
#endif//_AFX_APPLICATION_C
#endif//_AFX_CORE_C

AUX afxResult               AfxRunApplication(afxApplication app, afxUri const* uri); // Enters the main event loop and waits until exit() is called. Returns the value that was passed to exit() (which is 0 if exit() is called via quit()).

AUX void                    AfxQuitApplication(afxApplication app); // Asks the application to quit. If the quit is not interrupted the application will exit with return code 0 (success).
AUX void                    AfxEndApplication(afxApplication app, afxInt exitCode); // Tells the application to exit with a return code. After this function has been called, the application leaves the main event loop and returns from the call to exec().The exec() function returns returnCode.If the event loop is not running, this function does nothing.

AUX void                    AfxQueryApplicationTimer(afxApplication app, afxReal64* ct, afxReal64* dt);

AUX afxError                AfxAcquireWidgets(afxApplication app, afxNat cnt, afxWidgetConfig config[], afxWidget widgets[]);

AUX afxDrawContext          AfxApplicationGetDrawContext(afxApplication app);
AUX afxDrawInput            AfxApplicationGetDrawInput(afxApplication app);

AUX afxThread               AfxGetApplicationThread(afxApplication app);

AUX afxManager*               AfxGetWidgetClass(afxApplication app);

AUX afxNat                  AfxEnumerateWidgets(afxApplication app, afxNat first, afxNat cnt, afxWidget widgets[]);

AUX void                    AfxFocusWidget(afxApplication app, afxWidget widg, afxV2d const point);
AUX void                    AfxGrabWidget(afxApplication app, afxWidget widg, afxV2d const point);
AUX void                    AfxHoverWidget(afxApplication app, afxWidget widg, afxV2d const point);

////////////////////////////////////////////////////////////////////////////////

AUX afxNat                  AfxCountApplications(void);

AUX afxManager*               AfxGetApplicationClass(void);

AUX afxNat                  AfxEnumerateApplications(afxNat first, afxNat cnt, afxApplication applications[]);

AUX afxNat                  AfxInvokeApplications(afxNat first, afxNat cnt, afxBool(*f)(afxApplication, void*), void *udd);

AUX afxError                AfxAcquireApplications(afxNat cnt, afxApplicationConfig const config[], afxApplication applications[]);

AUX afxApplication          AfxLoadApplication(afxUri const* uri, afxApplicationConfig const* cfg);

#endif//AFX_APPLICATION_H
