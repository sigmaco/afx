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
#define _AKX_BODY_C
#define _AKX_ANIMATION_C
#define _AKX_MOTION_C
#define _AKX_SKELETON_C
#define _AKX_SIMULATION_C
#include "../sim/dev/AkxSimDevKit.h"

 // PUPPET INTERLINK

_AKXINL void _AkxDestroyMotive(akxMotive *intk)
{
    akxMotor moto = intk->moto;
    AfxPopLinkage(&intk->bod);

    if (intk->callbacks && intk->callbacks->CleanupBindingState)
        intk->callbacks->CleanupBindingState(intk);

    AkxReleaseUnusedMotors(1, &moto); // TODO move autokill to motive?
    AfxReleaseObjects(1, (void*[]) { moto });

    AfxDeallocate(intk);
}

_AKXINL akxMotive* AfxMotivateBody(akxBody bod, akxMotor moto, akxMotiveCallbacks const *callbacks)
{
    afxError err = AFX_ERR_NONE;
    afxSimulation sim = AfxGetObjectProvider(bod);
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

_AKX afxBool AfxGetBodyModel(akxBody bod, afxModel* model)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    afxModel mdl = bod->mdl;
    AfxTryAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssert(model);
    *model = mdl;
    return !!mdl;
}

_AKX afxBool AfxGetBodySkeleton(akxBody bod, afxSkeleton* skeleton)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    afxSkeleton skl = bod->skl;
    AfxTryAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssert(skeleton);
    *skeleton = skl;
    return !!skl;
}

_AKX void AfxReclockMotives(akxBody bod, afxReal newClock)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    akxMotive* intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        akxMotor moto = intk->moto;
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        AkxSetMotorClock(moto, newClock);
    }
}

_AKX void AfxPurgeTerminatedMotives(akxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    akxMotive* intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        akxMotor moto = intk->moto;
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        
        if (AfxMotorHasTerminated(moto))
        {
            _AkxDestroyMotive(intk);
        }
    }
}

extern void AnimationAccumulateLoopTransform(akxMotive *intk, afxReal SecondsElapsed, afxReal *TotalWeight, afxV3d ResultTranslation, afxV3d ResultRotation, afxBool Inverse);

_AKX void AfxGetBodyRootMotionVectors(akxBody bod, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    afxReal totalWeight = 0.0;    
    afxV4d t = { 0, 0, 0, 1 }, r = { 0, 0, 0, 0 };

    akxMotive* intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        akxMotor moto = intk->moto;
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

_AKX void AfxUpdateBodyMatrix(akxBody bod, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    
    afxV4d t, r;
    AfxGetBodyRootMotionVectors(bod, secsElapsed, inverse, t, r);
    AfxApplyRootMotionVectorsToMatrix(t, r, mm, m);
}

_AKX void AfxRecenterLinkedMotorClocks(akxBody bod, afxReal currClock)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    akxMotive *intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        akxMotor moto = intk->moto;
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        AkxRecenterMotorClocks(moto, currClock);
    }
}

extern void AnimationBuildDirect(akxMotive* Binding, afxNat BoneCount, const afxM4d Offset4x4, akxPoseBuffer Result, afxReal AllowedError);
extern void AnimationAccumulateBindingState(akxMotive* Binding, afxNat FirstBone, afxNat BoneCount, akxPose Result, afxReal AllowedError, const afxNat *SparseBoneArray);

extern void BeginLocalPoseAccumulation(akxPose LocalPose, afxNat FirstBone, afxNat BoneCount, const afxNat *SparseBoneArray);
extern void EndLocalPoseAccumulationLOD(akxPose LocalPose, afxNat FirstBone, afxNat BoneCount, afxSkeleton ReferenceSkeleton, afxReal AllowedError, const afxNat *SparseBoneArray);

_AKX void AfxAccumulateBodyAnimationsLODSparse(akxBody bod, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    
    // if allowedErr == 0.f, won't use LOD.
    // if sparseBoneArray == NIL, won't use sparse.

    akxMotive *intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        akxMotor moto = intk->moto;
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        
        if (AfxMotorHasEffect(moto))
        {
            AnimationAccumulateBindingState(intk, basePivotIdx, pivotCnt, rslt, allowedErr, sparseBoneArray);
        }
    }
}

_AKX void AfxSampleBodyAnimationsLODSparse(akxBody bod, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    // if allowedErr == 0.f, won't use LOD.
    // if sparseBoneArray == NIL, won't use sparse.

    BeginLocalPoseAccumulation(rslt, basePivotIdx, pivotCnt, sparseBoneArray);
    AfxAccumulateBodyAnimationsLODSparse(bod, basePivotIdx, pivotCnt, rslt, allowedErr, sparseBoneArray);
    EndLocalPoseAccumulationLOD(rslt, basePivotIdx, pivotCnt, bod->skl, allowedErr, sparseBoneArray);
}

_AKX afxBool AfxSampleSingleBodyAnimationLODSparse(akxBody bod, akxMotor moto, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    AfxAssertObjects(1, &moto, afxFcc_MOTO);
    
    afxBool fnd = 0;
    BeginLocalPoseAccumulation(rslt, basePivotIdx, pivotCnt, sparseBoneArray);
    
    akxMotive *intk;
    AfxChainForEveryLinkage(&bod->motives, akxMotive, bod, intk)
    {
        akxMotor moto2 = intk->moto;
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

_AKX void AfxSampleBodyAnimationsAcceleratedLOD(akxBody bod, afxNat pivotCnt, afxM4d const offset, akxPose scratch, akxPoseBuffer rslt, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    AfxAssert(scratch);

    afxSkeleton skl = bod->skl;
    afxNat boneCnt = skl->jointCnt;

    if (pivotCnt > boneCnt)
        pivotCnt = boneCnt;

    if (!offset)
    {
        offset = AFX_M4D_IDENTITY;
    }

    afxLinkage const* anchor = AfxGetAnchorConst(&bod->motives);
    afxLinkage* lnk = AfxGetLastLinkage(&bod->motives);
    akxMotive* intk = AfxRebase(lnk, akxMotive*, bod);
    akxMotor moto;

    if (lnk && AfxMotorHasEffect((moto = intk->moto)))
    {
        AfxAssertObjects(1, &moto, afxFcc_MOTO);
        afxNat lodBoneCnt = AfxCountSkeletonJoints(skl, allowedErr);
        
        lnk = AfxGetNextLinkage(lnk);
        akxMotive*intk2 = AfxRebase(lnk, akxMotive*, bod);

        if (lnk && AfxMotorHasEffect((moto = intk2->moto)))
        {
            AfxSampleBodyAnimationsLODSparse(bod, 0, lodBoneCnt, scratch, allowedErr, 0);
            AfxComputePoseBufferLod(skl, 0, pivotCnt, 0, lodBoneCnt, scratch, offset, rslt);
        }
        else
        {
            AnimationBuildDirect(intk, pivotCnt, offset, rslt, allowedErr);
        }
    }
    else
    {
        AfxComputeRestPoseBuffer(skl, 0, pivotCnt, offset, rslt);
    }
}

_AKX afxError _AkxBodDtor(akxBody bod)
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

_AKX afxError _AkxBodCtor(akxBody bod, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    afxSimulation sim = cookie->udd[0];
    afxModel mdl = cookie->udd[1];

    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxSkeleton skl = AfxGetModelSkeleton(mdl);
    
    if (!skl) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &skl, afxFcc_SKL);

        AfxReacquireObjects(1, &mdl);
        AfxReacquireObjects(1, &skl);
        bod->mdl = mdl;
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

    return err;
}

_AKX afxClassConfig _AkxBodMgrCfg =
{
    .fcc = afxFcc_BOD,
    .name = "Body",
    .desc = "Body",
    .fixedSiz = sizeof(AFX_OBJECT(akxBody)),
    .ctor = (void*)_AkxBodCtor,
    .dtor = (void*)_AkxBodDtor
};

////////////////////////////////////////////////////////////////////////////////

_AKX afxError AfxAcquireBodies(afxModel mdl, afxNat cnt, akxBody bod[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxSimulation sim = AfxGetObjectProvider(mdl);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (AfxAcquireObjects(AfxGetBodyClass(sim), cnt, (afxObject*)bod, (void const*[]) { sim, mdl }))
        AfxThrowError();

    return err;
}
