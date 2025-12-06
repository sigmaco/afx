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

#define _AFX_SYSTEM_C
#define _AFX_MODULE_C
#define _AUX_UX_C
#include "auxIcd.h"

AFX afxChain* _AfxGetSystemClassChain(void);

_AVX afxClass const* _AvxGetDisplayClass(afxModule icd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.vduCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VDU);
    return cls;
}

_AUX afxError _AuxGetInteropDisplayClass(afxUnit icd, afxString const* tool, afxClassConfig* clsc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    afxModule driver;
    if (!_AuxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }

    if (!AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AUX))
    {
        AfxThrowError();
        return NIL;
    }
    
    driver->icd.getVduClsc(driver, tool, clsc);

    return err;
}

_AUX afxError _AuxGetInteropSurfaceClass(afxDrawSystem dsys, afxString const* tool, afxClassConfig* clsc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxModule driver;
    if (!_AuxGetIcd(0, &driver))
    {
        AfxThrowError();
        return err;
    }
    driver->icd.getDoutClsc(dsys, tool, clsc);
    return err;
}

_AUX afxError _AuxGetInteropSinkClass(afxMixSystem msys, afxString const* tool, afxClassConfig* clsc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxModule driver;
    if (!_AuxGetIcd(0, &driver))
    {
        AfxThrowError();
        return err;
    }
    driver->icd.getSinkClsc(msys, tool, clsc);
    return err;
}

_AUX afxClass const* _AuxGetHidClass(afxModule icd)
{
    afxError err = { 0 };
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
    afxError err = { 0 };
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

_AUX afxClass const* _AuxIcdGetEnvClass(afxModule icd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AUX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.sesCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ENV);
    return cls;
}

_AUX afxError _AuxRegisterShells(afxModule icd, afxUnit cnt, _auxShellAcq const infos[], afxShell shells[])
{
    afxError err = { 0 };
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

_AUX afxError _AuxImplementShell(afxModule icd, _afxShellImpl const* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(cfg);

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxBool vduMounted = FALSE, sesMounted = FALSE;

    afxClassConfig vduCfg2 = cfg->vduCls.fcc ? cfg->vduCls : _AVX_VDU_CLASS_CONFIG;
    AFX_ASSERT(vduCfg2.fcc == afxFcc_VDU);
    AFX_ASSERT(vduCfg2.fixedSiz >= _AVX_VDU_CLASS_CONFIG.fixedSiz);
    if ((vduCfg2.fcc != afxFcc_VDU) ||
        (_AVX_VDU_CLASS_CONFIG.fixedSiz > vduCfg2.fixedSiz))
    {
        AfxThrowError();
        return err;
    }
    else if (AfxMountClass(&icd->icd.vduCls, &sys->vduCls, &icd->classes, &vduCfg2)) // require base*
    {
        AfxThrowError();
        return err;
    }
    else
    {
        vduMounted = TRUE;
    }

    afxClassConfig sesClsCfg = cfg->sesCls.fcc ? cfg->sesCls : _AUX_ENV_CLASS_CONFIG;
    AFX_ASSERT(sesClsCfg.fcc == afxFcc_ENV);
    AFX_ASSERT(sesClsCfg.fixedSiz >= _AUX_ENV_CLASS_CONFIG.fixedSiz);
    if ((sesClsCfg.fcc != afxFcc_ENV) ||
        (_AUX_ENV_CLASS_CONFIG.fixedSiz > sesClsCfg.fixedSiz))
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.vduCls);
        return err;
    }
    else if (AfxMountClass(&icd->icd.sesCls, NIL, &icd->classes, &sesClsCfg)) // require base*
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.vduCls);
        return err;
    }
    else
    {
        sesMounted = TRUE;
    }

    {
        icd->icd.getDoutClsc = cfg->getInteropDoutCls;
        icd->icd.getSinkClsc = cfg->getInteropSinkCls;

        AfxPushLink(&icd->icd.aux, &sys->auxIcdChain);
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
    afxError err = { 0 };
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule icd = NIL;
    while ((icdIdx < sys->auxIcdChain.cnt) && (icd = AFX_REBASE(AfxFindFirstLink(&sys->auxIcdChain, icdIdx), AFX_OBJ(afxModule), icd.aux)))
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
