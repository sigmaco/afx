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
#define _AUX_SESSION_C
#define _AUX_WINDOW_C
#include "../dev/AuxImplKit.h"

_AUX afxNat AfxGetSid(afxSession ses)
{
    afxError err = AFX_ERR_NONE;
    return ses ? AfxGetObjectId(ses) : 0;
}

_AUX afxClass const* AfxGetWidgetClass(afxSession ses)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxClass const* cls = &ses->widCls;
    AfxAssertClass(cls, afxFcc_WID);
    return cls;
}

_AUX afxClass const* AfxGetWindowClass(afxSession ses)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxClass const* cls = &ses->wndCls;
    AfxAssertClass(cls, afxFcc_WND);
    return cls;
}

_AUX afxClass const* AfxGetScriptClass(afxSession ses)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxClass const* cls = &ses->xssCls;
    AfxAssertClass(cls, afxFcc_XSS);
    return cls;
}

_AUX afxError AfxCloseSession(afxSession ses)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    AfxExhaustChainedClasses(&ses->classes);

    return err;
}

_AUX afxError AfxOpenSession(afxSession ses, afxUri const* host, afxAuthMethod method, afxString const* credential)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    
    return err;
}

_AUX afxError AfxStepSession(afxSession ses, void const* set, void* get)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    afxShell ssh = AfxGetParent(ses);
    //ssh->step(ssh, ses, set, get);

    return err;
}

_AUX afxError _AuxSesDtorCb(afxSession ses)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (ses->stdHid)
        AfxReleaseObjects(1, &ses->stdHid);

    AfxAssert(!ses->idd);
    AfxCleanUpChainedClasses(&ses->classes);

    afxUri location;
    AfxMakeUri(&location, 0, "system", 0);
    AfxDismountStorageUnit('d', &location, afxFileFlag_RWX);

    return err;
}

_AUX afxError _AuxSesCtorCb(afxSession ses, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    afxShell ssh = args[0];
    AfxAssertObjects(1, &ssh, afxFcc_SSH);
    afxSessionConfig const* cfg = ((afxSessionConfig const*)args[1]) + invokeNo;
    
    AfxDeployChain(&ses->classes, ses);

    afxClassConfig clsCfg;

    clsCfg = ssh->widClsCfg;
    AfxRegisterClass(&ses->widCls, NIL, &ses->classes, &clsCfg);

    clsCfg = ssh->wndClsCfg;
    AfxRegisterClass(&ses->wndCls, NIL, &ses->classes, &clsCfg);

    clsCfg = ssh->xssClsCfg;
    AfxRegisterClass(&ses->xssCls, NIL, &ses->classes, &clsCfg);

    ses->idd = NIL;

    ses->seatCnt = 1;

    for (afxNat i = 0; i < ses->seatCnt; i++)
    {
        ses->seats[i].hidNo;
        AfxZero(&ses->seats[i], sizeof(ses->seats[0]));
    }


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
        AfxCleanUpChainedClasses(&ses->classes);
    }
    return err;
}

_AUX afxClassConfig const _AuxSesStdImplementation =
{
    .fcc = afxFcc_SES,
    .name = "Session",
    .desc = "User Session",
    .fixedSiz = sizeof(AFX_OBJECT(afxSession)),
    .ctor = (void*)_AuxSesCtorCb,
    .dtor = (void*)_AuxSesDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireSession(afxNat sshId, afxSessionConfig const* cfg, afxSession* session)
{
    afxError err = AFX_ERR_NONE;

    afxShell ssh;
    AfxGetShell(sshId, &ssh);
    AfxAssertObjects(1, &ssh, afxFcc_SSH);

    afxClass* cls = (afxClass*)AfxGetSessionClass(ssh);
    AfxAssertClass(cls, afxFcc_SES);
    afxSession ses;

    if (!cfg) AfxThrowError();
    else
    {
        if (AfxAcquireObjects(cls, 1, (afxObject*)&ses, (void const*[]) { ssh, cfg, NIL })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &ses, afxFcc_SES);
            AfxAssert(session);
            *session = ses;
        }
    }
    return err;
}
