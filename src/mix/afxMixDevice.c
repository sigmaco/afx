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

#define _AMX_SOUND_C
#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_INPUT_C
#define _AMX_SINK_C
#define _AMX_MIX_SYSTEM_C
#include "impl/amxImplementation.h"

extern afxClassConfig const _AMX_ASIO_CLASS_CONFIG;
AFX afxChain* _AfxGetSystemClassChain(void);

_AMX afxString const sigmaMixSignature = AFX_STRING(
    "      ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::      \n"
    "     :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:     \n"
    "     +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+     \n"
    "     +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+     \n"
    "     +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+     \n"
    "     #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#     \n"
    "      ###### ###  ###   ###   ###     ### #########  ###    ###  ########      \n"
    "                                                                               \n"
    "   Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E   \n"
    "                                                                               \n"
    "                               Public Test Build                               \n"
    "                           (c) 2017 SIGMA FEDERATION                           \n"
    "                                www.sigmaco.org                                \n"
    "                                                                               \n"
);

////////////////////////////////////////////////////////////////////////////////
// SOUND DEVICE HANDLING                                                      //
////////////////////////////////////////////////////////////////////////////////

_AMX afxBool AfxIsMixDevicePrompt(afxMixDevice mdev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    return mdev->dev.serving;
}

_AMX afxUnit AfxCountMixPorts(afxMixDevice mdev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    return mdev->portCnt;
}

_AMX void AfxQueryMixDeviceLimits(afxMixDevice mdev, afxMixLimits* limits)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    AFX_ASSERT(limits);
    *limits = mdev->limits;
}

_AMX void AfxQueryMixDeviceFeatures(afxMixDevice mdev, afxMixFeatures* features)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    AFX_ASSERT(features);
    *features = mdev->features;
}

_AMX void AfxQueryMixPortCapabilities(afxMixDevice mdev, afxUnit portId, afxMixPortCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    AFX_ASSERT_RANGE(mdev->portCnt, portId, 1);
    AFX_ASSERT(caps);
    *caps = mdev->ports[portId].caps;
}

_AMX afxUnit AfxChooseMixPorts(afxMixDevice mdev, afxMixPortFlags caps, afxAcceleration accel, afxUnit maxCnt, afxUnit portId[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    afxUnit rslt = 0;

    for (afxUnit i = mdev->portCnt; i-- > 0;)
    {
        afxMixPortCaps const* port = &mdev->ports[i].caps;

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

_AMX afxError _AmxSdevDtorCb(afxMixDevice mdev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    AfxDeregisterChainedClasses(&mdev->dev.classes);

    AfxCallDevice(&mdev->dev, 2, NIL); // terminate

    AFX_ASSERT(!mdev->idd);
    AfxDeregisterChainedClasses(&mdev->dev.classes);

    afxObjectStash stashes[] =
    {
        {
            .cnt = mdev->portCnt,
            .siz = sizeof(mdev->ports[0]),
            .var = (void**)&mdev->ports
        }
    };
    AfxDeallocateInstanceData(mdev, ARRAY_SIZE(stashes), stashes);

    return err;
}

_AMX afxError _AmxSdevCtorCb(afxMixDevice mdev, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    afxMixDeviceInfo const* info = ((afxMixDeviceInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AfxDevBaseImplementation.ctor(&mdev->dev, (void*[]) { icd, (void*)&info->dev }, 0)) AfxThrowError();
    else
    {
        mdev->idd = NIL;

        mdev->limits = info->limits;
        mdev->features = info->features;

        mdev->portCnt = info->portCnt;

        afxObjectStash stashes[] =
        {
            {
                .cnt = mdev->portCnt,
                .siz = sizeof(mdev->ports[0]),
                .var = (void**)&mdev->ports
            }
        };

        if (AfxAllocateInstanceData(mdev, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
        else
        {
            if (!mdev->portCnt) AfxThrowError();
            else
            {
                AFX_ASSERT(mdev->ports);

                for (afxUnit i = 0; i < mdev->portCnt; i++)
                {
                    mdev->ports[i].caps = (afxMixPortCaps) { 0 };// info->portCaps[i];
                    AfxMakeString128(&mdev->ports[i].desc, NIL);
                    AfxMakeString8(&mdev->ports[i].urn, NIL);
                }

                if (AfxCallDevice(&mdev->dev, afxFcc_MSYS, NIL)) AfxThrowError();
                else
                {
                    if (err)
                    {
                        AfxDeregisterChainedClasses(&mdev->dev.classes);
                    }
                }
            }

            if (err)
                AfxDeallocateInstanceData(mdev, ARRAY_SIZE(stashes), stashes);
        }
    }
    return err;
}

_AMX afxClassConfig const _AMX_MDEV_CLASS_CONFIG =
{
    .fcc = afxFcc_MDEV,
    .name = "SoundDevice",
    .desc = "Sound Device Driver Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixDevice)),
    .ctor = (void*)_AmxSdevCtorCb,
    .dtor = (void*)_AmxSdevDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION DISCOVERY                                                   //
////////////////////////////////////////////////////////////////////////////////

_AMX afxUnit AfxInvokeSoundDevices(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxMixDevice), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetMixDeviceClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}

_AMX afxUnit AfxEvokeSoundDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxMixDevice), afxUnit cnt, afxMixDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetMixDeviceClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_AMX afxUnit AfxEnumerateSoundDevices(afxUnit icd, afxUnit first, afxUnit cnt, afxMixDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetMixDeviceClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_AMX afxUnit AfxEnumerateSoundSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxMixSystem systems[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetMixSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
        break;
    }
}

_AMX afxUnit AfxEvokeSoundSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxMixSystem), afxUnit cnt, afxMixSystem systems[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetMixSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)systems);
        break;
    }
    return rslt;
}

_AMX afxUnit AfxInvokeSoundSystems(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*,afxMixSystem), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetMixSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}
