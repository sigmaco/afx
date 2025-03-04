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
#define _ASX_ENGINE_C
#define _ASX_SIMULATION_C
#include "impl/asxImplementation.h"


////////////////////////////////////////////////////////////////////////////////
// MATH DEVICE HANDLING                                                       //
////////////////////////////////////////////////////////////////////////////////

_ASX afxUnit AfxCountEnginePorts(afxEngine seng)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
    return seng->portCnt;
}

_ASX afxBool AfxIsEnginePrompt(afxEngine seng)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
    return seng->dev.serving;
}

_ASX afxUnit AfxChooseSimPorts(afxEngine seng, afxSimPortFlags caps, afxAcceleration accel, afxUnit maxCnt, afxUnit portId[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
    afxUnit rslt = 0;

    for (afxUnit i = seng->portCnt; i-- > 0;)
    {
        afxSimPortCaps const* port = &seng->ports[i].caps;

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

_ASX afxError _AsxMdevDtorCb(afxEngine seng)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);

    if (seng->stopCb && seng->stopCb(seng))
        AfxThrowError();

    AFX_ASSERT(!seng->idd);
    AfxDeregisterChainedClasses(&seng->dev.classes);

    AfxDismantleMutex(&seng->relinkedCndMtx);
    AfxDismantleCondition(&seng->relinkedCnd);

    afxObjectStash stashes[] =
    {
        {
            .cnt = seng->portCnt,
            .siz = sizeof(seng->ports[0]),
            .var = (void**)&seng->ports
        }
    };
    AfxDeallocateInstanceData(seng, ARRAY_SIZE(stashes), stashes);

    return err;
}

_ASX afxError _AsxMdevCtorCb(afxEngine seng, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    afxEngineInfo const* info = ((afxEngineInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AFX_DEV_CLASS_CONFIG.ctor(&seng->dev, (void*[]) { icd, (void*)&info->dev }, 0))
    {
        AfxThrowError();
        return err;
    }

    AfxDeployCondition(&seng->relinkedCnd);
    AfxDeployMutex(&seng->relinkedCndMtx, AFX_MTX_PLAIN);

    seng->idd = NIL;

    seng->startCb = info->startCb;
    seng->stopCb = info->stopCb;
    seng->openSimCb = info->openSimCb;
    seng->closeSimCb = info->closeSimCb;

    seng->closeMdgeCb = info->closeMdgeCb;
    seng->openMdgeCb = info->openMdgeCb;

    seng->portCnt = info->portCnt;

    afxObjectStash stashes[] =
    {
        {
            .cnt = seng->portCnt,
            .siz = sizeof(seng->ports[0]),
            .var = (void**)&seng->ports
        }
    };

    if (AfxAllocateInstanceData(seng, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
    }
    else
    {
        if (!seng->portCnt) AfxThrowError();
        else
        {
            AFX_ASSERT(seng->ports);

            for (afxUnit i = 0; i < seng->portCnt; i++)
            {
                seng->ports[i].caps = info->portCaps[i];
            }

            if (AfxCallDevice(&seng->dev, afxFcc_MSYS, NIL)) AfxThrowError();
            else
            {
                if (err)
                {
                    AfxDeregisterChainedClasses(&seng->dev.classes);
                }
            }
        }

        if (err)
            AfxDeallocateInstanceData(seng, ARRAY_SIZE(stashes), stashes);
    }
    return err;
}

_ASX afxClassConfig const _ASX_SDEV_CLASS_CONFIG =
{
    .fcc = afxFcc_SDEV,
    .name = "Engine",
    .desc = "Compute Engine Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxEngine)),
    .ctor = (void*)_AsxMdevCtorCb,
    .dtor = (void*)_AsxMdevDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION DISCOVERY                                                   //
////////////////////////////////////////////////////////////////////////////////

_ASX afxUnit AfxInvokeEngines(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*,afxEngine), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetEngineClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}

_ASX afxUnit AfxEvokeEngines(afxUnit icd, afxUnit first, void* udd, afxBool(*flt)(void*,afxEngine), afxUnit cnt, afxEngine devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetEngineClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
        rslt = AfxEvokeObjects(cls, (void*)flt, udd, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_ASX afxUnit AfxEnumerateEngines(afxUnit icd, afxUnit first, afxUnit cnt, afxEngine devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetEngineClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_ASX afxUnit AfxInvokeSimulations(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxSimulation), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSimulationClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SIM);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}

_ASX afxUnit AfxEvokeSimulations(afxUnit icd, afxUnit first, void* udd, afxBool(*flt)(void*, afxSimulation), afxUnit cnt, afxSimulation simulations[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(simulations);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSimulationClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SIM);
        rslt = AfxEvokeObjects(cls, (void*)flt, udd, first, cnt, (afxObject*)simulations);
        break;
    }
    return rslt;
}

_ASX afxUnit AfxEnumerateSimulations(afxUnit icd, afxUnit first, afxUnit cnt, afxSimulation simulations[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(simulations);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSimulationClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SIM);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)simulations);
        break;
    }
    return rslt;
}
