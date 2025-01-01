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

#define _AMX_SIM_C
#define _AMX_SIMULATION_C
#include "impl/amxImplementation.h"

//extern afxChain* _AfxGetSystemClassChain(void);

// SIMULATION API //////////////////////////////////////////////////////////////

_AMX afxModule AfxGetSimulationIcd(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxModule icd = AfxGetProvider(sim);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX));
    return icd;
}

_AMX afxDrawSystem AfxGetSimulationDrawSystem(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim->dsys;
}

_AMX afxDrawInput AfxGetSimulationDrawInput(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim->din;
}

_AMX afxClass const* _AmxGetMathBridgeClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->mexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MEXU);
    return cls;
}

_AMX afxClass const* _AmxGetMotionClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->motCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MOT);
    return cls;
}

_AMX afxClass const* _AmxGetAnimationClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->aniCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ANI);
    return cls;
}

_AMX afxClass const* _AmxGetInstancedAnimationClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->anikCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ANIK);
    return cls;
}

_AMX afxClass const* _AmxGetPoseClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->poseCls;
    AFX_ASSERT_CLASS(cls, afxFcc_POSE);
    return cls;
}

_AMX afxClass const* _AmxGetPlacementClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->plceCls;
    AFX_ASSERT_CLASS(cls, afxFcc_PLCE);
    return cls;
}

_AMX afxClass const* _AmxGetCurveClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->curCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CUR);
    return cls;
}

_AMX afxClass const* _AmxGetMaterialClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->mtlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);
    return cls;
}

_AMX afxClass const* _AmxGetModelClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->mdlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);
    return cls;
}

_AMX afxClass* _AmxGetRendererClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass *class = &sim->renderers;
    AFX_ASSERT_CLASS(class, afxFcc_RND);
    return class;
}

_AMX afxClass* _AmxGetLightClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass *class = &sim->lights;
    AFX_ASSERT_CLASS(class, afxFcc_LIT);
    return class;
}

_AMX afxClass* _AmxGetBodyClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass *class = &sim->bodCls;
    AFX_ASSERT_CLASS(class, afxFcc_BOD);
    return class;
}

_AMX afxClass* _AmxGetCapstanClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass *class = &sim->motoCls;
    AFX_ASSERT_CLASS(class, afxFcc_MOT);
    return class;
}

_AMX afxClass* _AmxGetMotiveClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass *class = &sim->motvCls;
    AFX_ASSERT_CLASS(class, afxFcc_MOTV);
    return class;
}

_AMX afxClass* _AmxGetTerrainClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass *class = &sim->terCls;
    AFX_ASSERT_CLASS(class, afxFcc_TER);
    return class;
}

_AMX afxClass* _AmxGetNodeClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass *class = &sim->nodCls;
    AFX_ASSERT_CLASS(class, afxFcc_NOD);
    return class;
}

_AMX afxStringBase AfxGetModelUrnStringBase(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    if (!sim->strbMdlSklMotUrns)
        AfxAcquireStringCatalogs(1, &sim->strbMdlSklMotUrns);

    return sim->strbMdlSklMotUrns;
}

_AMX void AfxQuerySimulationErrorTolerance(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler)
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

_AMX afxReal _AmxGetAllowedLodErrorFadingFactor(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim->allowedLodErrFadingFactor;
}

_AMX void AfxComputeBasisConversion(afxSimulation sim, afxReal unitsPerMeter, afxV3d const right, afxV3d const up, afxV3d const back, afxV3d const origin, afxM3d ltm, afxM3d iltm, afxV3d atv)
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

_AMX afxError AfxStepSimulation(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxResult cnt = 0;

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

_AMX afxUnit AfxGetSimBridges(afxSimulation sim, afxUnit baseExuIdx, afxUnit cnt, afxSimBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    /// bridges must be a valid pointer to afxSimBridge handles.
    AFX_ASSERT(bridges);
    afxUnit rslt = 0;

    afxUnit bridgeCnt = sim->bridgeCnt;
    AFX_ASSERT_RANGE(bridgeCnt, baseExuIdx, cnt);

    if (baseExuIdx < bridgeCnt)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxSimBridge mexu = sim->bridges[baseExuIdx + i];
            AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
            bridges[rslt++] = mexu;
        }
    }
    return rslt;
}

_AMX afxUnit AfxQuerySimBridges(afxSimulation sim, afxUnit mdevId, afxUnit portId, afxUnit first, afxUnit cnt, afxSimBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxUnit found = 0;
    afxUnit rslt = 0;

    afxUnit bridgeCnt = sim->bridgeCnt;
    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        afxSimBridge mexu = sim->bridges[i];
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
        afxEngine mdev;
        afxUnit portId2 = AfxQuerySimBridgePort(mexu, &mdev);
        AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

        if ((mdevId != AFX_INVALID_INDEX) && (mdevId != AfxGetObjectId(mdev)))
            continue;

        if ((portId != AFX_INVALID_INDEX) && (portId != portId2))
            continue;

        if (cnt && (found >= first))
        {
            if (bridges)
            {
                bridges[rslt] = mexu;
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

_AMX afxError AfxWaitForSimBridge(afxSimulation sim, afxUnit exuIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    /// portIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)sim->bridgeCnt, exuIdx, 1);

    afxSimBridge mexu;
    while (!AfxGetSimBridges(sim, exuIdx, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    AfxWaitForIdleSimBridge(mexu, timeout);

    return err;
}

_AMX afxError AfxWaitForSimulation(afxSimulation sim, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxSimulation handle.
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

_AMX afxError AfxRollSimCommands(afxSimulation sim, avxSubmission* ctrl, afxUnit cnt, afxCatalyst contexts[])
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(contexts);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(cnt);

    afxSimBridge mexu;
    if (!AfxGetSimBridges(sim, ctrl->exuIdx, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }

    afxClass const* dqueCls = _AmxGetSimQueueClass(mexu);

    // sanitize arguments
    afxUnit totalQueCnt = dqueCls->instCnt;
    afxUnit baseQueIdx = AfxMin(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AfxClamp(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);
    afxBool queued = FALSE;

    do
    {
        for (afxUnit i = 0; i < queCnt; i++)
        {
            afxUnit queIdx = baseQueIdx + i;

            afxSimQueue mque;
            if (!AfxGetSimQueues(mexu, queIdx, 1, &mque))
            {
                AfxThrowError();
                break;
            }

            afxError err2;
            if ((err2 = _AmxSubmitSimCommands(mque, ctrl, 1, contexts)))
            {
                if (err2 == afxError_TIMEOUT)
                    continue;

                AfxThrowError();
                break;
            }
            queued = TRUE;
            break;
        }

        if (err || queued)
            break; // reiterate if not queue for timeout?

    } while (0);
    return err;
}

_AMX afxBool _AmxCaptureBodCb(afxBody bod, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    afxFrustum* frustum = udd[0];
    afxReal lodErr = *(afxReal*)udd[1];
    afxArray* pvs = udd[2];

    //AfxFrustumTestAabbs(&frustum, 1, &aabb);

    if (1) // do visibility culling
    {
        afxUnit arrel;

        if (!AfxPushArrayUnits(pvs, 1, &arrel, &bod)) AfxThrowError();
        else
        {

        }
    }

    return TRUE;
}

_AMX afxError AfxCullBodies(afxSimulation sim, afxFrustum const* f, afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(pvs);
    AFX_ASSERT(pvs->cap);
    AFX_ASSERT(f);

    // TODO: Move to implementation

    afxReal lodErr = 0.f;
    AfxInvokeBodies(sim, 0, AFX_N32_MAX, (void*)_AmxCaptureBodCb, (void*[]) { f, &lodErr, pvs });

    return err;
}

_AMX afxError _AmxSimDtorCb(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    AfxDeregisterChainedClasses(&sim->classes);

    return err;
}

_AMX afxError _AmxSimCtorCb(afxSimulation sim, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _amxSimulationAcquisition const* cfg = AFX_CAST(_amxSimulationAcquisition const*, args[1]) + invokeNo;
    _amxSimBridgeAcquisition* bridgeCfgs = AFX_CAST(_amxSimBridgeAcquisition*, args[2]) + invokeNo;

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

    AfxBoxCopy(&sim->extent, &cfg->extent);
    AfxV3dCopy(sim->origin, cfg->origin);

    if (!cfg->unitsPerMeter)
        sim->unitsPerMeter = 1.f;
    else
        sim->unitsPerMeter = cfg->unitsPerMeter;

    if (!cfg->allowedLodErrFadingFactor)
        sim->allowedLodErrFadingFactor = 0.80000001;
    else
        sim->allowedLodErrFadingFactor = cfg->allowedLodErrFadingFactor;

    // OBJECTS

    {
        afxChain* classes = &sim->classes;
        AfxDeployChain(classes, (void*)sim);

        afxClassConfig clsCfg;

        clsCfg = cfg->curClsCfg ? *cfg->curClsCfg : _AMX_CUR_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_CUR);
        AfxMountClass(&sim->curCls, NIL, classes, &clsCfg);

        clsCfg = cfg->poseClsCfg ? *cfg->poseClsCfg : _AMX_POSE_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_POSE);
        AfxMountClass(&sim->poseCls, NIL, classes, &clsCfg);

        clsCfg = cfg->plceClsCfg ? *cfg->plceClsCfg : _AMX_PLCE_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_PLCE);
        AfxMountClass(&sim->plceCls, NIL, classes, &clsCfg);

        clsCfg = cfg->mtlClsCfg ? *cfg->mtlClsCfg : _AMX_MTL_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MTL);
        AfxMountClass(&sim->mtlCls, NIL, classes, &clsCfg); // require tex

        clsCfg = cfg->mdlClsCfg ? *cfg->mdlClsCfg : _AMX_MDL_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MDL);
        AfxMountClass(&sim->mdlCls, NIL, classes, &clsCfg); // require skl, msh

        clsCfg = cfg->motClsCfg ? *cfg->motClsCfg : _AMX_MOT_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MOT);
        AfxMountClass(&sim->motCls, NIL, classes, &clsCfg);
        
        clsCfg = cfg->aniClsCfg ? *cfg->aniClsCfg : _AMX_ANI_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_ANI);
        AfxMountClass(&sim->aniCls, NIL, classes, &clsCfg); // require mot

        clsCfg = cfg->anikClsCfg ? *cfg->anikClsCfg : (afxClassConfig) { 0 }/* _AMX_ANI_CLASS_CONFIG*/;
        AFX_ASSERT(clsCfg.fcc == afxFcc_ANIK);
        AfxMountClass(&sim->anikCls, NIL, classes, &clsCfg); // require mot

        clsCfg = cfg->capsClsCfg ? *cfg->capsClsCfg : _AMX_MOTO_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MOTO);
        AfxMountClass(&sim->motoCls, NIL, classes, &clsCfg);

        clsCfg = cfg->motvClsCfg ? *cfg->motvClsCfg : (afxClassConfig) { 0 }/* _AMX_MOTO_CLASS_CONFIG*/;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MOTV);
        AfxMountClass(&sim->motvCls, NIL, classes, &clsCfg);

        clsCfg = cfg->bodClsCfg ? *cfg->bodClsCfg : _AMX_BOD_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_BOD);
        AfxMountClass(&sim->bodCls, NIL, classes, &clsCfg); // require moto

        clsCfg = cfg->terClsCfg ? *cfg->terClsCfg : _AMX_TER_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_TER);
        AfxMountClass(&sim->terCls, NIL, classes, &clsCfg); // require all

        clsCfg = cfg->litClsCfg ? *cfg->litClsCfg : _AMX_LIT_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_LIT);
        AfxMountClass(&sim->lights, NIL, classes, &clsCfg);

        clsCfg = cfg->rndClsCfg ? *cfg->rndClsCfg : _AMX_RND_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_RND);
        AfxMountClass(&sim->renderers, NIL, classes, &clsCfg); // require all


        clsCfg = cfg->mexuClsCfg ? *cfg->mexuClsCfg : _AMX_MEXU_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MEXU);
        AfxMountClass(&sim->mexuCls, NIL, classes, &clsCfg); // require all

    }

    sim->dsys = cfg->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &sim->dsys);

    sim->din = cfg->din;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &sim->din);

    {
        sim->stepNum = 0;
        sim->stepsPerSecond = 0;
        sim->stepTime = 0;

        sim->strbMdlSklMotUrns = NIL;
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

    afxClass* cls = (afxClass*)_AmxGetMathBridgeClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MEXU);

    if (AfxAcquireObjects(cls, sim->bridgeCnt, (afxObject*)sim->bridges, (void const*[]) { sim, bridgeCfgs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, sim->bridgeCnt, sim->bridges);

        afxEngine mdev = AfxGetMathBridgeDevice(sim->bridges[0]);

        AfxCallDevice((afxDevice)mdev, 3, sim);
        AfxCallDevice((afxDevice)mdev, 5, sim);

        if (AfxDoDeviceService((afxDevice)mdev)) AfxThrowError(); // let the device build its SPUs.
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

_AMX afxClassConfig const _AMX_SIM_CLASS_CONFIG =
{
    .fcc = afxFcc_SIM,
    .name = "Simulation",
    .desc = "Simulation",
    .fixedSiz = sizeof(AFX_OBJECT(afxSimulation)),
    .ctor = (void*)_AmxSimCtorCb,
    .dtor = (void*)_AmxSimDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxConfigureSimulation(afxUnit icd, afxSimulationConfig* cfg)
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
    if (!AmxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AMX));

    afxEngine mdev;
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    if (!AfxEnumerateEngines(icd, 0, 1, &mdev))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    AfxV3dSet(cfg->right, 1, 0, 0);
    AfxV3dSet(cfg->up, 0, 1, 0);
    AfxV3dSet(cfg->back, 0, 0, 1);
    AfxV3dZero(cfg->origin);
    AfxAabbSet(&cfg->extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    cfg->unitsPerMeter = 1.f;
    cfg->allowedLodErrFadingFactor = 0.80000001;

    cfg->prime.minQueCnt = _AMX_MQUE_CLASS_CONFIG.unitsPerPage;

    return err;
}

_AMX afxError AfxEstablishSimulation(afxUnit icd, afxSimulationConfig const* cfg, afxSimulation* simulation)
{
    afxError err = AFX_ERR_NONE;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    afxModule driver;
    if (!AmxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_AMX));

    afxEngine mdev;
    AFX_ASSERT(cfg->prime.engId != AFX_INVALID_INDEX);
    if (!AfxEnumerateEngines(icd, cfg->prime.engId, 1, &mdev))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    if (AfxCallDevice((afxDevice)mdev, 1, NIL))
    {
        AfxThrowError(); // let the device build its DPUs.
        return err;
    }

    // Acquire bridges and queues
    afxUnit portId = AFX_INVALID_INDEX;
    afxUnit totalSqueCnt = 0;
    afxUnit baseQueIdx[AMX_MAX_SIM_BRIDGE_PER_CONTEXT] = { 0 };
    _amxSimBridgeAcquisition bridgeCfg[AMX_MAX_SIM_BRIDGE_PER_CONTEXT] = { 0 };
    afxUnit bridgeCnt = 0;

    if (!AfxEnumerateEngines(icd, cfg->prime.engId, 1, &mdev))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    bridgeCfg[bridgeCnt].mdev = mdev;
    if (!AfxChooseSimPorts(mdev, cfg->prime.capabilities, cfg->prime.acceleration, 1, &portId))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT(portId != AFX_INVALID_INDEX);
    bridgeCfg[bridgeCnt].portId = portId;
    bridgeCfg[bridgeCnt].exuIdx = 0;
    bridgeCfg[bridgeCnt].minQueCnt = AfxClamp(cfg->prime.minQueCnt, 1, AMX_MAX_SIM_QUEUE_PER_BRIDGE);
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
                if (!AfxEnumerateEngines(icd, cfg->auxs[i].engId, 1, &mdev))
                {
                    AfxThrowError();
                    return err;
                }
                AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
                bridgeCfg[bridgeCnt].mdev = mdev;
                if (!AfxChooseSimPorts(mdev, cfg->auxs[i].capabilities, cfg->auxs[i].acceleration, 1, &portId))
                {
                    AfxThrowError();
                    return err;
                }
                AFX_ASSERT(portId != AFX_INVALID_INDEX);
                bridgeCfg[bridgeCnt].portId = portId;
                bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
                bridgeCfg[bridgeCnt].minQueCnt = AfxClamp(cfg->auxs[i].minQueCnt, 1, AMX_MAX_SIM_QUEUE_PER_BRIDGE);
                ++bridgeCnt;
            }
        }
    }

    _amxSimulationAcquisition cfg2 = { 0 };
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
    AfxBoxCopy(&cfg2.extent, &cfg->extent);

    afxClass* cls = (afxClass*)_AmxGetSimulationClass(driver);
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
