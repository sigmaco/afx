/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                     Q W A D R O   S O U N D   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>

#define _AFX_SYSTEM_C
#define _AFX_MODULE_C
#define _AMX_SOUND_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_QUEUE_C
//#define _AMX_BUFFER_C
//#define _AMX_WAVEFORM_C
//#define _AMX_VOICE_C
//#define _AMX_MIX_CONTEXT_C
//#define _AMX_SINK_C
#include "amxImplementation.h"

_AMX afxClass const* _AmxGetAudioServiceClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.asiCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ASI);
    return cls;
}

_AMX afxClass const* _AmxGetMixDeviceClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.sdevCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
    return cls;
}

_AMX afxClass const* _AmxGetMixSystemClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.msysCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
    return cls;
}

_AMX afxError _AmxRegisterAudioServices(afxModule icd, afxUnit cnt, afxMixDeviceInfo const infos[], afxMixDevice devices[])
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

    afxClass* cls = (afxClass*)_AmxGetAudioServiceClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)devices, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_ASI, cnt, devices);
    }
    return err;
}

_AMX afxError _AmxRegisterMixDevices(afxModule icd, afxUnit cnt, afxMixDeviceInfo const infos[], afxMixDevice devices[])
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

    afxClass* cls = (afxClass*)_AmxGetMixDeviceClass(icd);
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

_AMX afxError _AmxImplementMixSystem(afxModule icd, afxClassConfig const* asiCls, afxClassConfig const* mdevCls, afxClassConfig const* msysCls)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT((mdevCls && msysCls) || asiCls);

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxClassConfig clsCfg;

    if (asiCls)
    {
        clsCfg = *asiCls;
        AFX_ASSERT(clsCfg.fcc == afxFcc_ASI);
        //AFX_ASSERT(clsCfg.fixedSiz >= _AVX_VDU_CLASS_CONFIG.fixedSiz);
        if ((clsCfg.fcc != afxFcc_ASI) //||
            //(_AVX_VDU_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz)
            )
        {
            AfxThrowError();
            return err;
        }

        if (AfxMountClass(&icd->icd.asiCls, (afxClass*)AfxGetDeviceClass(), &icd->classes, &clsCfg)) // require base*
        {
            AfxThrowError();
            return err;
        }
    }

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
            clsCfg = msysCls ? *msysCls : _AMX_MSYS_CLASS_CONFIG;
            AFX_ASSERT(clsCfg.fcc == afxFcc_MSYS);
            AFX_ASSERT(clsCfg.fixedSiz >= _AMX_MSYS_CLASS_CONFIG.fixedSiz);
            if ((clsCfg.fcc != afxFcc_MSYS) ||
                (_AMX_MSYS_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
            {
                AfxThrowError();
            }
            else if (AfxMountClass(&icd->icd.msysCls, NIL, &icd->classes, &clsCfg)) // require base*
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
        AfxDismountClass(&icd->icd.msysCls);
        AfxDismountClass(&icd->icd.mdevCls);

        if (asiCls)
            AfxDismountClass(&icd->icd.asiCls);
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX));

    return err;
}

_AMX afxBool _AmxGetIcd(afxUnit icdIdx, afxModule* driver)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule icd;
    while ((icdIdx < sys->amx.icdChain.cnt) && (icd = AFX_REBASE(AfxFindFirstLink(&sys->amx.icdChain, icdIdx), AFX_OBJ(afxModule), icd.amx)))
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

_AMX afxError afxScmHook(afxModule mdle, afxManifest const* ini)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    //AfxMountClass(&sys->amx.sdevCls, (afxClass*)AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_AMX_MDEV_CLASS_CONFIG); // require base*

    if (!err)
    {
        //sys->amx.ready = TRUE;
    }

    //_AmxImplementMixSystem(mdle, , &_AMX_MDEV_CLASS_CONFIG, &_AMX_MSYS_CLASS_CONFIG);

    return err;
}

