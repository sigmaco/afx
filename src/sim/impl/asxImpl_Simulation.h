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

#include "qwadro/inc/sim/afxSimulation.h"

#define ASX_MAX_SIM_BRIDGE_PER_CONTEXT 16

AFX_DEFINE_STRUCT(_asxSimulationAcquisition)
{
    afxUnit             bridgeCnt;
    afxEngineFeatures   requirements;
    afxUnit             extensionCnt;
    afxString const*    extensions;
    void*               udd;

    afxDrawSystem       dsys;
    afxDrawInput        din;

    afxBox              extent;

    afxV3d              right;
    afxV3d              up;
    afxV3d              back;
    afxV3d              origin;
    afxReal             unitsPerMeter;
    afxReal             allowedLodErrFadingFactor;

    afxClassConfig const* sbufClsCfg;
    afxClassConfig const* curClsCfg;
    afxClassConfig const* motClsCfg;
    afxClassConfig const* capsClsCfg;
    afxClassConfig const* aniClsCfg;
    afxClassConfig const* anikClsCfg;
    afxClassConfig const* bodClsCfg;
    afxClassConfig const* motvClsCfg;
    afxClassConfig const* mdlClsCfg;
    afxClassConfig const* mshClsCfg;
    afxClassConfig const* mtlClsCfg;
    afxClassConfig const* poseClsCfg;
    afxClassConfig const* plceClsCfg;
    
    afxClassConfig const* terClsCfg;
    afxClassConfig const* litClsCfg;
    afxClassConfig const* rndClsCfg;

    afxClassConfig const* sexuClsCfg;
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

    afxClass      bodCls;
    afxClass      motoCls;
    afxClass      lights;

    afxClass      motCls;
    afxClass      motvCls;
    afxClass      aniCls;
    afxClass      anikCls;
    afxClass      nodCls;
    afxClass      renderers;
    afxClass      terCls;
    afxClass        scnCls;

    afxClass        sklCls;
    afxClass        poseCls;
    afxClass        plceCls;
    afxClass        curCls;
    afxClass        sbufCls;

    afxClass        mtlCls;
    afxClass        mshCls;
    afxClass        mdlCls;

    afxStringBase   strbMdlSklMotUrns;
    afxStringBase   strbJointBiasesTags;
    afxStringBase   strbMorphTags;

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

    asxMotive       globalCapstanInterlinks;

    afxError(*waitCb)(afxSimulation,afxTime);

    afxDrawSystem  dsys;
    afxDrawInput    din;

    struct smt* Smt;
    struct smt2* Smt2;
    struct _asxAnimVmt const*animVmt;

    struct _afxSimIdd* idd;
    void* udd; // user-defined data
};
#endif//_ASX_SIMULATION_C

AFX_DEFINE_STRUCT(_asxAnimVmt)
{
    void(*PoseAccumulateBindingState)(asxMotive, afxUnit, afxUnit, afxPose, afxReal, afxUnit const*);
    void(*AnimationAccumulateBindingState)(asxMotive, afxUnit, afxUnit, afxPose, afxReal, const afxUnit *);
    void(*AnimationAccumulateLoopTransform)(asxMotive, afxReal, afxReal*, afxV3d, afxV3d, afxBool);
    void(*AnimationBuildDirect)(asxMotive, afxUnit, afxM4d const, afxPlacement, afxReal);
    void(*PoseBuildDirect)(asxMotive, afxUnit, afxM4d const, afxPlacement);
};

ASX _asxAnimVmt const * _AsxSimGetAnimVmt(afxSimulation sim);

ASX afxReal _AsxGetAllowedLodErrorFadingFactor(afxSimulation sim);

ASX afxClass const* _AsxGetBufferClass(afxSimulation sim);
ASX afxClass const* _AsxGetCurveClass(afxSimulation sim);
ASX afxClass const* _AsxGetPoseClass(afxSimulation sim);
ASX afxClass const* _AsxGetPlacementClass(afxSimulation sim);

ASX afxClass const* _AsxGetMaterialClass(afxSimulation sim);
ASX afxClass const* _AsxGetModelClass(afxSimulation sim);
ASX afxClass const* _AsxGetMeshClass(afxSimulation sim);

ASX afxClass const* _AsxGetCapstanClass(afxSimulation sim);
ASX afxClass const* _AsxGetBodyClass(afxSimulation sim);
ASX afxClass const* _AsxGetTerrainClass(afxSimulation sim);

ASX afxClass const* _AsxGetMotionClass(afxSimulation sim);
ASX afxClass const* _AsxGetMotiveClass(afxSimulation sim);
ASX afxClass const* _AsxGetAnimationClass(afxSimulation sim);
ASX afxClass const* _AsxGetInstancedAnimationClass(afxSimulation sim);

ASX afxClass const* _AsxGetLightClass(afxSimulation sim);
ASX afxClass const* _AsxGetNodeClass(afxSimulation sim);
ASX afxClass const* _AsxGetRendererClass(afxSimulation sim);

ASX afxClass const* _AsxGetSceneClass(afxSimulation sim);

ASX afxStringBase   _AsxGetModelUrnStringBase(afxSimulation sim);
ASX afxStringBase   _AsxGetPivotTagStringBase(afxSimulation sim);
ASX afxStringBase   _AsxGetMorphTagStringBase(afxSimulation sim);


ASX afxClassConfig const _ASX_SIM_CLASS_CONFIG;

typedef struct smt
{
    void(*SampleTrackUUULocalAtTime0)(afxPivotalMotion const* SourceTrack, afxTransform *Result);
    void(*SampleTrackUUULocal)(const afxSampleContext *Context, const afxPivotalMotion *SourceTrack, asxCachedMotionTransform *Track, afxTransform *Result);
    void(*SampleTrackUUU)(const afxSampleContext *Context, const afxPivotalMotion *SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackIII)(const afxSampleContext *Context, const afxPivotalMotion*SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCII)(const afxSampleContext *Context, const afxPivotalMotion*SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCCI)(const afxSampleContext *Context, const afxPivotalMotion*SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCAI)(const afxSampleContext *Context, const afxPivotalMotion*SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackAII)(const afxSampleContext *Context, const afxPivotalMotion*SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackACI)(const afxSampleContext *Context, const afxPivotalMotion*SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackAAI)(const afxSampleContext *Context, const afxPivotalMotion*SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*Samplers[3][3][3])(const afxSampleContext *, const afxPivotalMotion *, asxCachedMotionTransform *, const afxTransform *, const afxM4d, const afxM4d, afxM4d, afxM4d);
    void(*SampleTrackSSS)(const afxSampleContext *Context, const afxPivotalMotion *SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*(*GetTrackSamplerFor)(const afxPivotalMotion *Track))(const afxSampleContext *, const afxPivotalMotion *, asxCachedMotionTransform *, const afxTransform *, const afxM4d, const afxM4d, afxM4d, afxM4d);
    void(*SampleTrackUUUAtTime0)(const afxSampleContext *Context, const afxPivotalMotion *SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult);
    void(*SampleTrackUUUBlendWithTime0)(const afxSampleContext *Context, const afxPivotalMotion *SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult, afxReal BlendAmount);
    void(*SampleTrackIIU)(const afxSampleContext *Context, const afxPivotalMotion *SourceTrack, asxCachedMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackPOLocal)(const afxSampleContext *Context, const afxPivotalMotion *SourceTrack, asxCachedMotionTransform *Track, afxV3d ResultPosition, afxQuat ResultOrientation);
} smt;

typedef struct smt2
{
    void(*CubicCoefficients)(afxReal const a[3], afxReal const b[3], afxReal t, afxReal* ci_3, afxReal* ci_2, afxReal* ci_1, afxReal* ci);
    void(*SampleBSpline)(afxUnit Degree, afxUnit Dimension, afxBool Normalize, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    char(*ConstructBSplineBuffers)(afxInt dim, afxCurve prev, afxCurve c, afxCurve next, afxReal prevCurDur, afxReal curDur, afxReal nextCurDur, afxInt knotIdx, afxReal* ti, afxReal* pi, afxReal** tiPtr, afxReal** piPtr, afxReal const* identityVec);
    void(*EvaluateCurve)(const afxSampleContext *Context, afxCurve Curve, const afxReal *IdentityVector, int Dimension, afxReal *Result, afxBool CurveIsNormalized);
} smt2;

#endif//ASX_IMPL___SIMULATION_H
