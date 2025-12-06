/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_ENVIRONMENT_C
#define _AUX_WINDOW_C
#include "auxIcd.h"
#include "../qwadro_xss/src/xss.h"

// TODO move to afxSystem.
 _AUX afxEnvironment gActiveEnv = NIL;

_AUX afxUnit AfxGetEnvironmentId(afxEnvironment env)
{
    afxError err = { 0 };
    return env ? AfxGetObjectId(env) : 0;
}

_AUX afxBool AfxGetEnvironment(afxEnvironment* environment)
{
    afxError err = { 0 };
    afxEnvironment env = gActiveEnv;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (environment)
        *environment = env;

    return !!env;
}

_AUX afxClass const* _AuxEnvGetWndClass(afxEnvironment env)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    afxClass const* cls = &env->wndCls;
    AFX_ASSERT_CLASS(cls, afxFcc_WND);
    return cls;
}

_AUX afxClass const* _AuxEnvGetFntClass(afxEnvironment env)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    afxClass const* cls = &env->fntCls;
    AFX_ASSERT_CLASS(cls, afxFcc_FNT);
    return cls;
}

_AUX afxClass const* _AuxEnvGetThemClass(afxEnvironment env)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    afxClass const* cls = &env->themCls;
    AFX_ASSERT_CLASS(cls, afxFcc_THEM);
    return cls;
}

_AUX afxClass const* _AuxEnvGetXssClass(afxEnvironment env)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    afxClass const* cls = &env->xssCls;
    AFX_ASSERT_CLASS(cls, afxFcc_XSS);
    return cls;
}

_AUX afxBool AfxGetFocusedWindow(afxUnit seat, afxWindow* window)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return afxError_NOT_READY;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    afxWindow wnd = env->focusedWnd;
    if (window) *window = wnd;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    return !!wnd;
}

_AUX afxError AfxFocusWindow(afxUnit seat, afxWindow wnd, afxFlags flags)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return afxError_NOT_READY;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    return env->ddi->focusCb(env, seat, wnd, flags);
}

_AUX afxError AfxDrawBackgroundEXT(afxDrawContext dctx, afxFlags flags)
{
    afxError err = { 0 };
    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return afxError_NOT_READY;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (!env->ddi->drawBgCb) return afxError_UNSUPPORTED;
    return env->ddi->drawBgCb(env, dctx, flags);
}

_AUX afxBool AfxGetEnvironmentVideo(afxDrawSystem* system)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return afxError_NOT_READY;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    afxDrawSystem dsys = env->dsys;
    if (system) *system = dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return !!dsys;
}

_AUX afxBool AfxGetEnvironmentAudio(afxMixSystem* system, afxSink* sink)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return afxError_NOT_READY;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    afxMixSystem msys = env->msys;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(system || sink);

    if (system)
        *system = msys;

    if (sink)
        *sink = env->aso;

    return !!msys;
}

_AUX afxBool AfxHasClipboardContent(afxUnit seat, afxUnit slot, afxFlags flags)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    return env->ddi->hasClipboardCb(env, seat, slot, flags);
}

_AUX afxUnit AfxGetClipboardContent(afxUnit seat, afxUnit slot, afxFlags flags, afxString* buf)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return 0;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    return env->ddi->getClipboardCb(env, seat, slot, flags, buf);
}

_AUX afxError AfxSetClipboardContent(afxUnit seat, afxUnit slot, afxFlags flags, afxString const* text)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return err;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    if (env->ddi->setClipboardCb(env, seat, slot, flags, text))
        AfxThrowError();

    return err;
}

_AUX afxBool AfxGetCursorPlacement(afxUnit seat, afxRect* rc, afxWindow wnd, afxRect* onFrame, afxRect* onSurface)
{
    afxError err = { 0 };
    AFX_ASSERT(!wnd || (onFrame || onSurface));
    AFX_ASSERT(rc || onFrame || onSurface);
    afxBool rslt = TRUE;

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    if (env->ddi->getCurs)
    {
        return env->ddi->getCurs(env, seat, rc, wnd, onFrame, onSurface);
    }
    else
    {
        afxRect cursRect = env->cursRect;

        if (rc)
            *rc = cursRect;

        if (wnd)
        {
            afxRect frameRc = wnd->frameRc, surfaceRc;
            AfxGetWindowRect(wnd, afxAnchor_TOP | afxAnchor_LEFT, &surfaceRc);

            if (onFrame)
            {
                rslt = AfxIntersectRects(onFrame, &cursRect, &frameRc);
            }

            if (onSurface)
            {
                rslt = AfxIntersectRects(onSurface, &cursRect, &surfaceRc);
            }
        }
    }
    return rslt;
}

_AUX afxTime AfxDoUx(afxFlags flags, afxUnit64 timeout)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetEnvironment(&env)) return 0;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    afxTime first, last, dt;
    AfxGetTime(&first);

    env->ddi->pumpCb(env, 0, timeout);

    dt = (AfxGetTime(&last) - first);

    return dt;
}

_AUX afxError AfxTakeFullscreen(afxWindow wnd, afxBool fullscreen)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (wnd)
    {
        AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

        if (!fullscreen && wnd->fullscreen)
        {
            env->ddi->fseCb(env, wnd, FALSE);
            wnd->fullscreen = FALSE;
        }
        else
        {
            env->ddi->fseCb(env, wnd, fullscreen);
            wnd->fullscreen = TRUE;
        }
    }
    else
    {
        env->ddi->fseCb(env, NIL, FALSE);
    }
    return err;
}

_AUX afxBool AFX_ENV_EVENT_HANDLER(afxEnvironment env, auxEvent *ev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    afxWindow focusedWnd = NIL;
    AfxGetFocusedWindow(ev->seat, &focusedWnd);

    switch (ev->id)
    {
    default:
    {
        auxEvent ev2 = *ev;
        ev2.ev.id = afxEvent_UX;
        ev2.ev.siz = sizeof(ev2);
        
        if (!ev2.wnd)
            ev2.wnd = focusedWnd;

        if (focusedWnd)
            AfxEmitEvent(focusedWnd, (void*)&ev2);
    }
    }
    return TRUE;
}

_AUX afxError _AuxEnvDtorCb(afxEnvironment env)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    AFX_ASSERT(!env->idd);
    AfxDeregisterChainedClasses(&env->classes);

    afxUri location;
    AfxMakeUri(&location, 0, "system", 0);
    AfxDismountStorageUnit('d', &location, afxFileFlag_RWX);

    return err;
}

_AUX afxError _AuxEnvCtorCb(afxEnvironment env, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _auxEnvAcq const* cfg = AFX_CAST(_auxEnvAcq const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    AfxInstallEventHandler(env, (void*)AFX_ENV_EVENT_HANDLER);

    env->ddi = &_AUX_DDI_ENV;
    env->idd = NIL;
    env->udd = cfg->udd;
    env->tag = cfg->tag;

    AfxMakeString512(&env->name, &cfg->name);

    {
        AfxMakeChain(&env->classes, env);

        afxClassConfig wndClsCfg = cfg->wndClsCfg ? *cfg->wndClsCfg : _AUX_WND_CLASS_CONFIG;
        AfxMountClass(&env->wndCls, NIL, &env->classes, &wndClsCfg);

        afxClassConfig fntClsCfg = cfg->fntClsCfg ? *cfg->fntClsCfg : _AUX_FNT_CLASS_CONFIG;
        AfxMountClass(&env->fntCls, NIL, &env->classes, &fntClsCfg);

        afxClassConfig themClsCfg = cfg->themClsCfg ? *cfg->themClsCfg : _AUX_THEM_CLASS_CONFIG;
        AfxMountClass(&env->themCls, NIL, &env->classes, &themClsCfg);

        afxClassConfig xssClsCfg = cfg->xssClsCfg ? *cfg->xssClsCfg : (afxClassConfig) { 0 };
        AfxMountClass(&env->xssCls, NIL, &env->classes, &xssClsCfg);
    }

    afxUnit dwmCnt = 1;

    for (afxUnit i = 0; i < dwmCnt; i++)
    {
        afxDesktop* dwm = &env->dwm;

        dwm->res.w = 1;
        dwm->res.h = 1;
        dwm->res.d = 1;
        dwm->dout = NIL;
        dwm->refreshRate = 1;
    }

    env->focusedWnd = NIL;
    env->cursCapturedOn = NIL;

    env->seatCnt = AFX_MAX(AFX_MIN(AFX_MAX_USERS_PER_ENVIRONMENT, cfg->seatCnt), 1);

    for (afxUnit i = 0; i < env->seatCnt; i++)
    {
        AfxMakeString4096(&env->seats[i].clipb, NIL);

        env->seats[i].hidNo;
        AfxZero(&env->seats[i], sizeof(env->seats[0]));
        env->seats[i].buttonCnt = AFX_MB_TOTAL;
        env->seats[i].keyCnt = afxKey_TOTAL;
    }

    if (cfg->dsys)
    {
        afxDrawSystem dsys = cfg->dsys;
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
        AfxReacquireObjects(1, &dsys);
        env->dsys = dsys;
        env->vduIdx = cfg->vduIdx;
    }
    else
    {
        env->dsys = NIL;
        env->vduIdx = AFX_INVALID_INDEX;
    }

    if (!err)
    {
        if (cfg->msys)
        {
            afxMixSystem msys = cfg->msys;
            AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
            AfxReacquireObjects(1, &msys);
            env->msys = msys;
            env->soutIdx = cfg->soutIdx;

            afxSinkConfig asoCfg = { 0 };
            AfxConfigureAudioSink(msys, &asoCfg);
            AfxOpenAudioSink(msys, &asoCfg, &env->aso);
        }
        else
        {
            env->msys = NIL;
            env->soutIdx = AFX_INVALID_INDEX;
#if 0
            afxMixSystem ssys = NIL;
            afxUnit sdevId = cfg->sdevId;

            if (sdevId != AFX_INVALID_INDEX)
            {
                amxSystemConfig sccfg;
                AmxConfigureMixSystem(sdevId, &sccfg);

                if (AmxEstablishMixSystem(sdevId, &sccfg, &ssys))
                    AfxThrowError();
            }
            env->ssys = ssys;
            env->sdevId = sdevId;
            env->soutIdx = cfg->soutIdx;
            afxSinkConfig asoCfg;
            AfxConfigureAudioSink(ssys, &asoCfg);
            AfxOpenAudioSink(ssys, &asoCfg, &env->aso);
#endif

#if 0
            afxStream file;
            AfxOpenFile(AfxUri("../system/boot"), afxFileFlag_R, &file);
            afxUnit siz = AfxMeasureStream(file);
            afxUnit16* buf;
            AfxAllocate(siz + 441000, 0, AfxHere(), (void**)&buf);
            AfxReadStreamAt(file, 44, siz, 0, buf);

            while (1)
            {
                afxReal data[48000 * 2];
                
                for (afxUnit i = 0; i < 48000; i++)
                    data[i] = AfxRandom2(8000, 490956520);

                //AmxUploadAudioStream(env->aso, file, 0);
                afxUnit fc = (siz / 2) / 44100 / 2;
                afxUnit fc2 = fc / 2;

                afxUnit j = 0;
                for (afxUnit i = 0; i < fc; i++)
                {
                    AfxPushVoice(env->aso, 44100, &buf[j]);
                    j += 44100 * 2;
                //AfxSleep(1);
                }
            }
            AfxDisposeObjects(1, &file);
#endif
        }

        if (!err)
        {
            afxUri location;
            AfxMakeUri(&location, 0, "tmp", 0);

            if (AfxMountStorageUnit('d', &location, afxFileFlag_RWX)) AfxThrowError();
            else
            {
                afxUri uri;
                AfxMakeUri(&uri, 0, "system/qwadro.xss", 0);

                afxString s;
                AfxMakeString(&s, 0, "qwadro", 0);
                //AfxLoadScript(&s, &uri);

                if (err)
                    AfxDismountStorageUnit('d', &location, afxFileFlag_RWX);
            }

            LunaConfiguration vmCfg = { 0 };
            xssInitConfiguration(&vmCfg);
            vmCfg.parent = NIL;// sysVm;
            env->vm = xssCreateVm(&vmCfg);

            if (err)
            {
                AfxDisposeObjects(1, &env->msys);
            }
        }

        if (err)
        {
            AfxDisposeObjects(1, &env->dsys);
        }
    }

    if (err)
        AfxDeregisterChainedClasses(&env->classes);

    return err;
}

_AUX afxClassConfig const _AUX_ENV_CLASS_CONFIG =
{
    .fcc = afxFcc_ENV,
    .name = "Environment",
    .desc = "MMUX Environment",
    .fixedSiz = sizeof(AFX_OBJECT(afxEnvironment)),
    .ctor = (void*)_AuxEnvCtorCb,
    .dtor = (void*)_AuxEnvDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxConfigureEnvironment(afxUnit icd, afxEnvironmentConfig const* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxModule driver;
    if (!_AuxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AUX));

    if (cfg->dsys)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &cfg->dsys);

    }

    if (cfg->msys)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &cfg->msys);

    }

    return err;
}

_AUX afxError AfxAcquireEnvironment(afxUnit icd, afxEnvironmentConfig const* cfg, afxEnvironment* environment)
{
    afxError err = { 0 };
    AFX_ASSERT(environment);
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxModule driver;
    if (!_AuxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AUX));

#if 0
    afxShell ssh;
    AFX_ASSERT(cfg->sshId != AFX_INVALID_INDEX);
    if (!AfxEnumerateShells(icd, cfg->sshId, 1, &ssh))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SSH, 1, &ssh);

    if (AfxCallDevice((afxDevice)ssh, 1, NIL))
    {
        AfxThrowError(); // let the device build its DPUs.
        return err;
    }
#endif

    _auxEnvAcq cfg2 = { 0 };

    cfg2.dsys = cfg->dsys;
    cfg2.seatCnt = cfg->seatCnt;
    cfg2.seats = cfg->seats;
    cfg2.ddevId = cfg->ddevId;
    cfg2.sdevId = cfg->sdevId;
    cfg2.soutIdx = cfg->soutIdx;
    cfg2.msys = cfg->msys;

    cfg2.tag = cfg->tag;
    cfg2.udd = cfg->udd;
    cfg2.name = cfg->name;

    afxClass* cls = (afxClass*)_AuxIcdGetEnvClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_ENV);

    afxEnvironment env;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&env, (void const*[]) { driver, &cfg2, NIL }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    *environment = env;

    return err;
}
