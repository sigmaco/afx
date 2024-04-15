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

#include "qwadro/draw/dev/afxDrawInput.h"
#include "qwadro/sound/afxSoundInput.h"
#include "qwadro/ux/afxWidget.h"
#include "qwadro/ux/afxOverlay.h"
#include "qwadro/ux/afxMouse.h"
#include "qwadro/core/afxThread.h"
#include "qwadro/env/afxShell.h"
#include "qwadro/core/afxManager.h"

// Add concept of environmental variables, such as $(name) -> value, to be used to form strings, paths, etc.

typedef enum afxUxEventId
{
    afxUxEvent_RUN = 1,
    afxUxEvent_QUIT
}afxUxEventId;

AFX_DEFINE_STRUCT(afxUxEvent)
{
    afxUxEventId    id;
    afxBool         posted, accepted;
    void*           udd[0];
};

typedef afxResult(*afxApplicationProc)(afxApplication app, afxUxEvent* ev);

AFX_DEFINE_STRUCT(afxApplicationConfig)
{
    afxNat              argc;
    afxChar const**     argv;

    afxMmu              mmu;
    afxDrawContext      dctx;
    afxSoundContext     sctx;
    afxDrawInput        din;

    afxApplicationProc  procCb;

    afxNat              maxWidCnt;
    afxNat              maxSimCnt;
};

#ifdef _AFX_UX_C
#ifdef _AFX_APPLICATION_C
AFX_OBJECT(afxApplication)
{
    afxChain            mgrChn;
    afxManager          ovyMgr;

    // memory allocation service
    afxMmu              mmu;

    afxNat              argc;
    afxChar const**     argv;

    // application
    afxString           path; // 1024
    //afxThread         thr;
    afxApplicationProc  procCb;

    afxDrawContext      dctx;
    afxDrawInput        din; // Used for draw widgets.
    
    afxClock            startClock;
    afxClock            lastClock;

    afxHandle           xssInitFn;
    afxHandle           xssQuitFn;
    afxHandle           xssStepFn;
    afxHandle           xssDrawFn;
    afxHandle           xssMainVar;

#ifdef _AFX_APPLICATION_IMPL
#ifdef AFX_PLATFORM_WIN
#endif//AFX_PLATFORM_WIN
#endif//_AFX_APPLICATION_C
};
#endif//_AFX_APPLICATION_C
#endif//_AFX_UX_C

AUX afxResult           AfxRunApplication(afxApplication app, afxUri const* uri); // Enters the main event loop and waits until exit() is called. Returns the value that was passed to exit() (which is 0 if exit() is called via quit()).

AUX void                AfxGetApplicationTime(afxApplication app, afxReal64* ct, afxReal64* dt);

AUX afxDrawContext      AfxApplicationGetDrawContext(afxApplication app);
AUX afxDrawInput        AfxApplicationGetDrawInput(afxApplication app);

AUX void                AfxFocusWidget(afxApplication app, afxWidget widg, afxV2d const point);
AUX void                AfxGrabWidget(afxApplication app, afxWidget widg, afxV2d const point);
AUX void                AfxHoverWidget(afxApplication app, afxWidget widg, afxV2d const point);

AUX afxNat              AfxEnumerateWidgets(afxOverlay ovy, afxNat first, afxNat cnt, afxWidget widgets[]);

////////////////////////////////////////////////////////////////////////////////

AUX afxNat              AfxCountApplications(void);

AUX afxManager*         AfxGetApplicationClass(void);

AUX afxNat              AfxEnumerateApplications(afxNat first, afxNat cnt, afxApplication applications[]);

AUX afxNat              AfxInvokeApplications(afxNat first, afxNat cnt, afxBool(*f)(afxApplication, void*), void *udd);

AUX afxError            AfxAcquireApplications(afxNat cnt, afxApplicationConfig const config[], afxApplication applications[]);

AUX afxApplication      AfxLoadApplication(afxUri const* uri, afxApplicationConfig const* cfg);

#endif//AFX_APPLICATION_H
