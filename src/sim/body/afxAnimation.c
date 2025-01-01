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
//#define _AMX_SIMULATION_C
#define _AMX_MOTION_C
#define _AMX_ANIMATION_C
//#define _AMX_SKELETON_C
//#define _AMX_BODY_C
//#define _AMX_MODEL_C
//#define _AMX_MOTOR_C
#include "../impl/amxImplementation.h"

////////////////////////////////////////////////////////////////////////////////

_AMX afxBool AfxGetAnimationUrn(afxAnimation ani, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(id);
    *id = ani->urn;
    return ani->urn.len;
}

_AMX afxBool AfxFindMotion(afxAnimation ani, afxString const* id, afxUnit *motIdx)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(id);
    AFX_ASSERT(motIdx);
    afxBool found = FALSE;
    afxUnit motCnt = ani->motSlotCnt;

    for (afxUnit i = 0; i < motCnt; i++)
    {
        afxMotion mot = ani->motSlots[i].mot;

        if (!mot)
            continue;
        
        afxString s;

        if (!AfxGetMotionId(mot, &s))
            continue;
        
        if (0 == AfxCompareStrings(&s, 0, TRUE, 1, id))
        {
            *motIdx = i;
            found = TRUE;
            break;
        }
    }
    return found;
}

_AMX afxError AfxGetMotions(afxAnimation ani, afxUnit first, afxUnit cnt, afxMotion motions[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT_RANGE(ani->motSlotCnt, first, cnt);

    first = AfxMin(first, ani->motSlotCnt);
    cnt = AfxMin(cnt, ani->motSlotCnt - first);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit slotIdx = first + i;
        AFX_ASSERT_RANGE(ani->motSlotCnt, slotIdx, 1);
        akxLinkedMotion* slot = &ani->motSlots[slotIdx];

        afxMotion mot = slot->mot;
        AFX_TRY_ASSERT_OBJECTS(afxFcc_MOT, 1, &mot);
        motions[i] = mot;
    }
    return err;
}

_AMX afxError AfxRelinkMotions(afxAnimation ani, afxUnit baseSlot, afxUnit slotCnt, afxMotion motions[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT_RANGE(ani->motSlotCnt, baseSlot, slotCnt);

    baseSlot = AfxMin(baseSlot, ani->motSlotCnt);
    slotCnt = AfxMin(slotCnt, ani->motSlotCnt - baseSlot);

    afxSimulation sim = AfxGetProvider(ani);

    for (afxUnit i = 0; i < slotCnt; i++)
    {
        afxUnit slotIdx = baseSlot + i;
        AFX_ASSERT_RANGE(ani->motSlotCnt, slotIdx, 1);
        akxLinkedMotion* slot = &ani->motSlots[slotIdx];
        afxMotion motCurr = slot->mot;
        afxMotion mot = motions ? motions[i] : NIL;

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

_AMX afxUnit AfxPerformAnimation(afxAnimation ani, afxReal startTime, afxUnit iterCnt, afxUnit cnt, afxBody bodies[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(bodies);
    afxUnit rslt = 0;
    
    afxUnit motCnt = ani->motSlotCnt;
    AFX_ASSERT(256 >= motCnt);
    amxTrackTarget targets[256];

    afxSimulation sim = AfxGetProvider(ani);

    afxUnit foundCnt = 0;

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        afxBody bod = bodies[mdlIdx];

        if (!bod)
            continue;

        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
        
        afxModel mdl;
        if (!AfxGetBodyModel(bod, &mdl)) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        afxString s;
        afxMotion mot;
        afxUnit motIdx;
        AfxGetModelUrn(mdl, &s);

        if (!AfxFindMotion(ani, &s, &motIdx)) continue;
        AfxGetMotions(ani, motIdx, 1, &mot);
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

        afxMask flags = AfxGetMotionFlags(mot) & 0xFFFF;
        
        if (flags & 0x1)
            targets[foundCnt].AccumulationMode = afxAccumOp_CONST;
        else
            targets[foundCnt].AccumulationMode = (flags >> 1) & afxAccumOp_DELTA;

        ++foundCnt;
    }

    if (!foundCnt)
        return rslt;

    amxInstancedAnimation animInsts[256]; // one per body
    if (AfxAcquireObjects((afxClass*)_AmxGetInstancedAnimationClass(sim), foundCnt, (afxObject*)animInsts, (void const*[]) { sim, &targets[0] }))
    {
        AfxThrowError();
    }
    else
    {
        afxCapstan moto;
        afxCapstanConfig motoCfg = { 0 };
        motoCfg.currClock = startTime;
        motoCfg.localDur = ani->dur;
        motoCfg.iterCnt = iterCnt;

        if (AfxAcquireCapstans(sim, &motoCfg, 1, &moto))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

            for (afxUnit i = 0; i < foundCnt; i++)
                targets[i].Binding = animInsts[i];

            amxMotive motives[256]; // one per body
            if (AfxAcquireObjects((afxClass*)_AmxGetMotiveClass(sim), foundCnt, (afxObject*)motives, (void const*[]) { sim, NIL, moto, &targets[0] }))
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

_AMX afxUnit AfxPerformAnimationBinding(afxAnimation ani, afxReal startTime, afxUnit iterCnt, amxInstancedAnimation binding, afxUnit cnt, afxBody bodies[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(bodies);
    afxUnit rslt = 0;

    afxUnit motCnt = ani->motSlotCnt;
    AFX_ASSERT(256 >= motCnt);
    amxTrackTarget targets[256];

    afxSimulation sim = AfxGetProvider(ani);

    afxUnit foundCnt = 0;

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        afxBody bod = bodies[mdlIdx];

        if (!bod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

        afxModel mdl;
        if (!AfxGetBodyModel(bod, &mdl)) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        afxString s;
        afxMotion mot;
        afxUnit motIdx;
        AfxGetModelUrn(mdl, &s);

        if (!AfxFindMotion(ani, &s, &motIdx)) continue;
        AfxGetMotions(ani, motIdx, 1, &mot);
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

        afxMask flags = AfxGetMotionFlags(mot) & 0xFFFF;

        if (flags & 0x1)
            targets[foundCnt].AccumulationMode = afxAccumOp_CONST;
        else
            targets[foundCnt].AccumulationMode = (flags >> 1) & afxAccumOp_DELTA;

        ++foundCnt;
    }

    if (!foundCnt)
        return rslt;

    afxCapstan moto;
    afxCapstanConfig motoCfg = { 0 };
    motoCfg.currClock = startTime;
    motoCfg.localDur = ani->dur;
    motoCfg.iterCnt = iterCnt;

    if (AfxAcquireCapstans(sim, &motoCfg, 1, &moto))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

        amxMotive motives[256]; // one per body
        if (AfxAcquireObjects((afxClass*)_AmxGetMotiveClass(sim), foundCnt, (afxObject*)motives, (void const*[]) { sim, NIL, moto, &targets[0] }))
        {
            AfxThrowError();
        }
        AfxDisposeObjects(1, &moto);
    }
    return rslt;
}

_AMX afxError _AmxAniDtorCb(afxAnimation ani)
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

_AMX afxError _AmxAniCtorCb(afxAnimation ani, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxAnimationBlueprint const* anib = args[1];
    anib += invokeNo;

    if (!AfxCatalogStrings(AfxGetModelUrnStringBase(sim), 1, &anib->id.str, &ani->urn))
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
        AfxZero2(ani->motSlots, sizeof(ani->motSlots[0]), motCnt);
        
        if (anib->motions && AfxRelinkMotions(ani, 0, motCnt, anib->motions))
            AfxThrowError();
    }
    AFX_ASSERT(ani->motSlotCnt == motCnt);

    return err;
}

_AMX afxClassConfig const _AMX_ANI_CLASS_CONFIG =
{
    .fcc = afxFcc_ANI,
    .name = "Animation",
    .desc = "Fullscene Kinematic Animation",
    .fixedSiz = sizeof(AFX_OBJECT(afxAnimation)),
    .ctor = (void*)_AmxAniCtorCb,
    .dtor = (void*)_AmxAniDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAssembleAnimations(afxSimulation sim, afxUnit cnt, afxAnimationBlueprint const blueprints[], afxAnimation animations[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(blueprints);
    AFX_ASSERT(animations);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxGetAnimationClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_ANI);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)animations, (void const*[]) { sim, blueprints }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_ANI, cnt, animations);

    return err;
}

_AMX void AfxTransformAnimations(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxUnit cnt, afxAnimation animations[])
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
