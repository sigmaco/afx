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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#define _AFX_UX_C
#define _AFX_APPLICATION_C
#define _AFX_APPLICATION_IMPL
#define _AFX_OVERLAY_C
#include "qwadro/mem/afxMmu.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/io/afxStream.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/ux/afxApplication.h"
#include "qwadro/draw/afxDrawSystem.h"

//extern afxChain* _AfxGetSystemClassChain(void);
extern afxClassConfig const _AuxWidMgrCfg;

#if 0
_AUX void AfxGrabWidget(afxApplication app, afxWidget widg, afxV2d const point)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);

    if (!widg)
    {
        if (app->grabbedWidg)
        {
            //if (app->ovly.grabbedItem->drop)
                //app->ovly.grabbedItem->drop(app->ovly.grabbedItem, point);

            app->grabbedWidg = NIL;
        }
    }
    else
    {
        AfxAssertObjects(1, &widg, afxFcc_WID);
        AfxCopyV2d(app->grabPoint, point);

        if (app->grabbedWidg != widg)
        {
            ((app->grabbedWidg = widg));
        }

        //if (widg->grab)
            //widg->grab(widg, point);
    }
}

_AUX void AfxFocusWidget(afxApplication app, afxWidget widg, afxV2d const point)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);

    if (!widg)
    {
        if (app->focusedWidg)
        {
            app->focusedWidg = NIL;
        }
    }
    else
    {
        AfxAssertObjects(1, &widg, afxFcc_WID);

        if (app->focusedWidg != widg)
        {
            ((app->focusedWidg = widg));
        }

        //if (widg->focus)
            //widg->focus(widg, point);
    }
}

_AUX void AfxHoverWidget(afxApplication app, afxWidget widg, afxV2d const point)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);

    if (!widg)
    {
        if (app->hoveredWidg)
        {
            app->hoveredWidg = NIL;
        }
    }
    else
    {
        AfxAssertObjects(1, &widg, afxFcc_WID);

        AfxCopyV2d(app->hoverPoint, point);

        if (app->hoveredWidg != widg)
        {
            ((app->hoveredWidg = widg));
        }

        //if (widg->hover)
            //widg->hover(widg, point);
    }
}
#endif

_AUX afxDrawContext AfxApplicationGetDrawContext(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxDrawContext dctx = app->dctx;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AUX afxDrawInput AfxApplicationGetDrawInput(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxDrawInput din = app->din;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return din;
}

_AUX afxResult AfxRunApplication(afxApplication app, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);

    if (uri)
    {
        afxString domain;
        AfxPickUriObjectToString(uri, &domain);

        if (AfxLoadScript(&domain, uri)) AfxThrowError();
        else
        {
            afxShell env;
            AfxGetShell(&env);

            afxHandle fn[4];
            afxString fns[4];
            AfxMakeString(&fns[0], "init()", 0);
            AfxMakeString(&fns[1], "quit()", 0);
            AfxMakeString(&fns[2], "step(_)", 0);
            AfxMakeString(&fns[3], "draw(_,_)", 0);
            XssHandleAnyCall(env, 4, fns, fn);

            afxHandle var;
            afxString vars;
            AfxMakeString(&vars, "Game", 0);
            XssHandleAnyVar(env, &domain, 0, 1, &vars, &var);
            
            app->xssInitFn = fn[0];
            app->xssQuitFn = fn[1];
            app->xssStepFn = fn[2];
            app->xssDrawFn = fn[3];
            app->xssMainVar = var;
            
            //XssPushHandle(env, 0, var);
            //XssCall(env, fn[0]); // init
            //XssUnhandle(env, 1, &fn[0]);
        }
    }

    AfxGetClock(&app->startClock);
    app->lastClock = app->startClock;
    return 0;
}

_AUX void AfxGetApplicationTime(afxApplication app, afxReal64* ct, afxReal64* dt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxReal64 ct2, dt2;
    afxClock currClock;
    AfxGetClock(&currClock);
    ct2 = AfxGetSecondsElapsed(&app->startClock, &currClock);
    dt2 = AfxGetSecondsElapsed(&app->lastClock, &currClock);
    app->lastClock = currClock;
    *ct = ct2;
    *dt = dt2;
}

////////////////////////////////////////////////////////////////////////////////

_AUX afxNat AfxInvokeApplications(afxNat first, afxNat cnt, afxBool(*f)(afxApplication, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetApplicationClass();
    AfxAssertClass(cls, afxFcc_APP);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AUX afxNat AfxEnumerateApplications(afxNat first, afxNat cnt, afxApplication applications[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(applications);
    afxManager* cls = AfxGetApplicationClass();
    AfxAssertClass(cls, afxFcc_APP);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)applications);
}

_AUX afxNat AfxCountApplications(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetApplicationClass();
    AfxAssertClass(cls, afxFcc_APP);
    return AfxCountObjects(cls);
}
