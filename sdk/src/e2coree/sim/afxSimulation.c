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
 *                       (c) 2017 SIGMA, Engineering In Technology
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

extern afxClassConfig const _AfxNodClsConfig;
extern afxClassConfig const _AfxSklClsConfig;
extern afxClassConfig const _AfxMtlClsConfig;
extern afxClassConfig const _AfxMshClsConfig;
extern afxClassConfig const _AfxVtdClsConfig;
extern afxClassConfig const _AfxMshtClsConfig;
extern afxClassConfig const _AfxMdlClsConfig;
extern afxClassConfig const _AwxBodClsConfig;
extern afxClassConfig const _AfxMotClsConfig;
extern afxClassConfig const _AfxEntClsConfig;
extern afxClassConfig const _AfxLitClsConfig;
extern afxClassConfig const _AfxRndClsConfig;
extern afxClassConfig const _AfxCadClsConfig;

// SIMULATION API //////////////////////////////////////////////////////////////

_AFX afxMmu AfxGetSimulationMmu(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMmu mmu = sim->genrlMem;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AFX afxClass* AfxGetRendererClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->renderers;
    AfxAssertClass(class, afxFcc_RND);
    return class;
}

_AFX afxClass* AfxGetMaterialClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->materials;
    AfxAssertClass(class, afxFcc_MTL);
    return class;
}

_AFX afxClass* AwxGetMeshClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->meshes;
    AfxAssertClass(class, afxFcc_MSH);
    return class;
}

_AFX afxClass* AwxGetMeshDataClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->meshDatas;
    AfxAssertClass(class, afxFcc_VTD);
    return class;
}

_AFX afxClass* AwxGetMeshTopologyClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->topologies;
    AfxAssertClass(class, afxFcc_MSHT);
    return class;
}

_AFX afxClass* AwxGetModelClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->models;
    AfxAssertClass(class, afxFcc_MDL);
    return class;
}

_AFX afxClass* AfxGetLightClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->lights;
    AfxAssertClass(class, afxFcc_LIT);
    return class;
}

_AFX afxClass* AfxGetBodyClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->bodies;
    AfxAssertClass(class, afxFcc_BOD);
    return class;
}

_AFX afxClass* AwxGetAnimusClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->motors;
    AfxAssertClass(class, afxFcc_MOT);
    return class;
}

_AFX afxClass* AfxGetEntityClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->entities;
    AfxAssertClass(class, afxFcc_ENT);
    return class;
}

_AFX afxClass* AfxGetSkeletonClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->skeletons;
    AfxAssertClass(class, afxFcc_SKL);
    return class;
}

_AFX afxClass* AfxGetNodeClass(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *class = &sim->nodes;
    AfxAssertClass(class, afxFcc_NOD);
    return class;
}

_AFX afxNat AfxEnumerateBodies(afxSimulation sim, afxNat base, afxNat cnt, awxBody bod[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *cls = AfxGetBodyClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountInstances(cls) >= base + cnt);
    return AfxEnumerateInstances(cls, base, cnt, (afxObject*)bod);
}

_AFX afxNat AfxEnumerateEntities(afxSimulation sim, afxNat base, afxNat cnt, awxEntity ent[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *cls = AfxGetEntityClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountInstances(cls) >= base + cnt);
    return AfxEnumerateInstances(cls, base, cnt, (afxObject*)ent);
}

_AFX afxNat AfxEnumerateMaterials(afxSimulation sim, afxNat base, afxNat cnt, afxMaterial mtl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *cls = AfxGetMaterialClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountInstances(cls) >= base + cnt);
    return AfxEnumerateInstances(cls, base, cnt, (afxObject*)mtl);
}

_AFX afxNat AfxEnumerateMeshes(afxSimulation sim, afxNat base, afxNat cnt, afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *cls = AwxGetMeshClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountInstances(cls) >= base + cnt);
    return AfxEnumerateInstances(cls, base, cnt, (afxObject*)msh);
}

_AFX afxNat AfxEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel mdl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *cls = AwxGetModelClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountInstances(cls) >= base + cnt);
    return AfxEnumerateInstances(cls, base, cnt, (afxObject*)mdl);
}

_AFX afxNat AfxEnumerateSkeletons(afxSimulation sim, afxNat base, afxNat cnt, afxSkeleton skl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass *cls = AfxGetSkeletonClass(sim);
    AfxAssertType(cls, afxFcc_CLS);
    //AfxAssert(AfxCountInstances(cls) >= base + cnt);
    return AfxEnumerateInstances(cls, base, cnt, (afxObject*)skl);
}

_AFX void AfxComputeAllowedErrorValues(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler)
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

_AFX void AfxComputeBasisConversion(afxSimulation sim, afxReal unitsPerMeter, afxReal const right[3], afxReal const up[3], afxReal const back[3], afxReal const origin[3], afxReal ltm[3][3], afxReal iltm[3][3], afxReal atv[3])
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
    AfxSetM3d(srcAxisSys, right, up, back);

    AfxMultiplyM3d(ltm, sim->basis, srcAxisSys);

    afxReal lambda = sim->unitsPerMeter / unitsPerMeter;
    AfxScaleV3d(ltm[0], ltm[0], lambda);
    AfxScaleV3d(ltm[1], ltm[1], lambda);
    AfxScaleV3d(ltm[2], ltm[2], lambda);
    
    AfxInvertM3d(ltm, iltm);

    AfxSubV3d(atv, sim->origin, origin);
}

_AFX afxError AfxCullSimulation(afxSimulation sim, afxCamera cam, afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssertType(pvs, afxFcc_ARR);



    return err;
}

_AFX afxError AfxStepSimulation(afxSimulation sim)
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

_AFX afxError AfxRenderSimulation(afxSimulation sim, afxArray const* pvs)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxThrowError();
    return err;
}

_AFX afxError _AfxSimulationProcess(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxStepSimulation(sim);
    return err;
}

_AFX afxError _AfxSimDtor(afxSimulation sim)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("sim=%p", sim);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    _AfxUninstallChainedClasses(&sim->classes);

    AfxReleaseObjects(1, (void*[]) { sim->genrlMem });

    return err;
}

_AFX afxError _AfxSimCtor(afxSimulation sim, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("sim=%p", sim);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    awxSimulationConfig const *config = ((awxSimulationConfig const*)cookie->udd[0]) + cookie->no;

    afxChain *classes = &sim->classes;
    AfxTakeChain(classes, sim);

    afxAllocationStrategy as;
    as.size = sizeof(afxByte);
    as.align = AFX_SIMD_ALIGN;
    as.cap = 0;
    as.duration = AFX_ALL_DUR_TRANSIENT;
    as.resizable = TRUE;
    as.stock = 4096000;

    if (AfxAcquireMmus(1, AfxHint(), &as, &sim->genrlMem)) AfxThrowError();
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

        AfxSetTransposedM3d(sim->basis, sim->right, sim->up, sim->back);

        {
            

            sim->stepNum = 0;
            sim->stepsPerSecond = 0;
            sim->stepTime = 0;

            //afxClassConfig tmp;

            AfxMountClass(&sim->assets, NIL, classes, &_AfxCadClsConfig);

            AfxMountClass(&sim->materials, NIL, classes, &_AfxMtlClsConfig);
            AfxMountClass(&sim->meshes, NIL, classes, &_AfxMshClsConfig);
            AfxMountClass(&sim->topologies, NIL, classes, &_AfxMshtClsConfig);
            AfxMountClass(&sim->meshDatas, NIL, classes, &_AfxVtdClsConfig);
            AfxMountClass(&sim->nodes, NIL, classes, &_AfxNodClsConfig);
            AfxMountClass(&sim->skeletons, NIL, classes, &_AfxSklClsConfig);
            AfxMountClass(&sim->models, NIL, classes, &_AfxMdlClsConfig);
            AfxMountClass(&sim->bodies, NIL, classes, &_AwxBodClsConfig);
            AfxMountClass(&sim->motors, NIL, classes, &_AfxMotClsConfig);

            AfxMountClass(&sim->entities, NIL, classes, &_AfxEntClsConfig);
            AfxMountClass(&sim->lights, NIL, classes, &_AfxLitClsConfig);

            AfxMountClass(&sim->renderers, NIL, classes, &_AfxRndClsConfig);

        }


        if (err)
        {
            _AfxUninstallChainedClasses(&sim->classes);


            AfxReleaseObjects(1, (void*[]) { sim->genrlMem });
        }
    }
    return err;
}

_AFX afxClassConfig _AfxSimClsConfig =
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

_AFX afxError AfxAcquireSimulations(afxNat cnt, awxSimulationConfig const config[], afxSimulation simulations[])
{
    afxError err = AFX_ERR_NONE;

    afxClass* cls = AfxGetSimulationClass();
    AfxAssertClass(cls, afxFcc_SIM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)simulations, (void const*[]) { (void*)config }))
        AfxThrowError();

    AfxAssertObjects(cnt, simulations, afxFcc_SIM);

    return err;
}

_AFX afxNat AfxInvokeSimulations(afxNat first, afxNat cnt, afxBool(*f)(afxSimulation, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetSimulationClass();
    AfxAssertClass(cls, afxFcc_SIM);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateSimulations(afxNat first, afxNat cnt, afxSimulation simulations[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(simulations);
    afxClass* cls = AfxGetSimulationClass();
    AfxAssertClass(cls, afxFcc_SIM);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)simulations);
}

_AFX afxNat AfxCountSimulations(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetSimulationClass();
    AfxAssertClass(cls, afxFcc_SIM);
    return AfxCountInstances(cls);
}
