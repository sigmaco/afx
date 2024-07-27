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
#define _AKX_POSE_BUFFER_C
#include "../sim/dev/AkxSimDevKit.h"

_AKX afxM4d* AfxPoseBufferGetWorldMatrixArray(akxPoseBuffer const wp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    return wp->world;
}

_AKX afxM4d* AfxPoseBufferGetCompositeMatrixArray(akxPoseBuffer const wp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    return wp->composite;
}

_AKX afxM4d* AfxGetCompositeMatrices(akxPoseBuffer const wp, afxNat baseArtIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    AfxAssertRange(wp->matCnt, baseArtIdx, 1);
    return &wp->composite[baseArtIdx];
}

_AKX afxM4d* AfxGetWorldMatrices(akxPoseBuffer const wp, afxNat baseArtIdx)
{
    //float (*__cdecl GetPoseBuffer4x4(const world_pose *PoseBuffer, int BoneIndex))[4]
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    AfxAssertRange(wp->matCnt, baseArtIdx, 1);
    return &wp->world[baseArtIdx];
}

_AKX afxNat AfxGetPoseBufferCapacity(akxPoseBuffer const wp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    return wp->matCnt;
}

_AKX afxError _AkxPosbDtor(akxPoseBuffer posb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &posb, afxFcc_POSB);

    if (posb->composite)
        AfxDeallocate(posb->composite);

    if (posb->world)
        AfxDeallocate(posb->world);

    return err;
}

_AKX afxError _AkxPosbCtor(akxPoseBuffer posb, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &posb, afxFcc_POSB);

    afxSimulation sim = cookie->udd[0];
    afxNat artCnt = *(((afxNat const*)(cookie->udd[1])) + cookie->no);
    afxBool excludeComposite = cookie->udd[2] ? *(((afxBool const*)(cookie->udd[2])) + cookie->no) : TRUE;
    
    posb->matCnt = artCnt;
    posb->world = posb->matCnt ? AfxAllocate(posb->matCnt, sizeof(posb->world[0]), 0, AfxHere()) : NIL;
    posb->composite = (!excludeComposite) && posb->matCnt ? AfxAllocate(posb->matCnt, sizeof(posb->composite[0]), 0, AfxHere()) : NIL;

    return err;
}

_AKX afxClassConfig const _AkxPosbMgrCfg =
{
    .fcc = afxFcc_POSB,
    .name = "Skeletal Pose Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(akxPoseBuffer)),
    .ctor = (void*)_AkxPosbCtor,
    .dtor = (void*)_AkxPosbDtor
};

////////////////////////////////////////////////////////////////////////////////

_AKX afxError AfxAcquirePoseBuffers(afxSimulation sim, afxNat cnt, afxNat const artCnt[], afxBool const excludeComposite[], akxPoseBuffer wp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxClass *cls = AfxGetPoseBufferClass(sim);
    AfxAssertClass(cls, afxFcc_POSB);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)wp, (void const*[]) { sim, artCnt, excludeComposite }))
        AfxThrowError();

    return err;
}
