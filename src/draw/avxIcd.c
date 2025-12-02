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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AFX_SYSTEM_C
#define _AFX_MODULE_C
#define _AVX_DRAW_C
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_SYSTEM_C
//#define _AVX_DRAW_QUEUE_C
//#define _AVX_SURFACE_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "avxIcd.h"

_AVX afxClass const* _AvxIcdGetDdevClass(afxModule icd)
{
    afxError err = { 0 };
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
    afxError err = { 0 };
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

_AVX afxError _AvxImplementDrawSystem(afxModule icd, _avxDsysImp const* cfg)
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

    afxBool dsysMounted = FALSE, ddevMounted = FALSE;

    afxClassConfig ddevCls2 = cfg->ddevCls.fcc ? cfg->ddevCls : _AVX_CLASS_CONFIG_DDEV;
    AFX_ASSERT(ddevCls2.fcc == afxFcc_DDEV);
    AFX_ASSERT(ddevCls2.fixedSiz >= _AVX_CLASS_CONFIG_DDEV.fixedSiz);
    if ((ddevCls2.fcc != afxFcc_DDEV) ||
        (_AVX_CLASS_CONFIG_DDEV.fixedSiz > ddevCls2.fixedSiz))
    {
        AfxThrowError();
        return err;
    }
    else if (AfxMountClass(&icd->icd.ddevCls, &sys->ddevCls, &icd->classes, &ddevCls2)) // require base*
    {
        AfxThrowError();
        return err;
    }

    ddevMounted = TRUE;

    afxClassConfig dsysCls2 = cfg->dsysCls.fcc ? cfg->dsysCls : _AVX_CLASS_CONFIG_DSYS;
    AFX_ASSERT(dsysCls2.fcc == afxFcc_DSYS);
    AFX_ASSERT(dsysCls2.fixedSiz >= _AVX_CLASS_CONFIG_DSYS.fixedSiz);
    if ((dsysCls2.fcc != afxFcc_DSYS) ||
        (_AVX_CLASS_CONFIG_DSYS.fixedSiz > dsysCls2.fixedSiz))
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.ddevCls);
        return err;
    }
    else if (AfxMountClass(&icd->icd.dsysCls, &sys->dsysCls, &icd->classes, &dsysCls2)) // require base*
    {
        AfxThrowError();
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
    }
    else
    {
        if (!icd->icd.avx.chain)
        {
            AfxPushLink(&icd->icd.avx, &sys->avxIcdChain);
            icd->flags |= afxModuleFlag_AVX;
        }
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX));

    return err;
}

_AVX afxBool _AvxGetIcd(afxUnit icdIdx, afxModule* driver)
{
    afxError err = { 0 };
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule icd = NIL;
    while ((icdIdx < sys->avxIcdChain.cnt) && (icd = AFX_REBASE(AfxFindFirstLink(&sys->avxIcdChain, icdIdx), AFX_OBJ(afxModule), icd.avx)))
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
