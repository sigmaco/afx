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

#ifndef AKX_SPATIAL_DEV_KIT_H
#define AKX_SPATIAL_DEV_KIT_H

#include "qwadro/sim/dev/afxSimulation.h"

#ifdef _AKX_CURVE_C
AFX_OBJECT(afxCurve)
{
    akxCurveFormat  fmt;
    afxNat          degree;
    afxNat          dimensionality;
    akxCurveFlags   flags;
    afxNat          knotCnt;
    union
    {
        afxReal*    knots; // 32f
        afxReal     knot0;
    };
    afxNat          ctrlCnt;
    union
    {
        afxV4d      ctrls4; // D4, Constant32f
        afxV3d      ctrls3;
        afxReal*    ctrls; // 32f
    };
};
#endif//_AKX_CURVE_C

#ifdef _AKX_POSE_C
AFX_DEFINE_STRUCT(akxArticulation)
{
    afxReal             weight;
    afxNat              cnt;
    afxTransform        xform;
    afxNat              traversalId;
};

AFX_OBJECT(akxPose)
{
    afxNat              artCnt;
    akxArticulation*    arts;
    afxReal             fillThreshold;
    afxNat              traversalId;
};
#endif//_AKX_POSE_C

#ifdef _AKX_POSE_BUFFER_C
AFX_OBJECT(akxPoseBuffer)
{
    afxNat      matCnt;
    afxM4d*     world;
    afxM4d*     composite;
};
#endif//_AKX_POSE_BUFFER_C

#ifdef _AKX_BODY_C
AFX_OBJECT(akxBody)
{
    afxModel            mdl;
    afxSkeleton         skl;
    afxChain            motives;
    void*               userData[4];
    afxNat32            reserved0,
                        reserved1;

    afxM4d matrix;
    afxM4d collWorldMtx;
    afxM4d invWorldInertiaMtx;
    afxQuat rot;

    afxV3d veloc;
    afxV3d omega;
    afxV3d accel;
    afxV3d alpha;
    afxV3d netForce;
    afxV3d netTorque;
    afxV3d prevExtlForce;
    afxV3d prevExtlTorque;

    afxV3d mass;
    afxV3d invMass;
    afxV3d aparentMass;
    afxV3d localCentreOfMass;
    afxV3d globalCentreOfMass;
    afxBox aabb;
    afxV3d dampCoef;

    int freeze : 1;
    int sleeping : 1;
    int autoSleep : 1;
    int isInWorld : 1;
    int equilibrium : 1;
    int continueCollisionMode : 1;
    int spawnnedFromCallback : 1;
    int collideWithLinkedBodies : 1;
    int solverInContinueCollision : 1;
    int inCallback : 1;

    akxPose             pose;
    afxM4d              placement;
};
#endif//_AKX_BODY_C

#ifdef _AKX_MOTION_C

AFX_DEFINE_STRUCT(akxPeriodicLoop)
{
    afxReal             radius;
    afxReal             dAngle;
    afxReal             dZ;
    afxV3d              basisX;
    afxV3d              basisY;
    afxV3d              axis;
};

AFX_OBJECT(akxMotion)
{
    afxString           id;
    afxMask             flags;
    afxNat              pivotCnt; // circuit count
    akxMotionTransform* pivotCurve; // uma arranjo de registros da moção de junta de afxSkeleton do correspondente afxModel.
    afxString*          pivots; // circuit id compatible with skeleton joints.
    //afxNat              pivotLodErrorCnt;
    afxReal*            pivotLodError;
    afxNat              vecCnt;
    akxMotionVector*    vecCurve;
    afxString*          vectors;
    afxTransform        displacement;
    akxMotionTransform* root;
    afxV3d              loopTranslation;
    akxPeriodicLoop*    periodicLoop;

#if 0
    afxNat              curveCnt;
    afxCurve*           curves;
    struct
    {
        afxNat          baseCurIdx;
        afxNat          curCnt;
    }*xforms;
    afxMask*            xformFlag;
    afxCurve*           deformation;
    afxCurve*           translation;
    afxCurve*           transmission;
    afxString*          articulation;
#endif
};
#endif//_AKX_MOTION_C

#ifdef _AKX_SKELETON_C
AFX_OBJECT(afxSkeleton)
{
    afxNat              jointCnt;
    afxNat*             parentIdx;
    afxTransform*       local;
    afxM4d*             iw;
    afxReal*            lodError;
    afxString*          joints;
    void**              udd;

    afxReal             allowedLodErrFadingFactor;
    afxNat              lodType;
    afxString           urn; // 32
};
#endif//_AKX_SKELETON_C


/*
    Body
    Animation
        Linked Motion
            Motion
            Skeleton
*/

typedef enum akxBoundMotionTransformFlag
{
    akxBoundMotionTransformFlag_TranslationIsIdentity = 0x0,
    akxBoundMotionTransformFlag_TranslationIsConstant = 0x1,
    akxBoundMotionTransformFlag_TranslationIsKeyframed = 0x2,
    akxBoundMotionTransformFlag_TranslationIsGeneral = 0x3,
    akxBoundMotionTransformFlag_TranslationMask = 0x3,

    akxBoundMotionTransformFlag_TransmissionIsIdentity = 0x0,
    akxBoundMotionTransformFlag_TransmissionIsConstant = 0x4,
    akxBoundMotionTransformFlag_TransmissionIsKeyframed = 0x8,
    akxBoundMotionTransformFlag_TransmissionIsGeneral = 0xC,
    akxBoundMotionTransformFlag_TransmissionMask = 0xC,

    akxBoundMotionTransformFlag_TransmutationIsIdentity = 0x0,
    akxBoundMotionTransformFlag_TransmutationIsConstant = 0x10,
    akxBoundMotionTransformFlag_TransmutationIsKeyframed = 0x20,
    akxBoundMotionTransformFlag_TransmutationIsGeneral = 0x30,
    akxBoundMotionTransformFlag_TransmutationMask = 0x30,
} akxBoundMotionTransformFlags;

typedef struct sample_context sample_context;

AFX_DEFINE_STRUCT(akxBoundMotionTransform)
{
    afxNat          srcTrackIdx; // short akxMotionTransform
    akxQuatBlend quatMode; // char 0x02
    akxBoundMotionTransformFlags flags; // char 0x03
    akxMotionTransform const* srcTrack;
    void(*sampler)(const sample_context *, const akxMotionTransform *, akxBoundMotionTransform *, const afxTransform *, const float *, const float *, float *, float *);
    afxReal         lodErr;
    afxTransform    lodTransform;
};

#ifdef _AKX_ANIMATION_C

struct animation_binding_identifier
{
    akxAnimation Animation;
    int motIdx;
    const char *TrackPattern;
    const char *BonePattern;
    afxModel OnModel;
    afxModel FromBasis;
    afxModel ToBasis;
};

struct animation_binding
{
    struct animation_binding_identifier ID;

    void *RebasingMemory; // 28
    afxNat TrackBindingCount; // 32
    akxBoundMotionTransform *TrackBindings; // 36 // one for each skeleton pivot
    struct animation_binding *Left;
    struct animation_binding *Right;
    int UsedBy;
    struct animation_binding *PreviousUnused;
    struct animation_binding *NextUnused;
};

struct track_target // draft
{
    struct animation_binding *Binding;
    struct animation_binding_identifier BindingID; // size = 28
    akxBody OnInstance; // 32
    akxAccumulateOp AccumulationMode; // 36
    float LODCopyScaler; // 40
    akxTrackMask *TrackMask; // 44
    akxTrackMask *ModelMask; // 48
};

AFX_DEFINE_STRUCT(akxLinkedMotion)
{
    akxMotion           mot;
    /*
    // --- identifier
    akxAnimation Animation;
    int SourceTrackGroupIndex;
    const char *TrackPattern;
    const char *BonePattern;
    const afxModel *OnModel;
    afxModel *FromBasis;
    afxModel *ToBasis;

    // ... binding
    void *RebasingMemory;
    int trackCnt;
    akxBoundMotionTransform *tracks;
    */


};

AFX_OBJECT(akxAnimation)
{
    afxNat              motSlotCnt;
    akxLinkedMotion*    motSlots;

    /// @timeStep é o intervalo após oversampling.
    /// Quando @timeStep e @oversampling forem iguais a 0.00416667 e 4.0, respectivamente, uma animação foi produzida a 60 FPS (0.016667 segundos entre cada frame) com oversampling de 4x.
    /// Multiplicando @timeStep por @oversampling, pode-se encontrar o intervalo original antes do oversampling.
    /// Tanto @timeStep quanto @oversampling não são usadas pelo Qwadro.
    /// Estão aqui de forma informativa preservando conhecimento sobre a intenção original do autor.
    afxReal             dur; /// especifica quão demorado, em segundos, a animação leva para executar a completação.
    afxReal             timeStep; /// especifica quão muitos segundos há entre cada frame.
    afxReal             oversampling; /// 1.f

    afxString           urn;
};
#endif

AFX_DECLARE_STRUCT(akxControlledAnimation);
AFX_DECLARE_STRUCT(akxControlledPose);

AFX_DECLARE_STRUCT(akxMotiveCallbacks);

typedef struct controlled_pose
{
    akxPose Pose; // 0
    const akxTrackMask *ModelMask; // 4
} controlled_pose;

typedef struct controlled_animation
{
    struct animation_binding *Binding; // 0
    akxAccumulateOp AccumulationMode; // 4
    const akxTrackMask *TrackMask; // 8
    const akxTrackMask *ModelMask; // 12
} controlled_animation;

AFX_DEFINE_STRUCT(akxMotive)
{
    akxMotor            moto;
    afxLinkage          bod;
    akxMotiveCallbacks const *callbacks;
    union
    {
        controlled_animation ca;
        controlled_pose      cp;
    };
    void*               reservedPtr;
};

AFX_DEFINE_STRUCT(akxMotiveCallbacks)
{
    struct controlled_animation*(*GetControlledAnimation)(akxMotive*);
    struct controlled_pose*(*GetControlledPose)(akxMotive*);
    afxBool(*InitializeBindingState)(akxMotive*, void*);
    void(*AccumulateBindingState)(akxMotive*, afxNat, afxNat, akxPose*, afxReal, afxNat const*);
    void(*BuildBindingDirect)(akxMotive*, afxNat, afxReal const*, akxPoseBuffer*, afxReal);
    void(*AccumulateLoopTransform)(akxMotive*, afxReal, afxReal*, afxReal*, afxReal*, afxBool);
    void(*CleanupBindingState)(akxMotive*);
};

#ifdef _AKX_MOTOR_C
typedef enum akxMotorFlags
{
    akxMotorFlags_ACTIVE = AFX_BIT(0),
    akxMotorFlags_KILL_ONCE_COMPLETE = AFX_BIT(1),
    akxMotorFlags_KILL_ONCE_UNUSED = AFX_BIT(2),
    akxMotorFlags_EASE_IN = AFX_BIT(3),
    akxMotorFlags_EASE_OUT = AFX_BIT(4),
    akxMotorFlags_FORCE_CLAMPLED_LOOPS = AFX_BIT(5)
} akxMotorFlags;

AFX_OBJECT(akxMotor)
{
    akxMotorFlags   flags;
    afxReal         currClock;
    afxReal         dtLocalClockPending;
    afxReal         localClock;
    afxReal         speed;
    afxReal         localDur;
    afxInt          currIterIdx;
    afxInt          iterCnt;
    afxReal         killClock;
    afxReal         currWeight;
    afxReal         easeInStartClock;
    afxReal         easeInEndClock;
    afxNat          easeInValues;
    afxReal         easeOutStartClock;
    afxReal         easeOutEndClock;
    afxNat          easeOutValues;
    void*           userData[4];
};
#endif//_AKX_MOTOR_C

typedef struct sample_context
{
    afxReal LocalClock;
    afxReal LocalDuration;
    afxBool UnderflowLoop;
    afxBool OverflowLoop;
    afxInt FrameIndex;
} sample_context;

typedef struct smt2
{
    void(*UncheckedSampleBSplineN)(int Degree, int Dimension, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    void(*UncheckedSampleBSpline)(int Degree, int Dimension, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    int(*CurveKnotCharacter)(afxCurve Curve);
    void(*CubicCoefficients)(afxReal const a[3], afxReal const b[3], afxReal t, afxReal* ci_3, afxReal* ci_2, afxReal* ci_1, afxReal* ci);
    void(*SampleBSpline)(afxNat Degree, afxNat Dimension, afxBool Normalize, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    char(*ConstructBSplineBuffers)(afxInt dim, afxCurve prev, afxCurve c, afxCurve next, afxReal prevCurDur, afxReal curDur, afxReal nextCurDur, afxInt knotIdx, afxReal* ti, afxReal* pi, afxReal** tiPtr, afxReal** piPtr, afxReal const* identityVec);
    void(*EvaluateCurve)(const sample_context *Context, afxCurve Curve, const afxReal *IdentityVector, int Dimension, afxReal *Result, afxBool CurveIsNormalized);
} smt2;

AKX smt2 Smt2;

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
    void(*SampleTrackPOLocal)(const sample_context *Context, const akxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, afxV3d ResultPosition, afxQuat ResultOrientation);
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

#endif//AKX_SPATIAL_DEV_KIT_H
