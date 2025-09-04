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

#define _ASX_SIM_C
#define _ASX_SIMULATION_C
#include "impl/asxImplementation.h"

extern afxClassConfig const _ASX_SHAP_CLASS_CONFIG;

//extern afxChain* _AfxGetSystemClassChain(void);

// SIMULATION API //////////////////////////////////////////////////////////////

_ASX afxChain const* _AsxSimGetDagRoots(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return &sim->dags;
}

_ARX afxStringBase _ArxDinGetAnimUrnStringBase(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim->strbAnimMotUrns;
}

_ARX afxClass const* _ArxGetMotionClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->motCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MOT);
    return cls;
}

_ARX afxClass const* _ArxGetAnimationClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->aniCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ANI);
    return cls;
}

_ARX afxClass const* _ArxGetInstancedAnimationClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->anikCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ANIK);
    return cls;
}

_ARX afxClass const* _ArxGetCurveClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->curCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CUR);
    return cls;
}

_ARX afxClass const* _ArxGetBodyClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const*cls = &sim->bodCls;
    AFX_ASSERT_CLASS(cls, afxFcc_BOD);
    return cls;
}

_ARX afxClass const* _ArxGetCapstanClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const*cls = &sim->motoCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MOT);
    return cls;
}

_ARX afxClass const* _ArxGetMotiveClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->motvCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MOTV);
    return cls;
}

_ASX afxModule AfxGetSimulationIcd(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxModule icd = AfxGetProvider(sim);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX));
    return icd;
}

_ASX afxDrawSystem AfxGetSimulationDrawSystem(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim->dsys;
}

_ASX arxRenderware AfxGetSimulationDrawInput(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim->din;
}

_ASX afxClass const* _AsxGetMathBridgeClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->sexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SEXU);
    return cls;
}

_ASX afxClass const* _AsxGetShapClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->shapCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SHAP);
    return cls;
}

_ASX afxClass const* _AsxGetBufferClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->sbufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SBUF);
    return cls;
}

_ASX afxClass const* _AsxGetNodeClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const*cls = &sim->nodCls;
    AFX_ASSERT_CLASS(cls, afxFcc_NOD);
    return cls;
}

_ASX void AfxQuerySimulationErrorTolerance(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    if (allowedErr > 0.0)
    {
        afxReal s = sim->allowedLodErrFadingFactor * allowedErr;
        *allowedErrEnd = s;
        *allowedErrScaler = 1.0 / (allowedErr - s);
    }
    else
    {
        *allowedErrEnd = 0.0;
        *allowedErrScaler = 0.0;
    }
}

_ASX afxReal _AsxGetAllowedLodErrorFadingFactor(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim->allowedLodErrFadingFactor;
}

_ASX void AfxComputeSimilarity(afxSimulation sim, afxReal unitsPerMeter, afxV3d const right, afxV3d const up, afxV3d const back, afxV3d const origin, afxM3d ltm, afxM3d iltm, afxV3d atv)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(unitsPerMeter);
    AFX_ASSERT(origin);
    AFX_ASSERT(right);
    AFX_ASSERT(back);
    AFX_ASSERT(up);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(atv);

    afxM3d srcAxisSys;
    AfxM3dSetTransposed(srcAxisSys, right, up, back);

    AfxM3dMultiply(ltm, sim->basis, srcAxisSys);

    afxReal lambda = sim->unitsPerMeter / unitsPerMeter;
    AfxV3dScale(ltm[0], ltm[0], lambda);
    AfxV3dScale(ltm[1], ltm[1], lambda);
    AfxV3dScale(ltm[2], ltm[2], lambda);
    
    AfxM3dInvert(iltm, ltm);

    AfxV3dSub(atv, sim->origin, origin);
}

_ASX afxError AfxStepSimulation(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    //afxResult cnt = 0;

    afxSize stepTime = AfxGetTimer();
    afxReal deltaTime = sim->stepDeltaTime = AFX_R(stepTime - sim->stepTime) * 0.001f; // multiplying by 0.001f turns deltaTime in seconds
    sim->stepTime = stepTime;

    //AfxStepDag(sim->dag, AFX_R(deltaTime));
    //AfxNodeUpdate(AfxConnectionGetObject(&sim->dagRoot));

    if (1000 <= (stepTime - sim->swapTime))
    {
        sim->swapTime = stepTime;
        sim->stepsPerSecond = sim->stepNum;
        sim->stepNum = 0;
    }

    ++sim->stepNum;
    return err;
}

_ASX afxUnit AfxGetSimBridges(afxSimulation sim, afxUnit baseExuIdx, afxUnit cnt, afxSimBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    // bridges must be a valid pointer to afxSimBridge handles.
    AFX_ASSERT(bridges);
    afxUnit rslt = 0;

    afxUnit bridgeCnt = sim->bridgeCnt;
    AFX_ASSERT_RANGE(bridgeCnt, baseExuIdx, cnt);

    if (baseExuIdx < bridgeCnt)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxSimBridge sexu = sim->bridges[baseExuIdx + i];
            AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
            bridges[rslt++] = sexu;
        }
    }
    return rslt;
}

_ASX afxUnit AfxQuerySimBridges(afxSimulation sim, afxUnit sengId, afxUnit portId, afxUnit first, afxUnit cnt, afxSimBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxUnit found = 0;
    afxUnit rslt = 0;

    afxUnit bridgeCnt = sim->bridgeCnt;
    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        afxSimBridge sexu = sim->bridges[i];
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
        afxEngine seng;
        afxUnit portId2 = AfxQuerySimBridgePort(sexu, &seng);
        AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);

        if ((sengId != AFX_INVALID_INDEX) && (sengId != AfxGetObjectId(seng)))
            continue;

        if ((portId != AFX_INVALID_INDEX) && (portId != portId2))
            continue;

        if (cnt && (found >= first))
        {
            if (bridges)
            {
                bridges[rslt] = sexu;
            }

            if (cnt == rslt)
                break;
        }

        if (found >= first)
            rslt++;

        found++;
    }
    return rslt;
}

_ASX afxError AfxWaitForSimBridge(afxSimulation sim, afxUnit exuIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    // portIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)sim->bridgeCnt, exuIdx, 1);

    afxSimBridge sexu;
    while (!AfxGetSimBridges(sim, exuIdx, 1, &sexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    AfxWaitForIdleSimBridge(sexu, timeout);

    return err;
}

_ASX afxError AfxWaitForSimulation(afxSimulation sim, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    if (!sim->waitCb)
    {
        afxUnit bridgeCnt = sim->bridgeCnt;

        for (afxUnit i = 0; i < bridgeCnt; i++)
            AfxWaitForSimBridge(sim, i, timeout);
    }
    else if (sim->waitCb(sim, timeout))
        AfxThrowError();

    return err;
}

_ASX afxError AfxExecuteSampleCommands(afxSimulation sim, asxSubmission* ctrl, afxUnit cnt, afxContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(contexts);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(cnt);

    afxSimBridge sexu;
    if (!AfxGetSimBridges(sim, ctrl->portIdx, 1, &sexu))
    {
        AfxThrowError();
        return err;
    }

    afxClass const* dqueCls = _AsxGetSimQueueClass(sexu);

    // sanitize arguments
    afxUnit totalQueCnt = dqueCls->instCnt;
    afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AFX_CLAMP(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);
    afxBool queued = FALSE;

    while (1)
    {
        for (afxUnit i = 0; i < queCnt; i++)
        {
            afxUnit queIdx = baseQueIdx + i;

            afxSimQueue sque;
            if (!AfxGetSimQueues(sexu, queIdx, 1, &sque))
            {
                AfxThrowError();
                break;
            }

            afxError err2 = _AsxExecuteSampleCommands(sque, ctrl, 1, contexts);

            if (!err2)
            {
                queued = TRUE;
                break;
            }

            if (err2 == afxError_TIMEOUT)
                continue;

            AfxThrowError();
            break;
        }

        if (err || queued)
            break; // reiterate if not queue for timeout?
    }
    return err;
}

_ASX afxBool _AsxCaptureBodCb(arxBody bod, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    afxFrustum* frustum = udd[0];
    afxReal lodErr = *(afxReal*)udd[1];
    afxArray* pvs = udd[2];

    //AfxDoesFrustumCullAabbs(&frustum, 1, &aabb);

    if (1) // do visibility culling
    {
        afxUnit arrel;

        if (!AfxPushArrayUnits(pvs, 1, &arrel, &bod, sizeof(bod))) AfxThrowError();
        else
        {

        }
    }

    return TRUE;
}

_ASX afxError AfxCullBodies(afxSimulation sim, afxFrustum const* f, afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    // sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(pvs);
    AFX_ASSERT(pvs->cap);
    AFX_ASSERT(f);

    // TODO: Move to implementation

    afxReal lodErr = 0.f;
    AfxInvokeBodies(sim, 0, AFX_U32_MAX, (void*)_AsxCaptureBodCb, (void*[]) { f, &lodErr, pvs });

    return err;
}

_ASX afxError _AsxSimDtorCb(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    AfxDeregisterChainedClasses(&sim->classes);

    return err;
}

_ASX afxError _AsxSimCtorCb(afxSimulation sim, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _asxSimulationAcquisition const* cfg = AFX_CAST(_asxSimulationAcquisition const*, args[1]) + invokeNo;
    _asxSimBridgeAcquisition* bridgeCfgs = AFX_CAST(_asxSimBridgeAcquisition*, args[2]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    if (!bridgeCfgs)
    {
        AFX_ASSERT(bridgeCfgs);
        AfxThrowError();
        return err;
    }

    sim->running = FALSE;

    sim->requirements = cfg->requirements;

    // UNIVERSE

#if 0

    AFX_ASSERT(cfg->unitsPerMeter);
    AFX_ASSERT(!AfxV3dIsZero(cfg->right));
    AFX_ASSERT(!AfxV3dIsZero(cfg->up));
    AFX_ASSERT(!AfxV3dIsZero(cfg->back));

    AfxV3dCopy(sim->right, config->right);
    AfxV3dCopy(sim->up, config->up);
    AfxV3dCopy(sim->back, config->back);
#else
    AfxM3dReset(sim->basis);
    AfxV3dSet(sim->right, 1, 0, 0);
    AfxV3dSet(sim->up, 0, 1, 0);
    AfxV3dSet(sim->back, 0, 0, 1);
#endif
    AfxM3dSet(sim->basis, sim->right, sim->up, sim->back);

    AfxCopyBoxes(1, &cfg->extent, 0, &sim->extent, 0);
    AfxV3dCopy(sim->origin, cfg->origin);

    if (!cfg->unitsPerMeter)
        sim->unitsPerMeter = 1.f;
    else
        sim->unitsPerMeter = cfg->unitsPerMeter;

    if (!cfg->allowedLodErrFadingFactor)
        sim->allowedLodErrFadingFactor = 0.80000001;
    else
        sim->allowedLodErrFadingFactor = cfg->allowedLodErrFadingFactor;

    AfxDeployChain(&sim->dags, sim);

    // OBJECTS

    {
        afxChain* classes = &sim->classes;
        AfxDeployChain(classes, (void*)sim);

        afxClassConfig clsCfg;

        clsCfg = cfg->sbufClsCfg ? *cfg->sbufClsCfg : _ASX_SBUF_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SBUF);
        AfxMountClass(&sim->sbufCls, NIL, classes, &clsCfg);

        clsCfg = /*cfg->shapCls ? *cfg->shapCls :*/ _ASX_SHAP_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SHAP);
        AfxMountClass(&sim->shapCls, NIL, classes, &clsCfg);

        clsCfg = _ASX_NOD_CLASS_CONFIG;
        AfxMountClass(&sim->nodCls, NIL, classes, &clsCfg);

        clsCfg = cfg->curClsCfg ? *cfg->curClsCfg : _ARX_CUR_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_CUR);
        AfxMountClass(&sim->curCls, NIL, classes, &clsCfg);

        clsCfg = cfg->motClsCfg ? *cfg->motClsCfg : _ARX_MOT_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MOT);
        AfxMountClass(&sim->motCls, NIL, classes, &clsCfg);

        clsCfg = cfg->aniClsCfg ? *cfg->aniClsCfg : _ARX_ANI_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_ANI);
        AfxMountClass(&sim->aniCls, NIL, classes, &clsCfg); // require mot

        clsCfg = cfg->anikClsCfg ? *cfg->anikClsCfg : (afxClassConfig) { 0 };// _ARX_ANIK_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_ANIK);
        AfxMountClass(&sim->anikCls, NIL, classes, &clsCfg); // require mot

        clsCfg = cfg->capsClsCfg ? *cfg->capsClsCfg : _ARX_MOTO_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MOTO);
        AfxMountClass(&sim->motoCls, NIL, classes, &clsCfg);

        clsCfg = cfg->motvClsCfg ? *cfg->motvClsCfg : (afxClassConfig) { 0 };//_ARX_MOTV_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MOTV);
        AfxMountClass(&sim->motvCls, NIL, classes, &clsCfg);

        clsCfg = cfg->bodClsCfg ? *cfg->bodClsCfg : _ARX_BOD_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_BOD);
        AfxMountClass(&sim->bodCls, NIL, classes, &clsCfg); // require moto

        clsCfg = cfg->sexuClsCfg ? *cfg->sexuClsCfg : _ASX_SEXU_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SEXU);
        AfxMountClass(&sim->sexuCls, NIL, classes, &clsCfg); // require all

        AfxAcquireStringCatalogs(1, &sim->strbAnimMotUrns);

    }

    sim->dsys = cfg->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &sim->dsys);

    sim->din = cfg->din;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &sim->din);

    {
        sim->stepNum = 0;
        sim->stepsPerSecond = 0;
        sim->stepTime = 0;

    }


    afxUnit totalDqueCnt = 0;
    afxUnit bridgeCnt = cfg->bridgeCnt;

    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        AFX_ASSERT(bridgeCfgs);
        totalDqueCnt += bridgeCfgs[i].minQueCnt;
    }

    sim->bridgeCnt = bridgeCnt;
    afxUnit baseQueIdx = 0;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(sim->bridges[0]),
            .var = (void**)&sim->bridges
        }
    };

    if (AfxAllocateInstanceData(sim, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&sim->classes);
        AFX_ASSERT(AfxIsChainEmpty(&sim->classes));
        return err;
    }

    afxClass* cls = (afxClass*)_AsxGetMathBridgeClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_SEXU);

    if (AfxAcquireObjects(cls, sim->bridgeCnt, (afxObject*)sim->bridges, (void const*[]) { sim, bridgeCfgs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, sim->bridgeCnt, sim->bridges);

        afxEngine seng = AfxGetMathBridgeDevice(sim->bridges[0]);

        AfxCallDevice((afxDevice)seng, 3, sim);
        AfxCallDevice((afxDevice)seng, 5, sim);

        if (AfxDoDeviceService((afxDevice)seng)) AfxThrowError(); // let the device build its SPUs.
        else
        {
            sim->running = TRUE;
        }
    }

    if (err)
    {
        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&sim->classes);
        AFX_ASSERT(AfxIsChainEmpty(&sim->classes));

        AfxDeallocateInstanceData(sim, ARRAY_SIZE(stashes), stashes);
    }
    return err;
}

_ASX afxClassConfig const _ASX_SIM_CLASS_CONFIG =
{
    .fcc = afxFcc_SIM,
    .name = "Simulation",
    .desc = "Simulation",
    .fixedSiz = sizeof(AFX_OBJECT(afxSimulation)),
    .ctor = (void*)_AsxSimCtorCb,
    .dtor = (void*)_AsxSimDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxConfigureSimulation(afxUnit icd, afxSimulationConfig* cfg)
{
    afxError err = AFX_ERR_NONE;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    *cfg = (afxSimulationConfig const) { 0 };

    afxModule driver;
    if (!_AsxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_ASX));

    afxEngine seng;
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    if (!AfxEnumerateEngines(icd, 0, 1, &seng))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);

    AfxV3dSet(cfg->right, 1, 0, 0);
    AfxV3dSet(cfg->up, 0, 1, 0);
    AfxV3dSet(cfg->back, 0, 0, 1);
    AfxV3dZero(cfg->origin);
    AfxMakeAabb(&cfg->extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    cfg->unitsPerMeter = 1.f;
    cfg->allowedLodErrFadingFactor = 0.80000001;

    cfg->prime.minQueCnt = _ASX_SQUE_CLASS_CONFIG.unitsPerPage;

    return err;
}

_ASX afxError AfxEstablishSimulation(afxUnit icd, afxSimulationConfig const* cfg, afxSimulation* simulation)
{
    afxError err = AFX_ERR_NONE;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    afxModule driver;
    if (!_AsxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_ASX));

    afxEngine seng;
    AFX_ASSERT(cfg->prime.engId != AFX_INVALID_INDEX);
    if (!AfxEnumerateEngines(icd, cfg->prime.engId, 1, &seng))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);

    if (AfxCallDevice((afxDevice)seng, 1, NIL))
    {
        AfxThrowError(); // let the device build its DPUs.
        return err;
    }

    // Acquire bridges and queues
    afxUnit portId = AFX_INVALID_INDEX;
    afxUnit totalSqueCnt = 0;
    afxUnit baseQueIdx[ASX_MAX_SIM_BRIDGE_PER_CONTEXT] = { 0 };
    _asxSimBridgeAcquisition bridgeCfg[ASX_MAX_SIM_BRIDGE_PER_CONTEXT] = { 0 };
    afxUnit bridgeCnt = 0;

    if (!AfxEnumerateEngines(icd, cfg->prime.engId, 1, &seng))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
    bridgeCfg[bridgeCnt].seng = seng;
    if (!AfxChooseSimPorts(seng, cfg->prime.capabilities, cfg->prime.acceleration, 1, &portId))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT(portId != AFX_INVALID_INDEX);
    bridgeCfg[bridgeCnt].portId = portId;
    bridgeCfg[bridgeCnt].exuIdx = 0;
    bridgeCfg[bridgeCnt].minQueCnt = AFX_CLAMP(cfg->prime.minQueCnt, 1, ASX_MAX_SIM_QUEUE_PER_BRIDGE);
    ++bridgeCnt;

    if (cfg->auxCnt && cfg->auxs)
    {
        AFX_ASSERT(15 >= cfg->auxCnt);

        for (afxUnit i = 0; i < cfg->auxCnt; i++)
        {
            afxUnit bridgeIdx = AFX_INVALID_INDEX;
#if 0 // allowed multibridge to same port
            for (afxUnit j = 0; j < bridgeCnt; j++)
            {
                if (cfg->auxs[j].portId == bridgeCfg[j].portId)
                {
                    bridgeCfg[bridgeIdx].minQueCnt += cfg->auxs[i].minQueCnt;
                    bridgeCfg[bridgeIdx].queuePriority = NIL;
                    //bridgeCfg[bridgeIdx].flags |= cfg->auxBridges[i].flags;

                    baseQueIdx[bridgeIdx] = totalSqueCnt;
                    totalSqueCnt += bridgeCfg[bridgeIdx].minQueCnt;
                    break;
                }
            }
#endif
            if (bridgeIdx == AFX_INVALID_INDEX)
            {
                if (!AfxEnumerateEngines(icd, cfg->auxs[i].engId, 1, &seng))
                {
                    AfxThrowError();
                    return err;
                }
                AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
                bridgeCfg[bridgeCnt].seng = seng;
                if (!AfxChooseSimPorts(seng, cfg->auxs[i].capabilities, cfg->auxs[i].acceleration, 1, &portId))
                {
                    AfxThrowError();
                    return err;
                }
                AFX_ASSERT(portId != AFX_INVALID_INDEX);
                bridgeCfg[bridgeCnt].portId = portId;
                bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
                bridgeCfg[bridgeCnt].minQueCnt = AFX_CLAMP(cfg->auxs[i].minQueCnt, 1, ASX_MAX_SIM_QUEUE_PER_BRIDGE);
                ++bridgeCnt;
            }
        }
    }

    _asxSimulationAcquisition cfg2 = { 0 };
    cfg2.bridgeCnt = bridgeCnt;
    cfg2.extensionCnt = cfg->extensionCnt;
    cfg2.extensions = cfg->extensions;
    cfg2.requirements = cfg->requirements;
    cfg2.udd = cfg->udd;

    cfg2.din = cfg->din;
    cfg2.dsys = cfg->dsys;

    cfg2.allowedLodErrFadingFactor = cfg->allowedLodErrFadingFactor;
    cfg2.unitsPerMeter = cfg->unitsPerMeter;

    AfxV3dCopy(cfg2.right, cfg->right);
    AfxV3dCopy(cfg2.up, cfg->up);
    AfxV3dCopy(cfg2.back, cfg->back);
    AfxV3dCopy(cfg2.origin, cfg->origin);
    AfxCopyBoxes(1, &cfg->extent, 0, &cfg2.extent, 0);

    afxClass* cls = (afxClass*)_AsxGetSimulationClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_SIM);

    afxSimulation sim;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&sim, (void const*[]) { driver, &cfg2, &bridgeCfg[0], }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(simulation);
    *simulation = sim;

    return err;
}
