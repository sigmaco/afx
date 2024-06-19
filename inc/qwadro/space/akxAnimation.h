#pragma once
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

#ifndef AFX_ANIMATION_H
#define AFX_ANIMATION_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/space/afxCurve.h"
#include "qwadro/math/afxTransform.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/space/afxMotion.h"
#include "qwadro/space/akxPose.h"

/// Qwadro armazena animações em partes baseadas em quão muitos modelos estão envolvidos numa animação.
/// Assim sendo, uma akxAnimation é a moção de um conjunto de afxModel's animando sobre tempo.
/// A akxAnimation contém um ou mais afxMotion's, cada do qual correspondendo à moção de um modelo específico (desde que uma animação pode envolver múltiplos modelos, se o autor assim escolher).
/// O afxMotion é constituído de curvas, cada da qual especifica a translação, rotação e escala de uma junta no afxSkeleton do correspondente afxModel.

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

AFX_DEFINE_STRUCT(akxBoundMotionTransform)
{
    afxNat          srcTrackIdx; // short akxMotionTransform
    quaternion_mode quatMode; // char 0x02
    akxBoundMotionTransformFlags flags; // char 0x03
    akxMotionTransform const* srcTrack;
    void(*sampler)(const sample_context *, const akxMotionTransform *, akxBoundMotionTransform *, const afxTransform *, const float *, const float *, float *, float *);
    afxReal         lodErr;
    afxTransform    lodTransform;
};

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
    afxBody OnInstance; // 32
    accumulation_mode AccumulationMode; // 36
    float LODCopyScaler; // 40
    akxTrackMask *TrackMask; // 44
    akxTrackMask *ModelMask; // 48
};

#ifdef _AFX_ANIMATION_C

AFX_DEFINE_STRUCT(akxLinkedMotion)
{
    afxMotion           mot;
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

    afxString           id;
    afxStringBase       strb;
};
#endif

AFX_DEFINE_STRUCT(akxAnimationBlueprint)
{
    afxReal             dur;
    afxReal             timeStep;
    afxReal             oversampling;
    afxNat              motSlotCnt;
    afxMotion*          motions;
    afxStringBase       strb;
    afxStringBase       strb2;
    afxString32         id;
};

AKX afxBool     AfxGetAnimationId(akxAnimation ani, afxString* id);

AKX afxBool     AfxFindMotion(akxAnimation ani, afxString const* id, afxNat *motIdx);

AKX afxError    AkxRelinkMotions(akxAnimation ani, afxNat baseSlot, afxNat slotCnt, afxMotion motions[]);

////////////////////////////////////////////////////////////////////////////////

AKX afxError    AkxAssembleAnimations(afxSimulation sim, afxNat cnt, akxAnimationBlueprint const blueprints[], akxAnimation animations[]);

AKX void        AkxTransformAnimations(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxNat cnt, akxAnimation ani[]);

#endif//AFX_ANIMATION_H
