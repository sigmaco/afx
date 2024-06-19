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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef AFX_SIMULATION_H
#define AFX_SIMULATION_H

#include "qwadro/base/afxClass.h"
#include "qwadro/sim/akxEntity.h"
#include "qwadro/sim/rendering/akxLighting.h"
#include "qwadro/space/afxSkeleton.h"
#include "qwadro/cad/afxModel.h"
#include "qwadro/space/akxAnimation.h"
#include "qwadro/cad/afxMaterial.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/space/akxMotor.h"
#include "qwadro/space/afxMotion.h"
#include "qwadro/sim/rendering/akxRenderer.h"
#include "qwadro/sound/afxSoundDefs.h"
#include "qwadro/space/afxCurve.h"

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. 
// By changing variables in the simulation, predictions may be made about the behaviour of the system. 
// It is a tool to virtually investigate the behaviour of the system under study.

// A space is a purely mathematical representation of a world within space and its interactions.

typedef struct smt
{
    void(*SampleTrackUUULocalAtTime0)(akxMotionTransform const* SourceTrack, afxTransform *Result);
    void(*SampleTrackUUULocal)(const sample_context *Context, const akxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, afxTransform *Result);
    void(*SampleTrackUUU)(const sample_context *Context, const akxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackIII)(const sample_context *Context, const akxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCII)(const sample_context *Context, const akxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCCI)(const sample_context *Context, const akxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCAI)(const sample_context *Context, const akxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackAII)(const sample_context *Context, const akxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackACI)(const sample_context *Context, const akxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackAAI)(const sample_context *Context, const akxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*Samplers[3][3][3])(const sample_context *, const akxMotionTransform *, akxBoundMotionTransform *, const afxTransform *, const afxM4d, const afxM4d, afxM4d, afxM4d);
    void(*SampleTrackSSS)(const sample_context *Context, const akxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*(*GetTrackSamplerFor)(const akxMotionTransform *Track))(const sample_context *, const akxMotionTransform *, akxBoundMotionTransform *, const afxTransform *, const afxM4d, const afxM4d, afxM4d, afxM4d);
    void(*SampleTrackUUUAtTime0)(const sample_context *Context, const akxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult);
    void(*SampleTrackUUUBlendWithTime0)(const sample_context *Context, const akxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult, afxReal BlendAmount);
    void(*SampleTrackIIU)(const sample_context *Context, const akxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackPOLocal)(const struct sample_context *Context, const akxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, afxV3d ResultPosition, afxQuat ResultOrientation);
} smt;

AKX smt Smt;

typedef struct mmt
{
    void(*BuildSingleCompositeFromPoseBuffer_Generic)(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxM4d ResultComposite);
    void(*BuildSingleCompositeFromPoseBufferTranspose_Generic)(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxReal ResultComposite3x4[3][4]);
    void(*BuildPositionPoseBufferOnly_Generic)(afxV3d const Position, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
    void(*BuildPositionPoseBufferComposite_Generic)(afxV3d const Position, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
    void(*BuildPositionOrientationPoseBufferOnly_Generic)(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
    void(*BuildPositionOrientationPoseBufferComposite_Generic)(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
    void(*BuildIdentityPoseBufferOnly_Generic)(afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
    void(*BuildIdentityPoseBufferComposite_Generic)(afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
    void(*BWP_Dispatch)(afxTransform *t, afxM4d const InverseWorld4x4, afxM4d Composite, afxM4d World, afxM4d const ParentWorld);
    void(*BWPNC_Dispatch)(afxTransform *t, afxM4d const ParentWorld, afxM4d World);
    void(*BuildFullPoseBufferOnly_Generic)(afxTransform const* t, const afxM4d ParentMatrix, afxM4d ResultWorldMatrix);
    void(*BuildFullPoseBufferComposite_Generic)(afxTransform const* t, const afxM4d ParentMatrix, const afxM4d InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
} mmt;

AKX mmt Mmt;




AFX_DEFINE_STRUCT(akxSimulationConfig)
{
    afxUri const*   driver;
    afxBox         extent;
    afxDrawContext  dctx;
    afxDrawInput    din;
    afxSoundContext sctx;
    afxV3d          right;
    afxV3d          up;
    afxV3d          back;
    afxV3d          origin;
    afxReal         unitsPerMeter;
};

#ifdef _AFX_SIMULATION_C
AFX_OBJECT(afxSimulation)
{
    afxMmu          genrlMem;
    afxChain        classes;
    afxClass      assets;
    afxClass      bodies;
    afxClass      motors;
    afxClass      entities;
    afxClass      lights;
    afxClass      materials;
    afxClass      meshes;
    afxClass      meshDatas;
    afxClass      topologies;
    afxClass      models;
    afxClass      skeletons;
    afxClass      poseMgr;
    afxClass      posbMgr;
    afxClass      curMgr;
    afxClass      motMgr;
    afxClass      aniMgr;
    afxClass      nodes;
    afxClass      renderers;

    afxV4d          right;
    afxV4d          up;
    afxV4d          back;
    afxM3d          basis;
    afxV4d          origin;
    afxBox         extent;
    afxReal         unitsPerMeter;
    afxReal         allowedLodErrFadingFactor;

    afxSize         stepNum, stepsPerSecond;
    afxSize         stepTime, swapTime;
    afxReal         stepDeltaTime;

    akxMotive       globalMotorInterlinks;

    afxDrawContext  dctx;
};
#endif//_AFX_SIMULATION_C

AKX afxMmu          AfxGetSimulationMmu(afxSimulation sim);

AKX afxClass*     AfxGetCurveClass(afxSimulation sim);
AKX afxClass*     AkxGetMotorClass(afxSimulation sim);
AKX afxClass*     AfxGetBodyClass(afxSimulation sim);
AKX afxClass*     AfxGetEntityClass(afxSimulation sim);
AKX afxClass*     AfxGetLightClass(afxSimulation sim);
AKX afxClass*     AfxGetMaterialClass(afxSimulation sim);
AKX afxClass*     AkxGetMeshClass(afxSimulation sim);
AKX afxClass*     AkxGetMeshDataClass(afxSimulation sim);
AKX afxClass*     AkxGetMeshTopologyClass(afxSimulation sim);
AKX afxClass*     AkxGetModelClass(afxSimulation sim);
AKX afxClass*     AfxGetNodeClass(afxSimulation sim);
AKX afxClass*     AfxGetPoseClass(afxSimulation sim);
AKX afxClass*     AfxGetPoseBufferClass(afxSimulation sim);
AKX afxClass*     AfxGetRendererClass(afxSimulation sim);
AKX afxClass*     AfxGetSkeletonClass(afxSimulation sim);

AKX afxNat          AfxEnumerateBodies(afxSimulation sim, afxNat base, afxNat cnt, afxBody bod[]);
AKX afxNat          AfxEnumerateEntities(afxSimulation sim, afxNat base, afxNat cnt, akxEntity ent[]);
AKX afxNat          AfxEnumerateMaterials(afxSimulation sim, afxNat base, afxNat cnt, afxMaterial mtl[]);
AKX afxNat          AfxEnumerateMeshes(afxSimulation sim, afxNat base, afxNat cnt, afxMesh msh[]);
AKX afxNat          AfxEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel mdl[]);
AKX afxNat          AfxEnumerateSkeletons(afxSimulation sim, afxNat base, afxNat cnt, afxSkeleton skl[]);


AKX afxError        _AfxSimulationProcess(afxSimulation sim);

AKX afxError        AfxStepSimulation(afxSimulation sim);
AKX afxError        AfxCullSimulation(afxSimulation sim, avxCamera cam, afxArray* pvs);
AKX afxError        AfxRenderSimulation(afxSimulation sim, afxArray const* pvs);

AKX afxNat          AfxCountMotors(afxSimulation sim);
AKX afxNat          AfxInvokeMotors(afxSimulation sim, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AKX afxNat          AfxEnumerateMotors(afxSimulation sim, afxNat first, afxNat cnt, afxDrawInput inputs[]);
AKX afxNat          AfxRecenterAllMotorClocks(afxSimulation sim, afxReal dCurrentClock);

AKX void            AfxQuerySimulationErrorTolerance(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler);

AKX void            AfxComputeBasisConversion(afxSimulation sim, afxReal unitsPerMeter, afxV3d const right, afxV3d const up, afxV3d const back, afxV3d const origin, afxM3d ltm, afxM3d iltm, afxV3d atv);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxNat          AfxCountSimulations(void);
AKX afxClass*     AfxGetSimulationClass(void);
AKX afxNat          AfxEnumerateSimulations(afxNat first, afxNat cnt, afxSimulation simulations[]);
AKX afxNat          AfxInvokeSimulations(afxNat first, afxNat cnt, afxBool(*f)(afxSimulation, void*), void *udd);
AKX afxError        AfxAcquireSimulations(afxNat cnt, akxSimulationConfig const config[], afxSimulation simulations[]);

#endif//AFX_SIMULATION_H
