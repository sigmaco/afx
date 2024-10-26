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
#define _AMX_SIMULATION_C
#define _AMX_MOTION_C
#define _AMX_ANIMATION_C
#define _AMX_SKELETON_C
#define _AMX_BODY_C
#define _AMX_MODEL_C
#define _AMX_MOTOR_C
#include "../dev/AmxImplKit.h"

AMXINL akxMotive* AfxMotivateBody(afxBody bod, afxMotor moto, akxMotiveCallbacks const *callbacks);

void CopyLODErrorValuesFromAnimation(struct animation_binding *AnimationBinding, afxReal ManualScaler)
{
    afxMotion v2 = AnimationBinding->ID.Animation->motSlots[AnimationBinding->ID.motIdx].mot;

    if (v2->pivotLodError)
    {
        for (afxUnit i = 0; i < AnimationBinding->TrackBindingCount; i++)
        {
            akxBoundMotionTransform *b = &AnimationBinding->TrackBindings[i];
            afxUnit c = b->srcTrackIdx;

            if (c != AFX_INVALID_INDEX)
            {
                afxMotionTransform const* v8 = b->srcTrack;
                b->lodErr = ManualScaler * v2->pivotLodError[c];
                Smt.SampleTrackUUULocalAtTime0(v8, &b->lodTransform);
            }
        }
    }
}

static afxBool TransformTrackHasKeyframedCurves(const afxMotionTransform *Track)
{
    return AfxIsCurveKeyframed(Track->translation)
        || AfxIsCurveKeyframed(Track->transmission)
        || AfxIsCurveKeyframed(Track->transmutation);
}

afxBool IsPlainWildcard(const char *Wildcard)
{
    return !Wildcard || *Wildcard == 42 && !Wildcard[1];
}

void ComputeTrackFlags(akxBoundMotionTransform *Track)
{
    akxBoundMotionTransform *v1; // esi
    const afxMotionTransform *v2; // eax
    char v3; // al
    char v4; // al
    char v5; // al
    const afxMotionTransform *v6; // eax
    const afxMotionTransform *v7; // ST00_4

    v1 = Track;
    v2 = Track->srcTrack;
    v1->flags = 0;

    if (!v2)
    {
        AfxResetTransform(&v1->lodTransform);
        return;
    }

    if (AfxIsCurveIdentity(v2->translation))
    {
        v3 = v1->flags & 0xFC;
    }
    else
    {
        if (AfxIsCurveConstantNotIdentity(v1->srcTrack->translation))
        {
            v1->flags = v1->flags & 0xFD | akxBoundMotionTransformFlag_TranslationIsConstant;
            goto LABEL_11;
        }

        if (AfxIsCurveKeyframed(v1->srcTrack->translation))
            v3 = v1->flags & 0xFE | akxBoundMotionTransformFlag_TranslationIsKeyframed;
        else
            v3 = v1->flags | akxBoundMotionTransformFlag_TranslationIsGeneral;
    }

    v1->flags = v3;

LABEL_11:
    if (AfxIsCurveIdentity(v1->srcTrack->transmission))
    {
        v4 = v1->flags & 0xF3;
    }
    else if (AfxIsCurveConstantNotIdentity(v1->srcTrack->transmission))
    {
        v4 = v1->flags & 0xF7 | akxBoundMotionTransformFlag_TransmissionIsConstant;
    }
    else
    {
        if (AfxIsCurveKeyframed(v1->srcTrack->transmission))
        {
            v1->flags = v1->flags & 0xFB | akxBoundMotionTransformFlag_TransmissionIsKeyframed;
            goto LABEL_19;
        }
        v4 = v1->flags | 0xC;
    }

    v1->flags = v4;

LABEL_19:
    if (AfxIsCurveIdentity(v1->srcTrack->transmutation))
    {
        v5 = v1->flags & 0xCF;
    }
    else
    {
        if (AfxIsCurveConstantNotIdentity(v1->srcTrack->transmutation))
        {
            v6 = v1->srcTrack;
            v1->flags = v1->flags & 0xDF | akxBoundMotionTransformFlag_TransmutationIsConstant;
            Smt.SampleTrackUUULocalAtTime0(v6, &v1->lodTransform);
            return;
        }

        if (AfxIsCurveKeyframed(v1->srcTrack->transmutation))
        {
            v7 = v1->srcTrack;
            v1->flags = v1->flags & 0xEF | akxBoundMotionTransformFlag_TransmutationIsKeyframed;
            Smt.SampleTrackUUULocalAtTime0(v7, &v1->lodTransform);
            return;
        }
        v5 = v1->flags | akxBoundMotionTransformFlag_TransmutationMask;
    }
    v1->flags = v5;
    Smt.SampleTrackUUULocalAtTime0(v1->srcTrack, &v1->lodTransform);
}

struct animation_binding *NewAnimationBinding(struct animation_binding_identifier *ID)
{
    afxError err;
    afxSkeleton skl;
    AfxGetModelSkeleton(ID->OnModel, &skl);
    afxUnit jointCnt = AfxCountSkeletonJoints(skl, 0);
    afxTransform* locals = AfxGetSkeletonTransform(skl, 0);

    struct animation_binding *Binding = AfxAllocate(1, sizeof(Binding[0]), 0, AfxHere());
    Binding->ID = *ID;
    Binding->UsedBy = 0;
    Binding->TrackBindingCount = jointCnt;
    Binding->TrackBindings = AfxAllocate(jointCnt, sizeof(Binding->TrackBindings[0]), 0, AfxHere());

    afxAnimation ani = ID->Animation;
    afxMotion mot = ani->motSlots[Binding->ID.motIdx].mot;

    afxBool KeyframedTracks = 0;

    for (afxUnit jointIdx = 0; jointIdx < jointCnt; jointIdx++)
    {
        afxString s = *AfxGetSkeletonJoints(skl, jointIdx);
        akxBoundMotionTransform* bound = &Binding->TrackBindings[jointIdx];
        afxUnit mtIdx;

        if (AfxFindMotionTransform(mot, &s, &mtIdx))
        {
            afxMotionTransform const* mt = &mot->pivotCurve[mtIdx];

            bound->srcTrackIdx = mtIdx;

            *(afxInt*)&bound->lodErr = 2131755007;
            bound->srcTrack = mt;
            ComputeTrackFlags(bound);

            if (AfxIsCurveConstantOrIdentity(mt->translation) && AfxIsCurveConstantOrIdentity(mt->transmission) && AfxIsCurveConstantOrIdentity(mt->transmutation))
            {
                bound->lodErr = 0;
            }

            bound->sampler = (void*)Smt.GetTrackSamplerFor(mt);
            //bound->sampler = (void*)Smt.SampleTrackUUU; // <-----------------

            if (TransformTrackHasKeyframedCurves(mt))
            {
                KeyframedTracks = 1;
            }
            else
            {
                afxQuat FirstQuaternion, LastQuaternion;

                if (AfxIsCurveIdentity(mt->transmission))
                {
                    AfxQuatReset(FirstQuaternion);
                    AfxQuatReset(LastQuaternion);
                }
                else
                {
                    AfxExtractCurveKnotValue(mt->transmission, 0, FirstQuaternion, AFX_QUAT_IDENTITY);
                    AfxExtractCurveKnotValue(mt->transmission, AfxCountCurveKnots(mt->transmission) - 1, LastQuaternion, AFX_QUAT_IDENTITY);
                }

                if (mt->flags & 1)
                    bound->quatMode = afxQuatBlend_ACCUM_ADJACENT;
                else if (AfxQuatDot(LastQuaternion, FirstQuaternion) >= 0.0)
                    bound->quatMode = (AfxQuatDot(FirstQuaternion, locals[jointIdx].orientation) < 0.0) ? afxQuatBlend_INV : afxQuatBlend_DIRECT;
                else
                    bound->quatMode = afxQuatBlend_ADJACENT;
            }
        }
        else
        {
            bound->sampler = (void*)Smt.SampleTrackSSS;
            bound->srcTrackIdx = AFX_INVALID_INDEX;
            bound->srcTrack = NIL;
            bound->quatMode = afxQuatBlend_DIRECT;
            *(afxInt*)&bound->lodErr = 2131755007;
            ComputeTrackFlags(bound);
        }
    }

    if (KeyframedTracks)
    {
        AfxThrowError();
    }
    return Binding;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxBool AfxGetAnimationUrn(afxAnimation ani, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ani, afxFcc_ANI);
    AFX_ASSERT(id);
    *id = ani->urn;
    return ani->urn.len;
}

_AMX afxBool AfxFindMotion(afxAnimation ani, afxString const* id, afxUnit *motIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ani, afxFcc_ANI);
    AFX_ASSERT(id);
    AFX_ASSERT(motIdx);
    afxBool found = FALSE;
    afxUnit motCnt = ani->motSlotCnt;

    for (afxUnit i = 0; i < motCnt; i++)
    {
        afxMotion mot = ani->motSlots[i].mot;

        if (mot)
        {
            afxString s;

            if (AfxGetMotionId(mot, &s))
            {
                if (0 == AfxCompareStrings(&s, TRUE, 1, id))
                {
                    *motIdx = i;
                    found = TRUE;
                    break;
                }
            }
        }
    }
    return found;
}

_AMX afxError AkxRelinkMotions(afxAnimation ani, afxUnit baseSlot, afxUnit slotCnt, afxMotion motions[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ani, afxFcc_ANI);
    AFX_ASSERT_RANGE(ani->motSlotCnt, baseSlot, slotCnt);

    afxSimulation sim = AfxGetProvider(ani);

    for (afxUnit i = 0; i < slotCnt; i++)
    {
        afxUnit slotIdx = baseSlot + i;
        AFX_ASSERT_RANGE(ani->motSlotCnt, slotIdx, 1);
        akxLinkedMotion* slot = &ani->motSlots[slotIdx];
        afxMotion motCurr = slot->mot;
        afxMotion mot = motions ? motions[i] : NIL;

        if (motCurr != mot)
        {
            if (mot) // attach operation
            {
                AfxAssertObjects(1, &mot, afxFcc_MOT);
                
                AfxReacquireObjects(1, &mot);
            }

            if (!err)
            {
                if (motCurr) // detach operation
                {
                    AfxAssertObjects(1, &motCurr, afxFcc_MOT);
                    AfxReleaseObjects(1, &motCurr);
                    slot->mot = NIL;
                }

                if (mot)
                {
                    slot->mot = mot;
                }
            }
        }
    }
    return err;
}

_AMX afxUnit AfxPerformAnimation(afxAnimation ani, afxReal startTime, afxUnit iterCnt, afxUnit cnt, afxBody bodies[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &ani, afxFcc_ANI);
    AFX_ASSERT(bodies);
    afxUnit rslt = 0;
    
    afxUnit motCnt = ani->motSlotCnt;
    AFX_ASSERT(256 >= motCnt);
    struct track_target targets[256];

    afxSimulation sim = AfxGetProvider(ani);

    afxUnit foundCnt = 0;

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        afxBody bod = bodies[mdlIdx];
        AfxAssertObjects(1, &bod, afxFcc_BOD);
        afxModel mdl;

        if (AfxGetBodyModel(bod, &mdl))
        {
            AfxAssertObjects(1, &mdl, afxFcc_MDL);

            afxString s;
            afxUnit motIdx;
            AfxGetModelUrn(mdl, &s);

            if (AfxFindMotion(ani, &s, &motIdx))
            {
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

                afxMask flags = ani->motSlots[motIdx].mot->flags & 0xFFFF;

                if (flags & 1)
                    targets[foundCnt].AccumulationMode = akxAccumulateOp_CONST;
                else
                    targets[foundCnt].AccumulationMode = (flags >> 1) & akxAccumulateOp_DELTA;

                ++foundCnt;
            }
        }
    }

    if (foundCnt)
    {
        afxMotor moto;

        if (AfxAcquireMotors(sim, startTime, ani->dur, iterCnt, 1, &moto)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &moto, afxFcc_MOTO);

            for (afxUnit i = 0; i < foundCnt; i++)
            {
                AFX_ASSERT(targets[i].OnInstance);
                akxMotive* intk = AfxMotivateBody(targets[i].OnInstance, moto, NIL);

                if (!intk) AfxThrowError();
                else
                {
                    struct animation_binding* v2 = NewAnimationBinding(&targets[i].BindingID);

                    intk->ca.Binding = v2;
                    intk->ca.AccumulationMode = targets[i].AccumulationMode;
                    intk->ca.TrackMask = targets[i].TrackMask;
                    intk->ca.ModelMask = targets[i].ModelMask;
                    intk->reservedPtr = (void *)1;

                    if (intk->ca.Binding)
                        CopyLODErrorValuesFromAnimation(intk->ca.Binding, targets[i].LODCopyScaler);

                    ++rslt;
                }
            }
            AfxReleaseObjects(1, &moto);
        }
    }
    return rslt;
}

_AMX afxUnit AfxPerformAnimationBinding(afxAnimation ani, afxReal startTime, afxUnit iterCnt, struct animation_binding *binding, afxUnit cnt, afxBody bodies[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &ani, afxFcc_ANI);
    AFX_ASSERT(bodies);
    afxUnit rslt = 0;

    afxUnit motCnt = ani->motSlotCnt;
    AFX_ASSERT(256 >= motCnt);
    struct track_target targets[256];

    afxSimulation sim = AfxGetProvider(ani);

    afxUnit foundCnt = 0;

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        afxBody bod = bodies[mdlIdx];
        AfxAssertObjects(1, &bod, afxFcc_BOD);
        afxModel mdl;

        if (AfxGetBodyModel(bod, &mdl))
        {
            AfxAssertObjects(1, &mdl, afxFcc_MDL);

            afxString s;
            afxUnit motIdx;
            AfxGetModelUrn(mdl, &s);

            if (AfxFindMotion(ani, &s, &motIdx))
            {
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

                afxMask flags = ani->motSlots[motIdx].mot->flags & 0xFFFF;

                if (flags & 1)
                    targets[foundCnt].AccumulationMode = akxAccumulateOp_CONST;
                else
                    targets[foundCnt].AccumulationMode = (flags >> 1) & akxAccumulateOp_DELTA;

                ++foundCnt;
            }
        }
    }

    if (foundCnt)
    {
        afxMotor moto;

        if (AfxAcquireMotors(sim, startTime, ani->dur, iterCnt, 1, &moto)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &moto, afxFcc_MOTO);

            for (afxUnit i = 0; i < foundCnt; i++)
            {
                AFX_ASSERT(targets[i].OnInstance);
                akxMotive* intk = AfxMotivateBody(targets[i].OnInstance, moto, NIL);

                if (!intk) AfxThrowError();
                else
                {
                    struct animation_binding* v2 = targets[i].Binding;

                    intk->ca.Binding = v2;
                    intk->ca.AccumulationMode = targets[i].AccumulationMode;
                    intk->ca.TrackMask = targets[i].TrackMask;
                    intk->ca.ModelMask = targets[i].ModelMask;
                    intk->reservedPtr = (void *)1;

                    if (intk->ca.Binding)
                        CopyLODErrorValuesFromAnimation(intk->ca.Binding, targets[i].LODCopyScaler);

                    ++rslt;
                }
            }
            AfxReleaseObjects(1, &moto);
        }
    }
    return rslt;
}

_AMX afxError _AmxAniCtorCb(afxAnimation ani, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ani, afxFcc_ANI);

    afxSimulation sim = args[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxAnimationBlueprint const* anib = args[1];
    anib += invokeNo;

    if (!AfxCatalogStrings(AfxGetModelUrnStringBase(sim), 1, &anib->id.str, &ani->urn))
        AfxThrowError();

    ani->dur = anib->dur;
    ani->timeStep = anib->timeStep;
    ani->oversampling = anib->oversampling;

    ani->motSlotCnt = 0;
    ani->motSlots = NIL;

    afxUnit motCnt = anib->motSlotCnt;

    if (motCnt && !(ani->motSlots = AfxAllocate(motCnt, sizeof(ani->motSlots[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        if (motCnt)
        {
            ani->motSlotCnt = motCnt;

            AfxZero2(ani->motSlots, sizeof(ani->motSlots[0]), motCnt);

            if (AkxRelinkMotions(ani, 0, motCnt, anib->motions))
                AfxThrowError();
        }
        AFX_ASSERT(ani->motSlotCnt == motCnt);
    }

    return err;
}

_AMX afxError _AmxAniDtorCb(afxAnimation ani)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ani, afxFcc_ANI);

    for (afxUnit i = ani->motSlotCnt; i--> 0;)
    {
        akxLinkedMotion* alm = &ani->motSlots[i];

        if (alm->mot)
            AfxReleaseObjects(1, &alm->mot);
    }

    if (ani->motSlots)
        AfxDeallocate(ani->motSlots);

    return err;
}

_AMX afxClassConfig _AmxAniMgrCfg =
{
    .fcc = afxFcc_ANI,
    .name = "Animation",
    .desc = "Animation",
    .fixedSiz = sizeof(AFX_OBJECT(afxAnimation)),
    .ctor = (void*)_AmxAniCtorCb,
    .dtor = (void*)_AmxAniDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AkxAssembleAnimations(afxSimulation sim, afxUnit cnt, afxAnimationBlueprint const blueprints[], afxAnimation animations[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AFX_ASSERT(blueprints);
    AFX_ASSERT(animations);
    AFX_ASSERT(cnt);

    if (AfxAcquireObjects(&sim->aniMgr, cnt, (afxObject*)animations, (void const*[]) { sim, blueprints }))
        AfxThrowError();

    return err;
}

_AMX void AkxTransformAnimations(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxUnit cnt, afxAnimation ani[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(ani);
    AfxThrowError();
    (void)affineTol;
    (void)linearTol;
    AfxThrowError();
}
