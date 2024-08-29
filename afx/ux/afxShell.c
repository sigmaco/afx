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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_WINDOW_C
#include "../dev/AuxImplKit.h"

AFX afxChain* _AfxGetSystemClassChain(void);

_AUX afxClass const* AuxGetShellClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &sys->aux.sshCls;
    AfxAssertClass(cls, afxFcc_SSH);
    return cls;
}

////////////////////////////////////////////////////////////////////////////////
// SHELL HANDLING                                                             //
////////////////////////////////////////////////////////////////////////////////

_AUX afxClass const* AfxGetMouseClass(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssh, afxFcc_SSH);
    afxClass const* cls = &ssh->mseCls;
    AfxAssertClass(cls, afxFcc_MSE);
    return cls;
}

_AUX afxClass const* AfxGetKeyboardClass(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssh, afxFcc_SSH);
    afxClass const* cls = &ssh->kbdCls;
    AfxAssertClass(cls, afxFcc_KBD);
    return cls;
}

_AUX afxClass const* AfxGetControllerClass(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssh, afxFcc_SSH);
    afxClass const* cls = &ssh->padCls;
    AfxAssertClass(cls, afxFcc_CTRL);
    return cls;
}

_AUX afxClass const* AfxGetSessionClass(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssh, afxFcc_SSH);
    afxClass const* cls = &ssh->sesCls;
    AfxAssertClass(cls, afxFcc_SES);
    return cls;
}

_AUX void* AfxGetShellIdd(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssh, afxFcc_SSH);
    return ssh->idd;
}

_AUX afxBool AfxIsShellPrompt(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssh, afxFcc_SSH);
    return ssh->dev.serving;
}

_AUX afxError _AuxSshDtorCb(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssh, afxFcc_SSH);

    if (ssh->stdHid)
        AfxReleaseObjects(1, &ssh->stdHid);

    AfxAssert(!ssh->idd);
    AfxCleanUpChainedClasses(&ssh->dev.classes);

    afxUri location;
    AfxMakeUri(&location, 0, "system", 0);
    AfxDismountStorageUnit('d', &location, afxFileFlag_RWX);

    return err;
}

_AUX afxError _AuxSshCtorCb(afxShell ssh, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssh, afxFcc_SSH);

    afxDriver icd = args[0];
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    afxShellInfo const* info = ((afxShellInfo const *)args[1]) + invokeNo;
    AfxAssert(info);

    if (_AfxDevBaseImplementation.ctor(&ssh->dev, (void*[]) { icd, (void*)&info->dev }, 0)) AfxThrowError();
    else
    {
        //AfxGetSubClass(AfxGetShellClass(ssh))->ctor(ssh, args, invokeNo);

        ssh->idd = NIL;

        afxClassConfig clsCfg;

        clsCfg = info->kbdClsCfg ? *info->kbdClsCfg : _AuxKbdStdImplementation;
        AfxRegisterClass(&ssh->kbdCls, NIL, &ssh->dev.classes, &clsCfg);

        clsCfg = info->mseClsCfg ? *info->mseClsCfg : _AuxMseStdImplementation;
        AfxRegisterClass(&ssh->mseCls, NIL, &ssh->dev.classes, &clsCfg);

        clsCfg = info->padClsCfg ? *info->padClsCfg : _AuxCtrlStdImplementation;
        AfxRegisterClass(&ssh->padCls, NIL, &ssh->dev.classes, &clsCfg);

        clsCfg = info->sesClsCfg ? *info->sesClsCfg : _AuxSesStdImplementation;
        AfxRegisterClass(&ssh->sesCls, NIL, &ssh->dev.classes, &clsCfg);


        clsCfg = info->widClsCfg ? *info->widClsCfg : _AuxWidStdImplementation;
        ssh->widClsCfg = clsCfg;

        clsCfg = info->wndClsCfg ? *info->wndClsCfg : _AuxWndStdImplementation;
        ssh->wndClsCfg = clsCfg;

        clsCfg = info->xssClsCfg ? *info->xssClsCfg : /*_AuxXssStdImplementation*/(afxClassConfig) { 0 };
        ssh->xssClsCfg = clsCfg;


        ssh->focusedWnd = NIL;
        ssh->curCapturedOn = NIL;

        afxNat dwmCnt = 1;

        for (afxNat i = 0; i < dwmCnt; i++)
        {
            afxDesktop* dwm = &ssh->dwm;

            dwm->res[0] = 1;
            dwm->res[1] = 1;
            dwm->res[2] = 1;
            dwm->dout = NIL;
            dwm->refreshRate = 1;
        }


        if (AfxCallDevice(&ssh->dev, afxFcc_USYS, NIL)) AfxThrowError();
        else
        {

            {

                afxUri location;
                AfxMakeUri(&location, 0, "system", 0);

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

                }
            }

            if (err)
            {
                AfxCleanUpChainedClasses(&ssh->dev.classes);
            }
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// SHELL DISCOVERY                                                            //
////////////////////////////////////////////////////////////////////////////////

_AUX afxNat AfxInvokeShells(afxNat first, afxNat cnt, afxBool(*f)(afxShell, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AuxGetShellClass();
    AfxAssertClass(cls, afxFcc_SSH);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AUX afxNat AfxEvokeShells(afxBool(*f)(afxShell, void*), void* udd, afxNat first, afxNat cnt, afxShell shells[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(shells);
    afxClass const* cls = AuxGetShellClass();
    AfxAssertClass(cls, afxFcc_SSH);
    return AfxEvokeClassInstances(cls, (void*)f, udd, first, cnt, (afxObject*)shells);
}

_AUX afxNat AfxEnumerateShells(afxNat first, afxNat cnt, afxShell shells[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(shells);
    afxClass const* cls = AuxGetShellClass();
    AfxAssertClass(cls, afxFcc_SSH);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)shells);
}

_AUX afxBool AfxGetShell(afxNat sshId, afxShell* shell)
{
    afxError err = AFX_ERR_NONE;
    afxShell ssh = NIL;
    while (AfxEnumerateShells(sshId, 1, &ssh))
    {
        AfxAssertObjects(1, &ssh, afxFcc_SSH);
        AfxAssert(shell);
        *shell = ssh;
        break;
    }
    return !!ssh;
}

_AUX afxError _AuxRegisterShells(afxDriver icd, afxNat cnt, afxShellInfo const infos[], afxShell shells[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    AfxAssert(shells);

    afxClass* cls = (afxClass*)AuxGetShellClass();
    AfxAssertClass(cls, afxFcc_SSH);

    AfxEnterSlockExclusive(&cls->poolLock);

    if (cls->pool.totalUsedCnt >= cls->maxInstCnt) AfxThrowError();
    else
    {
        if (_AfxAllocateClassInstances(cls, cnt, (afxObject*)shells)) AfxThrowError();
        else
        {
            if (_AfxConstructClassInstances(cls, cnt, (afxObject*)shells, (void*[]) { icd, (void*)infos }))
            {
                AfxThrowError();
            }

            if (!err)
            {
                AfxAssertObjects(cnt, shells, afxFcc_SSH);
            }
            else
            {
                _AfxDeallocateClassInstances(cls, cnt, (afxObject*)shells);
            }
        }
    }

    AfxExitSlockExclusive(&cls->poolLock);

    return err;
}

_AUX afxBool SshPump(afxShell ssh, void* udd)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ssh, afxFcc_SSH);

    ssh->pumpCb(ssh);

    return TRUE;
}

_AUX afxTime AfxPollInput(void)
{
    afxError err = AFX_ERR_NONE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    afxTime first, last, dt;
    AfxGetTime(&first);

    if (!sys->aux.disabled)
    {
        AfxInvokeClassInstances(&sys->aux.sshCls, 0, 0, (void*)SshPump, NIL);
    }
    dt = (AfxGetTime(&last) - first);
    return dt;
}

_AUX afxError afxScmHook(afxModule mdle, afxManifest const* ini)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    AfxRegisterClass(&sys->aux.hidCls, (afxClass*)AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_AuxHidStdImplementation);

    afxClassConfig const _AuxSshClsCfg =
    {
        .fcc = afxFcc_SSH,
        .name = "Shell",
        .desc = "Shell Driver Interface",
        .fixedSiz = sizeof(AFX_OBJECT(afxShell)),
        .ctor = (void*)_AuxSshCtorCb,
        .dtor = (void*)_AuxSshDtorCb
    };
    AfxRegisterClass(&sys->aux.sshCls, (afxClass*)AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_AuxSshClsCfg); // require base*

    if (!err)
    {
        //sys->aux.ready = TRUE;
    }

    return err;
}
