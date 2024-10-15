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
#include "../../dev/AmxImplKit.h"

extern afxClassConfig const _AfxNodMgrCfg;

extern afxClassConfig const _AmxBodMgrCfg;
extern afxClassConfig const _AmxAniMgrCfg;
extern afxClassConfig const _AmxMotMgrCfg;
extern afxClassConfig const _AmxMotoMgrCfg;
extern afxClassConfig const _AfxLitMgrCfg;
extern afxClassConfig const _AfxRndMgrCfg;
extern afxClassConfig const _AmxTerClsCfg;

extern afxClassConfig const _AmxSklClsCfg;
extern afxClassConfig const _AmxPoseClsCfg;
extern afxClassConfig const _AmxPosbClsCfg;
extern afxClassConfig const _AmxCurClsCfg;

extern afxClassConfig const _AmxMtlClsCfg;
extern afxClassConfig const _AmxMdlClsCfg;

//extern afxChain* _AfxGetSystemClassChain(void);

// SIMULATION API //////////////////////////////////////////////////////////////

_AMX afxDrawContext AfxGetSimulationDrawContext(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    return sim->dctx;
}

_AMX afxDrawInput AfxGetSimulationDrawInput(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    return sim->din;
}

_AMX afxClass const* AfxGetSkeletonClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass const* cls = &sim->sklCls;
    AfxAssertClass(cls, afxFcc_SKL);
    return cls;
}

_AMX afxClass const* AfxGetPoseClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass const* cls = &sim->poseCls;
    AfxAssertClass(cls, afxFcc_POSE);
    return cls;
}

_AMX afxClass const* AfxGetPoseBufferClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass const* cls = &sim->posbCls;
    AfxAssertClass(cls, afxFcc_POSB);
    return cls;
}

_AMX afxClass const* AfxGetCurveClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass const* cls = &sim->curCls;
    AfxAssertClass(cls, afxFcc_CUR);
    return cls;
}

_AMX afxClass const* AfxGetMaterialClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass const* cls = &sim->mtlCls;
    AfxAssertClass(cls, afxFcc_MTL);
    return cls;
}

_AMX afxClass const* AfxGetModelClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass const* cls = &sim->mdlCls;
    AfxAssertClass(cls, afxFcc_MDL);
    return cls;
}

_AMX afxClass* AfxGetRendererClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->renderers;
    AfxAssertClass(class, afxFcc_RND);
    return class;
}

_AMX afxClass* AfxGetLightClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->lights;
    AfxAssertClass(class, afxFcc_LIT);
    return class;
}

_AMX afxClass* AfxGetBodyClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->bodies;
    AfxAssertClass(class, afxFcc_BOD);
    return class;
}

_AMX afxClass* AkxGetMotorClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->motors;
    AfxAssertClass(class, afxFcc_MOT);
    return class;
}

_AMX afxClass* AfxGetTerrainClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->terCls;
    AfxAssertClass(class, afxFcc_TER);
    return class;
}

_AMX afxClass* AfxGetNodeClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->nodes;
    AfxAssertClass(class, afxFcc_NOD);
    return class;
}

_AMX afxStringBase AfxGetModelUrnStringBase(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (!sim->strbMdlSklMotUrns)
        AfxAcquireStringCatalogs(1, &sim->strbMdlSklMotUrns);

    return sim->strbMdlSklMotUrns;
}

_AMX void AfxQuerySimulationErrorTolerance(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

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

_AMX afxReal _AkxGetAllowedLodErrorFadingFactor(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    return sim->allowedLodErrFadingFactor;
}

_AMX void AfxComputeBasisConversion(afxSimulation sim, afxReal unitsPerMeter, afxV3d const right, afxV3d const up, afxV3d const back, afxV3d const origin, afxM3d ltm, afxM3d iltm, afxV3d atv)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(unitsPerMeter);
    AfxAssert(origin);
    AfxAssert(right);
    AfxAssert(back);
    AfxAssert(up);
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(atv);

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
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxResult cnt = 0;

    afxSize stepTime = AfxGetTimer();
    afxReal deltaTime = sim->stepDeltaTime = AfxScalar(stepTime - sim->stepTime) * 0.001f; // multiplying by 0.001f turns deltaTime in seconds
    sim->stepTime = stepTime;

    //AfxStepDag(sim->dag, AfxScalar(deltaTime));
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

_AMX afxNat AfxCountMathBridges(afxSimulation sim, afxNat baseBridIdx)
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxSimulation handle.
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssertRange(sim->exuCnt, baseBridIdx, 1);
    return sim->exuCnt - baseBridIdx;
}

_AMX afxBool AfxGetMathBridge(afxSimulation sim, afxNat portIdx, afxComboBridge* bridge)
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxSimulation handle.
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    /// portIdx must be one of the bridge indices specified when device was created.
    AfxAssertRange(sim->exuCnt, portIdx, 1);
    afxBool rslt;

    if (!(rslt = (portIdx < sim->exuCnt))) AfxThrowError();
    else
    {
        afxComboBridge mdge = sim->exus[portIdx];
        AfxAssertObjects(1, &mdge, afxFcc_MDGE);
        /// bridge must be a valid pointer to a afxComboBridge handle.
        *bridge = mdge;
    }
    return rslt;
}

_AMX afxBool AfxGetMathQueue(afxSimulation sim, afxNat portIdx, afxNat queIdx, afxMathQueue* queue)
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxSimulation handle.
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    /// portIdx must be one of the bridge indices specified when device was created.
    AfxAssertRange(sim->exuCnt, portIdx, 1);
    afxBool rslt = FALSE;
    afxComboBridge mdge;

    if (!AfxGetMathBridge(sim, portIdx, &mdge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &mdge, afxFcc_MDGE);
        afxMathQueue mque;

        if (!(rslt = _AmxGetMathQueue(mdge, queIdx, &mque))) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &mque, afxFcc_MQUE);
            /// queue must be a valid pointer to a afxMathQueue handle.
            AfxAssert(queue);
            *queue = mque;
        }
    }
    return rslt;
}

_AMX afxNat AfxCountMathQueues(afxSimulation sim, afxNat portIdx, afxNat baseQueIdx)
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxSimulation handle.
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    /// portIdx must be a valid index to a bridge.
    AfxAssertRange(sim->exuCnt, portIdx, 1);
    afxNat queCnt = 0;
    afxComboBridge mdge;

    if (!AfxGetMathBridge(sim, portIdx, &mdge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &mdge, afxFcc_MDGE);
        queCnt = _AmxCountMathQueues(mdge, baseQueIdx);
        AfxAssertRange(queCnt, baseQueIdx, 1);
        queCnt -= baseQueIdx;
    }
    return queCnt;
}

_AMX afxError AfxWaitForSimulation(afxSimulation sim, afxNat portIdx, afxNat firstQueIdx, afxNat queCnt)
{
    afxError err = AFX_ERR_NONE;
    /// sim must be a valid afxSimulation handle.
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (!sim->waitCb)
    {
        afxNat mdgeCnt = sim->exuCnt;

        if (portIdx != AFX_INVALID_INDEX)
        {
            afxComboBridge mdge = sim->exus[portIdx];
            AfxAssertObjects(1, &mdge, afxFcc_MDGE);

            afxNat queCnt2 = _AmxCountMathQueues(mdge, firstQueIdx) - queCnt;

            if (!queCnt || queCnt == queCnt2)
                _AmxWaitForIdleMathBridge(mdge);
            else for (afxNat j = 0; j < queCnt2; j++)
                _AmxWaitForIdleMathQueue(mdge, j);
        }
        else for (afxNat i = 0; i < mdgeCnt; i++)
        {
            afxComboBridge mdge = sim->exus[i];
            AfxAssertObjects(1, &mdge, afxFcc_MDGE);
            
            afxNat queCnt2 = _AmxCountMathQueues(mdge, firstQueIdx) - queCnt;

            if (!queCnt || queCnt == queCnt2)
                _AmxWaitForIdleMathBridge(mdge);
            else for (afxNat j = 0; j < queCnt2; j++)
                _AmxWaitForIdleMathQueue(mdge, j);
        }
    }
    else if (sim->waitCb(sim))
        AfxThrowError();

    return err;
}

_AMX afxError _AmxSimDtorCb(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    AfxDeregisterChainedClasses(&sim->classes);

    return err;
}

_AMX afxError _AmxSimCtorCb(afxSimulation sim, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    akxSimulationConfig const* config = ((akxSimulationConfig const*)args[1]) + invokeNo;

    afxChain* classes = &sim->classes;
    AfxDeployChain(classes, sim);

    afxAllocationStrategy as;
    as.size = sizeof(afxByte);
    as.align = AFX_SIMD_ALIGN;
    as.cap = 0;
    as.duration = AFX_ALL_DUR_TRANSIENT;
    as.resizable = TRUE;
    as.stock = 4096000;

    sim->dctx = config->dctx;
    AfxAssertObjects(1, &sim->dctx, afxFcc_DCTX);

    sim->din = config->din;
    AfxAssertObjects(1, &sim->din, afxFcc_DIN);

    // UNIVERSE

    AfxAabbSet(sim->extent, 2, (afxV3d const[]) { { 1000, 1000, 1000 }, { -1000, -1000, -1000 } });
    AfxResetV4d(sim->origin);
    AfxM3dReset(sim->basis);
    AfxV3dSet(sim->right,   1, 0, 0);
    AfxV3dSet(sim->up,   0, 1, 0);
    AfxV3dSet(sim->back,    0, 0, 1);
    sim->unitsPerMeter = 1.f;
    sim->allowedLodErrFadingFactor = 0.80000001;

    if (config)
    {
        AfxBoxCopy(sim->extent, config->extent);

        AfxAssert(config->unitsPerMeter);
        AfxAssert(!AfxV3dIsZero(config->right));
        AfxAssert(!AfxV3dIsZero(config->up));
        AfxAssert(!AfxV3dIsZero(config->back));

        AfxV3dCopy(sim->right, config->right);
        AfxV3dCopy(sim->up, config->up);
        AfxV3dCopy(sim->back, config->back);
        AfxV3dCopy(sim->origin, config->origin);
        sim->unitsPerMeter = config->unitsPerMeter;
    }

    AfxM3dSet(sim->basis, sim->right, sim->up, sim->back);

    {
            

        sim->stepNum = 0;
        sim->stepsPerSecond = 0;
        sim->stepTime = 0;

        //afxClassConfig tmp;

        //AfxRegisterClass(&sim->nodes, NIL, classes, &_AfxNodMgrCfg);

        afxClassConfig tmpClsConf;

        tmpClsConf = _AmxCurClsCfg;
        AfxRegisterClass(&sim->curCls, NIL, classes, &tmpClsConf);
        tmpClsConf = _AmxPoseClsCfg;
        AfxRegisterClass(&sim->poseCls, NIL, classes, &tmpClsConf);
        tmpClsConf = _AmxPosbClsCfg;
        AfxRegisterClass(&sim->posbCls, NIL, classes, &tmpClsConf);
        tmpClsConf = _AmxSklClsCfg;
        AfxRegisterClass(&sim->sklCls, NIL, classes, &tmpClsConf);

        tmpClsConf = _AmxMtlClsCfg;
        AfxRegisterClass(&sim->mtlCls, NIL, classes, &tmpClsConf); // require tex

        tmpClsConf = _AmxMdlClsCfg;
        AfxRegisterClass(&sim->mdlCls, NIL, classes, &tmpClsConf); // require skl, msh

        AfxRegisterClass(&sim->motMgr, NIL, classes, &_AmxMotMgrCfg);
        AfxRegisterClass(&sim->aniMgr, NIL, classes, &_AmxAniMgrCfg); // require mot
        AfxRegisterClass(&sim->motors, NIL, classes, &_AmxMotoMgrCfg);
        AfxRegisterClass(&sim->bodies, NIL, classes, &_AmxBodMgrCfg); // require moto
        
        AfxRegisterClass(&sim->terCls, NIL, classes, &_AmxTerClsCfg); // require all

        AfxRegisterClass(&sim->lights, NIL, classes, &_AfxLitMgrCfg);

        AfxRegisterClass(&sim->renderers, NIL, classes, &_AfxRndMgrCfg); // require all

        sim->strbMdlSklMotUrns = NIL;
    }


    if (err)
    {
        AfxDeregisterChainedClasses(&sim->classes);
    }
    return err;
}

_AMX afxClassConfig const _AmxSimStdImplementation =
{
    .fcc = afxFcc_SIM,
    .name = "Simulation",
    .desc = "Simulation",
    .fixedSiz = sizeof(AFX_OBJECT(afxSimulation)),
    .ctor = (void*)_AmxSimCtorCb,
    .dtor = (void*)_AmxSimDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxNat AfxInvokeSimulations(afxComboDevice mdev, afxNat first, afxNat cnt, afxBool(*f)(afxSimulation, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdev, afxFcc_MDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetSimulationClass(mdev);
    AfxAssertClass(cls, afxFcc_SIM);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AMX afxNat AfxEvokeSimulations(afxComboDevice mdev, afxBool(*flt)(afxSimulation, void*), void* fdd, afxNat first, afxNat cnt, afxSimulation simulations[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdev, afxFcc_MDEV);
    AfxAssert(simulations);
    AfxAssert(flt);
    AfxAssert(cnt);
    afxClass const* cls = AfxGetSimulationClass(mdev);
    AfxAssertClass(cls, afxFcc_SIM);
    return AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)simulations);
}

_AMX afxNat AfxEnumerateSimulations(afxComboDevice mdev, afxNat first, afxNat cnt, afxSimulation simulations[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdev, afxFcc_MDEV);
    AfxAssert(cnt);
    AfxAssert(simulations);
    afxClass const* cls = AfxGetSimulationClass(mdev);
    AfxAssertClass(cls, afxFcc_SIM);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)simulations);
}

_AMX afxError AfxAcquireSimulations(afxNat mdevId, akxSimulationConfig const* cfg, afxSimulation* simulation)
{
    afxError err = AFX_ERR_NONE;
    afxComboDevice mdev;

    if (!(AfxGetComboDevice(mdevId, &mdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &mdev, afxFcc_MDEV);
        afxClass* cls = (afxClass*)AfxGetSimulationClass(mdev);
        AfxAssertClass(cls, afxFcc_SIM);
        afxSimulation sim;

        if (AfxAcquireObjects(cls, 1, (afxObject*)&sim, (void const*[]) { mdev, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &sim, afxFcc_SIM);
            AfxAssert(simulation);
            *simulation = sim;
        }
    }
    return err;
}

_AMX afxError AfxConfigureSimulation(afxNat mdevId, akxSimulationConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mdevId != AFX_INVALID_INDEX);
    afxComboDevice mdev;

    if (!(AfxGetComboDevice(mdevId, &mdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &mdev, afxFcc_MDEV);
        AfxAssert(cfg);
        *cfg = (akxSimulationConfig) { 0 };
        cfg->unitsPerMeter = 1.f;
        AfxV3dSet(cfg->right,   1, 0, 0);
        AfxV3dSet(cfg->up,      0, 1, 0);
        AfxV3dSet(cfg->back,    0, 0, 1);
        AfxV3dZero(cfg->origin);
        AfxAabbSet(cfg->extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    }
    return err;
}
