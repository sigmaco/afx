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
#define _AKX_SIMULATION_C
#define _AKX_MOTION_C
#define _AKX_ANIMATION_C
#define _AKX_SKELETON_C
#define _AKX_BODY_C
#define _AKX_MODEL_C
#define _AKX_MOTOR_C
#include "../sim/dev/AkxSimDevKit.h"


struct animation_binding *NewAnimationBinding(struct animation_binding_identifier *ID);

#if 0
struct model_control_callbacks ControlledAnimationCallbacks =
{
  &PoseGetControlledPose,
  &CurveGetKnotCountDaIdentity,
  &AnimationInitializeBindingState,
  &AnimationAccumulateBindingState,
  &AnimationBuildDirect,
  &AnimationAccumulateLoopTransform,
  &AnimationCleanupBindingState
}; // idb
struct model_control_callbacks ControlledPoseCallbacks =
{
  &CurveGetKnotCountDaIdentity,
  &PoseGetControlledPose,
  &PoseInitializeBindingState,
  &PoseAccumulateBindingState,
  &PoseBuildDirect,
  NULL,
  NULL
}; // idb
#endif

_AKXINL void AfxComputeMotionDisplacement(akxMotion mot, afxM4d m)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AfxAssert(m);
    AfxComputeTransformM4d(&mot->displacement, m);
}

_AKXINL afxBool AfxFindMotionTransform(akxMotion mot, afxString const* seqId, afxNat *seqIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AfxAssert(seqId);
    AfxAssert(seqIdx);
    afxBool found = FALSE;
    afxNat pivotCnt = mot->pivotCnt;

    afxStringBase strb = AfxGetPivotTagStringBase(AfxGetObjectProvider(mot));

    for (afxNat i = 0; i < pivotCnt; i++)
    {
        afxString s = mot->pivots[i];

        if (0 == AfxCompareStringCi(seqId, &s))
        {
            *seqIdx = i;
            found = TRUE;
            break;
        }
    }

    if (!found)
        *seqIdx = AFX_INVALID_INDEX;

    return found;
}

_AKXINL afxBool AfxFindMotionVector(akxMotion mot, afxString const* seqId, afxNat *seqIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AfxAssert(seqId);
    AfxAssert(seqIdx);
    afxBool rslt = FALSE;
    afxNat vecCnt = mot->vecCnt;

    afxStringBase strb = AfxGetMorphTagStringBase(AfxGetObjectProvider(mot));

    for (afxNat i = 0; i < vecCnt; i++)
    {
        afxString s = mot->vectors[i];

        if (s.len && (0 == AfxCompareStringCi(seqId, &s)))
        {
            *seqIdx = i;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AKX void AfxUpdateMotionVectors(akxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, akxMotionVector const vectors[], afxNat fetchRate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AfxAssertRange(mot->vecCnt, baseSeqIdx, seqCnt);

    for (afxNat i = 0; i < seqCnt; i++)
    {
        afxNat seqIdx = baseSeqIdx + i;
        akxMotionVector const* src = &vectors[i];
        akxMotionVector* t = &mot->vecCurve[seqIdx];
        *t = *src;
    }
}

_AKX void AfxUpdateMotionTransforms(akxMotion mot, afxNat baseSeqIdx, afxNat seqCnt, akxMotionTransform const transforms[], afxNat fetchRate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AfxAssertRange(mot->pivotCnt, baseSeqIdx, seqCnt);

    for (afxNat i = 0; i < seqCnt; i++)
    {
        afxNat seqIdx = baseSeqIdx + i;
        akxMotionTransform const* src = &transforms[i];
        akxMotionTransform* t = &mot->pivotCurve[seqIdx];
        *t = *src;
    }
}

_AKX afxBool AfxGetMotionId(akxMotion mot, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AfxAssert(id);
    *id = mot->id;
    return mot->id.len;
}

_AKX afxError _AkxMotCtor(akxMotion mot, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOT);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    akxMotionBlueprint const* motb = cookie->udd[1];
    motb += cookie->no;

    if (!AfxRegisterModelUrns(sim, 1, &motb->id.str, &mot->id))
        AfxThrowError();

    afxNat vecCnt = motb->vecCnt;
    afxNat pivotCnt = motb->pivotCnt;

    mot->vecCurve = vecCnt ? AfxAllocate(vecCnt, sizeof(mot->vecCurve[0]), 0, AfxHere()) : NIL;
    mot->vectors = vecCnt ? AfxAllocate(vecCnt, sizeof(mot->vectors[0]), 0, AfxHere()) : NIL;
    mot->pivotCurve = pivotCnt ? AfxAllocate(pivotCnt, sizeof(mot->pivotCurve[0]), 0, AfxHere()) : NIL;
    mot->pivots = pivotCnt ? AfxAllocate(pivotCnt, sizeof(mot->pivots[0]), 0, AfxHere()) : NIL;
    mot->pivotLodError = motb->incPivotLodError && pivotCnt ? AfxAllocate(pivotCnt, sizeof(mot->pivotLodError[0]), 0, AfxHere()) : NIL;

    afxString s;

    if (!mot->vectors)
    {
        for (afxNat i = 0; i < vecCnt; i++)
            AfxResetString(&mot->vectors[i]);
    }
    else if (!AfxRegisterMorphTags(sim, vecCnt, motb->vectors, mot->vectors)) // AfxRegisterMorphTags is temp
        AfxThrowError();

    if ((mot->vecCnt = vecCnt))
    {
        for (afxNat i = 0; i < vecCnt; i++)
        {
            akxMotionVector* t = &mot->vecCurve[i];
        }

        afxString id;
        afxCurve value;
        afxNat key;
        afxInt dim;
        //AfxUpdateMotionVectors(mot, 0, vecCnt, &id, &key, &dim, &value, 0);
    }

    if (!mot->pivots)
    {
        for (afxNat i = 0; i < pivotCnt; i++)
            AfxResetString(&mot->pivots[i]);
    }
    else if (!AfxRegisterPivotTags(sim, pivotCnt, motb->pivots, mot->pivots))
        AfxThrowError();

    if ((mot->pivotCnt = pivotCnt))
    {
        for (afxNat i = 0; i < pivotCnt; i++)
        {
            akxMotionTransform* t = &mot->pivotCurve[i];
            
        }

        afxString id;
        afxFlags flag;
        afxCurve orient, scale, pos;
        //AfxUpdateMotionTransforms(mot, 0, xformMotCnt, &id, &flag, &orient, &pos, &scale, 0);
    }

    if (mot->pivotLodError)
    {
        if (motb->pivotLodError) for (afxNat i = 0; i < pivotCnt; i++)
            mot->pivotLodError[i] = motb->pivotLodError[i];
        else for (afxNat i = 0; i < pivotCnt; i++)
            mot->pivotLodError[i] = 1.9140881e38;
    }

    AfxCopyTransform(&mot->displacement, &motb->displacement);

    mot->flags = 2;
    mot->periodicLoop = 0;
    mot->root = NIL;
    AfxV3dZero(mot->loopTranslation);

#if !0
    // echo

    AfxGetMotionId(mot, &s);
    AfxLogEcho("Motion <%.*s> assembled. %p\n    Listing %u transform circuits:", AfxPushString(&s), mot, mot->pivotCnt);

    for (afxNat i = 0; i < mot->pivotCnt; i++)
    {
        s = mot->pivots[i];
        AfxLogEcho("\t%u <%.*s> %x", i, AfxPushString(&s), mot->pivotCurve[i].flags);
    }
#endif

    return err;
}

_AKX afxError _AkxMotDtor(akxMotion mot)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mot, afxFcc_MOT);

    if (mot->pivots)
        AfxDeallocate(mot->pivots);

    if (mot->pivotLodError)
        AfxDeallocate(mot->pivotLodError);

    if (mot->pivotCurve)
    {
        for (afxNat i = 0; i < mot->pivotCnt; i++)
        {
            akxMotionTransform* t = &mot->pivotCurve[i];
            afxCurve curves[3] = { t->translation, t->transmission, t->transmutation};
            AfxReleaseObjects(3, curves);
        }
        AfxDeallocate(mot->pivotCurve);
    }

    if (mot->vectors)
        AfxDeallocate(mot->vectors);

    if (mot->vecCurve)
    {
        for (afxNat i = 0; i < mot->vecCnt; i++)
        {
            akxMotionVector* v = &mot->vecCurve[i];
            AfxReleaseObjects(1, &v->value);
        }
        AfxDeallocate(mot->vecCurve);
    }

    return err;
}

_AKX afxClassConfig _AkxMotMgrCfg =
{
    .fcc = afxFcc_MOT,
    .name = "Motion",
    .desc = "Animated Motion",
    .fixedSiz = sizeof(AFX_OBJECT(akxMotion)),
    .ctor = (void*)_AkxMotCtor,
    .dtor = (void*)_AkxMotDtor
};

////////////////////////////////////////////////////////////////////////////////

_AKX afxError AfxAssembleMotions(afxSimulation sim, afxNat cnt, akxMotionBlueprint const blueprints[], akxMotion motions[])
{
    afxError err = AFX_ERR_NONE;
    
    if (AfxAcquireObjects(&sim->motMgr, 1, (afxObject*)motions, (void const*[]) { sim, blueprints }))
        AfxThrowError();

    return err;
}





afxReal GetTrackMaskBoneWeight(const akxTrackMask *Mask, afxNat BoneIndex)
{
    double result; // st7

    if (BoneIndex >= Mask->boneCnt)
        result = Mask->defWeight;
    else
        result = Mask->boneWeights[BoneIndex];

    return result;
}

void AnimationAccumulateBindingState(akxMotive* Binding, afxNat FirstBone, afxNat BoneCount, akxPose Result, afxReal AllowedError, const afxNat *SparseBoneArray)
{
    afxSimulation sim = AfxGetObjectProvider(Binding->moto);

    afxReal AllowedErrorEnd, AllowedErrorScaler;
    AfxQuerySimulationErrorTolerance(sim, AllowedError, &AllowedErrorEnd, &AllowedErrorScaler);

    akxMotor moto = Binding->moto;
    afxReal ControlWeight = AkxGetMotorEffectiveWeight(moto);

    if (ControlWeight > 0.001)
    {
        struct sample_context Context;
        Context.LocalClock = AkxGetMotorClampedLocalClock(moto);
        Context.FrameIndex = (signed int)Context.LocalClock / Binding->ca.Binding->ID.Animation->dur;
        Context.LocalDuration = AkxGetMotorLocalDuration(moto);
        AkxQueryMotorIterationState(moto, &Context.UnderflowLoop, &Context.OverflowLoop);
        
        for (afxNat i = FirstBone; i < FirstBone + BoneCount; i++)
        {
            afxNat SkeletonBoneIndex = i;

            if (SparseBoneArray)
                SkeletonBoneIndex = SparseBoneArray[i];

            //struct animation_binding* v12 = Binding->ca.Binding->TrackBindings;
            akxBoundMotionTransform* v14 = &Binding->ca.Binding->TrackBindings[SkeletonBoneIndex];
            akxMotionTransform const* v13 = v14->srcTrack;
            
            if (v13)
            {
                akxTrackMask const* v15 = (const akxTrackMask *)Binding->ca.TrackMask;
                afxReal Weight = ControlWeight;

                if (v15)
                    Weight = GetTrackMaskBoneWeight(v15, v14->srcTrackIdx) * ControlWeight;

                akxTrackMask const* v16 = (const akxTrackMask *)Binding->ca.ModelMask;

                if (v16)
                    Weight = GetTrackMaskBoneWeight(v16, SkeletonBoneIndex) * Weight;

                if (Weight > 0.001)
                {
                    afxTransform SampledTransform;

                    if (AllowedErrorEnd < (double)v14->lodErr)
                    {
                        if (AllowedError < (double)v14->lodErr)
                        {
                            SampledTransform.flags = 0;
                            Smt.SampleTrackUUULocal(&Context, v13, v14, &SampledTransform);
                        }
                        else
                        {
                            afxReal BlendFactor = (AllowedError - v14->lodErr) * AllowedErrorScaler;
                            Smt.SampleTrackUUULocal(&Context, v13, v14, &SampledTransform);
                            AfxLerpTransform(&SampledTransform, &SampledTransform, &v14->lodTransform, BlendFactor);
                        }
                    }
                    else
                    {
                        AfxCopyTransform(&SampledTransform, &v14->lodTransform);
                    }

                    if (Binding->ca.AccumulationMode == 2 && !SkeletonBoneIndex)
                    {
                        SampledTransform.position[2] = 0.0;
                        SampledTransform.position[1] = 0.0;
                        SampledTransform.position[0] = 0.0;
                        SampledTransform.orientation[0] = 0.0;
                        SampledTransform.orientation[1] = 0.0;
                        SampledTransform.orientation[2] = 0.0;
                        SampledTransform.orientation[3] = 1.0;
                    }
                    
                    afxSkeleton skl;
                    AfxGetBodySkeleton(AfxGetLinker(&Binding->bod), &skl);
                    AfxAccumulateLocalTransform(Result, i, SkeletonBoneIndex, Weight, skl, v14->quatMode, &SampledTransform);
                }
            }
        }
    }
}

void _SetUpSampleContext(sample_context* ctx, akxMotor moto)
{
    ctx->LocalClock = AkxGetMotorClampedLocalClock(moto);
    ctx->LocalDuration = AkxGetMotorLocalDuration(moto);;
    ctx->FrameIndex = 0;//Binding->ca.Binding->ID.Animation->dur / ctx->LocalClock;
    AkxQueryMotorIterationState(moto, &ctx->UnderflowLoop, &ctx->OverflowLoop);
}

void AnimationBuildDirect(akxMotive* Binding, afxNat pivotCnt, afxM4d const offset, akxPoseBuffer Result, afxReal allowedErr)
{
    struct animation_binding* cab = Binding->ca.Binding;
    akxBody bod = AfxGetLinker(&Binding->bod);
    afxSkeleton skl = AfxGetModelSkeleton(bod->mdl);

    afxReal allowedErrEnd, allowedErrScaler;
    AfxQuerySkeletonErrorTolerance(skl, allowedErr, &allowedErrEnd, &allowedErrScaler);

    sample_context Context;
    _SetUpSampleContext(&Context, Binding->moto);

    afxM4d* w = AfxGetWorldMatrices(Result, 0);
    afxM4d* composite = AfxGetCompositeMatrices(Result, 0);
    afxM4d const* iw = skl->iw;

    if (pivotCnt > cab->TrackBindingCount)
        pivotCnt = cab->TrackBindingCount;

    akxTrackMask const* trackMask = Binding->ca.TrackMask;
    akxTrackMask const* modelMask = Binding->ca.ModelMask;

    for (afxNat pivotIdx = 0; pivotIdx < pivotCnt; pivotIdx++)
    {
        akxBoundMotionTransform const* bound = &cab->TrackBindings[pivotIdx];
        akxMotionTransform const* mt = bound->srcTrack;
        afxTransform const* local = &skl->local[pivotIdx];
        afxNat parentIdx = skl->parentIdx[pivotIdx];
        afxV4d const* parent;

        if (parentIdx == AFX_INVALID_INDEX)
            parent = offset;
        else
            parent = w[parentIdx];

        if (pivotIdx == 0 && Binding->ca.AccumulationMode == 2 && mt)
        {
            Smt.SampleTrackIIU(&Context, mt, bound, local, iw[pivotIdx], parent, w[pivotIdx], composite ? composite[pivotIdx] : NIL);
        }
        else
        {
            if (trackMask && (GetTrackMaskBoneWeight(trackMask, bound->srcTrackIdx) < 0.001))
            {
                if (!composite)
                    Mmt.BuildFullPoseBufferOnly_Generic(local, parent, w[pivotIdx]);
                else
                    Mmt.BuildFullPoseBufferComposite_Generic(local, parent, iw[pivotIdx], composite ? composite[pivotIdx] : NIL, w[pivotIdx]);
            }
            else
            {
                if ((!modelMask) || GetTrackMaskBoneWeight(modelMask, pivotIdx) >= 0.001)
                {
                    afxReal lodErr = bound->lodErr;

                    if (allowedErrEnd < lodErr)
                    {
                        if (allowedErr < lodErr)
                        {
                            bound->sampler(&Context, mt, bound, local, iw[pivotIdx], parent, w[pivotIdx], composite ? composite[pivotIdx] : NIL);
                        }
                        else
                        {
                            afxReal blendFactor = (allowedErr - lodErr) * allowedErrScaler;
                            Smt.SampleTrackUUUBlendWithTime0(&Context, mt, bound, local, iw[pivotIdx], parent, w[pivotIdx], composite ? composite[pivotIdx] : NIL, blendFactor);
                        }
                    }
                    else
                    {
                        Smt.SampleTrackUUUAtTime0(&Context, mt, bound, local, iw[pivotIdx], parent, w[pivotIdx], composite ? composite[pivotIdx] : NIL);
                    }
                }
            }
        }
    }
}

void CopyLODErrorValuesToAnimation(struct animation_binding *AnimationBinding, afxReal ManualScaler, afxBool OnlyCopyIfGreater)
{
    akxMotion v3 = AnimationBinding->ID.Animation->motSlots[AnimationBinding->ID.motIdx].mot;
    
    if (v3->pivotLodError)
    {
        for (afxNat i = 0; i < AnimationBinding->TrackBindingCount; i++)
        {
            akxBoundMotionTransform *v6 = AnimationBinding->TrackBindings;
            afxNat SourceTrackIndex = v6[i].srcTrackIdx;
            
            if (SourceTrackIndex != AFX_INVALID_INDEX)
            {
                afxReal LODError = ManualScaler * v6[i].lodErr;
                afxReal* pivotLodError = v3->pivotLodError;

                if (OnlyCopyIfGreater)
                {
                    afxBool v11 = LODError < pivotLodError[SourceTrackIndex];
                    afxBool v12 = LODError == pivotLodError[SourceTrackIndex];
                    
                    if (!v11 && !v12)
                        pivotLodError[SourceTrackIndex] = LODError;
                }
                else
                {
                    pivotLodError[SourceTrackIndex] = LODError;
                }
            }
        }
    }
}

