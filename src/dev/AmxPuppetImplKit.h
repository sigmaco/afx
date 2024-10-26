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

#ifndef AMX_PUPPET_IMPL_KIT_H
#define AMX_PUPPET_IMPL_KIT_H

#include "qwadro/inc/sim/dev/afxSimulation.h"

#ifdef _AMX_BODY_C
AFX_OBJECT(afxBody)
{
    afxModel            mdl;
    afxSkeleton         skl;
    afxChain            motives;
    void*               userData[4];
    afxUnit32            reserved0,
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

    afxPose             pose;
    afxM4d              placement;
};
#endif//_AMX_BODY_C

#ifdef _AMX_MOTION_C

AFX_DEFINE_STRUCT(akxPeriodicLoop)
{
    afxReal             radius;
    afxReal             dAngle;
    afxReal             dZ;
    afxV3d              basisX;
    afxV3d              basisY;
    afxV3d              axis;
};

AFX_OBJECT(afxMotion)
{
    afxString           id;
    afxMask             flags;
    afxUnit              pivotCnt; // circuit count
    afxMotionTransform* pivotCurve; // uma arranjo de registros da moção de junta de afxSkeleton do correspondente afxModel.
    afxString*          pivots; // circuit id compatible with skeleton joints.
    //afxUnit              pivotLodErrorCnt;
    afxReal*            pivotLodError;
    afxUnit              vecCnt;
    afxMotionVector*    vecCurve;
    afxString*          vectors;
    afxTransform        displacement;
    afxMotionTransform* root;
    afxV3d              loopTranslation;
    akxPeriodicLoop*    periodicLoop;

#if 0
    afxUnit              curveCnt;
    afxCurve*           curves;
    struct
    {
        afxUnit          baseCurIdx;
        afxUnit          curCnt;
    }*xforms;
    afxMask*            xformFlag;
    afxCurve*           deformation;
    afxCurve*           translation;
    afxCurve*           transmission;
    afxString*          articulation;
#endif
};
#endif//_AMX_MOTION_C


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
    afxUnit          srcTrackIdx; // short afxMotionTransform
    afxQuatBlend quatMode; // char 0x02
    akxBoundMotionTransformFlags flags; // char 0x03
    afxMotionTransform const* srcTrack;
    void(*sampler)(const sample_context *, const afxMotionTransform *, akxBoundMotionTransform *, const afxTransform *, const float *, const float *, float *, float *);
    afxReal         lodErr;
    afxTransform    lodTransform;
};

#ifdef _AMX_ANIMATION_C

struct animation_binding_identifier
{
    afxAnimation Animation;
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
    afxUnit TrackBindingCount; // 32
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
    afxBody OnInstance; // 32
    akxAccumulateOp AccumulationMode; // 36
    float LODCopyScaler; // 40
    akxTrackMask *TrackMask; // 44
    akxTrackMask *ModelMask; // 48
};

AFX_DEFINE_STRUCT(akxLinkedMotion)
{
    afxMotion           mot;
    /*
    // --- identifier
    afxAnimation Animation;
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

AFX_OBJECT(afxAnimation)
{
    afxUnit              motSlotCnt;
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
    afxPose Pose; // 0
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
    afxMotor            moto;
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
    void(*AccumulateBindingState)(akxMotive*, afxUnit, afxUnit, afxPose*, afxReal, afxUnit const*);
    void(*BuildBindingDirect)(akxMotive*, afxUnit, afxReal const*, afxPoseBuffer*, afxReal);
    void(*AccumulateLoopTransform)(akxMotive*, afxReal, afxReal*, afxReal*, afxReal*, afxBool);
    void(*CleanupBindingState)(akxMotive*);
};

#ifdef _AMX_MOTOR_C
typedef enum afxMotorFlag
{
    afxMotorFlag_ACTIVE = AFX_BIT(0),
    afxMotorFlag_KILL_ONCE_COMPLETE = AFX_BIT(1),
    afxMotorFlag_KILL_ONCE_UNUSED = AFX_BIT(2),
    afxMotorFlag_EASE_IN = AFX_BIT(3),
    afxMotorFlag_EASE_OUT = AFX_BIT(4),
    afxMotorFlag_FORCE_CLAMPLED_LOOPS = AFX_BIT(5)
} afxMotorFlags;

AFX_OBJECT(afxMotor)
{
    afxMotorFlags   flags;
    afxReal         dtLocalClockPending;
    afxReal         localClock;
    afxReal         speed;
    afxReal         localDur;
    afxInt          currIterIdx;
    afxInt          iterCnt;
    afxReal         currWeight;
    afxMotorTiming  timing;
    afxUnit32        easeInValues;
    afxUnit32        easeOutValues;
    void*           userData[4];
};
#endif//_AMX_MOTOR_C

typedef struct smt
{
    void(*SampleTrackUUULocalAtTime0)(afxMotionTransform const* SourceTrack, afxTransform *Result);
    void(*SampleTrackUUULocal)(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, afxTransform *Result);
    void(*SampleTrackUUU)(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackIII)(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCII)(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCCI)(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCAI)(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackAII)(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackACI)(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackAAI)(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*Samplers[3][3][3])(const sample_context *, const afxMotionTransform *, akxBoundMotionTransform *, const afxTransform *, const afxM4d, const afxM4d, afxM4d, afxM4d);
    void(*SampleTrackSSS)(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*(*GetTrackSamplerFor)(const afxMotionTransform *Track))(const sample_context *, const afxMotionTransform *, akxBoundMotionTransform *, const afxTransform *, const afxM4d, const afxM4d, afxM4d, afxM4d);
    void(*SampleTrackUUUAtTime0)(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult);
    void(*SampleTrackUUUBlendWithTime0)(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult, afxReal BlendAmount);
    void(*SampleTrackIIU)(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackPOLocal)(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, afxV3d ResultPosition, afxQuat ResultOrientation);
} smt;

AMX smt Smt;

#endif//AMX_PUPPET_IMPL_KIT_H
