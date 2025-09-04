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

#ifndef ARX_IMPL___ANIMATION_H
#define ARX_IMPL___ANIMATION_H

//#include "../impl/afxExecImplKit.h"
#include "qwadro/inc/render/arxRenderware.h"

typedef enum arxTrackSampler
{
    arxTrackSampler_UUU, // uniform position, orientation and scale/shear.
    arxTrackSampler_III, // identity position, orientation and scale/shear.
    arxTrackSampler_CII, // constant position, identity orientation and scale/shear.
    arxTrackSampler_CCI, // constant position and orientation, identity scale/shear.
    arxTrackSampler_CAI, // constant position, accumulative orientation, identity scale/shear.
    arxTrackSampler_AII, // accumulative position, identity orientation and scale/shear.
    arxTrackSampler_ACI, // accumulative position, constant orientation, identity scale/shear.
    arxTrackSampler_AAI, // accumulative position and orientation, identity scale/shear.
    arxTrackSampler_IIU, // identity position, identity orientation, uniform scale/shear
    arxTrackSampler_SSS // static position, orientation and scale/shear.
} arxTrackSampler;

#ifdef _ARX_MOTOR_C
typedef enum arxCapstanFlag
{
    arxCapstanFlag_ACTIVE = AFX_BITMASK(0),
    arxCapstanFlag_KILL_ONCE_COMPLETE = AFX_BITMASK(1),
    arxCapstanFlag_KILL_ONCE_UNUSED = AFX_BITMASK(2),
    arxCapstanFlag_EASE_IN = AFX_BITMASK(3),
    arxCapstanFlag_EASE_OUT = AFX_BITMASK(4),
    arxCapstanFlag_FORCE_CLAMPLED_LOOPS = AFX_BITMASK(5)
} arxCapstanFlags;

#ifdef _ARX_MOTOR_IMPL
AFX_OBJECT(_arxCapstan)
#else
AFX_OBJECT(arxCapstan)
#endif
{
    arxCapstanFlags   flags;
    afxReal         dtLocalClockPending;
    afxReal         localClock;
    afxReal         speed;
    afxReal         localDur;
    afxInt          currIterIdx;
    afxInt          iterCnt;
    afxReal         currWeight;
    arxCapstanTiming  timing;
    afxUnit32       easeInValues;
    afxUnit32       easeOutValues;
    void*           udd[4];
};
#endif//_ARX_MOTOR_C

#ifdef _ARX_MOTION_C

AFX_DEFINE_STRUCT(akxPeriodicLoop)
{
    afxReal             radius;
    afxReal             dAngle;
    afxReal             dZ;
    afxV3d              basisX;
    afxV3d              basisY;
    afxV3d              axis;
};

#ifdef _ASX_MOTION_IMPL
AFX_OBJECT(_asxMotion)
#else
AFX_OBJECT(arxMotion)
#endif
{
    // Gesture? Motion of body

    // Motion Geometry?
    // Slight inversion of "geometric motion", gives it a unique identity.
    // Feels more like a data structure or class name.
    // Less likely to be misinterpreted by search engines than "geometric motion".

    afxString           id;
    afxMask             flags;
    afxUnit             pivotCnt; // circuit count
    arxPivotalMotion*   pivotCurve; // uma arranjo de registros da moção de junta de arxSkeleton do correspondente arxModel.
    afxString*          pivots; // circuit id compatible with skeleton joints.
    //afxUnit              pivotLodErrorCnt;
    afxReal*            pivotLodError;
    afxUnit             vecCnt;
    arxVectorialMotion* vecCurve;
    afxString*          vectors;
    afxTransform        displacement;
    arxPivotalMotion*   root;
    afxV3d              loopTranslation;
    akxPeriodicLoop*    periodicLoop;

#if 0
    afxUnit             curveCnt;
    arxCurve*           curves;
    struct
    {
        afxUnit         baseCurIdx;
        afxUnit         curCnt;
    }*xforms;
    afxMask*            xformFlag;
    arxCurve*           deformation;
    arxCurve*           translation;
    arxCurve*           transmission;
    afxString*          articulation;
#endif
};
#endif//_ARX_MOTION_C


typedef enum arxCachedMotionTransformFlag
{
    arxCachedMotionTransformFlag_Identity = 0x0,

    arxCachedMotionTransformFlag_TranslationIsConstant = 0x1,
    arxCachedMotionTransformFlag_TranslationIsKeyframed = 0x2,
    arxCachedMotionTransformFlag_TranslationIsGeneral = 0x3,
    arxCachedMotionTransformFlag_TranslationMask = 0x3,

    arxCachedMotionTransformFlag_TransmissionIsConstant = 0x4,
    arxCachedMotionTransformFlag_TransmissionIsKeyframed = 0x8,
    arxCachedMotionTransformFlag_TransmissionIsGeneral = 0xC,
    arxCachedMotionTransformFlag_TransmissionMask = 0xC,

    arxCachedMotionTransformFlag_TransmutationIsConstant = 0x10,
    arxCachedMotionTransformFlag_TransmutationIsKeyframed = 0x20,
    arxCachedMotionTransformFlag_TransmutationIsGeneral = 0x30,
    arxCachedMotionTransformFlag_TransmutationMask = 0x30,
} arxCachedMotionTransformFlags;

typedef struct arxSampleContext arxSampleContext;

AFX_DEFINE_STRUCT(arxCachedMotionTransform)
{
    afxUnit          srcTrackIdx; // short arxPivotalMotion
    afxQuatBlend quatMode; // char 0x02
    arxCachedMotionTransformFlags flags; // char 0x03
    arxPivotalMotion const* srcTrack;
    void(*sampler)(const arxSampleContext *, const arxPivotalMotion *, arxCachedMotionTransform *, const afxTransform *, const float *, const float *, float *, float *);
    afxReal         lodErr;
    afxTransform    lodTransform;
};

AFX_DEFINE_HANDLE(arxInstancedAnimation);

AFX_DEFINE_STRUCT(arxInstancedAnimationId)
{
    arxAnimation Animation;
    int motIdx;
    const char *TrackPattern;
    const char *BonePattern;
    arxModel OnModel;
    arxModel FromBasis;
    arxModel ToBasis;
};

AFX_OBJECT(arxInstancedAnimation)
{
    arxInstancedAnimationId ID;

    void *RebasingMemory; // 28
    afxUnit TrackBindingCount; // 32
    arxCachedMotionTransform *TrackBindings; // 36 // one for each skeleton pivot
    arxInstancedAnimation Left;
    arxInstancedAnimation Right;
    int UsedBy;
    arxInstancedAnimation PreviousUnused;
    arxInstancedAnimation NextUnused;
};

AFX_DEFINE_STRUCT(arxTrackTarget) // draft
{
    arxInstancedAnimation Binding;
    arxInstancedAnimationId BindingID; // size = 28
    arxBody OnInstance; // 32
    afxAccumOp AccumulationMode; // 36
    float LODCopyScaler; // 40
    akxTrackMask *TrackMask; // 44
    akxTrackMask *ModelMask; // 48
};

#ifdef _ARX_ANIMATION_C

AFX_DEFINE_STRUCT(akxLinkedMotion)
{
    arxMotion           mot;
    /*
    // --- identifier
    arxAnimation Animation;
    int SourceTrackGroupIndex;
    const char *TrackPattern;
    const char *BonePattern;
    const arxModel *OnModel;
    arxModel *FromBasis;
    arxModel *ToBasis;

    // ... binding
    void *RebasingMemory;
    int trackCnt;
    arxCachedMotionTransform *tracks;
    */


};

#ifdef _ARX_ANIMATION_IMPL
AFX_OBJECT(_arxAnimation)
#else
AFX_OBJECT(arxAnimation)
#endif
{
    arxAnimationFlags   flags;
    afxUnit             motSlotCnt;
    akxLinkedMotion*    motSlots;

    // @timeStep é o intervalo após oversampling.
    // Quando @timeStep e @oversampling forem iguais a 0.00416667 e 4.0, respectivamente, uma animação foi produzida a 60 FPS (0.016667 segundos entre cada frame) com oversampling de 4x.
    // Multiplicando @timeStep por @oversampling, pode-se encontrar o intervalo original antes do oversampling.
    // Tanto @timeStep quanto @oversampling não são usadas pelo Qwadro.
    // Estão aqui de forma informativa preservando conhecimento sobre a intenção original do autor.
    afxReal             dur; // especifica quão demorado, em segundos, a animação leva para executar a completação.
    afxReal             timeStep; // especifica quão muitos segundos há entre cada frame.
    afxReal             oversampling; // 1.f

    afxString           urn;
};
#endif

AFX_DECLARE_STRUCT(arxControlledAnimation);
AFX_DECLARE_STRUCT(arxControlledPose);

AFX_DECLARE_STRUCT(arxMotiveCallbacks);

typedef struct controlled_pose
{
    arxPose Attitude; // 0
    const akxTrackMask *ModelMask; // 4
} controlled_pose;

typedef struct controlled_animation
{
    arxInstancedAnimation Binding; // 0
    afxAccumOp AccumulationMode; // 4
    const akxTrackMask *TrackMask; // 8
    const akxTrackMask *ModelMask; // 12
} controlled_animation;

AFX_DEFINE_STRUCT(_arxMotvDdi)
{
    void(*PoseAccumulateBindingState)(arxMotive, afxUnit, afxUnit, arxPose, afxReal, afxUnit const*);
    void(*AnimationAccumulateBindingState)(arxMotive, afxUnit, afxUnit, arxPose, afxReal, const afxUnit *);
    void(*AnimationAccumulateLoopTransform)(arxMotive, afxReal, afxReal*, afxV3d, afxV3d, afxBool);
    void(*AnimationBuildDirect)(arxMotive, afxUnit, afxM4d const, arxPlacement, afxReal);
    void(*PoseBuildDirect)(arxMotive, afxUnit, afxM4d const, arxPlacement);
};

AFX_OBJECT(arxMotive)
{
    arxCapstan            moto;
    afxLink          bod;
    arxMotiveCallbacks const *callbacks;
    union
    {
        controlled_animation ca;
        controlled_pose      cp;
    };
    afxBool isAnim;//void*               reservedPtr;
    _arxMotvDdi* pimpl;
};

AFX_DEFINE_STRUCT(arxMotiveCallbacks)
{
    struct controlled_animation*(*GetControlledAnimation)(arxMotive*);
    struct controlled_pose*(*GetControlledAttitude)(arxMotive*);
    afxBool(*InitializeBindingState)(arxMotive*, void*);
    void(*AccumulateBindingState)(arxMotive*, afxUnit, afxUnit, arxPose*, afxReal, afxUnit const*);
    void(*BuildBindingDirect)(arxMotive*, afxUnit, afxReal const*, arxPlacement*, afxReal);
    void(*AccumulateLoopTransform)(arxMotive*, afxReal, afxReal*, afxReal*, afxReal*, afxBool);
    void(*CleanupBindingState)(arxMotive*);
};



ARX afxClassConfig const _ARX_MOT_CLASS_CONFIG;
ARX afxClassConfig const _ARX_MOTO_CLASS_CONFIG;
ARX afxClassConfig const _ARX_ANI_CLASS_CONFIG;

#endif//ARX_IMPL___ANIMATION_H
