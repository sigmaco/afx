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
#define _AUX_WINDOW_C
#include "impl/auxImplementation.h"

AFX afxChain* _AfxGetSystemClassChain(void);

_AUX afxString const sigmaUxSignature = AFX_STRING(
    "      ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::      \n"
    "     :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:     \n"
    "     +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+     \n"
    "     +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+     \n"
    "     +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+     \n"
    "     #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#     \n"
    "      ###### ###  ###   ###   ###     ### #########  ###    ###  ########      \n"
    "                                                                               \n"
    "     Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E     \n"
    "                                                                               \n"
    "                               Public Test Build                               \n"
    "                           (c) 2017 SIGMA FEDERATION                           \n"
    "                                www.sigmaco.org                                \n"
    "                                                                               \n"
);

////////////////////////////////////////////////////////////////////////////////
// SHELL HANDLING                                                             //
////////////////////////////////////////////////////////////////////////////////

_AUX afxClass const* AfxGetMouseClass(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSH, 1, &ssh);
    afxClass const* cls = &ssh->mseCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSE);
    return cls;
}

_AUX afxClass const* AfxGetKeyboardClass(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSH, 1, &ssh);
    afxClass const* cls = &ssh->kbdCls;
    AFX_ASSERT_CLASS(cls, afxFcc_KBD);
    return cls;
}

_AUX afxClass const* AfxGetControllerClass(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSH, 1, &ssh);
    afxClass const* cls = &ssh->padCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CTRL);
    return cls;
}

_AUX void* AfxGetShellIdd(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSH, 1, &ssh);
    return NIL;
}

_AUX afxBool AfxIsShellPrompt(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSH, 1, &ssh);
    return ssh->dev.serving;
}

_AUX afxError _AuxSshDtorCb(afxShell ssh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSH, 1, &ssh);

    AfxDeregisterChainedClasses(&ssh->dev.classes);

    return err;
}

_AUX afxError _AuxSshCtorCb(afxShell ssh, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSH, 1, &ssh);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    afxShellInfo const* info = ((afxShellInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AfxDevBaseImplementation.ctor(&ssh->dev, (void*[]) { icd, (void*)&info->dev }, 0)) AfxThrowError();
    else
    {
        //AfxGetSubClass(AfxGetShellClass(ssh))->ctor(ssh, args, invokeNo);

        afxClassConfig clsCfg;

        clsCfg = info->kbdClsCfg ? *info->kbdClsCfg : _AuxKbdStdImplementation;
        AfxMountClass(&ssh->kbdCls, NIL, &ssh->dev.classes, &clsCfg);

        clsCfg = info->mseClsCfg ? *info->mseClsCfg : _AuxMseStdImplementation;
        AfxMountClass(&ssh->mseCls, NIL, &ssh->dev.classes, &clsCfg);

        clsCfg = info->padClsCfg ? *info->padClsCfg : _AuxCtrlStdImplementation;
        AfxMountClass(&ssh->padCls, NIL, &ssh->dev.classes, &clsCfg);

        clsCfg = info->sesClsCfg ? *info->sesClsCfg : _AUX_SES_CLASS_CONFIG;
        AfxMountClass(&ssh->sesCls, NIL, &ssh->dev.classes, &clsCfg);

        clsCfg = info->wndClsCfg ? *info->wndClsCfg : _AUX_WND_CLASS_CONFIG;
        ssh->wndClsCfg = clsCfg;

        clsCfg = info->xssClsCfg ? *info->xssClsCfg : /*_AuxXssStdImplementation*/(afxClassConfig) { 0 };
        ssh->xssClsCfg = clsCfg;

        if (AfxCallDevice(&ssh->dev, afxFcc_USYS, NIL)) AfxThrowError();
        else
        {


            if (err)
            {
                AfxDeregisterChainedClasses(&ssh->dev.classes);
            }
        }
    }
    return err;
}

_AUX afxClassConfig const _AUX_SSH_CLASS_CONFIG =
{
    .fcc = afxFcc_SSH,
    .name = "Shell",
    .desc = "Shell Driver Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxShell)),
    .ctor = (void*)_AuxSshCtorCb,
    .dtor = (void*)_AuxSshDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION DISCOVERY                                                   //
////////////////////////////////////////////////////////////////////////////////

_AUX afxUnit AfxInvokeShells(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*,afxShell), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AuxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AUX) == (afxModuleFlag_ICD | afxModuleFlag_AUX));
        afxClass const* cls = _AuxGetShellClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SSH);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}

_AUX afxUnit AfxEvokeShells(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxShell), afxUnit cnt, afxShell shells[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(shells);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;
    afxModule mdle;
    while (_AuxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AUX) == (afxModuleFlag_ICD | afxModuleFlag_AUX));
        afxClass const* cls = _AuxGetShellClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SSH);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)shells);
        break;
    }
    return rslt;
}

_AUX afxUnit AfxEnumerateShells(afxUnit icd, afxUnit first, afxUnit cnt, afxShell shells[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(shells);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AuxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AUX) == (afxModuleFlag_ICD | afxModuleFlag_AUX));
        afxClass const* cls = _AuxGetShellClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SSH);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)shells);
        break;
    }
    return rslt;
}
