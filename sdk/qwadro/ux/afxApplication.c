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

#define _AFX_CORE_C
#define _AFX_APPLICATION_C
#define _AFX_SYSTEM_C
#include "qwadro/mem/afxMmu.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/io/afxStream.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/ux/afxApplication.h"
#include "qwadro/draw/afxDrawSystem.h"

//extern afxChain* _AfxGetSystemClassChain(void);
extern afxClassConfig const _AfxWidClsConfig;

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

_AUX afxManager* AfxGetWidgetClass(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxManager *cls = &app->widCls;
    AfxAssertClass(cls, afxFcc_WID);
    return cls;
}

_AUX afxNat AfxEnumerateWidgets(afxApplication app, afxNat first, afxNat cnt, afxWidget widgets[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxManager *cls = AfxGetWidgetClass(app);
    AfxAssertClass(cls, afxFcc_DCTX);
    //AfxAssertRange(AfxCountObjects(cls), first, cnt);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)widgets);
}

_AUX afxDrawInput AfxApplicationGetDrawInput(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxDrawInput din = app->din;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return din;
}

_AUX void AfxEndApplication(afxApplication app, afxInt exitCode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    (void)exitCode;
    AfxRequestThreadInterruption(app->thr);
    //AfxExitThread(app->thr, exitCode);
}

_AUX void AfxQuitApplication(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    AfxEndApplication(app, 0);
}

_AUX afxResult AfxRunApplication(afxApplication app, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxThread thr = app->thr;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    if (uri)
    {
        afxString domain;
        AfxPickUriObjectToString(uri, &domain);

        if (AfxLoadScript(&domain, uri)) AfxThrowError();
        else
        {
            afxEnvironment env;
            AfxGetEnvironment(&env);

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
    AfxRunThread(thr);
    return 0;
}

_AUX afxDrawContext AfxApplicationGetDrawContext(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxDrawContext dctx = app->dctx;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AUX afxThread AfxGetApplicationThread(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxThread thr = app->thr;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return thr;
}

_AUX afxError _AfxAppDtor(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);

    AfxQuitApplication(app);

    _AfxUninstallChainedClasses(&app->classes);

    AfxReleaseObjects(1, (void*[]) { app->thr });
    AfxReleaseObjects(1, (void*[]) { app->din });
    AfxReleaseObjects(1, (void*[]) { app->dctx });
    AfxReleaseObjects(1, (void*[]) { app->stdMse });
    //AfxReleaseObjects(1, (void*[]) { app->mmu });
    
    return err;
}

_AUX void AfxQueryApplicationTimer(afxApplication app, afxReal64* ct, afxReal64* dt)
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

_AUX afxError _AfxThrProcAppCb(afxThread thr, void *udd, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;
    afxApplication app = (afxApplication)udd; //AfxRebase(thr, afxApplication, thr);
    AfxAssert(app == (afxApplication)AfxGetThreadUdd(thr));
    AfxAssertObjects(1, &app, afxFcc_APP);
    AfxAssert(app == AfxGetThreadUdd(app->thr));

    switch (opcode)
    {
    case AFX_THR_OPCODE_RUN:
    {
        //afxEvent ev;
        //AfxEventDeploy(&ev, AFX_EVENT_APP_RUN, &app->thr.obj, NIL);
        //AfxObjectEmitEvent(&app->thr.obj, &ev);
        break;
    }
    case AFX_THR_OPCODE_QUIT:
    {
        //afxEvent ev;
        //AfxEventDeploy(&ev, AFX_EVENT_APP_EXIT, &app->thr.obj, NIL);
        //AfxObjectEmitEvent(&app->thr.obj, &ev);

        break;
    }
    default:break;
    };

    if (app->proc)
        app->proc(app, opcode);

    switch (opcode)
    {
    case AFX_THR_OPCODE_RUN:
    {
        if (app->xssInitFn)
        {
            afxEnvironment env;
            AfxGetEnvironment(&env);
            XssPushHandle(env, 0, app->xssMainVar);
            XssCall(env, app->xssInitFn);
        }
        break;
    }
    case AFX_THR_OPCODE_QUIT:
    {
        if (app->xssQuitFn)
        {
            afxEnvironment env;
            AfxGetEnvironment(&env);
            XssPushHandle(env, 0, app->xssMainVar);
            XssCall(env, app->xssQuitFn);
        }
        break;
    }
    default:
    {
        if (app->xssStepFn)
        {
            afxReal64 ct, dt;
            AfxQueryApplicationTimer(app, &ct, &dt);

            afxEnvironment env;
            AfxGetEnvironment(&env);
            XssPushHandle(env, 0, app->xssMainVar);
            XssPushReal(env, 1, dt);
            XssCall(env, app->xssStepFn);
        }
        break;
    }
    };

    return err;
}

_AUX afxError _AfxAppCtor(afxApplication app, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);

    afxApplicationConfig const *config = ((afxApplicationConfig const *)cookie->udd[0]) + cookie->no;

    afxThreadConfig thrConfig = { 0 };
    thrConfig.proc = _AfxThrProcAppCb;
    thrConfig.udd = (void*)app;

    if (AfxAcquireThreads(&thrConfig, AfxHint(), 1, &app->thr)) AfxThrowError();
    else
    {
        AfxSetUpChain(&app->classes, (void*)app);

        AfxResetString(&app->path);

        app->argc = 0;
        app->argv = NIL;

        if (!(app->mmu = config->mmu))
        {
            app->mmu = AfxGetSystemContext();
        }

        AfxAssertObjects(1, &app->mmu, afxFcc_MMU);

        //if (!(AfxObjectReacquire(&app->genrlMem->obj, &app->thr.obj, NIL, NIL, NIL))) AfxThrowError();
        //else
        {
            afxNat msePorts[] = { 0 };

            if (AfxAcquireMouse(0, &app->stdMse)) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &app->stdMse, afxFcc_MSE);
                AfxAssertObjects(1, &config->dctx, afxFcc_DCTX);

                app->dctx = config->dctx;
                //if (!(AfxObjectReacquire(&((app->dctx = spec->dctx)->obj), &app->thr.obj, NIL, NIL, NIL))) AfxThrowError();
                //else
                {
                    if (!(app->din = config->din))
                    {
                        afxDrawInputConfig dinConfig = { 0 };
                        dinConfig.cmdPoolMemStock = 4096;
                        dinConfig.estimatedSubmissionCnt = 2;
                        dinConfig.proc = NIL;

                        if (AfxAcquireDrawInput(0, &dinConfig, &app->din))
                            AfxThrowError();
                    }
                    else
                    {
                        //if (!(AfxObjectReacquire(&app->dinD->obj, &app->thr.obj, NIL, NIL, NIL)))
                            //  AfxThrowError();
                    }

                    if (!err)
                    {
                        AfxAssertObjects(1, &app->din, afxFcc_DIN);

                        AfxSetUpManager(&app->widCls, NIL, &app->classes, &_AfxWidClsConfig);

                        AfxZeroV2d(app->grabPoint);
                        AfxZeroV2d(app->hoverPoint);

                        app->proc = config ? config->proc : NIL;

                        app->grabbedWidg = NIL;
                        app->hoveredWidg = NIL;
                        app->focusedWidg = NIL;


                        AfxGetClock(&app->startClock);
                        app->lastClock = app->startClock;

                        //if (err)
                            //AfxReleaseObject(&app->dinD->obj);
                    }

                    //if (err)
                        //AfxReleaseObject(&app->dctxD->obj);
                }

                if (err)
                    AfxReleaseObjects(1, (void*[]) { app->stdMse });
            }

            //if (err)
                //  AfxReleaseObject(&app->genrlMem->obj);
        }
    }
    return err;
}

_AUX afxClassConfig const _AfxAppClsConfig =
{
    .fcc = afxFcc_APP,
    .name = "Application",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxApplication)),
    .mmu = NIL,
    .ctor = (void*)_AfxAppCtor,
    .dtor = (void*)_AfxAppDtor
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireApplications(afxNat cnt, afxApplicationConfig const config[], afxApplication applications[])
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetApplicationClass();
    AfxAssertClass(cls, afxFcc_APP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)applications, (void const*[]) { (void*)config }))
        AfxThrowError();

    AfxAssertObjects(cnt, applications, afxFcc_APP);
    return err;
}

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

_AUX afxManager* AfxGetApplicationClass(void)
{
    afxError err = AFX_ERR_NONE;
    static afxManager _appCls = { 0 };
    static afxBool appClsMounted = FALSE;

    if (_appCls.fcc != afxFcc_CLS)
    {
        AfxSetUpManager(&_appCls, NIL, /*_AfxGetSystemClassChain()*/NIL, &_AfxAppClsConfig);
        appClsMounted = TRUE;
    }

    return &_appCls;
}
