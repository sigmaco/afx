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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_SIM_C
#define _AFX_SIMULATION_C
#include "qwadro/core/afxSystem.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/sim/rendering/awxLight.h"
#include "qwadro/sim/afxMaterial.h"
#include "qwadro/sim/modeling/afxModel.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"

#include "qwadro/sim/modeling/afxMesh.h"

extern afxClassConfig const _AfxNodMgrCfg;
extern afxClassConfig const _AfxSklMgrCfg;
extern afxClassConfig const _AfxMtlMgrCfg;
extern afxClassConfig const _AfxMshMgrCfg;
extern afxClassConfig const _AfxVtdMgrCfg;
extern afxClassConfig const _AfxMshtMgrCfg;
extern afxClassConfig const _AfxMdlMgrCfg;
extern afxClassConfig const _AwxBodMgrCfg;
extern afxClassConfig const _AwxAniMgrCfg;
extern afxClassConfig const _AwxMotMgrCfg;
extern afxClassConfig const _AfxMotoMgrCfg;
extern afxClassConfig const _AfxEntMgrCfg;
extern afxClassConfig const _AfxLitMgrCfg;
extern afxClassConfig const _AfxRndMgrCfg;
extern afxClassConfig const _AfxCadMgrCfg;

//extern afxChain* _AfxGetSystemClassChain(void);

// SIMULATION API //////////////////////////////////////////////////////////////

_AKX afxMmu AfxGetSimulationMmu(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMmu mmu = sim->genrlMem;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AKX afxManager* AfxGetRendererClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->renderers;
    AfxAssertClass(class, afxFcc_RND);
    return class;
}

_AKX afxManager* AfxGetMaterialClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->materials;
    AfxAssertClass(class, afxFcc_MTL);
    return class;
}

_AKX afxManager* AwxGetMeshClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->meshes;
    AfxAssertClass(class, afxFcc_MSH);
    return class;
}

_AKX afxManager* AwxGetMeshDataClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->meshDatas;
    AfxAssertClass(class, afxFcc_VTD);
    return class;
}

_AKX afxManager* AwxGetMeshTopologyClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->topologies;
    AfxAssertClass(class, afxFcc_MSHT);
    return class;
}

_AKX afxManager* AwxGetModelClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->models;
    AfxAssertClass(class, afxFcc_MDL);
    return class;
}

_AKX afxManager* AfxGetLightClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->lights;
    AfxAssertClass(class, afxFcc_LIT);
    return class;
}

_AKX afxManager* AfxGetBodyClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->bodies;
    AfxAssertClass(class, afxFcc_BOD);
    return class;
}

_AKX afxManager* AwxGetMotorClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->motors;
    AfxAssertClass(class, afxFcc_MOT);
    return class;
}

_AKX afxManager* AfxGetEntityClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->entities;
    AfxAssertClass(class, afxFcc_ENT);
    return class;
}

_AKX afxManager* AfxGetSkeletonClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->skeletons;
    AfxAssertClass(class, afxFcc_SKL);
    return class;
}

_AKX afxManager* AfxGetNodeClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *class = &sim->nodes;
    AfxAssertClass(class, afxFcc_NOD);
    return class;
}

_AKX afxNat AfxEnumerateBodies(afxSimulation sim, afxNat base, afxNat cnt, awxBody bod[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *cls = AfxGetBodyClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountObjects(cls) >= base + cnt);
    return AfxEnumerateObjects(cls, base, cnt, (afxObject*)bod);
}

_AKX afxNat AfxEnumerateEntities(afxSimulation sim, afxNat base, afxNat cnt, awxEntity ent[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *cls = AfxGetEntityClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountObjects(cls) >= base + cnt);
    return AfxEnumerateObjects(cls, base, cnt, (afxObject*)ent);
}

_AKX afxNat AfxEnumerateMaterials(afxSimulation sim, afxNat base, afxNat cnt, afxMaterial mtl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *cls = AfxGetMaterialClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountObjects(cls) >= base + cnt);
    return AfxEnumerateObjects(cls, base, cnt, (afxObject*)mtl);
}

_AKX afxNat AfxEnumerateMeshes(afxSimulation sim, afxNat base, afxNat cnt, afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *cls = AwxGetMeshClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountObjects(cls) >= base + cnt);
    return AfxEnumerateObjects(cls, base, cnt, (afxObject*)msh);
}

_AKX afxNat AfxEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel mdl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *cls = AwxGetModelClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountObjects(cls) >= base + cnt);
    return AfxEnumerateObjects(cls, base, cnt, (afxObject*)mdl);
}

_AKX afxNat AfxEnumerateSkeletons(afxSimulation sim, afxNat base, afxNat cnt, afxSkeleton skl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxManager *cls = AfxGetSkeletonClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountObjects(cls) >= base + cnt);
    return AfxEnumerateObjects(cls, base, cnt, (afxObject*)skl);
}

_AKX void AfxQuerySimulationErrorTolerance(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler)
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

_AKX void AfxComputeBasisConversion(afxSimulation sim, afxReal unitsPerMeter, afxV3d const right, afxV3d const up, afxV3d const back, afxV3d const origin, afxM3d ltm, afxM3d iltm, afxV3d atv)
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
    AfxSetM3dTransposed(srcAxisSys, right, up, back);

    AfxMultiplyM3d(ltm, sim->basis, srcAxisSys);

    afxReal lambda = sim->unitsPerMeter / unitsPerMeter;
    AfxScaleV3d(ltm[0], ltm[0], lambda);
    AfxScaleV3d(ltm[1], ltm[1], lambda);
    AfxScaleV3d(ltm[2], ltm[2], lambda);
    
    AfxInvertM3d(ltm, iltm);

    AfxSubV3d(atv, sim->origin, origin);
}

_AKX afxError AfxCullSimulation(afxSimulation sim, afxCamera cam, afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssertType(pvs, afxFcc_ARR);



    return err;
}

_AKX afxError AfxStepSimulation(afxSimulation sim)
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

_AKX afxError AfxRenderSimulation(afxSimulation sim, afxArray const* pvs)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxThrowError();
    return err;
}

_AKX afxError _AfxSimulationProcess(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxStepSimulation(sim);
    return err;
}

_AKX afxError _AfxSimDtor(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    AfxCleanUpChainedManagers(&sim->classes);

    AfxReleaseObjects(1, (void*[]) { sim->genrlMem });

    return err;
}

_AKX afxError _AfxSimCtor(afxSimulation sim, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    awxSimulationConfig const *config = ((awxSimulationConfig const*)cookie->udd[0]) + cookie->no;

    afxChain *classes = &sim->classes;
    AfxSetUpChain(classes, sim);

    afxAllocationStrategy as;
    as.size = sizeof(afxByte);
    as.align = AFX_SIMD_ALIGN;
    as.cap = 0;
    as.duration = AFX_ALL_DUR_TRANSIENT;
    as.resizable = TRUE;
    as.stock = 4096000;

    if (AfxAcquireMmus(1, AfxHere(), &as, &sim->genrlMem)) AfxThrowError();
    else
    {
        sim->dctx = config->dctx;
        AfxAssertObjects(1, &sim->dctx, afxFcc_DCTX);

        // UNIVERSE

        AfxRecomputeAabb(sim->extent, 2, (afxV3d const[]) { { 1000, 1000, 1000 }, { -1000, -1000, -1000 } });
        AfxResetV4d(sim->origin);
        AfxResetM3d(sim->basis);
        AfxSetV3d(sim->right,   1, 0, 0);
        AfxSetV3d(sim->up,   0, 1, 0);
        AfxSetV3d(sim->back,    0, 0, 1);
        sim->unitsPerMeter = 1.f;
        sim->allowedLodErrFadingFactor = 0.80000001;

        if (config)
        {
            AfxCopyAabb(sim->extent, config->extent);

            AfxAssert(config->unitsPerMeter);
            AfxAssert(!AfxV3dIsZero(config->right));
            AfxAssert(!AfxV3dIsZero(config->up));
            AfxAssert(!AfxV3dIsZero(config->back));

            AfxCopyV3d(sim->right, config->right);
            AfxCopyV3d(sim->up, config->up);
            AfxCopyV3d(sim->back, config->back);
            AfxCopyV3d(sim->origin, config->origin);
            sim->unitsPerMeter = config->unitsPerMeter;
        }

        AfxSetM3d(sim->basis, sim->right, sim->up, sim->back);

        {
            

            sim->stepNum = 0;
            sim->stepsPerSecond = 0;
            sim->stepTime = 0;

            //afxClassConfig tmp;

            AfxEstablishManager(&sim->materials, NIL, classes, &_AfxMtlMgrCfg); // require tex
            
            AfxEstablishManager(&sim->topologies, NIL, classes, &_AfxMshtMgrCfg);
            AfxEstablishManager(&sim->meshDatas, NIL, classes, &_AfxVtdMgrCfg);
            AfxEstablishManager(&sim->meshes, NIL, classes, &_AfxMshMgrCfg); // require msht, vtd

            //AfxEstablishManager(&sim->nodes, NIL, classes, &_AfxNodMgrCfg);
            AfxEstablishManager(&sim->skeletons, NIL, classes, &_AfxSklMgrCfg);
            AfxEstablishManager(&sim->models, NIL, classes, &_AfxMdlMgrCfg); // require skl, msh
            
            AfxEstablishManager(&sim->motions, NIL, classes, &_AwxMotMgrCfg);
            AfxEstablishManager(&sim->animations, NIL, classes, &_AwxAniMgrCfg); // require mot

            AfxEstablishManager(&sim->motors, NIL, classes, &_AfxMotoMgrCfg);
            AfxEstablishManager(&sim->bodies, NIL, classes, &_AwxBodMgrCfg); // require moto
            
            AfxEstablishManager(&sim->entities, NIL, classes, &_AfxEntMgrCfg);
            AfxEstablishManager(&sim->lights, NIL, classes, &_AfxLitMgrCfg);

            AfxEstablishManager(&sim->assets, NIL, classes, &_AfxCadMgrCfg); // require all

            AfxEstablishManager(&sim->renderers, NIL, classes, &_AfxRndMgrCfg); // require all


        }


        if (err)
        {
            AfxCleanUpChainedManagers(&sim->classes);


            AfxReleaseObjects(1, (void*[]) { sim->genrlMem });
        }
    }
    return err;
}

_AKX afxClassConfig _AfxSimMgrCfg =
{
    .fcc = afxFcc_SIM,
    .name = "Simulation",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSimulation)),
    .mmu = NIL,
    .ctor = (void*)_AfxSimCtor,
    .dtor = (void*)_AfxSimDtor
};

////////////////////////////////////////////////////////////////////////////////

_AKX afxError AfxAcquireSimulations(afxNat cnt, awxSimulationConfig const config[], afxSimulation simulations[])
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetSimulationClass();
    AfxAssertClass(cls, afxFcc_SIM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)simulations, (void const*[]) { (void*)config }))
        AfxThrowError();

    AfxAssertObjects(cnt, simulations, afxFcc_SIM);
    
    return err;
}

_AKX afxNat AfxInvokeSimulations(afxNat first, afxNat cnt, afxBool(*f)(afxSimulation, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetSimulationClass();
    AfxAssertClass(cls, afxFcc_SIM);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AKX afxNat AfxEnumerateSimulations(afxNat first, afxNat cnt, afxSimulation simulations[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(simulations);
    afxManager* cls = AfxGetSimulationClass();
    AfxAssertClass(cls, afxFcc_SIM);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)simulations);
}

_AKX afxNat AfxCountSimulations(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetSimulationClass();
    AfxAssertClass(cls, afxFcc_SIM);
    return AfxCountObjects(cls);
}

_AKX afxManager* AfxGetSimulationClass(void)
{
    afxError err = AFX_ERR_NONE;
    static afxManager _simCls = { 0 };
    static afxBool simClsMounted = FALSE;

    if (_simCls.fcc != afxFcc_CLS)
    {
        AfxEstablishManager(&_simCls, NIL, /*_AfxGetSystemClassChain()*/NIL, &_AfxSimMgrCfg);
        simClsMounted = TRUE;
    }
    return &_simCls;
}