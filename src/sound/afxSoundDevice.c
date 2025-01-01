/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <stdio.h>
#include <Windows.h>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AFX_SYSTEM_C

#define _ASX_SOUND_C
#define _ASX_SOUND_DEVICE_C
#define _ASX_SOUND_INPUT_C
#define _ASX_SINK_C
#define _ASX_SOUND_SYSTEM_C
#define _ASX_SOUND_SYSTEM_C
#include "impl/asxImplementation.h"

extern afxClassConfig const _ASX_ASIO_CLASS_CONFIG;
AFX afxChain* _AfxGetSystemClassChain(void);

////////////////////////////////////////////////////////////////////////////////
// SOUND DEVICE HANDLING                                                      //
////////////////////////////////////////////////////////////////////////////////

_ASX afxClass const* _AsxGetSoundBridgeClass(afxSoundSystem ssys)
{
    afxError err = AFX_ERR_NONE;
    /// sdev must be a valid afxSoundDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxClass const* cls = &ssys->sexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SEXU);
    return cls;
}

_ASX afxBool AfxIsSoundDevicePrompt(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    return sdev->dev.serving;
}

_ASX afxUnit AfxCountSoundPorts(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    return sdev->portCnt;
}

_ASX void AfxQuerySoundDeviceLimits(afxSoundDevice sdev, afxSoundLimits* limits)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    AFX_ASSERT(limits);
    *limits = sdev->limits;
}

_ASX void AfxQuerySoundDeviceFeatures(afxSoundDevice sdev, afxSoundFeatures* features)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    AFX_ASSERT(features);
    *features = sdev->features;
}

_ASX void AfxQuerySoundPortCapabilities(afxSoundDevice sdev, afxUnit portId, afxSoundPortCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    AFX_ASSERT_RANGE(sdev->portCnt, portId, 1);
    AFX_ASSERT(caps);
    *caps = sdev->ports[portId].caps;
}

_ASX afxUnit AfxChooseSoundPorts(afxSoundDevice sdev, afxSoundPortFlags caps, afxAcceleration accel, afxUnit maxCnt, afxUnit portId[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    afxUnit rslt = 0;

    for (afxUnit i = sdev->portCnt; i-- > 0;)
    {
        afxSoundPortCaps const* port = &sdev->ports[i].caps;

        if (!caps || (caps == (caps & port->capabilities)))
        {
            if (!accel || (accel == (accel & port->acceleration)))
            {
                portId[rslt++] = i;
            }
        }

        if (rslt == maxCnt)
            break;
    }
    return rslt;
}

_ASX afxError _AsxSdevDtorCb(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

    AfxDeregisterChainedClasses(&sdev->dev.classes);

    AfxCallDevice(&sdev->dev, 2, NIL); // terminate

    AFX_ASSERT(!sdev->idd);
    AfxDeregisterChainedClasses(&sdev->dev.classes);

    afxObjectStash stashes[] =
    {
        {
            .cnt = sdev->portCnt,
            .siz = sizeof(sdev->ports[0]),
            .var = (void**)&sdev->ports
        }
    };
    AfxDeallocateInstanceData(sdev, ARRAY_SIZE(stashes), stashes);

    return err;
}

_ASX afxError _AsxSdevCtorCb(afxSoundDevice sdev, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    afxSoundDeviceInfo const* info = ((afxSoundDeviceInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AfxDevBaseImplementation.ctor(&sdev->dev, (void*[]) { icd, (void*)&info->dev }, 0)) AfxThrowError();
    else
    {
        sdev->idd = NIL;

        sdev->limits = info->limits;
        sdev->features = info->features;

        sdev->portCnt = info->portCnt;

        afxObjectStash stashes[] =
        {
            {
                .cnt = sdev->portCnt,
                .siz = sizeof(sdev->ports[0]),
                .var = (void**)&sdev->ports
            }
        };

        if (AfxAllocateInstanceData(sdev, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
        else
        {
            if (!sdev->portCnt) AfxThrowError();
            else
            {
                AFX_ASSERT(sdev->ports);

                for (afxUnit i = 0; i < sdev->portCnt; i++)
                {
                    sdev->ports[i].caps = (afxSoundPortCaps) { 0 };// info->portCaps[i];
                    AfxMakeString128(&sdev->ports[i].desc, NIL);
                    AfxMakeString8(&sdev->ports[i].urn, NIL);
                }

                if (AfxCallDevice(&sdev->dev, afxFcc_SSYS, NIL)) AfxThrowError();
                else
                {
                    if (err)
                    {
                        AfxDeregisterChainedClasses(&sdev->dev.classes);
                    }
                }
            }

            if (err)
                AfxDeallocateInstanceData(sdev, ARRAY_SIZE(stashes), stashes);
        }
    }
    return err;
}

_ASX afxClassConfig const _ASX_SDEV_CLASS_CONFIG =
{
    .fcc = afxFcc_SDEV,
    .name = "SoundDevice",
    .desc = "Sound Device Driver Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundDevice)),
    .ctor = (void*)_AsxSdevCtorCb,
    .dtor = (void*)_AsxSdevDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION DISCOVERY                                                   //
////////////////////////////////////////////////////////////////////////////////

_ASX afxUnit AfxInvokeSoundDevices(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxSoundDevice), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSoundDeviceClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}

_ASX afxUnit AfxEvokeSoundDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxSoundDevice), afxUnit cnt, afxSoundDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSoundDeviceClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_ASX afxUnit AfxEnumerateSoundDevices(afxUnit icd, afxUnit first, afxUnit cnt, afxSoundDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSoundDeviceClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_ASX afxUnit AfxEnumerateSoundSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxSoundSystem systems[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSoundSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SSYS);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
        break;
    }
}

_ASX afxUnit AfxEvokeSoundSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxSoundSystem), afxUnit cnt, afxSoundSystem systems[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSoundSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SSYS);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)systems);
        break;
    }
    return rslt;
}

_ASX afxUnit AfxInvokeSoundSystems(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*,afxSoundSystem), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSoundSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SSYS);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}
