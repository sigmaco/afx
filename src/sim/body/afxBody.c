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
#include "../impl/amxImplementation.h"

_AMX afxBool AfxGetBodyModel(afxBody bod, afxModel* model)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    afxModel mdl = bod->mdl;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(model);
    *model = mdl;
    return (mdl != NIL);
}

_AMX void AfxUpdateBodyMotives(afxBody bod, afxReal newClock)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    
    // Update all the body's motives to be set to the current game clock.
    // like SetModelClock
    // it will automatically set all the motive's to the given clock, saving you the trouble of doing so.

    amxMotive intk;
    AfxChainForEveryLinkage(&bod->motives, AFX_OBJ(amxMotive), bod, intk)
    {
        afxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);
        AfxUpdateCapstanClock(newClock, 1, &moto);
    }
}

_AMX void AfxPurgeTerminatedMotives(afxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    amxMotive intk;
    AfxChainForEveryLinkage(&bod->motives, AFX_OBJ(amxMotive), bod, intk)
    {
        afxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);
        
        if (AfxCapstanHasTerminated(moto))
        {
            AfxDisposeObjects(1, &intk);
            //_AmxDestroyMotive(intk);
        }
    }
}

_AMX void AfxRecenterBodyMotiveClocks(afxBody bod, afxReal currClock)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    amxMotive intk;
    AfxChainForEveryLinkage(&bod->motives, AFX_OBJ(amxMotive), bod, intk)
    {
        afxCapstan moto = intk->moto;
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);
        AfxRebaseCapstanClocks(currClock, 1, &moto);
    }
}

_AMX void AfxDoBodyDynamics(afxBody bod, afxReal dt)
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

_AMX void AfxApplyForceAndTorque(afxBody bod, afxV3d const force, afxV3d const torque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
    AfxV3dAdd(bod->rigid.force, bod->rigid.force, force);
    AfxV3dAdd(bod->rigid.torque, bod->rigid.torque, torque);
}

_AMX afxError _AmxBodDtorCb(afxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    amxMotive intk;
    AfxChainForEveryLinkage(&bod->motives, AFX_OBJ(amxMotive), bod, intk)
    {
        //_AmxDestroyMotive(intk);
        AfxDisposeObjects(1, &intk);
    }

    if (bod->mdl)
        AfxDisposeObjects(1, &bod->mdl);

    return err;
}

_AMX afxError _AmxBodCtorCb(afxBody bod, void** args, afxUnit invokeNo)
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

    AfxAcquirePoses(sim, 1, (afxUnit[]) { AfxCountJoints(mdl, 0) }, &bod->pose);

    return err;
}

_AMX afxClassConfig _AMX_BOD_CLASS_CONFIG =
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
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = _AmxGetBodyClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_BOD);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)bodies);
}

_AMX afxUnit AfxInvokeBodies(afxSimulation sim, afxUnit first, afxUnit cnt, afxBool(*f)(afxBody, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass const* cls = _AmxGetBodyClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_BOD);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AMX afxError AfxAcquireBodies(afxSimulation sim, afxModel mdl, afxUnit cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_AmxGetBodyClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_BOD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)bodies, (void const*[]) { sim, mdl }))
        AfxThrowError();

    return err;
}



