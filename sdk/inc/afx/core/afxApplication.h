/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_APPLICATION_H
#define AFX_APPLICATION_H

#include "afx/draw/afxDrawInput.h"
#include "afx/sim/afxSimulation.h"
#include "afx/mmux/afxWidget.h"
#include "afx/core/io/afxMouse.h"
#include "afx/core/async/afxThread.h"

// Add concept of environmental variables, such as $(name) -> value, to be used to form strings, paths, etc.

AFX_DEFINE_STRUCT(afxApplicationConfig)
{
    afxNat                  argc;
    afxChar const           **argv;

    afxContext               genrlMem;
    afxDrawContext          dctx;
    afxDrawInput            din;

    afxError                (*proc)(afxThread thr, afxApplication app, afxThreadOpcode opcode);

    afxNat                  maxWidCnt;
    afxNat                  maxSimCnt;
};

#ifdef _AFX_APPLICATION_C
AFX_OBJECT(afxApplication)
{
    afxChain                classes;
    afxClass                widgets;
    afxClass                simulations;

    afxThread               thr;

    // memory allocation service
    afxContext               genrlMem;

    // human input service
    afxMouse                stdMse;

    afxNat                  argc;
    afxChar const           **argv;

    // application
    afxString               path; // 1024
    //afxThread               thr;
    afxError                (*proc)(afxThread thr, afxApplication app, afxThreadOpcode opcode);

    afxDrawContext          dctx;
    afxDrawInput            din; // Used for draw widgets.
    afxWidget               hoveredWidg;
    afxV2d                  hoverPoint;
    afxWidget               focusedWidg;
    afxWidget               grabbedWidg;
    afxV2d                  grabPoint;
};
#endif//_AFX_APPLICATION_C

AFX afxNat                  AfxEnumerateApplications(afxNat first, afxNat cnt, afxApplication app[]);

AFX afxError                AfxAcquireApplications(afxNat cnt, afxApplication app[], afxApplicationConfig const config[]);

AFX void                    AfxEndApplication(afxApplication app, afxInt exitCode); // Tells the application to exit with a return code. After this function has been called, the application leaves the main event loop and returns from the call to exec().The exec() function returns returnCode.If the event loop is not running, this function does nothing.
AFX void                    AfxQuitApplication(afxApplication app); // Asks the application to quit. If the quit is not interrupted the application will exit with return code 0 (success).
AFX afxResult               AfxRunApplication(afxApplication app); // Enters the main event loop and waits until exit() is called. Returns the value that was passed to exit() (which is 0 if exit() is called via quit()).

AFX afxError                AfxAcquireSimulations(afxApplication app, afxNat cnt, afxSimulation sim[], afxSimulationConfig const config[]);
AFX afxError                AfxAcquireWidgets(afxApplication app, afxNat cnt, afxWidget wid[], afxWidgetConfig config[]);

AFX afxDrawContext            AfxApplicationGetDrawContext(afxApplication app);
AFX afxDrawInput            AfxApplicationGetDrawInput(afxApplication app);

AFX afxThread               AfxGetApplicationThread(afxApplication app);

AFX afxClass*               AfxGetWidgetClass(afxApplication app);
AFX afxClass*               AfxGetSimulationClass(afxApplication app);

AFX afxNat                  AfxEnumerateWidgets(afxApplication app, afxNat first, afxNat cnt, afxWidget wid[]);
AFX afxNat                  AfxEnumerateSimulations(afxApplication app, afxNat first, afxNat cnt, afxSimulation sim[]);

AFX void                    AfxApplicationFocusWidget(afxApplication app, afxWidget widg, afxV2d const point);
AFX void                    AfxApplicationGrabWidget(afxApplication app, afxWidget widg, afxV2d const point);
AFX void                    AfxApplicationHoverWidget(afxApplication app, afxWidget widg, afxV2d const point);

//AFX afxBool                 _AfxGetAppD(afxApplication app, struct _afxAppD **appD, struct _afxSysD* sysD);

#endif//AFX_APPLICATION_H