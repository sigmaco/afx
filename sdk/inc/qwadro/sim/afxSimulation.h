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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef AFX_SIMULATION_H
#define AFX_SIMULATION_H

#include "qwadro/core/afxManager.h"
#include "qwadro/sim/awxEntity.h"
#include "qwadro/sim/rendering/awxLight.h"
#include "qwadro/sim/afxSkeleton.h"
#include "qwadro/sim/modeling/afxModel.h"
#include "qwadro/sim/motion/awxAnimation.h"
#include "qwadro/sim/afxMaterial.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/sim/motion/awxMotor.h"
#include "qwadro/sim/motion/awxMotion.h"
#include "qwadro/sim/rendering/awxRenderer.h"
#include "qwadro/sound/afxSoundDefs.h"

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. 
// By changing variables in the simulation, predictions may be made about the behaviour of the system. 
// It is a tool to virtually investigate the behaviour of the system under study.

// A space is a purely mathematical representation of a world within space and its interactions.

typedef struct smt
{
    void(*UncheckedSampleBSplineN)(int Degree, int Dimension, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    void(*UncheckedSampleBSpline)(int Degree, int Dimension, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    void(*SampleTrackUUULocalAtTime0)(awxMotionTransform const* SourceTrack, afxTransform *Result);
    void(*SampleTrackUUULocal)(const sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, afxTransform *Result);
    void(*SampleTrackUUU)(const sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxReal *InverseWorld4x4Aliased, const afxReal *ParentMatrixAliased, afxReal *WorldResultAliased, afxReal *CompositeResultAliased);
    void(*SampleTrackIII)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxReal *InverseWorld4x4Aliased, const afxReal *ParentMatrixAliased, afxReal *WorldResultAliased, afxReal *CompositeResultAliased);
    void(*SampleTrackCII)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxReal *InverseWorld4x4Aliased, const afxReal *ParentMatrixAliased, afxReal *WorldResultAliased, afxReal *CompositeResultAliased);
    void(*SampleTrackCCI)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxReal *InverseWorld4x4Aliased, const afxReal *ParentMatrixAliased, afxReal *WorldResultAliased, afxReal *CompositeResultAliased);
    void(*SampleTrackCAI)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxReal *InverseWorld4x4Aliased, const afxReal *ParentMatrixAliased, afxReal *WorldResultAliased, afxReal *CompositeResultAliased);
    void(*SampleTrackAII)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxReal *InverseWorld4x4Aliased, const afxReal *ParentMatrixAliased, afxReal *WorldResultAliased, afxReal *CompositeResultAliased);
    void(*SampleTrackACI)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxReal *InverseWorld4x4Aliased, const afxReal *ParentMatrixAliased, afxReal *WorldResultAliased, afxReal *CompositeResultAliased);
    void(*SampleTrackAAI)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxReal *InverseWorld4x4Aliased, const afxReal *ParentMatrixAliased, afxReal *WorldResultAliased, afxReal *CompositeResultAliased);
    void(*Samplers[3][3][3])(const sample_context *, const awxMotionTransform *, awxBoundMotionTransform *, const afxTransform *, const afxReal *, const afxReal *, afxReal *, afxReal *);
    void(*SampleTrackSSS)(const sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxReal *InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*(*GetTrackSamplerFor)(const awxMotionTransform *Track))(const sample_context *, const awxMotionTransform *, awxBoundMotionTransform *, const afxTransform *, const afxReal *, const afxReal *, afxReal *, afxReal *);
    int(*CurveKnotCharacter)(const afxCurve *Curve);
    void(*SampleTrackUUUAtTime0)(const sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult);
    void(*SampleTrackUUUBlendWithTime0)(const sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult, afxReal BlendAmount);
    void(*SampleTrackIIU)(const sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*CubicCoefficients)(afxReal const a[3], afxReal const b[3], afxReal t, afxReal* ci_3, afxReal* ci_2, afxReal* ci_1, afxReal* ci);
    void(*SampleBSpline)(afxNat Degree, afxNat Dimension, afxBool Normalize, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    char(*ConstructBSplineBuffers)(afxInt dim, afxCurve const* prev, afxCurve const* c, afxCurve const* next, afxReal prevCurDur, afxReal curDur, afxReal nextCurDur, afxInt knotIdx, afxReal* ti, afxReal* pi, afxReal** tiPtr, afxReal** piPtr, afxReal const* identityVec);
    void(*EvaluateCurve)(const sample_context *Context, const afxCurve *Curve, const afxReal *IdentityVector, int Dimension, afxReal *Result, afxBool CurveIsNormalized);
    void(*SampleTrackPOLocal)(const struct sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, afxV3d ResultPosition, afxQuat ResultOrientation);
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




AFX_DEFINE_STRUCT(awxSimulationConfig)
{
    afxUri const*           driver;
    afxAabb                 extent;
    afxDrawContext          dctx;
    afxDrawInput            din;
    afxSoundContext         sctx;
    afxV3d                  right;
    afxV3d                  up;
    afxV3d                  back;
    afxV3d                  origin;
    afxReal                 unitsPerMeter;
};

#ifdef _AFX_SIMULATION_C
AFX_OBJECT(afxSimulation)
{
    afxMmu              genrlMem;
    afxChain                classes;
    afxManager                assets;
    afxManager                bodies;
    afxManager                motors;
    afxManager                entities;
    afxManager                lights;
    afxManager                materials;
    afxManager                meshes;
    afxManager                meshDatas;
    afxManager                topologies;
    afxManager                models;
    afxManager                skeletons;
    afxManager                animations;
    afxManager                motions;
    afxManager                nodes;
    afxManager                renderers;

    afxV4d                  right;
    afxV4d                  up;
    afxV4d                  back;
    afxM3d                  basis;
    afxV4d                  origin;
    afxAabb                 extent;
    afxReal                 unitsPerMeter;
    afxReal                 allowedLodErrFadingFactor;

    afxSize                 stepNum, stepsPerSecond;
    afxSize                 stepTime, swapTime;
    afxReal                 stepDeltaTime;

    awxMotive    globalMotorInterlinks;

    afxDrawContext          dctx;
};
#endif//_AFX_SIMULATION_C

AKX afxMmu      AfxGetSimulationMmu(afxSimulation sim);

AKX afxManager*       AwxGetMotorClass(afxSimulation sim);
AKX afxManager*       AfxGetBodyClass(afxSimulation sim);
AKX afxManager*       AfxGetEntityClass(afxSimulation sim);
AKX afxManager*       AfxGetLightClass(afxSimulation sim);
AKX afxManager*       AfxGetMaterialClass(afxSimulation sim);
AKX afxManager*       AwxGetMeshClass(afxSimulation sim);
AKX afxManager*       AwxGetMeshDataClass(afxSimulation sim);
AKX afxManager*       AwxGetMeshTopologyClass(afxSimulation sim);
AKX afxManager*       AwxGetModelClass(afxSimulation sim);
AKX afxManager*       AfxGetNodeClass(afxSimulation sim);
AKX afxManager*       AfxGetRendererClass(afxSimulation sim);
AKX afxManager*       AfxGetSkeletonClass(afxSimulation sim);

AKX afxNat          AfxEnumerateBodies(afxSimulation sim, afxNat base, afxNat cnt, awxBody bod[]);
AKX afxNat          AfxEnumerateEntities(afxSimulation sim, afxNat base, afxNat cnt, awxEntity ent[]);
AKX afxNat          AfxEnumerateMaterials(afxSimulation sim, afxNat base, afxNat cnt, afxMaterial mtl[]);
AKX afxNat          AfxEnumerateMeshes(afxSimulation sim, afxNat base, afxNat cnt, afxMesh msh[]);
AKX afxNat          AfxEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel mdl[]);
AKX afxNat          AfxEnumerateSkeletons(afxSimulation sim, afxNat base, afxNat cnt, afxSkeleton skl[]);


AKX afxError        _AfxSimulationProcess(afxSimulation sim);

AKX afxError        AfxStepSimulation(afxSimulation sim);
AKX afxError        AfxCullSimulation(afxSimulation sim, afxCamera cam, afxArray* pvs);
AKX afxError        AfxRenderSimulation(afxSimulation sim, afxArray const* pvs);

AKX afxNat          AfxCountMotors(afxSimulation sim);
AKX afxNat          AfxInvokeMotors(afxSimulation sim, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AKX afxNat          AfxEnumerateMotors(afxSimulation sim, afxNat first, afxNat cnt, afxDrawInput inputs[]);
AKX afxNat          AfxRecenterAllMotorClocks(afxSimulation sim, afxReal dCurrentClock);

AKX void            AfxQuerySimulationErrorTolerance(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler);

AKX void            AfxComputeBasisConversion(afxSimulation sim, afxReal unitsPerMeter, afxReal const right[3], afxReal const up[3], afxReal const back[3], afxReal const origin[3], afxReal ltm[3][3], afxReal iltm[3][3], afxReal atv[3]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxNat          AfxCountSimulations(void);
AKX afxManager*       AfxGetSimulationClass(void);
AKX afxNat          AfxEnumerateSimulations(afxNat first, afxNat cnt, afxSimulation simulations[]);
AKX afxNat          AfxInvokeSimulations(afxNat first, afxNat cnt, afxBool(*f)(afxSimulation, void*), void *udd);
AKX afxError        AfxAcquireSimulations(afxNat cnt, awxSimulationConfig const config[], afxSimulation simulations[]);

#endif//AFX_SIMULATION_H
