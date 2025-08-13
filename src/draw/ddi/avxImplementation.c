/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_SYSTEM_C
#define _AFX_MODULE_C
#define _AVX_DRAW_C
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_SYSTEM_C
//#define _AVX_DRAW_QUEUE_C
//#define _AVX_DRAW_OUTPUT_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "avxImplementation.h"

_AVX afxClass const* _AvxGetDisplayClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.viddCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VDU);
    return cls;
}

_AVX afxClass const* _AvxIcdGetDdevClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.ddevCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DDEV);
    return cls;
}

_AVX afxClass const* _AvxIcdGetDsysClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.dsysCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DSYS);
    return cls;
}

_AVX afxError _AvxImplementDrawSystem(afxModule icd, _avxDrawSystemImplementation const* cfg)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    //AFX_ASSERT((ddevCls && dsysCls) || viddCls);

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxBool viddMounted = FALSE, dsysMounted = FALSE, ddevMounted = FALSE;

    afxClassConfig viddCfg2;

    viddCfg2 = cfg->viddCls.fcc ? cfg->viddCls : _AVX_VDU_CLASS_CONFIG;
    AFX_ASSERT(viddCfg2.fcc == afxFcc_VDU);
    AFX_ASSERT(viddCfg2.fixedSiz >= _AVX_VDU_CLASS_CONFIG.fixedSiz);
    if ((viddCfg2.fcc != afxFcc_VDU) ||
        (_AVX_VDU_CLASS_CONFIG.fixedSiz > viddCfg2.fixedSiz))
    {
        AfxThrowError();
        return err;
    }
    else if (AfxMountClass(&icd->icd.viddCls, &sys->avx.viddCls, &icd->classes, &viddCfg2)) // require base*
    {
        AfxThrowError();
        return err;
    }
    else
    {
        viddMounted = TRUE;
    }

    afxClassConfig ddevCls2 = cfg->ddevCls.fcc ? cfg->ddevCls : _AVX_DDEV_CLASS_CONFIG;
    AFX_ASSERT(ddevCls2.fcc == afxFcc_DDEV);
    AFX_ASSERT(ddevCls2.fixedSiz >= _AVX_DDEV_CLASS_CONFIG.fixedSiz);
    if ((ddevCls2.fcc != afxFcc_DDEV) ||
        (_AVX_DDEV_CLASS_CONFIG.fixedSiz > ddevCls2.fixedSiz))
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.viddCls);
        return err;
    }
    else if (AfxMountClass(&icd->icd.ddevCls, &sys->avx.ddevCls, &icd->classes, &ddevCls2)) // require base*
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.viddCls);
        return err;
    }

    ddevMounted = TRUE;

    afxClassConfig dsysCls2 = cfg->dsysCls.fcc ? cfg->dsysCls : _AVX_DSYS_CLASS_CONFIG;
    AFX_ASSERT(dsysCls2.fcc == afxFcc_DSYS);
    AFX_ASSERT(dsysCls2.fixedSiz >= _AVX_DSYS_CLASS_CONFIG.fixedSiz);
    if ((dsysCls2.fcc != afxFcc_DSYS) ||
        (_AVX_DSYS_CLASS_CONFIG.fixedSiz > dsysCls2.fixedSiz))
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.viddCls);
        AfxDismountClass(&icd->icd.ddevCls);
        return err;
    }
    else if (AfxMountClass(&icd->icd.dsysCls, &sys->avx.dsysCls, &icd->classes, &dsysCls2)) // require base*
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.viddCls);
        AfxDismountClass(&icd->icd.ddevCls);
        return err;
    }
    else
    {
        dsysMounted = TRUE;
    }

    if (err)
    {
        if (dsysMounted)
            AfxDismountClass(&icd->icd.dsysCls);

        if (ddevMounted)
            AfxDismountClass(&icd->icd.ddevCls);

        if (viddMounted)
            AfxDismountClass(&icd->icd.viddCls);
    }
    else
    {
        if (!icd->icd.avx.chain)
        {
            afxSystem sys;
            AfxGetSystem(&sys);
            AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
            AfxPushLink(&icd->icd.avx, &sys->avx.icdChain);
            icd->flags |= afxModuleFlag_AVX;
        }
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX));

    return err;
}

_AVX afxBool _AvxGetIcd(afxUnit icdIdx, afxModule* driver)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule icd = NIL;
    while ((icdIdx < sys->avx.icdChain.cnt) && (icd = AFX_REBASE(AfxFindFirstLink(&sys->avx.icdChain, icdIdx), AFX_OBJ(afxModule), icd.avx)))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

        if (AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX))
            found = TRUE;
        else
            icd = NIL;

        break;
    }
    AFX_ASSERT(driver);
    *driver = icd;
    return found;
}

_AVX afxError avxScmHook(afxModule mdle, afxManifest const* ini)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxClassConfig ddevClsCfg = _AVX_DDEV_CLASS_CONFIG;
    ddevClsCfg.ctor = NIL;
    ddevClsCfg.dtor = NIL;
    AfxMountClass(&sys->avx.ddevCls, (afxClass*)_AfxGetDeviceClass(), &sys->classes, &ddevClsCfg);
    
    afxClassConfig dsysClsCfg = _AVX_DSYS_CLASS_CONFIG;
    dsysClsCfg.ctor = NIL;
    dsysClsCfg.dtor = NIL;
    AfxMountClass(&sys->avx.dsysCls, NIL, &sys->classes, &dsysClsCfg);
    
    afxClassConfig viddClsCfg = _AVX_VDU_CLASS_CONFIG;
    viddClsCfg.ctor = NIL;
    viddClsCfg.dtor = NIL;
    AfxMountClass(&sys->avx.viddCls, (afxClass*)_AfxGetDeviceClass(), &sys->classes, &viddClsCfg);

    if (!err)
    {
        //sys->avx.ready = TRUE;
    }

    return err;
}
