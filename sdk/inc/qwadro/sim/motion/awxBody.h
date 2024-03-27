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

#ifndef AFX_BODY_H
#define AFX_BODY_H

#include "../modeling/afxModel.h"
#include "qwadro/sim/motion/awxMotor.h"

/// O objeto awxBody aloja o estado de um afxModel no tempo de execução.
/// O afxModel é apenas dados estruturados para um modelo particular --- isto é, este não há qualquer conceito de onde este está, qual estado este está em, ou quais animações estão interagindo em si.
/// O awxBody mantém rastreio disto e provém um número de funções úteis para facilitar o trabalho com estados de modelo. 

/// Enquanto todas as suas entidades idênticas deveriam compartilhar o mesmo afxModel, desde que seus dados de malha e esqueleto sejam idênticos, cada um deveria ainda haver seu próprio awxBody.
/// Dito isto porque os dados do awxBody são todos dinâmicos, e não são compartilhados através de múltiplos modelos, mesmo se estes foram feitos do mesmo afxModel.
/// Assim, enquanto é comum colocar a hierarquia de estruturas do afxModel no cache e instanciá-las múltiplas vezes, você nunca deveria colocar objetos awxBody no cache e reusá-los.
/// Eles deveriam sempre ser instanciados e liberados ao longo da existência das entidades que eles representam na sua aplicação.

/// Note, no entanto, que estes são simplesmente ponteiros para os dados originais que vocÊ passou durante a instanciação.
/// O awxBody não mantém cópias dos dados (para minimizar uso de memória), então você não pode liberar os dados originais do modelo e esperar obter ponteiros válidos de volta aqui.
/// Para aquilo que importa, se você liberar os dados originais do modelo, nenhuma das chamadas para awxBody funcionarão, desde que elas todas dependem daqueles dados, como você esperaria.

AFX_OBJECT(awxBody)
{
    afxModel            mdl; // afxModel
    afxSkeleton         cachedSkl;
    void*               cachedBones;
    afxNat              cachedBoneCnt;
    afxChain            motives;
    void*               userData[4];
    afxNat32            reserved0,
                        reserved1;

    afxReal             mass; // totally of masses of parts. Additive to initial "fake" value. May be used to create physical behavoir on empty bodies.
    afxV4d              centre;
    afxV4d              velocity;
    afxV4d              force;
    afxV4d              torque;

    afxReal             linearDamping;
    afxReal             angularDamping;
    awxPose             pose;
    afxM4d              placement;
};

AKX afxModel            AwxGetBodyModel(awxBody bod);
AKX afxSkeleton         AwxGetBodySkeleton(awxBody bod);
    
AKX void                AwxResetBodyClock(awxBody bod);
AKX void                AwxUpdateBodyMatrix(awxBody bod, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m);

AKX void                AwxRecenterLinkedMotorClocks(awxBody bod, afxReal currClock);

/// Update all the motors affecting a particular puppet.
AKX void                AwxReclockMotives(awxBody bod, afxReal newClock);

AKX void                AwxPurgeTerminatedMotives(awxBody bod);

AKX void                AwxGetBodyRootMotionVectors(awxBody bod, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation);

AKX awxMotor            AwxRunManipulatedPose(awxBody bod, afxReal currClock, afxReal duration, awxPose const* pose, awxTrackMask *ModelMask);
AKX awxMotor            AwxRunAnimationBinding(awxBody bod, afxReal startTime, awxAnimation ani, struct animation_binding *binding);
AKX awxMotor            AwxRunAnimation(awxBody bod, afxReal startTime, awxAnimation ani);

AKX void                AwxAccumulateBodyAnimationsLODSparse(awxBody bod, afxNat basePivotIdx, afxNat pivotCnt, awxPose* rslt, afxReal allowedErr, afxNat const* sparseBoneArray);
AKX void                AwxSampleBodyAnimationsLODSparse(awxBody bod, afxNat basePivotIdx, afxNat pivotCnt, awxPose* rslt, afxReal allowedErr, afxNat const* sparseBoneArray);
AKX void                AwxSampleBodyAnimationsAcceleratedLOD(awxBody bod, afxNat pivotCnt, afxM4d const offset, awxPose* scratch, awxPoseBuffer* rslt, afxReal allowedErr);
AKX afxBool             AwxSampleSingleBodyAnimationLODSparse(awxBody bod, awxMotor moto, afxNat basePivotIdx, afxNat pivotCnt, awxPose* rslt, afxReal allowedErr, afxNat const* sparseBoneArray);

////////////////////////////////////////////////////////////////////////////////

AKX afxError            AwxEmbodyModel(afxModel mdl, afxNat cnt, awxBody bod[]);

#endif//AFX_BODY_H
