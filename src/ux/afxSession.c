/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_SESSION_C
#define _AUX_WINDOW_C
#include "impl/auxImplementation.h"

_AUX afxSession gActiveSes = NIL;

_AUX afxUnit AfxGetSid(afxSession ses)
{
    afxError err = AFX_ERR_NONE;
    return ses ? AfxGetObjectId(ses) : 0;
}

_AUX afxBool AfxGetSession(afxSession* session)
{
    afxError err = NIL;
    afxSession ses = gActiveSes;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (session)
        *session = ses;

    return !!ses;
}

_AUX afxTime AfxPollInput(afxFlags flags, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (AfxGetSession(&ses))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
        afxTime first, last, dt;
        AfxGetTime(&first);

        ses->pimpl->pump(ses, 0, timeout);

        dt = (AfxGetTime(&last) - first);
        return dt;
    }
    return 0;
}

_AUX afxBool AfxHasClipboardContent(afxFlags flags)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (AfxGetSession(&ses))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
        return ses->pimpl->hasClipContent(ses, flags);
    }
    return FALSE;
}

_AUX afxUnit AfxGetClipboardContent(afxString* buf)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (AfxGetSession(&ses))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
        return ses->pimpl->getClipContent(ses, buf);
    }
    return 0;
}

_AUX afxError AfxSetClipboardContent(afxString const* text)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (AfxGetSession(&ses))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
        
        if (ses->pimpl->setClipContent(ses, text))
            AfxThrowError();
    }
    return err;
}

_AUX afxBool AfxGetCursorPosition(afxWindow wnd, afxInt position[2])
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (AfxGetSession(&ses))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

        afxBool(*getCurPos)(afxSession, afxWindow, afxInt[]) = ses->pimpl->getCurPos;

        if (getCurPos)
        {
            return getCurPos(ses, wnd, position);
        }
        else
        {
            if (wnd)
            {
                afxRect frame;
                AfxGetWindowRect(wnd, TRUE, &frame);
                position[0] = ses->curPos[0] - frame.x;
                position[1] = ses->curPos[1] - frame.y;
                return position[0] >= 0 && position[1] >= 0 && position[0] < frame.w && position[1] < frame.h;
            }
            else
            {
                position[0] = ses->curPos[0];
                position[1] = ses->curPos[1];
            }
        }
    }
    return FALSE;
}

_AUX afxError AfxImmergeWindow(afxWindow wnd, afxBool fullscreen)
{
    afxError err = AFX_ERR_NONE;
    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (wnd)
    {
        AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

        if (!fullscreen && wnd->fullscreen)
        {
            ses->pimpl->immerge(ses, wnd, FALSE);
            wnd->fullscreen = FALSE;
        }
        else
        {
            ses->pimpl->immerge(ses, wnd, fullscreen);
            wnd->fullscreen = TRUE;
        }
    }
    else
    {
        ses->pimpl->immerge(ses, NIL, FALSE);
    }
    return err;
}

_AUX afxClass const* _AuxSesGetWndClass(afxSession ses)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    afxClass const* cls = &ses->wndCls;
    AFX_ASSERT_CLASS(cls, afxFcc_WND);
    return cls;
}

_AUX afxClass const* _AuxSesGetXssClass(afxSession ses)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    afxClass const* cls = &ses->xssCls;
    AFX_ASSERT_CLASS(cls, afxFcc_XSS);
    return cls;
}

_AUX afxBool AfxGetSessionVideo(afxSession ses, afxDrawSystem* system)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    afxDrawSystem dsys = ses->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(system);
    *system = dsys;
    return !!dsys;
}

_AUX afxBool AfxGetSessionAudio(afxSession ses, afxMixSystem* system, afxSink* sink)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    afxMixSystem msys = ses->msys;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(system);
    *system = msys;
    AFX_ASSERT(sink);
    *sink = ses->aso;
    return !!msys;
}

_AUX afxError AfxCloseSession(void)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (AfxGetSession(&ses))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

        AfxExhaustChainedClasses(&ses->classes);

        gActiveSes = NIL;
    }
    return err;
}

_AUX afxError AfxOpenSession(afxSession ses, afxUri const* host, afxAuthMethod method, afxString const* credential)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    
    afxSession curr;
    if (AfxGetSession(&curr))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &curr);
        AfxCloseSession();
    }

    if (!gActiveSes)
    {
        gActiveSes = ses;
    }
    return err;
}

_AUX afxError AfxStepSession(afxSession ses, void const* set, void* get)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    afxShell ssh = AfxGetProvider(ses);
    //ssh->step(ssh, ses, set, get);

    return err;
}

_AUX afxError _AuxSesDtorCb(afxSession ses)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    AFX_ASSERT(!ses->idd);
    AfxDeregisterChainedClasses(&ses->classes);

    afxUri location;
    AfxMakeUri(&location, 0, "system", 0);
    AfxDismountStorageUnit('d', &location, afxFileFlag_RWX);

    return err;
}

_AUX afxError _AuxSesCtorCb(afxSession ses, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _auxSessionAcquisition const* cfg = AFX_CAST(_auxSessionAcquisition const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    ses->pimpl = &_AUX_SES_IMPL;

    {
        AfxDeployChain(&ses->classes, ses);

        afxClassConfig clsCfg;

        clsCfg = cfg->wndClsCfg ? *cfg->wndClsCfg : _AUX_WND_CLASS_CONFIG;
        AfxMountClass(&ses->wndCls, NIL, &ses->classes, &clsCfg);

        clsCfg = cfg->xssClsCfg ? *cfg->xssClsCfg : (afxClassConfig) { 0 };
        AfxMountClass(&ses->xssCls, NIL, &ses->classes, &clsCfg);
    }

    ses->focusedWnd = NIL;
    ses->curCapturedOn = NIL;

    ses->idd = NIL;

    afxUnit dwmCnt = 1;

    for (afxUnit i = 0; i < dwmCnt; i++)
    {
        afxDesktop* dwm = &ses->dwm;

        dwm->res.w = 1;
        dwm->res.h = 1;
        dwm->res.d = 1;
        dwm->dout = NIL;
        dwm->refreshRate = 1;
    }

    ses->seatCnt = 1;

    for (afxUnit i = 0; i < ses->seatCnt; i++)
    {
        ses->seats[i].hidNo;
        AfxZero(&ses->seats[i], sizeof(ses->seats[0]));
        ses->seats[i].buttonCnt = AFX_MB_TOTAL;
        ses->seats[i].keyCnt = afxKey_TOTAL;
    }

    if (cfg->dsys)
    {
        afxDrawSystem dsys = cfg->dsys;
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
        AfxReacquireObjects(1, &dsys);
        ses->dsys = dsys;
        ses->vduIdx = cfg->vduIdx;
    }
    else
    {
        ses->dsys = NIL;
        ses->vduIdx = AFX_INVALID_INDEX;
    }

    if (!err)
    {
        if (cfg->msys)
        {
            afxMixSystem msys = cfg->msys;
            AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
            AfxReacquireObjects(1, &msys);
            ses->msys = msys;
            ses->soutIdx = cfg->soutIdx;

            afxSinkConfig asoCfg;
            AfxConfigureAudioSink(msys, &asoCfg);
            AfxOpenAudioSink(msys, &asoCfg, &ses->aso);
        }
        else
        {
            ses->msys = NIL;
            ses->soutIdx = AFX_INVALID_INDEX;
#if 0
            afxMixSystem ssys = NIL;
            afxUnit sdevId = cfg->sdevId;

            if (sdevId != AFX_INVALID_INDEX)
            {
                afxMixSystemConfig sccfg;
                AfxConfigureMixSystem(sdevId, &sccfg);

                if (AfxEstablishMixSystem(sdevId, &sccfg, &ssys))
                    AfxThrowError();
            }
            ses->ssys = ssys;
            ses->sdevId = sdevId;
            ses->soutIdx = cfg->soutIdx;
            afxSinkConfig asoCfg;
            AfxConfigureAudioSink(ssys, &asoCfg);
            AfxOpenAudioSink(ssys, &asoCfg, &ses->aso);
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

                //AfxUploadAudioStream(ses->aso, file, 0);
                afxUnit fc = (siz / 2) / 44100 / 2;
                afxUnit fc2 = fc / 2;

                afxUnit j = 0;
                for (afxUnit i = 0; i < fc; i++)
                {
                    AfxPushVoice(ses->aso, 44100, &buf[j]);
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

            if (err)
            {
                AfxDisposeObjects(1, &ses->msys);
            }
        }

        if (err)
        {
            AfxDisposeObjects(1, &ses->dsys);
        }
    }

    if (err)
        AfxDeregisterChainedClasses(&ses->classes);

    return err;
}

_AUX afxClassConfig const _AUX_SES_CLASS_CONFIG =
{
    .fcc = afxFcc_SES,
    .name = "Session",
    .desc = "User Session",
    .fixedSiz = sizeof(AFX_OBJECT(afxSession)),
    .ctor = (void*)_AuxSesCtorCb,
    .dtor = (void*)_AuxSesDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireSession(afxUnit icd, afxSessionConfig const* cfg, afxSession* session)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(session);
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

    _auxSessionAcquisition cfg2 = { 0 };

    cfg2.dsys = cfg->dsys;
    cfg2.seatCnt = cfg->seatCnt;
    cfg2.seats = cfg->seats;
    cfg2.username = cfg->username;
    cfg2.ddevId = cfg->ddevId;
    cfg2.sdevId = cfg->sdevId;
    cfg2.soutIdx = cfg->soutIdx;
    cfg2.msys = cfg->msys;

    afxClass* cls = (afxClass*)_AuxGetSessionClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_SES);

    afxSession ses;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&ses, (void const*[]) { driver, &cfg2, NIL }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    *session = ses;

    return err;
}
