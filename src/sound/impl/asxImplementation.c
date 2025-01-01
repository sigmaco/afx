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
#define _ASX_SOUND_C
//#define _ASX_SOUND_BRIDGE_C
//#define _ASX_SOUND_SYSTEM_C
//#define _ASX_SOUND_QUEUE_C
//#define _ASX_BUFFER_C
//#define _ASX_WAVEFORM_C
//#define _ASX_VOICE_C
//#define _ASX_MIXER_C
//#define _ASX_SINK_C
#include "asxImplementation.h"

_ASX afxClass const* _AsxGetAudioServiceClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.asiCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ASI);
    return cls;
}

_ASX afxClass const* _AsxGetSoundDeviceClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.sdevCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
    return cls;
}

_ASX afxClass const* _AsxGetSoundSystemClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.ssysCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SSYS);
    return cls;
}

_ASX afxError _AsxRegisterAudioServices(afxModule icd, afxUnit cnt, afxSoundDeviceInfo const infos[], afxSoundDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(devices);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX))
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AsxGetAudioServiceClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_SDEV);

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

_ASX afxError _AsxRegisterSoundDevices(afxModule icd, afxUnit cnt, afxSoundDeviceInfo const infos[], afxSoundDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(devices);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX))
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AsxGetSoundDeviceClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_SDEV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)devices, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SDEV, cnt, devices);
    }
    return err;
}

_ASX afxError _AsxImplementSoundSystem(afxModule icd, afxClassConfig const* asiCls, afxClassConfig const* sdevCls, afxClassConfig const* ssysCls)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT((sdevCls && ssysCls) || asiCls);

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
        clsCfg = sdevCls ? *sdevCls : _ASX_SDEV_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SDEV);
        AFX_ASSERT(clsCfg.fixedSiz >= _ASX_SDEV_CLASS_CONFIG.fixedSiz);
        if ((clsCfg.fcc != afxFcc_SDEV) ||
            (_ASX_SDEV_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
        {
            AfxThrowError();
        }
        else if (AfxMountClass(&icd->icd.sdevCls, (afxClass*)AfxGetDeviceClass(), &icd->classes, &clsCfg)) // require base*
        {
            AfxThrowError();
        }
        else
        {
            clsCfg = ssysCls ? *ssysCls : _ASX_SSYS_CLASS_CONFIG;
            AFX_ASSERT(clsCfg.fcc == afxFcc_SSYS);
            AFX_ASSERT(clsCfg.fixedSiz >= _ASX_SSYS_CLASS_CONFIG.fixedSiz);
            if ((clsCfg.fcc != afxFcc_SSYS) ||
                (_ASX_SSYS_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
            {
                AfxThrowError();
            }
            else if (AfxMountClass(&icd->icd.ssysCls, NIL, &icd->classes, &clsCfg)) // require base*
            {
                AfxThrowError();
            }
            else
            {
                afxSystem sys;
                AfxGetSystem(&sys);
                AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
                AfxPushLink(&icd->icd.asx, &sys->asx.icdChain);
                icd->flags |= afxModuleFlag_ASX;
            }
        }
    }

    if (err)
    {
        AfxDismountClass(&icd->icd.ssysCls);
        AfxDismountClass(&icd->icd.sdevCls);

        if (asiCls)
            AfxDismountClass(&icd->icd.asiCls);
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX));

    return err;
}

_ASX afxBool AsxGetIcd(afxUnit icdIdx, afxModule* driver)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule icd;
    while ((icd = AFX_REBASE(AfxFindFirstLink(&sys->asx.icdChain, icdIdx), AFX_OBJ(afxModule), icd.asx)))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

        if (AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX))
            found = TRUE;
        else
            icd = NIL;

        break;
    }
    AFX_ASSERT(driver);
    *driver = icd;
    return found;
}

_ASX afxError afxScmHook(afxModule mdle, afxManifest const* ini)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    //AfxMountClass(&sys->asx.sdevCls, (afxClass*)AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_ASX_SDEV_CLASS_CONFIG); // require base*

    if (!err)
    {
        //sys->asx.ready = TRUE;
    }

    //_AsxImplementSoundSystem(mdle, , &_ASX_SDEV_CLASS_CONFIG, &_ASX_SSYS_CLASS_CONFIG);

    return err;
}

