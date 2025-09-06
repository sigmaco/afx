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

#ifndef ASX_IMPL___SIMULATION_H
#define ASX_IMPL___SIMULATION_H

#include "qwadro/sim/afxSimulation.h"
#include "../../render/ddi/arxImpl_Input.h"

#define ASX_MAX_SIM_BRIDGE_PER_CONTEXT 16

AFX_DEFINE_STRUCT(_asxSimulationAcquisition)
{
    afxUnit             bridgeCnt;
    afxEngineFeatures   requirements;
    afxUnit             extensionCnt;
    afxString const*    extensions;
    void*               udd;

    afxDrawSystem       dsys;
    arxRenderware        rwe;

    afxBox              extent;

    afxV3d              right;
    afxV3d              up;
    afxV3d              back;
    afxV3d              origin;
    afxReal             unitsPerMeter;
    afxReal             allowedLodErrFadingFactor;

    afxClassConfig const* sexuClsCfg;

    afxClassConfig const* curClsCfg;
    afxClassConfig const* motClsCfg;
    afxClassConfig const* capsClsCfg;
    afxClassConfig const* aniClsCfg;
    afxClassConfig const* anikClsCfg;
    afxClassConfig const* bodClsCfg;
    afxClassConfig const* motvClsCfg;
};

#ifdef _ASX_SIMULATION_C
#ifdef _ASX_SIMULATION_IMPL
AFX_OBJECT(_asxSimulation)
#else
AFX_OBJECT(afxSimulation)
#endif
{
    afxBool         running;

    afxUnit         bridgeCnt;
    afxSimBridge*   bridges;
    afxEngineFeatures requirements;

    afxChain        classes;
    afxClass        sexuCls;

    afxClass      nodCls;

    afxClass        shapCls;


    afxClass      curCls;
    afxClass      motCls;
    afxClass      motvCls;
    afxClass      aniCls;
    afxClass      anikCls;
    afxClass      motoCls;
    afxClass      bodCls;

    afxStringBase   strbAnimMotUrns;

    afxChain        dags;
    afxV4d          right;
    afxV4d          up;
    afxV4d          back;
    afxM3d          basis;
    afxV4d          origin;
    afxBox          extent;
    afxReal         unitsPerMeter;
    afxReal         allowedLodErrFadingFactor;

    afxSize         stepNum, stepsPerSecond;
    afxSize         stepTime, swapTime;
    afxReal         stepDeltaTime;

    arxMotive       globalCapstanInterlinks;

    afxError(*waitCb)(afxSimulation,afxTime);

    afxDrawSystem  dsys;
    arxRenderware    rwe;

    struct smt* Smt;
    struct smt2* Smt2;

    struct _afxSimIdd* idd;
    void* udd; // user-defined data
};
#endif//_ASX_SIMULATION_C

ASX afxChain const* _AsxSimGetDagRoots(afxSimulation sim);

ASX afxReal _AsxGetAllowedLodErrorFadingFactor(afxSimulation sim);

ASX afxClass const* _AsxGetShapClass(afxSimulation sim);

ASX afxClass const* _ArxGetNodeClass(afxSimulation sim);


ASX afxClassConfig const _ASX_SIM_CLASS_CONFIG;

typedef struct smt
{
    void(*SampleTrackUUULocalAtTime0)(arxPivotalMotion const* SourceTrack, afxTransform *Result);
    void(*SampleTrackUUULocal)(const arxSampleContext *Context, const arxPivotalMotion *SourceTrack, arxCachedMotionTransform *Track, afxTransform *Result);
    void(*SampleTrackUUU)(const arxSampleContext *Context, const arxPivotalMotion *SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackIII)(const arxSampleContext *Context, const arxPivotalMotion*SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCII)(const arxSampleContext *Context, const arxPivotalMotion*SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCCI)(const arxSampleContext *Context, const arxPivotalMotion*SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCAI)(const arxSampleContext *Context, const arxPivotalMotion*SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackAII)(const arxSampleContext *Context, const arxPivotalMotion*SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackACI)(const arxSampleContext *Context, const arxPivotalMotion*SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackAAI)(const arxSampleContext *Context, const arxPivotalMotion*SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*Samplers[3][3][3])(const arxSampleContext *, const arxPivotalMotion *, arxCachedMotionTransform *, const afxTransform *, const afxM4d, const afxM4d, afxM4d, afxM4d);
    void(*SampleTrackSSS)(const arxSampleContext *Context, const arxPivotalMotion *SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*(*GetTrackSamplerFor)(const arxPivotalMotion *Track))(const arxSampleContext *, const arxPivotalMotion *, arxCachedMotionTransform *, const afxTransform *, const afxM4d, const afxM4d, afxM4d, afxM4d);
    void(*SampleTrackUUUAtTime0)(const arxSampleContext *Context, const arxPivotalMotion *SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult);
    void(*SampleTrackUUUBlendWithTime0)(const arxSampleContext *Context, const arxPivotalMotion *SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult, afxReal BlendAmount);
    void(*SampleTrackIIU)(const arxSampleContext *Context, const arxPivotalMotion *SourceTrack, arxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackPOLocal)(const arxSampleContext *Context, const arxPivotalMotion *SourceTrack, arxCachedMotionTransform *Track, afxV3d ResultPosition, afxQuat ResultOrientation);
} smt;

typedef struct smt2
{
    void(*CubicCoefficients)(afxReal const a[3], afxReal const b[3], afxReal t, afxReal* ci_3, afxReal* ci_2, afxReal* ci_1, afxReal* ci);
    void(*SampleBSpline)(afxUnit Degree, afxUnit Dimension, afxBool Normalize, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    char(*ConstructBSplineBuffers)(afxInt dim, arxCurve prev, arxCurve c, arxCurve next, afxReal prevCurDur, afxReal curDur, afxReal nextCurDur, afxInt knotIdx, afxReal* ti, afxReal* pi, afxReal** tiPtr, afxReal** piPtr, afxReal const* identityVec);
    void(*EvaluateCurve)(const arxSampleContext *Context, arxCurve Curve, const afxReal *IdentityVector, int Dimension, afxReal *Result, afxBool CurveIsNormalized);
} smt2;

ARX afxClass const* _ArxGetCurveClass(afxSimulation sim);
ARX afxClass const* _ArxGetCapstanClass(afxSimulation sim);
ARX afxClass const* _ArxGetBodyClass(afxSimulation sim);
ARX afxClass const* _ArxGetMotionClass(afxSimulation sim);
ARX afxClass const* _ArxGetMotiveClass(afxSimulation sim);
ARX afxClass const* _ArxGetAnimationClass(afxSimulation sim);
ARX afxClass const* _ArxGetInstancedAnimationClass(afxSimulation sim);

ARX afxStringBase   _ArxRweGetAnimUrnStringBase(afxSimulation sim);

#endif//ASX_IMPL___SIMULATION_H
