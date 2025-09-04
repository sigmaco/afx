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
#define _AMX_MIX_C
#define _AFX_DEVICE_C
#define _AMX_MIX_DEVICE_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_QUEUE_C
//#define _AMX_BUFFER_C
//#define _AMX_AUDIO_C
//#define _AMX_VOICE_C
//#define _AMX_MIX_CONTEXT_C
//#define _AMX_SINK_C
#include "amxImplementation.h"

_AMX afxClass const* _AmxIcdGetAudioServiceClass(afxModule icd)
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

_AMX afxClass const* _AmxIcdGetMcdcClass(afxModule icd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.mcdcCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
    return cls;
}

_AMX afxClass const* _AmxIcdGetMdevClass(afxModule icd)
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

_AMX afxClass const* _AmxIcdGetMsysClass(afxModule icd)
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

_AMX afxError _AmxRegisterAudioServices(afxModule icd, afxUnit cnt, afxMixDeviceInfo const infos[], afxDevice devices[])
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

    afxClass* cls = (afxClass*)_AmxIcdGetAudioServiceClass(icd);
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

_AMX afxError _AmxRegisterAudioStreamInterface(afxModule icd, afxClassConfig const* asiCls)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(asiCls);

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxClassConfig clsCfg;

    clsCfg = *asiCls;
    AFX_ASSERT(clsCfg.fcc == afxFcc_ASI);
    //AFX_ASSERT(clsCfg.fixedSiz >= _AVX_VDU_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_ASI) //||
        //(_AVX_VDU_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz)
        )
    {
        AfxThrowError();
    }

    // open URI through ASI by id.
    // AfxOpenAudioSink(vaioId, "//./out0"); // speaker
    // AfxOpenAudioSink(vaioId, "//./out1"); // headphone
    // AfxOpenAudioSink(vaioId, "//./in0"); // aux
    // AfxOpenAudioSink(vaioId, "//./in1"); // mic
    // AfxOpenAudioSink(vaioId, "//./in2"); // headmic
    // AfxOpenAudioSink(vaioId, "//./in3"); // mix

    if (AfxMountClass(&icd->icd.asiCls, (afxClass*)_AfxGetDeviceClass(), &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
    }
    else
    {
        afxSystem sys;
        AfxGetSystem(&sys);
        AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
        AfxPushLink(&icd->icd.amx, &sys->amxIcdChain);
        icd->flags |= afxModuleFlag_AMX;
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX));

    return err;
}

_AMX afxError _AmxImplementMixSystem(afxModule icd, _amxMixSystemImplementation const* cfg)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    //AFX_ASSERT((mdevCls && msysCls));

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxClassConfig clsCfg;

    clsCfg = _AMX_ASIO_CLASS_CONFIG;
    if (AfxMountClass(&icd->icd.asiCls, (afxClass*)NIL, &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
        return err;
    }

    clsCfg = cfg->mcdcCls.fcc ? cfg->mcdcCls : _AMX_MCDC_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_MCDC);
    AFX_ASSERT(clsCfg.fixedSiz >= _AMX_MCDC_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_MCDC) ||
        (_AMX_MCDC_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        return err;
    }
    else if (AfxMountClass(&icd->icd.mcdcCls, (afxClass*)NIL, &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        return err;
    }

    clsCfg = cfg->mdevCls.fcc ? cfg->mdevCls : _AMX_MDEV_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_MDEV);
    AFX_ASSERT(clsCfg.fixedSiz >= _AMX_MDEV_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_MDEV) ||
        (_AMX_MDEV_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        AfxDismountClass(&icd->icd.mcdcCls);
        return err;
    }
    else if (AfxMountClass(&icd->icd.mdevCls, (afxClass*)_AfxGetDeviceClass(), &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        AfxDismountClass(&icd->icd.mcdcCls);
        return err;
    }

    clsCfg = cfg->msysCls.fcc ? cfg->msysCls : _AMX_MSYS_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_MSYS);
    AFX_ASSERT(clsCfg.fixedSiz >= _AMX_MSYS_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_MSYS) ||
        (_AMX_MSYS_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        AfxDismountClass(&icd->icd.mcdcCls);
        AfxDismountClass(&icd->icd.mdevCls);
        return err;
    }
    else if (AfxMountClass(&icd->icd.msysCls, NIL, &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.asiCls);
        AfxDismountClass(&icd->icd.mcdcCls);
        AfxDismountClass(&icd->icd.mdevCls);
        return err;
    }

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    AfxPushLink(&icd->icd.amx, &sys->amxIcdChain);
    icd->flags |= afxModuleFlag_AMX;

    if (err)
    {
        AfxDismountClass(&icd->icd.asiCls);
        AfxDismountClass(&icd->icd.mcdcCls);
        AfxDismountClass(&icd->icd.mdevCls);
        AfxDismountClass(&icd->icd.msysCls);
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

    afxModule icd = NIL;
    while ((icdIdx < sys->amxIcdChain.cnt) && (icd = AFX_REBASE(AfxFindFirstLink(&sys->amxIcdChain, icdIdx), AFX_OBJ(afxModule), icd.amx)))
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

_AMX afxError amxScmHook(afxModule mdle, afxManifest const* ini)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    //AfxMountClass(&sys->amx.sdevCls, (afxClass*)_AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_AMX_MDEV_CLASS_CONFIG); // require base*

    if (!err)
    {
        //sys->amx.ready = TRUE;
    }

    //_AmxImplementMixSystem(mdle, , &_AMX_MDEV_CLASS_CONFIG, &_AMX_MSYS_CLASS_CONFIG);

    return err;
}

_AMX afxError amxIcdHook(afxModule icd, afxUri const* manifest)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    afxClassConfig mdevClsCfg = _AMX_MDEV_CLASS_CONFIG;
    afxClassConfig msysClsCfg = _AMX_MSYS_CLASS_CONFIG;

    _amxMixSystemImplementation mimpl = { 0 };
    mimpl.mcdcCls = _AMX_MCDC_CLASS_CONFIG;
    mimpl.mdevCls = _AMX_MDEV_CLASS_CONFIG;
    mimpl.msysCls = _AMX_MSYS_CLASS_CONFIG;

    if (_AmxImplementMixSystem(icd, &mimpl))
    {
        AfxThrowError();
        return err;
    }

    static afxMixFeatures features = { 0 };

    // In Qwadro AFX, I used to name things based on bad experiences solving issues.
    // Vaio has been named after a bad experience making way for interop with a mix system.
    // Targa has been named after a bad experience handling graphical operations with images originating from left/bottom side.
    // Kurwa has been named after a bad experience when implementing curve-based motion in simulation framework.

    _amxMixDeviceRegistration mdevInfos[] =
    {
        {
            .dev.urn = AFX_STRING("vaio"),
            .dev.type = afxDeviceType_SOUND,

            .features = features,

            //.dev.ioctl = (void*)_ZalSdevIoctrlCb,
            //.relinkAsioCb = _ZalRelinkAsioWasapiCb,

            .capabilities = afxMixCaps_MIX | afxMixCaps_SIM,
            .minQueCnt = 2,
            .maxQueCnt = 16,
        },
    };

    afxMixDevice mdevices[ARRAY_SIZE(mdevInfos)];

    if (_AmxRegisterMixDevices(icd, ARRAY_SIZE(mdevInfos), mdevInfos, mdevices))
    {
        AfxThrowError();
    }

    return err;
}
