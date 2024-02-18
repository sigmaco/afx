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

#if (defined(_WIN32) || defined(_WIN64))
#   include <Windows.h>
#   include <Shlwapi.h>
#   include <combaseapi.h>
#else
#   include <unistd.h>
#endif

#define _AFX_CORE_C
#define _AFX_APPLICATION_C
#include "qwadro/mem/afxMmu.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/io/afxStream.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/ux/afxApplication.h"
#include "qwadro/draw/afxDrawSystem.h"

extern afxClassConfig const _AfxWidClsConfig;

_AFX void AfxApplicationGrabWidget(afxApplication app, afxWidget widg, afxV2d const point)
{
    AfxEntry("app=%p,widg=%p,point(%f,%f))", app, widg, point ? point[0] : NIL, point ? point[1] : NIL);
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

_AFX void AfxApplicationFocusWidget(afxApplication app, afxWidget widg, afxV2d const point)
{
    AfxEntry("app=%p,widg=%p,point(%f,%f))", app, widg, point ? point[0] : NIL, point ? point[1] : NIL);
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

_AFX void AfxApplicationHoverWidget(afxApplication app, afxWidget widg, afxV2d const point)
{
    AfxEntry("app=%p,widg=%p,point(%f,%f))", app, widg, point ? point[0] : NIL, point ? point[1] : NIL);
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

_AFX afxClass* AfxGetWidgetClass(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxClass *class = &app->widgets;
    AfxAssertClass(class, afxFcc_WID);
    return class;
}

_AFX afxNat AfxEnumerateWidgets(afxApplication app, afxNat first, afxNat cnt, afxWidget wid[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxClass *cls = AfxGetWidgetClass(app);
    AfxAssertClass(cls, afxFcc_DCTX);
    //AfxAssertRange(AfxCountInstances(cls), first, cnt);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)wid);
}

_AFX afxDrawInput AfxApplicationGetDrawInput(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxDrawInput din = app->din;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return din;
}

_AFX void AfxEndApplication(afxApplication app, afxInt exitCode)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);
    (void)exitCode;
    AfxAssertObjects(1, &app, afxFcc_APP);
    AfxRequestThreadInterruption(app->thr);
    //AfxExitExecution(app->thr, exitCode);
}

_AFX void AfxQuitApplication(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);
    AfxAssertObjects(1, &app, afxFcc_APP);
    AfxEndApplication(app, 0);
}

_AFX afxResult AfxRunApplication(afxApplication app)
{
    AfxEntry("app=%p", app);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxThread thr = app->thr;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    AfxRunThread(thr);
    return 0;
}

_AFX afxDrawContext AfxApplicationGetDrawContext(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxDrawContext dctx = app->dctx;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AFX afxThread AfxGetApplicationThread(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);
    afxThread thr = app->thr;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return thr;
}

_AFX afxError _AfxAppDtor(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);

    AfxAssertObjects(1, &app, afxFcc_APP);

    AfxQuitApplication(app);

    _AfxUninstallChainedClasses(&app->classes);

    AfxReleaseObjects(1, (void*[]) { app->thr });
    AfxReleaseObjects(1, (void*[]) { app->din });
    AfxReleaseObjects(1, (void*[]) { app->dctx });
    AfxReleaseObjects(1, (void*[]) { app->stdMse });
    AfxReleaseObjects(1, (void*[]) { app->mmu });
    
    return err;
}

_AFX afxError _AfxThrProcAppCb(afxThread thr, void *udd, afxThreadOpcode opcode)
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

    return err;
}

_AFX afxError _AfxAppCtor(afxApplication app, afxCookie const* cookie)
{
    AfxEntry("app=%p", app);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);

    afxApplicationConfig const *config = ((afxApplicationConfig const *)cookie->udd[0]) + cookie->no;

    afxThreadConfig thrConfig = { 0 };
    thrConfig.proc = _AfxThrProcAppCb;
    thrConfig.udd = (void*)app;

    if (AfxAcquireThreads(1, AfxHint(), &thrConfig, &app->thr)) AfxThrowError();
    else
    {
        AfxTakeChain(&app->classes, (void*)app);

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

            if (AfxAcquireMouses(1, msePorts, &app->stdMse)) AfxThrowError();
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

                        if (AfxOpenDrawInputs(0, 1, &dinConfig, &app->din))
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

                        AfxMountClass(&app->widgets, NIL, &app->classes, &_AfxWidClsConfig);

                        AfxZeroV2d(app->grabPoint);
                        AfxZeroV2d(app->hoverPoint);

                        app->proc = config ? config->proc : NIL;

                        app->grabbedWidg = NIL;
                        app->hoveredWidg = NIL;
                        app->focusedWidg = NIL;

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

_AFX afxClassConfig const _AfxAppClsConfig =
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

_AFX afxError AfxAcquireApplications(afxNat cnt, afxApplicationConfig const config[], afxApplication applications[])
{
    AfxEntry("cnt=%u,app=%p,config=%p", cnt, applications, config);
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetApplicationClass();
    AfxAssertClass(cls, afxFcc_APP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)applications, (void const*[]) { (void*)config }))
        AfxThrowError();

    AfxAssertObjects(cnt, applications, afxFcc_APP);

    return err;
}

_AFX afxNat AfxInvokeApplications(afxNat first, afxNat cnt, afxBool(*f)(afxApplication, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetApplicationClass();
    AfxAssertClass(cls, afxFcc_APP);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateApplications(afxNat first, afxNat cnt, afxApplication applications[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(applications);
    afxClass* cls = AfxGetApplicationClass();
    AfxAssertClass(cls, afxFcc_APP);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)applications);
}

_AFX afxNat AfxCountApplications(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetApplicationClass();
    AfxAssertClass(cls, afxFcc_APP);
    return AfxCountInstances(cls);
}
