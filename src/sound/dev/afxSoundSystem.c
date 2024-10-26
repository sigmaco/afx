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
#define _ASX_SOUND_OUTPUT_C
#define _ASX_SOUND_SYSTEM_C
#define _ASX_SOUND_CONTEXT_C
#include "../../dev/AsxImplKit.h"

extern afxClassConfig const _AsxSoutStdImplementation;
extern afxClassConfig const _AsxSinStdImplementation;
AFX afxChain* _AfxGetSystemClassChain(void);

_ASX afxClass const* AfxGetSoundDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &sys->asx.sdevCls;
    AfxAssertClass(cls, afxFcc_SDEV);
    return cls;
}

////////////////////////////////////////////////////////////////////////////////
// SOUND DEVICE HANDLING                                                      //
////////////////////////////////////////////////////////////////////////////////

_ASX afxClass const* AfxGetSoundBridgeClass(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    /// sdev must be a valid afxSoundDevice handle.
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxClass const* cls = &sdev->sdgeCls;
    AfxAssertClass(cls, afxFcc_SDGE);
    return cls;
}

_ASX afxClass const* AfxGetSoundContextClass(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    /// sdev must be a valid afxSoundDevice handle.
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxClass const* cls = &sdev->sctxCls;
    AfxAssertClass(cls, afxFcc_SCTX);
    return cls;
}

_ASX afxClass const* AfxGetSoundOutputClass(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    /// sdev must be a valid afxSoundDevice handle.
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxClass const* cls = &sdev->soutCls;
    AfxAssertClass(cls, afxFcc_SOUT);
    return cls;
}

_ASX afxClass const* AfxGetSoundInputClass(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    /// sdev must be a valid afxSoundDevice handle.
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxClass const* cls = &sdev->sinCls;
    AfxAssertClass(cls, afxFcc_SIN);
    return cls;
}

_ASX afxBool AfxIsSoundDevicePrompt(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev->dev.serving;
}

_ASX afxUnit AfxCountSoundPorts(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev->portCnt;
}

_ASX afxError _AsxSdevDtorCb(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    AFX_ASSERT(sdev->stopCb);

    if (sdev->stopCb(sdev))
        AfxThrowError();

    AFX_ASSERT(!sdev->idd);
    AfxDeregisterChainedClasses(&sdev->dev.classes);

    AfxCleanUpMutex(&sdev->relinkedCndMtx);
    AfxCleanUpCondition(&sdev->relinkedCnd);

    afxObjectStash stashes[] =
    {
        {
            .cnt = sdev->portCnt,
            .siz = sizeof(sdev->portCaps[0]),
            .var = (void**)&sdev->portCaps
        }
    };
    AfxDeallocateInstanceData(sdev, ARRAY_SIZE(stashes), stashes);

    return err;
}

_ASX afxError _AsxSdevCtorCb(afxSoundDevice sdev, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;

    afxModule icd = args[0];
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
    afxSoundDeviceInfo const* info = ((afxSoundDeviceInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AfxDevBaseImplementation.ctor(&sdev->dev, (void*[]) { icd, (void*)&info->dev }, 0)) AfxThrowError();
    else
    {
        AfxSetUpCondition(&sdev->relinkedCnd);
        AfxSetUpMutex(&sdev->relinkedCndMtx, AFX_MTX_PLAIN);

        sdev->idd = NIL;

        sdev->startCb = info->startCb;
        sdev->stopCb = info->stopCb;
        sdev->openCb = info->openCb;
        sdev->closeCb = info->closeCb;

        sdev->closeSdgeCb = info->closeSdgeCb;
        sdev->openSdgeCb = info->openSdgeCb;

        sdev->openSoutCb = info->openSoutCb;
        sdev->closeSoutCb = info->closeSoutCb;
        sdev->relinkSoutCb = info->relinkSoutCb;

        sdev->openSinCb = info->openSinCb;
        sdev->closeSinCb = info->closeSinCb;
        sdev->relinkSinCb = info->relinkSinCb;

        sdev->portCnt = info->portCnt;

        afxClassConfig clsCfg;
        clsCfg = info->sdgeClsCfg ? *info->sdgeClsCfg : _AsxSdgeStdImplementation;
        AfxRegisterClass(&sdev->sdgeCls, NIL, &sdev->dev.classes, &clsCfg);

        clsCfg = info->sctxClsCfg ? *info->sctxClsCfg : _AsxSctxStdImplementation;
        AfxRegisterClass(&sdev->sctxCls, NIL, &sdev->dev.classes, &clsCfg);

        clsCfg = info->soutClsCfg ? *info->soutClsCfg : _AsxSoutStdImplementation;
        AfxRegisterClass(&sdev->soutCls, NIL, &sdev->dev.classes, &clsCfg); // require sdev, sout

        clsCfg = info->sinClsCfg ? *info->sinClsCfg : _AsxSinStdImplementation;
        AfxRegisterClass(&sdev->sinCls, NIL, &sdev->dev.classes, &clsCfg); // require sdev, sin

        afxObjectStash stashes[] =
        {
            {
                .cnt = sdev->portCnt,
                .siz = sizeof(sdev->portCaps[0]),
                .var = (void**)&sdev->portCaps
            }
        };

        if (AfxAllocateInstanceData(sdev, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
        else
        {
            if (!sdev->portCnt) AfxThrowError();
            else
            {
                AFX_ASSERT(sdev->portCaps);

                for (afxUnit i = 0; i < sdev->portCnt; i++)
                {
                    sdev->portCaps[i] = info->portCaps[i];
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

////////////////////////////////////////////////////////////////////////////////
// SOUND DEVICE DISCOVERY                                                     //
////////////////////////////////////////////////////////////////////////////////

_ASX afxUnit AfxInvokeSoundDevices(afxUnit first, afxUnit cnt, afxBool(*f)(afxSoundDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass const* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_ASX afxUnit AfxEvokeSoundDevices(afxBool(*flt)(afxSoundDevice, void*), void* fdd, afxUnit first, afxUnit cnt, afxSoundDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(devices);
    afxClass const* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)devices);
}

_ASX afxUnit AfxEnumerateSoundDevices(afxUnit first, afxUnit cnt, afxSoundDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(devices);
    afxClass const* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)devices);
}

_ASX afxBool AfxGetSoundDevice(afxUnit sdevId, afxSoundDevice* device)
{
    afxError err = AFX_ERR_NONE;
    afxSoundDevice sdev = NIL;    
    while (AfxEnumerateSoundDevices(sdevId, 1, &sdev))
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        AFX_ASSERT(device);
        *device = sdev;
        break;
    }
    return !!sdev;
}

_ASX afxError _AsxRegisterSoundDevices(afxModule icd, afxUnit cnt, afxSoundDeviceInfo const infos[], afxSoundDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
    AFX_ASSERT(devices);

    afxClass* cls = (afxClass*)AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);

    AfxEnterSlockExclusive(&cls->poolLock);

    if (cls->pool.totalUsedCnt >= cls->maxInstCnt) AfxThrowError();
    else
    {
        if (_AfxAllocateObjects(cls, cnt, (afxObject*)devices)) AfxThrowError();
        else
        {
            if (_AfxConstructObjects(cls, cnt, (afxObject*)devices, (void*[]) { icd, (void*)infos }))
            {
                AfxThrowError();
            }

            if (!err)
            {
                AfxAssertObjects(cnt, devices, afxFcc_SDEV);
            }
            else
            {
                _AfxDeallocateObjects(cls, cnt, (afxObject*)devices);
            }
        }
    }

    AfxExitSlockExclusive(&cls->poolLock);

    return err;
}

_ASX afxError afxScmHook(afxModule mdle, afxManifest const* ini)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    afxClassConfig const _AsxSdevClsCfg =
    {
        .fcc = afxFcc_SDEV,
        .name = "SoundDevice",
        .desc = "Sound Device Driver Interface",
        .fixedSiz = sizeof(AFX_OBJECT(afxSoundDevice)),
        .ctor = (void*)_AsxSdevCtorCb,
        .dtor = (void*)_AsxSdevDtorCb
    };

    AfxRegisterClass(&sys->asx.sdevCls, (afxClass*)AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_AsxSdevClsCfg); // require base*

    if (!err)
    {
        //sys->asx.ready = TRUE;
    }

    return err;
}
