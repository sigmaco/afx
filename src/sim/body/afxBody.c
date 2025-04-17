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
#define _ASX_BODY_C
#define _ASX_ANIMATION_C
#define _ASX_MOTION_C
#define _ASX_SKELETON_C
#define _ASX_SIMULATION_C
#include "../impl/asxImplementation.h"

AFX_DEFINE_STRUCT(afxModelLink)
{
    afxLink     mdl; // afxModel.instances;
    afxLink     body; // afxBody.mdl;
};

_ASX afxBool AfxGetBodyModel(afxBody bod, afxModel* model)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    afxModel mdl = bod->mdl;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(model);
    *model = mdl;
    return (mdl != NIL);
}

_ASX afxBool AfxGetBodyPose(afxBody bod, afxPose* pose)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    afxPose pos = bod->pose;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_POSE, 1, &pos);
    AFX_ASSERT(pose);
    *pose = pos;
    return (pos != NIL);
}

_ASX afxUnit AfxCountBodyMotives(afxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    return bod->motives.cnt;
}

_ASX void AfxUpdateBodyMotives(afxBody bod, afxReal newClock)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    
    // Update all the body's motives to be set to the current game clock.
    // like SetModelClock
    // it will automatically set all the motive's to the given clock, saving you the trouble of doing so.

    asxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(asxMotive), intk, bod, &bod->motives)
    {
        afxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);
        AfxUpdateCapstanClock(newClock, 1, &moto);
    }
}

_ASX void AfxPurgeTerminatedMotives(afxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    asxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(asxMotive), intk, bod, &bod->motives)
    {
        afxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);
        
        if (AfxCapstanHasTerminated(moto))
        {
            AfxDisposeObjects(1, &intk);
            //_AsxDestroyMotive(intk);
        }
    }
}

_ASX void AfxRecenterBodyMotiveClocks(afxBody bod, afxReal currClock)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    asxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(asxMotive), intk, bod, &bod->motives)
    {
        afxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);
        AfxRebaseCapstanClocks(currClock, 1, &moto);
    }
}

_ASX void AfxQueryBodyRootMotionVectors(afxBody bod, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation)
{
    // AfxGetBodyRootMotionVectors

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    afxReal totalWeight = 0.0;
    afxV4d t = { 0, 0, 0, 1 }, r = { 0, 0, 0, 0 };

    asxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(asxMotive), intk, bod, &bod->motives)
    {
        afxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

        if (AfxCapstanHasEffect(moto))
        {
            if (intk->isAnim) // only for anims; poses does not move.
            {
                _AsxSimGetAnimVmt(AfxGetProvider(bod))->AnimationAccumulateLoopTransform(intk, secsElapsed, &totalWeight, t, r, inverse);
            }
        }
    }

    if (totalWeight > 0.001)
    {
        afxReal s = 1.0 / totalWeight;
        AfxV3dScale(translation, t, s);
        AfxV3dScale(rotation, r, s);
    }
}

_ASX void AfxUpdateBodyMatrix(afxBody bod, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    afxV4d t, r;
    AfxQueryBodyRootMotionVectors(bod, secsElapsed, inverse, t, r);
    AfxApplyMatrixRootMotionVectors(t, r, mm, m);
}

_ASX void AfxAccumulateBodyAnimations(afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose rslt, afxReal allowedErr, afxUnit const sparseJntMap[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    // AfxAccumulateBodyAnimationsLODSparse

    // if allowedErr == 0.f, won't use LOD.
    // if sparseBoneArray == NIL, won't use sparse.

    asxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(asxMotive), intk, bod, &bod->motives)
    {
        afxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

        if (AfxCapstanHasEffect(moto))
        {
            if (intk->isAnim)
                _AsxSimGetAnimVmt(AfxGetProvider(bod))->AnimationAccumulateBindingState(intk, basePivotIdx, pivotCnt, rslt, allowedErr, sparseJntMap);
            else
                _AsxSimGetAnimVmt(AfxGetProvider(bod))->PoseAccumulateBindingState(intk, basePivotIdx, pivotCnt, rslt, allowedErr, sparseJntMap);
        }
    }
}

#if 0
_ASX void AfxSampleBodyAnimations(afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const sparseJntMap[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    // AfxSampleBodyAnimationsLODSparse

    // if allowedErr == 0.f, won't use LOD.
    // if sparseBoneArray == NIL, won't use sparse.

    AsxCommencePoseAccumulation(pose, basePivotIdx, pivotCnt, sparseJntMap);

    AfxAccumulateBodyAnimations(bod, basePivotIdx, pivotCnt, pose, allowedErr, sparseJntMap);

    AsxConcludePoseAccumulation(pose, basePivotIdx, pivotCnt, bod->mdl, allowedErr, sparseJntMap);
}

_ASX afxBool AfxSampleSingleBodyAnimation(afxBody bod, afxCapstan moto, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose pose, afxReal allowedErr, afxUnit const sparseJntMap[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

    // AfxSampleSingleBodyAnimationLODSparse

    afxBool fnd = 0;
    AsxCommencePoseAccumulation(pose, basePivotIdx, pivotCnt, sparseJntMap);

    asxMotive intk;
    AFX_ITERATE_CHAIN(&bod->motives, AFX_OBJ(asxMotive), bod, intk)
    {
        afxCapstan moto2 = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto2);

        if (moto2 == moto)
        {
            if (AfxCapstanHasEffect(moto))
            {
                if (intk->isAnim)
                    _AsxSimGetAnimVmt(AfxGetProvider(bod))->AnimationAccumulateBindingState(intk, basePivotIdx, pivotCnt, pose, allowedErr, sparseJntMap);
                else
                    _AsxSimGetAnimVmt(AfxGetProvider(bod))->PoseAccumulateBindingState(intk, basePivotIdx, pivotCnt, pose, allowedErr, sparseJntMap);
            }
            ++fnd;
        }
    }

    AsxConcludePoseAccumulation(pose, basePivotIdx, pivotCnt, bod->mdl, allowedErr, sparseJntMap);
    return fnd;
}

_ASX void AfxSampleBodyAnimationsAccelerated(afxBody bod, afxUnit pivotCnt, afxM4d const displace, afxPose scratch, afxPlacement plce, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    AFX_ASSERT(scratch);

    // AfxSampleBodyAnimationsAcceleratedLOD

    afxUnit maxJntCnt = AsxCountJoints(bod->mdl, 0);

    if (pivotCnt > maxJntCnt)
        pivotCnt = maxJntCnt;

    if (!displace)
        displace = AFX_M4D_IDENTITY;
    
    if (bod->motives.cnt == 1)
    {
        // when only one animation is running, we can fast generate a placement.
        afxLink* lnk = AfxGetLastLink(&bod->motives);
        asxMotive intk = AFX_REBASE(lnk, AFX_OBJ(asxMotive), bod);

        if (intk->isAnim)
            _AsxSimGetAnimVmt(AfxGetProvider(bod))->AnimationBuildDirect(intk, pivotCnt, displace, plce, allowedErr);
        else
            _AsxSimGetAnimVmt(AfxGetProvider(bod))->PoseBuildDirect(intk, pivotCnt, displace, plce);
    }
    else if (bod->motives.cnt > 1)
    {
        // when more than one anim is running, we must compute pose by accumulation of all animation samples.
        afxUnit lodJntCnt = AsxCountJoints(bod->mdl, allowedErr);
        AfxSampleBodyAnimations(bod, 0, lodJntCnt, scratch, allowedErr, 0);
        AfxBuildPlacement(plce, scratch, bod->mdl, 0, pivotCnt, 0, lodJntCnt, displace, FALSE);
    }
    else
    {
        // when no animation is running, we can just compute the rest pose.
        AfxBuildPlacement(plce, NIL, bod->mdl, 0, pivotCnt, 0, pivotCnt, displace, FALSE);
    }
}
#endif

#if 0
_ZCL afxBool AfxSampleBodyAnimationsUnified(afxBody bod, afxAnimSampleContext const* ctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    afxBool rslt = TRUE;

    if (ctx->moto)
    {
        rslt = AfxSampleSingleBodyAnimation(bod, ctx->moto, ctx->firstPivot, ctx->pivotCnt, ctx->pose, ctx->allowedErr, ctx->sparseBoneArray);
    }
    else
    {
        if (ctx->accelerated)
        {
            AfxSampleBodyAnimationsAccelerated(bod, ctx->pivotCnt, ctx->displacement, ctx->pose, ctx->posb, ctx->allowedErr);
        }
        else
        {
            AfxSampleBodyAnimations(bod, ctx->firstPivot, ctx->pivotCnt, ctx->pose, ctx->allowedErr, ctx->sparseBoneArray);
        }
    }
}
#endif

_ASX void AfxDoBodyDynamics(afxBody bod, afxReal dt)
{
    // compute inertia matrices
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (i == j) {
                bod->rigid.Ibodyinv[i][j] = 1.0 / bod->rigid.Ibody[i][i];
            }
            else
            {
                bod->rigid.Ibodyinv[i][j] = 0.0;
            }
        }
    }

    // Compute linear momentum p = m * v
    AfxV3dScale(bod->rigid.linearMom, bod->rigid.v, bod->rigid.mass);

    // Compute angular momentum L = I * omega
    AfxV3dScale(bod->rigid.angularMom, bod->rigid.omega, bod->rigid.Ibody[0][0]); // For simplicity with diagonal inertia tensor
    
    // Linear acceleration (F = ma)
    afxV3d t;
    afxV3d acceleration;
    AfxV3dScale(acceleration, bod->rigid.force, 1.0 / bod->rigid.mass);
    AfxV3dScale(t, acceleration, dt);
    AfxV3dAdd(bod->rigid.v, bod->rigid.v, t);
    AfxV3dScale(t, bod->rigid.v, dt);
    AfxV3dAdd(bod->rigid.x, bod->rigid.x, t);

    // Angular acceleration (alpha = I^(-1) * tau)
    afxV3d angular_acceleration;
    angular_acceleration[0] = bod->rigid.torque[0] * bod->rigid.Ibodyinv[0][0];
    angular_acceleration[1] = bod->rigid.torque[1] * bod->rigid.Ibodyinv[1][1];
    angular_acceleration[2] = bod->rigid.torque[2] * bod->rigid.Ibodyinv[2][2];

    // Update angular velocity and orientation (omega = omega0 + alpha * dt)
    afxV3d angular_acceleration2;
    AfxV3dScale(angular_acceleration2, angular_acceleration, dt);
    AfxV3dAdd(bod->rigid.omega, bod->rigid.omega, angular_acceleration2);

    // Update the orientation using angular velocity (simplified method)
    afxQuat omega_quaternion = { bod->rigid.omega[0], bod->rigid.omega[1], bod->rigid.omega[2], 0 };
    afxQuat delta_orientation;
    AfxQuatMultiply(delta_orientation, bod->rigid.q, omega_quaternion);
    AfxQuatNormalize(bod->rigid.q, delta_orientation);

    AfxV3dZero(bod->rigid.force);
    AfxV3dZero(bod->rigid.torque);
}

_ASX void AfxApplyForceAndTorque(afxBody bod, afxV3d const force, afxV3d const torque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    AfxV3dAdd(bod->rigid.force, bod->rigid.force, force);
    AfxV3dAdd(bod->rigid.torque, bod->rigid.torque, torque);
}

_ASX afxError _AsxBodDtorCb(afxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    asxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(asxMotive), intk, bod, &bod->motives)
    {
        //_AsxDestroyMotive(intk);
        AfxDisposeObjects(1, &intk);
    }

    if (bod->mdl)
        AfxDisposeObjects(1, &bod->mdl);

    if (bod->pose)
        AfxDisposeObjects(1, &bod->pose);

    return err;
}

_ASX afxError _AsxBodCtorCb(afxBody bod, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    afxSimulation sim = args[0];
    afxModel mdl = args[1];
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    AfxReacquireObjects(1, &mdl);
    bod->mdl = mdl;
    AfxDeployChain(&bod->motives, bod);
    bod->reserved0 = 0;
    bod->reserved1 = 0;

    AfxV3dZero(bod->rigid.x); // Initial position (m)
    AfxV3dZero(bod->rigid.v); // Initial velocity (m/s)
    AfxV3dZero(bod->rigid.omega); // Initial angular velocity (rad/s)
    AfxV3dZero(bod->rigid.force); // No external force initially
    AfxV3dZero(bod->rigid.torque); // No external torque initially
    AfxQuatReset(bod->rigid.q); // Identity quaternion (no rotation)
    AfxV3dZero(bod->rigid.angularMom); // Initial angular momentum
    AfxV3dZero(bod->rigid.linearMom); // Initial linear momentum
    AfxM3dZero(bod->rigid.Ibody); // Moment of inertia (kg*m^2)
    bod->rigid.mass = 0; // Mass in kg

    AfxM4dReset(bod->placement);

    AfxAcquirePoses(sim, 1, (afxUnit[]) { AsxCountJoints(mdl, 0) }, &bod->pose);

    return err;
}

_ASX afxClassConfig _ASX_BOD_CLASS_CONFIG =
{
    .fcc = afxFcc_BOD,
    .name = "Body",
    .desc = "Modelled Body Instance",
    .fixedSiz = sizeof(AFX_OBJECT(afxBody)),
    .ctor = (void*)_AsxBodCtorCb,
    .dtor = (void*)_AsxBodDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxUnit AfxEnumerateBodies(afxSimulation sim, afxUnit first, afxUnit cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = _AsxGetBodyClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_BOD);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)bodies);
}

_ASX afxUnit AfxInvokeBodies(afxSimulation sim, afxUnit first, afxUnit cnt, afxBool(*f)(afxBody, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass const* cls = _AsxGetBodyClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_BOD);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_ASX afxError AfxAcquireBodies(afxSimulation sim, afxModel mdl, afxUnit cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_AsxGetBodyClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_BOD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)bodies, (void const*[]) { sim, mdl }))
        AfxThrowError();

    return err;
}



