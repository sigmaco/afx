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

#include "qwadro/core/afxClass.h"
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
    void(*UncheckedSampleBSplineN)(int Degree, int Dimension, const float *ti, const float *pi, float t, float *Result);
    void(*UncheckedSampleBSpline)(int Degree, int Dimension, const float *ti, const float *pi, float t, float *Result);
    void(*SampleTrackUUULocalAtTime0)(awxMotionTransform const* SourceTrack, afxTransform *Result);
    void(*SampleTrackUUULocal)(const sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, afxTransform *Result);
    void(*SampleTrackUUU)(const sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4Aliased, const float *ParentMatrixAliased, float *WorldResultAliased, float *CompositeResultAliased);
    void(*SampleTrackIII)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4Aliased, const float *ParentMatrixAliased, float *WorldResultAliased, float *CompositeResultAliased);
    void(*SampleTrackCII)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4Aliased, const float *ParentMatrixAliased, float *WorldResultAliased, float *CompositeResultAliased);
    void(*SampleTrackCCI)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4Aliased, const float *ParentMatrixAliased, float *WorldResultAliased, float *CompositeResultAliased);
    void(*SampleTrackCAI)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4Aliased, const float *ParentMatrixAliased, float *WorldResultAliased, float *CompositeResultAliased);
    void(*SampleTrackAII)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4Aliased, const float *ParentMatrixAliased, float *WorldResultAliased, float *CompositeResultAliased);
    void(*SampleTrackACI)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4Aliased, const float *ParentMatrixAliased, float *WorldResultAliased, float *CompositeResultAliased);
    void(*SampleTrackAAI)(const sample_context *Context, const awxMotionTransform*SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4Aliased, const float *ParentMatrixAliased, float *WorldResultAliased, float *CompositeResultAliased);
    void(*Samplers[3][3][3])(const sample_context *, const awxMotionTransform *, awxBoundMotionTransform *, const afxTransform *, const float *, const float *, float *, float *);
    void(*SampleTrackSSS)(const struct sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4Aliased, const float *ParentMatrixAliased, float *WorldResultAliased, float *CompositeResultAliased);
    void(*(*GetTrackSamplerFor)(const awxMotionTransform *Track))(const struct sample_context *, const awxMotionTransform *, awxBoundMotionTransform *, const afxTransform *, const float *, const float *, float *, float *);
    int(*CurveKnotCharacter)(const afxCurve *Curve);
    void(*SampleTrackUUUAtTime0)(const struct sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4, const float *ParentMatrix, float *WorldResult, float *CompositeResult);
    void(*SampleTrackUUUBlendWithTime0)(const struct sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4, const float *ParentMatrix, float *WorldResult, float *CompositeResult, float BlendAmount);
    void(*SampleTrackIIU)(const struct sample_context *Context, const awxMotionTransform *SourceTrack, awxBoundMotionTransform *Track, const afxTransform *RestTransform, const float *InverseWorld4x4Aliased, const float *ParentMatrixAliased, float *WorldResultAliased, float *CompositeResultAliased);
    void(*CubicCoefficients)(afxReal const a[3], afxReal const b[3], afxReal t, afxReal* ci_3, afxReal* ci_2, afxReal* ci_1, afxReal* ci);
    void(*SampleBSpline)(afxNat Degree, afxNat Dimension, afxBool Normalize, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    char(*ConstructBSplineBuffers)(afxInt dim, afxCurve const* prev, afxCurve const* c, afxCurve const* next, afxReal prevCurDur, afxReal curDur, afxReal nextCurDur, afxInt knotIdx, afxReal* ti, afxReal* pi, afxReal** tiPtr, afxReal** piPtr, afxReal const* identityVec);
    void(*EvaluateCurve)(const struct sample_context *Context, const afxCurve *Curve, const float *IdentityVector, int Dimension, float *Result, afxBool CurveIsNormalized);
} smt;

AKX smt Smt;

typedef struct mmt
{
    void(*BuildSingleCompositeFromWorldPose_Generic)(const afxReal InverseWorld4x4[4][4], const afxReal WorldMatrix[4][4], afxReal ResultComposite[4][4]);
    void(*BuildSingleCompositeFromWorldPoseTranspose_Generic)(const afxReal InverseWorld4x4[4][4], const afxReal WorldMatrix[4][4], afxReal ResultComposite3x4[3][4]);
    void(*BuildPositionWorldPoseOnly_Generic)(afxReal const Position[3], afxReal const ParentMatrix[4][4], afxReal ResultWorldMatrix[4][4]);
    void(*BuildPositionWorldPoseComposite_Generic)(afxReal const Position[3], afxReal const ParentMatrix[4][4], afxReal const InverseWorld4x4[4][4], afxReal ResultComposite[4][4], afxReal ResultWorldMatrix[4][4]);
    void(*BuildPositionOrientationWorldPoseOnly_Generic)(afxReal const Position[3], afxReal const Orientation[4], afxReal const ParentMatrix[4][4], afxReal ResultWorldMatrix[4][4]);
    void(*BuildPositionOrientationWorldPoseComposite_Generic)(afxReal const Position[3], afxReal const Orientation[4], afxReal const ParentMatrix[4][4], afxReal const InverseWorld4x4[4][4], afxReal ResultComposite[4][4], afxReal ResultWorldMatrix[4][4]);
    void(*BuildIdentityWorldPoseOnly_Generic)(afxReal const ParentMatrix[4][4], afxReal ResultWorldMatrix[4][4]);
    void(*BuildIdentityWorldPoseComposite_Generic)(afxReal const ParentMatrix[4][4], afxReal const InverseWorld4x4[4][4], afxReal ResultComposite[4][4], afxReal ResultWorldMatrix[4][4]);
    void(*BWP_Dispatch)(afxTransform *t, afxReal const parentW[4][4], afxReal const iw[4][4], afxReal composite[4][4], afxReal w[4][4]);
    void(*BWPNC_Dispatch)(afxTransform *t, afxReal const parentW[4][4], afxReal w[4][4]);
    void(*BuildFullWorldPoseOnly_Generic)(afxTransform const* t, const afxReal *ParentMatrix, afxReal *ResultWorldMatrix);
    void(*BuildFullWorldPoseComposite_Generic)(afxTransform const* t, const afxReal *ParentMatrix, const afxReal *InverseWorld4x4, afxReal *ResultComposite, afxReal *ResultWorldMatrix);
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
    afxClass                assets;
    afxClass                bodies;
    afxClass                motors;
    afxClass                entities;
    afxClass                lights;
    afxClass                materials;
    afxClass                meshes;
    afxClass                meshDatas;
    afxClass                topologies;
    afxClass                models;
    afxClass                skeletons;
    afxClass                animations;
    afxClass                motions;
    afxClass                nodes;
    afxClass                renderers;

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

AKX afxClass*       AwxGetMotorClass(afxSimulation sim);
AKX afxClass*       AfxGetBodyClass(afxSimulation sim);
AKX afxClass*       AfxGetEntityClass(afxSimulation sim);
AKX afxClass*       AfxGetLightClass(afxSimulation sim);
AKX afxClass*       AfxGetMaterialClass(afxSimulation sim);
AKX afxClass*       AwxGetMeshClass(afxSimulation sim);
AKX afxClass*       AwxGetMeshDataClass(afxSimulation sim);
AKX afxClass*       AwxGetMeshTopologyClass(afxSimulation sim);
AKX afxClass*       AwxGetModelClass(afxSimulation sim);
AKX afxClass*       AfxGetNodeClass(afxSimulation sim);
AKX afxClass*       AfxGetRendererClass(afxSimulation sim);
AKX afxClass*       AfxGetSkeletonClass(afxSimulation sim);

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
AKX afxClass*       AfxGetSimulationClass(void);
AKX afxNat          AfxEnumerateSimulations(afxNat first, afxNat cnt, afxSimulation simulations[]);
AKX afxNat          AfxInvokeSimulations(afxNat first, afxNat cnt, afxBool(*f)(afxSimulation, void*), void *udd);
AKX afxError        AfxAcquireSimulations(afxNat cnt, awxSimulationConfig const config[], afxSimulation simulations[]);

#endif//AFX_SIMULATION_H
