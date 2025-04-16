/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   U S E R   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_SYSTEM_C
#define _AFX_MODULE_C
#define _AUX_UX_C
#include "auxImplementation.h"

AFX afxChain* _AfxGetSystemClassChain(void);

_AUX afxClass const* _AuxGetHidClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AUX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.hidCls;
    AFX_ASSERT_CLASS(cls, afxFcc_HID);
    return cls;
}

_AUX afxClass const* _AuxGetShellClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AUX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.sshCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SSH);
    return cls;
}

_AUX afxClass const* _AuxGetSessionClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AUX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.sesCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SES);
    return cls;
}

_AUX afxError _AuxRegisterShells(afxModule icd, afxUnit cnt, afxShellInfo const infos[], afxShell shells[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(shells);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AUX))
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AuxGetShellClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_SSH);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)shells, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SSH, cnt, shells);

    }
    return err;
}

_AUX afxError _AuxImplementShell(afxModule icd, afxClassConfig const* sesCls)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(sesCls);

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxClassConfig clsCfg;

    clsCfg = sesCls ? *sesCls : _AUX_SES_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_SES);
    AFX_ASSERT(clsCfg.fixedSiz >= _AUX_SES_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_SES) ||
        (_AUX_SES_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
    {
        AfxThrowError();
    }
    else if (AfxMountClass(&icd->icd.sesCls, NIL, &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
    }
    else
    {
        afxSystem sys;
        AfxGetSystem(&sys);
        AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
        AfxPushLink(&icd->icd.aux, &sys->aux.icdChain);
        icd->flags |= afxModuleFlag_AUX;
    }

    if (err)
    {
        AfxDismountClass(&icd->icd.sesCls);
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AUX));

    return err;
}

_AUX afxBool _AuxGetIcd(afxUnit icdIdx, afxModule* driver)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule icd = NIL;
    while ((icdIdx < sys->aux.icdChain.cnt) && (icd = AFX_REBASE(AfxFindFirstLink(&sys->aux.icdChain, icdIdx), AFX_OBJ(afxModule), icd.aux)))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

        if (AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AUX))
            found = TRUE;
        else
            icd = NIL;

        break;
    }
    AFX_ASSERT(driver);
    *driver = icd;
    return found;
}

_AUX afxError auxScmHook(afxModule mdle, afxManifest const* ini)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    // HIDs must be installed here. It is not implementation-dependent. It does not store state.
    AfxMountClass(&sys->aux.hidCls, (afxClass*)_AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_AUX_HID_CLASS_CONFIG);

    //AfxMountClass(&sys->aux.sshCls, (afxClass*)_AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_AUX_SSH_CLASS_CONFIG); // require base*

    if (!err)
    {
        afxHidInfo hidInfos[] =
        {
            {
                .dev.urn = AFX_STRING("kbd"),
                .dev.type = afxDeviceType_HID,
                .type = afxHidType_KEYBOARD
            },
            {
                .dev.urn = AFX_STRING("mse"),
                .dev.type = afxDeviceType_HID,
                .type = afxHidType_MOUSE
            },
            {
                .dev.urn = AFX_STRING("gpad"),
                .dev.type = afxDeviceType_HID,
                .type = afxHidType_GAMEPAD
            }
        };
        afxHid hidevices[ARRAY_SIZE(hidInfos)];

        if (_AuxRegisterHids(mdle, ARRAY_SIZE(hidInfos), hidInfos, hidevices)) AfxThrowError();
        else
        {
        }
    }

    return err;
}
