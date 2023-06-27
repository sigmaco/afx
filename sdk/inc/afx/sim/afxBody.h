/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_BODY_H
#define AFX_BODY_H

#include "dag/afxSkeleton.h"
#include "modeling/afxModel.h"

// The afxBody object helps you manage the run-time state of a afxModel.
// The afxModel is just the structured data for a particular model - it does not have any concept of where it is, what state it's in, or what animations are playing on it.
// The afxBody keeps track of this and provides a number of handy utility functions to ease working with model states. 

// While all your identical entities should share the same afxModel, since their mesh and skeletal data is identical, each should still have its own afxBody.
// This is because the afxBody data is all dynamic, and is not shared across multiple models even if they are made from the same afxModel data.
// So while it is common to cache the afxModel hierarchy of structures and instantiate them multiple times, you never want to cache or reuse afxBody objects.
// They should always be instantiated and freed along with the entities they represent in your application.

// Note, however, that these are simply pointers into the original data you passed during instantiation. 
// The afxBody does not keep copies of the data (to minimize memory use), so you cannot free the original model data and expect to get valid pointers back here. 
// For that matter, if you free the original model data, none of the afxBody calls will work either, since they all rely on that data, as you would expect.

// afxBody is a instanced afxModel, following the Granny concept.

AFX_DEFINE_HANDLE(afxBody);

AFX_OBJECT(afxBody)
{
    afxObject           obj;

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