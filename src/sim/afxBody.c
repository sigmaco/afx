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

#define _AFX_SIM_C
#define _AMX_BODY_C
#define _AMX_ANIMATION_C
#define _AMX_MOTION_C
#define _AMX_SKELETON_C
#define _AMX_SIMULATION_C
#include "../dev/AmxImplKit.h"

 // PUPPET INTERLINK

_AMXINL void _AkxDestroyMotive(akxMotive *intk)
{
    afxMotor moto = intk->moto;
    AfxPopLinkage(&intk->bod);

    if (intk->callbacks && intk->callbacks->CleanupBindingState)
        intk->callbacks->CleanupBindingState(intk);

    AfxReleaseUnusedMotors(1, &moto); // TODO move autokill to motive?
    AfxReleaseObjects(1, (void*[]) { moto });

    AfxDeallocate(intk);
}

_AMXINL akxMotive* AfxMotivateBody(afxBody bod, afxMotor moto, akxMotiveCallbacks const *callbacks)
{
    afxError err = AFX_ERR_NONE;
    afxSimulation sim = AfxGetProvider(bod);
    akxMotive* intk = NIL;

    intk = AfxAllocate(1, sizeof(*intk), 0, AfxHere());

    if (!intk) AfxThrowError();
    else
    {
        AfxReacquireObjects(1, &moto);
        intk->moto = moto;

        intk->callbacks = callbacks;
        intk->reservedPtr = 0;

        if (AfxMotorIsActive(moto))
            AfxPushLinkage(&intk->bod, &bod->motives);
        else
            AfxPushBackLinkage(&intk->bod, &bod->motives);
    }
    return intk;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxBool AfxGetBodyModel(afxBody bod, afxModel* model)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    afxModel mdl = bod->mdl;
    AfxTryAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT(model);
    *model = mdl;
    return !!mdl;
}

_AMX afxBool AfxGetBodySkeleton(afxBody bod, afxSkeleton* skeleton)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    afxSkeleton skl = bod->skl;
    AfxTryAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT(skeleton);
    *skeleton = skl;
    return !!skl;
}

_AMX void AfxUpdateBodyMotives(afxBody bod, afxReal newClock)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    
    // Update all the body's motives to be set to the current game clock.
    // like SetModelClock
    // it will automatically set all the motive's to the given clock, saving you the trouble of doing so.

    akxMotive* intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        afxMotor moto = intk->moto;
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        AfxUpdateMotorClock(newClock, 1, &moto);
    }
}

_AMX void AfxPurgeTerminatedMotives(afxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    akxMotive* intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        afxMotor moto = intk->moto;
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        
        if (AfxMotorHasTerminated(moto))
        {
            _AkxDestroyMotive(intk);
        }
    }
}

extern void AnimationAccumulateLoopTransform(akxMotive *intk, afxReal SecondsElapsed, afxReal *TotalWeight, afxV3d ResultTranslation, afxV3d ResultRotation, afxBool Inverse);

_AMX void AfxGetBodyRootMotionVectors(afxBody bod, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    afxReal totalWeight = 0.0;    
    afxV4d t = { 0, 0, 0, 1 }, r = { 0, 0, 0, 0 };

    akxMotive* intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        afxMotor moto = intk->moto;
        AfxAssertObjects(1, &moto, afxFcc_MOTO);

        if (AfxMotorHasEffect(moto))
        {
            //intk->callbacks->AccumulateLoopTransform(intk, secsElapsed, &totalWeight, t, r, inverse);
            AnimationAccumulateLoopTransform(intk, secsElapsed, &totalWeight, t, r, inverse);
        }
    }

    if (totalWeight > 0.001)
    {
        afxReal s = 1.0 / totalWeight;
        AfxV3dScale(translation, t, s);
        AfxV3dScale(rotation, r, s);
    }
}

_AMX void AfxUpdateBodyMatrix(afxBody bod, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    
    afxV4d t, r;
    AfxGetBodyRootMotionVectors(bod, secsElapsed, inverse, t, r);
    AfxApplyRootMotionVectorsToMatrix(t, r, mm, m);
}

_AMX void AfxRecenterBodyMotiveClocks(afxBody bod, afxReal currClock)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    akxMotive *intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        afxMotor moto = intk->moto;
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        AfxRebaseMotorClocks(currClock, 1, &moto);
    }
}

extern void AnimationBuildDirect(akxMotive* Binding, afxUnit BoneCount, const afxM4d Offset4x4, afxPoseBuffer Result, afxReal AllowedError);
extern void AnimationAccumulateBindingState(akxMotive* Binding, afxUnit FirstBone, afxUnit BoneCount, afxPose Result, afxReal AllowedError, const afxUnit *SparseBoneArray);

extern void BeginLocalPoseAccumulation(afxPose LocalPose, afxUnit FirstBone, afxUnit BoneCount, const afxUnit *SparseBoneArray);
extern void EndLocalPoseAccumulationLOD(afxPose LocalPose, afxUnit FirstBone, afxUnit BoneCount, afxSkeleton ReferenceSkeleton, afxReal AllowedError, const afxUnit *SparseBoneArray);

_AMX void AfxAccumulateBodyAnimationsLODSparse(afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose rslt, afxReal allowedErr, afxUnit const* sparseBoneArray)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    
    // if allowedErr == 0.f, won't use LOD.
    // if sparseBoneArray == NIL, won't use sparse.

    akxMotive *intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        afxMotor moto = intk->moto;
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        
        if (AfxMotorHasEffect(moto))
        {
            AnimationAccumulateBindingState(intk, basePivotIdx, pivotCnt, rslt, allowedErr, sparseBoneArray);
        }
    }
}

_AMX void AfxSampleBodyAnimationsLODSparse(afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose rslt, afxReal allowedErr, afxUnit const* sparseBoneArray)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    // if allowedErr == 0.f, won't use LOD.
    // if sparseBoneArray == NIL, won't use sparse.

    BeginLocalPoseAccumulation(rslt, basePivotIdx, pivotCnt, sparseBoneArray);
    AfxAccumulateBodyAnimationsLODSparse(bod, basePivotIdx, pivotCnt, rslt, allowedErr, sparseBoneArray);
    EndLocalPoseAccumulationLOD(rslt, basePivotIdx, pivotCnt, bod->skl, allowedErr, sparseBoneArray);
}

_AMX afxBool AfxSampleSingleBodyAnimationLODSparse(afxBody bod, afxMotor moto, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose rslt, afxReal allowedErr, afxUnit const* sparseBoneArray)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    
    afxBool fnd = 0;
    BeginLocalPoseAccumulation(rslt, basePivotIdx, pivotCnt, sparseBoneArray);
    
    akxMotive *intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        afxMotor moto2 = intk->moto;
        AfxAssertObjects(1, &moto2, afxFcc_MOTO);

        if (moto2 == moto)
        {
            if (AfxMotorHasEffect(moto))
            {
                //intk->callbacks->AccumulateBindingState(intk, basePivotIdx, pivotCnt, rslt, allowedErr, sparseBoneArray);
                AnimationAccumulateBindingState(intk, basePivotIdx, pivotCnt, rslt, allowedErr, sparseBoneArray);
            }
            ++fnd;
        }
    }

    EndLocalPoseAccumulationLOD(rslt, basePivotIdx, pivotCnt, bod->skl, allowedErr, sparseBoneArray);    
    return fnd;
}

_AMX void AfxSampleBodyAnimationsAcceleratedLOD(afxBody bod, afxUnit pivotCnt, afxM4d const offset, afxPose scratch, afxPoseBuffer rslt, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    AFX_ASSERT(scratch);

    afxSkeleton skl = bod->skl;
    afxUnit boneCnt = AfxCountSkeletonJoints(skl, 0);

    if (pivotCnt > boneCnt)
        pivotCnt = boneCnt;

    if (!offset)
    {
        offset = AFX_M4D_IDENTITY;
    }

    if (bod->motives.cnt)
    {
        if (bod->motives.cnt > 1)
        {
            afxUnit lodBoneCnt = AfxCountSkeletonJoints(skl, allowedErr);
            AfxSampleBodyAnimationsLODSparse(bod, 0, lodBoneCnt, scratch, allowedErr, 0);
            AfxComputePoseBufferLod(skl, 0, pivotCnt, 0, lodBoneCnt, scratch, offset, rslt);            
        }
        else
        {
            afxLinkage* lnk = AfxGetLastLinkage(&bod->motives);
            akxMotive* intk = AFX_REBASE(lnk, akxMotive, bod);
            AnimationBuildDirect(intk, pivotCnt, offset, rslt, allowedErr);
        }
    }
    else
    {
        AfxComputeRestPoseBuffer(skl, 0, pivotCnt, offset, rslt);
    }
}

_AMX afxBool AfxSampleBodyAnimations(afxBody bod, afxAnimSampleContext const* ctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    afxBool rslt = TRUE;
    
    if (ctx->moto)
    {
        rslt = AfxSampleSingleBodyAnimationLODSparse(bod, ctx->moto, ctx->firstPivot, ctx->pivotCnt, ctx->pose, ctx->allowedErr, ctx->sparseBoneArray);
    }
    else
    {
        if (ctx->accelerated)
        {
            AfxSampleBodyAnimationsAcceleratedLOD(bod, ctx->pivotCnt, ctx->displacement, ctx->pose, ctx->posb, ctx->allowedErr);
        }
        else
        {
            AfxSampleBodyAnimationsLODSparse(bod, ctx->firstPivot, ctx->pivotCnt, ctx->pose, ctx->allowedErr, ctx->sparseBoneArray);
        }
    }
}

_AMX afxError _AmxBodDtorCb(afxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    akxMotive *intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        _AkxDestroyMotive(intk);
    }

    if (bod->mdl)
        AfxReleaseObjects(1, &bod->mdl);

    if (bod->skl)
        AfxReleaseObjects(1, &bod->skl);

    return err;
}

_AMX afxError _AmxBodCtorCb(afxBody bod, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    afxSimulation sim = args[0];
    afxModel mdl = args[1];

    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxSkeleton skl;
    AfxGetModelSkeleton(mdl, &skl);
    
    if (!skl) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &skl, afxFcc_SKL);

        AfxReacquireObjects(1, &mdl);
        bod->mdl = mdl;
        AfxReacquireObjects(1, &skl);
        bod->skl = skl;
        AfxDeployChain(&bod->motives, bod);
        bod->reserved0 = 0;
        bod->reserved1 = 0;
    }

    AfxV3dZero(bod->mass);
    AfxV3dZero(bod->veloc);
    /*
    bod->mass = AfxScalar(0);
    AfxV4dZero(bod->centre);
    AfxV4dZero(bod->velocity);
    AfxV4dZero(bod->force);
    AfxV4dZero(bod->torque);
    bod->linearDamping = 0;
    bod->angularDamping = 0;
    */

    AfxM4dReset(bod->placement);

    AfxAcquirePoses(sim, 1, (afxUnit[]) { AfxCountSkeletonJoints(skl, 0) }, &bod->pose);

    return err;
}

_AMX afxClassConfig _AmxBodMgrCfg =
{
    .fcc = afxFcc_BOD,
    .name = "Body",
    .desc = "Modelled Body Instance",
    .fixedSiz = sizeof(AFX_OBJECT(afxBody)),
    .ctor = (void*)_AmxBodCtorCb,
    .dtor = (void*)_AmxBodDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxUnit AfxEnumerateBodies(afxSimulation sim, afxUnit first, afxUnit cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass const* cls = AfxGetBodyClass(sim);
    AfxAssertClass(cls, afxFcc_BOD);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)bodies);
}

_AMX afxUnit AfxInvokeBodies(afxSimulation sim, afxUnit first, afxUnit cnt, afxBool(*f)(afxBody, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass const* cls = AfxGetBodyClass(sim);
    AfxAssertClass(cls, afxFcc_BOD);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AMX afxError AfxAcquireBodies(afxSimulation sim, afxModel mdl, afxUnit cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxClass* cls = (afxClass*)AfxGetBodyClass(sim);
    AfxAssertClass(cls, afxFcc_BOD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)bodies, (void const*[]) { sim, mdl }))
        AfxThrowError();

    return err;
}




AMXINL akxMotive* AfxMotivateBody(afxBody bod, afxMotor moto, akxMotiveCallbacks const *callbacks);

_AMX afxUnit AfxPerformManipulatedPose(afxPose pose, afxReal startTime, afxReal duration, afxUnit iterCnt, akxTrackMask* modelMask, afxUnit cnt, afxBody bodies[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &pose, afxFcc_POSE);
    AFX_ASSERT(bodies);
    afxUnit rslt = 0;

    afxSimulation sim = AfxGetProvider(pose);
    afxMotor moto;

    if (AfxAcquireMotors(sim, startTime, duration, iterCnt, 1, &moto)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &moto, afxFcc_MOTO);

        for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
        {
            afxBody bod = bodies[mdlIdx];
            AfxAssertObjects(1, &bod, afxFcc_BOD);

            akxMotive* intk = AfxMotivateBody(bod, moto, NIL);

            if (!(intk)) AfxThrowError();
            else
            {
                intk->cp.Pose = pose;
                intk->cp.ModelMask = modelMask;
                intk->reservedPtr = NIL;

                ++rslt;
            }
        }
        AfxReleaseObjects(1, &moto);
    }
    return rslt;
}
