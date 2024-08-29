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

#define _AMX_POSE_BUFFER_C
#include "../dev/AmxCadImplKit.h"

_AMX afxM4d* AfxPoseBufferGetWorldMatrixArray(afxPoseBuffer const wp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    return wp->world;
}

_AMX afxM4d* AfxPoseBufferGetCompositeMatrixArray(afxPoseBuffer const wp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    return wp->composite;
}

_AMX afxM4d* AfxGetCompositeMatrices(afxPoseBuffer const wp, afxNat baseArtIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    AfxAssertRange(wp->matCnt, baseArtIdx, 1);
    return &wp->composite[baseArtIdx];
}

_AMX afxM4d* AfxGetWorldMatrices(afxPoseBuffer const wp, afxNat baseArtIdx)
{
    //float (*__cdecl GetPoseBuffer4x4(const world_pose *PoseBuffer, int BoneIndex))[4]
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    AfxAssertRange(wp->matCnt, baseArtIdx, 1);
    return &wp->world[baseArtIdx];
}

_AMX afxNat AfxGetPoseBufferCapacity(afxPoseBuffer const wp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    return wp->matCnt;
}

_AMX afxError _AmxPosbDtorCb(afxPoseBuffer posb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &posb, afxFcc_POSB);

    if (posb->composite)
        AfxDeallocate(posb->composite);

    if (posb->world)
        AfxDeallocate(posb->world);

    return err;
}

_AMX afxError _AmxPosbCtorCb(afxPoseBuffer posb, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &posb, afxFcc_POSB);

    afxSimulation sim = args[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxNat artCnt = *(((afxNat const*)(args[1])) + invokeNo);
    afxBool excludeComposite = args[2] ? *(((afxBool const*)(args[2])) + invokeNo) : TRUE;
    
    posb->matCnt = artCnt;
    posb->world = posb->matCnt ? AfxAllocate(posb->matCnt, sizeof(posb->world[0]), 0, AfxHere()) : NIL;
    posb->composite = (!excludeComposite) && posb->matCnt ? AfxAllocate(posb->matCnt, sizeof(posb->composite[0]), 0, AfxHere()) : NIL;

    return err;
}

_AMX afxClassConfig const _AmxPosbClsCfg =
{
    .fcc = afxFcc_POSB,
    .name = "PoseBuffer",
    .desc = "Skeletal Pose Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxPoseBuffer)),
    .ctor = (void*)_AmxPosbCtorCb,
    .dtor = (void*)_AmxPosbDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquirePoseBuffers(afxSimulation sim, afxNat cnt, afxNat const artCnt[], afxBool const excludeComposite[], afxPoseBuffer wp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxClass* cls = (afxClass*)AfxGetPoseBufferClass(sim);
    AfxAssertClass(cls, afxFcc_POSB);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)wp, (void const*[]) { sim, artCnt, excludeComposite }))
        AfxThrowError();

    return err;
}
