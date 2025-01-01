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
#define _AMX_ENGINE_C
#define _AMX_SIMULATION_C
#include "impl/amxImplementation.h"


////////////////////////////////////////////////////////////////////////////////
// MATH DEVICE HANDLING                                                       //
////////////////////////////////////////////////////////////////////////////////

_AMX afxUnit AfxCountEnginePorts(afxEngine mdev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    return mdev->portCnt;
}

_AMX afxBool AfxIsEnginePrompt(afxEngine mdev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    return mdev->dev.serving;
}

_AMX afxUnit AfxChooseSimPorts(afxEngine mdev, afxSimPortFlags caps, afxAcceleration accel, afxUnit maxCnt, afxUnit portId[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    afxUnit rslt = 0;

    for (afxUnit i = mdev->portCnt; i-- > 0;)
    {
        afxSimPortCaps const* port = &mdev->ports[i].caps;

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

_AMX afxError _AmxMdevDtorCb(afxEngine mdev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    if (mdev->stopCb && mdev->stopCb(mdev))
        AfxThrowError();

    AFX_ASSERT(!mdev->idd);
    AfxDeregisterChainedClasses(&mdev->dev.classes);

    AfxDismantleMutex(&mdev->relinkedCndMtx);
    AfxDismantleCondition(&mdev->relinkedCnd);

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

_AMX afxError _AmxMdevCtorCb(afxEngine mdev, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    afxEngineInfo const* info = ((afxEngineInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AfxDevBaseImplementation.ctor(&mdev->dev, (void*[]) { icd, (void*)&info->dev }, 0))
    {
        AfxThrowError();
        return err;
    }

    AfxDeployCondition(&mdev->relinkedCnd);
    AfxDeployMutex(&mdev->relinkedCndMtx, AFX_MTX_PLAIN);

    mdev->idd = NIL;

    mdev->startCb = info->startCb;
    mdev->stopCb = info->stopCb;
    mdev->openSimCb = info->openSimCb;
    mdev->closeSimCb = info->closeSimCb;

    mdev->closeMdgeCb = info->closeMdgeCb;
    mdev->openMdgeCb = info->openMdgeCb;

    mdev->portCnt = info->portCnt;

    afxObjectStash stashes[] =
    {
        {
            .cnt = mdev->portCnt,
            .siz = sizeof(mdev->ports[0]),
            .var = (void**)&mdev->ports
        }
    };

    if (AfxAllocateInstanceData(mdev, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
    }
    else
    {
        if (!mdev->portCnt) AfxThrowError();
        else
        {
            AFX_ASSERT(mdev->ports);

            for (afxUnit i = 0; i < mdev->portCnt; i++)
            {
                mdev->ports[i].caps = info->portCaps[i];
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
    return err;
}

_AMX afxClassConfig const _AMX_MDEV_CLASS_CONFIG =
{
    .fcc = afxFcc_MDEV,
    .name = "Engine",
    .desc = "Compute Engine Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxEngine)),
    .ctor = (void*)_AmxMdevCtorCb,
    .dtor = (void*)_AmxMdevDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION DISCOVERY                                                   //
////////////////////////////////////////////////////////////////////////////////

_AMX afxUnit AfxInvokeEngines(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*,afxEngine), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetEngineClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}

_AMX afxUnit AfxEvokeEngines(afxUnit icd, afxUnit first, void* udd, afxBool(*flt)(void*,afxEngine), afxUnit cnt, afxEngine devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetEngineClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
        rslt = AfxEvokeObjects(cls, (void*)flt, udd, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_AMX afxUnit AfxEnumerateEngines(afxUnit icd, afxUnit first, afxUnit cnt, afxEngine devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetEngineClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_AMX afxUnit AfxInvokeSimulations(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxSimulation), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetSimulationClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SIM);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}

_AMX afxUnit AfxEvokeSimulations(afxUnit icd, afxUnit first, void* udd, afxBool(*flt)(void*, afxSimulation), afxUnit cnt, afxSimulation simulations[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(simulations);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetSimulationClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SIM);
        rslt = AfxEvokeObjects(cls, (void*)flt, udd, first, cnt, (afxObject*)simulations);
        break;
    }
    return rslt;
}

_AMX afxUnit AfxEnumerateSimulations(afxUnit icd, afxUnit first, afxUnit cnt, afxSimulation simulations[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(simulations);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (AmxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
        afxClass const* cls = _AmxGetSimulationClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SIM);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)simulations);
        break;
    }
    return rslt;
}
