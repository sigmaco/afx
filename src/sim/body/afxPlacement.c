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

#define _AMX_PLACEMENT_C
#include "../impl/amxImplementation.h"

_AMX afxM4d* AfxPostureGetWorldMatrixArray(afxPlacement const plce)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    return plce->world;
}

_AMX afxM4d* AfxPostureGetCompositeMatrixArray(afxPlacement const plce)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    return plce->delta;
}

_AMX afxM4d* AfxGetPlacementDeltas(afxPlacement const plce, afxUnit baseArtIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, baseArtIdx, 1);
    return &plce->delta[baseArtIdx];
}

_AMX afxM4d* AfxGetPlacementMatrices(afxPlacement const plce, afxUnit baseArtIdx)
{
    //float (*__cdecl GetPosture4x4(const world_pose *Posture, int BoneIndex))[4]
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, baseArtIdx, 1);
    return &plce->world[baseArtIdx];
}

_AMX afxUnit AfxGetPlacementCapacity(afxPlacement const plce)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    return plce->matCnt;
}

_AMX afxError _AmxPosbDtorCb(afxPlacement plce)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);

    if (plce->delta)
        AfxDeallocate((void**)&plce->delta, AfxHere());

    if (plce->world)
        AfxDeallocate((void**)&plce->world, AfxHere());

    return err;
}

_AMX afxError _AmxPosbCtorCb(afxPlacement plce, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxUnit artCnt = *(((afxUnit const*)(args[1])) + invokeNo);
    afxBool excludeComposite = args[2] ? *(((afxBool const*)(args[2])) + invokeNo) : TRUE;
    
    plce->matCnt = artCnt;
    AfxAllocate(plce->matCnt * sizeof(plce->world[0]), 0, AfxHere(), (void**)&plce->world);
    plce->delta = NIL;
    
    if ((!excludeComposite) && plce->matCnt)
        AfxAllocate(plce->matCnt * sizeof(plce->delta[0]), 0, AfxHere(), (void**)&plce->delta);

    return err;
}

_AMX afxClassConfig const _AMX_PLCE_CLASS_CONFIG =
{
    .fcc = afxFcc_PLCE,
    .name = "Placement",
    .desc = "Skeletal Placement Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxPlacement)),
    .ctor = (void*)_AmxPosbCtorCb,
    .dtor = (void*)_AmxPosbDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquirePlacements(afxSimulation sim, afxUnit cnt, afxUnit const artCnt[], afxBool const excludeComposite[], afxPlacement placements[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_AmxGetPlacementClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_PLCE);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)placements, (void const*[]) { sim, artCnt, excludeComposite }))
        AfxThrowError();

    return err;
}
