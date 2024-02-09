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
#define _AFX_POSE_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxQuaternion.h"

_AFX afxNat AfxGetPoseCapacity(awxPose const *lp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lp);
    return lp->cap;
}

_AFX afxTransform* AfxGetPoseTransform(awxPose const *lp, afxNat artIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lp);
    AfxAssert(artIdx < lp->cap);
    return &lp->xforms[artIdx].xform;
}

_AFX void AfxCopyPose(awxPose *pose, awxPose const *from)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(pose);
    AfxAssert(from);

    for (afxNat i = 0; i < from->cap; i++)
    {
        pose->xforms[i].weight = from->xforms[i].weight;
        pose->xforms[i].cnt = from->xforms[i].cnt;
        pose->xforms[i].traversalId = from->xforms[i].traversalId;
        AfxCopyTransform(&pose->xforms[i].xform, &from->xforms[i].xform);
        AfxThrowError(); // incompleto por causa da loucura que estava no IDA.
    }
}

_AFX void AfxApplyRootMotionVectorsToPose(awxPose *pose, afxReal const translation[3], afxReal const rotation[3])
{
    afxTransform* t = AfxGetPoseTransform(pose, 0);
    AfxAddV3d(t->position, t->position, translation);

    afxQuat rot;
    AfxQuatFromAngularVelocity(rot, rotation);
    AfxMultiplyQuat(t->orientation, rot, t->orientation);
}

_AFX void AfxReleasePoses(afxNat cnt, awxPose *lp[])
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (lp[i]->xforms)
            AfxDeallocate(NIL, lp[i]->xforms);

        AfxDeallocate(NIL, lp[i]);
    }
}

_AFX afxError AfxAcquirePoses(void *sim, afxNat cnt, afxNat const cap[], awxPose *lp[])
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cnt; i++)
    {
        lp[i] = AfxAllocate(NIL, 1, sizeof(*lp[0]), 0, AfxHint());
        AfxAssert(lp[i]);
        lp[i]->cap = cap[i];
        lp[i]->fillThreshold = 0.2;
        lp[i]->traversalId = 0;
        lp[i]->xforms = lp[i]->cap ? AfxAllocate(NIL, lp[i]->cap, sizeof(lp[i]->xforms[0]), 0, AfxHint()) : NIL;
        AfxAssert(lp[i]->xforms);
        AfxZero(lp[i]->cap, sizeof(lp[i]->xforms[0]), lp[i]->xforms);
    }
    return err;
}
