/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_APPLICATION_H
#define AFX_APPLICATION_H

#include "afx/draw/afxDrawInput.h"
#include "afx/simul/afxSimulation.h"
#include "afx/mmux/afxWidget.h"
#include "afx/core/io/afxMouse.h"

// Add concept of environmental variables, such as $(name) -> value, to be used to form strings, paths, etc.

AFX_DEFINE_HANDLE(afxApplication);

AFX_DEFINE_STRUCT(afxApplicationSpecification)
{
    afxNat                  argc;
    afxChar const           **argv;

    afxMemory               genrlMem;
    afxDrawContext          dctx;
    afxDrawInput            din;

    afxResult               (*enter)(afxApplication);
    afxResult               (*update)(afxApplication);
    afxResult               (*exit)(afxApplication);

    afxNat                  maxWidCnt;
    afxNat                  maxSimCnt;
};

AFX void                    AfxApplicationExit(afxApplication app, afxResult exitCode); // Tells the application to exit with a return code. After this function has been called, the application leaves the main event loop and returns from the call to exec().The exec() function returns returnCode.If the event loop is not running, this function does nothing.
AFX void                    AfxApplicationQuit(afxApplication app); // Asks the application to quit. If the quit is not interrupted the application will exit with return code 0 (success).
AFX afxResult               AfxApplicationExecute(afxApplication app); // Enters the main event loop and waits until exit() is called. Returns the value that was passed to exit() (which is 0 if exit() is called via quit()).

AFX afxSimulation           AfxApplicationAcquireSimulation(afxApplication app, afxSimulationSpecification const *spec);
AFX afxWidget               AfxApplicationAcquireWidget(afxApplication app, afxString const *name, afxWidget parent, afxUri const *uri, afxResult(*f)(afxWidget, afxUri const*, void *data));

AFX void*                   AfxApplicationGetSystem(afxApplication app);
AFX void*                   AfxApplicationGetDrawContext(afxApplication app);
AFX afxDrawInput            AfxApplicationGetDrawInput(afxApplication app);


AFX afxClass*               AfxApplicationGetSimulationClass(afxApplication app);
AFX afxClass*               AfxApplicationGetWidgetClass(afxApplication app);
AFX afxResult               AfxApplicationForEachSimulation(afxApplication app, void(*f)(afxIterator *iter), void *data);
AFX afxResult               AfxApplicationForEachWidget(afxApplication app, void(*f)(afxIterator *iter), void *data);

AFX void                    AfxApplicationFocusWidget(afxApplication app, afxWidget widg, afxV2d const point);
AFX void                    AfxApplicationGrabWidget(afxApplication app, afxWidget widg, afxV2d const point);
AFX void                    AfxApplicationHoverWidget(afxApplication app, afxWidget widg, afxV2d const point);

AFX afxResult               _AfxApplicationProcess(afxApplication app);
AFX afxResult               AfxApplicationStep(afxApplication app, afxReal dt);

#ifndef _AFX_API_ONLY

AFX_OBJECT(afxApplication)
{
    afxObject               obj;
    afxChain                provisions;

    // memory allocation service
    afxMemory               genrlMem;

    // human input service
    afxMouse                stdMse;

    afxNat                  argc;
    afxChar const           **argv;

    afxArray(afxEvent)      postedEvents;
    afxBool                 excludeInputEvents, waitForNewEvents;

    // application
    afxString1024           path;
    afxBool                 running;
    afxResult               exitCode;

    afxResult               (*enter)(afxApplication);
    afxResult               (*update)(afxApplication);
    afxResult               (*exit)(afxApplication);

    afxSize                 lastFrameTime, lastAnimTime;
    afxReal                 deltaTime;

    afxDrawContext          dctx;
    afxDrawInput            din; // Used for draw widgets.
    afxWidget               hoveredWidg;
    afxV2d                  hoverPoint;
    afxWidget               focusedWidg;
    afxWidget               grabbedWidg;
    afxV2d                  grabPoint;
    afxClass                widClass;
    afxClass                simClass;
};

#endif


#endif//AFX_APPLICATION_H