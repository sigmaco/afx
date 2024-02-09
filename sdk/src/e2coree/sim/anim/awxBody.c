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

#define _AFX_SIM_C
#define _AFX_BODY_C
#define _AFX_SKELETON_C
#define _AFX_SIMULATION_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"

_AFX afxModel AwxGetBodyModel(awxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    afxModel mdl = bod->mdl;
    AfxTryAssertObjects(1, &mdl, afxFcc_MDL);
    return mdl;
}

_AFX afxSkeleton AwxGetBodySkeleton(awxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    afxSkeleton skl = bod->cachedSkl;
    AfxTryAssertObjects(1, &skl, afxFcc_SKL);
    return skl;
}

_AFX void AwxSetBodyClock(awxBody bod, afxReal newClock)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    awxAnimusInterlink *intk;
    AfxChainForEveryLinkage(&bod->motors, awxAnimusInterlink, bod, intk)
    {
        awxAnimus anir = _AwxAnimusInterlinkGetAnimus(intk);
        afxReal v6 = newClock - anir->currentClock;
        anir->currentClock = newClock;
        anir->dTLocalClockPending = v6 + anir->dTLocalClockPending;
    }
}

_AFX void AwxFreeCompletedBodyControls(awxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    awxAnimusInterlink *intk;
    AfxChainForEveryLinkage(&bod->motors, awxAnimusInterlink, bod, intk)
    {
        awxAnimus anir = _AwxAnimusInterlinkGetAnimus(intk);
        AwxFreeMotorIfComplete(anir);
    }
}

_AFX void** AwxGetBodyUserDataArray(awxBody bod)
{
    return bod->userData;
}

_AFX void AwxResetBodyClock(awxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    AfxError("Not implemented");
}

_AFX void GetUnnormalizedRootMotionVectors(awxBody bod, afxReal secsElapsed, afxBool inverse, afxReal *totalWeight, afxReal translation[3], afxReal rotation[3])
{
    awxAnimusInterlink *intk;
    AfxChainForEveryLinkage(&bod->motors, awxAnimusInterlink, bod, intk)
    {
        awxAnimus anir = _AwxAnimusInterlinkGetAnimus(intk);
        
        if (!(_AfxMotorHasEffect(anir)))
            break;

        AfxAccumulateAnimusInterlinkLoopTransform(intk, secsElapsed, totalWeight, translation, rotation, inverse);
    }
}

_AFX void AwxGetBodyRootMotionVectors(awxBody bod, afxReal secsElapsed, afxBool inverse, afxReal translation[3], afxReal rotation[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    AfxZeroV3d(translation);
    AfxZeroV3d(rotation);

    afxReal totalWeight = 0.0;
    GetUnnormalizedRootMotionVectors(bod, secsElapsed, inverse, &totalWeight, translation, rotation);

    if (totalWeight > 0.001)
    {
        afxReal s = 1.0 / totalWeight;
        AfxScaleV3d(translation, translation, s);
        AfxScaleV3d(rotation, rotation, s);
    }
}

_AFX void AwxUpdateBodyModelMatrix(awxBody bod, afxReal secsElapsed, afxBool inverse, afxReal const mm[4][4], afxReal m[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);
    
    afxV3d t, r;
    AwxGetBodyRootMotionVectors(bod, secsElapsed, inverse, t, r);
    AfxApplyRootMotionVectorsToMatrix(t, r, mm, m);
}

_AFX void AwxRecenterBodyAllClocks(awxBody bod, afxReal currClock)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    awxAnimusInterlink *intk;
    AfxChainForEveryLinkage(&bod->motors, awxAnimusInterlink, bod, intk)
    {
        awxAnimus anir = _AwxAnimusInterlinkGetAnimus(intk);
        AwxRecenterAnimusClocks(anir, currClock);
    }
}

_AFX afxError _AwxBodDtor(awxBody bod)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("bod=%p", bod);
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    if (bod->mdl)
        AfxReleaseObjects(1, (void*[]) { bod->mdl });

    _FreeBodyRing(&bod->motors);

    return err;
}

_AFX afxError _AwxBodCtor(awxBody bod, afxCookie const* cookie)
{
    AfxEntry("bod=%p", bod);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    afxSimulation sim = cookie->udd[0];
    afxModel mdl = cookie->udd[1];

    afxSkeleton skl = AfxGetModelSkeleton(mdl);
    
    if (!skl) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &skl, afxFcc_SKL);

        AfxReacquireObjects(1, (void*[]) { mdl });
        bod->mdl = mdl;
        bod->cachedSkl = skl;
        bod->cachedBones = skl->bones;
        bod->cachedBoneCnt = skl->boneCnt;
        AfxTakeChain(&bod->motors, bod);
        bod->reserved0 = 0;
        bod->reserved1 = 0;
    }

    bod->mass = AfxScalar(0);
    AfxZeroV4d(bod->centre);
    AfxZeroV4d(bod->velocity);
    AfxZeroV4d(bod->force);
    AfxZeroV4d(bod->torque);
    bod->linearDamping = 0;
    bod->angularDamping = 0;

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AwxEmbodyModel(afxModel mdl, afxNat cnt, awxBody bod[])
{
    AfxEntry("mdl=%p", mdl);

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxSimulation sim = AfxGetObjectProvider(mdl);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (AfxAcquireObjects(AfxGetBodyClass(sim), cnt, (afxObject*)bod, (void const*[]) { sim, mdl }))
        AfxThrowError();

    return err;
}

_AFX afxClassConfig _AwxBodClsConfig =
{
    .fcc = afxFcc_BOD,
    .name = "Body",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(awxBody)),
    .mmu = NIL,
    .ctor = (void*)_AwxBodCtor,
    .dtor = (void*)_AwxBodDtor
};
