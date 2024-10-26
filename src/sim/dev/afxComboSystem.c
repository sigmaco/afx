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

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AFX_SIM_C
#define _AMX_MATH_DEVICE_C
#define _AMX_SIMULATION_C
#include "../../dev/afxExecImplKit.h"
#include "../../dev/AmxImplKit.h"

AFX afxChain* _AfxGetSystemClassChain(void);

_AMX afxClass const* AfxGetComboDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &sys->amx.mdevCls;
    AfxAssertClass(cls, afxFcc_MDEV);
    return cls;
}

////////////////////////////////////////////////////////////////////////////////
// MATH DEVICE HANDLING                                                       //
////////////////////////////////////////////////////////////////////////////////

_AMX afxClass const* AfxGetMathBridgeClass(afxComboDevice mdev)
{
    afxError err = AFX_ERR_NONE;
    /// mdev must be a valid afxComboDevice handle.
    AfxAssertObjects(1, &mdev, afxFcc_MDEV);
    afxClass const* cls = &mdev->mdgeCls;
    AfxAssertClass(cls, afxFcc_MDGE);
    return cls;
}

_AMX afxClass const* AfxGetSimulationClass(afxComboDevice mdev)
{
    afxError err = AFX_ERR_NONE;
    /// mdev must be a valid afxComboDevice handle.
    AfxAssertObjects(1, &mdev, afxFcc_MDEV);
    afxClass const* cls = &mdev->simCls;
    AfxAssertClass(cls, afxFcc_SIM);
    return cls;
}

_AMX afxUnit AfxCountPragmaPorts(afxComboDevice mdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdev, afxFcc_MDEV);
    return mdev->portCnt;
}

_AMX afxBool AfxIsComboDevicePrompt(afxComboDevice mdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdev, afxFcc_MDEV);
    return mdev->dev.serving;
}

_AMX afxError _AmxMdevDtorCb(afxComboDevice mdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdev, afxFcc_MDEV);

    if (mdev->stopCb && mdev->stopCb(mdev))
        AfxThrowError();

    AFX_ASSERT(!mdev->idd);
    AfxDeregisterChainedClasses(&mdev->dev.classes);

    AfxCleanUpMutex(&mdev->relinkedCndMtx);
    AfxCleanUpCondition(&mdev->relinkedCnd);

    afxObjectStash stashes[] =
    {
        {
            .cnt = mdev->portCnt,
            .siz = sizeof(mdev->portCaps[0]),
            .var = (void**)&mdev->portCaps
        }
    };
    AfxDeallocateInstanceData(mdev, ARRAY_SIZE(stashes), stashes);

    return err;
}

_AMX afxError _AmxMdevCtorCb(afxComboDevice mdev, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdev, afxFcc_MDEV);

    afxModule icd = args[0];
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
    afxComboDeviceInfo const* info = ((afxComboDeviceInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AfxDevBaseImplementation.ctor(&mdev->dev, (void*[]) { icd, (void*)&info->dev }, 0)) AfxThrowError();
    else
    {
        AfxSetUpCondition(&mdev->relinkedCnd);
        AfxSetUpMutex(&mdev->relinkedCndMtx, AFX_MTX_PLAIN);

        mdev->idd = NIL;

        mdev->startCb = info->startCb;
        mdev->stopCb = info->stopCb;
        mdev->openSimCb = info->openSimCb;
        mdev->closeSimCb = info->closeSimCb;

        mdev->closeMdgeCb = info->closeMdgeCb;
        mdev->openMdgeCb = info->openMdgeCb;

        mdev->portCnt = info->portCnt;

        afxClassConfig clsCfg;
        clsCfg = info->mdgeClsCfg ? *info->mdgeClsCfg : _AmxMdgeStdImplementation;
        AfxRegisterClass(&mdev->mdgeCls, NIL, &mdev->dev.classes, &clsCfg);

        clsCfg = info->simClsCfg ? *info->simClsCfg : _AmxSimStdImplementation;
        AfxRegisterClass(&mdev->simCls, NIL, &mdev->dev.classes, &clsCfg);

        afxObjectStash stashes[] =
        {
            {
                .cnt = mdev->portCnt,
                .siz = sizeof(mdev->portCaps[0]),
                .var = (void**)&mdev->portCaps
            }
        };

        if (AfxAllocateInstanceData(mdev, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
        else
        {
            if (!mdev->portCnt) AfxThrowError();
            else
            {
                AFX_ASSERT(mdev->portCaps);

                for (afxUnit i = 0; i < mdev->portCnt; i++)
                {
                    mdev->portCaps[i] = info->portCaps[i];
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

////////////////////////////////////////////////////////////////////////////////
// MATH DEVICE DISCOVERY                                                      //
////////////////////////////////////////////////////////////////////////////////

_AMX afxUnit AfxInvokeComboDevices(afxUnit first, afxUnit cnt, afxBool(*f)(afxComboDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass const* cls = AfxGetComboDeviceClass();
    AfxAssertClass(cls, afxFcc_MDEV);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AMX afxUnit AfxEvokeComboDevices(afxBool(*flt)(afxComboDevice, void*), void* fdd, afxUnit first, afxUnit cnt, afxComboDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(devices);
    afxClass const* cls = AfxGetComboDeviceClass();
    AfxAssertClass(cls, afxFcc_MDEV);
    return AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)devices);
}

_AMX afxUnit AfxEnumerateComboDevices(afxUnit first, afxUnit cnt, afxComboDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(devices);
    afxClass const* cls = AfxGetComboDeviceClass();
    AfxAssertClass(cls, afxFcc_MDEV);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)devices);
}

_AMX afxBool AfxGetComboDevice(afxUnit mdevId, afxComboDevice* device)
{
    afxError err = AFX_ERR_NONE;
    afxComboDevice mdev = NIL;
    while (AfxEnumerateComboDevices(mdevId, 1, &mdev))
    {
        AfxAssertObjects(1, &mdev, afxFcc_MDEV);
        AFX_ASSERT(device);
        *device = mdev;
        break;
    }
    return !!mdev;
}

_AMX afxError _AmxRegisterComboDevices(afxModule icd, afxUnit cnt, afxComboDeviceInfo const infos[], afxComboDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
    AFX_ASSERT(devices);

    afxClass* cls = (afxClass*)AfxGetComboDeviceClass();
    AfxAssertClass(cls, afxFcc_MDEV);

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
                AfxAssertObjects(cnt, devices, afxFcc_MDEV);
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

_AMX afxError afxScmHook(afxModule mdle, afxManifest const* ini)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    afxClassConfig const _AmxMdevClsCfg =
    {
        .fcc = afxFcc_MDEV,
        .name = "ComboDevice",
        .desc = "Math Device Driver Interface",
        .fixedSiz = sizeof(AFX_OBJECT(afxComboDevice)),
        .ctor = (void*)_AmxMdevCtorCb,
        .dtor = (void*)_AmxMdevDtorCb
    };

    AfxRegisterClass(&sys->amx.mdevCls, (afxClass*)AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_AmxMdevClsCfg); // require base*

    if (!err)
    {
        //sys->asx.ready = TRUE;
    }

    return err;
}
