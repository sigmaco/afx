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

#ifndef ASX_IMPL___ANIMATION_H
#define ASX_IMPL___ANIMATION_H

//#include "../impl/afxExecImplKit.h"
#include "qwadro/inc/sim/afxSimulation.h"

typedef enum asxTrackSampler
{
    asxTrackSampler_UUU, // uniform position, orientation and scale/shear.
    asxTrackSampler_III, // identity position, orientation and scale/shear.
    asxTrackSampler_CII, // constant position, identity orientation and scale/shear.
    asxTrackSampler_CCI, // constant position and orientation, identity scale/shear.
    asxTrackSampler_CAI, // constant position, accumulative orientation, identity scale/shear.
    asxTrackSampler_AII, // accumulative position, identity orientation and scale/shear.
    asxTrackSampler_ACI, // accumulative position, constant orientation, identity scale/shear.
    asxTrackSampler_AAI, // accumulative position and orientation, identity scale/shear.
    asxTrackSampler_IIU, // identity position, identity orientation, uniform scale/shear
    asxTrackSampler_SSS // static position, orientation and scale/shear.
} asxTrackSampler;

#ifdef _ASX_MOTOR_C
typedef enum afxCapstanFlag
{
    afxCapstanFlag_ACTIVE = AFX_BIT(0),
    afxCapstanFlag_KILL_ONCE_COMPLETE = AFX_BIT(1),
    afxCapstanFlag_KILL_ONCE_UNUSED = AFX_BIT(2),
    afxCapstanFlag_EASE_IN = AFX_BIT(3),
    afxCapstanFlag_EASE_OUT = AFX_BIT(4),
    afxCapstanFlag_FORCE_CLAMPLED_LOOPS = AFX_BIT(5)
} afxCapstanFlags;

#ifdef _ASX_MOTOR_IMPL
AFX_OBJECT(_asxCapstan)
#else
AFX_OBJECT(afxCapstan)
#endif
{
    afxCapstanFlags   flags;
    afxReal         dtLocalClockPending;
    afxReal         localClock;
    afxReal         speed;
    afxReal         localDur;
    afxInt          currIterIdx;
    afxInt          iterCnt;
    afxReal         currWeight;
    afxCapstanTiming  timing;
    afxUnit32       easeInValues;
    afxUnit32       easeOutValues;
    void*           udd[4];
};
#endif//_ASX_MOTOR_C

#ifdef _ASX_MOTION_C

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
AFX_OBJECT(afxMotion)
#endif
{
    afxString           id;
    afxMask             flags;
    afxUnit             pivotCnt; // circuit count
    afxPivotalMotion*   pivotCurve; // uma arranjo de registros da moção de junta de afxSkeleton do correspondente afxModel.
    afxString*          pivots; // circuit id compatible with skeleton joints.
    //afxUnit              pivotLodErrorCnt;
    afxReal*            pivotLodError;
    afxUnit             vecCnt;
    afxVectorialMotion* vecCurve;
    afxString*          vectors;
    afxTransform        displacement;
    afxPivotalMotion*   root;
    afxV3d              loopTranslation;
    akxPeriodicLoop*    periodicLoop;

#if 0
    afxUnit             curveCnt;
    afxCurve*           curves;
    struct
    {
        afxUnit         baseCurIdx;
        afxUnit         curCnt;
    }*xforms;
    afxMask*            xformFlag;
    afxCurve*           deformation;
    afxCurve*           translation;
    afxCurve*           transmission;
    afxString*          articulation;
#endif
};
#endif//_ASX_MOTION_C


typedef enum asxCachedMotionTransformFlag
{
    asxCachedMotionTransformFlag_Identity = 0x0,

    asxCachedMotionTransformFlag_TranslationIsConstant = 0x1,
    asxCachedMotionTransformFlag_TranslationIsKeyframed = 0x2,
    asxCachedMotionTransformFlag_TranslationIsGeneral = 0x3,
    asxCachedMotionTransformFlag_TranslationMask = 0x3,

    asxCachedMotionTransformFlag_TransmissionIsConstant = 0x4,
    asxCachedMotionTransformFlag_TransmissionIsKeyframed = 0x8,
    asxCachedMotionTransformFlag_TransmissionIsGeneral = 0xC,
    asxCachedMotionTransformFlag_TransmissionMask = 0xC,

    asxCachedMotionTransformFlag_TransmutationIsConstant = 0x10,
    asxCachedMotionTransformFlag_TransmutationIsKeyframed = 0x20,
    asxCachedMotionTransformFlag_TransmutationIsGeneral = 0x30,
    asxCachedMotionTransformFlag_TransmutationMask = 0x30,
} asxCachedMotionTransformFlags;

typedef struct afxSampleContext afxSampleContext;

AFX_DEFINE_STRUCT(asxCachedMotionTransform)
{
    afxUnit          srcTrackIdx; // short afxPivotalMotion
    afxQuatBlend quatMode; // char 0x02
    asxCachedMotionTransformFlags flags; // char 0x03
    afxPivotalMotion const* srcTrack;
    void(*sampler)(const afxSampleContext *, const afxPivotalMotion *, asxCachedMotionTransform *, const afxTransform *, const float *, const float *, float *, float *);
    afxReal         lodErr;
    afxTransform    lodTransform;
};

AFX_DEFINE_HANDLE(asxInstancedAnimation);

AFX_DEFINE_STRUCT(asxInstancedAnimationId)
{
    afxAnimation Animation;
    int motIdx;
    const char *TrackPattern;
    const char *BonePattern;
    afxModel OnModel;
    afxModel FromBasis;
    afxModel ToBasis;
};

AFX_OBJECT(asxInstancedAnimation)
{
    asxInstancedAnimationId ID;

    void *RebasingMemory; // 28
    afxUnit TrackBindingCount; // 32
    asxCachedMotionTransform *TrackBindings; // 36 // one for each skeleton pivot
    asxInstancedAnimation Left;
    asxInstancedAnimation Right;
    int UsedBy;
    asxInstancedAnimation PreviousUnused;
    asxInstancedAnimation NextUnused;
};

AFX_DEFINE_STRUCT(asxTrackTarget) // draft
{
    asxInstancedAnimation Binding;
    asxInstancedAnimationId BindingID; // size = 28
    afxBody OnInstance; // 32
    afxAccumOp AccumulationMode; // 36
    float LODCopyScaler; // 40
    akxTrackMask *TrackMask; // 44
    akxTrackMask *ModelMask; // 48
};

#ifdef _ASX_ANIMATION_C

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
    asxCachedMotionTransform *tracks;
    */


};

#ifdef _ASX_ANIMATION_IMPL
AFX_OBJECT(_asxAnimation)
#else
AFX_OBJECT(afxAnimation)
#endif
{
    afxAnimationFlags   flags;
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

AFX_DECLARE_STRUCT(asxControlledAnimation);
AFX_DECLARE_STRUCT(asxControlledPose);

AFX_DECLARE_STRUCT(asxMotiveCallbacks);

typedef struct controlled_pose
{
    afxPose Attitude; // 0
    const akxTrackMask *ModelMask; // 4
} controlled_pose;

typedef struct controlled_animation
{
    asxInstancedAnimation Binding; // 0
    afxAccumOp AccumulationMode; // 4
    const akxTrackMask *TrackMask; // 8
    const akxTrackMask *ModelMask; // 12
} controlled_animation;

AFX_DEFINE_HANDLE(asxMotive);

AFX_OBJECT(asxMotive)
{
    afxCapstan            moto;
    afxLink          bod;
    asxMotiveCallbacks const *callbacks;
    union
    {
        controlled_animation ca;
        controlled_pose      cp;
    };
    afxBool isAnim;//void*               reservedPtr;
};

AFX_DEFINE_STRUCT(asxMotiveCallbacks)
{
    struct controlled_animation*(*GetControlledAnimation)(asxMotive*);
    struct controlled_pose*(*GetControlledAttitude)(asxMotive*);
    afxBool(*InitializeBindingState)(asxMotive*, void*);
    void(*AccumulateBindingState)(asxMotive*, afxUnit, afxUnit, afxPose*, afxReal, afxUnit const*);
    void(*BuildBindingDirect)(asxMotive*, afxUnit, afxReal const*, afxPlacement*, afxReal);
    void(*AccumulateLoopTransform)(asxMotive*, afxReal, afxReal*, afxReal*, afxReal*, afxBool);
    void(*CleanupBindingState)(asxMotive*);
};



ASX afxClassConfig const _ASX_MOT_CLASS_CONFIG;
ASX afxClassConfig const _ASX_MOTO_CLASS_CONFIG;
ASX afxClassConfig const _ASX_ANI_CLASS_CONFIG;

#endif//ASX_IMPL___ANIMATION_H
