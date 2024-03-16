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
#define _AFX_WORLD_POSE_C
#include "qwadro/sim/afxSimulation.h"

_AKX afxM4d* AfxPoseBufferGetWorldMatrixArray(awxPoseBuffer const *wp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    return wp->world;
}

_AKX afxM4d* AfxPoseBufferGetCompositeMatrixArray(awxPoseBuffer const *wp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    return wp->composite;
}

_AKX afxV4d* AfxPoseBufferGetCompositeMatrix(awxPoseBuffer const *wp, afxNat artIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    AfxAssert(artIdx < wp->matCnt);
    return wp->composite[artIdx];
}

_AKX afxV4d* AfxPoseBufferGetWorldMatrix(awxPoseBuffer const *wp, afxNat artIdx)
{
    //float (*__cdecl GetPoseBuffer4x4(const world_pose *PoseBuffer, int BoneIndex))[4]
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    AfxAssert(artIdx < wp->matCnt);
    return wp->world[artIdx];
}

_AKX afxNat AfxPoseBufferGetArticulationCount(awxPoseBuffer const *wp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    return wp->matCnt;
}

_AKX void AfxReleasePoseBuffers(afxNat cnt, awxPoseBuffer *wp[])
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (wp[i]->composite)
            AfxDeallocate(NIL, wp[i]->composite);

        if (wp[i]->world)
            AfxDeallocate(NIL, wp[i]->world);

        AfxDeallocate(NIL, wp[i]);
    }
}

_AKX afxError AfxAcquirePoseBuffers(void *sim, afxNat cnt, afxNat const artCnt[], afxBool const excludeComposite[], awxPoseBuffer *wp[])
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cnt; i++)
    {
        wp[i] = AfxAllocate(NIL, 1, sizeof(*wp[0]), 0, AfxHint());
        AfxAssert(wp[i]);
        wp[i]->matCnt = artCnt[i];
        wp[i]->world = wp[i]->matCnt ? AfxAllocate(NIL, wp[i]->matCnt, sizeof(wp[i]->world[0]), 0, AfxHint()) : NIL;
        wp[i]->composite = (excludeComposite && !excludeComposite[i]) && wp[i]->matCnt ? AfxAllocate(NIL, wp[i]->matCnt, sizeof(wp[i]->composite[0]), 0, AfxHint()) : NIL;
    }
    return err;
}
