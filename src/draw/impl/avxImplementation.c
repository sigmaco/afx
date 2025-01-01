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

_AVX afxClass const* _AvxGetDrawDeviceClass(afxModule icd)
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

_AVX afxClass const* _AvxGetDrawSystemClass(afxModule icd)
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

_AVX afxError _AvxRegisterDrawDevices(afxModule icd, afxUnit cnt, afxDrawDeviceInfo const infos[], afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(devices);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX))
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AvxGetDrawDeviceClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_DDEV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)devices, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_DDEV, cnt, devices);

    }
    return err;
}

_AVX afxError _AvxImplementDrawSystem(afxModule icd, afxClassConfig const* vduCls, afxClassConfig const* ddevCls, afxClassConfig const* dsysCls)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT((ddevCls && dsysCls) || vduCls);

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxClassConfig clsCfg;

    if (vduCls)
    {
        clsCfg = *vduCls;
        AFX_ASSERT(clsCfg.fcc == afxFcc_VDU);
        //AFX_ASSERT(clsCfg.fixedSiz >= _AVX_VDU_CLASS_CONFIG.fixedSiz);
        if ((clsCfg.fcc != afxFcc_VDU) //||
            //(_AVX_VDU_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz)
            )
        {
            AfxThrowError();
            return err;
        }

        if (AfxMountClass(&icd->icd.vduCls, (afxClass*)AfxGetDeviceClass(), &icd->classes, &clsCfg)) // require base*
        {
            AfxThrowError();
            return err;
        }
    }

    if (!err)
    {
        clsCfg = ddevCls ? *ddevCls : _AVX_DDEV_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_DDEV);
        AFX_ASSERT(clsCfg.fixedSiz >= _AVX_DDEV_CLASS_CONFIG.fixedSiz);
        if ((clsCfg.fcc != afxFcc_DDEV) ||
            (_AVX_DDEV_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
        {
            AfxThrowError();
        }
        else if (AfxMountClass(&icd->icd.ddevCls, (afxClass*)AfxGetDeviceClass(), &icd->classes, &clsCfg)) // require base*
        {
            AfxThrowError();
        }
        else
        {
            clsCfg = dsysCls ? *dsysCls : _AVX_DSYS_CLASS_CONFIG;
            AFX_ASSERT(clsCfg.fcc == afxFcc_DSYS);
            AFX_ASSERT(clsCfg.fixedSiz >= _AVX_DSYS_CLASS_CONFIG.fixedSiz);
            if ((clsCfg.fcc != afxFcc_DSYS) ||
                (_AVX_DSYS_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
            {
                AfxThrowError();
            }
            else if (AfxMountClass(&icd->icd.dsysCls, NIL, &icd->classes, &clsCfg)) // require base*
            {
                AfxThrowError();
            }
            else
            {
                afxSystem sys;
                AfxGetSystem(&sys);
                AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
                AfxPushLink(&icd->icd.avx, &sys->avx.icdChain);
                icd->flags |= afxModuleFlag_AVX;
            }
        }
    }

    if (err)
    {
        AfxDismountClass(&icd->icd.dsysCls);
        AfxDismountClass(&icd->icd.ddevCls);

        if (vduCls)
            AfxDismountClass(&icd->icd.vduCls);
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX));

    return err;
}

_AVX afxBool AvxGetIcd(afxUnit icdIdx, afxModule* driver)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule icd;
    while ((icd = AFX_REBASE(AfxFindFirstLink(&sys->avx.icdChain, icdIdx), AFX_OBJ(afxModule), icd.avx)))
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

_AVX afxError afxScmHook(afxModule mdle, afxManifest const* ini)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    //AfxMountClass(&sys->avx.ddevCls, (afxClass*)AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_AVX_DDEV_CLASS_CONFIG); // require base*

    if (!err)
    {
        //sys->avx.ready = TRUE;
    }

    return err;
}
