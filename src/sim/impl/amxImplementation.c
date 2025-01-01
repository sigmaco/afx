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

#define _AFX_SYSTEM_C
#define _AFX_MODULE_C
#define _AFX_SIM_C
#include "amxImplementation.h"

AFX afxChain* _AfxGetSystemClassChain(void);

_AMX afxBool AmxGetIcd(afxUnit icdIdx, afxModule* driver)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule icd;
    while ((icd = AFX_REBASE(AfxFindFirstLink(&sys->amx.icdChain, icdIdx), AFX_OBJ(afxModule), icd.amx)))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

        if (AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
            found = TRUE;
        else
            icd = NIL;

        break;
    }
    AFX_ASSERT(driver);
    *driver = icd;
    return found;
}

_AMX afxClass const* _AmxGetEngineClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.mdevCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
    return cls;
}

_AMX afxClass const* _AmxGetSimulationClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.simCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SIM);
    return cls;
}

_AMX afxError _AmxRegisterEngines(afxModule icd, afxUnit cnt, afxEngineInfo const infos[], afxEngine devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(devices);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AmxGetEngineClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)devices, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDEV, cnt, devices);
    }
    return err;
}

_AMX afxError _AmxImplementSimulation(afxModule icd, afxClassConfig const* mdevCls, afxClassConfig const* simCls)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT((mdevCls && simCls));

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxClassConfig clsCfg;

    if (!err)
    {
        clsCfg = mdevCls ? *mdevCls : _AMX_MDEV_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MDEV);
        AFX_ASSERT(clsCfg.fixedSiz >= _AMX_MDEV_CLASS_CONFIG.fixedSiz);
        if ((clsCfg.fcc != afxFcc_MDEV) ||
            (_AMX_MDEV_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
        {
            AfxThrowError();
        }
        else if (AfxMountClass(&icd->icd.mdevCls, (afxClass*)AfxGetDeviceClass(), &icd->classes, &clsCfg)) // require base*
        {
            AfxThrowError();
        }
        else
        {
            clsCfg = simCls ? *simCls : _AMX_SIM_CLASS_CONFIG;
            AFX_ASSERT(clsCfg.fcc == afxFcc_SIM);
            AFX_ASSERT(clsCfg.fixedSiz >= _AMX_SIM_CLASS_CONFIG.fixedSiz);
            if ((clsCfg.fcc != afxFcc_SIM) ||
                (_AMX_SIM_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
            {
                AfxThrowError();
            }
            else if (AfxMountClass(&icd->icd.simCls, NIL, &icd->classes, &clsCfg)) // require base*
            {
                AfxThrowError();
            }
            else
            {
                afxSystem sys;
                AfxGetSystem(&sys);
                AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
                AfxPushLink(&icd->icd.amx, &sys->amx.icdChain);
                icd->flags |= afxModuleFlag_AMX;
            }
        }
    }

    if (err)
    {
        AfxDismountClass(&icd->icd.simCls);
        AfxDismountClass(&icd->icd.mdevCls);
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX));

    return err;
}

_AMX afxError afxScmHook(afxModule mdle, afxManifest const* ini)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    //AfxMountClass(&sys->amx.mdevCls, (afxClass*)AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_AMX_MDEV_CLASS_CONFIG); // require base*

    if (!err)
    {
        //sys->asx.ready = TRUE;
    }

    return err;
}
