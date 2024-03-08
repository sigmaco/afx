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
 *                      (c 2017 SIGMA Co. & SIGMA Technology Group
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_ANIMATION_H
#define AFX_ANIMATION_H

#include "qwadro/sim/awxSimDefs.h"
#include "afxCurve2.h"
#include "qwadro/math/afxTransform.h"
#include "qwadro/math/afxCurve.h"
#include "qwadro/core/afxFixedString.h"
#include "qwadro/sim/motion/awxMotion.h"

/// Qwadro armazena animações em partes baseadas em quão muitos modelos estão envolvidos numa animação.
/// Assim sendo, uma awxAnimation é a moção de um conjunto de afxModel's animando sobre tempo.
/// A awxAnimation contém um ou mais awxMotion's, cada do qual correspondendo à moção de um modelo específico (desde que uma animação pode envolver múltiplos modelos, se o autor assim escolher).
/// O awxMotion é constituído de curvas, cada da qual especifica a translação, rotação e escala de uma junta no afxSkeleton do correspondente afxModel.

/*
    Body
    Animation
        Linked Motion
            Motion
            Skeleton
*/

typedef struct sample_context
{
    float LocalClock;
    float LocalDuration;
    afxBool UnderflowLoop;
    afxBool OverflowLoop;
    int FrameIndex;
} sample_context;

AFX_DEFINE_STRUCT(awxBoundMotionTransform)
{
    afxNat          srcTrackIdx; // awxMotionTransform
    char            quatMode;
    afxMask         flags;
    awxMotionTransform const* srcTrack;
    void(*sampler)(const sample_context *, const awxMotionTransform *, awxBoundMotionTransform *, const afxTransform *, const float *, const float *, float *, float *);
    afxReal         lodErr;
    afxTransform    lodTransform;
};

struct animation_binding_identifier
{
    awxAnimation Animation;
    int SourceTrackGroupIndex;
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
    awxBoundMotionTransform *TrackBindings; // 36 // one for each skeleton pivot
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
    awxBody OnInstance; // 32
    enum accumulation_mode AccumulationMode; // 36
    float LODCopyScaler; // 40
    awxTrackMask *TrackMask; // 44
    awxTrackMask *ModelMask; // 48
};

#ifdef _AFX_ANIMATION_C

AFX_DEFINE_STRUCT(awxLinkedMotion)
{
    awxMotion           mot;
    /*
    // --- identifier
    awxAnimation Animation;
    int SourceTrackGroupIndex;
    const char *TrackPattern;
    const char *BonePattern;
    const afxModel *OnModel;
    afxModel *FromBasis;
    afxModel *ToBasis;

    // ... binding
    void *RebasingMemory;
    int trackCnt;
    awxBoundMotionTransform *tracks;
    */


};

AFX_OBJECT(awxAnimation)
{
    afxNat              motSlotCnt;
    awxLinkedMotion*    motSlots;

    /// @timeStep é o intervalo após oversampling.
    /// Quando @timeStep e @oversampling forem iguais a 0.00416667 e 4.0, respectivamente, uma animação foi produzida a 60 FPS (0.016667 segundos entre cada frame) com oversampling de 4x.
    /// Multiplicando @timeStep por @oversampling, pode-se encontrar o intervalo original antes do oversampling.
    /// Tanto @timeStep quanto @oversampling não são usadas pelo Qwadro.
    /// Estão aqui de forma informativa preservando conhecimento sobre a intenção original do autor.
    afxReal             dur; /// especifica quão demorado, em segundos, a animação leva para executar a completação.
    afxReal             timeStep; /// especifica quão muitos segundos há entre cada frame.
    afxReal             oversampling; /// 1.f

    afxString           id;
    afxStringCatalog    strc;
};
#endif

AFX_DEFINE_STRUCT(awxAnimationBlueprint)
{
    afxReal             dur;
    afxReal             timeStep;
    afxReal             oversampling;
    afxNat              motSlotCnt;
    awxMotion*          motions;
    afxStringCatalog    strc;
    afxStringCatalog    strc2;
    afxString32         id;
};

AKX afxBool     AfxGetAnimationId(awxAnimation ani, afxString* id);

AKX afxBool     AfxFindMotion(awxAnimation ani, afxString const* id, afxNat *motIdx);

AKX afxError    AwxRelinkMotions(awxAnimation ani, afxNat baseSlot, afxNat slotCnt, awxMotion motions[]);

AKX afxError    AwxAssembleAnimations(afxSimulation sim, afxNat cnt, awxAnimationBlueprint const blueprints[], awxAnimation animations[]);

AKX void        AwxTransformAnimations(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal linearTol, afxReal const atv[4], afxReal affineTol, afxFlags flags, afxNat cnt, awxAnimation ani[]);

#endif//AFX_ANIMATION_H
