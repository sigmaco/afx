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
    afxInstance           obj;

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

AFX afxModel            AfxBodyGetModel(afxBody bod);
AFX afxSkeleton         AfxBodyGetSkeleton(afxBody bod);
    
AFX void                AfxBodyResetClock(afxBody bod);
AFX void                AfxBodyUpdateModelMatrix(afxBody bod, afxReal secondsElapsed, afxM4d const mm, afxM4d dest, afxBool inverse);

AFX void                AfxBodyRecenterAllClocks(afxBody bod, afxReal dCurrentClock);

AFX void                AfxBodySetClock(afxBody bod, afxReal newClock);
AFX void                AfxBodyFreeCompletedControls(afxBody bod);
AFX void **             AfxBodyGetUserDataArray(afxBody bod);

#if 0
GRANNY_DYNLINK(granny_model_instance *) GrannyInstantiateModel(granny_model const * Model);
GRANNY_DYNLINK(void) GrannyFreeModelInstance(granny_model_instance * ModelInstance);
GRANNY_DYNLINK(granny_model *) GrannyGetSourceModel(granny_model_instance const * Model);
GRANNY_DYNLINK(granny_skeleton *) GrannyGetSourceSkeleton(granny_model_instance const * Model);
GRANNY_DYNLINK(void) GrannySetModelClock(granny_model_instance const * ModelInstance,
    granny_real32 NewClock);
GRANNY_DYNLINK(void) GrannyFreeCompletedModelControls(granny_model_instance const * ModelInstance);
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
GRANNY_DYNLINK(void) GrannyGetRootMotionVectors(granny_model_instance const * ModelInstance,
    granny_real32 SecondsElapsed,
    granny_real32 * ResultTranslation3,
    granny_real32 * ResultRotation3,
    bool Inverse);
GRANNY_DYNLINK(void) GrannyClipRootMotionVectors(granny_real32 const * Translation3,
    granny_real32 const * Rotation3,
    granny_uint32 AllowableDOFs,
    granny_real32 * AllowedTranslation3,
    granny_real32 * AllowedRotation3,
    granny_real32 * DisallowedTranslation3,
    granny_real32 * DisallowedRotation3);
GRANNY_DYNLINK(void) GrannyApplyRootMotionVectorsToMatrix(granny_real32 const * ModelMatrix4x4,
    granny_real32 const * Translation3,
    granny_real32 const * Rotation3,
    granny_real32 * DestMatrix4x4);
GRANNY_DYNLINK(void) GrannyApplyRootMotionVectorsToLocalPose(granny_local_pose * Pose,
    granny_real32 const * Translation3,
    granny_real32 const * Rotation3);
GRANNY_DYNLINK(void) GrannyUpdateModelMatrix(granny_model_instance const * ModelInstance,
    granny_real32 SecondsElapsed,
    granny_real32 const * ModelMatrix4x4,
    granny_real32 * DestMatrix4x4,
    bool Inverse);
GRANNY_DYNLINK(void **) GrannyGetModelUserDataArray(granny_model_instance const * ModelInstance);
#endif

#endif//AFX_BODY_H