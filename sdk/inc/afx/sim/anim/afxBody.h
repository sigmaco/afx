/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_BODY_H
#define AFX_BODY_H

#include "../modeling/afxModel.h"

/// O objeto afxBody aloja o estado de um afxModel no tempo de execução.
/// O afxModel é apenas dados estruturados para um modelo particular --- isto é, este não há qualquer conceito de onde este está, qual estado este está em, ou quais animações estão interagindo em si.
/// O afxBody mantém rastreio disto e provém um número de funções úteis para facilitar o trabalho com estados de modelo. 

/// Enquanto todas as suas entidades idênticas deveriam compartilhar o mesmo afxModel, desde que seus dados de malha e esqueleto sejam idênticos, cada um deveria ainda haver seu próprio afxBody.
/// Dito isto porque os dados do afxBody são todos dinâmicos, e não são compartilhados através de múltiplos modelos, mesmo se estes foram feitos do mesmo afxModel.
/// Assim, enquanto é comum colocar a hierarquia de estruturas do afxModel no cache e instanciá-las múltiplas vezes, você nunca deveria colocar objetos afxBody no cache e reusá-los.
/// Eles deveriam sempre ser instanciados e liberados ao longo da existência das entidades que eles representam na sua aplicação.

/// Note, no entanto, que estes são simplesmente ponteiros para os dados originais que vocÊ passou durante a instanciação.
/// O afxBody não mantém cópias dos dados (para minimizar uso de memória), então você não pode liberar os dados originais do modelo e esperar obter ponteiros válidos de volta aqui.
/// Para aquilo que importa, se você liberar os dados originais do modelo, nenhuma das chamadas para afxBody funcionarão, desde que elas todas dependem daqueles dados, como você esperaria.

AFX_DEFINE_HANDLE(afxBody);

AFX_OBJECT(afxBody)
{
    afxModel            mdl; // afxModel
    afxSkeleton         cachedSkl;
    void*               cachedBones;
    afxNat              cachedBoneCnt;
    afxChain            controls;
    void*               userData[4];
    afxNat32            reserved0,
                        reserved1;

    afxReal             mass; // totally of masses of parts. Additive to initial "fake" value. May be used to create physical behavoir on empty bodies.
    afxV4d              centre;
    afxV4d              velocity;
    afxV4d              force;
    afxV4d              torque;

    afxReal             linearDamping,
                        angularDamping;
};

AFX afxError            AfxEmbodyModel(afxModel mdl, afxNat cnt, afxBody bod[]);

AFX afxModel            AfxGetBodyModel(afxBody bod);
AFX afxSkeleton         AfxGetBodySkeleton(afxBody bod);
    
AFX void                AfxResetBodyClock(afxBody bod);
AFX void                AfxUpdateBodyModelMatrix(afxBody bod, afxReal secsElapsed, afxM4d const mm, afxM4d dest, afxBool inverse);

AFX void                AfxRecenterBodyAllClocks(afxBody bod, afxReal currClock);

AFX void                AfxSetBodyClock(afxBody bod, afxReal newClock);
AFX void                AfxFreeCompletedBodyControls(afxBody bod);
AFX void **             AfxGetBodyUserDataArray(afxBody bod);

AFX void                AfxGetBodyRootMotionVectors(afxBody bod, afxReal secsElapsed, afxReal translation[3], afxReal rotation[3], afxBool inverse);

AFX void                AfxApplyRootMotionVectorsToMatrix(afxReal const mm[4][4], afxReal const translation[3], afxReal const rotation[3], afxReal m[4][4]);

#if 0

GRANNY_DYNLINK(void) GrannyAccumulateModelAnimationsLODSparse(granny_model_instance const * ModelInstance,
    granny_int32x FirstBone,
    granny_int32x BoneCount,
    granny_local_pose * Result,
    granny_real32 AllowedError,
    granny_int32x const * SparseBoneArray);
GRANNY_DYNLINK(void) GrannySampleModelAnimationsLODSparse(granny_model_instance const * ModelInstance,
    granny_int32x FirstBone,
    granny_int32x BoneCount,
    granny_local_pose * Result,
    granny_real32 AllowedError,
    granny_int32x const * SparseBoneArray);
GRANNY_DYNLINK(bool) GrannySampleSingleModelAnimationLODSparse(granny_model_instance const * ModelInstance,
    granny_control const * Control,
    granny_int32x FirstBone,
    granny_int32x BoneCount,
    granny_local_pose * Result,
    granny_real32 AllowedError,
    granny_int32x const * SparseBoneArray);
GRANNY_DYNLINK(void) GrannySampleModelAnimationsAcceleratedLOD(granny_model_instance const * ModelInstance,
    granny_int32x BoneCount,
    granny_real32 const * Offset4x4,
    granny_local_pose * Scratch,
    granny_world_pose * Result,
    granny_real32 AllowedError);
GRANNY_DYNLINK(void) GrannyAccumulateModelAnimations(granny_model_instance const * ModelInstance,
    granny_int32x FirstBone,
    granny_int32x BoneCount,
    granny_local_pose * Result);
GRANNY_DYNLINK(void) GrannySampleModelAnimations(granny_model_instance const * ModelInstance,
    granny_int32x FirstBone,
    granny_int32x BoneCount,
    granny_local_pose * Result);
GRANNY_DYNLINK(bool) GrannySampleSingleModelAnimation(granny_model_instance const * ModelInstance,
    granny_control const * Control,
    granny_int32x FirstBone,
    granny_int32x BoneCount,
    granny_local_pose * Result);
GRANNY_DYNLINK(void) GrannySampleModelAnimationsAccelerated(granny_model_instance const * ModelInstance,
    granny_int32x BoneCount,
    granny_real32 const * Offset4x4,
    granny_local_pose * Scratch,
    granny_world_pose * Result);
GRANNY_DYNLINK(void) GrannyAccumulateModelAnimationsLOD(granny_model_instance const * ModelInstance,
    granny_int32x FirstBone,
    granny_int32x BoneCount,
    granny_local_pose * Result,
    granny_real32 AllowedError);
GRANNY_DYNLINK(void) GrannySampleModelAnimationsLOD(granny_model_instance const * ModelInstance,
    granny_int32x FirstBone,
    granny_int32x BoneCount,
    granny_local_pose * Result,
    granny_real32 AllowedError);
GRANNY_DYNLINK(bool) GrannySampleSingleModelAnimationLOD(granny_model_instance const * ModelInstance,
    granny_control const * Control,
    granny_int32x FirstBone,
    granny_int32x BoneCount,
    granny_local_pose * Result,
    granny_real32 AllowedError);

GRANNY_DYNLINK(void) GrannyClipRootMotionVectors(granny_real32 const * Translation3,
    granny_real32 const * Rotation3,
    granny_uint32 AllowableDOFs,
    granny_real32 * AllowedTranslation3,
    granny_real32 * AllowedRotation3,
    granny_real32 * DisallowedTranslation3,
    granny_real32 * DisallowedRotation3);

GRANNY_DYNLINK(void) GrannyApplyRootMotionVectorsToLocalPose(granny_local_pose * Pose,
    granny_real32 const * Translation3,
    granny_real32 const * Rotation3);

#endif

#endif//AFX_BODY_H