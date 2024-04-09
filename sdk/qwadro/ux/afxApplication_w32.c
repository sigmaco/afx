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
#define _AFX_OVERLAY_C
#define _AFX_APPLICATION_IMPL
#define _AFX_OVERLAY_IMPL
#define _AFX_DRAW_C
#define _AFX_DRAW_OUTPUT_IMPL
#define _AFX_DRAW_OUTPUT_C
#define _AFX_APPLICATION_C
#include "qwadro/mem/afxMmu.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/io/afxStream.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/ux/afxApplication.h"
#include "qwadro/draw/afxDrawSystem.h"

//extern afxChain* _AfxGetSystemClassChain(void);
//extern afxClassConfig const _AuxOvyMgrCfg;

_AUX afxResult AppThreadProc(afxThread thr, afxEvent* ev)
{
    afxError err = AFX_ERR_NONE;
    afxApplication app = AfxGetThreadUdd(thr);
    AfxAssertObjects(1, &app, afxFcc_APP);

    switch (ev->id)
    {
    case afxThreadEvent_RUN:
    {
        //afxEvent ev;
        //AfxEventDeploy(&ev, AFX_EVENT_APP_RUN, &app->thr.obj, NIL);
        //AfxNotifyObject(&app->thr.obj, &ev);
        break;
    }
    case afxThreadEvent_QUIT:
    {
        //afxEvent ev;
        //AfxEventDeploy(&ev, AFX_EVENT_APP_EXIT, &app->thr.obj, NIL);
        //AfxNotifyObject(&app->thr.obj, &ev);

        break;
    }
    default:break;
    };

    AfxAssert(app->procCb);
    app->procCb(app, (afxUxEvent*)ev);

    switch (ev->id)
    {
    case afxThreadEvent_RUN:
    {
        if (app->xssInitFn)
        {
            afxShell env;
            AfxGetShell(&env);
            XssPushHandle(env, 0, app->xssMainVar);
            XssCall(env, app->xssInitFn);
        }
        break;
    }
    case afxThreadEvent_QUIT:
    {
        if (app->xssQuitFn)
        {
            afxShell env;
            AfxGetShell(&env);
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
            AfxGetApplicationTime(app, &ct, &dt);

            afxShell env;
            AfxGetShell(&env);
            XssPushHandle(env, 0, app->xssMainVar);
            XssPushReal(env, 1, dt);
            XssCall(env, app->xssStepFn);
        }
        break;
    }
    }

    return err;
}

_AUX afxError _AuxAppCtor(afxApplication app, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);

    afxApplicationConfig const *config = ((afxApplicationConfig const *)cookie->udd[1]) + cookie->no;

    AfxSetUpChain(&app->mgrChn, (void*)app);

    AfxResetString(&app->path);

    app->argc = 0;
    app->argv = NIL;

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

            //AfxEstablishManager(&app->ovyMgr, NIL, &app->mgrChn, &_AuxOvyMgrCfg);

            app->procCb = config ? config->procCb : NIL;


            AfxGetClock(&app->startClock);
            app->lastClock = app->startClock;

            //if (err)
                //AfxReleaseObject(&app->dinD->obj);
        }

        //if (err)
            //AfxReleaseObject(&app->dctxD->obj);
    }
    return err;
}

_AUX afxError _AuxAppDtor(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, afxFcc_APP);

    AfxCleanUpChainedManagers(&app->mgrChn);

    AfxReleaseObjects(1, (void*[]) { app->din });
    AfxReleaseObjects(1, (void*[]) { app->dctx });

    return err;
}

_AUX afxClassConfig const _AuxAppMgrCfg =
{
    .fcc = afxFcc_APP,
    .name = "Application",
    .desc = "Application",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxApplication)),
    .mmu = NIL,
    .ctor = (void*)_AuxAppCtor,
    .dtor = (void*)_AuxAppDtor
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireApplications(afxNat cnt, afxApplicationConfig const config[], afxApplication applications[])
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetApplicationClass();
    AfxAssertClass(cls, afxFcc_APP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)applications, (void const*[]) { NIL, (void*)config }))
        AfxThrowError();

    AfxAssertObjects(cnt, applications, afxFcc_APP);
    return err;
}

_AUX afxManager* AfxGetApplicationClass(void)
{
    afxError err = AFX_ERR_NONE;
    static afxManager _appCls = { 0 };
    static afxBool appClsMounted = FALSE;

    if (_appCls.fcc != afxFcc_CLS)
    {
        AfxEstablishManager(&_appCls, NIL, /*_AfxGetSystemClassChain()*/NIL, &_AuxAppMgrCfg);
        appClsMounted = TRUE;
    }

    return &_appCls;
}
