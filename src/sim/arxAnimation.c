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

#define _ARX_MOTION_C
#define _ARX_ANIMATION_C
//#define _ARX_SKELETON_C
//#define _ARX_BODY_C
//#define _ARX_MODEL_C
//#define _ARX_MOTOR_C
#include "impl/asxImplementation.h"

////////////////////////////////////////////////////////////////////////////////

_ARX afxBool ArxGetAnimationUrn(arxAnimation ani, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(id);
    *id = ani->urn;
    return ani->urn.len;
}

_ARX afxBool ArxFindMotion(arxAnimation ani, afxString const* id, afxUnit *motIdx)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(id);
    AFX_ASSERT(motIdx);
    afxBool found = FALSE;
    afxUnit motCnt = ani->motSlotCnt;

    for (afxUnit i = 0; i < motCnt; i++)
    {
        arxMotion mot = ani->motSlots[i].mot;

        if (!mot)
            continue;
        
        afxString s;

        if (!ArxGetMotionId(mot, &s))
            continue;
        
        if (AfxCompareStrings(&s, 0, TRUE, 1, id, NIL))
        {
            *motIdx = i;
            found = TRUE;
            break;
        }
    }
    return found;
}

_ARX afxError ArxGetMotions(arxAnimation ani, afxUnit first, afxUnit cnt, arxMotion motions[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT_RANGE(ani->motSlotCnt, first, cnt);

    first = AFX_MIN(first, ani->motSlotCnt);
    cnt = AFX_MIN(cnt, ani->motSlotCnt - first);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit slotIdx = first + i;
        AFX_ASSERT_RANGE(ani->motSlotCnt, slotIdx, 1);
        akxLinkedMotion* slot = &ani->motSlots[slotIdx];

        arxMotion mot = slot->mot;
        AFX_TRY_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);
        motions[i] = mot;
    }
    return err;
}

_ARX afxError ArxRelinkMotions(arxAnimation ani, afxUnit baseSlot, afxUnit slotCnt, arxMotion motions[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT_RANGE(ani->motSlotCnt, baseSlot, slotCnt);

    baseSlot = AFX_MIN(baseSlot, ani->motSlotCnt);
    slotCnt = AFX_MIN(slotCnt, ani->motSlotCnt - baseSlot);

    afxSimulation sim = AfxGetHost(ani);

    for (afxUnit i = 0; i < slotCnt; i++)
    {
        afxUnit slotIdx = baseSlot + i;
        AFX_ASSERT_RANGE(ani->motSlotCnt, slotIdx, 1);
        akxLinkedMotion* slot = &ani->motSlots[slotIdx];
        arxMotion motCurr = slot->mot;
        arxMotion mot = motions ? motions[i] : NIL;

        if (motCurr == mot)
            continue;

        if (motCurr) // detach operation
        {
            AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &motCurr);
            AfxDisposeObjects(1, &motCurr);
            slot->mot = NIL;
        }

        if (mot) // attach operation
        {
            AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);
            AfxReacquireObjects(1, &mot);
            slot->mot = mot;
        }
    }
    return err;
}

_ARX afxUnit ArxPerformAnimation(arxAnimation ani, afxReal startTime, afxUnit iterCnt, afxUnit cnt, arxBody bodies[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(bodies);
    afxUnit rslt = 0;
    
    afxUnit motCnt = ani->motSlotCnt;
    AFX_ASSERT(256 >= motCnt);
    arxTrackTarget targets[256];

    afxSimulation sim = AfxGetHost(ani);

    afxUnit foundCnt = 0;

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        arxBody bod = bodies[mdlIdx];
        if (!bod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
        
        arxModel mdl;
        if (!ArxGetBodyModel(bod, &mdl)) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        afxString s;
        arxMotion mot;
        afxUnit motIdx;
        ArxGetModelUrn(mdl, &s);

        if (!ArxFindMotion(ani, &s, &motIdx)) continue;
        ArxGetMotions(ani, motIdx, 1, &mot);
        AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);

        targets[foundCnt].Binding = NIL;
        targets[foundCnt].BindingID.Animation = ani;
        targets[foundCnt].BindingID.TrackPattern = "*";
        targets[foundCnt].BindingID.BonePattern = "*";
        targets[foundCnt].BindingID.motIdx = motIdx;
        targets[foundCnt].BindingID.FromBasis = 0;
        targets[foundCnt].BindingID.ToBasis = 0;
        targets[foundCnt].BindingID.OnModel = mdl;
        targets[foundCnt].OnInstance = bod;

        targets[foundCnt].LODCopyScaler = 1.0;
        targets[foundCnt].TrackMask = NIL;
        targets[foundCnt].ModelMask = NIL;

        afxMask flags = ArxGetMotionFlags(mot) & 0xFFFF;
        
        if (flags & 0x1)
            targets[foundCnt].AccumulationMode = arxAccumOp_CONST;
        else
            targets[foundCnt].AccumulationMode = (flags >> 1) & arxAccumOp_DELTA;

        ++foundCnt;
    }

    if (!foundCnt)
        return rslt;

    arxInstancedAnimation animInsts[256]; // one per body
    if (AfxAcquireObjects((afxClass*)_ArxGetInstancedAnimationClass(sim), foundCnt, (afxObject*)animInsts, (void const*[]) { sim, &targets[0] }))
    {
        AfxThrowError();
    }
    else
    {
        arxCapstan moto;
        arxCapstanConfig motoCfg = { 0 };
        motoCfg.currTime = startTime;
        motoCfg.localDur = ani->dur;
        motoCfg.iterCnt = iterCnt;

        if (ArxAcquireCapstans(sim, &motoCfg, 1, &moto))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

            for (afxUnit i = 0; i < foundCnt; i++)
                targets[i].Binding = animInsts[i];

            arxMotive motives[256]; // one per body
            if (AfxAcquireObjects((afxClass*)_ArxGetMotiveClass(sim), foundCnt, (afxObject*)motives, (void const*[]) { sim, NIL, moto, &targets[0] }))
            {
                AfxThrowError();
            }

            AfxDisposeObjects(1, &moto);
        }

        if (err)
            AfxDisposeObjects(foundCnt, animInsts);
    }
    return rslt;
}

_ARX afxUnit ArxPerformAnimationBinding(arxAnimation ani, afxReal startTime, afxUnit iterCnt, arxInstancedAnimation binding, afxUnit cnt, arxBody bodies[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(bodies);
    afxUnit rslt = 0;

    afxUnit motCnt = ani->motSlotCnt;
    AFX_ASSERT(256 >= motCnt);
    arxTrackTarget targets[256];

    afxSimulation sim = AfxGetHost(ani);

    afxUnit foundCnt = 0;

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        arxBody bod = bodies[mdlIdx];
        if (!bod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

        arxModel mdl;
        if (!ArxGetBodyModel(bod, &mdl)) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        afxString s;
        arxMotion mot;
        afxUnit motIdx;
        ArxGetModelUrn(mdl, &s);

        if (!ArxFindMotion(ani, &s, &motIdx)) continue;
        ArxGetMotions(ani, motIdx, 1, &mot);
        AFX_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);

        targets[foundCnt].Binding = binding;
        targets[foundCnt].BindingID.Animation = ani;
        targets[foundCnt].BindingID.TrackPattern = "*";
        targets[foundCnt].BindingID.BonePattern = "*";
        targets[foundCnt].BindingID.motIdx = motIdx;
        targets[foundCnt].BindingID.FromBasis = 0;
        targets[foundCnt].BindingID.ToBasis = 0;
        targets[foundCnt].BindingID.OnModel = mdl;
        targets[foundCnt].OnInstance = bod;

        targets[foundCnt].LODCopyScaler = 1.0;
        targets[foundCnt].TrackMask = NIL;
        targets[foundCnt].ModelMask = NIL;

        afxMask flags = ArxGetMotionFlags(mot) & 0xFFFF;

        if (flags & 0x1)
            targets[foundCnt].AccumulationMode = arxAccumOp_CONST;
        else
            targets[foundCnt].AccumulationMode = (flags >> 1) & arxAccumOp_DELTA;

        ++foundCnt;
    }

    if (!foundCnt)
        return rslt;

    arxCapstan moto;
    arxCapstanConfig motoCfg = { 0 };
    motoCfg.currTime = startTime;
    motoCfg.localDur = ani->dur;
    motoCfg.iterCnt = iterCnt;

    if (ArxAcquireCapstans(sim, &motoCfg, 1, &moto))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

        arxMotive motives[256]; // one per body
        if (AfxAcquireObjects((afxClass*)_ArxGetMotiveClass(sim), foundCnt, (afxObject*)motives, (void const*[]) { sim, NIL, moto, &targets[0] }))
        {
            AfxThrowError();
        }
        AfxDisposeObjects(1, &moto);
    }
    return rslt;
}

_ARX afxError _ArxAniDtorCb(arxAnimation ani)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);

    for (afxUnit i = ani->motSlotCnt; i-- > 0;)
    {
        akxLinkedMotion* alm = &ani->motSlots[i];

        if (alm->mot)
            AfxDisposeObjects(1, &alm->mot);
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = ani->motSlotCnt,
            .siz = sizeof(ani->motSlots[0]),
            .var = (void**)&ani->motSlots
        }
    };

    AfxDeallocateInstanceData(ani, ARRAY_SIZE(stashes), stashes);

    return err;
}

_ARX afxError _ArxAniCtorCb(arxAnimation ani, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    arxAnimationBlueprint const* anib = args[1];
    anib += invokeNo;

    if (!AfxCatalogStrings(_ArxRweGetAnimUrnStringBase(sim), 1, &anib->id.s, &ani->urn))
        AfxThrowError();

    ani->dur = anib->dur;
    ani->timeStep = anib->timeStep;
    ani->oversampling = anib->oversampling;

    afxUnit motCnt = anib->motSlotCnt;
    ani->motSlotCnt = motCnt;
    ani->motSlots = NIL;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = ani->motSlotCnt,
            .siz = sizeof(ani->motSlots[0]),
            .var = (void**)&ani->motSlots
        }
    };

    if (AfxAllocateInstanceData(ani, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    if (motCnt)
    {
        AfxZero(ani->motSlots, sizeof(ani->motSlots[0]) * motCnt);
        
        if (anib->motions && ArxRelinkMotions(ani, 0, motCnt, anib->motions))
            AfxThrowError();
    }
    AFX_ASSERT(ani->motSlotCnt == motCnt);

    return err;
}

_ARX afxClassConfig const _ARX_ANI_CLASS_CONFIG =
{
    .fcc = afxFcc_ANI,
    .name = "Animation",
    .desc = "Full Motion Animation", // or Multi-body Animation
    .fixedSiz = sizeof(AFX_OBJECT(arxAnimation)),
    .ctor = (void*)_ArxAniCtorCb,
    .dtor = (void*)_ArxAniDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAssembleAnimations(afxSimulation sim, afxUnit cnt, arxAnimationBlueprint const blueprints[], arxAnimation animations[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(blueprints);
    AFX_ASSERT(animations);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_ArxGetAnimationClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_ANI);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)animations, (void const*[]) { sim, blueprints }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_ANI, cnt, animations);

    return err;
}

_ARX void ArxTransformAnimations(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxUnit cnt, arxAnimation animations[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(animations);
    AfxThrowError();
    (void)affineTol;
    (void)linearTol;
    AfxThrowError();
}
