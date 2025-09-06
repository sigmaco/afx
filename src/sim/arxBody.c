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
#define _ARX_BODY_C
#define _ARX_ANIMATION_C
#define _ARX_MOTION_C
#define _ARX_SKELETON_C
#include "impl/asxImplementation.h"

AFX_DEFINE_STRUCT(arxModelLink)
{
    afxLink     mdl; // arxModel.instances;
    afxLink     body; // arxBody.mdl;
};

_ARX afxBool ArxGetBodyModel(arxBody bod, arxModel* model)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    arxModel mdl = bod->mdl;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(model);
    *model = mdl;
    return (mdl != NIL);
}

_ARX afxBool ArxGetBodyPose(arxBody bod, arxPose* pose, arxPlacement* placement)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    arxPose pos = bod->pose;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_POSE, 1, &pos);
    AFX_ASSERT(pose);
    *pose = pos;

    if (placement)
        *placement = bod->placement;

    return (pos != NIL);
}

_ARX afxUnit ArxCountBodyMotives(arxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    return bod->motives.cnt;
}

_ARX void ArxStepBodyMotives(arxBody bod, afxReal time)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    
    // Update all the body's motives to be set to the current game clock.
    // like SetModelClock
    // it will automatically set all the motive's to the given clock, saving you the trouble of doing so.

    arxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(arxMotive), intk, bod, &bod->motives)
    {
        arxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);
        ArxStepCapstans(time, 1, &moto);
    }
}

_ARX void ArxPurgeTerminatedMotives(arxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    arxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(arxMotive), intk, bod, &bod->motives)
    {
        arxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);
        
        if (ArxCapstanHasTerminated(moto))
        {
            AfxDisposeObjects(1, &intk);
            //_ArxDestroyMotive(intk);
        }
    }
}

_ARX void ArxRecenterBodyMotiveClocks(arxBody bod, afxReal currClock)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    arxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(arxMotive), intk, bod, &bod->motives)
    {
        arxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);
        ArxRebaseCapstanTimes(currClock, 1, &moto);
    }
}

_ARX void ArxComputeBodyMotionVectors(arxBody bod, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation)
{
    // AfxGetBodyRootMotionVectors
    // AfxQueryBodyRootMotionVectors

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    afxReal totalWeight = 0.0;
    afxV4d t = { 0, 0, 0, 1 }, r = { 0, 0, 0, 0 };

    arxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(arxMotive), intk, bod, &bod->motives)
    {
        arxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

        if (ArxCapstanHasEffect(moto))
        {
            if (intk->isAnim) // only for anims; poses does not move.
            {
                intk->pimpl->AnimationAccumulateLoopTransform(intk, secsElapsed, &totalWeight, t, r, inverse);
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

_ARX void ArxComputeBodyMotionMatrix(arxBody bod, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    afxV4d t, r;
    ArxComputeBodyMotionVectors(bod, secsElapsed, inverse, t, r);
    AfxM4dApplyRigidMotion(m, mm, r, t);
}

_ARX void ArxAccumulateBodyAnimations(arxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, arxPose rslt, afxReal allowedErr, afxUnit const sparseJntMap[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    // ArxAccumulateBodyAnimationsLODSparse

    // if allowedErr == 0.f, won't use LOD.
    // if sparseBoneArray == NIL, won't use sparse.

    arxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(arxMotive), intk, bod, &bod->motives)
    {
        arxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

        if (ArxCapstanHasEffect(moto))
        {
            if (intk->isAnim)
                intk->pimpl->AnimationAccumulateBindingState(intk, basePivotIdx, pivotCnt, rslt, allowedErr, sparseJntMap);
            else
                intk->pimpl->PoseAccumulateBindingState(intk, basePivotIdx, pivotCnt, rslt, allowedErr, sparseJntMap);
        }
    }
}

#if 0
_ARX void AfxSampleBodyAnimations(arxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, arxPose pose, afxReal allowedErr, afxUnit const sparseJntMap[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    // AfxSampleBodyAnimationsLODSparse

    // if allowedErr == 0.f, won't use LOD.
    // if sparseBoneArray == NIL, won't use sparse.

    ArxCommencePoseAccumulation(pose, basePivotIdx, pivotCnt, sparseJntMap);

    ArxAccumulateBodyAnimations(bod, basePivotIdx, pivotCnt, pose, allowedErr, sparseJntMap);

    ArxConcludePoseAccumulation(pose, basePivotIdx, pivotCnt, bod->mdl, allowedErr, sparseJntMap);
}

_ARX afxBool AfxSampleSingleBodyAnimation(arxBody bod, arxCapstan moto, afxUnit basePivotIdx, afxUnit pivotCnt, arxPose pose, afxReal allowedErr, afxUnit const sparseJntMap[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

    // AfxSampleSingleBodyAnimationLODSparse

    afxBool fnd = 0;
    ArxCommencePoseAccumulation(pose, basePivotIdx, pivotCnt, sparseJntMap);

    arxMotive intk;
    AFX_ITERATE_CHAIN(&bod->motives, AFX_OBJ(arxMotive), bod, intk)
    {
        arxCapstan moto2 = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto2);

        if (moto2 == moto)
        {
            if (ArxCapstanHasEffect(moto))
            {
                if (intk->isAnim)
                    _ArxSimGetAnimVmt(AfxGetHost(bod))->AnimationAccumulateBindingState(intk, basePivotIdx, pivotCnt, pose, allowedErr, sparseJntMap);
                else
                    _ArxSimGetAnimVmt(AfxGetHost(bod))->PoseAccumulateBindingState(intk, basePivotIdx, pivotCnt, pose, allowedErr, sparseJntMap);
            }
            ++fnd;
        }
    }

    ArxConcludePoseAccumulation(pose, basePivotIdx, pivotCnt, bod->mdl, allowedErr, sparseJntMap);
    return fnd;
}

_ARX void AfxSampleBodyAnimationsAccelerated(arxBody bod, afxUnit pivotCnt, afxM4d const displace, arxPose scratch, arxPlacement plce, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    AFX_ASSERT(scratch);

    // AfxSampleBodyAnimationsAcceleratedLOD

    afxUnit maxJntCnt = ArxCountJoints(bod->mdl, 0);

    if (pivotCnt > maxJntCnt)
        pivotCnt = maxJntCnt;

    if (!displace)
        displace = AFX_M4D_IDENTITY;
    
    if (bod->motives.cnt == 1)
    {
        // when only one animation is running, we can fast generate a placement.
        afxLink* lnk = AfxGetLastLink(&bod->motives);
        arxMotive intk = AFX_REBASE(lnk, AFX_OBJ(arxMotive), bod);

        if (intk->isAnim)
            _ArxSimGetAnimVmt(AfxGetHost(bod))->AnimationBuildDirect(intk, pivotCnt, displace, plce, allowedErr);
        else
            _ArxSimGetAnimVmt(AfxGetHost(bod))->PoseBuildDirect(intk, pivotCnt, displace, plce);
    }
    else if (bod->motives.cnt > 1)
    {
        // when more than one anim is running, we must compute pose by accumulation of all animation samples.
        afxUnit lodJntCnt = ArxCountJoints(bod->mdl, allowedErr);
        AfxSampleBodyAnimations(bod, 0, lodJntCnt, scratch, allowedErr, 0);
        ArxBuildPlacement(plce, scratch, bod->mdl, 0, pivotCnt, 0, lodJntCnt, displace, FALSE);
    }
    else
    {
        // when no animation is running, we can just compute the rest pose.
        ArxBuildPlacement(plce, NIL, bod->mdl, 0, pivotCnt, 0, pivotCnt, displace, FALSE);
    }
}
#endif

#if 0
_ZCL afxBool AfxSampleBodyAnimationsUnified(arxBody bod, arxAnimSampleContext const* ctx)
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

_ARX void ArxDoBodyDynamics(arxBody bod, afxReal dt)
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

_ARX void ArxApplyForceAndTorque(arxBody bod, afxV3d const force, afxV3d const torque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    AfxV3dAdd(bod->rigid.force, bod->rigid.force, force);
    AfxV3dAdd(bod->rigid.torque, bod->rigid.torque, torque);
}

_ARX afxError ArxNodulateBody(arxBody bod, afxUnit basePartIdx, afxUnit cnt, arxNode nod, void(*sync)(arxNodular*), afxFlags flags, afxMask mask)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    AFX_ASSERT_RANGE(bod->partCnt, basePartIdx, cnt);

    for (afxUnit i = 0; i < bod->partCnt; i++)
    {
        afxUnit partIdx = basePartIdx + i;
        arxBodyPart* part = &bod->parts[partIdx];

        arxNode curr = AfxGetLinker(&part->nodr.nod);

        if (curr != nod)
        {
            if (curr)
            {
                AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &curr);
                AfxPopLink(&part->nodr);
            }

            if (nod)
            {
                AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
                AfxPushLink(&part->nodr, &nod->nodulars);
            }
        }

        part->nodr.mask = mask;
        part->nodr.flags = flags;
        if (sync) part->nodr.sync = sync;
    }

    return err;
}

_ARX afxError _ArxBodDtorCb(arxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    arxMotive intk;
    AFX_ITERATE_CHAIN(AFX_OBJ(arxMotive), intk, bod, &bod->motives)
    {
        //_ArxDestroyMotive(intk);
        AfxDisposeObjects(1, &intk);
    }

    if (bod->mdl)
        AfxDisposeObjects(1, &bod->mdl);

    if (bod->pose)
        AfxDisposeObjects(1, &bod->pose);

    if (bod->placement)
        AfxDisposeObjects(1, &bod->placement);

    if (bod->parts)
    {
        afxObjectStash bodpStash =
        {
            .cnt = bod->partCnt,
            .siz = sizeof(bod->parts[0]),
            .var = &bod->parts,
        };
        if (AfxDeallocateInstanceData(bod, 1, &bodpStash))
            AfxThrowError();
    }

    return err;
}

_ARX afxError _ArxBodCtorCb(arxBody bod, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    afxSimulation sim = args[0];
    arxModel mdl = args[1];

    AfxDeployChain(&bod->motives, bod);
    bod->reserved0 = 0;
    bod->reserved1 = 0;

    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    afxUnit jntCnt = ArxCountJoints(mdl, 0);

    arxModelInfo mdli;
    ArxDescribeModel(mdl, &mdli);

    afxObjectStash bodpStash =
    {
        .cnt = mdli.rigCnt,
        .siz = sizeof(bod->parts[0]),
        .var = &bod->parts,
    };
    if (AfxAllocateInstanceData(bod, 1, &bodpStash))
    {
        AfxThrowError();
    }

    AFX_ASSERT(bod->parts);
    bod->partCnt = mdli.rigCnt;

    for (afxUnit i = 0; i < bod->partCnt; i++)
    {
        arxBodyPart* part = &bod->parts[i];
        *part = (arxBodyPart) { 0 };
        part->rigIdx = i; // actually 1:1
    }

    AfxReacquireObjects(1, &mdl);
    bod->mdl = mdl;

    AfxV3dZero(bod->rigid.x); // Initial position (m)
    AfxV3dZero(bod->rigid.v); // Initial velocity (m/s)
    AfxV3dZero(bod->rigid.omega); // Initial angular velocity (rad/s)
    AfxV3dZero(bod->rigid.force); // No external force initially
    AfxV3dZero(bod->rigid.torque); // No external torque initially
    AfxQuatReset(bod->rigid.q); // Identity quaternion (no rotation)
    AfxV3dZero(bod->rigid.angularMom); // Initial angular momentum
    AfxV3dZero(bod->rigid.linearMom); // Initial linear momentum
    AfxM3dReset(bod->rigid.Ibody); // Moment of inertia (kg*m^2)
    bod->rigid.mass = AFX_R32_MAX; // Mass in kg

    arxRenderware rwe = AfxGetSimulationDrawInput(sim);

    arxPoseInfo posei = { 0 };
    posei.artCnt = jntCnt;
    ArxAcquirePoses(rwe, 1, &posei, &bod->pose);
    ArxAcquirePlacements(rwe, 1, (afxUnit[]) { jntCnt }, NIL, &bod->placement);

    return err;
}

_ARX afxClassConfig _ARX_BOD_CLASS_CONFIG =
{
    .fcc = afxFcc_BOD,
    .name = "Body",
    .desc = "Modelled Body Instance",
    .fixedSiz = sizeof(AFX_OBJECT(arxBody)),
    .ctor = (void*)_ArxBodCtorCb,
    .dtor = (void*)_ArxBodDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxUnit AfxEnumerateBodies(afxSimulation sim, afxUnit first, afxUnit cnt, arxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = _ArxGetBodyClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_BOD);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)bodies);
}

_ARX afxUnit AfxInvokeBodies(afxSimulation sim, afxUnit first, afxUnit cnt, afxBool(*f)(arxBody, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass const* cls = _ArxGetBodyClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_BOD);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_ARX afxError ArxAcquireBodies(afxSimulation sim, arxModel mdl, afxUnit cnt, arxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_ArxGetBodyClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_BOD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)bodies, (void const*[]) { sim, mdl }))
        AfxThrowError();

    return err;
}

_ARX afxUnit ArxPerformManipulatedPose(arxPose pose, afxReal startTime, afxReal duration, afxUnit iterCnt, arxTrackMask* modelMask, afxUnit cnt, arxBody bodies[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT(bodies);
    afxUnit rslt = 0;

    arxRenderware rwe = AfxGetHost(pose);
    afxSimulation sim = NIL;
    arxTrackTarget targets[256];

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        arxBody bod = bodies[mdlIdx];

        if (!bod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
        sim = AfxGetHost(bod);

        targets[mdlIdx] = (arxTrackTarget) { 0 };
        targets[mdlIdx].ModelMask = modelMask;
        targets[mdlIdx].OnInstance = bod;
    }

    arxCapstan moto;
    arxCapstanConfig motoCfg = { 0 };
    motoCfg.currTime = startTime;
    motoCfg.localDur = duration;
    motoCfg.iterCnt = iterCnt;

    if (ArxAcquireCapstans(sim, &motoCfg, 1, &moto))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

        arxMotive motives[256];
        if (AfxAcquireObjects((afxClass*)_ArxGetMotiveClass(sim), cnt, (afxObject*)motives, (void const*[]) { sim, pose, moto, &targets[0] }))
        {
            AfxThrowError();
        }
        AfxDisposeObjects(1, &moto);
    }
    return rslt;
}
